# Changelog

Bitácora de los cambios que se fueron haciendo al analizador léxico de MiniC.

## [Práctica 3] - 2026-09-21

### Añadido
- **Implementación del Analizador Sintáctico (`parser.c` / `parser.h`):**
  - Se creó la estructura `Parser`, compuesta por un puntero al `Lexer`, el token actual (`current`), el token previo (`previus`), y banderas de control de errores (`had_error` y `panic_mode`).
  - **Funciones principales y de gestión de anticipación (*lookahead*):**
    - `int parser_init(Parser *parser, Lexer *lexer);`: Inicializa el estado del parser, configura las banderas de error y solicita el primer token de anticipación.
    - `void parser_destroy(Parser *parser);`: Libera adecuadamente los recursos y tokens asociados.
    - `int parser_advance(Parser *parser);`: Avanza al siguiente token utilizando la interfaz incremental del lexer, manejando de forma transparente y desacoplada la filtración de errores léxicos.
    - `int parser_check(const Parser *parser, TokenType type);` y `int parser_match(Parser *parser, TokenType type);`: Permiten consultar y consumir condicionalmente los tokens del flujo.
    - `void consume(Parser *parser, TokenType expected, const char *message);`: Verifica y consume el token esperado o dispara un error sintáctico en caso contrario.
  - **Manejo de Errores y Recuperación:**
    - `void syntax_error(Parser *parser, const Token *token, const char *message);`: Reporta errores sintácticos con formato estricto a `stderr` incluyendo línea y columna, evitando cascadas de errores mediante el uso de `panic_mode`.
    - `void synchronize(Parser *parser);`: Implementa la estrategia de recuperación en modo pánico (*panic mode*) buscando puntos y coma o palabras reservadas de sincronización para reanudar el análisis de forma segura.
  - **Funciones de la Gramática (Descenso Recursivo):**
    - `int parse_program(Parser *parser);`: Valida la secuencia completa de sentencias hasta encontrar el final del archivo (`TOKEN_EOF`).
    - **Sentencias y Estructuras de Control:** `parse_statement`, `parse_declaration`, `parse_assigment`, `parse_print`, `parse_if`, `parse_while` y `parse_block`.
    - **Expresiones y Precedencia de Operadores:** Se implementó la jerarquía completa respetando asociatividad y precedencia (de menor a mayor prioridad): `parse_expression` (`parse_or`), `parse_and`, `parse_equal`, `parse_comparison`, `parse_add`, `parse_multiplicative`, `parse_unary` y `parse_primary`.

### Modificado
- **Interfaz del Analizador Léxico (`lexer.h` / `lexer.c`):**
  - Se adaptó el lexer de un análisis masivo a una interfaz incremental bajo demanda mediante `LexerStatus lexer_next_token(Lexer *lexer, Token *out);`, permitiendo que el parser solicite tokens uno a uno sin imprimirlos en la salida estándar.
  - Se ajustó el valor de retorno de `lexer_init()` para que devuelva `1` en caso de éxito y `0` ante fallos, esto tras hacer las pruebas proporcionadas y ver que no se daba la salida esperada se ajusto para el estandar del proyecto.
- **Controlador Principal (`main.c`):**
  - Se actualizó el flujo de ejecución para inicializar el `Lexer` y el `Parser`, gestionar el análisis sintáctico global y emitir los códigos de salida correspondientes (`0` para éxito con el mensaje `"Programa sintacticamente correcto."`, `1` para errores léxicos/sintácticos y `2` para errores de uso o apertura de archivos).

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
