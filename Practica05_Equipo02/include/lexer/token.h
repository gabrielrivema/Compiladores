#ifndef MINIC_TOKEN_H
#define MINIC_TOKEN_H

#include <stddef.h>

typedef enum {
    TOKEN_EOF,
    ERROR,
    IDENTIFIER,
    INTEGER,
    INT,
    BOOL,
    IF,
    ELSE,
    WHILE,
    PRINT,
    TRUE,
    FALSE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    AND,
    OR,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    SEMICOLON
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
    size_t line;
    size_t column;
} Token;

const char *token_type_name(TokenType type);
int token_init(Token *token, TokenType type, const char *lexeme,
               size_t line, size_t column);
void token_print(const Token *token);
void token_destroy(Token *token);

#endif
