#include "textstats.h"

#include <stdio.h>

int main(int argc, char **argv) {
    TextStats stats;
    int result;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivo.txt\n", argv[0]);
        return 1;
    }

    if (!stats_init(&stats)) {
        fprintf(stderr, "Error: no se pudo reservar memoria.\n");
        return 2;
    }

    result = analyze_file(argv[1], &stats);
    if (result == 0) {
        print_stats(argv[1], &stats);
    }

    stats_destroy(&stats);
    return result;
}
