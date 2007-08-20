#ifndef _ALPHA_SOFTIRQ_H
#define _ALPHA_SOFTIRQ_H

#include <linux/stddef.h>
#include <asm/atomic.h>
#include <asm/hardirq.h>

extern inline void cpu_bh_disable(int cpu)
{
	local_bh_count(cpu)++;
	mb();
}

extern inline void cpu_bh_enable(int cpu)
{
	mb();
	local_bh_count(cpu)--;
}

#define local_bh_enable()	cpu_bh_enable(smp_processor_id())
#define __local_bh_enable	local_bh_enable
#define local_bh_disable()	cpu_bh_disable(smp_processor_id())

#define in_softirq() (local_bh_count(smp_processor_id()) != 0)

#define __cpu_raise_softirq(cpu,nr) set_bit((nr), &softirq_pending(cpu))

#endif /* _ALPHA_SOFTIRQ_H */
