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
COPY container/installed-compilers.sh /usr/local/bin/installed-compilers.sh
RUN chmod +x /usr/local/bin/installed-compilers.sh

WORKDIR /work
COPY . .

CMD ["make", "test"]
