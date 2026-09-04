#include "textstats.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t text_length(const char *text) {
    /* TODO 1: recorrer text hasta encontrar '\0'. */
    (void)text;
    return 0;
}

CharType classify_char(int c) {
    /* TODO 2: clasificar letras ASCII, dígitos, blancos y otros. */
    (void)c;
    return CHAR_OTHER;
}

int buffer_init(TextBuffer *buffer, size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 1;
    }

    buffer->data = malloc(initial_capacity);
    if (buffer->data == NULL) {
        buffer->length = 0;
        buffer->capacity = 0;
        return 0;
    }

    buffer->data[0] = '\0';
    buffer->length = 0;
    buffer->capacity = initial_capacity;
    return 1;
}

int buffer_append(TextBuffer *buffer, char c) {
    if (buffer->length + 2 > buffer->capacity) {
        size_t new_capacity = 0;
        char *new_data = NULL;

        /*
         * TODO 3:
         * - calcular una capacidad mayor;
         * - verificar que el cálculo no desborde SIZE_MAX;
         * - llamar realloc mediante el puntero temporal new_data;
         * - actualizar data y capacity solamente si tiene éxito.
         */
        (void)new_capacity;
        (void)new_data;
        return 0;
    }

    buffer->data[buffer->length] = c;
    buffer->length++;
    buffer->data[buffer->length] = '\0';
    return 1;
}

void buffer_free(TextBuffer *buffer) {
    free(buffer->data);
    buffer->data = NULL;
    buffer->length = 0;
    buffer->capacity = 0;
}

int stats_init(TextStats *stats) {
    stats->characters = 0;
    stats->letters = 0;
    stats->digits = 0;
    stats->spaces = 0;
    stats->others = 0;
    return buffer_init(&stats->content, 8);
}

void stats_destroy(TextStats *stats) {
    buffer_free(&stats->content);
}

static void update_counter(TextStats *stats, CharType type) {
    switch (type) {
        case CHAR_LETTER:
            stats->letters++;
            break;
        case CHAR_DIGIT:
            stats->digits++;
            break;
        case CHAR_SPACE:
            stats->spaces++;
            break;
        case CHAR_OTHER:
            stats->others++;
            break;
    }
}

int analyze_file(const char *path, TextStats *stats) {
    FILE *file = fopen(path, "rb");
    int c;

    if (file == NULL) {
        fprintf(stderr, "Error: no se pudo abrir '%s'.\n", path);
        return 2;
    }

    /*
     * TODO 4:
     * mientras fgetc no devuelva EOF:
     * - incrementar characters;
     * - clasificar el byte y actualizar su contador;
     * - agregarlo al buffer;
     * - manejar un fallo de buffer_append.
    */
    (void)c;
    (void)stats;

    if (ferror(file)) {
        fprintf(stderr, "Error: no se pudo leer '%s'.\n", path);
        fclose(file);
        return 2;
    }

    fclose(file);
    return 0;
}

void print_stats(const char *path, const TextStats *stats) {
    printf("=== TextStats de clase ===\n");
    printf("Archivo: %s\n", path);
    printf("Caracteres: %zu\n", stats->characters);
    printf("Letras: %zu\n", stats->letters);
    printf("Dígitos: %zu\n", stats->digits);
    printf("Blancos: %zu\n", stats->spaces);
    printf("Otros: %zu\n", stats->others);
    printf("Contenido (%zu):\n%s", text_length(stats->content.data), stats->content.data);
}
