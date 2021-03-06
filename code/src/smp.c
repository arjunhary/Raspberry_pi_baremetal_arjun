#include "./../include/smp.h"
#include "./../include/mmu.h"
#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/sysconfig.h"
#include "./../include/fifo.h"
#include "./../include/synchronization.h"



int core_to_execute = 0;
int shared_int = 0;
extern int mutex;
extern fifo_t fifo;
int signal = 0;
#define FIFO_LOOP_COUNT 200

void core0_function(void)
{
	sem_inc(&signal);
	fifo_producer(&fifo,FIFO_LOOP_COUNT);
	while(1)
	{
		dummy(0);
		/*if(Get_current_coreid() == core_to_execute)
		{
			#ifdef UART_PRINT_COREID
			uart_printf("\nMPIDR Reg: %x", Read_Multiprocessor_affinity_register());
			print_coreid();
			#endif
			core_to_execute = ((core_to_execute + 1)%CORES);
			instructionsyncbarrier();
			sev();
		}
		else
		{
			wfe();
		}
		enter_critical_section(&mutex);
		uart_printf("\nCore ID: %d , Number: %d", Get_current_coreid(),shared_int);
		shared_int++;
		exit_critical_section(&mutex);*/
		
	}
}

void core0_idleloop(void)
{
	while(1)
	{
		dummy(0);
	}
}

void core1_function(void)
{
	fifo_consumer(&fifo,FIFO_LOOP_COUNT*2);
	while(1)
	{
		dummy(0);
		/*if(Get_current_coreid() == core_to_execute)
		{
			#ifdef UART_PRINT_COREID
			uart_printf("\nMPIDR Reg: %x", Read_Multiprocessor_affinity_register());
			print_coreid();
			#endif
			//factorial(50);	
			core_to_execute = ((core_to_execute + 1)%CORES);
			instructionsyncbarrier();
			sev();
		}
		else
		{
			wfe();
		}
		enter_critical_section(&mutex);
		uart_printf("\nCore ID: %d , Number: %d", Get_current_coreid(),shared_int);
		shared_int++;
		exit_critical_section(&mutex);*/
	}
}

void core2_function(void)
{
	sem_dec(&signal);
	fifo_producer(&fifo,FIFO_LOOP_COUNT);
	while(1)
	{
		dummy(0);
		/*if(Get_current_coreid() == core_to_execute)
		{
			#ifdef UART_PRINT_COREID
			uart_printf("\nMPIDR Reg: %x", Read_Multiprocessor_affinity_register());
			print_coreid();
			#endif
			//factorial(50);				
			core_to_execute = ((core_to_execute + 1)%CORES);
			instructionsyncbarrier();
			sev();
		}
		else
		{
			wfe();
		}
		enter_critical_section(&mutex);
		uart_printf("\nCore ID: %d , Number: %d", Get_current_coreid(),shared_int);
		shared_int++;
		exit_critical_section(&mutex);*/
	}
}

void core3_function(void)
{
	while(1)
	{
		dummy(0);
		/*if(Get_current_coreid() == core_to_execute)
		{
			#ifdef UART_PRINT_COREID
			uart_printf("\nMPIDR Reg: %x", Read_Multiprocessor_affinity_register());
			print_coreid();
			#endif
			//factorial(50);				
			core_to_execute = ((core_to_execute + 1)%CORES);
			instructionsyncbarrier();
			sev();
		}
		else
		{
			wfe();
		}
		enter_critical_section(&mutex);
		uart_printf("\nCore ID: %d , Number: %d", Get_current_coreid(),shared_int);
		shared_int++;
		exit_critical_section(&mutex);*/
	}
}


int multicore_init(void)
{
	int core_index = 0;
	int timeout = 100; //100 ms
	
	for(core_index = 1 ; core_index < CORES; core_index++)
	{
		datasyncbarrier();
		
		unsigned int mailbox_clear = (ARM_LOCAL_MAILBOX3_CLR0 + (core_index* 0x10));
		
		//Check if the mailbox is clear
		while(timeout != 0)
		{
			if(GET32(mailbox_clear) == 0)
			{				
				break;
			}
			else
			{
				time_sleep(1);
				timeout--;
			}
		}
		
		//put the address of the function in mailbox 3
		PUT32((ARM_LOCAL_MAILBOX3_SET0 + (core_index*0x10)),(unsigned int)&start_secondary_core);
		
		timeout = 100;
		//Check if the mailbox is clear
		while(timeout != 0)
		{
			if(GET32(mailbox_clear) == 0)
			{
				uart_printf("\nCore %u Initialization Successful",core_index);
				datamembarrier();
				break;
			}
			else
			{
				time_sleep(1);
				timeout--;
			}
		}

	}
	return 1;
}

void sysinit_secondary(void)
{
	int core_id = Get_current_coreid();
	
	//inititalize the MMU
	mmu_init();
	
	//Clear the mailbox so that core 0 can proceed
	PUT32((ARM_LOCAL_MAILBOX3_CLR0 + (core_id*0x10)), 0);
	
	
	if(core_id == 1)
	{
		core1_function();
	}
	else if(core_id == 2)
	{
		core2_function();
	}
	else if(core_id == 3)
	{
		core3_function();
	}
}
