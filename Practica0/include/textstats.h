#ifndef TEXTSTATS_H
#define TEXTSTATS_H

#include <stddef.h>

typedef enum {
    CHAR_LETTER,
    CHAR_DIGIT,
    CHAR_SPACE,
    CHAR_OTHER
} CharType;

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} TextBuffer;

typedef struct {
    size_t characters;
    size_t letters;
    size_t digits;
    size_t spaces;
    size_t others;
    TextBuffer content;
} TextStats;

size_t text_length(const char *text);
CharType classify_char(int c);
int buffer_init(TextBuffer *buffer, size_t initial_capacity);
int buffer_append(TextBuffer *buffer, char c);
void buffer_free(TextBuffer *buffer);
int stats_init(TextStats *stats);
void stats_destroy(TextStats *stats);
int analyze_file(const char *path, TextStats *stats);
void print_stats(const char *path, const TextStats *stats);

#endif
