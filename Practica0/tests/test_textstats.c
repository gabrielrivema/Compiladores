#include "textstats.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "FALLO: %s\n", message);
        failures++;
    }
}

int main(void) {
    TextBuffer buffer;

    check(text_length("") == 0, "cadena vacía");
    check(text_length("lexer") == 5, "longitud de lexer");
    check(classify_char('A') == CHAR_LETTER, "clasificar letra");
    check(classify_char('7') == CHAR_DIGIT, "clasificar dígito");
    check(classify_char('\n') == CHAR_SPACE, "clasificar blanco");
    check(classify_char('@') == CHAR_OTHER, "clasificar otro");

    check(buffer_init(&buffer, 2), "inicializar buffer");
    if (buffer.data != NULL) {
        check(buffer_append(&buffer, 'a'), "append a");
        check(buffer_append(&buffer, 'b'), "append y crecimiento");
        check(buffer_append(&buffer, 'c'), "segundo append");
        check(strcmp(buffer.data, "abc") == 0, "contenido conservado");
        check(buffer.length == 3, "longitud del buffer");
        buffer_free(&buffer);
    }

    if (failures == 0) {
        puts("repaso: OK");
        return 0;
    }
    return 1;
}
