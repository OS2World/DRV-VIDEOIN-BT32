# BT32 (BT32.SYS)

32-bit OS/2 physical device driver for TV and video capture cards with the Brooktree/Conexant BT848/BT878 chips ("bt8x8"), a port of the Linux `bttv` driver
(with the tuner and audio chip drivers) to OS/2 on top of the LXAPI32 Linux-compatibility driver.

| Field | Details |
|---|---|
| Status | **Source only. Not built and not tested** with the tools of the driver update plan: it cannot be built with Open Watcom alone (see "Build tools"). Nothing in `src\` was changed from the original. |
| Tested on | Not tested (no BT8x8 card, and the build is not reproducible yet). |
| License | GNU GPL, as stated by the original README; see [LICENSE](LICENSE) and **the license notes below and in [doc/LICENSES.md](doc/LICENSES.md): the `LICENSE` file is the Library GPL (LGPL 2.0), most of the code is derived from GPL code of the Linux kernel, and the files of the OS/2 port have no license header.** |
| Authors | Stefan Milcke (OS/2 port, 2001-2004, version 0.8); Ralph Metzler, Gerd Knorr and others (Linux `bttv`); Csaba Halasz, Eric Sandeen (chip drivers of the Linux kernel); Creative Labs (`hwaccess.h`); published on OS2World by Martin Iturbide (2013) |
| Original source | https://github.com/OS2World/DRV-VIDEOIN-BT32 (initial commit 2013-03-19, git tag `upstream-original` = the state before this update, tag `bt32` = the original tag); the files carry CVS ids from 2004-02 |
| Build tools | Original: Open Watcom `wpp386`, GCC (ELF) and `ELFOMF`, IBM DDK (`ALP`, headers), `ILINK` or `LINK386`, EMX `make`, and the LXAPI32 development tree (`LXAPI32DEV`, https://github.com/OS2World/DRV-LXAPI32). **No new build yet**, see below. |
| Install | Needs `LXAPI32.SYS` (https://github.com/OS2World/DRV-LXAPI32, "provides a small Linux API to other device drivers") loaded first: `DEVICE=<path>\LXAPI32.SYS`, then `DEVICE=<path>\BT32.SYS [parameters]` (the driver prints "Unable to attach to LXAPI32.SYS" otherwise). The MINSTALL files in `src\Install` (`CARDINFO.DLL`, `StBT_VSD.DLL`) are for the MMPM video capture part. Back up `CONFIG.SYS` first. Not tested. |
| Parameters | As in the original, see below (not yet the standard set of the plan). |
| Sample / test | `sample\` and `test\` do not exist: nothing to run without the card and a build. |

## Contents

```text
src\BTTV      the Linux bttv driver and the tuner / audio chip drivers (bt848, tuner, tda*, msp3400, tvaudio, bt832, card table), OS/2 adaptations (bttvos2.c)
src\Dev32     the 32-bit device driver itself (header.asm, strategy, init, IDC, IOCTL, parameters, stream interface for MMPM)
src\Lib32     helper library (kmod, misc, debug, hwaccess.h)
src\Include   header files for programs that use the driver (bttvos2.h, bt_ioctl.h)
src\Install   MINSTALL files of the card information DLL
doc\original  the original build system (makefiles, Build.CMD, Makes\, Tools\) and the original README, for reference only
doc\CHANGES.md, doc\LICENSES.md
```

## Build tools (why there is no `compile_wat.cmd` yet)

The plan asks for an Open Watcom 2 build with `wmake`. That is not possible for this driver today, for these reasons (checked 2026-11-18):

1. The sources include the Linux headers of the **LXAPI32 development tree** (`<linux/pci.h>`, `<linux/i2c.h>`, `<lxapilib.h>`, `<devhlp32.h>`, `<ldefos2.h>`, `<lxrmcall.h>` ...), and the original makefiles take that tree from the environment
   variable `LXAPI32DEV` (`Makes\mainmake.mak`). The tree (and the Linux-style libraries `linux.lib`, `impdos.lib` that BT32 links) is a separate project: https://github.com/OS2World/DRV-LXAPI32 (about 900 files).
2. LXAPI32 and the original BT32 build use GCC (ELF) with `ELFOMF`, the IBM `ALP` assembler and `ILINK`, which are not installed on the ArcaOS test systems. Linux-style code compiles with Open Watcom in other projects
   (UNIAUD32 uses Open Watcom 1.9 and `ILINK`), but LXAPI32 would have to be ported to that first.
3. The driver needs `LXAPI32.SYS` at run time and a BT8x8 card to be tested.

A possible path: port or rebuild LXAPI32 with Open Watcom (`wcc386`, `wlink` or `ILINK`), then build `bttv.lib` and `BT32.SYS` from `src\` with it, following the recipe of the UNIAUD32 build. Until then the status stays "source only".

## Parameters and boot messages (original behaviour)

Parameters of `DEVICE=...BT32.SYS`:

| Parameter | Effect |
|---|---|
| `/V` | verbose |
| `/VM` | verbose and list the modules |
| `/3` | execute an `int 3` (debugger breakpoint) at start |
| `/NOCOREMODULE`, `/NOMODULE` | do not load the core module / any module |
| `/MODPARM:<module>:<parm>` | set a module parameter |
| `/LOADMODULE:<name>` | load a module (accepted, does nothing in this version) |
| `/NOIRQ` | do not allow the IRQ |

Messages are prefixed `BT32: ` for parameter errors; the banner `BT32 support driver V<major>.<minor> Build <n> bttv(<a>.<b>.<c>)` is printed at every start and contains a copyright line.
**Not yet done** (plan section 5, needs a build to be tested): the standard parameters `/Q`, `/W`, `/AA`, quiet by default, a prefix on every line, and no copyright text in the boot message.

## Known issues

- Not built, not tested; see above.
- License situation unclear, see `doc\LICENSES.md`.
- `src\Install\rcstub.c` and `cardinfo.rc` carry an IBM copyright notice (an MMPM sample stub); the terms are not stated in the files.

## Links

- https://github.com/OS2World/DRV-VIDEOIN-BT32
- https://github.com/OS2World/DRV-LXAPI32 (LXAPI32.SYS and the LXAPI32 development tree)
- Linux bttv: https://linuxtv.org

## Changelog

- 2026-11-18: standard project layout (sources in `src\`, old build files in `doc\original\`), README, `doc\CHANGES.md`, `doc\LICENSES.md`; no source change.
- 0.8 (2004-02): last version of the original (Stefan Milcke).
