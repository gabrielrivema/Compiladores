#include "parser/parser.h"
#include <stdio.h>
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

const Token *parser_current(const Parser *parser) {
    /* TODO: devolver NULL cuando todavía no exista lookahead. */
    (void)parser;
    return 0;
}

void parser_destroy(Parser *parser) {
    /* TODO: liberar el token actual exactamente una vez. */
    if(!parser) return;
    token_destroy(&parser->current);
    token_destroy(&parser->previus);

    /* TODO: dejar el estado en una condición segura. */
}

/////////////////Gramatica/////////////////////

/**
 * Consume INT o BOOL
 */
void parse_declaration(Parser *parser){
    parser_advance(parser);
    consume(parser, IDENTIFIER, "un identificador");
    if(parser_match(parser, ASSIGN))
        parse_expression(parser);

    consume(parser, SEMICOLON, "';'");
}

/**
 * Consume IDENTIFIER
 */
void parse_assigment(Parser *parser){
    parser_advance(parser);
    consume(parser, ASSIGN, "'='");
    parse_expression(parser);
    consume(parser, SEMICOLON, "';'");
}


void parse_print(Parser *parser){
    parser_advance(parser);
    consume(parser, LPAREN, "'('");
    parse_expression(parser);
    consume(parser, RPAREN, "')'");
    consume(parser, SEMICOLON, "';'");
}

void parse_if(Parser *parser){
    parser_advance(parser);
    consume(parser, LPAREN, "'('");
    parse_expression(parser);
    consume(parser, RPAREN, "')'");
    parse_statement(parser);

    if(parser_match(parser, ELSE))
        parse_statement(parser);
}

void parse_while(Parser *parser){
    parser_advance(parser);
    consume(parser, LPAREN, "'('");
    parse_expression(parser);
    consume(parser, RPAREN, "')'");
    parse_statement(parser);
}

void parse_block(Parser *parser){
    parser_advance(parser);
    while(!parser_check(parser, RBRACE) && !parser_check(parser, TOKEN_EOF))
        parse_statement(parser);

    consume(parser, RBRACE, "'}'");
}

/**
 * Metodo que filtra las sentencias dadas
 */
void parse_statement(Parser *parser){
    switch (parser->current.type){
        case INT:
        case BOOL:
            parse_declaration(parser);
            break;
        case IDENTIFIER:
            parse_assigment(parser);
            break;
        case PRINT:
            parse_print(parser);
            break;
        case IF:
            parse_if(parser);
            break;
        case WHILE:
            parse_while(parser);
            break;
        case LBRACE:
            parse_block(parser);
            break;
        default:
            syntax_error(parser, &parser->current, "una sentencia");
            synchronize(parser);
            break;
    }
    if(parser->panic_mode)
        synchronize(parser);
}

void parse_primary(Parser *parser){
    if (parser_match(parser, INTEGER) || parser_match(parser, TRUE) || parser_match(parser, FALSE) || parser_match(parser, IDENTIFIER))
        return;
    
    if (parser_match(parser, LPAREN)) {
        parse_expression(parser);
        consume(parser, RPAREN, "')'");
        return;
    }

    syntax_error(parser, &parser->current, "una expresion");
}

void parse_unary(Parser *parser){
    if(parser_match(parser, MINUS))
        parse_unary(parser);
    else
        parse_primary(parser);
}

void parse_multiplicative(Parser *parser){
    parse_unary(parser);
    while(parser_match(parser, STAR) || parser_match(parser, SLASH))
        parse_unary(parser);
}

void parse_add(Parser *parser){
    parse_multiplicative(parser);
    while(parser_match(parser, PLUS) || parser_match(parser, MINUS))
        parse_multiplicative(parser);
}

void parse_comparison(Parser *parser){
    parse_add(parser);
    while(parser_match(parser, LESS) || parser_match(parser, LESS_EQUAL) || parser_match(parser, GREATER) || parser_match(parser, GREATER_EQUAL))
        parse_add(parser);
}

void parse_equal(Parser *parser){
    parse_comparison(parser);
    while(parser_match(parser, EQUAL) || parser_match(parser, NOT_EQUAL))
        parse_comparison(parser);
}

void parse_and(Parser *parser){
    parse_equal(parser);
    while(parser_match(parser, AND))
        parse_equal(parser);
}

void parse_or(Parser *parser){
    parse_and(parser);
    while(parser_match(parser, OR))
        parse_and(parser);
}

void parse_expression(Parser *parser){
    parse_or(parser);
}

int parse_program(Parser *parser){
    while(!parser_check(parser, TOKEN_EOF))
        parse_statement(parser);

    if(!parser_check(parser, TOKEN_EOF))
        syntax_error(parser, &parser->current, "Fin del archivo");
    
    return !parser->had_error;
}