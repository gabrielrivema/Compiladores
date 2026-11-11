#include "semantic/scope.h"
#include <stdlib.h>
#include <string.h>

Scope *scope_create(Scope *parent)
{
    /* TODO: allocate a scope, initialize storage, level and borrowed parent. */
    Scope *scope = malloc(sizeof(Scope));
    if(!scope) return NULL;

    scope->capacity = 8;
    scope->count = 0;
    scope->symbols = malloc(scope->capacity * sizeof(Symbol *));
    if(!scope->symbols){
        free(scope);
        return NULL;
    }

    scope->level = parent ? parent->level + 1 : 0;
    scope->parent = parent;
    scope->next_owned = NULL;

    return scope;
}

void scope_destroy(Scope *scope)
{
    /* TODO: free owned symbols and storage; do not recursively free links. */
    if(!scope) return;

    for(size_t i = 0; i<scope->count; i++)
        symbol_destroy(scope->symbols[i]);

    free(scope->symbols);
    scope->symbols = NULL;
    scope->count = 0;
    scope->capacity = 0;
    free(scope);
}
