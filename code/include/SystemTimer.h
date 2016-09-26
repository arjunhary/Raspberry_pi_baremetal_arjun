#ifndef __SYSTEM_TIMER__H__
#define __SYSTEM_TIMER__H__

#define SYSTEM_TIMER_BASE_ADDRESS 0x3F003000

#define SYSTEM_TIMER_CS_REG			SYSTEM_TIMER_BASE_ADDRESS
#define SYSTEM_TIMER_CL0_REG_LSB	SYSTEM_TIMER_BASE_ADDRESS+0x4
#define SYSTEM_TIMER_CL0_REG_MSB	SYSTEM_TIMER_BASE_ADDRESS+0x8
#define SYSTEM_TIMER_CMP0			SYSTEM_TIMER_BASE_ADDRESS+0xC
#define SYSTEM_TIMER_CMP1			SYSTEM_TIMER_BASE_ADDRESS+0x10
#define SYSTEM_TIMER_CMP2			SYSTEM_TIMER_BASE_ADDRESS+0x14
#define SYSTEM_TIMER_CMP3			SYSTEM_TIMER_BASE_ADDRESS+0x18

typedef union __attribute__((packed)) _System_timer_control_reg_t
{
    struct
    {
       //LSB
       unsigned int     System_timer0_match    			:1;
	   unsigned int     System_timer1_match    			:1;
	   unsigned int     System_timer2_match    			:1;
	   unsigned int     System_timer3_match    			:1;
	   unsigned int     Reserved		      	  		:28;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} System_timer_control_Reg_t;

#endif //__SYSTEM_TIMER__H__