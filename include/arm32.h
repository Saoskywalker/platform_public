#ifndef __ARM32_H__
#define __ARM32_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined(__CC_ARM)

/*
读 cp15-c0
*/
uint32_t __inline arm32_read_p15_c0(uint32_t ld)
{
	uint32_t value;
	if(ld==0)
	{
		__asm
		{
			mrc p15, 0, value, c0, c0, 0
		}
	}else
	{
		__asm
		{
			mrc p15, 0, value, c0, c0, 1
		}	
	}
	return value;
}
/*
读 cp15-c1
*/
uint32_t __inline arm32_read_p15_c1(void)
{
	uint32_t value;
	__asm
	{
		mrc p15, 0, value, c1, c0, 0
	}
	return value;
}
/*
写 cp15-c1
*/
void __inline arm32_write_p15_c1(uint32_t value)
{
	__asm
	{
		mcr p15, 0, value, c1, c0, 0
	}
}

static __inline void arm32_interrupt_enable(void)
{
	uint32_t tmp;

	__asm
	{
		mrs tmp, cpsr
		bic tmp, tmp, #(1<<7)
		msr cpsr_cxsf, tmp
	}
}

static __inline void arm32_interrupt_disable(void)
{
	uint32_t tmp;

	__asm 
	{
		mrs tmp, cpsr
		orr tmp, tmp, #(1<<7)
		msr cpsr_cxsf, tmp
	}
}

void __inline arm32_mmu_enable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value | (1 << 0));
}

void __inline arm32_mmu_disable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value & ~(1 << 0));
}

void __inline arm32_dcache_enable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value | (1 << 2));
}

void __inline arm32_dcache_disable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value & ~(1 << 2));
}

void __inline arm32_icache_enable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value | (1 << 12));
}

void __inline arm32_icache_disable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value & ~(1 << 12));
}
/*
写 cp15-c2 页表地址
*/
void __inline arm32_ttb_set(uint32_t base)
{
	__asm
	{
		mcr p15, 0, base, c2, c0, 0
	}	
}
/*
写 cp15-c3 域权限
*/
void __inline arm32_domain_set(uint32_t domain)
{
	__asm
	{
		mcr p15, 0, domain, c3, c0, 0
	}	
}
/*
写 cp15- tlb无效
*/
void __inline arm32_tlb_invalidate(void)
{
	uint32_t value =0;
	__asm
	{
		mcr p15, 0, value, c7, c10, 4

		mcr p15, 0, value, c8, c6, 0
		mcr p15, 0, value, c8, c5, 0
	}			
}

void __inline arm32_dcache_flush_line(unsigned int addr)
{		
	__asm
	{
		mcr p15, 0, addr, c7, c14, 1 /* 清空并使无效数据... */
	}		
}

void __inline arm32_icache_flush_line(unsigned int addr)
{		
	__asm
	{
		mcr p15, 0, addr, c7, c5, 1 /* invalidate I cache */ 
	}		
}

void __inline arm32_dcache_icache_flush_line(unsigned int addr)
{		
	__asm
	{
		mcr p15, 0, addr, c7, c14, 1 /* 清空并使无效数据... */
		mcr p15, 0, addr, c7, c5, 1 /* invalidate I cache */ 
	}		
}

#else

static inline uint32_t arm32_read_p15_c1(void)
{
	uint32_t value;

	__asm__ __volatile__(
		"mrc p15, 0, %0, c1, c0, 0"
		: "=r" (value)
		:
		: "memory");

	return value;
}

static inline void arm32_write_p15_c1(uint32_t value)
{
	__asm__ __volatile__(
		"mcr p15, 0, %0, c1, c0, 0"
		:
		: "r" (value)
		: "memory");
	arm32_read_p15_c1();
}

static inline void arm32_interrupt_enable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"bic %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

static inline void arm32_interrupt_disable(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrs %0, cpsr\n"
		"orr %0, %0, #(1<<7)\n"
		"msr cpsr_cxsf, %0"
		: "=r" (tmp)
		:
		: "memory");
}

static inline void arm32_mmu_enable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value | (1 << 0));
}

static inline void arm32_mmu_disable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value & ~(1 << 0));
}

static inline void arm32_dcache_enable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value | (1 << 2));
}

static inline void arm32_dcache_disable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value & ~(1 << 2));
}

static inline void arm32_icache_enable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value | (1 << 12));
}

static inline void arm32_icache_disable(void)
{
	uint32_t value = arm32_read_p15_c1();
	arm32_write_p15_c1(value & ~(1 << 12));
}

static inline uint32_t arm32_smp_processor_id(void)
{
	uint32_t tmp;

	__asm__ __volatile__(
		"mrc p15,0,%0,c0,c0,5\n"
		"and %0,%0,#0x3\n"
		: "=r" (tmp)
		:
		: "memory");
	return tmp;
}

static inline void arm32_ttb_set(uint32_t base)
{
	__asm__ __volatile__(
		"mcr p15, 0, %0, c2, c0, 0"
		:
		: "r" (base)
		: "memory");
}

static inline void arm32_domain_set(uint32_t domain)
{
	__asm__ __volatile__(
		"mcr p15, 0, %0, c3, c0, 0"
		:
		: "r" (domain)
		: "memory");
}

static inline void arm32_tlb_invalidate(void)
{
	__asm__ __volatile__(
		"mov r0, #0\n"
		"mcr p15, 0, r0, c7, c10, 4\n"
		"mcr p15, 0, r0, c8, c6, 0\n"
		"mcr p15, 0, r0, c8, c5, 0\n"
		:
		:
		: "r0");
}

void inline arm32_dcache_flush_line(unsigned int addr)
{
	/* 清空并使无效数据... */
	__asm__ __volatile__(
	"mcr p15,0,%0,c7,c14,1\n"
	: 
	: "r" (addr)
	: "memory");
}

void inline arm32_icache_flush_line(unsigned int addr)
{
	/* invalidate I cache */
	__asm__ __volatile__(
	"mcr p15,0,%0,c7,c5,1\n"
	: 
	: "r" (addr)
	: "memory");
}

void inline arm32_dcache_icache_flush_line(unsigned int addr)
{
	/* 清空并使无效数据... */
	/* invalidate I cache */
	__asm__ __volatile__(
	"mcr p15,0,%0,c7,c14,1\n"
	"mcr p15,0,%0,c7,c5,1\n"
	: 
	: "r" (addr)
	: "memory");
}

#endif

#define CONFIG_SYS_CACHELINE_SIZE 32 /*Cache line组织宽度为32byte*/

void cpu_flush_dcache_icache_range(unsigned long start, unsigned long end);
void cpu_flush_dcache_icache(unsigned long start, unsigned long size);

static inline void cpu_flush_dcache_range(unsigned long start, unsigned long end)
{
	unsigned int addr;

	start = start & (~(CONFIG_SYS_CACHELINE_SIZE - 1));
	end = end & (~(CONFIG_SYS_CACHELINE_SIZE - 1));
	for (addr = start; addr <= end; addr += CONFIG_SYS_CACHELINE_SIZE)
		arm32_dcache_flush_line(addr); /*F1C100S没有写缓冲*/
}

static inline void cpu_flush_dcache(unsigned long start, unsigned long size)
{
	cpu_flush_dcache_range(start, start + size);
}

#ifdef __cplusplus
}
#endif

#endif /* __ARM32_H__ */
