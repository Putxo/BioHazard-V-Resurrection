# Binary Inventory — Resident Evil 5 PC 1.1.0 Dev

This report contains metadata only. The executable used to generate it is not stored in this repository.

## Identity

- SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`
- File size: `19,977,216` bytes
- Format: PE32 / i386 (`Machine 0x014C`)
- PE timestamp: `1513562232` (2017-12-18 01:57:12 UTC)
- ImageBase: `0x00400000`
- Entry RVA: `0x00E570A9`
- Entry VA: `0x012570A9`
- PE checksum field: `0x0130D750`
- Section alignment: `0x1000`
- File alignment: `0x200`
- Subsystem: Windows GUI (`2`)
- COFF characteristics: `0x0103` (including relocations stripped / executable / 32-bit machine)
- DLL characteristics: `0x0000`

## Sections

| Section | RVA | VA | Virtual bytes | Raw offset | Raw bytes | Characteristics |
|---|---:|---:|---:|---:|---:|---:|
| `.text` | `0x00001000` | `0x00401000` | 16,185,403 | `0x00000400` | 16,185,856 | `0x60000020` |
| `.rdata` | `0x00F71000` | `0x01371000` | 2,053,450 | `0x00F6FE00` | 2,053,632 | `0x40000040` |
| `.data` | `0x01167000` | `0x01567000` | 2,083,528 | `0x01165400` | 977,920 | `0xC0000040` |
| `.rsrc` | `0x01364000` | `0x01764000` | 758,528 | `0x01254000` | 758,784 | `0x40000040` |

## Data directories

Present: Import, Resource, Debug, Load Config and IAT.

Absent in the PE directory table: Export, Exception, Security, Base Relocation, TLS, Bound Import, Delay Import and COM Descriptor.

## Imports

- Imported DLLs: **21**
- Imported symbols/ordinals: **376**
- Major imported surfaces include Win32 (`KERNEL32`, `USER32`), Direct3D 9 / D3DX9, DirectInput, XInput, DirectSound, Winsock, Steam API and GFWL/XLive.
- Per-DLL counts are stored in `database/imports_summary.csv`.

## Debug / toolchain evidence

The PE Debug Directory contains one CodeView RSDS record:

- GUID bytes: `08f48cb151773942b0a185d621aeaf94`
- Age: `23`
- PDB basename: `BH5DCRelease.pdb`
- Recorded build path ends in `ProjectOutput\ReleaseWin32\BH5DCRelease.pdb`.

The binary also contains Microsoft Visual C++ Runtime Library strings. These facts are evidence for later toolchain investigation; no exact compiler version is claimed yet.

## Important boundaries for later phases

- `.text` begins at VA `0x00401000` / RVA `0x00001000`.
- `.text` virtual end is VA `0x0137083B` / RVA `0x00F7083B` (exclusive, start + virtual size).
- Function count is intentionally still **UNKNOWN**. Phase 11 must discover real function boundaries before the README can display a denominator or percentage.
