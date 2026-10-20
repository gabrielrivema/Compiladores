#ifndef MINIC_SEMANTIC_H
#define MINIC_SEMANTIC_H

#include "semantic/symbol_table.h"

typedef enum {
    SEMANTIC_OK,
    SEMANTIC_INVALID,
    SEMANTIC_INTERNAL_ERROR
} SemanticStatus;

typedef struct {
    SymbolTable symbol_table;
    size_t error_count;
    int had_internal_error;
} SemanticAnalyzer;

/* init returns OK or INTERNAL_ERROR. */
SemanticStatus semantic_analyzer_init(SemanticAnalyzer *analyzer);
/* Borrowed AST, already validated by lexer/parser. No type checks in P05. */
SemanticStatus semantic_analyze(SemanticAnalyzer *analyzer, ASTNode *program);
void semantic_report_internal_error(SemanticAnalyzer *analyzer,
                                    const char *message);
void semantic_analyzer_destroy(SemanticAnalyzer *analyzer);

#endif
