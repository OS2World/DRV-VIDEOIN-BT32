# Changes from the original BT32

Original state: git tag `upstream-original` (commit "Update README.md", on top of "Original Source Code", 2013-03-19; the sources are version 0.8 of Stefan Milcke, CVS ids from 2004-02).

## Layout (2026-11-18, no source change)

All moves were done with `git mv`, so the history of every file is kept.

| Original | Now | Why |
|---|---|---|
| `BTTV\*.c, *.h` | `src\BTTV\` | sources into `src\` (plan section 2) |
| `Dev32\*.c, *.h, *.asm` | `src\Dev32\` | same |
| `Lib32\*.c, *.h` | `src\Lib32\` | same |
| `Include\*.h` | `src\Include\` | same |
| `Install\*` (MINSTALL files) | `src\Install\` | same |
| `BTTV\makefile`, `Dev32\makefile`, `Lib32\makefile`, `Install\makefile` | `doc\original\BTTV.makefile`, `Dev32.makefile`, `Lib32.makefile`, `Install.makefile` | original build files only in `doc\original\` |
| `makefile`, `Build.CMD`, `clean.cmd`, `release.cmd`, `Makes\`, `Tools\` | `doc\original\` | same (they are not used by a new build) |
| `README.md` (3 lines, empty template) | `doc\original\README.md` | replaced by the new `README.md` |
| `LICENSE` | unchanged (see `doc\LICENSES.md`) | |

The include paths inside the sources are relative to each other (for example `<..\bttv\bttvp.h>` in `Dev32\init.c`) and still resolve, because the directories stay side by side under `src\`.

## New files

- `README.md` (plan template), `doc\CHANGES.md`, `doc\LICENSES.md`, `.gitignore`.

## Not done

- **Build with the new tools (plan steps 2 and 3).** Not possible: BT32 needs the LXAPI32 development tree (Linux headers and libraries, `https://github.com/OS2World/DRV-LXAPI32`) and was built with GCC (ELF) + `ELFOMF`, IBM `ALP`,
  `ILINK`, EMX `make` and the IBM DDK, none of which is on the ArcaOS test systems; the driver also needs `LXAPI32.SYS` and a BT8x8 card to run. No baseline binary exists to compare with. See "Build tools" in the README.
- **Boot messages and parameters (step 4).** Not changed, needs a build for testing. The existing parameters are listed in the README; the banner printed at every start contains a copyright line, which the plan says must not be printed.
- **Assembler.** The two assembler files (`src\Dev32\header.asm`, `strmstub.asm`, `end.asm`) were assembled with IBM `ALP` in the original build; which assembler a new build would use (WASM or NASM) is not decided.
