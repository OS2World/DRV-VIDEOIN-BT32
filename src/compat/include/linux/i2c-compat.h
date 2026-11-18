/* compat: replaces the i2c package header <linux/i2c-compat.h> that the 2003 LXAPI32 tree does not have.
 * Module parameters (MODULE_PARM) do not exist in the OS/2 build and Open Watcom cannot expand the
 * macro, so I2C_CLIENT_MODULE_PARM only declares the address lists. */
#ifndef COMPAT_I2C_COMPAT_H
#define COMPAT_I2C_COMPAT_H

#ifdef I2C_CLIENT_MODULE_PARM
#undef I2C_CLIENT_MODULE_PARM
#endif
#define I2C_CLIENT_MODULE_PARM(var,desc) \
  static unsigned short var[I2C_CLIENT_MAX_OPTS] = I2C_CLIENT_DEFAULTS

#endif
