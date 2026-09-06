# GATE 04 — Build System

Phase: 4 — Build System

## Requirements

- [x] CMake configures a C/C++ project.
- [x] C++20 is the primary source standard; C11 remains available for procedural units.
- [x] `re5_recovered`, `re5_tests` and `metadata_tests` targets exist.
- [x] GCC/Clang warnings include `-Wall -Wextra -Wpedantic`; MSVC uses `/W4`.
- [x] The basic build does not require the proprietary executable.
- [x] Preset configure/build/test commands exist.

## Executed validation

The exact CMake/source contents committed in this phase were reproduced in a clean local validation directory and executed with:

`cmake --preset dev`
`cmake --build --preset dev`
`ctest --preset dev`

Result: 1/1 smoke test passed; configure and build succeeded with GNU C/C++ 14.2.0.

Result: PASS
