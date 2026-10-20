#ifndef MINIC_SYMBOL_H
#define MINIC_SYMBOL_H

typedef struct ASTNode ASTNode;

typedef enum {
    TYPE_INT,
    TYPE_BOOL,
    TYPE_ERROR,   /* Reserved for P06. */
    TYPE_UNKNOWN  /* Reserved for P06. */
} Type;

typedef struct Symbol {
    char *name;             /* Owned copy. */
    Type type;
    int scope_level;
    int line;
    int column;
    ASTNode *declaration;   /* Borrowed: never freed by symbol_destroy. */
} Symbol;

Symbol *symbol_create(const char *name, Type type, int scope_level,
                      int line, int column, ASTNode *declaration);
void symbol_destroy(Symbol *symbol);

#endif
