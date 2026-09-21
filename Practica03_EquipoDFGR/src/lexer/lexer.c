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

int lexer_init(Lexer *lexer, FILE *source){
    if(!lexer || !source) return 0;

    lexer->file = source;
    lexer->line = 1;
    lexer->column = 0;
    return 1;
}

void lexer_destroy(Lexer *lexer){
    if(lexer)
        lexer->file = NULL;
}

LexerStatus lexer_next_token(Lexer *lexer, Token *out){
    if(!lexer || !out || !lexer->file)
        return LEXER_STATUS_IO_ERROR;

    int c;

    while ((c = fgetc(lexer->file)) != EOF) {
        size_t token_line = lexer->line;
        size_t token_column = lexer->column;

        if(c == '\r') {
            int next = fgetc(lexer->file);
            if (next != '\n') {
                ungetc(next, lexer->file);
            } else {
                c = next;
            }
        }
        advance_position(c, &lexer->line, &lexer->column);

        if (is_ignored_space(c)) {
            continue;
        }

        // primero se revisa la diagonal porque también puede iniciar un comentario
        if (c == '/') {
            int next = fgetc(lexer->file);
            if (next == '/') {
                // Si hay otra es un comentario de una sola linea
                advance_position(next, &lexer->line, &lexer->column);
                while ((next = fgetc(lexer->file)) != EOF) {
                    if (next == '\r') {
                        int next2 = fgetc(lexer->file);
                        if (next2 == '\n') next = '\n';
                        else if (next2 != EOF) ungetc(next2, lexer->file);
                    }
                    advance_position(next, &lexer->line, &lexer->column);
                    if (next == '\n' || next == '\r') break;
                }
                continue;
            } else {
                if (next != EOF) ungetc(next, lexer->file);
                if (!token_init(out, SLASH, "/", token_line, token_column)) return LEXER_STATUS_MEMORY_ERROR;
                return LEXER_STATUS_OK;
            }
        }

        // 2. Identificadores y palabras reservadas
        if (isalpha(c) || c == '_') {
            size_t cap = 16;
            size_t len = 0;
            char *lexeme = malloc(cap);
            if (!lexeme) return LEXER_STATUS_MEMORY_ERROR;
            lexeme[len++] = (char)c;

            int next;
            while ((next = fgetc(lexer->file)) != EOF && (isalnum(next) || next == '_')) {
                advance_position(next, &lexer->line, &lexer->column);
                if (len + 1 >= cap) {
                    cap *= 2;
                    char *temp = realloc(lexeme, cap);
                    if (!temp) { 
                        free(lexeme); 
                        return LEXER_STATUS_MEMORY_ERROR; 
                    }
                    lexeme = temp;
                }
                lexeme[len++] = (char)next;
            }
            if (next != EOF) ungetc(next, lexer->file);
            lexeme[len] = '\0';

            TokenType t_type = check_keyword(lexeme);
            if (!token_init(out, t_type, lexeme, token_line, token_column)) {
                free(lexeme); 
                return LEXER_STATUS_MEMORY_ERROR;
            }
            return LEXER_STATUS_OK;
        }

        // Números ent
        if (isdigit(c)) {
            size_t cap = 16;
            size_t len = 0;
            char *lexeme = malloc(cap);
            if (!lexeme) return LEXER_STATUS_MEMORY_ERROR;
            lexeme[len++] = (char)c;

            int next;
            while ((next = fgetc(lexer->file)) != EOF && isdigit(next)) {
                advance_position(next, &lexer->line, &lexer->column);
                if (len + 1 >= cap) {
                    cap *= 2;
                    char *temp = realloc(lexeme, cap);
                    if (!temp) { 
                        free(lexeme); 
                        return LEXER_STATUS_MEMORY_ERROR; 
                    }
                    lexeme = temp;
                }
                lexeme[len++] = (char)next;
            }
            if (next != EOF) ungetc(next, lexer->file);
            lexeme[len] = '\0';

            if (!token_init(out, INTEGER, lexeme, token_line, token_column)) {
                free(lexeme); 
                return LEXER_STATUS_MEMORY_ERROR;
            }
            return LEXER_STATUS_OK;
        }

        // Operadores

        int next;
        int token_created = 1;

        switch(c) {
            case '=':
                next = fgetc(lexer->file);
                if (next == '=') {
                    advance_position(next, &lexer->line, &lexer->column);
                    token_created = token_init(out, EQUAL, "==", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, lexer->file);
                    token_created = token_init(out, ASSIGN, "=", token_line, token_column);
                }
                break;
            case '!':
                next = fgetc(lexer->file);
                if (next == '=') {
                    advance_position(next, &lexer->line, &lexer->column);
                    token_created = token_init(out, NOT_EQUAL, "!=", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, lexer->file);
                    char lex[2] = {(char)c, '\0'};
                    token_created = token_init(out, ERROR, lex, token_line, token_column);
                }
                break;
            case '<':
                next = fgetc(lexer->file);
                if (next == '=') {
                    advance_position(next, &lexer->line, &lexer->column);
                    token_created = token_init(out, LESS_EQUAL, "<=", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, lexer->file);
                    token_created = token_init(out, LESS, "<", token_line, token_column);
                }
                break;
            case '>':
                next = fgetc(lexer->file);
                if (next == '=') {
                    advance_position(next, &lexer->line, &lexer->column);
                    token_created = token_init(out, GREATER_EQUAL, ">=", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, lexer->file);
                    token_created = token_init(out, GREATER, ">", token_line, token_column);
                }
                break;
            case '&':
                next = fgetc(lexer->file);
                if (next == '&') {
                    advance_position(next, &lexer->line, &lexer->column);
                    token_created = token_init(out, AND, "&&", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, lexer->file);
                    char lex[2] = {(char)c, '\0'};
                    token_created = token_init(out, ERROR, lex, token_line, token_column);
                }
                break;
            case '|':
                next = fgetc(lexer->file);
                if (next == '|') {
                    advance_position(next, &lexer->line, &lexer->column);
                    token_created = token_init(out, OR, "||", token_line, token_column);
                } else {
                    if (next != EOF) ungetc(next, lexer->file);
                    char lex[2] = {(char)c, '\0'};
                    token_created = token_init(out, ERROR, lex, token_line, token_column);
                }
                break;
            case '+': token_created = token_init(out, PLUS, "+", token_line, token_column); break;
            case '-': token_created = token_init(out, MINUS, "-", token_line, token_column); break;
            case '*': token_created = token_init(out, STAR, "*", token_line, token_column); break;
            case '(': token_created = token_init(out, LPAREN, "(", token_line, token_column); break;
            case ')': token_created = token_init(out, RPAREN, ")", token_line, token_column); break;
            case '{': token_created = token_init(out, LBRACE, "{", token_line, token_column); break;
            case '}': token_created = token_init(out, RBRACE, "}", token_line, token_column); break;
            case ';': token_created = token_init(out, SEMICOLON, ";", token_line, token_column); break;
            default: {
                char lex[2] = {(char)c, '\0'};
                token_created = token_init(out, ERROR, lex, token_line, token_column);
                break;
            }
        }

        return token_created ? LEXER_STATUS_OK : LEXER_STATUS_IO_ERROR;
    }

    if (ferror(lexer->file)) {
        return LEXER_STATUS_IO_ERROR;
    }

    if (!token_init(out, TOKEN_EOF, "", lexer->line, lexer->column)) {
        return LEXER_STATUS_MEMORY_ERROR;
    }

    return LEXER_STATUS_OK;
}

int lexer_scan(FILE *file){
    Lexer lexer;
    if(!lexer_init(&lexer, file))
        return 2;

    Token token;
    LexerStatus status;

    while((status = lexer_next_token(&lexer, &token)) == LEXER_STATUS_OK){
        token_print(&token);
        TokenType type = token.type;
        token_destroy(&token);
        if(type == TOKEN_EOF) break;
    }

    lexer_destroy(&lexer);

    if(status == LEXER_STATUS_OK)
        printf("Programa sintacticamente correcto");
    return (status == LEXER_STATUS_OK) ? 0 : 2;
}