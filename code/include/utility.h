#ifndef __UTILITY__H__
#define __UTILITY__H__

#include <stdint.h>

#define NULL 0

extern unsigned int time_ms;

void BRANCHTO(unsigned int addr);

#define BITS_PER_BYTE 8
#define sizeofvar(x) ((char*)(&x+1) - (char*)&x)
//#define sizeof(type) ((type*)0 + 1)

//Timer functions
int time_sleep(unsigned int time_ms);
uint64_t get_current_time(void);
unsigned int calculate_execution_time(uint64_t start_time);
int time_sleep_us(unsigned int time_us);

//Serial parser
void serial_command_parse(char* cmd, int cmd_length);

//Memory functions
void memcpy(void*src,void*dest,int length);
void memset(void* ptr,char value,int length_in_bytes);
void memfill_pattern(void* dest,char* src,int length_in_bytes, int pattern_length_in_bytes);
int memcmp(void* addr1, void* addr2, int size);
int get_memory_alignment(unsigned int addr);
void memory_alignment_tests(void);
void memory_tests(void);


//LED functions
int LEDTurnon(void);
int LEDTurnoff(void);
int LEDInit(void);
void LED_fastblink(void);

//ARM JTAG Init
void arm_jtag_init(void);


//String functions
int strlen(char* str);
int strcmp(char* str1, char*str2);
void string_tests(void);
int strncmp(char* str1, char*str2, int num_of_bytes);

//Misc
int factorial(int n);
void cache_test(void);
void sizeof_tests(void);

//malloc

enum
{
	MALLOC_STATUS_FREE = 0xDEADBEEF,
	MALLOC_STATUS_OCCUPIED = 0xBEEFDEAD,
}malloc_status;

typedef struct malloc_metadata{
	unsigned int status;
	unsigned int size;
}malloc_metadata_t;

void print_heap_size(void);
void malloc_init(void);
void* malloc(int number_of_bytes);
void free(void* ptr);
void print_malloc_blocks(void);
void malloc_tests(void);
void malloc_garbage_collect(void);
#endif

