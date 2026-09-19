#ifndef MINIC_LEXER_INCREMENTAL_REFERENCE_H
#define MINIC_LEXER_INCREMENTAL_REFERENCE_H

/*
 * Declaración de referencia: adapten estas operaciones a lexer/lexer.h y a los
 * tipos que ya existen en su proyecto. No mantengan dos interfaces duplicadas.
 */

#include <stdio.h>

#include "lexer/token.h"

typedef struct Lexer Lexer;

typedef enum {
    LEXER_STATUS_OK,
    LEXER_STATUS_IO_ERROR,
    LEXER_STATUS_MEMORY_ERROR
} LexerStatus;

int lexer_init(Lexer *lexer, FILE *source);
LexerStatus lexer_next_token(Lexer *lexer, Token *out);
void lexer_destroy(Lexer *lexer);

#endif

