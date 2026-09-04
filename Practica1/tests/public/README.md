# Pruebas públicas — Práctica 1

## Uso

Cada archivo de `inputs/` debe ejecutarse con el programa `minic`. La salida estándar debe coincidir exactamente con el archivo del mismo nombre ubicado en `expected/`, cambiando la extensión `.mc` por `.out`.

Ejemplo:

```text
./minic tests/public/inputs/p5_integer_expression.mc
```

Los diagnósticos operativos no deben mezclarse con la secuencia de tokens en `stdout`.

## Casos

| Entrada | Propósito principal |
|---|---|
| `p1_empty.mc` | Archivo vacío y posición inicial de `TOKEN_EOF`. |
| `p2_whitespace.mc` | Espacios, tabulador y saltos CRLF. |
| `p3_single_symbols.mc` | Todos los símbolos simples. |
| `p4_integer.mc` | Entero de varios dígitos. |
| `p5_integer_expression.mc` | Enteros y símbolos en una línea. |
| `p6_multiple_lines.mc` | Seguimiento de varias líneas. |
| `p7_tabs_spaces.mc` | Columnas con tabuladores y espacios. |
| `p8_error_character.mc` | `ERROR` y recuperación. |
| `p9_negative_number.mc` | Separación entre `MINUS` e `INTEGER`. |
| `p10_mixed_basic.mc` | Combinación básica en varias líneas. |

## Alcance

Estas pruebas son públicas y mínimas. Cada equipo debe incluir sus propias pruebas. Durante la evaluación podrán utilizarse casos adicionales que respeten la especificación.
