#ifndef MINIC_PARSER_H
#define MINIC_PARSER_H

/* Adapten esta inclusión a la interfaz incremental de su proyecto. */
#include "../lexer/lexer.h"

typedef struct {
    Lexer *lexer;
    Token current;
    Token previus;
    int had_error;
    int panic_mode;
} Parser;

int parser_init(Parser *parser, Lexer *lexer);
void parser_destroy(Parser *parser);

int parser_advance(Parser *parser);
int parser_check(const Parser *parser, TokenType type);
int parser_match(Parser *parser, TokenType type);
void consume(Parser *parser, TokenType expected, const char *message);
void syntax_error(Parser *parser, const Token *token, const char *message);
void synchronize(Parser *parser);

/* TODO: agreguen aquí la operación pública que inicia el análisis completo. */
int parse_program(Parser *parser);

void parse_statement(Parser *parser);
void parse_assigment(Parser *parser);
void parse_declaration(Parser *parser);
void parse_print(Parser *parser);
void parse_if(Parser *parser);
void parse_while(Parser *parser);
void parse_block(Parser *parser);
void parse_expression(Parser *parser);
void parse_or(Parser *parser);
void parse_and(Parser *parser);
void parse_equal(Parser *parser);
void parse_comparison(Parser *parser);
void parse_add(Parser *parser);
void parse_multiplicative(Parser *parser);
void parse_unary(Parser *parser);
void parse_primary(Parser *parser);



#endif

