#include "lexer/lexer.h"

#include <stdio.h>

int main(int argc, char **argv) {
    FILE *file;
    int result;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s programa.mc\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: no se pudo abrir '%s'.\n", argv[1]);
        return 2;
    }

    result = lexer_scan(file);

    if (fclose(file) != 0 && result == 0) {
        fprintf(stderr, "Error: no se pudo cerrar '%s'.\n", argv[1]);
        return 2;
    }

    return result;
}
