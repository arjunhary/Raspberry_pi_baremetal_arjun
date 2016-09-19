#include "./../include/smp.h"
#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/sysconfig.h"


static volatile __attribute__ ((aligned (0x4000))) unsigned int page_table_core0[4096];
static volatile __attribute__ ((aligned (0x4000))) unsigned int page_table_core1[4096];
static volatile __attribute__ ((aligned (0x4000))) unsigned int page_table_core2[4096];
static volatile __attribute__ ((aligned (0x4000))) unsigned int page_table_core3[4096];


void init_page_table()
{
	int core_id = Get_current_coreid();
	int base = 0;
	volatile unsigned int* page_table;
	
	switch (core_id)
	{
		case 0:
			page_table = page_table_core0;
			break;
		case 1:
			page_table = page_table_core1;
			break;
		case 2:
			page_table = page_table_core2;
			break;
		case 3:
			page_table = page_table_core3;
			break;
		default:
			return;
	}
//0x1540A
//0x1140E
	for(base = 0; base < 1024 - 16; base++)
	{
		page_table[base] = ((base << 20) | (0x1140E));
	}
	
	for(;base < 1024; base++)
	{
		page_table[base] = ((base << 20) | (0x10416));
	}
	
	page_table[base] = ((base << 20) | (0x10416));
	base++;
	
	for(; base< 4096; base++)
	{
		page_table[base] = 0;
	}
}

void mmu_init(void)
{
	int core_id = Get_current_coreid();
	//uart_printf("\nCore : %u MMU Init start", Get_current_coreid());
	//print_arm_sys_ctrl_register();
	invalidate_tlb();
	//uart_printf("\nCore : %u  TLB Invalidate success",Get_current_coreid());
	disable_mmu_caches_branch_prediction();
	//uart_printf("\nCore : %u Caches Invalidate success",Get_current_coreid());
	//uart_printf("\nCore: %u Page table init success",Get_current_coreid());
	set_domain(0, DOMAIN_CLIENT);
	//uart_printf("\nCore % %u Set Domain Success" , Get_current_coreid());
	switch (core_id)
	{
		case 0:
			init_page_table(page_table_core0);
			write_translation_table_base_register((unsigned int)page_table_core0, 0x07A);
			break;
		case 1:
			init_page_table(page_table_core1);
			write_translation_table_base_register((unsigned int)page_table_core1, 0x07A);
			break;
		case 2:
			init_page_table(page_table_core1);		
			write_translation_table_base_register((unsigned int)page_table_core2, 0x07A);
			break;
		case 3:
			init_page_table(page_table_core1);		
			write_translation_table_base_register((unsigned int)page_table_core3, 0x07A);
			break;
		default:
			return;
	}
	//print_arm_sys_ctrl_register();
	datamembarrier();
	enable_mmu_caches_branch_prediction();
	//enable_mmu();
	//enable_instruction_cache();
	datamembarrier();
	//print_arm_sys_ctrl_register();
	//uart_printf("\nCore : %u MMU Init Done", Get_current_coreid());
}
