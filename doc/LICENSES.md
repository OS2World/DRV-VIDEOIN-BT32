# License notes (checked 2026-11-18, from the files in this repository)

**Stated by the repository:** the original README says "GNU GPL". The `LICENSE` file is the text of the **GNU Library General Public License, version 2 (LGPL 2.0)**, not the ordinary GPL. The two do not match, and the
sources do not say which one applies to the OS/2 port.

**What the files say:**

| Files | Origin | Notice in the file |
|---|---|---|
| `src\BTTV\bt848.h`, `bttvp.h`, `bttv.h` | Linux `bttv`, Ralph Metzler 1996-98 | GNU General Public License (version 2 or later, standard Linux text) in `bt848.h` and `bttvp.h`; `bttv.h` has only the copyright line |
| `src\BTTV\bttv-cards.c` (card table section), `tda7432.c` | Csaba Halasz 1999; Eric Sandeen 2000 | "placed under the terms of the GNU General Public License" |
| `src\BTTV\bttv-driver.c`, `bttv-if.c`, `msp3400.c`, `tuner.c`, `tvaudio.c`, `tda9875.c`, `tda9887.c`, `bt832.h`, `id.h` | Linux kernel drivers adapted for OS/2 (Stefan Milcke, 2001-2004) | only "Autor / Erstellt am" lines of Stefan Milcke, no license text (the Linux originals are GPL) |
| `src\Lib32\hwaccess.h` | Creative Labs 1999-2000 (probably from the Linux emu10k1 sound driver; not checked) | GNU General Public License |
| `src\Dev32\*`, `src\Lib32\*.c`, `src\Include\*`, `src\BTTV\bttvos2.c`, `bttv-cards.h`, `msp3400.h` | Stefan Milcke 2001-2004 | author line only, **no license text** |
| `src\Install\rcstub.c`, `cardinfo.rc` | IBM MMPM sample files | IBM Corporation 1993 copyright, no terms in the file |
| `doc\original\Tools\*`, `Makes\*`, `Build.CMD` | build scripts, Stefan Milcke | author line only |

**Assessment (not legal advice):** the driver is a derived work of GPL code (Linux `bttv` and the chip drivers), so the combination can only be distributed under the GPL (version 2). The files of the OS/2 port have no
license header; the author's intention is shown only by the README ("GNU GPL") and the LXAPI32 project it belongs to, which states the same (https://github.com/OS2World/DRV-LXAPI32). The IBM sample files in `src\Install`
(`rcstub.c`, `cardinfo.rc`) have unclear terms: they should be replaced by clean-room code or removed before a release.

**To do before a release:** confirm the license with the original author (Stefan Milcke) or the OS2World maintainers; replace `LICENSE` with the ordinary GPL v2 text if that is confirmed (the Library GPL text does not fit the code);
add license headers to the files of the OS/2 port; replace or remove the IBM files in `src\Install`.
