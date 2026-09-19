#ifndef MINIC_LEXER_H
#define MINIC_LEXER_H

#include "token.h"

#include <stdio.h>

int simple_token_type(int c, TokenType *type);
int lexer_scan(FILE *file);

#endif
