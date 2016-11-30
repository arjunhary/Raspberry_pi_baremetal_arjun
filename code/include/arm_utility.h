#ifndef __ARM_UTILITY__H__
#define __ARM_UTILITY__H__

enum{
	DOMAIN_NO_ACCESS = 0,
	DOMAIN_CLIENT,
	DOMAIN_RESERVED,
	DOMAIN_MANAGER
}domain_access_values;

void PUT32(unsigned int addr, unsigned int value);
unsigned int GET32(unsigned int addr);
void dummy ( unsigned int dummy_value);


void SET_BREAKPOINT(void);
unsigned int Read_ARM_Auxiliary_Control_Register(void);
unsigned int Read_Watchpoint_control_Register0(void);
unsigned int Read_ARM_System_Control_Register(void);
unsigned int Read_ARM_Auxiliary_Control_Register(void);
unsigned int Read_Breakpoint_control_Register0(void);
unsigned int Read_Interrupt_status_Register(void);
unsigned int Read_CPSR_Register(void);
unsigned int Read_Instruction_Fault_Status_Register(void);
unsigned int Read_Data_Fault_Status_Register(void);
unsigned int Read_Multiprocessor_affinity_register(void);
void wfe(void);
void sev(void);
void wfi(void);
void instructionsyncbarrier(void);
void print_coreid(void);
unsigned int Get_current_coreid(void);
void datasyncbarrier(void);
void datamembarrier(void);
void enable_L2_cache(void);
void enable_instruction_cache(void);
void enable_L1_cache(void);
void print_arm_registers(void);
void print_arm_sys_ctrl_register(void);
void print_arm_aux_ctrl_register(void);
void invalidate_tlb(void);
void invalidate_caches(void);
void enable_mmu_caches_branch_prediction(void);
void disable_mmu(void);
void enable_mmu(void);
void disable_mmu_caches_branch_prediction(void);
void set_domain(unsigned int domain, unsigned int domain_type);
void write_translation_table_base_register(unsigned int addr, unsigned int flags);
unsigned int strex(unsigned int addr, unsigned int value);
unsigned int ldrex(unsigned int addr);

#endif //__ARM_UTILITY__H__