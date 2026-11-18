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
