#!/usr/bin/env python3
"""Conservative source recovery for one SHA-256-locked local PE32.

Only complete, whitelisted, padded scalar leaves are emitted. Unsupported code
is NOT converted into stubs. This is not a whole-game decompiler or registry
importer. All proprietary input stays local. Standard library + GNU objdump.
"""
from __future__ import annotations
import argparse
import collections
import ctypes
import hashlib
import json
import pathlib
import re
import struct
import subprocess
import tempfile

SHA256 = '323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815'
SIZE = 19977216
ROOT = pathlib.Path(__file__).resolve().parents[1]
PATTERNS = [
    ('constant', rb'\xb8(.{4})\xc3'),
    ('zero', rb'(?:\x33\xc0|\x31\xc0)\xc3'),
    ('identity', rb'\x8b\xc1\xc3'),
]
for family, opcode in [('load32', b'\x8b'), ('load8', b'\x0f\xb6'),
                       ('load16', b'\x0f\xb7'), ('sign8', b'\x0f\xbe'),
                       ('sign16', b'\x0f\xbf')]:
    for suffix, modrm, operand in [('0', b'\x01', b''), ('8', b'\x41', b'(.)'),
                                   ('32', b'\x81', b'(.{4})')]:
        PATTERNS.append((family + '_' + suffix, re.escape(opcode + modrm) + operand + b'\xc3'))
PATTERNS += [('address_8', rb'\x8d\x41(.)\xc3'), ('address_32', rb'\x8d\x81(.{4})\xc3')]
PATTERNS = [(name, re.compile(pattern, re.S)) for name, pattern in PATTERNS]


def read_target(path: pathlib.Path):
    data = path.read_bytes()
    if len(data) != SIZE or hashlib.sha256(data).hexdigest() != SHA256:
        raise ValueError('Rejected input: size/SHA-256 is not the authorized attached EXE')
    pe = struct.unpack_from('<I', data, 0x3C)[0]
    if data[:2] != b'MZ' or data[pe:pe+4] != b'PE\0\0':
        raise ValueError('Invalid PE headers')
    opt = pe + 24
    if struct.unpack_from('<H', data, pe+4)[0] != 0x14C or struct.unpack_from('<H', data, opt)[0] != 0x10B:
        raise ValueError('Expected I386 PE32')
    base = struct.unpack_from('<I', data, opt+28)[0]
    section = opt + struct.unpack_from('<H', data, pe+20)[0]
    for i in range(struct.unpack_from('<H', data, pe+6)[0]):
        off = section + 40*i
        if data[off:off+8].rstrip(b'\0') == b'.text':
            size, rva, raw_size, raw = struct.unpack_from('<IIII', data, off+8)
            if base != 0x400000 or raw+raw_size > len(data) or size > raw_size:
                raise ValueError('Unexpected image mapping')
            return data, base+rva, size, raw
    raise ValueError('No .text section')


def match_body(data: bytes, offset: int):
    for name, pattern in PATTERNS:
        match = pattern.match(data[offset:offset+16])
        if not match or data[offset+match.end():offset+match.end()+2] != b'\xcc\xcc':
            continue
        family, _, encoding = name.partition('_')
        displacement = 0
        if encoding in ('8', '32'):
            displacement = int.from_bytes(match.group(1), 'little', signed=True)
        # Keep the initial delivery's object-window contract small and explicit.
        if not 0 <= displacement <= 0x1000000:
            continue
        immediate = int.from_bytes(match.group(1), 'little') if family == 'constant' else 0
        variant = (1 if data[offset] == 0x31 else 0) if family == 'zero' else int(encoding or 0)
        width = {'load8': 1, 'sign8': 1, 'load16': 2, 'sign16': 2, 'load32': 4}.get(family, 0)
        return dict(family=family, encoding=variant, displacement=displacement,
                    immediate=immediate, size=match.end(), width=width)
    return None


class LLVMDecoder:
    def __init__(self, library: str):
        self.lib = ctypes.CDLL(library)
        for name in ['LLVMInitializeX86TargetInfo', 'LLVMInitializeX86Target',
                     'LLVMInitializeX86TargetMC', 'LLVMInitializeX86AsmPrinter',
                     'LLVMInitializeX86Disassembler']:
            getattr(self.lib, name)()
        self.lib.LLVMCreateDisasm.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p]
        self.lib.LLVMCreateDisasm.restype = ctypes.c_void_p
        self.context = self.lib.LLVMCreateDisasm(b'i386-pc-windows-msvc', None, 0, None, None)
        if not self.context:
            raise RuntimeError('LLVM could not create an I386 disassembler')
        self.lib.LLVMSetDisasmOptions.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
        self.lib.LLVMSetDisasmOptions(self.context, 6)
        self.lib.LLVMDisasmInstruction.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_char_p, ctypes.c_size_t]
        self.lib.LLVMDisasmInstruction.restype = ctypes.c_size_t
        self.lib.LLVMDisasmDispose.argtypes = [ctypes.c_void_p]

    def decode(self, code: bytes, va: int):
        result = []
        offset = 0
        while offset < len(code):
            source = ctypes.create_string_buffer(code[offset:])
            text = ctypes.create_string_buffer(256)
            size = self.lib.LLVMDisasmInstruction(self.context, source, len(code)-offset, va+offset, text, len(text))
            if not size:
                raise ValueError(f'Independent I386 decoder rejected {va+offset:08X}')
            result.append({'va': va+offset, 'size': size,
                           'assembly': ' '.join(text.value.decode('ascii').split())})
            offset += size
        if not result[-1]['assembly'].startswith('ret') or len(result) != 2:
            raise ValueError(f'Unexpected leaf instruction boundary at {va:08X}')
        return result

    def close(self):
        self.lib.LLVMDisasmDispose(self.context)


