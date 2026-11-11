#!/bin/sh

set -u
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd) || exit 2
executable=${1:-./minic}

if [ ! -f "$executable" ] || [ ! -x "$executable" ]; then
    printf 'No se encontro un ejecutable en: %s\n' "$executable" >&2
    printf 'Compile su proyecto e indique la ruta, por ejemplo ./minic.\n' >&2
    exit 2
fi
for command_name in diff mktemp; do
    if ! command -v "$command_name" >/dev/null 2>&1; then
        printf 'Falta la herramienta: %s\n' "$command_name" >&2
        exit 2
    fi
done
tmp_dir=$(mktemp -d "${TMPDIR:-/tmp}/minic-p5.XXXXXX") || exit 2
cleanup() {
    rm -f -- "$tmp_dir/stdout" "$tmp_dir/stderr" "$tmp_dir/status"
    rmdir -- "$tmp_dir"
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM HUP
passed=0
failed=0
manual=0

while read -r name kind; do
    [ -n "$name" ] || continue
    input="$script_dir/inputs/$name.mc"
    if [ ! -f "$input" ]; then
        printf 'Falta la entrada: %s\n' "$input" >&2
        exit 2
    fi
    case "$kind" in valid|invalid) ;; *) exit 2 ;; esac
    rm -f -- "$tmp_dir/status"
    if command -v timeout >/dev/null 2>&1; then
        timeout -k 2 "${P5_TEST_TIMEOUT:-10}" sh -c \
            '"$1" "$2"; result=$?; printf "%s\n" "$result" >"$3"' \
            sh "$executable" "$input" "$tmp_dir/status" \
            >"$tmp_dir/stdout" 2>"$tmp_dir/stderr"
    else
        "$executable" "$input" >"$tmp_dir/stdout" 2>"$tmp_dir/stderr"
        printf '%s\n' "$?" >"$tmp_dir/status"
    fi
    reason=
    status=0
    if [ ! -s "$tmp_dir/status" ]; then
        reason='ejecucion interrumpida o timeout; no se obtuvo un resultado'
    else
        read -r status < "$tmp_dir/status"
    fi
    if [ -n "$reason" ]; then
        :
    elif [ "$kind" = valid ]; then
        if [ "$status" -ne 0 ]; then
            reason="entrada valida para P05 con codigo $status"
        elif ! diff -u "$script_dir/expected/programa_correcto.out" "$tmp_dir/stdout"; then
            reason='stdout no coincide con el mensaje publicado'
        elif [ -s "$tmp_dir/stderr" ]; then
            reason='entrada valida con contenido en stderr'
        fi
    else
        if [ "$status" -eq 0 ]; then
            reason='entrada invalida con codigo 0'
        elif [ -s "$tmp_dir/stdout" ]; then
            reason='entrada invalida con contenido en stdout'
        elif [ ! -s "$tmp_dir/stderr" ]; then
            reason='entrada invalida sin diagnosticos'
        fi
    fi
    if [ -n "$reason" ]; then
        printf '[FALLO] %s: %s\n' "$name" "$reason"
        failed=$((failed + 1))
    else
        printf '[OK automatico] %s\n' "$name"
        passed=$((passed + 1))
    fi
    if [ "$kind" = invalid ]; then
        manual=$((manual + 1))
        printf '  REVISAR diagnosticos contra expected/CASOS.md:\n'
    fi
    if [ -s "$tmp_dir/stderr" ]; then
        cat "$tmp_dir/stderr"
        printf '\n'
    fi
done < "$script_dir/cases.txt"

printf '\nComprobaciones automaticas: %s aprobadas, %s fallidas.\n' "$passed" "$failed"
printf 'Casos que requieren revisar diagnosticos: %s. No es una calificacion.\n' "$manual"
[ "$failed" -eq 0 ] || exit 1
exit 0
