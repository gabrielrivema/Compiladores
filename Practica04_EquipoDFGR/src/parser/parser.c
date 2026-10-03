#define _POSIX_C_SOURCE 200809L
#include "parser/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Función generica para cuando se encuentra un error.
 * Evita cascada de errores mostrando solo el primer encontrado.
 */
void syntax_error(Parser *parser, const Token *token, const char *message){
    if(parser->panic_mode) return;
    parser->panic_mode = 1;
    parser->had_error = 1;

    if(token->type == TOKEN_EOF){
        fprintf(stderr, "Error sintactico [%zu:%zu]: se esperaba %s, pero se encontro TOKEN_EOF.\n", token->line, token->column, message);
    } else {
        fprintf(stderr, "Error sintactico [%zu:%zu]: se esperaba %s, pero se encontro '%s'.\n", token->line, token->column, message, token->lexeme ? token->lexeme : "");
    }
}

int parser_init(Parser *parser, Lexer *lexer) {
    /* TODO: validar los argumentos. */
    if(!parser || !lexer)
        return 0;
    /* TODO: inicializar todos los campos del parser. */
    parser->lexer = lexer;
    parser->had_error = 0;
    parser->panic_mode = 0;
    parser->current.lexeme = NULL;
    parser->previus.lexeme = NULL;
    /* TODO: solicitar el primer token de anticipación. */
    return parser_advance(parser);
}

int parser_advance(Parser *parser) {
    if(!parser || !parser->lexer)
        return 0;
    
    /* TODO: liberar current cuando exista y le pertenezca al parser. */
    token_destroy(&parser->previus);
    parser->previus = parser->current;


    
    while(true) { 
        /* TODO: solicitar el siguiente token sin imprimirlo. */
        LexerStatus status = lexer_next_token(parser->lexer, &parser->current);
        if(status != LEXER_STATUS_OK){
            parser->had_error = 1;
            return 0;
        }
    
        /* TODO: distinguir token ERROR de un fallo interno del lexer. */
        if(parser->current.type == ERROR){
            parser->had_error = 1;
            fprintf(stderr, "Error lexico [%zu:%zu]: caracter invalido '%s'.\n", parser->current.line, parser->current.column, parser->current.lexeme ? parser->current.lexeme : "");
            token_destroy(&parser->current);
            continue;
        }

        break;
    }
    
    return 1;
}

int parser_check(const Parser *parser, TokenType type) {
    /* TODO: comprobar el lookahead sin consumirlo. */
    if(!parser) return 0;

    return parser->current.type == type;
}

int parser_match(Parser *parser, TokenType type) {
    /* TODO: consumir el token sólo si coincide con type. */
    if(parser_check(parser, type)){
        parser_advance(parser);
        return 1;
    }
    return 0;
}

/**
 * Consume el token esperado (, ), }, 
 */
void consume(Parser *parser, TokenType expected, const char *message){
    if(parser_check(parser, expected)){
        parser_advance(parser);
        return;
    }
    syntax_error(parser, &parser->current, message);
}

/**
 * Sincroniza las sentencias para poder seguir avanzando en caso de error
 */
void synchronize(Parser *parser){
    parser->panic_mode = 0;

    while(parser->current.type != TOKEN_EOF){
        if (parser->current.type == SEMICOLON){
            parser_advance(parser);
            return;
        }

        switch (parser->current.type){
            case INT:
            case BOOL:
            case IF:
            case WHILE:
            case PRINT:
            case IDENTIFIER:
            case LBRACE:
            case RBRACE:
            case ELSE:
                return;
            default:
                break;
        }
        parser_advance(parser);
    }
}

void parser_destroy(Parser *parser) {
    /* TODO: liberar el token actual exactamente una vez. */
    if(!parser) return;
    token_destroy(&parser->current);
    token_destroy(&parser->previus);

    /* TODO: dejar el estado en una condición segura. */
}

/////////////////Gramatica + Construcción del AST/////////////////////

/**
 * Consume INT o BOOL
 */
ASTNode *parse_declaration(Parser *parser){
    TokenType type_token = parser->current.type;
    parser_advance(parser);
    if(!parser_check(parser, IDENTIFIER)){
        syntax_error(parser, &parser->current, "un identificador");
        return NULL;
    }
    
    char *name = strdup(parser->current.lexeme);
    int line = parser->current.line;
    int column = parser->current.column;
    parser_advance(parser);

    ASTDeclaredType declared = (type_token == INT) ? AST_TYPE_INT : AST_TYPE_BOOL;
    ASTNode *initializer = NULL;

    if(parser_match(parser, ASSIGN)){
        initializer = parse_expression(parser);
        if(initializer == NULL){
            free(name);
            return NULL;
        }
    }

    consume(parser, SEMICOLON, "';'");
    if(parser->had_error && parser->panic_mode){
        free(name);
        ast_destroy(initializer);
        return NULL;
    }

    return ast_create_variable_declaration(declared, name, initializer, line, column);
    
    }

