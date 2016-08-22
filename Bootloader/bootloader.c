#include "GPIO.h"
#include "MiniUART.h"
#include "SystemTimer.h"
#include "interrupts.h"

#define MAX_DIGITS_IN_32_BIT_NUMBER 10

extern void dummy ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
extern void BRANCHTO (unsigned int);

extern unsigned int GET32 ( unsigned int );
extern void enable_irq(void);
extern void enable_fiq(void);
int xmodem_recv(void*ptr);
int xmodem_send(void*ptr, int byte_count);

void memcpy(void*src,void*dest,int length);

unsigned char* bootloader_ptr = (unsigned char*) 0x8000;

unsigned int time_ms = 1000;
unsigned char uart_buf[UART_BUF_SIZE];

int UARTInit(void);
int LEDInit(void);
int uart_recvchar(unsigned char* data,unsigned int timeout_ms);
void uart_putchar(unsigned char c);
void uart_print_string(char* str);
int time_sleep(unsigned int time_ms);
int enable_system_timer_irq_interrupt(void);
int enable_system_timer_fiq_interrupt(void);
void init_uart_buf(void);
void arm_jtag_int(void);

unsigned char calc_checksum(unsigned char* ptr,int number_of_bytes)
{
	unsigned char sum = 0;
	int i =0;
	for(i = 0; i <number_of_bytes ; i++)
	{
		sum+= ptr[i];
	}
	return sum;
}

void arm_jtag_int(void)
{
	//Select alt fn 5 for gpio4,gpio5
	volatile GPIO_Funtion_Select0_t gpiofnsel0;
	gpiofnsel0 = (GPIO_Funtion_Select0_t)GET32(GPIO_REG_GPFSEL0);
	gpiofnsel0.mBits.FunctionSelGPIO4 = GPIO_SET_alternate_function_5;
	gpiofnsel0.mBits.FunctionSelGPIO5 = GPIO_SET_alternate_function_5;
	PUT32(GPIO_REG_GPFSEL0,gpiofnsel0.mAsU32);
	
	//Select alt fn 5 for gpio13
	volatile GPIO_Funtion_Select1_t gpiofnsel1;
	gpiofnsel1 = (GPIO_Funtion_Select1_t)GET32(GPIO_REG_GPFSEL1);
	gpiofnsel1.mBits.FunctionSelGPIO13 = GPIO_SET_alternate_function_5;
	PUT32(GPIO_REG_GPFSEL1,gpiofnsel1.mAsU32);
	
	//Alt fn 4 for 22,27
	volatile GPIO_Funtion_Select2_t gpiofnsel2;
	gpiofnsel2 = (GPIO_Funtion_Select2_t)GET32(GPIO_REG_GPFSEL2);
	gpiofnsel2.mBits.FunctionSelGPIO22 = GPIO_SET_alternate_function_4;
	gpiofnsel2.mBits.FunctionSelGPIO27 = GPIO_SET_alternate_function_4;
	PUT32(GPIO_REG_GPFSEL2,gpiofnsel2.mAsU32);	
}

void memcpy(void* dest,void *src,int length)
{
	int i=0;
	char* char_src = (char*)src;
	char* char_dest = (char*)dest;
	for(i=0;i<length;i++)
	{
		*char_dest=*char_src;
		char_dest++;
		char_src++;
	}
	
}

void init_uart_buf(void)
{
	int i =0;
	for(i=0;i<UART_BUF_SIZE;i++)
	{
		uart_buf[i] = '0';
	}
}


void uart_print_number(int num)
{
	int index = 0;
	int i = 0;
	char num_to_char[MAX_DIGITS_IN_32_BIT_NUMBER]= {'0','0','0','0','0','0','0','0','0','0'};
	if(num == 0)
	{
		uart_putchar('0');
	}
	else
	{
		while(num != 0)
		{
			num_to_char[index] = (num%10) + '0';
			num = num/10;
			index++;
		}
		for(i =index-1; i >=0 ; i--)
		{
			uart_putchar(num_to_char[i]);
			if((i%3 == 0) && (i!=0))
			{
				uart_putchar(',');
			}
		}
	}
}

