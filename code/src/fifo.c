#include "./../include/MiniUART.h"
#include "./../include/utility.h"
#include "./../include/arm_utility.h"
#include "./../include/sysconfig.h"
#include "./../include/synchronization.h"
#include "./../include/fifo.h"

/*
FIFO notes:
1) Supports multiple producers and consumers
2) Circular FIFO has the boundary case problem of detecting full and empty. So currently a FIFO of size n
supports n-1 elements. Also there is a counting semaphore to keep track of number of buffers
3) Tested with multiple producers/cores. Without mutex on pushing and popping code does not work as expected :)
4) Tested also by adding delays in consumer to see if the producer waits. It does.
*/
int numbuff_consumer = 0;
int numbuff_producer = 0;
fifo_t fifo;
extern int mutex;

//FIFO functions
//insert at head , pop at tail
//Initialize the numbuff_consumer and numbuff_producer semaphores
void fifo_init(fifo_t* fifo_ptr,int size)
{
	fifo_ptr->head = 0;
	fifo_ptr->tail = 0;
	fifo_ptr->size = size;
	sem_init(&numbuff_consumer,0);
	sem_init(&numbuff_producer,(MAX_FIF0_SIZE-1));
}

//return 1 if empty
//return 0 if not empty
//return -1 if error
int is_fifo_empty(fifo_t* fifo_ptr)
{
	if(fifo_ptr == NULL)
	{
		return -1;
	}
	
	if(fifo_ptr->head == fifo_ptr->tail)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//return 1 if full
//return 0 if not full
//return -1 if error
int is_fifo_full(fifo_t* fifo_ptr)
{
	if(fifo_ptr == NULL)
	{
		return -1;
	}
	
	if(((fifo_ptr->head+1)%(fifo_ptr->size)) == fifo_ptr->tail)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//return 1 if full
//return 0 of successful
//return -1 if error
int fifo_push(fifo_t* fifo_ptr, int value)
{
	int status = -1;
	if(fifo_ptr == NULL)
	{
		return -1;
	}
	sem_dec(&numbuff_producer);
	if(is_fifo_full(fifo_ptr))
	{
		status =  1;
	}
	else
	{
		enter_critical_section(&mutex);
		fifo_ptr->data[fifo_ptr->head] = value;
		fifo_ptr->head = ((fifo_ptr->head+1)%(fifo_ptr->size));
		exit_critical_section(&mutex);
		status = 0;
	}
	sem_inc(&numbuff_consumer);
	//uart_printf("\nFIFO Push Number : %d   Status : %d  Num producer : %d   Num consumer : %d ",value,status,numbuff_producer,numbuff_consumer);
	return status;
}

//return 1 if empty
//return 0 of successful
//return -1 if error
int fifo_pop(fifo_t* fifo_ptr, int* value)
{
	int status = -1;
	if(fifo_ptr == NULL)
	{
		return -1;
	}
	sem_dec(&numbuff_consumer);
	if(is_fifo_empty(fifo_ptr))
	{
		status = 1;
	} 
	else
	{
		enter_critical_section(&mutex);
		*value = fifo_ptr->data[fifo_ptr->tail];
		fifo_ptr->tail = ((fifo_ptr->tail+1)%(fifo_ptr->size));
		exit_critical_section(&mutex);
		status = 0;
	}
	sem_inc(&numbuff_producer);
	//uart_printf("\nFIFO Pop Number  : %d   Status : %d  Num producer : %d   Num consumer : %d ",(*value),status,numbuff_producer,numbuff_consumer);
	return status;
}

void print_fifo(fifo_t* fifo_ptr)
{
	int i =0;
	uart_printf("\nFIFO Data...\n");
	for(i =0;i<fifo_ptr->size;i++)
	{
		uart_printf(" %d ",fifo_ptr->data[i]);
	}
}


void fifo_producer(fifo_t* fifo, int loop_count)
{
	//uart_printf("\nEntering producer %d...", Get_current_coreid());
	int i = 0,status = -1;
	for(i = (0+(Get_current_coreid()*loop_count));i < (loop_count+(Get_current_coreid()*loop_count)); i++)
	{
		status = fifo_push(fifo,i);
		if(status == 1)
		{
			uart_printf("\nQueue Full");
		}
	}
	uart_printf("\nExiting producer %d...", Get_current_coreid());
}

void fifo_consumer(fifo_t* fifo,int loop_count)
{
	int array[400];
	uart_printf("\nEntering consumer %d...",Get_current_coreid());
	int i = 0,status = -1;int data =0;
	for(i = 0;i < loop_count; i++)
	{
		status = fifo_pop(fifo,&data);
		if(status == 1)
		{
			uart_printf("\nQueue empty");
		}
		else if(status == 0)
		{
			array[i] = data;
		}
	}
	uart_printf("\nExiting consumer %d...",Get_current_coreid());
	for(i = 0; i < 400; i++)
	{
		uart_printf(" %d ", array[i]);
	}
}
