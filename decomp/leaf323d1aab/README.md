# 7.171 rutinas hoja: C++ recuperado del adjunto exacto

Este módulo incorpora el lote local pendiente a la compilación del repositorio.
**No es el juego completo y no suma 7.171 entradas al contador canónico.**
Sólo utiliza el EXE suministrado por el usuario, de 19.977.216 bytes y SHA-256
`323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815`.
El EXE, las DLL, los objetos compilados y los datos del juego permanecen fuera de GitHub.

## Código y alcance

`generated/src/` contiene 7.171 definiciones C++ reales en 29 unidades y una tabla
con sus direcciones. `generated/include/re5/` declara sus interfaces. Los cuerpos
C++ y cabeceras son **idénticos byte a byte al lote local anterior**, comprobados
mediante el resumen `783468f4a39ede06d931caf72e1e2c8bf0a051a17933039d74e594eaf5220947`.
No son stubs, pseudocódigo, código máquina embebido ni llamadas a un emulador.

| Operación | Cuerpos |
|---|---:|
| Constante de 32 bits | 6.358 |
| Lectura de campos de 8/16/32 bits | 598 |
| Retorno de cero | 154 |
| Dirección relativa al receptor | 60 |
| Retorno del receptor | 1 |
| Total | 7.171 |

Las lecturas incluyen extensión de signo cuando aparece en el original.
Se preservan accesos sin alineación y el resultado EAX. El contrato no introduce
comprobaciones de puntero nulo que el original no tenía. Se excluyen direcciones
inferiores a `0x00410000` y el piloto `0x00BB9000..0x00BB90FF`.
Los cuerpos originales ocupan 42.745 bytes; su cantidad no representa una
proporción equivalente de la lógica del motor o de la jugabilidad.

## Compilación y pruebas

La inclusión CMake del repositorio construye este módulo junto al trabajo anterior.
También se puede compilar independientemente:

```sh
cmake -S decomp/leaf323d1aab -B build-leaves -DCMAKE_BUILD_TYPE=Release
cmake --build build-leaves --parallel 2
ctest --test-dir build-leaves --output-on-failure
```

Las pruebas ordinarias no requieren el EXE: propiedades de todas las funciones,
lecturas de byte/palabra exhaustivas, integridad, rechazo de entradas incorrectas,
analizador de pertenencia mediante fixtures sintéticos, reproducción de los
fuentes y, en POSIX, páginas inaccesibles y datos de sólo lectura para las 598
rutinas de carga. No se ejecuta código original en CI.

`reports/publication-validation.json` distingue los resultados locales de los
estados de GitHub Actions. La comparación local con fragmentos originales se
repitió: 4.967.895 comprobaciones y 598 rutinas de lectura junto a una página de
guarda. Esos fragmentos se ejecutan en modo x86-64 compatible con datos low32,
**no en un proceso IA32 ni como juego**. La comparación Windows I386 COFF también
se repitió con Clang 17: 7.171 cuerpos compilados, 7.013 idénticos en sus bytes
originales (42.259 bytes, sin relleno). Eso no promociona el nivel `MATCHED` del
registro ni acredita el enlazado del juego.

## Reproducción y procedencia

`metadata/semantic.b85` contiene únicamente descriptores semánticos comprimidos:
delta de dirección, identificador de operación, variante de codificación y
constante/desplazamiento. **No contiene un ejecutable ni una copia de .text.**
`metadata/calls.b85` conserva los sitios CALL observados. Ambos tienen huellas
fijas comprobadas por `tools/restore_sources.py`. Los descriptores se expanden
al C++ legible versionado, y CI detecta cualquier divergencia.

```sh
python decomp/leaf323d1aab/tools/restore_sources.py --check
python decomp/leaf323d1aab/tools/restore_sources.py --check --exe "/ruta/al/adjunto.exe"
python decomp/leaf323d1aab/tools/verify_local.py --exe "/ruta/al/adjunto.exe" --build build-leaves
python decomp/leaf323d1aab/tools/compare_coff.py --exe "/ruta/al/adjunto.exe" --build build-coff --report informe-local.json
```

La última herramienta emite objetos sólo fuera del árbol fuente. Ninguna sube
el EXE ni inicia el juego. El catálogo normalizado conserva VA, RVA, desplazamiento
de archivo, longitud, operación, operandos, hash del cuerpo y llamadas directas.
La huella del catálogo de descubrimiento original (con desensamblado LLVM I386)
se conserva en `metadata/provenance.json`; no se simula una nueva verificación
LLVM en la normalización. El generador local completo sigue disponible en
`tools/recover_leaves.py`, con barrido GNU y comprobación LLVM opcional.

## Registro canónico y trabajo restante

Este módulo utiliza el espacio de nombres independiente `re5::leaf323d1aab`.
No reemplaza implementaciones anteriores ni cambia `functions.csv`, `claims.csv`,
los niveles de calidad o el denominador **79.782**. La pertenencia literal al
`function_candidates.csv` original sigue pendiente: el archivo no estaba presente
en los materiales locales. No se sustituye por un conjunto nuevo que simplemente
produzca la misma cifra. La equivalencia de un cuerpo y su pertenencia a ese
universo son comprobaciones distintas.

`tools/audit_membership.py --candidates /ruta/function_candidates.csv --functions
/ruta/database/functions.csv` verifica la segunda columna decimal, exige 79.782
filas únicas y detecta coincidencias con el registro. No modifica nada.

Siguen pendientes las restantes funciones, las clases y datos globales, la
resolución de direcciones, dependencias y llamadas, el enlazado Windows y la
validación con el juego. Los retornos que contienen direcciones originales aún
no se han reubicado a un mapa completo nuevo. Compilar este módulo no equivale
a reconstruir el ejecutable completo.
