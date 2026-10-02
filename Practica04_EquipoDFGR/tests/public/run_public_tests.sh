#!/bin/sh

set -u

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
executable=${1:-./minic}

if [ ! -x "$executable" ]; then
    printf 'No se encontró un ejecutable en: %s\n' "$executable" >&2
    printf 'Compile el proyecto o indique la ruta como primer argumento.\n' >&2
    exit 2
fi

tmp_dir=$(mktemp -d 2>/dev/null || mktemp -d -t minic-p4)
trap 'rm -rf "$tmp_dir"' EXIT HUP INT TERM

passed=0
failed=0

pass_case() {
    printf '[OK] %s\n' "$1"
    passed=$((passed + 1))
}

fail_case() {
    printf '[FALLO] %s: %s\n' "$1" "$2"
    failed=$((failed + 1))
}

run_valid() {
    name=$1
    input="$script_dir/inputs/$name.mc"
    expected="$script_dir/expected/$name.out"
    stdout_file="$tmp_dir/$name.stdout"
    stderr_file="$tmp_dir/$name.stderr"

    "$executable" "$input" >"$stdout_file" 2>"$stderr_file"
    status=$?

    if [ "$status" -ne 0 ]; then
        fail_case "$name" "un programa válido terminó con código $status"
    elif ! diff -u "$expected" "$stdout_file"; then
        fail_case "$name" "stdout no coincide con el AST canónico esperado"
    elif [ -s "$stderr_file" ]; then
        fail_case "$name" "un programa válido produjo salida en stderr"
    else
        pass_case "$name"
    fi
}

run_invalid() {
    name=$1
    kind=$2
    input="$script_dir/inputs/$name.mc"
    stdout_file="$tmp_dir/$name.stdout"
    stderr_file="$tmp_dir/$name.stderr"

    "$executable" "$input" >"$stdout_file" 2>"$stderr_file"
    status=$?

    if [ "$status" -eq 0 ]; then
        fail_case "$name" "un programa inválido terminó con código 0"
    elif [ -s "$stdout_file" ]; then
        fail_case "$name" "un programa inválido produjo salida en stdout"
    elif [ ! -s "$stderr_file" ]; then
        fail_case "$name" "no se produjo ningún diagnóstico en stderr"
    elif [ "$kind" = syntax ] && ! grep -Eq '^Error sintactico \[[0-9]+:[0-9]+\]:' "$stderr_file"; then
        fail_case "$name" "falta un diagnóstico sintáctico con el formato general publicado"
    elif [ "$kind" = lexical ] && grep -Eq '^Error sintactico ' "$stderr_file"; then
        fail_case "$name" "el único token ERROR produjo además un diagnóstico sintáctico"
    else
        pass_case "$name"
    fi
}

for name in \
    p01_empty \
    p02_declarations \
    p03_initializers \
    p04_assignment_print \
    p05_precedence \
    p06_unary_parentheses \
    p07_if_else \
    p08_while_block \
    p09_nested_blocks \
    p10_complete_program
do
    run_valid "$name"
done

run_invalid p11_syntax_error syntax
run_invalid p12_lexical_error lexical

printf '\nAprobadas: %s\n' "$passed"
printf 'Fallidas: %s\n' "$failed"

if [ "$failed" -ne 0 ]; then
    exit 1
fi

exit 0

