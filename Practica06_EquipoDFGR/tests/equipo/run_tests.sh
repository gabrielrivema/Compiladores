#!/bin/sh
set -eu
base=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exe=${1:-./minic}
tmp=$(mktemp -d)
trap 'rm -f "$tmp/stdout" "$tmp/stderr"; rmdir "$tmp"' EXIT
failed=0
for input in "$base"/inputs/*.mc; do
    name=$(basename "$input" .mc)
    expected="$base/expected/$name.out"
    status=0
    "$exe" "$input" >"$tmp/stdout" 2>"$tmp/stderr" || status=$?
    ok=1
    case "$name" in
        p3|p4|p9)
            [ "$status" -eq 1 ] && [ ! -s "$tmp/stdout" ] || ok=0
            diff -u "$expected" "$tmp/stderr" || ok=0
            ;;
        *)
            [ "$status" -eq 0 ] && [ ! -s "$tmp/stderr" ] || ok=0
            diff -u "$expected" "$tmp/stdout" || ok=0
            ;;
    esac
    if [ "$ok" -eq 1 ]; then printf '[OK] %s\n' "$name";
    else printf '[FALLO] %s (codigo %s)\n' "$name" "$status"; failed=1; fi
done
exit "$failed"
