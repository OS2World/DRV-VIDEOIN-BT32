/* compat shim: the 2004 LXAPI32 name of DEVHELP.H of the 2003 tree, plus the 2004 names that BT32 uses */
#ifndef COMPAT_DEVHLP32_H
#define COMPAT_DEVHLP32_H
#include <devhelp.h>
#include <stacktoflat.h>
#ifndef MAKE_FARPTR32
#define MAKE_FARPTR32(a) MAKE_FAR48(a)      /* 16:16 pointer value -> 16:32 far pointer */
#endif
#ifndef Int3
#define Int3() _asm int 3
#endif
#endif
