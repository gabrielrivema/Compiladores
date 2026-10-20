#ifndef MINIC_PARSER_H
#define MINIC_PARSER_H

/* Adapten esta inclusión a la interfaz incremental de su proyecto. */
#include "../lexer/lexer.h"
#include "ast/ast.h"

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
ASTNode *parse_program(Parser *parser);
ASTNode *parse_statement(Parser *parser);
ASTNode *parse_assigment(Parser *parser);
ASTNode *parse_declaration(Parser *parser);
ASTNode *parse_print(Parser *parser);
ASTNode *parse_if(Parser *parser);
ASTNode *parse_while(Parser *parser);
ASTNode *parse_block(Parser *parser);
ASTNode *parse_expression(Parser *parser);
ASTNode *parse_or(Parser *parser);
ASTNode *parse_and(Parser *parser);
ASTNode *parse_equal(Parser *parser);
ASTNode *parse_comparison(Parser *parser);
ASTNode *parse_add(Parser *parser);
ASTNode *parse_multiplicative(Parser *parser);
ASTNode *parse_unary(Parser *parser);
ASTNode *parse_primary(Parser *parser);



#endif

