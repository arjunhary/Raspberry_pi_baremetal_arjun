#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/MiniUART.h"



void SET_BREAKPOINT(void)
{
	__asm__("BKPT");
}

void instructionsyncbarrier(void)
{
	__asm__("ISB");
}

void datamembarrier(void)
{
	__asm__("DMB");
}

void wfe(void)
{
	__asm__("wfe");
}

void datasyncbarrier(void)
{
	__asm__("DSB");
}

void wfi(void)
{
	__asm__("wfi");
}

void sev(void)
{
	__asm__("sev");
}

void print_arm_sys_ctrl_register(void)
{
	uart_printf("\nARM SYS CTRL REG: %x", Read_ARM_System_Control_Register());
}

void print_arm_aux_ctrl_register(void)
{
	uart_printf("\nARM AUX CTRL REG: %x", Read_ARM_Auxiliary_Control_Register());
}

unsigned int Read_Multiprocessor_affinity_register(void)
{				   
	unsigned int MPIDR=0;
	asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (MPIDR));
	return MPIDR;
}

unsigned int Get_current_coreid(void)
{				   
	return (Read_Multiprocessor_affinity_register() & 3);
}


unsigned int Read_Interrupt_status_Register(void)
{		
	unsigned int ISR=0;
	asm volatile("mrc p15, 0, %0, c12, c1, 0" : "=r" (ISR));
	return ISR;
}

unsigned int Read_Watchpoint_control_Register0(void)
{
	unsigned int WCR0=0;
	asm volatile("mrc p14, 0, %0, c0, c0, 7" : "=r" (WCR0));
	return WCR0;
	
}

unsigned int Read_Breakpoint_control_Register0(void)
{
	unsigned int BCR0=0;
	asm volatile("mrc p14, 0, %0 , c0, c0, 4" : "=r" (BCR0));
	return BCR0;	
}

unsigned int Read_CPSR_Register(void)
{
	unsigned int CPSR=0;
	asm volatile("mrs %0,cpsr" : "=r" (CPSR));
	return CPSR;
}

unsigned int Read_ARM_Auxiliary_Control_Register(void)
{
	unsigned int ACR=0;
	asm volatile("mrc p15, 0, %0, c1, c0, 1" : "=r" (ACR));
	return ACR;
}
	
unsigned int Read_ARM_System_Control_Register(void)
{
	unsigned int ASCR=0;
	asm volatile("mrc p15, 0, %0, c1, c0, 0" : "=r" (ASCR));
	return ASCR;	
}

unsigned int Read_Instruction_Fault_Status_Register(void)
{
	unsigned int IFSR=0;
	asm volatile("MRC p15, 0, %0, c5, c0, 1": "=r" (IFSR)); //Read Instruction Fault Status Register
	return IFSR;	
}

unsigned int Read_Data_Fault_Status_Register(void)
{
	unsigned int DFSR=0;
	asm volatile("mrc p15, 0, %0, c5, c0, 0": "=r" (DFSR)); //Read Data Fault Status Register
	return DFSR;
}

void invalidate_tlb(void)
{
	asm volatile("mov r2,#0\n\t"
				 "MCR p15, 0, r2, c8, c7, 0");
}

void invalidate_caches(void)
{
	asm volatile("mov r2,#0\n\t"
				 "MCR p15, 0, r2, c7, c7, 0");
}

void enable_instruction_cache(void)
{
	asm volatile("mrc p15, 0, r0, c1, c0, 0\n\t"
				"orr r0,r0,#0x00001000\n\t"
				"mcr p15, 0, r0, c1, c0, 0");
}


void enable_mmu(void)
{
	asm volatile("mrc p15, 0, r0, c1, c0, 0\n\t"
				 "orr r0,r0,#1\n\t"
				 "mcr p15, 0, r0, c1, c0, 0");
}


void enable_mmu_caches_branch_prediction(void)
{
	unsigned int mode = 0;
	asm volatile("mrc p15, 0, %0, c1, c0, 0": "=r" (mode));
	mode = mode | 0x24001807;
	asm volatile("mcr p15, 0, %0, c1, c0, 0":: "r" (mode): "memory");
}

void disable_mmu(void)
{
	asm volatile("mrc p15, 0, r0, c1, c0, 0\n\t"
				 "bic r0,r0,#1\n\t"
				 "mcr p15, 0, r0, c1, c0, 0");
}

void disable_mmu_caches_branch_prediction(void)
{
	asm volatile("mrc p15, 0, r0, c1, c0, 0\n\t"
				 "bic r0,r0,#0x1800\n\t"
				 "bic r0,r0,#7\n\t"
				 "bic r0,r0,#0x4000000\n\t"
				 "mcr p15, 0, r0, c1, c0, 0");
}

void set_domain(unsigned int domain, unsigned int domain_type)
{
	asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (domain_type));
}

void write_translation_table_base_register(unsigned int addr, unsigned int flags)
{
	unsigned int ttbr = (addr | flags);
	asm volatile("MCR p15, 0, %0, c2, c0, 0"::"r" (ttbr): "memory");
}
	
void print_arm_registers(void)
{
	unsigned int arm_aux_ctrl_reg = Read_ARM_Auxiliary_Control_Register();
	unsigned int arm_sys_ctrl_reg = Read_ARM_System_Control_Register();
	unsigned int arm_watchpoint_ctrl_reg = Read_Watchpoint_control_Register0();	
	unsigned int arm_breakpoint_ctrl_reg  = Read_Breakpoint_control_Register0();
	unsigned int arm_interrupt_reg  = Read_Interrupt_status_Register();
	unsigned int arm_cpsr_reg = Read_CPSR_Register();
	uart_printf("\nARM SYS CTRL REG: %x" ,arm_sys_ctrl_reg);
	uart_printf("\nARM WATCHPOINT CTRL REG: %x",arm_watchpoint_ctrl_reg);
	uart_printf("\nARM BREAKPOINT CTRL REG: %x",arm_breakpoint_ctrl_reg);
	uart_printf("\nARM AUX CTRL REG: %x",arm_aux_ctrl_reg);
	uart_printf("\nARM CPSR REG: %x",arm_cpsr_reg);
	uart_printf("\nARM INTR REG: %x",arm_interrupt_reg);
}

void print_coreid(void)
{
	uart_printf("\nCurrent core executing: %u",(Read_Multiprocessor_affinity_register() & 3));
}

void get_lock(volatile int* lock)
{
	int status = -1;
	do
	{
		while(ldrex(0,(unsigned int) lock) != 0);
		status = strex(0,(unsigned int)lock,1);
	}while(status!=0);
	datamembarrier();
}

void free_lock(volatile int* lock)
{
	*lock = 0;
	datamembarrier();

}