/**
 * Consume IDENTIFIER
 */
ASTNode *parse_assigment(Parser *parser){
    char *name = strdup(parser->current.lexeme);
    int line = parser->current.line;
    int column = parser->current.column;
    parser_advance(parser);

    consume(parser, ASSIGN, "'='");
    
    ASTNode *value = parse_expression(parser);
    if(value == NULL){
        free(name);
        return NULL;
    }

    consume(parser, SEMICOLON, "';'");

    if(parser->had_error && parser->panic_mode){
        free(name);
        ast_destroy(value);
        return NULL;
    }

    return ast_create_assignment(name, value, line, column);
}


ASTNode *parse_print(Parser *parser){
    int line = parser->current.line;
    int column = parser->current.column;

    parser_advance(parser);
    consume(parser, LPAREN, "'('");

    ASTNode *expr = parse_expression(parser);
    if(expr == NULL) return NULL;

    consume(parser, RPAREN, "')'");
    consume(parser, SEMICOLON, "';'");

    if(parser->had_error && parser->panic_mode){
        ast_destroy(expr);
        return NULL;
    }

    return ast_create_print(expr, line, column);
}

ASTNode *parse_if(Parser *parser){
    int line = parser->current.line;
    int column = parser->current.column;
    
    parser_advance(parser);
    consume(parser, LPAREN, "'('");
    ASTNode *condition = parse_expression(parser);

    if(condition == NULL) return NULL;


    consume(parser, RPAREN, "')'");
    ASTNode *then_branch = parse_statement(parser);
    if(then_branch == NULL){
        ast_destroy(condition);
        return NULL;
    }
    ASTNode *else_branch = NULL;
    if(parser_match(parser, ELSE)){
        else_branch = parse_statement(parser);
        if(else_branch == NULL){
            ast_destroy(condition);
            ast_destroy(then_branch);
            return NULL;
        }
    }

    return ast_create_if(condition, then_branch, else_branch, line, column);
}

ASTNode *parse_while(Parser *parser){
    int line = parser->current.line;
    int column = parser->current.column;

    parser_advance(parser);
    consume(parser, LPAREN, "'('");
    ASTNode *condition = parse_expression(parser);
    if(condition == NULL) return NULL;

    consume(parser, RPAREN, "')'");
    ASTNode *body = parse_statement(parser);

    if(body==NULL){
        ast_destroy(condition);
        return NULL;
    }

    return ast_create_while(condition, body, line, column);
}

ASTNode *parse_block(Parser *parser){
    int line = parser->current.line;
    int column = parser->current.column;

    parser_advance(parser);

    ASTNodeList statements;
    ast_node_list_init(&statements);

    while(!parser_check(parser, RBRACE) && !parser_check(parser, TOKEN_EOF)){
        ASTNode *stmt = parse_statement(parser);
        if(stmt != NULL){
            if(!ast_node_list_append(&statements, stmt)){
                ast_destroy(stmt);
                ast_node_list_destroy(&statements);
                return NULL;
            }
        } else {
            if(parser->panic_mode)
                synchronize(parser);
            else
                break;
        }
    }

    consume(parser, RBRACE, "'}'");
    if(parser->had_error && parser->panic_mode){
        ast_node_list_destroy(&statements);
        return NULL;
    }

    return ast_create_block(statements, line, column);
}

/**
 * Metodo que filtra las sentencias dadas
 */
ASTNode *parse_statement(Parser *parser){
    ASTNode *stmt = NULL;
    switch (parser->current.type){
        case INT:
        case BOOL:
            stmt = parse_declaration(parser);
            break;
        case IDENTIFIER:
            stmt = parse_assigment(parser);
            break;
        case PRINT:
            stmt = parse_print(parser);
            break;
        case IF:
            stmt = parse_if(parser);
            break;
        case WHILE:
            stmt = parse_while(parser);
            break;
        case LBRACE:
            stmt = parse_block(parser);
            break;
        default:
            syntax_error(parser, &parser->current, "una sentencia");
            synchronize(parser);
            break;
    }
    if(parser->panic_mode)
        synchronize(parser);
    return stmt;
}

