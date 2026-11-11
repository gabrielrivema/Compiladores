#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
tmp=$(mktemp -d)
trap 'rm -f "$tmp/minic" "$tmp/stdout" "$tmp/stderr" "$tmp/valgrind"; rmdir "$tmp"' EXIT
cd "$root"
${CC:-gcc} -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude \
    src/main.c src/lexer/lexer.c src/lexer/token.c src/parser/parser.c \
    src/ast/ast.c src/semantic/symbol.c src/semantic/scope.c \
    src/semantic/symbol_table.c src/semantic/semantic.c tests/memoria/fail_alloc.c \
    -Wl,--wrap=malloc -Wl,--wrap=realloc -Wl,--wrap=strdup -o "$tmp/minic"
for index in $(seq 1 1000); do
    status=0
    MINIC_FAIL_ALLOC=$index valgrind --quiet --leak-check=full \
        --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=99 \
        --log-file="$tmp/valgrind" "$tmp/minic" tests/tipos/inputs/valido.mc \
        >"$tmp/stdout" 2>"$tmp/stderr" || status=$?
    if [ "$status" -eq 99 ] || [ -s "$tmp/valgrind" ]; then
        printf 'Error de memoria en reserva %s\n' "$index"; cat "$tmp/valgrind"; exit 1
    fi
    if [ "$status" -eq 0 ]; then
        if [ -s "$tmp/stderr" ] || ! diff -u tests/public/expected/programa_correcto.out "$tmp/stdout"; then
            printf 'Resultado incorrecto al terminar las reservas.\n' >&2
            exit 1
        fi
        printf 'Comprobadas %s reservas fallidas sin errores de memoria.\n' "$((index - 1))"
        exit 0
    fi
    if [ "$status" -gt 2 ] || [ -s "$tmp/stdout" ] || ! grep -q 'Error interno' "$tmp/stderr"; then
        printf 'Resultado inesperado en reserva %s (codigo %s)\n' "$index" "$status"
        cat "$tmp/stdout" "$tmp/stderr"
        exit 1
    fi
    if grep -Eq 'fallo de lectura|Error (lexico|sintactico|semantico)' "$tmp/stderr"; then
        printf 'El fallo de reserva produjo una categoria incorrecta en %s\n' "$index"
        cat "$tmp/stderr"
        exit 1
    fi
done
printf 'No se llegó al final de las reservas.\n' >&2
exit 1
