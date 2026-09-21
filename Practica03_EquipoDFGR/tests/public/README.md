# Pruebas públicas — Práctica 3

## Uso

Desde la raíz del proyecto del equipo:

```text
sh tests/public/run_public_tests.sh
```

También puede indicarse otra ruta para el ejecutable:

```text
sh tests/public/run_public_tests.sh ./ruta/al/minic
```

## Qué comprueban

Para programas válidos, el ejecutor verifica:

- código de salida `0`;
- el mensaje de éxito exacto en `stdout`;
- ausencia de diagnósticos en `stderr`.

Para programas inválidos, verifica:

- código de salida distinto de cero;
- ausencia del mensaje de éxito y de otra salida en `stdout`;
- presencia de diagnósticos en `stderr`;
- forma general y posición para los casos sintácticos.

La redacción del elemento esperado y el número exacto de diagnósticos pueden
depender de la recuperación implementada. Por ello, salvo en el caso diseñado
explícitamente con dos errores independientes, estas pruebas no exigen un texto
completo ni una cantidad exacta de mensajes.

## Casos

| Entrada | Resultado | Propósito principal |
|---|---|---|
| `p01_empty.mc` | válido | Programa vacío y `TOKEN_EOF`. |
| `p02_declarations.mc` | válido | Declaraciones con y sin inicialización. |
| `p03_assignment_print.mc` | válido | Asignación y sentencia `print`. |
| `p04_expression_precedence.mc` | válido | Todos los niveles de expresiones y paréntesis. |
| `p05_if_else.mc` | válido | Condicionales con y sin `else`. |
| `p06_dangling_else.mc` | válido | Asociación del `else` con el `if` más cercano. |
| `p07_while_blocks.mc` | válido | Ciclos, bloques vacíos y anidamiento. |
| `p08_complete_program.mc` | válido | Integración de las construcciones principales. |
| `p09_missing_semicolon.mc` | inválido | Punto y coma ausente. |
| `p10_missing_parenthesis.mc` | inválido | Paréntesis derecho ausente. |
| `p11_missing_brace.mc` | inválido | Llave de cierre ausente al llegar a EOF. |
| `p12_incomplete_expression.mc` | inválido | Expresión sin operando. |
| `p13_operator_without_operand.mc` | inválido | Operador binario sin operando izquierdo. |
| `p14_unexpected_token.mc` | inválido | Token que no inicia una sentencia. |
| `p15_multiple_errors.mc` | inválido | Dos errores y recuperación entre sentencias. |
| `p16_lexical_error.mc` | inválido | `ERROR` léxico sin diagnóstico sintáctico duplicado. |
| `p17_trailing_content.mc` | inválido | Contenido inesperado tras una sentencia válida. |

## Alcance

Estas pruebas son públicas y mínimas. No cubren todas las combinaciones de la
gramática, la recuperación ni la administración de memoria. Cada equipo debe
diseñar e incluir pruebas propias. Durante la evaluación podrán usarse casos
adicionales que respeten los requisitos publicados.

