#!/bin/sh

set -u

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
executable=${1:-./minic}

if [ ! -x "$executable" ]; then
    printf 'No se encontró un ejecutable en: %s\n' "$executable" >&2
    printf 'Compile el proyecto o indique la ruta como primer argumento.\n' >&2
    exit 2
fi

tmp_dir=$(mktemp -d 2>/dev/null || mktemp -d -t minic-p2)
trap 'rm -rf "$tmp_dir"' EXIT HUP INT TERM

passed=0
failed=0

for input in "$script_dir"/inputs/*.mc; do
    name=$(basename "$input" .mc)
    expected="$script_dir/expected/$name.out"
    actual="$tmp_dir/$name.out"

    if [ ! -f "$expected" ]; then
        printf '[ERROR] Falta la salida esperada para %s\n' "$name"
        failed=$((failed + 1))
        continue
    fi

    "$executable" "$input" > "$actual"
    status=$?

    if [ "$status" -ne 0 ]; then
        printf '[AVISO] %s terminó con código %s; la práctica no define ese código como parte de la salida.\n' "$name" "$status"
    fi

    if diff -u "$expected" "$actual"; then
        printf '[OK] %s\n' "$name"
        passed=$((passed + 1))
    else
        printf '[FALLO] %s\n' "$name"
        failed=$((failed + 1))
    fi
done

printf '\nAprobadas: %s\n' "$passed"
printf 'Fallidas: %s\n' "$failed"

if [ "$failed" -ne 0 ]; then
    exit 1
fi

exit 0

