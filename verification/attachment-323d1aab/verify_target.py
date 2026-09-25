#!/usr/bin/env python3
"""Read-only identity/range verification. Never runs or uploads the supplied EXE."""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import struct
import tempfile

ROOT = Path(__file__).resolve().parent
EXPECTED_SHA256 = "323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815"
EXPECTED_SIZE = 19977216


def manifest() -> dict:
    m = json.loads((ROOT / "target.json").read_text(encoding="utf-8"))
    if m["input_sha256"] != EXPECTED_SHA256 or m["input_size"] != EXPECTED_SIZE:
        raise ValueError("Manifest does not describe the exclusively authorized EXE")
    if m["new_canonical_functions"] != 0 or m["full_game_compiles"]:
        raise ValueError("This reference package cannot add new functions or claim a game build")
    if len(m["ranges"]) != 4 or sum(r["byte_count"] for r in m["ranges"]) != 142:
        raise ValueError("Unexpected reference ranges")
    for r in m["ranges"]:
        if r["va"] != m["image_base"] + r["rva"]:
            raise ValueError("VA/RVA mismatch")
        if r["end_exclusive_va"] - r["va"] != r["byte_count"]:
            raise ValueError("Range length mismatch")
    return m


def verify(path: Path) -> None:
    m = manifest()
    with path.open("rb") as f:
        data = f.read(EXPECTED_SIZE + 1)
    if len(data) != EXPECTED_SIZE or hashlib.sha256(data).hexdigest() != EXPECTED_SHA256:
        raise ValueError("Wrong input: SHA-256/size do not match the exclusively authorized EXE")
    pe = struct.unpack_from("<I", data, 0x3c)[0]
    if data[:2] != b"MZ" or data[pe:pe+4] != b"PE\0\0":
        raise ValueError("Invalid PE signature")
    machine, count = struct.unpack_from("<HH", data, pe+4)
    opt = pe + 24
    if machine != 0x14c or struct.unpack_from("<H", data, opt)[0] != 0x10b:
        raise ValueError("Not the expected x86 PE32")
    base = struct.unpack_from("<I", data, opt+28)[0]
    entry = struct.unpack_from("<I", data, opt+16)[0]
    if base != m["image_base"] or entry != m["entry_rva"]:
        raise ValueError("Image base/entry mismatch")
    section_table = opt + struct.unpack_from("<H", data, pe+20)[0]
    sections = [struct.unpack_from("<IIII", data, section_table + 40*i + 8)
                for i in range(count)]

    def offset(rva: int, length: int) -> int:
        for virtual_size, start, raw_size, raw_offset in sections:
            if start <= rva and rva + length <= start + raw_size:
                return raw_offset + rva - start
        raise ValueError("RVA does not have a complete file-backed range")

    for r in m["ranges"]:
        off = offset(r["rva"], r["byte_count"])
        if off != r["file_offset"]:
            raise ValueError("Range file offset mismatch: " + r["symbol"])
        actual = hashlib.sha256(data[off:off+r["byte_count"]]).hexdigest()
        if actual != r["sha256"]:
            raise ValueError("Instruction hash mismatch: " + r["symbol"])
    if data[offset(m["fallback_va"]-base, 1)] != m["fallback_first_byte"]:
        raise ValueError("Fallback byte mismatch")
    print("PASS exact attached EXE and all four instruction ranges")


def self_test() -> None:
    manifest()
    with tempfile.TemporaryDirectory() as d:
        p = Path(d) / "wrong.bin"
        p.write_bytes(b"MZnot the authorized executable")
        try:
            verify(p)
        except ValueError as e:
            if "Wrong input" not in str(e):
                raise
        else:
            raise AssertionError("Wrong input accepted")
        try:
            verify(Path(d) / "missing.exe")
        except FileNotFoundError:
            pass
        else:
            raise AssertionError("Missing input accepted")
    print("PASS locked manifest and wrong/missing input rejection")


def check_source() -> None:
    forbidden = {".exe", ".dll", ".obj", ".o", ".a", ".lib", ".so", ".pdb",
                 ".arc", ".tex", ".wav", ".ogg", ".mp4", ".wmv", ".bin", ".zip"}
    for p in ROOT.rglob("*"):
        rel = p.relative_to(ROOT)
        if any(part.startswith("build") or part in {".git", "__pycache__", "local", "out"}
               for part in rel.parts):
            continue
        if p.is_file():
            if p.suffix.lower() in forbidden:
                raise ValueError("Prohibited artifact: " + str(rel))
            with p.open("rb") as f:
                head = f.read(4)
            if head[:2] == b"MZ" or head in {b"\x7fELF", b"PK\x03\x04"}:
                raise ValueError("Binary signature: " + str(rel))
    print("PASS source-only package")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--exe", type=Path)
    group.add_argument("--self-test", action="store_true")
    group.add_argument("--check-source", action="store_true")
    args = parser.parse_args()
    if args.exe is not None:
        verify(args.exe)
    elif args.self_test:
        self_test()
    else:
        check_source()


if __name__ == "__main__":
    main()
