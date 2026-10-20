#ifndef MINIC_AST_H
#define MINIC_AST_H

#include <stddef.h>

typedef struct ASTNode ASTNode;

typedef enum {
    AST_PROGRAM,
    AST_BLOCK,
    AST_VARIABLE_DECLARATION,
    AST_ASSIGNMENT,
    AST_PRINT,
    AST_IF,
    AST_WHILE,
    AST_BINARY_EXPRESSION,
    AST_UNARY_EXPRESSION,
    AST_IDENTIFIER,
    AST_INTEGER_LITERAL,
    AST_BOOLEAN_LITERAL
} ASTNodeType;

typedef enum {
    AST_TYPE_INT,
    AST_TYPE_BOOL
} ASTDeclaredType;

typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_AND,
    OP_OR
} BinaryOperator;

typedef enum {
    OP_NEGATE
} UnaryOperator;

typedef struct {
    ASTNode **items;
    size_t count;
    size_t capacity;
} ASTNodeList;

struct ASTNode {
    ASTNodeType type;
    int line;
    int column;

    union {
        struct {
            ASTNodeList statements;
        } program;

        struct {
            ASTNodeList statements;
        } block;

        struct {
            ASTDeclaredType declared_type;
            char *name;
            ASTNode *initializer;
        } variable_declaration;

        struct {
            char *name;
            ASTNode *value;
        } assignment;

        struct {
            ASTNode *expression;
        } print_statement;

        struct {
            ASTNode *condition;
            ASTNode *then_branch;
            ASTNode *else_branch;
        } if_statement;

        struct {
            ASTNode *condition;
            ASTNode *body;
        } while_statement;

        struct {
            BinaryOperator operator;
            ASTNode *left;
            ASTNode *right;
        } binary;

        struct {
            UnaryOperator operator;
            ASTNode *operand;
        } unary;

        struct {
            char *name;
        } identifier;

        struct {
            char *lexeme;
        } integer_literal;

        struct {
            int value;
        } boolean_literal;
    } data;
};

void ast_node_list_init(ASTNodeList *list);
int ast_node_list_append(ASTNodeList *list, ASTNode *node);
void ast_node_list_destroy(ASTNodeList *list);

ASTNode *ast_create_program(
    ASTNodeList statements,
    int line,
    int column
);

ASTNode *ast_create_block(
    ASTNodeList statements,
    int line,
    int column
);

ASTNode *ast_create_variable_declaration(
    ASTDeclaredType declared_type,
    const char *name,
    ASTNode *initializer,
    int line,
    int column
);

ASTNode *ast_create_assignment(
    const char *name,
    ASTNode *value,
    int line,
    int column
);

ASTNode *ast_create_print(
    ASTNode *expression,
    int line,
    int column
);

ASTNode *ast_create_if(
    ASTNode *condition,
    ASTNode *then_branch,
    ASTNode *else_branch,
    int line,
    int column
);

ASTNode *ast_create_while(
    ASTNode *condition,
    ASTNode *body,
    int line,
    int column
);

ASTNode *ast_create_binary(
    BinaryOperator operator,
    ASTNode *left,
    ASTNode *right,
    int line,
    int column
);

ASTNode *ast_create_unary(
    UnaryOperator operator,
    ASTNode *operand,
    int line,
    int column
);

ASTNode *ast_create_identifier(
    const char *name,
    int line,
    int column
);

ASTNode *ast_create_integer(
    const char *lexeme,
    int line,
    int column
);

ASTNode *ast_create_boolean(
    int value,
    int line,
    int column
);

void ast_print(const ASTNode *node);
void ast_destroy(ASTNode *node);

#endif

