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

WORKDIR /work
COPY . .

CMD ["make", "test"]
