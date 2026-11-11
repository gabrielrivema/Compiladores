#define _POSIX_C_SOURCE 200809L
#include "semantic/symbol.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Symbol *symbol_create(const char *name, Type type, int scope_level,
                      int line, int column, ASTNode *declaration)
{
    /* TODO: allocate, copy name, initialize fields, clean up on failure. */
    if(!name) return NULL;
    
    Symbol *symbol = malloc(sizeof(Symbol));
    if(!symbol) return NULL;

    symbol->name = strdup(name);
    if(!symbol->name){
        free(symbol);
        return NULL;
    }

    symbol->type = type;
    symbol->scope_level = scope_level;
    symbol->line = line;
    symbol->column = column;
    symbol->declaration = declaration;

    return symbol; 
}

void symbol_destroy(Symbol *symbol)
{
    /* TODO: accept NULL, free owned name and symbol; never free declaration. */
    if(!symbol) return;

    free(symbol->name);
    symbol->name = NULL;
    free(symbol);
}

const char *type_to_string(Type type)
{
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_BOOL: return "bool";
        case TYPE_ERROR: return "error";
        case TYPE_UNKNOWN: return "desconocido";
    }
    return "desconocido";
}