void uart_print_string_newline(char* str)
{
	uart_putchar(0x0D);
	uart_putchar(0x0A);
	while(*str != '\0')
	{
		uart_putchar(*str);
		str++;
	}	
}

void uart_print_string(char* str)
{
	while(*str != '\0')
	{
		uart_putchar(*str);
		str++;
	}
	
}

int time_sleep(unsigned int time_ms)
{
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	
	while(1)
	{
		if((GET32(SYSTEM_TIMER_CL0_REG_LSB) - start_tick) > (time_ms*1000))
		{
			break;
		}
	}
	
	return 0;
}

void flush_uart_rx_buffer(void) {
    unsigned char c;
    while (uart_recvchar(&c, 100) == 0);
}



int uart_recvchar(unsigned char* data,unsigned int timeout_ms)
{
	volatile AUX_MU_STAT_Reg_t MUstatus;
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	
	AUX_MU_IO_Reg_t MUIoReg;	
	while(1)
	{
		MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
		if(MUstatus.mbits.symbol_available == 1)
		{
			break;
		}
		if((GET32(SYSTEM_TIMER_CL0_REG_LSB) - start_tick) > (timeout_ms*1000))
		{
			return -1;
		}				
	}
	MUIoReg.mAsU32 = GET32(AUX_MU_IO_REG);
	*data = (char)(MUIoReg.receive_data.received_data);
	return 0;
}

void uart_putchar(unsigned char c)
{
	volatile AUX_MU_STAT_Reg_t MUstatus;
	volatile AUX_MU_IO_Reg_t MUIoReg;
	MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
	while(1)
	{
		MUstatus.mAsU32 = GET32(AUX_MU_STAT_REG);
		if(MUstatus.mbits.transmit_space_available == 1)
		{
			break;
		}
	}
		
	MUIoReg.mAsU32 = GET32(AUX_MU_IO_REG);
	MUIoReg.transmit_data.transmit_data = c;
	PUT32(AUX_MU_IO_REG,MUIoReg.mAsU32);	
}

int UARTInit(void)
{
	//Enable MiniUART
	AUX_Enable_Reg_t auxenablereg;
	auxenablereg = (AUX_Enable_Reg_t)GET32(AUX_ENABLE_REG);
	auxenablereg.mbits.mini_UART_enable = 1;
	PUT32(AUX_ENABLE_REG,auxenablereg.mAsU32);
	
	//Select Alternate function 5 for GPIO14 and 15
	volatile GPIO_Funtion_Select1_t gpiofnsel1;
	gpiofnsel1 = (GPIO_Funtion_Select1_t)GET32(GPIO_REG_GPFSEL1);
	gpiofnsel1.mBits.FunctionSelGPIO14 = GPIO_SET_alternate_function_5;
	gpiofnsel1.mBits.FunctionSelGPIO15 = GPIO_SET_alternate_function_5;
	PUT32(GPIO_REG_GPFSEL1,gpiofnsel1.mAsU32);
	
	//Disable transmitter and receiver
	volatile AUX_MU_CNTL_Reg_t cntrlreg;
	cntrlreg = (AUX_MU_CNTL_Reg_t)GET32(AUX_MU_CNTL_REG);
	cntrlreg.mbits.receiver_enable = 0;
	cntrlreg.mbits.transmitter_enable = 0;
	PUT32(AUX_MU_CNTL_REG,cntrlreg.mAsU32);
	
	//Clear Interrrupts
	volatile AUX_MU_IER_Reg_t IERReg;
	IERReg = (AUX_MU_IER_Reg_t)GET32(AUX_MU_IER_REG);
	IERReg.Interrupt_bits.Enable_transmit_intr = 0;
	IERReg.Interrupt_bits.Enable_receive_intr  = 0;
	PUT32(AUX_MU_IER_REG,IERReg.mAsU32);
	
	//Clear the FIFOs
	volatile AUX_MU_IIR_Reg_t IIRReg;
	IIRReg = (AUX_MU_IIR_Reg_t)GET32(AUX_MU_IIR_REG);
	IIRReg.mbits.Interrupt_ID = 3;
	IIRReg.mbits.fifo_enables = 3;
	PUT32(AUX_MU_IIR_REG,IIRReg.mAsU32);
	
	//Set the data size to 8 bit
	volatile AUX_MU_LCR_Reg_t LCRReg;
	LCRReg = (AUX_MU_LCR_Reg_t)GET32(AUX_MU_LCR_REG);
	LCRReg.mbits.data_size = 3;
	LCRReg.mbits.dlab_acess = 0;
	PUT32(AUX_MU_LCR_REG,LCRReg.mAsU32);
	
	//Set the baud rate to 115200
	volatile AUX_MU_BAUD_RATE_Reg_t baudreg;
	baudreg = (AUX_MU_BAUD_RATE_Reg_t)GET32(AUX_MU_BAUD_REGISTER);
	baudreg.mbits.baud_rate = 270; //115200 baud rate
	PUT32(AUX_MU_BAUD_REGISTER,baudreg.mAsU32);
	
	//Enable transmitter and receiver
	cntrlreg = (AUX_MU_CNTL_Reg_t)GET32(AUX_MU_CNTL_REG);
	cntrlreg.mbits.receiver_enable = 1;
	cntrlreg.mbits.transmitter_enable = 1;
	PUT32(AUX_MU_CNTL_REG,cntrlreg.mAsU32);
	
	return 0;
}

