# Changelog

Bitácora de los cambios que se fueron haciendo al analizador léxico de MiniC.

## [Práctica 2] - 2026-09-12

### Correcciones de la Práctica 1

- **Manejo de memoria:** Se cambió la forma de guardar los lexemas, ya que en la práctica anterior se usaban arreglos de tamaño fijo. Ahora se usa `malloc` y `realloc` para poder aumentar el espacio cuando sea necesario.
- Se corrigió el problema señalado en el feedback de la práctica anterior relacionado con el uso de memoria.

### Añadido

- **Identificadores y palabras reservadas (`lexer.c` / `token.h`):**
  - Se agregó el reconocimiento de identificadores. Pueden empezar con una letra o `_` y después tener letras, números o `_`.
  - Se agregó la función `check_keyword()` para revisar si un identificador es una palabra reservada como `int`, `bool`, `if`, `else`, `while` o `print`, o si es `true` o `false`.

- **Operadores compuestos:**
  - Se agregó el reconocimiento de `==`, `!=`, `<=`, `>=`, `&&` y `||`.
  - Se utiliza `ungetc` cuando se lee un carácter que no forma parte del operador, para poder procesarlo después.

- **Comentarios:**
  - Se agregó el manejo de comentarios de una línea (`//`) y comentarios multilínea (`/* ... */`).
  - Mientras se leen los comentarios se siguen actualizando las posiciones de línea y columna.

- **Manejo de errores:**
  - Se agregó el token `ERROR` para caracteres como `!`, `&` y `|` cuando aparecen solos.
  - Estos casos ya no detienen el análisis léxico y se generan como tokens de error.

### Modificado

- **`TokenType`:** Se agregaron nuevos tipos de token para identificadores, palabras reservadas, booleanos y operadores compuestos:
  `IDENTIFIER`, `INT`, `BOOL`, `IF`, `ELSE`, `WHILE`, `PRINT`, `TRUE`, `FALSE`, `EQUAL`, `NOT_EQUAL`, `LESS_EQUAL`, `GREATER_EQUAL`, `AND` y `OR`.

---

## [Práctica 1] - 2026-09-05

### Añadido

- **Tokens (`token.h` / `token.c`):**
  - Se definió el `enum TokenType`.
  - Se creó la estructura `Token`, que guarda el tipo, lexema, línea y columna.

- **Lexer (`lexer.h` / `lexer.c`):**
  - Se agregó el seguimiento de la posición dentro del archivo, empezando en línea 1 y columna 0.
  - Se agregó el manejo de espacios y saltos de línea (`\n`, `\r` y `\r\n`).
  - Se agregaron los operadores y símbolos simples: `+`, `-`, `*`, `/`, `=`, `<`, `>`, `(`, `)`, `{`, `}`, `;`.
  - Se agregó el reconocimiento de números enteros.
  - Se agregó `TOKEN_EOF` para indicar el final del archivo.

- **`main.c`:**
  - Se agregó el manejo de los argumentos de la línea de comandos.
  - Se agregó la lectura del archivo y la impresión de los tokens.

- **Infraestructura:**
  - Se creó un `Makefile` para facilitar la compilación y limpieza del proyecto.
  - Se agregaron las pruebas del equipo en `tests/equipo/`.
