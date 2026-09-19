#include "lexer/lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static int is_ignored_space(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static void advance_position(int c, size_t *line, size_t *column) {
    if (c == '\n' || c == '\r') {
        (*line)++;
        *column = 0;
    } else {
        (*column)++;
    }
}

// Revisa si el identificador es una palabra reservada o un booleano
static TokenType check_keyword(const char *str) {
    if (strcmp(str, "int") == 0) return INT;
    if (strcmp(str, "bool") == 0) return BOOL;
    if (strcmp(str, "if") == 0) return IF;
    if (strcmp(str, "else") == 0) return ELSE;
    if (strcmp(str, "while") == 0) return WHILE;
    if (strcmp(str, "print") == 0) return PRINT;
    if (strcmp(str, "true") == 0) return TRUE;
    if (strcmp(str, "false") == 0) return FALSE;
    return IDENTIFIER;
}

int simple;

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

        if(c == '\r') {
            int next = fgetc(file);
            if (next != '\n') {
                ungetc(next, file);
            } else {
                c = next;
            }
        }
        advance_position(c, &line, &column);

        if (is_ignored_space(c)) {
            continue;
        }

        // primero se revisa la diagonal porque también puede iniciar un comentario
        if (c == '/') {
            int next = fgetc(file);
            if (next == '/') {
                // Si hay otra es un comentario de una sola linea
                advance_position(next, &line, &column);
                while ((next = fgetc(file)) != EOF) {
                    if (next == '\r') {
                        int next2 = fgetc(file);
                        if (next2 == '\n') next = '\n';
                        else if (next2 != EOF) ungetc(next2, file);
                    }
                    advance_position(next, &line, &column);
                    if (next == '\n' || next == '\r') break;
                }
                continue;
            } else {
                if (next != EOF) ungetc(next, file);
                Token token;
                if (!token_init(&token, SLASH, "/", token_line, token_column)) {
                    fprintf(stderr, "Error: no se pudo reservar memoria.\n"); return 2;
                }
                token_print(&token);
                token_destroy(&token);
                continue;
            }
        }

        // 2. Identificadores y palabras reservadas
        if (isalpha(c) || c == '_') {
            size_t cap = 16;
            size_t len = 0;
            char *lexeme = malloc(cap);
            if (!lexeme) {
                fprintf(stderr, "Error: no se pudo reservar memoria.\n"); return 2;
            }
            lexeme[len++] = (char)c;

            int next;
            while ((next = fgetc(file)) != EOF && (isalnum(next) || next == '_')) {
                advance_position(next, &line, &column);
                if (len + 1 >= cap) {
                    cap *= 2;
                    char *temp = realloc(lexeme, cap);
                    if (!temp) { free(lexeme); return 2; }
                    lexeme = temp;
                }
                lexeme[len++] = (char)next;
            }
            if (next != EOF) ungetc(next, file);
            lexeme[len] = '\0';

            TokenType t_type = check_keyword(lexeme);
            Token token;
            if (!token_init(&token, t_type, lexeme, token_line, token_column)) {
                free(lexeme); return 2;
            }
            token_print(&token);
            token_destroy(&token);
            free(lexeme);
            continue;
        }

        // Números ent
        if (isdigit(c)) {
            size_t cap = 16;
            size_t len = 0;
            char *lexeme = malloc(cap);
            if (!lexeme) {
                fprintf(stderr, "Error: no se pudo reservar memoria.\n"); return 2;
            }
            lexeme[len++] = (char)c;

            int next;
            while ((next = fgetc(file)) != EOF && isdigit(next)) {
                advance_position(next, &line, &column);
                if (len + 1 >= cap) {
                    cap *= 2;
                    char *temp = realloc(lexeme, cap);
                    if (!temp) { free(lexeme); return 2; }
                    lexeme = temp;
                }
                lexeme[len++] = (char)next;
            }
            if (next != EOF) ungetc(next, file);
            lexeme[len] = '\0';

            Token token;
            if (!token_init(&token, INTEGER, lexeme, token_line, token_column)) {
                free(lexeme); return 2;
            }
            token_print(&token);
            token_destroy(&token);
            free(lexeme);
            continue;
        }

        // Operadores
        Token token;
        int next;
        int token_created = 1;

        switch(c) {
            case '=':
                next = fgetc(file);
                if (next == '=') {
                    advance_position(next, &line, &column);
                    token_created = token_init(&token, EQUAL, "==", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, file);
                    token_created = token_init(&token, ASSIGN, "=", token_line, token_column);
                }
                break;
            case '!':
                next = fgetc(file);
                if (next == '=') {
                    advance_position(next, &line, &column);
                    token_created = token_init(&token, NOT_EQUAL, "!=", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, file);
                    char lex[2] = {(char)c, '\0'};
                    token_created = token_init(&token, ERROR, lex, token_line, token_column);
                }
                break;
            case '<':
                next = fgetc(file);
                if (next == '=') {
                    advance_position(next, &line, &column);
                    token_created = token_init(&token, LESS_EQUAL, "<=", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, file);
                    token_created = token_init(&token, LESS, "<", token_line, token_column);
                }
                break;
            case '>':
                next = fgetc(file);
                if (next == '=') {
                    advance_position(next, &line, &column);
                    token_created = token_init(&token, GREATER_EQUAL, ">=", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, file);
                    token_created = token_init(&token, GREATER, ">", token_line, token_column);
                }
                break;
            case '&':
                next = fgetc(file);
                if (next == '&') {
                    advance_position(next, &line, &column);
                    token_created = token_init(&token, AND, "&&", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, file);
                    char lex[2] = {(char)c, '\0'};
                    token_created = token_init(&token, ERROR, lex, token_line, token_column);
                }
                break;
            case '|':
                next = fgetc(file);
                if (next == '|') {
                    advance_position(next, &line, &column);
                    token_created = token_init(&token, OR, "||", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, file);
                    char lex[2] = {(char)c, '\0'};
                    token_created = token_init(&token, ERROR, lex, token_line, token_column);
                }
                break;
            case '+': token_created = token_init(&token, PLUS, "+", token_line, token_column); break;
            case '-': token_created = token_init(&token, MINUS, "-", token_line, token_column); break;
            case '*': token_created = token_init(&token, STAR, "*", token_line, token_column); break;
            case '(': token_created = token_init(&token, LPAREN, "(", token_line, token_column); break;
            case ')': token_created = token_init(&token, RPAREN, ")", token_line, token_column); break;
            case '{': token_created = token_init(&token, LBRACE, "{", token_line, token_column); break;
            case '}': token_created = token_init(&token, RBRACE, "}", token_line, token_column); break;
            case ';': token_created = token_init(&token, SEMICOLON, ";", token_line, token_column); break;
            default: {
                char lex[2] = {(char)c, '\0'};
                token_created = token_init(&token, ERROR, lex, token_line, token_column);
                break;
            }
        }

        if (!token_created) {
            fprintf(stderr, "Error: no se pudo reservar memoria.\n");
            return 2;
        }

        token_print(&token);
        token_destroy(&token);
    }

    if (ferror(file)) {
        fprintf(stderr, "Error: no se pudo leer el archivo.\n");
        return 2;
    }

    Token token_eof;
    if (!token_init(&token_eof, TOKEN_EOF, "", line, column)) {
        fprintf(stderr, "Error: no se pudo reservar memoria.\n");
        return 2;
    }

    token_print(&token_eof);
    token_destroy(&token_eof);
    return 0;
}
