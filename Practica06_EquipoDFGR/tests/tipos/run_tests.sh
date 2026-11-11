#!/bin/sh
set -eu
base=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exe=${1:-./minic}
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
failed=0
for input in "$base"/inputs/*.mc; do
    name=$(basename "$input" .mc)
    status=0
    "$exe" "$input" >"$tmp/stdout" 2>"$tmp/stderr" || status=$?
    printf '%s\n' "$status" >"$tmp/status"
    ok=1
    for stream in stdout stderr status; do
        if ! diff -u "$base/expected/$name.$stream" "$tmp/$stream"; then ok=0; fi
    done
    if [ "$ok" -eq 1 ]; then printf '[OK] %s\n' "$name"; else printf '[FALLO] %s\n' "$name"; failed=1; fi
done
exit "$failed"
