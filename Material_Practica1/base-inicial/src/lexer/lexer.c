#include "lexer/lexer.h"

#include <stdio.h>

int simple_token_type(int c, TokenType *type) {
    if (type == NULL) {
        return 0;
    }

    switch (c) {
        case '+':
            *type = PLUS;
            return 1;

        /* TODO: agregar los demás símbolos simples de la práctica. */

        default:
            return 0;
    }
}

static int is_ignored_space(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static void advance_position(int c, size_t *line, size_t *column) {
    if (c == '\n') {
        (*line)++;
        *column = 0;
    } else {
        (*column)++;
    }

    /* TODO: adaptar esta lógica para \r aislado y para la secuencia \r\n. */
}

int lexer_scan(FILE *file) {
    size_t line = 1;
    size_t column = 0;
    int c;

    if (file == NULL) {
        return 2;
    }

    while ((c = fgetc(file)) != EOF) {
        size_t token_line = line;
        size_t token_column = column;
        TokenType type;

        advance_position(c, &line, &column);

        if (is_ignored_space(c)) {
            continue;
        }

        if (simple_token_type(c, &type)) {
            char lexeme[2] = {(char)c, '\0'};
            Token token;

            if (!token_init(&token, type, lexeme,
                            token_line, token_column)) {
                fprintf(stderr, "Error: no se pudo reservar memoria.\n");
                return 2;
            }

            token_print(&token);
            token_destroy(&token);
            continue;
        }

        /*
         * TODO: reconocer enteros.
         * TODO: reemplazar este diagnóstico por un token ERROR.
         */
        fprintf(stderr, "%zu:%zu PENDIENTE %c\n",
                token_line, token_column, c);
    }

    if (ferror(file)) {
        fprintf(stderr, "Error: no se pudo leer el archivo.\n");
        return 2;
    }

    /* TODO: generar TOKEN_EOF con la posición actual. */
    return 0;
}
