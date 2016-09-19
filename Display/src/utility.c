#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/SystemTimer.h"
#include "./../include/interrupts.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/sysconfig.h"
#include <stdarg.h>


unsigned char uart_buf[UART_BUF_SIZE];

unsigned int current_time_lsb = 0;
unsigned int current_time_msb = 0;

int factorial(int n)
{
	uart_printf("\nNumber % %d\t Stack Address : %x",n ,&n);
	if(n == 0)
	{	
		return 1;
	}
	return (n*factorial(n-1));
}


int strlen(char* str)
{
	int length = 0;
	while(*str)
	{
		length++;
		str++;
	}
	return length;
}

int strcmp(char* str1, char*str2)
{
	while((*str1) && (*str2))
	{
		if(*str1 != *str2)
		{
			return 0;
		}
		str1++;
		str2++;
	}
	if((*str1 == '\0') && (*str2 == '\0'))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void arm_jtag_init(void)
{
	//Select alt fn 5 for gpio4,gpio5
	GPIO_Funtion_Select0_t gpiofnsel0;
	gpiofnsel0 = (GPIO_Funtion_Select0_t)GET32(GPIO_REG_GPFSEL0);
	gpiofnsel0.mBits.FunctionSelGPIO4 = GPIO_SET_alternate_function_5;
	gpiofnsel0.mBits.FunctionSelGPIO5 = GPIO_SET_alternate_function_5;
	PUT32(GPIO_REG_GPFSEL0,gpiofnsel0.mAsU32);
	
	//Select alt fn 5 for gpio13
	GPIO_Funtion_Select1_t gpiofnsel1;
	gpiofnsel1 = (GPIO_Funtion_Select1_t)GET32(GPIO_REG_GPFSEL1);
	gpiofnsel1.mBits.FunctionSelGPIO13 = GPIO_SET_alternate_function_5;
	PUT32(GPIO_REG_GPFSEL1,gpiofnsel1.mAsU32);
	
	//Alt fn 4 for 22,27
	GPIO_Funtion_Select2_t gpiofnsel2;
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


int memcmp(void* addr1, void* addr2, int size)
{
	int i =0;
	if((get_memory_alignment((unsigned int)addr1) >= 32) && (get_memory_alignment((unsigned int)addr2) >= 32) &&(size%(unsigned int)sizeof(int) ==0))
	{
		//uart_print_string_newline("Doing 32 bit compare");
		for(i =0; i< (size/(unsigned int)sizeof(int)); i++)
		{
			if(((int*)addr1)[i] != ((int*)addr2)[i])
			{
				return -1;
			}
			else
			{
				continue;
			}
		}
	}
	else if((get_memory_alignment((unsigned int)addr1) == 16) && (get_memory_alignment((unsigned int)addr2) == 16) &&(size%(unsigned int)sizeof(short) ==0))
	{
		//uart_print_string_newline("Doing 16 bit compare");
		for(i =0; i< (size/(unsigned int)sizeof(short)); i++)
		{
			if(((short*)addr1)[i] != ((short*)addr2)[i])
			{
				return -1;
			}
			else
			{
				continue;
			}
		}
	}
	else
	{
		//uart_print_string_newline("Doing 8 bit compare");
		for(i =0; i< size; i++)
		{
			if(((char*)addr1)[i] != ((char*)addr2)[i])
			{
				return -1;
			}
			else
			{
				continue;
			}
		}
	}

	return 0;
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

void memfill_pattern(void* dest,char* src,int length_in_bytes, int pattern_length_in_bytes)
{
	int i=0,j=0;
	char* char_ptr = (char*)dest;
	for(i=0;i< (length_in_bytes/pattern_length_in_bytes);i++)
	{
		for(j=0;j<pattern_length_in_bytes;j++)
		{
			char_ptr[j+i]=src[j];
		}
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

void get_current_time(void)
{
	current_time_lsb = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	current_time_msb = GET32(SYSTEM_TIMER_CL0_REG_MSB);
}

int calculate_execution_time(void)
{	
	unsigned int execution_time_lsb = GET32(SYSTEM_TIMER_CL0_REG_LSB) - current_time_lsb;
	unsigned int execution_time_msb = GET32(SYSTEM_TIMER_CL0_REG_MSB) - current_time_msb;
	uart_printf("\nExecution Time : %u , %u uS",execution_time_msb,execution_time_lsb);
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

void LED_fastblink(void)
{
	while(1)
	{
		LEDTurnon();
		time_sleep(200);
		LEDTurnoff();
		time_sleep(200);		
	}
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

void cache_test(void)
{
	get_current_time();
	memory_tests();
	calculate_execution_time();
	
	datamembarrier();
	enable_L1_cache();
	time_sleep(1000);
	get_current_time();
	memory_tests();
	calculate_execution_time();
	
	datamembarrier();
	enable_L2_cache();
	time_sleep(1000);
	get_current_time();
	memory_tests();
	calculate_execution_time();
	
}

int get_memory_alignment(unsigned int addr)
{
	int i =0;
	for(i = 1024; i >=8 ; i =i/2)
	{
		if((addr & (i-1)) == 0)
			return i;
	}
	return 0;
}


void memory_alignment_tests(void)
{
	uart_print_string_newline("Memory alignment tests:");
	unsigned int alignment = 0;
	unsigned int addr = 0x00000400;
	alignment = get_memory_alignment(addr);
	uart_print_number(alignment);
	uart_print_string(",");
	addr = 0x00000200;
	alignment = get_memory_alignment(addr);
	uart_print_number(alignment);
	uart_print_string(",");
	addr = 0x00000100;
	alignment = get_memory_alignment(addr);
	uart_print_number(alignment);
	uart_print_string(",");
	addr = 0x00000080;
	alignment = get_memory_alignment(addr);
	uart_print_number(alignment);
	uart_print_string(",");
	addr = 0x00000008;
	alignment = get_memory_alignment(addr);
	uart_print_number(alignment);
}

void memory_tests(void)
{
	int i =0,cmp_result[100];
	int loopcount = 5;
	int size = 0x10000;
	unsigned int* addr1 	=  (unsigned int*)0x10000000;
	unsigned int* addr2  	=  (unsigned int*)0x10010000;
	unsigned int* addr3 	=  (unsigned int*)0x10040010;
	unsigned int* addr4  	=  (unsigned int*)0x10050010;
	unsigned int* addr5 	=  (unsigned int*)0x10080008;
	unsigned int* addr6  	=  (unsigned int*)0x10090008;

	
	
	//Test1
	memset((void*)addr1,0xCD,size);
	memset((void*)addr2,0xCD,size);
	
	datamembarrier();
	
	//Time start
	get_current_time();
	for(i =0; i<loopcount;i++)
	{
		if(memcmp((char*)addr1, (char*)addr2,size)==0)
		{
			cmp_result[i] = 1;
		}
		else
		{
			cmp_result[i] = 0;
		}
	}
	//TIme calculate
	calculate_execution_time();
	
	if(cmp_result[0] == 1)
	{
		uart_print_string_newline("Memcmp successful");
	}
	else
	{
		uart_print_string_newline("Memcmp not successful");
	}
	
	//Test2
	memset((void*)addr3,0xAB,size);
	memset((void*)addr4,0xAB,size);
	
	datamembarrier();
	//Time start
	get_current_time();
	for(i =0; i<loopcount;i++)
	{
		if(memcmp((void*)addr3, (void*)addr4,size)==0)
		{
			cmp_result[i] = 1;
		}
		else
		{
			cmp_result[i] = 0;
		}
	}
	//TIme calculate
	calculate_execution_time();
	
	if(cmp_result[0] == 1)
	{
		uart_print_string_newline("Memcmp successful");
	}
	else
	{
		uart_print_string_newline("Memcmp not successful");
	}
	
	//Test3
	memset((void*)addr5,0xEF,size);
	memset((void*)addr6,0xEF,size);
	
	datamembarrier();
	//Time start
	get_current_time();
	for(i =0; i<loopcount;i++)
	{
		if(memcmp((void*)addr5, (void*)addr6,size)==0)
		{
			cmp_result[i] = 1;
		}
		else
		{
			cmp_result[i] = 0;
		}
	}
	//TIme calculate
	calculate_execution_time();
	
	if(cmp_result[0] == 1)
	{
		uart_print_string_newline("Memcmp successful");
	}
	else
	{
		uart_print_string_newline("Memcmp not successful");
	}
}

void string_tests(void)
{
	uart_print_string_newline("Length :");
	uart_print_number(strlen("Test"));
	uart_print_string_newline("Length :");
	uart_print_number(strlen("Test12345"));
	uart_print_string_newline("Compare:");
	uart_print_number(strcmp("Test12345","Test12345"));
	uart_print_string_newline("Compare:");
	uart_print_number(strcmp("Test1234","Test12345"));
}

extern unsigned int __heap_limit__;
extern unsigned int __heap_start__;
extern unsigned int __heap_size__;
extern unsigned int __stack_limit__;
extern unsigned int __stack_start__;
extern unsigned int __stack_size__;

void print_heap_size(void)
{
	uart_printf("\nHeap start : %x , size : %x",&(__heap_start__),&(__heap_size__));
}


void sizeof_tests(void)
{
	int a;
	short b;
	char c;
	int arr[10];
	uart_print_string_newline("sizeof tests:");
	uart_print_number(sizeofvar(a));
	uart_print_string(",");
	uart_print_number(sizeofvar(b));
	uart_print_string(",");
	uart_print_number(sizeofvar(c));
	uart_print_string(",");
	uart_print_number(sizeofvar(arr));
	uart_print_string(",");
	uart_print_number((unsigned int)sizeof(int));
	uart_print_string(",");
	uart_print_number((unsigned int)sizeof(short));
	uart_print_string(",");
	uart_print_number((unsigned int)sizeof(char));
}



