# Pruebas públicas — Práctica 2

## Uso

Estas pruebas comparan únicamente la salida estándar producida por `minic`.
Desde la raíz del proyecto del equipo puede ejecutarse:

```text
sh tests/public/run_public_tests.sh
```

También puede indicarse otra ruta para el ejecutable:

```text
sh tests/public/run_public_tests.sh ./ruta/al/minic
```

Cada archivo de `inputs/` debe producir exactamente el archivo del mismo nombre
ubicado en `expected/`, cambiando la extensión `.mc` por `.out`.

Ejemplo manual:

```text
./minic tests/public/inputs/p01_identifiers.mc > resultado.out
diff -u tests/public/expected/p01_identifiers.out resultado.out
```

Los diagnósticos operativos no deben mezclarse con la secuencia de tokens en
`stdout`.

## Casos

| Entrada | Propósito principal |
|---|---|
| `p01_identifiers.mc` | Identificadores válidos y máxima longitud. |
| `p02_reserved_words.mc` | Todas las palabras reservadas y booleanos. |
| `p03_case_sensitive.mc` | Distinción entre mayúsculas y minúsculas. |
| `p04_booleans.mc` | Booleanos dentro de declaraciones y asignaciones. |
| `p05_compound_operators.mc` | Todos los operadores compuestos. |
| `p06_incomplete_operators.mc` | `!`, `&` y `|` como errores independientes. |
| `p07_comments.mc` | Comentarios después de otros tokens y en varias líneas. |
| `p08_comment_at_eof.mc` | Comentario al final del archivo sin salto final. |
| `p09_slash_and_comments.mc` | Distinción entre división y comentario. |
| `p10_longest_match.mc` | Prioridad y máxima coincidencia. |
| `p11_error_recovery.mc` | Recuperación después de varios errores. |
| `p12_complete_lexer.mc` | Combinación de todas las categorías principales. |
| `p13_crlf.mc` | Comentarios y posiciones con terminadores CRLF. |

## Alcance

Estas pruebas son públicas y mínimas. No comprueban todas las combinaciones
posibles ni sustituyen las pruebas que debe incluir cada equipo. Durante la
evaluación podrán utilizarse casos adicionales que respeten la especificación
publicada.

