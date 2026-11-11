#ifndef MINIC_SYMBOL_TABLE_H
#define MINIC_SYMBOL_TABLE_H

#include "semantic/scope.h"

typedef enum {
    SYMBOL_TABLE_OK,
    SYMBOL_TABLE_DUPLICATE,
    SYMBOL_TABLE_INTERNAL_ERROR
} SymbolTableStatus;

typedef struct {
    Scope *current_scope;
    Scope *owned_scopes;
} SymbolTable;

/* init/enter return OK or INTERNAL_ERROR, never DUPLICATE. */
SymbolTableStatus symbol_table_init(SymbolTable *table);
SymbolTableStatus symbol_table_enter_scope(SymbolTable *table);
/* Only after a successful entry; must not exit the global scope. */
void symbol_table_exit_scope(SymbolTable *table);

/* Borrowed pointers; NULL means not found in the completed implementation. */
Symbol *symbol_table_lookup_current(const SymbolTable *table, const char *name);
Symbol *symbol_table_lookup(const SymbolTable *table, const char *name);

/* Takes ownership only on OK; preserves original entry on DUPLICATE. */
SymbolTableStatus symbol_table_insert(SymbolTable *table, Symbol *symbol);
/* Also safe after an incomplete initialization. */
void symbol_table_destroy(SymbolTable *table);

#endif
