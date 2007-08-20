/*
 * linux/include/asm-arm/arch-nexuspci/io.h
 *
 * Copyright (C) 1997-1999 Russell King
 * Copyright (C) 2000 FutureTV Labs Ltd.
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffff

/*
 * Translation of various region addresses to virtual addresses
 */
#define __io(a)			(PCIO_BASE + (a))
#if 1
#define __mem_pci(a)		((unsigned long)(a))
#define __mem_isa(a)		(PCIMEM_BASE + (unsigned long)(a))
#else

extern __inline__ unsigned long ___mem_pci(unsigned long a)
{
	/* PCI addresses must have been ioremapped */
	if (a <= 0xc0000000 || a >= 0xe0000000)
		*((int *)0) = 0;
	return a;
}

extern __inline__ unsigned long ___mem_isa(unsigned long a)
{
	if (a >= 16*1048576)
		BUG();
	return PCIMEM_BASE + a;
}
#define __mem_pci(a)		___mem_pci((unsigned long)(a))
#define __mem_isa(a)		___mem_isa((unsigned long)(a))
#endif

/*
 * Generic virtual read/write
 */
#define __arch_getw(a)		(*(volatile unsigned short *)(a))
#define __arch_putw(v,a)	(*(volatile unsigned short *)(a) = (v))

/*
 * ioremap support - validate a PCI memory address,
 * and convert a PCI memory address to a physical
 * address for the page tables.
 */
#define iomem_valid_addr(iomem,sz)	\
	((iomem) < 0x80000000 && (iomem) + (sz) <= 0x80000000)
#define iomem_to_phys(iomem)	((iomem) + PLX_MEM_START)

#endif
