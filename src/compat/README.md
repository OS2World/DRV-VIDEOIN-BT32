# compat: overlay headers for building BT32 with Open Watcom against the 2003 LXAPI32 (work in progress)

These headers go **in front of** the LXAPI32 `Include\` directory. They are not part of the original BT32 and were added during the port attempt of 2026-11-18 (see `doc\BUILD-ATTEMPT.md`).

| File | What | Origin |
|---|---|---|
| `devhlp32.h` | shim: includes `devhelp.h` and `stacktoflat.h`; `MAKE_FARPTR32` -> `MAKE_FAR48`, `Int3()` -> `int 3` | written for this project |
| `linux\module.h` | the 2003 LXAPI32 header plus the 2004 macro `MODULE_PARM_A` | LXAPI32 (GPL) + one macro |
| `linux\i2c-compat.h`, `asm\pgtable-2level.h` | small shims for headers the 2003 tree lacks | written for this project |
| `media\tuner.h`, `media\audiochip.h`, `media\ir-common.h`, `media\video-buf.h`, `linux\i2c-algo-bit.h` | headers BT32 includes but the tree lacks | Linux kernel 2.6.12 (tag v2.6.12), GPL v2, `tuner.h` trimmed to its non-kernel part |

Still missing: the 2004 `OS2_*` functions (`lock_mem`, `unlock_mem`, `open_device_called`, ...), more I2C ids, and the MMPM video-capture types (`VCASCREENINFO`, ...).