def discover(exe: pathlib.Path, objdump: str, decoder=None):
    data, low, text_size, raw = read_target(exe)
    # Early ordered work and the original BB90 pilot are excluded, not overwritten.
    provisional = {}
    for va in range(max(low, 0x410000), low+text_size, 16):
        if 0xBB9000 <= va < 0xBB9100:
            continue
        off = raw + va-low
        if data[off-2:off] != b'\xcc\xcc':
            continue
        body = match_body(data, off)
        if body:
            body.update(va=va, rva=va-0x400000, file_offset=off,
                        symbol=f'FUN_{va:08X}',
                        body_sha256=hashlib.sha256(data[off:off+body['size']]).hexdigest())
            provisional[va] = body
    calls = collections.defaultdict(list)
    decoded_roots = set()
    previous = None
    padding = 0
    instruction_rows = 0
    line_re = re.compile(r'^\s*([0-9a-f]+):\s+(.*)$')
    call_re = re.compile(r'^call\s+0x([0-9a-f]+)\s*$')
    with tempfile.TemporaryFile(mode='w+', encoding='utf-8') as stream:
        subprocess.run([objdump, '-d', '--no-show-raw-insn', '-Mintel', str(exe)], stdout=stream, check=True)
        stream.seek(0)
        for line in stream:
            match = line_re.match(line)
            if not match:
                continue
            va = int(match[1], 16)
            assembly = match[2].strip()
            if not low <= va < low+text_size:
                continue
            instruction_rows += 1
            if va in provisional and padding >= 2 and previous == va-1:
                decoded_roots.add(va)
            call = call_re.match(assembly)
            if call:
                target = int(call[1], 16)
                off = raw+va-low
                if (target in provisional and data[off] == 0xE8
                        and ((va+5+struct.unpack_from('<i', data, off+1)[0]) & 0xFFFFFFFF) == target):
                    calls[target].append(va)
            if assembly == 'int3':
                padding = padding+1 if previous == va-1 else 1
            else:
                padding = 0
            previous = va
    records = []
    for va in sorted(decoded_roots):
        record = provisional[va]
        record['direct_call_sites'] = calls.get(va, [])
        record['boundary_evidence'] = '16-byte aligned; two decoded preceding INT3; two trailing INT3; exact whole whitelist body'
        if decoder:
            record['llvm_i386'] = decoder.decode(data[record['file_offset']:record['file_offset']+record['size']], va)
        records.append(record)
    return records, {'gnu_linear_instruction_rows': instruction_rows,
                     'raw_pattern_hits': len(provisional), 'rejected_nondecoded_starts': len(provisional)-len(records)}


def expression(record):
    family, offset = record['family'], record['displacement']
    if family == 'constant':
        return f'0x{record["immediate"]:08X}U'
    if family == 'zero':
        return '0U'
    if family in ('identity', 'address'):
        return f'static_cast<u32>(reinterpret_cast<uptr>(self)) + {offset}U'
    return f'detail::{family}(self, {offset}U)'