ASTNode *parse_primary(Parser *parser){
    if(parser_match(parser, INTEGER))
        return ast_create_integer(parser->previus.lexeme, parser->previus.line, parser->previus.column);

    if(parser_match(parser, TRUE))
        return ast_create_boolean(1, parser->previus.line, parser->previus.column);

    if(parser_match(parser, FALSE))
        return ast_create_boolean(0, parser->previus.line, parser->previus.column);
    
    if(parser_match(parser, IDENTIFIER))
        return ast_create_identifier(parser->previus.lexeme, parser->previus.line, parser->previus.column);    
    
    if(parser_match(parser, LPAREN)){
        ASTNode *expr = parse_expression(parser);
        consume(parser, RPAREN, "')'");
        return expr;
    }

    syntax_error(parser, &parser->current, "una expresion");
    return NULL;
}

ASTNode *parse_unary(Parser *parser){
    if(parser_match(parser, MINUS)){
        int line = parser->current.line;
        int column = parser->current.column;
        ASTNode *operand = parse_unary(parser);
        return ast_create_unary(OP_NEGATE, operand, line, column);
    }
    return parse_primary(parser);
}

ASTNode *parse_multiplicative(Parser *parser){
    ASTNode *left = parse_unary(parser);
    while(parser_match(parser, STAR) || parser_match(parser, SLASH)){
        Token op_token = parser->previus;
        ASTNode *right = parse_unary(parser);
        BinaryOperator op = (op_token.type == STAR) ? OP_MULTIPLY : OP_DIVIDE;
        left = ast_create_binary(op, left, right, op_token.line, op_token.column);
    }
    return left;
}

ASTNode *parse_add(Parser *parser){
    ASTNode *left = parse_multiplicative(parser);
    while(parser_match(parser, PLUS) || parser_match(parser, MINUS)){
        Token op_token = parser->previus;
        ASTNode *right = parse_multiplicative(parser);
        BinaryOperator op = (op_token.type == PLUS) ? OP_ADD : OP_SUBTRACT;
        left = ast_create_binary(op, left, right, op_token.line, op_token.column);
    }
    return left;
}

ASTNode *parse_comparison(Parser *parser){
    ASTNode *left = parse_add(parser);
    while(parser_match(parser, LESS) || parser_match(parser, LESS_EQUAL) || parser_match(parser, GREATER) || parser_match(parser, GREATER_EQUAL)){
        Token op_token = parser->previus;
        ASTNode *right = parse_add(parser);
        BinaryOperator op;
        if (op_token.type == LESS) op = OP_LESS;
        else if (op_token.type == LESS_EQUAL) op = OP_LESS_EQUAL;
        else if (op_token.type == GREATER) op = OP_GREATER;
        else op = OP_GREATER_EQUAL;
        left = ast_create_binary(op, left, right, op_token.line, op_token.column);
    }
    return left;
}

ASTNode *parse_equal(Parser *parser){
    ASTNode *left = parse_comparison(parser);
    while(parser_match(parser, EQUAL) || parser_match(parser, NOT_EQUAL)){
        Token op_token = parser->previus;
        ASTNode *right = parse_comparison(parser);
        BinaryOperator op = (op_token.type == EQUAL) ? OP_EQUAL : OP_NOT_EQUAL;
        left = ast_create_binary(op, left, right, op_token.line, op_token.column);
    }
    return left;
}

ASTNode *parse_and(Parser *parser){
    ASTNode *left = parse_equal(parser);
    while(parser_match(parser, AND)){
        Token op_token = parser->previus;
        ASTNode *right = parse_equal(parser);
        left = ast_create_binary(OP_AND, left, right, op_token.line, op_token.column);
    }
    return left;
}

ASTNode *parse_or(Parser *parser){
    ASTNode *left = parse_and(parser);
    while(parser_match(parser, OR)){
        Token op_token = parser->previus;
        ASTNode *right = parse_and(parser);
        left = ast_create_binary(OP_OR, left, right, op_token.line, op_token.column);
    }
    return left;
}

ASTNode *parse_expression(Parser *parser){
    return parse_or(parser);
}

ASTNode *parse_program(Parser *parser){
    ASTNodeList statements;
    ast_node_list_init(&statements);

    while(!parser_check(parser, TOKEN_EOF)){
        ASTNode *stmt = parse_statement(parser);
        if(stmt != NULL){
            if(!ast_node_list_append(&statements, stmt)){
                ast_destroy(stmt);
                ast_node_list_destroy(&statements);
                return NULL;
            }
        } else {
            if(parser->panic_mode){
                synchronize(parser);
            } else {
                break;
            }
        }
    }

    if(!parser_check(parser, TOKEN_EOF)){
        syntax_error(parser, &parser->current, "Fin del archivo");
    }
    
    if (parser->had_error){
        ast_node_list_destroy(&statements);
        return NULL;
    }

    int line = parser->current.line;
    int column = parser->current.column;
    if(statements.count > 0 && statements.items[0] != NULL){
        line = statements.items[0]->line;
        column = statements.items[0]->column;
    }

    return ast_create_program(statements, line, column);
}