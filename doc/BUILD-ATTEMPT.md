# Build attempt with current tools (2026-11-18)

Goal: plan steps 2 and 3 (build the original code unchanged with new tools, then fix what is needed to run on ArcaOS). Result: **no build possible yet**. This file records what was tried and what was found,
so the next attempt does not start from zero. Nothing in `src\` was changed.

## What was used

- ArcaOS Dev VM, GCC 9.2.0 (OS/2 RPM build, `gcc.exe -Zomf`, `emxomf`), Open Watcom 2 (`wpp386`, `wlink`, `wasm`), NASM, IBM OS/2 DDK 2004 (`C:\Desktop\DDK`).
- The LXAPI32 tree cloned from https://github.com/OS2World/DRV-LXAPI32 (commit `7d13b5c`, files from 2005-2006) next to this project, as `..\DRV-LXAPI32`.
- A probe script that compiles each C source of BT32 (24 files in `src\BTTV`, `src\Dev32`, `src\Lib32`) with
  `gcc -c -Zomf -fno-builtin -ffreestanding -fgnu89-inline -fcommon -DTARGET_OS2 -D__KERNEL__ -DTARGET_OS2_ELF -DOS2OLDGCC -DBUILD_LEVEL=1 -U__GNUC__ -D__GNUC__=3 -D__GNUC_MINOR__=3`
  and the include path `..\DRV-LXAPI32\Include`, `Include\asm\mach-default`, `src\BTTV`, `src\Include`, `src\Dev32`, `src\Lib32`, then `C:\usr\include` and the DDK headers last
  (the original makefile compiles all C code with GCC and uses `-I$(GCC)\Include -I$(DDK)\BASE\H`, see `doc\original\Makes\mainmake.mak` of LXAPI32).

## Results

Compiles without errors: `Dev32\blddat.c`, `Lib32\debug.c`. All other files stop early:

| Class | Files | First error |
|---|---|---|
| Header of the **old LXAPI API** is missing from the LXAPI32 tree | `Dev32\init.c`, `ioctl.c`, `param.c` | `ldefos2.h: No such file` |
| | `Dev32\idc.c` | `stacktoflat.h: No such file` |
| | `Lib32\kmod.c` | `asm/semaphor.h: No such file` (the tree has `asm/semaphore.h`) |
| Old Linux headers do not compile with GCC 9.2 even with `__GNUC__` set to 3 | all of `src\BTTV` | `asm/processor.h:90: array type has incomplete element type 'struct tss_struct'` |
| | `Dev32\strategy.c`, `stream*.c`, `strm*.c`, `BTTV\msp3400.c`, `tda*.c`, `tuner.c`, `tvaudio.c` | `linux/time.h: unknown type name 'time_t'` / `suseconds_t`, `linux/jiffies.h: unknown type name 'u64'` |
| | `Lib32\misc.c` | `linux/audit.h:101: field 'nlh' has incomplete type` |

(`-include sys/types.h` and further header patches were not carried on; the list is the first layer, there are certainly more behind it.)

## The important finding: BT32 and the LXAPI32 tree are different generations

- BT32's `Dev32` sources are written for the **Watcom-style** LXAPI of 2004: they use `__far` pointers and `MAKE_FARPTR32` (`Dev32\init.c`: `char __far *str16 = (char __far *)MAKE_FARPTR32(msg->MsgPtr)`), `#include <ldefos2.h>`,
  `<stacktoflat.h>`, `<asm/semaphor.h>`, and the original `Build.CMD` checks for `WATCOM\BINP\WPP386.EXE`.
- The only LXAPI32 tree available (OS2World) is the **GCC-only** version of 2005-2006: `DEVTYPE.H (for GCC)` of 2006-02-20, far pointers as `FARPTR16` / `MAKE_FP16`, `lxstack.h`, `asm/semaphore.h`. Its `dev.txt` lists
  Watcom 11.0c, ALP, ILINK and "Innotek's GCC beta 3" as the tools.
