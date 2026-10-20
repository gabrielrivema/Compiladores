#define _POSIX_C_SOURCE 200809L
#include "ast/ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ast_node_list_init(ASTNodeList *list) {
    if (list == NULL) {
        return;
    }

    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int ast_node_list_append(ASTNodeList *list, ASTNode *node) {
    /* TODO: validar argumentos. */
    if(list == NULL || node == NULL) return 0;
    /* TODO: crecer el arreglo sin perder el anterior si realloc falla. */
    if(list->count >= list->capacity){
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        ASTNode **new_items = realloc(list->items, new_capacity * sizeof(ASTNode *));
        if(new_items == NULL)
            return 0;
        list->items = new_items;
        list->capacity = new_capacity;
    }
    /* TODO: transferir la propiedad de node sólo cuando la inserción funcione. */
    list->items[list->count++] = node;

    return 1;
}

void ast_node_list_destroy(ASTNodeList *list) {
    /* TODO: destruir cada nodo contenido. */
    if(list==NULL) return;
    for(size_t i = 0; i < list->count; i++)
        ast_destroy(list->items[i]);

    /* TODO: liberar el arreglo interno y dejar la lista vacía. */
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static ASTNode *ast_create_node(ASTNodeType type, int line, int column){
    ASTNode *node = malloc(sizeof(ASTNode));
    if(node == NULL) return NULL;
    node->type = type;
    node->line = line;
    node->column = column;
    return node;
}

ASTNode *ast_create_program(
    ASTNodeList statements,
    int line,
    int column
) {
    /* TODO: construir la raíz y adquirir statements sólo si hay éxito. */
    ASTNode *node = ast_create_node(AST_PROGRAM, line, column);
    if(node == NULL) return NULL;
    node->data.program.statements = statements;
    return node;
}

ASTNode *ast_create_block(
    ASTNodeList statements,
    int line,
    int column
) {
    /* TODO: construir el bloque y adquirir statements sólo si hay éxito. */
    ASTNode *node = ast_create_node(AST_BLOCK, line, column);
    if(node == NULL) return NULL;
    node->data.block.statements = statements;
    return node;
}

ASTNode *ast_create_variable_declaration(
    ASTDeclaredType declared_type,
    const char *name,
    ASTNode *initializer,
    int line,
    int column
) {
    /* TODO: copiar name y aplicar el contrato de propiedad a initializer. */
    ASTNode *node = ast_create_node(AST_VARIABLE_DECLARATION, line, column);
    if(node==NULL) return NULL;

    node->data.variable_declaration.declared_type = declared_type;
    node->data.variable_declaration.name = strdup(name);
    if(node->data.variable_declaration.name == NULL){
        free(node);
        return NULL;
    }
    node->data.variable_declaration.initializer = initializer;
    return node;
}

ASTNode *ast_create_assignment(
    const char *name,
    ASTNode *value,
    int line,
    int column
) {
    /* TODO: copiar name y aplicar el contrato de propiedad a value. */
    ASTNode *node = ast_create_node(AST_ASSIGNMENT, line, column);
    if(node == NULL) return NULL;

    node->data.assignment.name = strdup(name);
    if(node->data.assignment.name == NULL){
        free(node);
        return NULL;
    }
    node->data.assignment.value = value;
    return node;
}

ASTNode *ast_create_print(
    ASTNode *expression,
    int line,
    int column
) {
    /* TODO: construir el nodo y aplicar el contrato a expression. */
    ASTNode *node = ast_create_node(AST_PRINT, line, column);
    if(node == NULL) return NULL;

    node->data.print_statement.expression = expression;
    return node;
}

ASTNode *ast_create_if(
    ASTNode *condition,
    ASTNode *then_branch,
    ASTNode *else_branch,
    int line,
    int column
) {
    /* TODO: conservar condition, then_branch y el else opcional. */
    ASTNode *node = ast_create_node(AST_IF, line, column);
    if(node == NULL) return NULL;
    node->data.if_statement.condition = condition;
    node->data.if_statement.then_branch = then_branch;
    node->data.if_statement.else_branch = else_branch;
    
    return node;
}

ASTNode *ast_create_while(
    ASTNode *condition,
    ASTNode *body,
    int line,
    int column
) {
    /* TODO: construir el ciclo con su condición y cuerpo. */
    ASTNode *node = ast_create_node(AST_WHILE, line, column);
    if(node == NULL) return NULL;
    node->data.while_statement.condition = condition;
    node->data.while_statement.body = body;
    
    return node;
}

ASTNode *ast_create_binary(
    BinaryOperator operator,
    ASTNode *left,
    ASTNode *right,
    int line,
    int column
) {
    /* TODO: construir el operador y adquirir ambos operandos sólo si hay éxito. */
    ASTNode *node = ast_create_node(AST_BINARY_EXPRESSION, line, column);
    if(node == NULL) return NULL;
    node->data.binary.operator = operator;
    node->data.binary.left = left;
    node->data.binary.right = right;
    
    return node;
}

ASTNode *ast_create_unary(
    UnaryOperator operator,
    ASTNode *operand,
    int line,
    int column
) {
    /* TODO: construir el operador y adquirir operand sólo si hay éxito. */
    ASTNode *node = ast_create_node(AST_UNARY_EXPRESSION, line, column);
    if(node == NULL) return NULL;
    node->data.unary.operator = operator;
    node->data.unary.operand = operand;
    
    return node;
}

ASTNode *ast_create_identifier(
    const char *name,
    int line,
    int column
) {
    /* TODO: copiar name; no conservar el apuntador del token. */
    ASTNode *node = ast_create_node(AST_IDENTIFIER, line, column);
    if(node == NULL) return NULL;
    node->data.identifier.name = strdup(name);
    if(node->data.identifier.name == NULL){
        free(node);
        return NULL;
    }
    
    return node;
}

ASTNode *ast_create_integer(
    const char *lexeme,
    int line,
    int column
) {
    /* TODO: copiar el lexema completo del entero. */
    ASTNode *node = ast_create_node(AST_INTEGER_LITERAL, line, column);
    if(node == NULL) return NULL;
    node->data.integer_literal.lexeme = strdup(lexeme);
    if(node->data.integer_literal.lexeme == NULL){
        free(node);
        return NULL;
    }
    
    return node;
}

ASTNode *ast_create_boolean(
    int value,
    int line,
    int column
) {
    /* TODO: normalizar y conservar el valor booleano. */
    ASTNode *node = ast_create_node(AST_BOOLEAN_LITERAL, line, column);
    if(node == NULL) return NULL;
    node->data.boolean_literal.value = value;
    
    return node;
}

//Funciones auxiliares para imprimir
static void print_space(int depth){
    for(int i = 0; i < depth; i++)
        printf("  ");
}

static void ast_print_indented(const ASTNode *node, int depth){
    if(node == NULL) return;

    print_space(depth);

    switch (node->type){
    case AST_PROGRAM:
        printf("Program\n");
        for(size_t i = 0; i < node->data.program.statements.count; i++)
            ast_print_indented(node->data.program.statements.items[i], depth+1);
        break;
    case AST_BLOCK:
        printf("Block\n");
        for(size_t i = 0; i < node->data.block.statements.count; i++)
            ast_print_indented(node->data.block.statements.items[i], depth+1);
        break;
    case AST_VARIABLE_DECLARATION:
        if(node->data.variable_declaration.declared_type == AST_TYPE_INT)
            printf("VariableDeclaration(int, %s)\n", node->data.variable_declaration.name);
        else
            printf("VariableDeclaration(bool, %s)\n", node->data.variable_declaration.name);
        if(node->data.variable_declaration.initializer != NULL)
            ast_print_indented(node->data.variable_declaration.initializer, depth+1);
        break;
    case AST_ASSIGNMENT:
        printf("Assignment(%s)\n", node->data.assignment.name);
        ast_print_indented(node->data.assignment.value, depth+1);
        break;
    case AST_PRINT:
        printf("Print\n");
        ast_print_indented(node->data.print_statement.expression, depth+1);
        break;
    case AST_IF:
        printf("If\n");
        print_space(depth+1);
        printf("Condition\n");
        ast_print_indented(node->data.if_statement.condition, depth+2);
        print_space(depth+1);
        printf("Then\n");
        ast_print_indented(node->data.if_statement.then_branch, depth+2);
        if(node->data.if_statement.else_branch != NULL){
            print_space(depth+1);
            printf("Else\n");
            ast_print_indented(node->data.if_statement.else_branch, depth+2);
        }
        break;
    case AST_WHILE:
        printf("While\n");
        print_space(depth+1);
        printf("Condition\n");
        ast_print_indented(node->data.while_statement.condition, depth+2);
        print_space(depth+1);
        printf("Body\n");
        ast_print_indented(node->data.while_statement.body, depth+2);
        break;
    case AST_BINARY_EXPRESSION:
        switch(node->data.binary.operator){
            case OP_ADD:printf("Add\n"); break;
            case OP_SUBTRACT:printf("Subtract\n"); break;
            case OP_MULTIPLY:printf("Multiply\n"); break;
            case OP_DIVIDE:printf("Divide\n"); break;
            case OP_LESS:printf("Less\n"); break;
            case OP_LESS_EQUAL:printf("LessEqual\n"); break;
            case OP_GREATER:printf("Greater\n"); break;
            case OP_GREATER_EQUAL:printf("GreaterEqual\n"); break;
            case OP_EQUAL:printf("Equal\n"); break;
            case OP_NOT_EQUAL:printf("NotEqual\n"); break;
            case OP_AND:printf("And\n"); break;
            case OP_OR:printf("Or\n"); break;
            default: break;
        }
        ast_print_indented(node->data.binary.left, depth+1);
        ast_print_indented(node->data.binary.right, depth+1);
        break;
    case AST_UNARY_EXPRESSION:
        switch(node->data.unary.operator){
            case OP_NEGATE: printf("Negate\n"); break;
            default: break;
        }
        ast_print_indented(node->data.unary.operand, depth+1);
        break;
    case AST_IDENTIFIER:
        printf("Identifier(%s)\n", node->data.identifier.name);
        break;
    case AST_INTEGER_LITERAL:
        printf("Integer(%s)\n", node->data.integer_literal.lexeme);
        break;
    case AST_BOOLEAN_LITERAL:
        if(node->data.boolean_literal.value)
            printf("Boolean(true)\n");
        else
            printf("Boolean(false)\n");
        break;
    default:
        break;
    }
}

void ast_print(const ASTNode *node) {
    /* TODO: recorrer node con el formato canónico publicado. */
    if(node==NULL) return;
    ast_print_indented(node, 0);
}

void ast_destroy(ASTNode *node) {
    /* TODO: aceptar NULL y liberar recursivamente según node->type. */
    if(node == NULL) return;

    switch (node->type){
        case AST_PROGRAM:
            ast_node_list_destroy(&node->data.program.statements);
            break;
        case AST_BLOCK:
            ast_node_list_destroy(&node->data.block.statements);
            break;
        case AST_VARIABLE_DECLARATION:
            free(node->data.variable_declaration.name);
            ast_destroy(node->data.variable_declaration.initializer);
            break;
        case AST_ASSIGNMENT:
            free(node->data.assignment.name);
            ast_destroy(node->data.assignment.value);
            break;
        case AST_PRINT:
            ast_destroy(node->data.print_statement.expression);
            break;
        case AST_IF:
            ast_destroy(node->data.if_statement.condition);
            ast_destroy(node->data.if_statement.then_branch);
            ast_destroy(node->data.if_statement.else_branch);
            break;
        case AST_WHILE:
            ast_destroy(node->data.while_statement.condition);
            ast_destroy(node->data.while_statement.body);
            break;
        case AST_BINARY_EXPRESSION:
            ast_destroy(node->data.binary.left);
            ast_destroy(node->data.binary.right);
            break;
        case AST_UNARY_EXPRESSION:
            ast_destroy(node->data.unary.operand);
            break;
        case AST_IDENTIFIER:
            free(node->data.identifier.name);
            break;
        case AST_INTEGER_LITERAL:
            free(node->data.integer_literal.lexeme);
            break;
        case AST_BOOLEAN_LITERAL:
            break;
        default:
            break;
    }
    free(node);
}

