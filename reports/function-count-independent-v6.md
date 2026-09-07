# Independent function-candidate cross-check — V6

Target SHA-256: `323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`

An independent full-file static analysis of the exact RE5DX9 1.1.0 Dev Release target produced a broader candidate set than the V5 fixed-point CFG baseline.

## Independent evidence counts

- direct `CALL` targets inside code: **19,195**
- targets appearing in aligned sequences of at least two code pointers: **40,912**
- 16-byte-aligned starts after at least two `INT3`/`NOP` padding bytes: **69,076**
- deduplicated union of those three evidence classes: **79,782 potential function starts**

The complete linear `.text` pass contained **4,837,889** instruction/data rows and retained **662** non-decodable bytes explicitly as data rather than manufacturing instructions.

## Dashboard policy

The public progress denominator is updated to **79,782 potential function starts** because this is the broader independently reconstructed candidate universe currently available for the exact target.

This number is **not** claimed to be an authoritative original C/C++ source-symbol count. The PE has no COFF function-symbol table, no exports that enumerate internal functions, and the matching PDB/MAP is unavailable. V5's stricter **79,016** fixed-point CFG set is retained as a high-confidence subset/cross-check rather than presented as the total candidate universe.

## Relationship to V5

- V5 fixed-point high-confidence entries: **79,016**
- V6 broader independent candidate union: **79,782**
- difference: **766** candidates (**0.97%** of the V6 total)

Future refinement must classify the 766-difference pool rather than silently discarding it or calling every item a proven source function.
