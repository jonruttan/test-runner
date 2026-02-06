# Helpers

Optional helper headers live in `include/`:

- `include/test-helper-file.h`: in-memory file descriptor buffers for read/write style tests
- `include/test-helper-mem.h`: allocator hooks and deterministic allocation helpers (failure simulation + counters)
- `include/test-helper-system.h`: helpers around `system(3)` and exit status

