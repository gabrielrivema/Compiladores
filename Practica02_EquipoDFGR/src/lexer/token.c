#include "lexer/token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_EOF: return "TOKEN_EOF";
        case ERROR: return "ERROR";
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER: return "INTEGER";
        case INT: return "INT";
        case BOOL: return "BOOL";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case PRINT: return "PRINT";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case STAR: return "STAR";
        case SLASH: return "SLASH";
        case ASSIGN: return "ASSIGN";
        case EQUAL: return "EQUAL";
        case NOT_EQUAL: return "NOT_EQUAL";
        case LESS: return "LESS";
        case LESS_EQUAL: return "LESS_EQUAL";
        case GREATER: return "GREATER";
        case GREATER_EQUAL: return "GREATER_EQUAL";
        case AND: return "AND";
        case OR: return "OR";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case SEMICOLON: return "SEMICOLON";
    }

    return "UNKNOWN";
}

int token_init(Token *token, TokenType type, const char *lexeme,
               size_t line, size_t column) {
    size_t length;

    if (token == NULL || lexeme == NULL) {
        return 0;
    }

    length = strlen(lexeme);
    token->lexeme = malloc(length + 1);
    if (token->lexeme == NULL) {
        return 0;
    }

    memcpy(token->lexeme, lexeme, length + 1);
    token->type = type;
    token->line = line;
    token->column = column;
    return 1;
}

void token_print(const Token *token) {
    if (token == NULL) {
        return;
    }

    if (token->type == TOKEN_EOF) {
        printf("%zu:%zu %s\n",
               token->line,
               token->column,
               token_type_name(token->type));
        return;
    }

    printf("%zu:%zu %s %s\n",
           token->line,
           token->column,
           token_type_name(token->type),
           token->lexeme);
}

void token_destroy(Token *token) {
    if (token == NULL) {
        return;
    }

    free(token->lexeme);
    token->lexeme = NULL;
}
