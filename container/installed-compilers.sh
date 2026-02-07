#!/bin/sh
set -eu

out=""
for cc in gcc clang tcc; do
	if command -v "$cc" >/dev/null 2>&1; then
		out="${out}${out:+ }$cc"
	fi
done

echo "$out"

