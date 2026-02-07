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
COPY container/installed-compilers.sh /usr/local/bin/installed-compilers.sh
RUN chmod +x /usr/local/bin/installed-compilers.sh

WORKDIR /work
COPY . .

CMD ["make", "test"]
