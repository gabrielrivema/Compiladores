#include "textstats.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

size_t text_length(const char *text) {
    /* TODO 1: recorrer text hasta encontrar '\0'. */
    size_t length = 0;

    if (text == NULL)
        return 0;

    //Lee hasta encontrar '\0'
    while (text[length] != '\0')
        length++;

    return length;
}

CharType classify_char(int c) {
    /* TODO 2: clasificar letras ASCII, dígitos, blancos y otros. */
    
    if((c >= 'A' && c<='Z') || (c >= 'a' && c<='z'))
        return CHAR_LETTER;
    
    if (c >= '0' && c <= '9')
        return CHAR_DIGIT;

    if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        return CHAR_SPACE;

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
         * - verificar que el cálculo no desborde SIZE_MAX;
         */
        if (buffer->capacity > SIZE_MAX / 2){
            if (buffer->capacity > SIZE_MAX - 2)
                return 0;
        } else {
            // * - calcular una capacidad mayor;
            new_capacity = buffer->capacity * 2;
            if (new_capacity < buffer->length + 2)
                new_capacity = buffer->length + 2;
        }
        
        // * - llamar realloc mediante el puntero temporal new_data;
        new_data = realloc(buffer->data, new_capacity);
        if (new_data == NULL)
            return 0;
        // * - actualizar data y capacity solamente si tiene éxito.
        buffer->data = new_data;
        buffer->capacity = new_capacity;

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
     * mientras fgetc no devuelva EOF:*/
    while ((c=fgetc(file)) != EOF){
        // * - incrementar characters;
        stats->characters++;        

        // * - clasificar el byte y actualizar su contador;
        CharType type = classify_char(c);
        update_counter(stats, type);

        // * - agregarlo al buffer;
        // * - manejar un fallo de buffer_append.
        if (!buffer_append(&stats->content, (char)c)){
            fprintf(stderr, "Error: No se pudo reservar la memoria");
            fclose(file);
            return 2;
        }
    }

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
