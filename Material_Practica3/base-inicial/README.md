# Base inicial adaptable — Práctica 3

Esta carpeta contiene un punto de partida para integrar el parser al proyecto de
la Práctica 2. No es un proyecto completo y no compila de manera independiente.

## Antes de copiar

1. Respalden su versión funcional de P02.
2. Lean `../CONTRATO_LEXER_PARSER.md`.
3. Comparen los tipos y nombres propuestos con su propia interfaz.
4. Adapten los archivos; no sustituyan módulos funcionales sin entender el cambio.

## Contenido

- `include/lexer/lexer_incremental.h`: declaración de referencia para adaptar el lexer.
- `include/parser/parser.h`: estado y operaciones iniciales del parser.
- `src/parser/parser.c`: esqueleto incompleto de esas operaciones.

Los `TODO` señalan trabajo que corresponde al equipo. La base no implementa la
gramática, los diagnósticos ni la recuperación.

## Integración mínima sugerida

1. Integren en su `lexer.h` una operación equivalente a `lexer_next_token`.
2. Hagan que esa operación produzca tokens sin imprimirlos.
3. Adapten `Parser` al tipo de lexer y al convenio de memoria del equipo.
4. Completen `parser_init`, `parser_advance`, `parser_check` y `parser_destroy`.
5. Agreguen los archivos del parser a su `Makefile`.
6. Sólo después comiencen a trasladar las producciones a funciones.

