#ifndef __FIFO__H__
#define __FIFO__H__

#define MAX_FIF0_SIZE 100
typedef struct fifo{
	unsigned int size;
	unsigned int head;
	unsigned int tail;
	unsigned int data[MAX_FIF0_SIZE];
}fifo_t;

//fifo functions
void fifo_init(fifo_t* fifo_ptr,int size);
int is_fifo_full(fifo_t* fifo_ptr);
int is_fifo_empty(fifo_t* fifo_ptr);
int fifo_push(fifo_t* fifo_ptr, int value);
int fifo_pop(fifo_t* fifo_ptr, int* value);
void print_fifo(fifo_t* fifo_ptr);
void fifo_producer(fifo_t* fifo, int loop_count);
void fifo_consumer(fifo_t* fifo, int loop_count);


#endif //__FIFO__H__