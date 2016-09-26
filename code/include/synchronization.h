#ifndef __SYNCHRONIZATION__H__
#define __SYNCHRONIZATION__H__

void ex_tests(void);
//Semaphore and mutex
void enter_critical_section(int*lock);
void exit_critical_section(int* lock);
void sem_inc(int* lock);
void sem_dec(int* lock);
void init_critical_section(void);



#endif //__SYNCHRONIZATION__H__