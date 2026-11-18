# compat: overlay headers for building BT32 with GCC 9.2 (work in progress)

These headers go **in front of** the LXAPI32 `Include\` directory (`-Isrc\compat\include`). They are not part of the original BT32 and were added during the port attempt of 2026-11-18 (see `doc\BUILD-ATTEMPT.md`).

| File | What | Origin |
|---|---|---|
| `asm\processor.h` | the LXAPI32 header with `extern struct tss_struct init_tss[NR_CPUS];` moved after the definition of `struct tss_struct` (GCC 9 does not accept an array of an incomplete type) | LXAPI32 (GPL), one line moved |
| `linux\i2c.h` | the LXAPI32 header with `struct i2c_msg msg[]` / `msgs[]` parameters changed to pointers (same reason) | LXAPI32 / Linux (GPL), two parameters |
| `media\tuner.h`, `media\audiochip.h`, `media\ir-common.h`, `media\video-buf.h`, `linux\i2c-algo-bit.h` | headers that BT32 includes but that the LXAPI32 tree does not have | Linux kernel 2.6.12 (https://github.com/torvalds/linux, tag v2.6.12), GPL v2, unmodified; BT32 was written against 2.6.2/2.6.3 era versions, so these may differ in details |

Still missing (the 2004 LXAPI32 API; see `doc\BUILD-ATTEMPT.md`): `ldefos2.h`, `stacktoflat.h`, `linux\process.h`, `linux\wrapper.h`, `linux\i2c-compat.h`, `linux\tvaudio.h`, `asm\semaphor.h` (= `semaphore.h`), `linux\malloc.h` (= `slab.h`), `ioremap.h`,
and the functions `OS2_initlxapi`, `OS2_add_module`, `OS2_lock_mem`, `OS2_get_current_pid`, `OS2_lxioctl`, ... that `src\Dev32` calls.
