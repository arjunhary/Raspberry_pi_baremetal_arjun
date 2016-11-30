#include "./../include/GPIO.h"
#include "./../include/MiniUART.h"
#include "./../include/SystemTimer.h"
#include "./../include/interrupts.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/sysconfig.h"
#include "./../include/synchronization.h"
#include "./../include/bluefruit.h"
#include <stdarg.h>
#include <stdint.h>


unsigned char uart_buf[UART_BUF_SIZE];

int factorial(int n)
{
	uart_printf("\nNumber % %d\t Stack Address : %x",n ,&n);
	if(n == 0)
	{	
		return 1;
	}
	return (n*factorial(n-1));
}

//0 means not a match
//1 is a match
int strncmp(char* str1, char*str2, int num_of_bytes)
{
	int count = 0;
	while((*str1) && (*str2))
	{
		if((*str1 != *str2) || (count == num_of_bytes))
		{
			break;
		}		
		count++;		
		str1++;
		str2++;
	}
	if(count == num_of_bytes)
	{
		return 1;
	}
	else
	{
		return 0;
	}
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

void BRANCHTO(unsigned int addr)
{
    asm volatile("bx r0");
}

int memcmp(void* addr1, void* addr2, int size)
{
	int i =0;
	if((get_memory_alignment((unsigned int)addr1) >= 32) && (get_memory_alignment((unsigned int)addr2) >= 32) &&(size%(unsigned int)sizeof(int) ==0))
	{
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
	if((get_memory_alignment((unsigned int)dest) >= 32) && (get_memory_alignment((unsigned int)src) >= 32) &&(length%(unsigned int)sizeof(int) ==0))
	{
		uart_printf("\n Doing 32 bit copy");
		for(i=0;i<(length/(unsigned int)sizeof(int));i++)
		{
			*(int*)dest++=*(int*)src++;
		}
	}
	else if((get_memory_alignment((unsigned int)dest) == 16) && (get_memory_alignment((unsigned int)src) == 16) &&(length%(unsigned int)sizeof(short) ==0))
	{
		uart_printf("\n Doing 16 bit copy");
		for(i=0;i<(length/(unsigned int)sizeof(short));i++)
		{
			*(short*)dest++=*(short*)src++;
		}
	}
	else
	{
		uart_printf("\n Doing 8 bit copy");
		for(i=0;i<length;i++)
		{
			*(char*)dest++ = *(char*)src++;
		}
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
	int int_pattern = 0;
	short short_pattern = 0;
	if((get_memory_alignment((unsigned int)ptr) >= 32) && (length_in_bytes%(unsigned int)sizeof(int) ==0))
	{
		for(i =0; i< sizeof(int);i++)
		{
			((char*)&int_pattern)[i] = value;
		}
		for(i=0;i<(length_in_bytes/(unsigned int)sizeof(int));i++)
		{
			((int*)ptr)[i]=int_pattern;
		}
	}
	else if((get_memory_alignment((unsigned int)ptr) == 16) && (length_in_bytes%(unsigned int)sizeof(short) ==0))
	{
		for(i =0; i< sizeof(short);i++)
		{
			((char*)&short_pattern)[i] = value;
		}
		for(i=0;i<(length_in_bytes/(unsigned int)sizeof(short));i++)
		{
			((short*)ptr)[i]=short_pattern;
		}
	}
	else
	{
		for(i=0;i<length_in_bytes;i++)
		{
			((char*)ptr)[i]=value;
		}
	}
	
}

uint64_t get_current_time(void)
{
	volatile unsigned int lsb = 0;
	volatile unsigned int msb0 = 0;
	volatile unsigned int msb1 = 1;
	
	do
	{	msb0 = GET32(SYSTEM_TIMER_CL0_REG_MSB);
		lsb  = GET32(SYSTEM_TIMER_CL0_REG_LSB);
		msb1 = GET32(SYSTEM_TIMER_CL0_REG_MSB);
	}while(msb0 != msb1);
	return(((uint64_t)msb0 << 32)|lsb);
}

unsigned int calculate_execution_time(uint64_t start_time)
{	
	uint64_t current_time = get_current_time();
	uint32_t execution_time = current_time - start_time;
	uart_printf("\nExecution Time : %u uS",execution_time);
	return execution_time;
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

int time_sleep_us(unsigned int time_us)
{
	unsigned int start_tick = GET32(SYSTEM_TIMER_CL0_REG_LSB);
	
	while(1)
	{
		if((GET32(SYSTEM_TIMER_CL0_REG_LSB) - start_tick) > time_us)
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
		time_sleep(100);
		LEDTurnoff();
		time_sleep(100);		
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
	uint64_t start_time = get_current_time();
	memory_tests();
	calculate_execution_time(start_time);
	
	datamembarrier();
	enable_L1_cache();
	time_sleep(1000);
	start_time = get_current_time();
	memory_tests();
	calculate_execution_time(start_time);
	
	datamembarrier();
	enable_L2_cache();
	time_sleep(1000);
	start_time = get_current_time();
	memory_tests();
	calculate_execution_time(start_time);
	
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
	uint64_t start_time = 0;
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
	start_time = get_current_time();
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
	calculate_execution_time(start_time);
	
	if(cmp_result[0] == 1)
	{
		uart_printf("\nMemcmp successful");
	}
	else
	{
		uart_printf("\nMemcmp not successful");
	}
	
	//Test2
	memset((void*)addr3,0xAB,size);
	memset((void*)addr4,0xAB,size);
	
	datamembarrier();
	//Time start
	start_time = get_current_time();
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
	calculate_execution_time(start_time);
	
	if(cmp_result[0] == 1)
	{
		uart_printf("\nMemcmp successful");
	}
	else
	{
		uart_printf("\nMemcmp not successful");
	}
	
	//Test3
	memset((void*)addr5,0xEF,size);
	memset((void*)addr6,0xEF,size);
	
	datamembarrier();
	//Time start
	start_time = get_current_time();
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
	calculate_execution_time(start_time);
	
	if(cmp_result[0] == 1)
	{
		uart_printf("\nMemcmp successful");
	}
	else
	{
		uart_printf("\nMemcmp not successful");
	}
}

void string_tests(void)
{
	uart_printf("\nString tests...");
	uart_printf("\nlength of %s : %u","Test",strlen("Test"));
	uart_printf("\nlength of %s : %u","Test12345",strlen("Test12345"));
	uart_printf("\nCompare of %s and %s : %u","Test12345","Test12345",strcmp("Test12345","Test12345"));
	uart_printf("\nCompare of %s and %s : %u","Test1234","Test12345",strcmp("Test1234","Test12345"));
	uart_printf("\nCompare of first %d bytes of %s and %s : %u",5,"Test1234","Test12345",strncmp("Test1234","Test12345",5));		
	uart_printf("\nCompare of first %d bytes of %s and %s : %u",2,"ab","a",strncmp("ab","a",2));
	uart_printf("\nCompare of first %d bytes of %s and %s : %u",2,"ab","ab",strncmp("ab","ab",2));
	uart_printf("\nCompare of first %d bytes of %s and %s : %u",3,"abcd","abc",strncmp("abcd","abc",3));
	uart_printf("\nCompare of first %d bytes of %s and %s : %u",3,"abed","abc",strncmp("abed","abc",3));
}


void sizeof_tests(void)
{
	int a;
	short b;
	char c;
	double d =560.23;
	long e = 5*100;
	long long f = 9223372036854775807;
	int arr[10];
	uart_printf("\nsizeof tests...");
	uart_printf("\nMy size of int: %d, short: %d, char: %d, Double : %d , long: %d , Array: %d , long long :%d",sizeofvar(a),sizeofvar(b),sizeofvar(c),sizeofvar(d),sizeofvar(e),sizeofvar(arr),sizeofvar(f));
	uart_printf("\nsize of int: %d, short: %d, char: %d, Double : %d , long: %d , long long: %d",sizeof(int),sizeof(short),sizeof(char),sizeof(double),sizeof(long),sizeof(long long));
}


extern unsigned int __heap_limit__;
extern unsigned int __heap_start__;
extern unsigned int __heap_size__;
extern unsigned int __stack_limit__;
extern unsigned int __stack_start__;
extern unsigned int __stack_size__;

void print_heap_size(void)
{
	uart_printf("\nHeap start : %x , size : %x  End : %x",&(__heap_start__),&(__heap_size__), &(__heap_limit__));
}

void malloc_init(void)
{
	malloc_metadata_t* malloc_init = (malloc_metadata_t*)&(__heap_start__);
	malloc_init->status = MALLOC_STATUS_FREE;
	malloc_init->size = ((unsigned int)&(__heap_size__) - sizeof(malloc_metadata_t));
	print_heap_size();
}

void print_malloc_blocks(void)
{
	uart_printf("\nMalloc Block Status...");
	unsigned int heap_index = (unsigned int)&(__heap_start__);
	malloc_metadata_t* malloc_metadata;
	int block_index = 0;
	while(heap_index < (unsigned int)&(__heap_limit__))
	{
		block_index++;
		malloc_metadata = (malloc_metadata_t*)heap_index;
		uart_printf("\nBlock : %d , Address Start : %x, size % %u , Status : %x ",block_index,(unsigned int)malloc_metadata, malloc_metadata->size, malloc_metadata->status);
		heap_index += malloc_metadata->size + sizeof(malloc_metadata_t);
	}
}

void* malloc(int number_of_bytes)
{
	if(number_of_bytes == 0)
	{
		return NULL;
	}
	
	malloc_metadata_t *malloc_metadata,*malloc_metadata_newblock;
	
	unsigned int heap_index = (unsigned int)&(__heap_start__);
	while(heap_index < (unsigned int)&(__heap_limit__))
	{
		malloc_metadata = (malloc_metadata_t*)heap_index;
		
		if((malloc_metadata->status == MALLOC_STATUS_FREE) && (malloc_metadata->size >= number_of_bytes))
		{
			//We found the block that we need
			malloc_metadata_newblock = (malloc_metadata_t*)(heap_index + sizeof(malloc_metadata_t) + number_of_bytes);
			malloc_metadata_newblock->status = MALLOC_STATUS_FREE;
			malloc_metadata_newblock->size = (malloc_metadata->size - sizeof(malloc_metadata_t) - number_of_bytes);
			
			//Update the status of the old block
			malloc_metadata->status = MALLOC_STATUS_OCCUPIED;
			malloc_metadata->size   =  number_of_bytes;
			return (void*)(heap_index+sizeof(malloc_metadata_t));
		}
		else
		{
			//Move to the next block
			heap_index += malloc_metadata->size + sizeof(malloc_metadata_t);
		}
	}
	return NULL;
	
}

void free(void* ptr)
{
	if((ptr == NULL) || ((unsigned int)ptr < (unsigned int)&(__heap_start__)) || ((unsigned int)ptr > (unsigned int)&(__heap_limit__)))
	{
		uart_printf("\n Malloc Error: Trying to free Invalid address : %x",ptr);
		return;
	}
	
	malloc_metadata_t *malloc_metadata_free;
		
	malloc_metadata_free  = (malloc_metadata_t*)((unsigned int)ptr - sizeof(malloc_metadata_t));
	if(malloc_metadata_free->status == MALLOC_STATUS_OCCUPIED)
	{
		malloc_metadata_free->status = MALLOC_STATUS_FREE;
	}
	else
	{
		uart_printf("\nMalloc Error: Trying to free u : %x",ptr);
	}
}

void malloc_tests(void)
{
	void* ptr[12];
	void* ptr1[12];
	int i =0, j=0 , count = 0;
	
	//2d array of size 10x10
	unsigned int** twod_array;
	
	for(i = 0; i < 12; i++)
	{
		ptr1[i] = malloc(0x10000-8);
		uart_printf("\nMalloc address return : %x",ptr1[i]);
	}
	
	for(i = 0; i < 12; i++)
	{
		ptr[i] = malloc(0x100000-8);
		uart_printf("\nMalloc address return : %x",ptr[i]);
	}

	print_malloc_blocks();
	
	for(i = 0; i < 4; i++)
	{
		free(ptr[i]);
	}
	
	print_malloc_blocks();
	
	malloc_garbage_collect();
	
	for(i = 0; i < 4; i++)
	{
		free(ptr1[i]);
	}
	malloc_garbage_collect();
	
	for(i = 0; i< 100; i++)
	{
		malloc(0x100);
	}
	
	//test 2d array allocation
	twod_array = (unsigned int**)malloc(10*sizeof(unsigned int*));
	if(twod_array == NULL)
	{
		uart_printf("\n2d array allocation error");
	}
	
	for(i =0; i< 10; i++)
	{
		twod_array[i] = (unsigned int*)malloc(10*sizeof(unsigned int));
		if(twod_array[i] == NULL)
		{
			uart_printf("\nsub array 2d array allocation error");
		}
	}
	
	for(i =0; i< 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			twod_array[i][j] = count;
			count++;
		}
	}
		
	print_malloc_blocks();
	
	uart_printf("\n");
	for(i =0; i< 10; i++)
	{
		for(j = 0; j < 10; j++)
		{
			uart_printf(" %u ",twod_array[i][j]);
		}
	}
	
	for(i = 0; i< 10; i++)
	{
		free(twod_array[i]);
	}
	
	free(twod_array);
	malloc_garbage_collect();
	print_malloc_blocks();
}

void malloc_garbage_collect(void)
{
	uart_printf("\nEntering garbage collection...");
	malloc_metadata_t *malloc_metadata,*malloc_metadata_nextfreeblock;
	
	unsigned int heap_index = (unsigned int)&(__heap_start__);
	while(heap_index < (unsigned int)&(__heap_limit__))
	{
		malloc_metadata = (malloc_metadata_t*)heap_index;
		if(malloc_metadata->status == MALLOC_STATUS_OCCUPIED)
		{
			//Block is allocated nothing to do. Move to next block
			heap_index += (sizeof(malloc_metadata_t) + malloc_metadata->size);
		}
		else if(malloc_metadata->status == MALLOC_STATUS_FREE)
		{
			uart_printf("\nStart free block address : %x",malloc_metadata);
			//Move to next block. To check if it is free
			//malloc_metadata - holds current free block
			//malloc_metadata_nextfreeblock - holds next block if it is free
			heap_index += (sizeof(malloc_metadata_t) + malloc_metadata->size);
			malloc_metadata_nextfreeblock = (malloc_metadata_t*)heap_index;
			while(malloc_metadata_nextfreeblock->status == MALLOC_STATUS_FREE)
			{
				malloc_metadata->size += (malloc_metadata_nextfreeblock->size + sizeof(malloc_metadata_t));
				//Move to next block
				heap_index += (sizeof(malloc_metadata_t) + malloc_metadata_nextfreeblock->size);
				malloc_metadata_nextfreeblock = (malloc_metadata_t*)heap_index;
			}
		}
	}
}


//UART command parser
void serial_command_parse(char* cmd, int cmd_length)
{
	if((strncmp(cmd,"ble",3)) == 1)
	{
		uart_printf("\nReceived bluetooth command");
		if((strncmp(&cmd[4],"send",4)) == 1)
		{
			
		}
		else if((strncmp(&cmd[4],"recv",4)) == 1)
		{
			
		}
	}
}


