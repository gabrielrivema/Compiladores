#!/usr/bin/env bash
set -u
set -o pipefail
base="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)" || exit 2
exe="${1:-./minic}"
if [[ ! -f "$exe" || ! -x "$exe" ]]; then
    printf 'No es un ejecutable accesible: %s\n' "$exe" >&2
    exit 2
fi
exe="$(cd -- "$(dirname -- "$exe")" && pwd)/$(basename -- "$exe")"
for command in timeout cmp mktemp; do
    command -v "$command" >/dev/null || { printf 'Falta %s\n' "$command" >&2; exit 2; }
done
limit="${TIMEOUT_S:-5}"
[[ "$limit" =~ ^[1-9][0-9]*$ ]] || { printf 'TIMEOUT_S debe ser entero positivo.\n' >&2; exit 2; }
tmp="$(mktemp -d)" || exit 2
trap 'rm -f -- "$tmp/out" "$tmp/err" "$tmp/status"; rmdir -- "$tmp"' EXIT
passed=0
failed=0
while read -r id kind; do
    [[ -z "$id" || "$id" == \#* ]] && continue
    rm -f -- "$tmp/status"
    # El archivo status distingue un exit 124 del alumno de timeout.
    timeout -k 1s "${limit}s" bash -c '
        "$1" "$2" >"$3/out" 2>"$3/err"
        code=$?
        printf "%s\n" "$code" >"$3/status"
    ' _ "$exe" "$base/casos/$id.mc" "$tmp"
    if [[ ! -s "$tmp/status" ]]; then
        printf '[FALLO EJECUCION] %s: no termino dentro del limite o no pudo lanzarse.\n' "$id"
        failed=$((failed + 1))
        continue
    fi
    read -r code < "$tmp/status"
    ok=0
    if [[ "$kind" == valid ]]; then
        if [[ "$code" == 0 && ! -s "$tmp/err" ]] && cmp -s "$tmp/out" "$base/esperados/programa_correcto.out"; then ok=1; fi
    elif [[ "$kind" == invalid ]]; then
        if [[ "$code" != 0 && ! -s "$tmp/out" && -s "$tmp/err" ]]; then ok=1; fi
        printf '\nDiagnosticos de %s (revisar CASOS.md):\n' "$id"
        cat -- "$tmp/err"
    fi
    if [[ "$ok" == 1 ]]; then
        printf '[CONTRATO OK] %s\n' "$id"; passed=$((passed + 1))
    else
        printf '[FALLO] %s (salida %s)\n' "$id" "$code"; failed=$((failed + 1))
    fi
done < "$base/cases.txt"
printf '\nContratos: %s correctos, %s fallidos. Diagnosticos pendientes de revision humana.\n' "$passed" "$failed"
(( failed == 0 ))
