# FUN_004086A0 resumption verification

Resumed exclusively from `decomp/004086a0-next` at original commit `5bef52167f35b0a712237a667b42c0f1c1bfc4e5`.

- Integration snapshot: `3eadb787bb165b706cee86eef0809e48563f8109`.
- Public main snapshot at preflight: `184a9d7ea3a858e1fdf364de8a2d1f6ddbc71737`.
- Existing source, header, unit tests and original extraction evidence retained byte-for-byte.
- Shared registries and build wiring reconstructed from current integration plus only the target delta.
- All pre-existing registry/history rows preserved byte-for-byte.
- Canonical denominator remains **79,782**; inventory freeze unchanged.
- Full CMake dev build and CTest suite: PASS.
- Canonical report generation and metadata validator: PASS.
- Runner log: https://github.com/Putxo/BioHazard-V-Resurrection/actions/runs/36107778664

Separately, the original three C++ file blobs were verified against Git blob hashes and the isolated original unit test passed with both GCC and Clang, with AddressSanitizer and UndefinedBehaviorSanitizer enabled. The documented 18-byte body hash and objdump instruction stream were checked again. The private full EXE is not available in this resumption environment: its original extraction evidence is reused, not represented as a fresh full-EXE extraction.

`fast_pass_validated` remains false until this reconstruction PR actually completes all five required CI checks. Native code has no conditional branch; the null-this early return is explicitly portable-only.

No EXE, DLL, proprietary asset, object file or build product is staged.
