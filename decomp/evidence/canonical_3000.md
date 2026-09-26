# Canonical 3000 milestone

This milestone registers **1,000 additional existing C++ leaf implementations** on top of the official 2,000-row registry. The selected range starts at `FUN_00736B20` and ends at `FUN_008D3470` and covers **6,001 original instruction bytes**.

Selection is restricted to address-named bodies already present in `decomp/leaf323d1aab`, absent from the 2,000-row registry, present in the reproduced frozen 79,782-address universe, and not listed among the published nonmatching Windows-I386 COFF definitions. The read-only CI recompiles all selected functions again and requires **1,000/1,000 exact body hashes** before publication.

This is registry progress, not a claim of original high-level source names/types or whole-game linkage. The denominator remains 79,782 and no EXE/assets/compiled outputs are committed.
