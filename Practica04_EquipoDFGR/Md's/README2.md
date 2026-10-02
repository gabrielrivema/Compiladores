# Base inicial adaptable — Práctica 4

Esta carpeta contiene un punto de partida para incorporar el módulo del AST al
proyecto de la Práctica 3. No es un proyecto completo y no compila de manera
independiente.

## Antes de copiar

1. Respalden su versión funcional de P03.
2. Lean `../CONTRATO_PARSER_AST.md`.
3. Comparen los tipos y nombres propuestos con su propia interfaz.
4. Adapten los archivos; no sustituyan módulos funcionales sin entender el cambio.

## Contenido

- `include/ast/ast.h`: tipos, datos e interfaces de referencia.
- `src/ast/ast.c`: esqueleto deliberadamente incompleto de las operaciones.

Los `TODO` señalan trabajo que corresponde al equipo. La base no implementa el
crecimiento de listas, los constructores, la impresión ni la liberación del AST.

## Integración mínima sugerida

1. Incorporen `include/ast/` y `src/ast/` a su proyecto.
2. Adapten los tipos propuestos a sus convenciones.
3. Agreguen `src/ast/ast.c` a su proceso de compilación.
4. Completen y prueben las listas dinámicas.
5. Implementen los constructores de expresiones.
6. Modifiquen el parser por niveles, comenzando con `primary`.
7. Agreguen constructores de sentencias, bloques y programa.
8. Implementen la impresión y destrucción recursiva.

Los nombres exactos pueden cambiar. El comportamiento observable y las reglas de
propiedad deben coincidir con el enunciado publicado.

