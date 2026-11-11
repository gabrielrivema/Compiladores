/* Solo se enlaza en la prueba de inyección, nunca en minic normal. */
#include <stddef.h>
#include <stdlib.h>

void *__real_malloc(size_t size);
void *__real_realloc(void *ptr, size_t size);
char *__real_strdup(const char *s);

static int should_fail(void) {
    static unsigned long count;
    const char *value = getenv("MINIC_FAIL_ALLOC");
    unsigned long target = value ? strtoul(value, NULL, 10) : 0;
    return ++count == target;
}

void *__wrap_malloc(size_t size) {
    return should_fail() ? NULL : __real_malloc(size);
}

void *__wrap_realloc(void *ptr, size_t size) {
    return should_fail() ? NULL : __real_realloc(ptr, size);
}

char *__wrap_strdup(const char *s) {
    return should_fail() ? NULL : __real_strdup(s);
}