def write_sources(records, output: pathlib.Path, scan):
    if output.exists() and any(output.iterdir()):
        raise ValueError('Output must be empty; existing work will not be overwritten')
    inc = output/'include'/'re5'
    src = output/'src'
    inc.mkdir(parents=True, exist_ok=True)
    src.mkdir(parents=True, exist_ok=True)
    header = '''// Generated from complete whitelisted leaves of the exact attached EXE.\n#pragma once\n#if defined(__i386__) || defined(_M_IX86)\n# if defined(_MSC_VER)\n#  define RE5_LEAF_CC __thiscall\n# else\n#  define RE5_LEAF_CC __attribute__((thiscall))\n# endif\n#else\n# define RE5_LEAF_CC\n#endif\nnamespace re5::leaf323d1aab {\nusing u8 = unsigned char;\nusing u16 = unsigned short;\nusing u32 = unsigned int;\n#if defined(_MSC_VER) && !defined(__clang__)\n# if defined(_WIN64)\nusing uptr = unsigned long long;\n# else\nusing uptr = unsigned int;\n# endif\n#else\nusing uptr = __UINTPTR_TYPE__;\n#endif\nstatic_assert(sizeof(u8)==1 && sizeof(u16)==2 && sizeof(u32)==4);\nstatic_assert(sizeof(uptr)==sizeof(void*));\nusing Function = u32 (RE5_LEAF_CC *)(const void*) noexcept;\n'''
    for row in records:
        header += f'u32 RE5_LEAF_CC {row["symbol"]}(const void* self) noexcept;\n'
    header += '} // namespace re5::leaf323d1aab\n'
    (inc/'leaves.hpp').write_text(header, encoding='utf-8')
    (inc/'detail.hpp').write_text('''#pragma once
#include "re5/leaves.hpp"
namespace re5::leaf323d1aab::detail {
// Unsigned byte access permits unaligned storage and does not assume a C++ class layout.
inline u32 load8(const void* self, u32 offset) noexcept {
    return static_cast<const u8*>(self)[offset];
}
inline u32 load16(const void* self, u32 offset) noexcept {
    const auto* p = static_cast<const u8*>(self) + offset;
    return u32(p[0]) | (u32(p[1]) << 8U);
}
inline u32 load32(const void* self, u32 offset) noexcept {
    const auto* p = static_cast<const u8*>(self) + offset;
    return u32(p[0]) | (u32(p[1]) << 8U) | (u32(p[2]) << 16U) | (u32(p[3]) << 24U);
}
inline u32 sign8(const void* self, u32 offset) noexcept {
    const u32 v = load8(self, offset);
    return (v & 0x80U) != 0U ? v | 0xFFFFFF00U : v;
}
inline u32 sign16(const void* self, u32 offset) noexcept {
    const u32 v = load16(self, offset);
    return (v & 0x8000U) != 0U ? v | 0xFFFF0000U : v;
}
} // namespace re5::leaf323d1aab::detail
''', encoding='utf-8')
    for number, start in enumerate(range(0, len(records), 256)):
        text = '#include "re5/detail.hpp"\nnamespace re5::leaf323d1aab {\n'
        for row in records[start:start+256]:
            text += f'// VA 0x{row["va"]:08X}; {row["size"]} original bytes; {row["family"]}.\n'
            text += f'u32 RE5_LEAF_CC {row["symbol"]}(const void* self) noexcept {{\n'
            if row['family'] in ('constant', 'zero'):
                text += '    (void)self;\n'
            text += f'    return {expression(row)};\n}}\n'
        text += '} // namespace re5::leaf323d1aab\n'
        (src/f'leaves_{number:03d}.cpp').write_text(text, encoding='utf-8')
    families = ['constant', 'zero', 'identity', 'address', 'load8', 'load16', 'load32', 'sign8', 'sign16']
    catalogue_header = '#pragma once\n#include "re5/leaves.hpp"\nnamespace re5::leaf323d1aab {\n'
    catalogue_header += 'enum class Kind : u8 { ' + ', '.join(families) + ' };\n'
    catalogue_header += 'struct Entry { u32 va; u32 file_offset; u32 displacement; u32 immediate; u8 size; u8 encoding; u8 width; Kind kind; Function function; };\n'
    catalogue_header += f'inline constexpr unsigned entry_count = {len(records)}U;\nextern const Entry entries[entry_count];\n}}\n'
    (inc/'catalog.hpp').write_text(catalogue_header, encoding='utf-8')
    text = '#include "re5/catalog.hpp"\nnamespace re5::leaf323d1aab {\nconst Entry entries[entry_count] = {\n'
    for row in records:
        text += f'    {{0x{row["va"]:08X}U, {row["file_offset"]}U, {row["displacement"]}U, 0x{row["immediate"]:08X}U, {row["size"]}, {row["encoding"]}, {row["width"]}, Kind::{row["family"]}, &{row["symbol"]}}},\n'
    text += '};\n}\n'
    (src/'catalog.cpp').write_text(text, encoding='utf-8')
    manifest = dict(schema=1, input_sha256=SHA256, input_size=SIZE,
                    recovered_leaf_ranges=len(records), recovered_instruction_bytes=sum(x['size'] for x in records),
                    families=dict(sorted(collections.Counter(x['family'] for x in records).items())),
                    scan=scan, canonical_registry_imported=False,
                    canonical_csv_membership_checked=False, canonical_denominator=79782,
                    denominator_changed=False, linked_game=False,
                    records=records)
    (output/'catalog.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exe', type=pathlib.Path, required=True)
    parser.add_argument('--out', type=pathlib.Path, required=True)
    parser.add_argument('--objdump', default='objdump')
    parser.add_argument('--llvm-library', help='Optional local libLLVM for independent I386 decoding')
    args = parser.parse_args()
    decoder = LLVMDecoder(args.llvm_library) if args.llvm_library else None
    try:
        records, scan = discover(args.exe, args.objdump, decoder)
        report = write_sources(records, args.out, scan)
        print(json.dumps({key: value for key, value in report.items() if key != 'records'}, indent=2))
    finally:
        if decoder:
            decoder.close()

if __name__ == '__main__':
    main()
