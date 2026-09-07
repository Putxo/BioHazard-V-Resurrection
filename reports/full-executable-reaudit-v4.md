# Full Executable Re-audit V4 — RE5DX9 1.1.0 Dev Release

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

## Scope

This audit re-analyzes the complete PE, every byte of the executable `.text` section, every instruction emitted by two independent x86 disassemblers, all direct control-flow targets, compiler-padding boundaries, address-taken code pointers, MSVC RTTI/vtables, imports, printable strings and all 64-KiB `.text` regions.

Decoders:

- GNU Binutils `objdump` 2.44
- LLVM `llvm-objdump` 17.0.0

No sample-only analysis is used.

## Target identity

- PE32 / Intel i386
- File size: **19,977,216 bytes**
- ImageBase: `0x00400000`
- Entry VA: `0x012570A9`
- COFF symbols: **0**
- Debug reference: `BH5DCRelease.pdb`
- PDB GUID: `b18cf408-7751-4239-b0a1-85d621aeaf94`
- PDB age: 23
- Matching PDB/MAP: unavailable

## PE sections

- `.text` VA `0x00401000`–`0x0137083B`: 16,185,403 virtual bytes / 16,185,856 raw bytes
- `.rdata` VA `0x01371000`–`0x0156654A`: 2,053,450 virtual bytes / 2,053,632 raw bytes
- `.data` VA `0x01567000`–`0x017638C8`: 2,083,528 virtual bytes / 977,920 raw bytes
- `.rsrc` VA `0x01764000`–`0x0181D300`: 758,528 virtual bytes / 758,784 raw bytes

## Complete instruction pass

Instruction starts:

- GNU: **4,836,558**
- LLVM: **4,836,307**
- common to both: **4,835,646**
- GNU-only: **912**
- LLVM-only: **661**

GNU control flow:

- CALL: 244,978 — 196,792 direct / 48,186 indirect
- JMP: 53,450 — 50,138 direct / 3,312 indirect
- conditional Jcc: 256,138
- RET: 107,008
- INT3: 618,049

LLVM control flow:

- CALL: 244,922 — 196,736 direct / 48,186 indirect
- JMP: 53,351 — 50,043 direct / 3,308 indirect
- conditional Jcc: 256,157
- RET: 107,010
- INT3: 618,033

Common branch targets inside `.text`:

- direct CALL targets: **19,061**
- direct JMP targets: **35,878**
- conditional-branch targets: **187,062**

Compiler-padding starts:

- common starts after any `INT3` run: **80,494**
- common 16-byte-aligned starts after `INT3`: **72,919**

## Function-entry audit

V4 rejects the previous claim that 79,391 was an exact verified total.

### Dual-decoder base

The conservative base set contains:

- common 16-byte-aligned post-`INT3` starts;
- common direct CALL targets;
- PE entrypoint.

Deduplicated base: **74,752 machine-code entries**.

### RTTI / vtables

Recounted directly from the executable:

- MSVC TypeDescriptors: **433**
- CompleteObjectLocators: **494**
- structural vtables: **494**
- virtual slots: **3,534**
- unique virtual targets: **1,682**
- structural vtable targets outside base: **108**

Two vtable slots are in the tiny GNU/LLVM instruction-boundary disagreement set. Structural RTTI evidence preserves these entries rather than dropping the vtables.

### Extended high-confidence working set

For non-base address-taken/control-flow entries, V4 requires independent function-boundary evidence. The principal rule requires an instruction start after a RET boundary in both decoders and excludes conditional-branch labels. Exact structural vtable targets are retained independently.

Sequential additions outside the base:

- address-taken table entries with dual-RET boundary: **4,344**
- immediate callback/function-address entries with dual-RET boundary: **323**
- direct JMP/tail entries with dual-RET boundary: **68**
- structural vtable entries still outside the resulting union: **6**

Final V4 high-confidence working denominator: **79,493**.

## Interpretation

`79,493` is not presented as the exact number of original source functions.

The stripped PE lacks the authoritative information needed to prove a source-symbol count:

- COFF symbol count is zero;
- no complete x86 function-boundary table is embedded;
- the matching `BH5DCRelease.pdb` / MAP is unavailable;
- static analysis must distinguish address-taken callbacks from switch/jump-table labels using evidence rather than original symbols.

Therefore the repository tracks both:

- **74,752 dual-decoder base entries**;
- **79,493 high-confidence working entries**;
- exact source-symbol count: **unavailable**.

## Whole-.text regional coverage

The complete `.text` span was partitioned into 64-KiB audit regions:

- regions: **247 / 247**
- regions with zero common decoded instructions: **0**
- regions with zero base function entries: **0**
- common instruction starts per region: **9,168–27,765**
- base entries per region: **3–3,945**

No address region was intentionally skipped.

## Imported modules

Total: **376 imports across 21 DLLs**.

- `d3dx9_43.dll` — 6
- `steam_api.dll` — 18
- `KERNEL32.dll` — 143
- `USER32.dll` — 63
- `ADVAPI32.dll` — 3
- `SHELL32.dll` — 4
- `WS2_32.dll` — 27
- `IPHLPAPI.DLL` — 2
- `WINMM.dll` — 3
- `d3d9.dll` — 1
- `XINPUT1_3.dll` — 3
- `DINPUT8.dll` — 1
- `DSOUND.dll` — 1
- `WMVCore.DLL` — 1
- `AVIFIL32.dll` — 9
- `MSVFW32.dll` — 2
- `xlive.dll` — 75
- `imagehlp.dll` — 7
- `GDI32.dll` — 1
- `ole32.dll` — 4
- `OLEAUT32.dll` — 2

## Printable-string scan

Length >= 4, raw section scan:

- `.text`: 104,381 ASCII / 3,191 UTF-16LE
- `.rdata`: 42,279 ASCII / 171 UTF-16LE
- `.data`: 1,817 ASCII / 5 UTF-16LE
- `.rsrc`: 9,713 ASCII / 36 UTF-16LE

String counts are descriptive only and are not function-entry evidence.

## Reproducibility hashes

- target EXE: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- GNU no-raw disassembly: `c4b73d106c2f6cc0104a2265bab697e15e6f2ab57cb08ce7b0aa0bad2f73eca4`
- LLVM no-raw disassembly: `328fdcf5dc4b7de15f0c39b4523fc07b508724d09f6ea037b5e818ce8f081f4e`
- common instruction-address map: `bb9f844ed1eb5c9a47ad1151c17388775fd7b25474695dc104acdaa2da29f91e`
- V4 high-confidence entry-address set: `1e4f3603444e46abec503d63e3dc1ac9cb4684170aed448edfe3006c48cf5bd1`

The large disassembly dumps themselves remain local and are not committed.

## Policy after V4

Use **79,493** as the current high-confidence decompilation working denominator and preserve **74,752** as the dual-decoder base set. Do not describe either value as a symbol-perfect source-function count. Any future change requires another evidence-backed whole-executable audit and CI PASS.