int LEDInit(void)
{
	volatile GPIO_Funtion_Select4_t GPIOFnsel4;
	GPIOFnsel4 = (GPIO_Funtion_Select4_t)GET32(GPIO_REG_GPFSEL4);
	GPIOFnsel4.mBits.FunctionSelGPIO47 = GPIO_SET_AS_OUTPUT;
	PUT32(GPIO_REG_GPFSEL4,GPIOFnsel4.mAsU32);
	return 0;
}

int LEDTurnon(void)
{
	volatile GPIO_Output_Set_Register1_t GPIOSetReg1;
	GPIOSetReg1 = (GPIO_Output_Set_Register1_t)GET32(GPIO_REG_GPSET1);
	GPIOSetReg1.mBits.SetGPIO47 = 1;
	PUT32(GPIO_REG_GPSET1,GPIOSetReg1.mAsU32);
	return 0;
}

int LEDTurnoff(void)
{
	volatile GPIO_Output_Clear_Register1_t GPIOClearReg1;
	GPIOClearReg1 = (GPIO_Output_Clear_Register1_t)GET32(GPIO_REG_GPCLR1);
	GPIOClearReg1.mBits.ClearGPIO47 = 1;
	PUT32(GPIO_REG_GPCLR1,GPIOClearReg1.mAsU32);
	return 0;
}

int disable_system_timer_interrupt(void)
{
	Interrupt_IRQ_Disable1_Reg_t intrbasdisable;
	intrbasdisable = (Interrupt_IRQ_Disable1_Reg_t) GET32(INTERRUPT_DISABLE_IRQ1);
	intrbasdisable.mBits.irq1 = 1;
	PUT32(INTERRUPT_DISABLE_IRQ1,intrbasdisable.mAsU32);
	return 0;
}

int enable_system_timer_irq_interrupt(void)
{
	//Set the system timer register
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Set the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	//Set the Interrupt senable register
	Interrupt_IRQ_Enable1_Reg_t intrirqenable;
	intrirqenable = (Interrupt_IRQ_Enable1_Reg_t) GET32(INTERRUPT_ENABLE_IRQ1);
	intrirqenable.mBits.irq1 = 1;
	PUT32(INTERRUPT_ENABLE_IRQ1,intrirqenable.mAsU32);
	
	enable_irq();
	
	return 0;
}

int enable_system_timer_fiq_interrupt(void)
{
	//Set the system timer register
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Set the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	//Set the Interrupt senable register
	Interrupt_FIQ_control_Reg_t intrfiqenable;
	intrfiqenable = (Interrupt_FIQ_control_Reg_t) GET32(INTERRUPT_FIQ_CONTROL);
	intrfiqenable.mBits.FIQ_enable = 1;
	intrfiqenable.mBits.FIQ_source = 1;
	PUT32(INTERRUPT_FIQ_CONTROL,intrfiqenable.mAsU32);
	
	enable_fiq();
	
	return 0;
}



