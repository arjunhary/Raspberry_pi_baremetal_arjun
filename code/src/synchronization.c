#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/synchronization.h"


#define SEMAPHORE_LOCKED 0
#define SEMAPHORE_UNLOCKED 1

int mutex=1;

void init_critical_section(void)
{
	mutex = 1;
}

void enter_critical_section(int* lock)
{
	int status = -1;
	do
	{
		//Somebody already has the lock. This is a busy wait loop for now
		while(1)
		{
			if(ldrex((unsigned int)lock) == SEMAPHORE_UNLOCKED)
			{
				break;
			}
		}
		//Lock the semaphore by setting it to 0.
		status = strex((unsigned int)lock,SEMAPHORE_LOCKED);	
	}while(status!=0);
	
	datamembarrier();
}

void sem_dec(int* lock)
{
	int status = -1;
	volatile int val = -1;
	do
	{
		while(1)
		{
			//If semphore value is 0,it is locked. Wait till not 0			
			val = ldrex((unsigned int)lock);
			if(val != SEMAPHORE_LOCKED)
			{
				break;
			}
		}
		//Decrement the value of the semaphore
		val--;
		//Lock the semaphore by setting it to 0.
		status = strex((unsigned int)lock,val);	
	}while(status!=0);
	datamembarrier();
	//uart_printf("\nInside semaphore decrement : %d", val);
	
}

void exit_critical_section(int* lock)
{
	int status = -1;
	do
	{
		//Check if already unlocked. Then nothing to do
		if(ldrex((unsigned int)lock) == SEMAPHORE_UNLOCKED)
		{
			return;
		}
		//Unlock the semaphore by setting to 1
		status = strex((unsigned int)lock,SEMAPHORE_UNLOCKED);		
	}while(status != 0);
	
	datamembarrier();
}

void sem_inc(int* lock)
{
	int status = -1;
	volatile int val = -1;
	do
	{
		val = ldrex((unsigned int)lock);
		//Check if already unlocked. Then nothing to do
		val++;
		//Increment the value of the semaphore
		status = strex((unsigned int)lock,val);		
	}while(status != 0);	
	datamembarrier();
}

void sem_init(int* lock, unsigned int value)
{
	*lock = value;
	datamembarrier();
}



void ex_tests(void)
{
	int lock = 500;
	int lock1 = 1000;
	uart_printf("\nAddress Lock: %x, Lock1 : %x, mutex : %x",&lock,&lock1,&mutex);
	uart_printf("\nFirst strex status : %d",strex((unsigned int)&lock1,1500));
	uart_printf("\nLDREX of lock1: %d", ldrex((unsigned int)&lock1));	
	uart_printf("\nSTREX status of lock1: %d", strex((unsigned int)&lock1,1500));	
	uart_printf("\nLDREX of lock: %d", ldrex((unsigned int)&lock));	
	uart_printf("\nSTREX status of lock: %d", strex((unsigned int)&lock,1000));
	uart_printf("\nLDREX of mutex: %d", ldrex((unsigned int)&mutex));	
	uart_printf("\nSTREX status of mutex: %d", strex((unsigned int)&mutex,1));	
	uart_printf("\nAfter strex ldrex of lock: %d", ldrex((unsigned int)&lock));
	//uart_printf("\nSecond ldrex: %d", ldrex((unsigned int)&lock));	
	//uart_printf("\nSecond ldrex: %d", ldrex((unsigned int)&lock1));		
	//uart_printf("\nSecond ldrex: %d", ldrex((unsigned int)&mutex));	
}