- So BT32 cannot simply be built against this tree. Either the 2004 LXAPI32 headers have to be found (the matching `ldefos2.h`, `stacktoflat.h`, `devhlp32.h` of that time) or `src\Dev32` and the other sources have to be **ported to the 2006 API**
  (use the sources of LXAPI32's own `Dev32` as the model), which is a code change, not a build setup. And `LXAPI32.SYS` would have to be built from the same tree to match at run time.
- On top of that, the Linux-style headers of LXAPI32 need fixes for GCC 9.2 (the errors above), and `linux.lib`, `bttv.lib`, `impdos.lib` have to be built from 150+ sources with the same toolchain before `BT32.SYS` can be linked.
  The link step (`LINK386`/`ILINK` for a 32-bit `PHYSICAL DEVICE` with `DATA32`/`CODE32` segments, see `doc\original\Dev32.makefile`) has not been tried with `wlink`.

## Options

1. Find the LXAPI32 snapshot of early 2004 (BT32 0.8 matches it) and build both from it, with GCC 9.2 for the Linux-style code.
2. Port `src\Dev32` to the 2006 LXAPI32 API and build both with the current tree (new code, no hardware to test: a BT8x8 card and a machine for it are needed).
3. Stay at "source only": publish the layout and the documents, and keep this record.

## Second round: porting src\Dev32 to the 2006 API (2026-11-18)

Fixed in an overlay (`src\compat\`, see its README): the preprocessor flags (one `set CPP=-D...` line broke the probe script, the flags had to go inline), the include order (the kLIBC `C:\usr\include` before the DDK headers, otherwise the 16-bit
DDK `os2.h` is used), two header errors of GCC 9.2 (`tss_struct`, `i2c_msg`), and five Linux headers from kernel 2.6.12 that the LXAPI32 tree lacks. A test file that includes the Linux headers BT32 uses (`linux/module.h`, `pci.h`, `i2c.h`, `videodev.h`, ...)
now compiles without an error.

Then the real sources show the actual size of the problem:

- **More missing 2004 headers:** `linux/process.h` (included by six chip drivers and `stream.h`), `linux/wrapper.h`, `linux/i2c-compat.h`, `linux/tvaudio.h`, `ioremap.h`, `linux/malloc.h`, `asm/semaphor.h` (the last two are only renamed in the 2006 tree).
- **The OS/2 client API of LXAPI32 no longer exists.** `src\Dev32` calls `OS2_initlxapi()`, `OS2_add_module()` (for the modules bttv, tvaudio, tuner, msp3400), `OS2_lock_mem()`, `OS2_unlock_mem()`, `OS2_get_current_pid()`, `OS2_lxioctl()`,
  `OS2_open_device_called()`, `OS2_set_module_parm()`, `OS2_get_module_name()` and others. **None of these names appears anywhere in the 2006 LXAPI32 tree** (searched all sources, headers, asm and def files); the 2006 tree builds Linux modules a different way
  (`modules\`, `makes\modmake.mak`). In the 2006 design BT32 would be a module loaded by LXAPI32 instead of a stand-alone driver that registers its modules itself.
- Consequence: porting `src\Dev32` is not a set of renames. The driver start-up, the IOCTL path, the stream interface for MMPM and the module registration have to be redesigned against a different architecture, in code that cannot be run without a
  BT8x8 card, and `LXAPI32.SYS` of the 2006 tree would have to be built and installed too (needs GCC-ELF era tools, see above).

## Recommendation

Look for the early-2004 LXAPI32 (Netlabs / Hobbes archives; BT32 0.8 matches it) before writing new glue code. If it is found, the work is mainly the GCC 9.2 compatibility of the Linux-style headers and the link recipe. If it is not found, this driver should stay at
"source only" until someone with the card can test a redesign.

## Third round: the 2003 Watcom LXAPI32 zip + Open Watcom (2026-11-18)

Base used: `lxapi32.zip` (Watcom-based LXAPI32 of 2002-2003, provided by Martin, extracted next to this project as `..\LXAPI32-2003`, not part of this repository). It does have the old client API
(`OS2_initlxapi`, `OS2_add_module`, `OS2_lxioctl`, `OS2_set_module_parm`, ...) and `ldefos2.h`/`stacktoflat.h`, so it is the right generation of tree. Compiler: Open Watcom 2 (`wcc386` for BTTV and Lib32, `wpp386 -xd` for Dev32).
Compile-only probe, include order: `src\compat\include`, LXAPI32-2003 `Include`, `src\BTTV`, `src\Include`, `src\Dev32`, `src\Lib32`, Watcom `H`, DDK `base\h`.

Result: 11 of 24 sources compile clean (blddat, bttv-cards, bttv-if, bttvos2, debug, idc, ioctl, kmod, misc, tda7432, tda9887, tuner). The rest still fail, and the remaining errors are API drift between the 2003 tree and the 2004 tree BT32 0.8 was written for:

- `OS2_lock_mem`, `OS2_unlock_mem`, `OS2_open_device_called`, `OS2_close_device_called`, `OS2_get_module_version_code` do not exist in the 2003 tree; `OS2_set_module_parm` and `kernel_thread` and `DevPushfCli` have different parameter lists.
- `I2C_TDA9874` (and probably more ids) missing from `linux\i2c-id.h`; `tvaudio.c` uses a `sem` that is not declared.
- `Dev32\stream.h` uses the MMPM video-capture types `VCASCREENINFO`, `VCASETCAPTURERECT`, `VCAI_SCREENINFO`, `VCACAPISIZE`. They are in neither the Toolkit 4.5 `h` nor the DDK 2004 headers available here (probably in an MMPM/video-capture header set that is not installed).
- `bttv-driver.c` lines 324 and 1165 reference an undeclared symbol `asdfasdf` (placeholder left in the source; GCC builds may have compiled it out).
- Missing `<stdio.h>`/`<stdlib.h>`-style declarations (`sprintf`, `malloc`, `free`) in the Watcom kernel include setup, and the module objects (`bttv_module`, `tuner_module`, ...) that the 2004 `module.h` declared.

Added to the overlay (so far): `MODULE_PARM_A` (2004 macro) in `linux\module.h`, `devhlp32.h` mapping `MAKE_FARPTR32` to `MAKE_FAR48` and `Int3()` to `int 3`. No `BT32.SYS` was linked; nothing was run.

Remaining effort is mostly guesswork about the 2004 API (new code for lock_mem/unlock_mem etc.) in a driver that cannot be exercised without a BT8x8 card. The next useful input would be the real 2004 LXAPI32 headers (or the MMPM video-capture header with `VCASCREENINFO`).
