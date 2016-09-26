#ifndef __UTILITY__H__
#define __UTILITY__H__

extern unsigned int time_ms;

extern void dummy ( unsigned int );
void PUT32(unsigned int addr, unsigned int value);
unsigned int GET32(unsigned int addr);
void BRANCHTO(unsigned int addr);

#define BITS_PER_BYTE 8
#define sizeofvar(x) ((char*)(&x+1) - (char*)&x)
//#define sizeof(type) ((type*)0 + 1)

void memcpy(void*src,void*dest,int length);
void memset(void* ptr,char value,int length_in_bytes);
void memfill_pattern(void* dest,char* src,int length_in_bytes, int pattern_length_in_bytes);
int LEDTurnon(void);
int LEDTurnoff(void);
int LEDInit(void);
int time_sleep(unsigned int time_ms);
void arm_jtag_init(void);
void get_current_time(void);
int calculate_execution_time(void);
int strlen(char* str);
int strcmp(char* str1, char*str2);
void string_tests(void);
int factorial(int n);
int memcmp(void* addr1, void* addr2, int size);
void memory_tests(void);
void cache_test(void);
int get_memory_alignment(unsigned int addr);
void memory_alignment_tests(void);
void sizeof_tests(void);
void print_heap_size(void);
void LED_fastblink(void);



#endif

