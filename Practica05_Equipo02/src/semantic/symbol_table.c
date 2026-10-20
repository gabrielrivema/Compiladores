#include "semantic/symbol_table.h"
#include <stdlib.h>
#include <string.h>

SymbolTableStatus symbol_table_init(SymbolTable *table)
{
    if(!table) return SYMBOL_TABLE_INTERNAL_ERROR;

    table->current_scope = NULL;
    table->owned_scopes = NULL;

    /* TODO: create global scope. Leave cleanup safe if allocation fails. */
    Scope *global_scope = scope_create(NULL);
    if(!global_scope) return SYMBOL_TABLE_INTERNAL_ERROR;

    global_scope->next_owned = NULL;
    table->owned_scopes = global_scope;
    table->current_scope = global_scope;

    return SYMBOL_TABLE_OK;
}

SymbolTableStatus symbol_table_enter_scope(SymbolTable *table)
{
    if(!table || !table->current_scope) return SYMBOL_TABLE_INTERNAL_ERROR;

    /* TODO: create child, register ownership and change current only on success. */
    Scope *child_scope = scope_create(table->current_scope);
    if(!child_scope) return SYMBOL_TABLE_INTERNAL_ERROR;

    child_scope->next_owned = table->owned_scopes;
    table->owned_scopes = child_scope;

    table->current_scope = child_scope;
    
    return SYMBOL_TABLE_OK;
}

void symbol_table_exit_scope(SymbolTable *table)
{
    if(!table || !table->current_scope) return;
    /* TODO: restore parent; protect global; follow the chosen lifetime policy. */
    if(table->current_scope->parent == NULL) return;

    table->current_scope = table->current_scope->parent;
}

Symbol *symbol_table_lookup_current(const SymbolTable *table, const char *name)
{
    if(!table || !table->current_scope || !name) return NULL;
    
    /* TODO: search only the current scope, comparing full names. */
    Scope *scope = table->current_scope;
    for(size_t i = 0; i<scope->count; i++)
        if(strcmp(scope->symbols[i]->name, name) == 0)
            return scope->symbols[i];

    return NULL;
}

Symbol *symbol_table_lookup(const SymbolTable *table, const char *name)
{
    if(!table || !table->current_scope || !name) return NULL;

    /* TODO: search from current scope through parents, never inactive siblings. */
    Scope *scope = table->current_scope;
    while(scope != NULL){
        for(size_t i = 0; i<scope->count; i++)
            if(strcmp(scope->symbols[i]->name, name) == 0)
                return scope->symbols[i];
        scope = scope->parent;
    }
    return NULL;
}

SymbolTableStatus symbol_table_insert(SymbolTable *table, Symbol *symbol)
{
    if(!table || !table->current_scope || !symbol) return SYMBOL_TABLE_INTERNAL_ERROR;

    /* TODO: detect local duplicate; grow storage safely; transfer only on OK. */
    if(symbol_table_lookup_current(table, symbol->name) != NULL) return SYMBOL_TABLE_DUPLICATE;

    Scope *scope = table->current_scope;
    if(scope->count >= scope->capacity){
        size_t new_cap = scope->capacity * 2;
        Symbol **new_symbols = realloc(scope->symbols, new_cap * sizeof(Symbol *));
        if(!new_symbols) return SYMBOL_TABLE_INTERNAL_ERROR;

        scope->symbols = new_symbols;
        scope->capacity = new_cap;
    }

    scope->symbols[scope->count++] = symbol;
    return SYMBOL_TABLE_OK;
}

void symbol_table_destroy(SymbolTable *table)
{
    if(!table) return;
    /* TODO: free every owned scope once, including retained inactive scopes.
       Implement cleanup before adding allocations elsewhere. */
    Scope *current = table->owned_scopes;
    while(current != NULL){
        Scope *next = current->next_owned;
        scope_destroy(current);
        current = next;
    }
    
    table->current_scope = NULL;
    table->owned_scopes = NULL;
}
