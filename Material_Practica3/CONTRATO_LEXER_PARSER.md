# Contrato de referencia — Lexer y parser

Este documento concreta la interfaz incremental solicitada por la Práctica 3.
Es una referencia adaptable: no agrega requisitos ni sustituye el enunciado.

## Responsabilidades del lexer

La interfaz pública del lexer debe permitir:

1. inicializar su estado a partir del archivo fuente;
2. entregar exactamente el siguiente token en cada solicitud;
3. conservar entre llamadas el estado de lectura, anticipación y posición;
4. distinguir un token léxico `ERROR` de un fallo interno;
5. liberar los recursos que conserve el lexer.

Una interfaz posible es:

```c
typedef enum {
    LEXER_STATUS_OK,
    LEXER_STATUS_IO_ERROR,
    LEXER_STATUS_MEMORY_ERROR
} LexerStatus;

int lexer_init(Lexer *lexer, FILE *source);
LexerStatus lexer_next_token(Lexer *lexer, Token *out);
void lexer_destroy(Lexer *lexer);
```

Los nombres y valores de retorno pueden cambiar para ajustarse al proyecto del
equipo, siempre que esos comportamientos permanezcan distinguibles.

## Responsabilidades del parser

El parser debe:

- solicitar tokens sólo por medio de la interfaz pública del lexer;
- conservar al menos un token de anticipación;
- consumir o liberar cada token conforme al convenio documentado;
- tratar `ERROR` como un error léxico y no duplicarlo como error sintáctico;
- detectar los fallos internos del lexer;
- no volver a analizar directamente los caracteres del archivo.

## Salida y efectos secundarios

Solicitar un token no debe imprimirlo ni producir mensajes de depuración. En P03
los tokens son datos internos del parser.

```text
archivo -> lexer -> Token -> parser
```

## Propiedad de memoria

El equipo debe documentar quién posee cada token y su lexema. Para el esqueleto
incluido se propone este convenio:

- con `LEXER_STATUS_OK`, `lexer_next_token` entrega en `out` un token válido;
- el parser adquiere la propiedad del token y de su lexema;
- antes de reemplazar `current`, el parser llama a `token_destroy`;
- `parser_destroy` libera el último token conservado;
- el lexer sólo libera el estado que todavía le pertenezca.

Otro convenio es válido si está documentado y evita fugas, dobles liberaciones y
punteros colgantes.

## Dos clases de fallo

```text
LEXER_STATUS_OK y out->type == ERROR
```

representa un elemento inválido del programa fuente. El parser debe emitir un
único diagnóstico léxico, marcar el programa como inválido, consumirlo y continuar.

```text
LEXER_STATUS_IO_ERROR o LEXER_STATUS_MEMORY_ERROR
```

representa un fallo interno que impide obtener normalmente el siguiente token.

