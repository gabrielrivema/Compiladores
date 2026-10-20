#ifndef MINIC_LEXER_H
#define MINIC_LEXER_H
#include <stdio.h>

#include "token.h"


typedef enum {
    LEXER_STATUS_OK,
    LEXER_STATUS_IO_ERROR,
    LEXER_STATUS_MEMORY_ERROR
} LexerStatus;

typedef struct Lexer{
    FILE *file;
    size_t line;
    size_t column;
} Lexer;


int lexer_scan(FILE *file);
int lexer_init(Lexer *lexer, FILE *source);
LexerStatus lexer_next_token(Lexer *lexer, Token *out);
void lexer_destroy(Lexer *lexer);

#endif
