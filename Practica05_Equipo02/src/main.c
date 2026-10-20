#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "semantic/semantic.h"

#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s programa.mc\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: no se pudo abrir '%s'.\n", argv[1]);
        return 2;
    }

    Lexer lexer;
    if(!lexer_init(&lexer, file)){
        fprintf(stderr, "Error: no se pudo iniciar el lexer.\n");
        fclose(file);
        return 2;
    }
    
    Parser parser;
    if(!parser_init(&parser, &lexer)){
        fprintf(stderr, "Error: no se pudo iniciar el parser.\n");
        lexer_destroy(&lexer);
        fclose(file);
        return 2;
    }

    ASTNode *ast_main = parse_program(&parser);

    int has_error = parser.had_error;

    parser_destroy(&parser);
    lexer_destroy(&lexer);
    
    if (fclose(file) != 0) {
        fprintf(stderr, "Error: no se pudo cerrar '%s'.\n", argv[1]);
        return 2;
    }

    if(has_error || ast_main==NULL){
        if(ast_main != NULL)
            ast_destroy(ast_main);
        return 1;
    }

    SemanticAnalyzer analyzer;
    if(semantic_analyzer_init(&analyzer) != SEMANTIC_OK){
        ast_destroy(ast_main);
        return 1;
    }

    SemanticStatus sem_status = semantic_analyze(&analyzer, ast_main);

    int return_value = 0;

    if(sem_status == SEMANTIC_OK){
        printf("Programa semanticamente correcto.\n");
    }else{
        return_value = 1;
    }

    semantic_analyzer_destroy(&analyzer);
    ast_destroy(ast_main);

    return return_value;
}
