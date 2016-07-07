#ifndef __UTILITY__H__
#define __UTILITY__H__

extern unsigned int time_ms;

extern void dummy ( unsigned int );
//void PUT32 ( unsigned int addr, unsigned int value);
void PUT32 ( unsigned int , unsigned int );
unsigned int GET32(unsigned int addr);
//unsigned int GET32(unsigned int);
extern void BRANCHTO (unsigned int);

#define BITS_PER_BYTE 8

void SET_BREAKPOINT(void);

unsigned int Read_ARM_Auxiliary_Control_Register(void);
unsigned int Read_Watchpoint_control_Register0(void);
unsigned int Read_ARM_System_Control_Register(void);
unsigned int Read_ARM_Auxiliary_Control_Register(void);
unsigned int Read_Breakpoint_control_Register0(void);
void enable_L2_cache(void);
void enable_instruction_cache(void);
void enable_data_and_unified_cache(void);

void enable_irq(void);
void enable_fiq(void);
int xmodem_recv(void*ptr);
int xmodem_send(void*ptr, int byte_count);

void memcpy(void*src,void*dest,int length);
void memset(void* ptr,char value,int length_in_bytes);

int LEDTurnon(void);
int LEDTurnoff(void);
int UARTInit(void);
int LEDInit(void);
int uart_recvchar(unsigned char* data,unsigned int timeout_ms);
void uart_putchar(unsigned char c);
void uart_print_string(char* str);
int time_sleep(unsigned int time_ms);
int enable_system_timer_irq_interrupt(void);
int enable_system_timer_fiq_interrupt(void);
void init_uart_buf(void);
void arm_jtag_init(void);
void uart_print_number(int num);
void uart_print_number_int_hex(unsigned int num);
void uart_print_string_newline(char* str);
int UART_disable_transmitter_and_receiver(void);
int UART_enable_transmitter_and_receiver(void);
int UART_clear_FIFOs(void);
int wait_till_transmitter_done(void);
int get_current_time(void);
int calculate_execution_time(unsigned int start_time);
int uart_tests(void);


#endif

