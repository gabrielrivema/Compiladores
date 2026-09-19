#include "parser/parser.h"

int parser_init(Parser *parser, Lexer *lexer) {
    /* TODO: validar los argumentos. */
    /* TODO: inicializar todos los campos del parser. */
    /* TODO: solicitar el primer token de anticipación. */
    (void)parser;
    (void)lexer;
    return 0;
}

int parser_advance(Parser *parser) {
    /* TODO: liberar current cuando exista y le pertenezca al parser. */
    /* TODO: solicitar el siguiente token sin imprimirlo. */
    /* TODO: distinguir token ERROR de un fallo interno del lexer. */
    (void)parser;
    return 0;
}

int parser_check(const Parser *parser, TokenType type) {
    /* TODO: comprobar el lookahead sin consumirlo. */
    (void)parser;
    (void)type;
    return 0;
}

int parser_match(Parser *parser, TokenType type) {
    /* TODO: consumir el token sólo si coincide con type. */
    (void)parser;
    (void)type;
    return 0;
}

const Token *parser_current(const Parser *parser) {
    /* TODO: devolver NULL cuando todavía no exista lookahead. */
    (void)parser;
    return 0;
}

void parser_destroy(Parser *parser) {
    /* TODO: liberar el token actual exactamente una vez. */
    /* TODO: dejar el estado en una condición segura. */
    (void)parser;
}

