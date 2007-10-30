/* linux/include/asm/arch-s3c2413/regs-irq.h
 *
 * Copyright (c) 2003 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 *
 *  Changelog:
 *    19-06-2003     BJD     Created file
 *    12-03-2004     BJD     Updated include protection
 */


#ifndef ___ASM_ARCH_REGS_IRQ_H
#define ___ASM_ARCH_REGS_IRQ_H "$Id: regs-irq.h,v 1.1.1.1 2005/11/16 00:55:04 yongkal Exp $"

/* interrupt controller */

#define S3C2413_IRQREG(x)   ((x) + S3C2413_VA_IRQ)
#define S3C2413_EINTREG(x)  ((x) + S3C2413_VA_GPIO)

#define S3C2413_SRCPND	       S3C2413_IRQREG(0x000)
#define S3C2413_INTMOD	       S3C2413_IRQREG(0x004)
#define S3C2413_INTMSK	       S3C2413_IRQREG(0x008)
#define S3C2413_PRIORITY       S3C2413_IRQREG(0x00C)
#define S3C2413_INTPND	       S3C2413_IRQREG(0x010)
#define S3C2413_INTOFFSET      S3C2413_IRQREG(0x014)
#define S3C2413_SUBSRCPND      S3C2413_IRQREG(0x018)
#define S3C2413_INTSUBMSK      S3C2413_IRQREG(0x01C)

/* mask: 0=enable, 1=disable
 * 1 bit EINT, 4=EINT4, 23=EINT23
 * EINT0,1,2,3 are not handled here.
*/

#define S3C2413_EINTMASK       S3C2413_EINTREG(0x0B4)
#define S3C2413_EINTPEND       S3C2413_EINTREG(0X0B8)

#endif /* ___ASM_ARCH_REGS_IRQ_H */
