#ifndef MINIC_SCOPE_H
#define MINIC_SCOPE_H

#include <stddef.h>
#include "semantic/symbol.h"

/* Example dynamic representation; a hash table is also acceptable. */
typedef struct Scope {
    Symbol **symbols;
    size_t count;
    size_t capacity;
    int level;
    struct Scope *parent;      /* Visibility chain. */
    struct Scope *next_owned;  /* Optional ownership registry, not visibility. */
} Scope;

/* NULL means allocation failure. Does not take ownership of parent. */
Scope *scope_create(Scope *parent);
/* Frees this scope and its symbols only, not parent or next_owned. */
void scope_destroy(Scope *scope);

#endif
