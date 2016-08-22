#ifndef __UTILITY__H__
#define __UTILITY__H__

extern unsigned int time_ms;

extern void dummy ( unsigned int );
void PUT32 ( unsigned int , unsigned int );
unsigned int GET32(unsigned int addr);
extern void BRANCHTO (unsigned int);

#define BITS_PER_BYTE 8

void SET_BREAKPOINT(void);

unsigned int Read_ARM_Auxiliary_Control_Register(void);
unsigned int Read_Watchpoint_control_Register0(void);
unsigned int Read_ARM_System_Control_Register(void);
unsigned int Read_ARM_Auxiliary_Control_Register(void);
unsigned int Read_Breakpoint_control_Register0(void);
unsigned int Read_Interrupt_status_Register(void);
unsigned int Read_CPSR_Register(void);
void enable_L2_cache(void);
void enable_instruction_cache(void);
void enable_data_and_unified_cache(void);
void memcpy(void*src,void*dest,int length);
void memset(void* ptr,char value,int length_in_bytes);
void memfill_pattern(void* dest,char* src,int length_in_bytes, int pattern_length_in_bytes);
int LEDTurnon(void);
int LEDTurnoff(void);
int LEDInit(void);
int time_sleep(unsigned int time_ms);
void arm_jtag_init(void);
int get_current_time(void);
int calculate_execution_time(unsigned int start_time);
void print_arm_registers(void);
void print_arm_sys_ctrl_register(void);


#endif

