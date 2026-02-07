FROM debian:bookworm-slim

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    ca-certificates \
    build-essential \
    clang \
    tcc \
    make \
    git \
    gcovr \
    valgrind \
  && rm -rf /var/lib/apt/lists/*

# Debian (glibc) includes gconv modules as part of the base system.

# Container-provided helpers so the Makefile doesn't have to guess what is installed.
# `installed-compilers.sh` prints the available compiler executables (space-separated).
RUN cat > /usr/local/bin/installed-compilers.sh <<'EOF'
#!/bin/sh
set -eu

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
