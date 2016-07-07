#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/SystemTimer.h"
#include "./../include/interrupts.h"
#include "./../include/utility.h"

unsigned char uart_buf[UART_BUF_SIZE];

void SET_BREAKPOINT(void)
{
		__asm__("BKPT");
}


void arm_jtag_init(void)
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


/*void PUT32(unsigned int addr, unsigned int value)
{
	*(unsigned int*)addr= value;
}

unsigned int GET32(unsigned int addr)
{
	return (*(unsigned int*)addr);
}*/


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

void memset(void* ptr,char value,int length_in_bytes)
{
	int i=0;
	char* char_ptr = (char*)ptr;
	for(i=0;i<length_in_bytes;i++)
	{
		char_ptr[i]=value;
	}
	
}

int get_current_time(void)
{
	return(GET32(SYSTEM_TIMER_CL0_REG_LSB));
}

int calculate_execution_time(unsigned int start_time)
{	
	unsigned int execution_time = GET32(SYSTEM_TIMER_CL0_REG_LSB) - start_time;
	uart_print_string_newline("Execution time:");
	uart_print_number(execution_time);
	uart_print_string("uS");
	return 0;
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


