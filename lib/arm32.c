/*cache操作 flush*/
#include <arm32.h>

void cpu_flush_dcache_icache_range(unsigned long start, unsigned long end)
{
	unsigned int addr;

	start = start & (~(CONFIG_SYS_CACHELINE_SIZE - 1));
	end = end & (~(CONFIG_SYS_CACHELINE_SIZE - 1));
	for (addr = start; addr <= end; addr += CONFIG_SYS_CACHELINE_SIZE)
		arm32_dcache_icache_flush_line(addr); 
}

void cpu_flush_dcache_icache(unsigned long start, unsigned long size)
{
	//	sysFlushCache(I_D_CACHE);
	cpu_flush_dcache_range(start, start + size);
}
