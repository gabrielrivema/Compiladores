#include "semantic/semantic.h"
#include "ast/ast.h" /* Adapt to the existing P04 AST. */
#include <stdio.h>

void semantic_report_internal_error(SemanticAnalyzer *analyzer,
                                    const char *message)
{
    if (!analyzer->had_internal_error) {
        fprintf(stderr, "Error interno: %s\n", message);
    }
    analyzer->had_internal_error = 1;
}

SemanticStatus semantic_analyzer_init(SemanticAnalyzer *analyzer)
{
    analyzer->error_count = 0;
    analyzer->had_internal_error = 0;
    if (symbol_table_init(&analyzer->symbol_table) != SYMBOL_TABLE_OK) {
        semantic_report_internal_error(analyzer,
            "no fue posible inicializar la tabla de simbolos.");
        return SEMANTIC_INTERNAL_ERROR;
    }
    return SEMANTIC_OK;
}

//Obtenemos el tipo de la declaración
static Type map_declared_type(ASTDeclaredType dt){
    if(dt==AST_TYPE_BOOL) return TYPE_INT;
    if(dt==AST_TYPE_INT) return TYPE_INT;
    return TYPE_UNKNOWN;
}

//Creación de errores independientes
static void report_semantic_error(SemanticAnalyzer *analyzer, size_t line, size_t column, const char *message){
    fprintf(stderr, "Error semantico [%zu:%zu]: \n %s \n", line, column, message);
    analyzer->error_count++;
}   

/////////////////Implementacion de visit's/////////////////////

static void visit_statement(SemanticAnalyzer *analyzer, ASTNode *node);
static void visit_expression(SemanticAnalyzer *analyzer, ASTNode *node);


static void visit_program(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;

    for(size_t i = 0; i<node->data.program.statements.count; i++){
        if(analyzer->had_internal_error) break;
        visit_statement(analyzer, node->data.program.statements.items[i]);
    }
}

static void visit_block(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;

    if(symbol_table_enter_scope(&analyzer->symbol_table) != SYMBOL_TABLE_OK){
        semantic_report_internal_error(analyzer, "no fue posible entrar a un nuevo ambito.");
        return;
    }

    for(size_t i = 0; i<node->data.block.statements.count; i++){
        if(analyzer->had_internal_error) break;
        visit_statement(analyzer, node->data.block.statements.items[i]);
    }

    symbol_table_exit_scope(&analyzer->symbol_table);
}

static void visit_variable_declaration(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;

    const char *name = node->data.variable_declaration.name;
    Type t = map_declared_type(node->data.variable_declaration.declared_type);
    int level = analyzer->symbol_table.current_scope ? analyzer->symbol_table.current_scope->level : 0;
    int line = node->line;
    int column = node->column;

    Symbol *exist = symbol_table_lookup_current(&analyzer->symbol_table, name);
    if(exist != NULL){
        char buf[256];
        snprintf(buf, sizeof(buf), "la variable '%s' ya fue declarada dentro de este ambito en [%zu,%zu].", name, (size_t)exist->line, (size_t)exist->column);
        report_semantic_error(analyzer, line, column, buf);
    }else{
        Symbol *sym = symbol_create(name, t, level, line, column, node);
        if(!sym){
            semantic_report_internal_error(analyzer, "fallo al crear memoria para el simbolo.");
            return;
        }
        if(symbol_table_insert(&analyzer->symbol_table, sym) != SYMBOL_TABLE_OK){
            symbol_destroy(sym);
            semantic_report_internal_error(analyzer, "fallo al insertar el simbolo en la tabla.");
            return;
        }
    }

    if(node->data.variable_declaration.initializer)
        visit_expression(analyzer, node->data.variable_declaration.initializer);
}

static void visit_assignment(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;

    const char *name = node->data.assignment.name;
    int line = node->line;
    int column = node->column;

    Symbol *sym = symbol_table_lookup(&analyzer->symbol_table, name);
    if(!sym){
        char buf[256];
        snprintf(buf, sizeof(buf), "la variable '%s' no ha sido declarada.", name);
        report_semantic_error(analyzer, line, column, buf);
    }

    if(node->data.assignment.value)
        visit_expression(analyzer, node->data.assignment.value);
}

static void visit_print_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    
    if(node->data.print_statement.expression)
        visit_expression(analyzer, node->data.print_statement.expression);
}

static void visit_if_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    if(node->data.if_statement.condition)
        visit_expression(analyzer, node->data.if_statement.condition);

    if(node->data.if_statement.then_branch)
        visit_statement(analyzer, node->data.if_statement.then_branch);

    if(node->data.if_statement.else_branch)
        visit_statement(analyzer, node->data.if_statement.else_branch);
}

static void visit_while_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    
    if(node->data.while_statement.condition)
        visit_expression(analyzer, node->data.while_statement.condition);

    if(node->data.while_statement.body)
        visit_statement(analyzer, node->data.while_statement.body);
}

static void visit_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;

    switch(node->type){
        case AST_VARIABLE_DECLARATION:
            visit_variable_declaration(analyzer, node);
            break;
        case AST_ASSIGNMENT:
            visit_assignment(analyzer, node);
            break;
        case AST_PRINT:
            visit_print_statement(analyzer, node);
            break;
        case AST_IF:
            visit_if_statement(analyzer, node);
            break;
        case AST_WHILE:
            visit_while_statement(analyzer, node);
            break;
        case AST_BLOCK:
            visit_block(analyzer, node);
            break;
        default: break;
    }
}

static void visit_expression(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    switch(node->type){
        case AST_IDENTIFIER:{
                const char *name = node->data.identifier.name;
                Symbol *sym = symbol_table_lookup(&analyzer->symbol_table, name);
                if(!sym){
                    char buf[256];
                    snprintf(buf, sizeof(buf), "la variable '%s' no ha sido declarada.", name);
                    report_semantic_error(analyzer, node->line, node->column, buf);
                }
            }
            break;
        case AST_BINARY_EXPRESSION:
            visit_expression(analyzer, node->data.binary.left);
            visit_expression(analyzer, node->data.binary.right);
            break;
        case AST_UNARY_EXPRESSION:
            visit_expression(analyzer, node->data.unary.operand);
            break;
        default: break;
    }
}

SemanticStatus semantic_analyze(SemanticAnalyzer *analyzer, ASTNode *program)
{
    if (analyzer->had_internal_error) {
        return SEMANTIC_INTERNAL_ERROR;
    }
    /* TODO: add separated visitors for program, statements and expressions.
       Adapt fields to your AST. Preserve source order and positions.
       Continue after semantic errors, unwind scopes after internal failures.
       No expression type checking, no printing of the AST, no execution. */
    //semantic_report_internal_error(analyzer, "recorrido semantico pendiente de implementar en la base inicial.");
    visit_program(analyzer, program);
    if(analyzer->had_internal_error)
        return SEMANTIC_INTERNAL_ERROR;

    /* TODO: when completed, choose OK/INVALID/INTERNAL_ERROR from actual state. */
    return analyzer->error_count > 0 ? SEMANTIC_INVALID : SEMANTIC_OK;
}

void semantic_analyzer_destroy(SemanticAnalyzer *analyzer)
{
    if(!analyzer) return;
    symbol_table_destroy(&analyzer->symbol_table);
    analyzer->error_count = 0;
    analyzer->had_internal_error = 0;
    /* The AST is borrowed and is not destroyed here. */
}