void c_irq_handler (void)
{
	static int led_on = 0;

	//Reset the counter match register	
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Clear the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	if(led_on == 0)
	{
		LEDTurnon();
		led_on = 1;
	}
	else
	{
		LEDTurnoff();
		led_on = 0;
	}
}

void c_fiq_handler (void)
{
	static int led_on = 0;

	//Reset the counter match register	
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	PUT32(SYSTEM_TIMER_CMP1,(start_tick + (time_ms*1000)));
	
	//Clear the system timer match register
	System_timer_control_Reg_t systimctrl;
	systimctrl = (System_timer_control_Reg_t) GET32(SYSTEM_TIMER_CS_REG);
	systimctrl.mBits.System_timer1_match = 1;
	PUT32(SYSTEM_TIMER_CS_REG,systimctrl.mAsU32);
	
	if(led_on == 0)
	{
		LEDTurnon();
		led_on = 1;
	}
	else
	{
		LEDTurnoff();
		led_on = 0;
	}
}

int xmodem_send(void*ptr , int byte_count)
{
	unsigned char temp= '0';
	//int i =0;
	while(1)
	{
		if(uart_recvchar(&temp,1000) == 0)
		{
			if(temp == XMODEM_NAK)
			{
				break;
			}
		}		
	}
	
	
	//for(i=0;i < byte_count/)
	
	
	
	
	return 0;
}


int xmodem_recv(void* recv_ptr)
{
	unsigned int char_index = 1;
	unsigned char seq_num = 1;
	unsigned int byte_count = 0;
	int SOH_received = -1;

	LEDTurnon();


	//Wait till we receive a character
	while(1)
	{		
		uart_recvchar(&uart_buf[0],10000);
		
		if(uart_buf[0] == XMODEM_SOH)
		{
			SOH_received = 1;
			while(char_index < UART_BUF_SIZE)
			{
				if(uart_recvchar(&uart_buf[char_index],1000) == 0)
				{
					char_index++;
				}
				else
				{
					break;
				}
			}
			//At this point we have the whole packet or we dont
			if((uart_buf[1] ==seq_num) && (uart_buf[2] == (unsigned char)(~seq_num)) && (uart_buf[131] == (calc_checksum(&uart_buf[3],XMODEM_PAYLOAD_SIZE))))
			{
				SOH_received = 0;
				seq_num++;
				char_index = 1;				
				memcpy((recv_ptr+byte_count),&uart_buf[3],XMODEM_PAYLOAD_SIZE);
				byte_count += XMODEM_PAYLOAD_SIZE;
				init_uart_buf();
				uart_putchar(XMODEM_ACK);
			}
			else
			{
				SOH_received = 0;
				char_index = 1;
				init_uart_buf();
				flush_uart_rx_buffer();
				uart_putchar(XMODEM_NAK);
			}
		}
		else if(uart_buf[0] == XMODEM_EOT)
		{
			LEDTurnoff();
			uart_putchar(XMODEM_ACK);
			SOH_received = -1;
			return byte_count;
		}
		else if(uart_buf[0] == XMODEM_CAN)
		{
			char_index = 1;
			seq_num = 1;
			byte_count = 0;
			uart_putchar(XMODEM_ACK);
			SOH_received = -1;
		}
		else if(SOH_received == -1)
		{
			uart_putchar(XMODEM_NAK);
		}
	}
	
	return 0;
	
}


int notmain(void)
{
	unsigned int byte_count = 0;
	LEDInit();
	UARTInit();
	arm_jtag_int();
	init_uart_buf();
	
	uart_print_string_newline("Send bin file to boot");	
	byte_count= xmodem_recv(bootloader_ptr);
	uart_print_string_newline("File size sent:");
	uart_print_number(byte_count);
	uart_print_string_newline("Booting bin file");
	time_sleep(1000);
	
	BRANCHTO((unsigned int)bootloader_ptr);
	
		
	while(1)
	{
		dummy(0);
	}
	return 0;
	
}
