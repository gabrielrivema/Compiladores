#include "lexer/lexer.h"

#include <stdio.h>

int simple_token_type(int c, TokenType *type) {
    if (type == NULL) {
        return 0;
    }
    //Simbolos simples para pŕactica 1, falta expandir para práctica 2
    switch (c) {
        case '+':
            *type = PLUS;
            return 1;
        case '-':
            *type = MINUS;
            return 1;
        case '*':
            *type = STAR;
            return 1;
        case '/':
            *type = SLASH;
            return 1;
        case '=':
            *type = ASSIGN;
            return 1;
        case '<':
            *type = LESS;
            return 1;
        case '>':
            *type = GREATER;
            return 1;
        case '(':
            *type = LPAREN;
            return 1;
        case ')':
            *type = RPAREN;
            return 1;
        case '{':
            *type = LBRACE;
            return 1;
        case '}':
            *type = RBRACE;
            return 1;
        case ';':
            *type = SEMICOLON;
            return 1;
        default:
            return 0;
    }
}

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

    /* TODO: adaptar esta lógica para \r aislado y para la secuencia \r\n. */
}

int is_integer(int c){
    return (c >= '0' && c<='9');
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

        if(c == '\r') {
            int next = fgetc(file);
                if (next != '\n') {
                    ungetc(next, file);
                }else{
                    c = next;
                }
        }
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

        if(is_integer(c)) {
            //Suponemos un integer de 100 numeros
            char lexeme[100];
            Token token;

            //Posición donde se va a guardar el primer numero
            int idx = 0;
            lexeme[idx++] = (char)c;
            
            //Leemos desde ese numero en espera de otro, en caso contrario termina de leer
            while ((c = fgetc(file)) != EOF && is_integer(c)) {
                advance_position(c, &line, &column);
                if(idx < (int)(sizeof(lexeme)-1)){
                    lexeme[idx++] = (char)c;
                }
            }

            //En la última posición de indice se cierra la cadena
            lexeme[idx] = '\0';

            //Como termino de encontrar numeros entonces hubo otro caracter, retrocedemos para que lo analice como de costrumbre el método
            ungetc(c, file);

            //Guardamos el token 
            if (!token_init(&token, INTEGER, lexeme,
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
        
        char lexeme[2] = {(char)c, '\0'};
        Token token;

        if (!token_init(&token, ERROR, lexeme,
                            token_line, token_column)) {
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
    if (!token_init(&token_eof, TOKEN_EOF, "",
                            line, column)) {
                fprintf(stderr, "Error: no se pudo reservar memoria.\n");
                return 2;
            }

            token_print(&token_eof);
            token_destroy(&token_eof);
    /* TODO: generar TOKEN_EOF con la posición actual. */
    return 0;
}
