#include "semantic/semantic.h"
#include "ast/ast.h" /* Adapt to the existing P04 AST. */
#include <stdio.h>
#include <stdarg.h>

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
    if(dt==AST_TYPE_BOOL) return TYPE_BOOL;
    if(dt==AST_TYPE_INT) return TYPE_INT;
    return TYPE_UNKNOWN;
}

//Creación de errores independientes
static void report_semantic_error(SemanticAnalyzer *analyzer, size_t line, size_t column, const char *format, ...){
    va_list arguments;
    fprintf(stderr, "Error semantico [%zu:%zu]: \n ", line, column);
    va_start(arguments, format);
    vfprintf(stderr, format, arguments);
    va_end(arguments);
    fputs(" \n", stderr);
    analyzer->error_count++;
}   

/////////////////Implementacion de visit's/////////////////////

static void visit_statement(SemanticAnalyzer *analyzer, ASTNode *node);
static Type visit_expression(SemanticAnalyzer *analyzer, ASTNode *node);


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
        report_semantic_error(analyzer, line, column,
            "la variable '%s' ya fue declarada dentro de este ambito en [%zu,%zu].",
            name, (size_t)exist->line, (size_t)exist->column);
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

    if(node->data.variable_declaration.initializer) {
        Type actual = visit_expression(analyzer, node->data.variable_declaration.initializer);
        if (actual != TYPE_ERROR && actual != t) {
            fprintf(stderr, "Error semantico [%d:%d]: inicializacion de '%s': se esperaba %s, se encontro %s.\n",
                    line, column, name, type_to_string(t), type_to_string(actual));
            analyzer->error_count++;
        }
    }
}

static void visit_assignment(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;

    const char *name = node->data.assignment.name;
    int line = node->line;
    int column = node->column;

    Symbol *sym = symbol_table_lookup(&analyzer->symbol_table, name);
    if(!sym){
        report_semantic_error(analyzer, line, column,
            "la variable '%s' no ha sido declarada.", name);
    }

    if(node->data.assignment.value) {
        Type actual = visit_expression(analyzer, node->data.assignment.value);
        if (sym && actual != TYPE_ERROR && actual != sym->type) {
            fprintf(stderr, "Error semantico [%d:%d]: asignacion a '%s': se esperaba %s, se encontro %s.\n",
                    line, column, name, type_to_string(sym->type), type_to_string(actual));
            analyzer->error_count++;
        }
    }
}

static void visit_print_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    
    if(node->data.print_statement.expression)
        visit_expression(analyzer, node->data.print_statement.expression);
}

static void check_condition(SemanticAnalyzer *analyzer, ASTNode *condition, const char *construct) {
    Type actual = visit_expression(analyzer, condition);
    if (actual != TYPE_ERROR && actual != TYPE_BOOL) {
        fprintf(stderr, "Error semantico [%d:%d]: condicion de %s: se esperaba bool, se encontro %s.\n",
                condition->line, condition->column, construct, type_to_string(actual));
        analyzer->error_count++;
    }
}

static void visit_if_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    if(node->data.if_statement.condition)
        check_condition(analyzer, node->data.if_statement.condition, "if");

    if(node->data.if_statement.then_branch)
        visit_statement(analyzer, node->data.if_statement.then_branch);

    if(node->data.if_statement.else_branch)
        visit_statement(analyzer, node->data.if_statement.else_branch);
}

static void visit_while_statement(SemanticAnalyzer *analyzer, ASTNode *node){
    if(!node || analyzer->had_internal_error) return;
    
    if(node->data.while_statement.condition)
        check_condition(analyzer, node->data.while_statement.condition, "while");

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

static const char *binary_operator_name(BinaryOperator op) {
    switch (op) {
        case OP_ADD: return "+"; case OP_SUBTRACT: return "-";
        case OP_MULTIPLY: return "*"; case OP_DIVIDE: return "/";
        case OP_LESS: return "<"; case OP_LESS_EQUAL: return "<=";
        case OP_GREATER: return ">"; case OP_GREATER_EQUAL: return ">=";
        case OP_EQUAL: return "=="; case OP_NOT_EQUAL: return "!=";
        case OP_AND: return "&&"; case OP_OR: return "||";
    }
    return "?";
}

static Type visit_binary(SemanticAnalyzer *analyzer, ASTNode *node) {
    Type left = visit_expression(analyzer, node->data.binary.left);
    Type right = visit_expression(analyzer, node->data.binary.right);
    if (left == TYPE_ERROR || right == TYPE_ERROR || analyzer->had_internal_error)
        return TYPE_ERROR;
    BinaryOperator op = node->data.binary.operator;
    int equality = op == OP_EQUAL || op == OP_NOT_EQUAL;
    int logical = op == OP_AND || op == OP_OR;
    int arithmetic = op == OP_ADD || op == OP_SUBTRACT || op == OP_MULTIPLY || op == OP_DIVIDE;
    Type expected = logical ? TYPE_BOOL : TYPE_INT;
    if ((equality && left != right) || (!equality && (left != expected || right != expected))) {
        fprintf(stderr, "Error semantico [%d:%d]: operador '%s': se esperaban %s; se encontraron %s y %s.\n",
                node->line, node->column, binary_operator_name(op),
                equality ? "operandos del mismo tipo" : (logical ? "operandos bool" : "operandos int"),
                type_to_string(left), type_to_string(right));
        analyzer->error_count++;
        return TYPE_ERROR;
    }
    return arithmetic ? TYPE_INT : TYPE_BOOL;
}

static Type visit_expression(SemanticAnalyzer *analyzer, ASTNode *node) {
    if (!node || analyzer->had_internal_error) return TYPE_ERROR;
    switch (node->type) {
        case AST_INTEGER_LITERAL: return TYPE_INT;
        case AST_BOOLEAN_LITERAL: return TYPE_BOOL;
        case AST_IDENTIFIER: {
            const char *name = node->data.identifier.name;
            Symbol *sym = symbol_table_lookup(&analyzer->symbol_table, name);
            if (!sym) {
                report_semantic_error(analyzer, node->line, node->column,
                    "la variable '%s' no ha sido declarada.", name);
                return TYPE_ERROR;
            }
            return sym->type;
        }
        case AST_BINARY_EXPRESSION: return visit_binary(analyzer, node);
        case AST_UNARY_EXPRESSION: {
            Type operand = visit_expression(analyzer, node->data.unary.operand);
            if (operand == TYPE_ERROR) return TYPE_ERROR;
            if (operand != TYPE_INT) {
                fprintf(stderr, "Error semantico [%d:%d]: operador unario '-': se esperaba int, se encontro %s.\n",
                        node->line, node->column, type_to_string(operand));
                analyzer->error_count++;
                return TYPE_ERROR;
            }
            return TYPE_INT;
        }
        default:
            semantic_report_internal_error(analyzer, "nodo de expresion inesperado.");
            return TYPE_ERROR;
    }
}

SemanticStatus semantic_analyze(SemanticAnalyzer *analyzer, ASTNode *program)
{
    if (analyzer->had_internal_error) {
        return SEMANTIC_INTERNAL_ERROR;
    }
    visit_program(analyzer, program);
    if(analyzer->had_internal_error)
        return SEMANTIC_INTERNAL_ERROR;

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
