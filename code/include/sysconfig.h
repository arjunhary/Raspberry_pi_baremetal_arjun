#ifndef _sysconfig_h
#define _sysconfig_h

/*extern unsigned int __heap_limit__;
extern unsigned int __heap_start__;
extern unsigned int __heap_size__;
extern unsigned int __stack_limit__;
extern unsigned int __stack_start__;
extern unsigned int __stack_size__;*/


//Stack picture
/*  			
			Core 3    	Addr: 0x00280000
				***************************
				*	Exception stack 32K   *
				***************************
				***************************
				*	Interrupt stack 32K   *
				***************************
				***************************
				*	FIQ stack 32K   	  *
				***************************
				***************************
				*	SVC code stack 32K    *
				***************************
			Core 2		Addr: 0x00260000
				***************************
				*	Exception stack 32K   *
				***************************
				***************************
				*	Interrupt stack 32K   *
				***************************
				***************************
				*	FIQ stack 32K   	  *
				***************************
				***************************
				*	SVC code stack 32K    *
				***************************
			Core 1  	Addr: 0x00240000  	
				***************************
				*	Exception stack 32K   *
				***************************
				***************************
				*	Interrupt stack 32K   *
				***************************
				***************************
				*	FIQ stack 32K   	  *
				***************************
				***************************
				*	SVC code stack 32K    *
				***************************
			Core 0 		Addr: 0x00220000    	
				***************************
				*	Exception stack 32K   *
				***************************
				***************************
				*	Interrupt stack 32K   *
				***************************
				***************************
				*	FIQ stack 32K   	  *
				***************************
				***************************
				*	SVC code stack 32K    *
				***************************
*/ 


// memory addresses and sizesp
#define MEGABYTE		0x100000

#define MEM_SIZE				(1024 * MEGABYTE)		// default size
#define GPU_MEM_SIZE			(64 * MEGABYTE)			// set in config.txt
#define ARM_MEM_SIZE			(MEM_SIZE - GPU_MEM_SIZE)	// normally overwritten


#define KERNEL_MAX_SIZE			(2 * MEGABYTE)			// all sizes must be a multiple of 16K
#define KERNEL_STACK_SIZE		0x20000


#define EXCEPTION_STACK_SIZE	0x00008000
#define IRQ_STACK_SIZE			0x00008000
#define FIQ_STACK_SIZE			0x00008000
#define CODE_STACK_SIZE			0x00008000

#define CORE_TOTAL_STACK_SIZE (EXCEPTION_STACK_SIZE + IRQ_STACK_SIZE + FIQ_STACK_SIZE + CODE_STACK_SIZE)

#define MEM_KERNEL_START	0x8000
#define MEM_STACK_START 	0x00200000	// expands down

#define CORE0_EXCEPTION_STACK  (MEM_STACK_START + CORE_TOTAL_STACK_SIZE)
#define CORE0_IRQ_STACK		   (CORE0_EXCEPTION_STACK - EXCEPTION_STACK_SIZE)
#define CORE0_FIQ_STACK		   (CORE0_IRQ_STACK - IRQ_STACK_SIZE)
#define CORE0_CODE_STACK	   (CORE0_FIQ_STACK -  FIQ_STACK_SIZE)

#define CORES				 4					// must be a power of 2
#define MEM_ABORT_STACK		(MEM_STACK_START + KERNEL_STACK_SIZE * (CORES-1) + EXCEPTION_STACK_SIZE)
#define MEM_IRQ_STACK		(MEM_ABORT_STACK + EXCEPTION_STACK_SIZE * (CORES-1) + EXCEPTION_STACK_SIZE)
#define MEM_PAGE_TABLE1		(MEM_IRQ_STACK + EXCEPTION_STACK_SIZE * (CORES-1))


#endif
