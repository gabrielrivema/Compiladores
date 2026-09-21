#!/bin/sh

set -u

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
executable=${1:-./minic}

if [ ! -x "$executable" ]; then
    printf 'No se encontró un ejecutable en: %s\n' "$executable" >&2
    printf 'Compile el proyecto o indique la ruta como primer argumento.\n' >&2
    exit 2
fi

tmp_dir=$(mktemp -d 2>/dev/null || mktemp -d -t minic-p3)
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
    expected="$script_dir/expected/programa_correcto.out"
    stdout_file="$tmp_dir/$name.stdout"
    stderr_file="$tmp_dir/$name.stderr"

    "$executable" "$input" >"$stdout_file" 2>"$stderr_file"
    status=$?

    if [ "$status" -ne 0 ]; then
        fail_case "$name" "un programa válido terminó con código $status"
    elif ! diff -u "$expected" "$stdout_file"; then
        fail_case "$name" "la salida estándar no coincide con el mensaje publicado"
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
    elif [ "$kind" = multiple ] && [ "$(grep -Ec '^Error sintactico \[[0-9]+:[0-9]+\]:' "$stderr_file")" -lt 2 ]; then
        fail_case "$name" "no se observaron diagnósticos para los dos errores independientes"
    else
        pass_case "$name"
    fi
}

for name in \
    p01_empty \
    p02_declarations \
    p03_assignment_print \
    p04_expression_precedence \
    p05_if_else \
    p06_dangling_else \
    p07_while_blocks \
    p08_complete_program
do
    run_valid "$name"
done

for name in \
    p09_missing_semicolon \
    p10_missing_parenthesis \
    p11_missing_brace \
    p12_incomplete_expression \
    p13_operator_without_operand \
    p14_unexpected_token \
    p17_trailing_content
do
    run_invalid "$name" syntax
done

run_invalid p15_multiple_errors multiple
run_invalid p16_lexical_error lexical

printf '\nAprobadas: %s\n' "$passed"
printf 'Fallidas: %s\n' "$failed"

if [ "$failed" -ne 0 ]; then
    exit 1
fi

exit 0

