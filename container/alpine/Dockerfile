FROM alpine:3.20

# Minimal: compilers + make + basic POSIX tooling.
# Note: Alpine is musl-based; glibc-specific gconv modules are not available here.
RUN apk add --no-cache \
  build-base \
  clang llvm \
  make \
  git \
  coreutils findutils

# Optional tools (package availability varies by Alpine version/arch).
RUN apk add --no-cache valgrind || echo "valgrind not available for this arch/repo; skipping"
RUN apk add --no-cache tcc || echo "tcc not available for this arch/repo; skipping"

# Container-provided helpers so the Makefile doesn't have to guess what is installed.
# `installed-compilers.sh` prints the available compiler executables (space-separated).
RUN cat > /usr/local/bin/installed-compilers.sh <<'EOF'
#!/bin/sh
out=""
for cc in gcc clang tcc; do
	if command -v "$cc" >/dev/null 2>&1; then
		out="${out}${out:+ }$cc"
	fi
done

echo "$out"
EOF
RUN chmod +x /usr/local/bin/installed-compilers.sh

WORKDIR /work
COPY . .

CMD ["make", "test"]
