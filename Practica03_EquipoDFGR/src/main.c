#include "lexer/lexer.h"
#include "parser/parser.h"

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

    int success = parse_program(&parser);
    parser_destroy(&parser);
    lexer_destroy(&lexer);
    
    if (fclose(file) != 0) {
        fprintf(stderr, "Error: no se pudo cerrar '%s'.\n", argv[1]);
        return 2;
    }

    if(success){
        printf("Programa sintacticamente correcto.\n");
        return 0;
    }

    

    return 1;
}
