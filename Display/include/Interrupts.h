#define INTERRUPT_BASE_ADDRESS 0x3F00B000

#define INTERRUPT_IRQ_BASIC_PENDING		INTERRUPT_BASE_ADDRESS+0x200
#define INTERRUPT_IRQ_PENDING1			INTERRUPT_BASE_ADDRESS+0x204
#define INTERRUPT_IRQ_PENDING2			INTERRUPT_BASE_ADDRESS+0x208
#define INTERRUPT_FIQ_CONTROL			INTERRUPT_BASE_ADDRESS+0x20C
#define INTERRUPT_ENABLE_IRQ1			INTERRUPT_BASE_ADDRESS+0x210
#define INTERRUPT_ENABLE_IRQ2			INTERRUPT_BASE_ADDRESS+0x214
#define INTERRUPT_ENABLE_BASIC_IRQ		INTERRUPT_BASE_ADDRESS+0x218
#define INTERRUPT_DISABLE_IRQ1			INTERRUPT_BASE_ADDRESS+0x21C
#define INTERRUPT_DISABLE_IRQ2			INTERRUPT_BASE_ADDRESS+0x220
#define INTERRUPT_DISABLE_BASICIRQ		INTERRUPT_BASE_ADDRESS+0x224

int enable_Ft6206_touch_screen_i2c_interrupt(void);
int is_system_timer_interrupt_pending(void);
//void __attribute__((interrupt("IRQ"))) c_irq_handler (void);
//void __attribute__((interrupt("FIQ"))) c_fiq_handler (void);
void polled_sys_timer_interrupt_handler(void);
int is_gpio_interrupt_pending(void);
void  polled_Ft6206_irq_handler (void);


typedef union __attribute__((packed)) _Interrupt_IRQ_basic_pending_reg_t
{
    struct
    {
       //LSB
       unsigned int     ARM_time_irq_pending    					:1;
	   unsigned int     ARM_mailbox_irq_pending    					:1;
	   unsigned int     ARM_doorbell_0_pending    					:1;
	   unsigned int     ARM_doorbell_1_pending    					:1;
	   unsigned int     gpu0_halted_irq_pending		      			:1;
	   unsigned int     gpu1_halted_irq_pending		      			:1;
	   unsigned int     illegal_access_type_irq1_pending		    :1;
	   unsigned int     illegal_access_type_irq0_pending		    :1;
	   unsigned int     bits_set_in_pending_reg1		      		:1;
	   unsigned int     bits_set_in_pending_reg2		      		:1;
	   unsigned int     gpu_irq7		      						:1;
	   unsigned int     gpu_irq9		      						:1;
	   unsigned int     gpu_irq10		      						:1;
	   unsigned int     gpu_irq18		      						:1;
	   unsigned int     gpu_irq19		      						:1;
	   unsigned int     gpu_irq53		      						:1;
	   unsigned int     gpu_irq54		      						:1;
	   unsigned int     gpu_irq55		      						:1;
	   unsigned int     gpu_irq56		      						:1;
	   unsigned int     gpu_irq57		      						:1;
	   unsigned int     gpu_irq60		      						:1;
	   unsigned int     reserved		      						:10;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_basic_pending_Reg_t;

typedef union __attribute__((packed)) _Interrupt_IRQ_pending1_reg_t
{
    struct
    {
       //LSB
       unsigned int     irq0    					:1;
	   unsigned int     irq1    					:1;
	   unsigned int     irq2    					:1;
	   unsigned int     irq3    					:1;
	   unsigned int     irq4		      			:1;
	   unsigned int     irq5		      			:1;
	   unsigned int     irq6		    			:1;
	   unsigned int     irq7		    			:1;
	   unsigned int     irq8		      			:1;
	   unsigned int     irq9		      			:1;
	   unsigned int     irq10		      			:1;
	   unsigned int     irq11		      			:1;
	   unsigned int     irq12		      			:1;
	   unsigned int     irq13		      			:1;
	   unsigned int     irq14		      			:1;
	   unsigned int     irq15		      			:1;
	   unsigned int     irq16		      			:1;
	   unsigned int     irq17		      			:1;
	   unsigned int     irq18		      			:1;
	   unsigned int     irq19		      			:1;
	   unsigned int     irq20		      			:1;
	   unsigned int     irq21		      			:1;
	   unsigned int     irq22		      			:1;
	   unsigned int     irq23		      			:1;
	   unsigned int     irq24		      			:1;
	   unsigned int     irq25		      			:1;
	   unsigned int     irq26		      			:1;
	   unsigned int     irq27		      			:1;
	   unsigned int     irq28		      			:1;
	   unsigned int     irq29		      			:1;
	   unsigned int     irq30		      			:1;
	   unsigned int     irq31		      			:1;	 
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_pending1_Reg_t;

typedef union __attribute__((packed)) _Interrupt_IRQ_pending2_reg_t
{
    struct
    {
       //LSB
       unsigned int     irq32    					:1;
	   unsigned int     irq33    					:1;
	   unsigned int     irq34    					:1;
	   unsigned int     irq35    					:1;
	   unsigned int     irq36		      			:1;
	   unsigned int     irq37		      			:1;
	   unsigned int     irq38		    			:1;
	   unsigned int     irq39		    			:1;
	   unsigned int     irq40		      			:1;
	   unsigned int     irq41		      			:1;
	   unsigned int     irq42		      			:1;
	   unsigned int     irq43		      			:1;
	   unsigned int     irq44		      			:1;
	   unsigned int     irq45		      			:1;
	   unsigned int     irq46		      			:1;
	   unsigned int     irq47		      			:1;
	   unsigned int     irq48		      			:1;
	   unsigned int     irq49		      			:1;
	   unsigned int     irq50		      			:1;
	   unsigned int     irq51		      			:1;
	   unsigned int     irq52		      			:1;
	   unsigned int     irq53		      			:1;
	   unsigned int     irq54		      			:1;
	   unsigned int     irq55		      			:1;
	   unsigned int     irq56		      			:1;
	   unsigned int     irq57		      			:1;
	   unsigned int     irq58		      			:1;
	   unsigned int     irq59		      			:1;
	   unsigned int     irq60		      			:1;
	   unsigned int     irq61		      			:1;
	   unsigned int     irq62		      			:1;
	   unsigned int     irq63		      			:1;	 
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_pending2_Reg_t;



typedef union __attribute__((packed)) _Interrupt_FIQ_control_reg_t
{
    struct
    {
       //LSB
       unsigned int     FIQ_source    					:7;
	   unsigned int     FIQ_enable    					:1;
	   unsigned int     reserved		      			:24;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_FIQ_control_Reg_t;

typedef union __attribute__((packed)) _Interrupt_IRQ_Enable1_reg_t
{
    struct
    {
       //LSB
       unsigned int     irq0    					:1;
	   unsigned int     irq1    					:1;
	   unsigned int     irq2    					:1;
	   unsigned int     irq3    					:1;
	   unsigned int     irq4		      			:1;
	   unsigned int     irq5		      			:1;
	   unsigned int     irq6		    			:1;
	   unsigned int     irq7		    			:1;
	   unsigned int     irq8		      			:1;
	   unsigned int     irq9		      			:1;
	   unsigned int     irq10		      			:1;
	   unsigned int     irq11		      			:1;
	   unsigned int     irq12		      			:1;
	   unsigned int     irq13		      			:1;
	   unsigned int     irq14		      			:1;
	   unsigned int     irq15		      			:1;
	   unsigned int     irq16		      			:1;
	   unsigned int     irq17		      			:1;
	   unsigned int     irq18		      			:1;
	   unsigned int     irq19		      			:1;
	   unsigned int     irq20		      			:1;
	   unsigned int     irq21		      			:1;
	   unsigned int     irq22		      			:1;
	   unsigned int     irq23		      			:1;
	   unsigned int     irq24		      			:1;
	   unsigned int     irq25		      			:1;
	   unsigned int     irq26		      			:1;
	   unsigned int     irq27		      			:1;
	   unsigned int     irq28		      			:1;
	   unsigned int     irq29		      			:1;
	   unsigned int     irq30		      			:1;
	   unsigned int     irq31		      			:1;	 
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_Enable1_Reg_t;

typedef union __attribute__((packed)) _Interrupt_IRQ_Enable2_reg_t
{
    struct
    {
       //LSB
       unsigned int     irq32    					:1;
	   unsigned int     irq33    					:1;
	   unsigned int     irq34    					:1;
	   unsigned int     irq35    					:1;
	   unsigned int     irq36		      			:1;
	   unsigned int     irq37		      			:1;
	   unsigned int     irq38		    			:1;
	   unsigned int     irq39		    			:1;
	   unsigned int     irq40		      			:1;
	   unsigned int     irq41		      			:1;
	   unsigned int     irq42		      			:1;
	   unsigned int     irq43		      			:1;
	   unsigned int     irq44		      			:1;
	   unsigned int     irq45		      			:1;
	   unsigned int     irq46		      			:1;
	   unsigned int     irq47		      			:1;
	   unsigned int     irq48		      			:1;
	   unsigned int     irq49		      			:1;
	   unsigned int     irq50		      			:1;
	   unsigned int     irq51		      			:1;
	   unsigned int     irq52		      			:1;
	   unsigned int     irq53		      			:1;
	   unsigned int     irq54		      			:1;
	   unsigned int     irq55		      			:1;
	   unsigned int     irq56		      			:1;
	   unsigned int     irq57		      			:1;
	   unsigned int     irq58		      			:1;
	   unsigned int     irq59		      			:1;
	   unsigned int     irq60		      			:1;
	   unsigned int     irq61		      			:1;
	   unsigned int     irq62		      			:1;
	   unsigned int     irq63		      			:1;	 
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_Enable2_Reg_t;

typedef union __attribute__((packed)) _Interrupt_IRQ_Disable1_reg_t
{
    struct
    {
       //LSB
       unsigned int     irq0    					:1;
	   unsigned int     irq1    					:1;
	   unsigned int     irq2    					:1;
	   unsigned int     irq3    					:1;
	   unsigned int     irq4		      			:1;
	   unsigned int     irq5		      			:1;
	   unsigned int     irq6		    			:1;
	   unsigned int     irq7		    			:1;
	   unsigned int     irq8		      			:1;
	   unsigned int     irq9		      			:1;
	   unsigned int     irq10		      			:1;
	   unsigned int     irq11		      			:1;
	   unsigned int     irq12		      			:1;
	   unsigned int     irq13		      			:1;
	   unsigned int     irq14		      			:1;
	   unsigned int     irq15		      			:1;
	   unsigned int     irq16		      			:1;
	   unsigned int     irq17		      			:1;
	   unsigned int     irq18		      			:1;
	   unsigned int     irq19		      			:1;
	   unsigned int     irq20		      			:1;
	   unsigned int     irq21		      			:1;
	   unsigned int     irq22		      			:1;
	   unsigned int     irq23		      			:1;
	   unsigned int     irq24		      			:1;
	   unsigned int     irq25		      			:1;
	   unsigned int     irq26		      			:1;
	   unsigned int     irq27		      			:1;
	   unsigned int     irq28		      			:1;
	   unsigned int     irq29		      			:1;
	   unsigned int     irq30		      			:1;
	   unsigned int     irq31		      			:1;	 
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_Disable1_Reg_t;

typedef union __attribute__((packed)) _Interrupt_IRQ_Disable2_reg_t
{
    struct
    {
       //LSB
       unsigned int     irq32    					:1;
	   unsigned int     irq33    					:1;
	   unsigned int     irq34    					:1;
	   unsigned int     irq35    					:1;
	   unsigned int     irq36		      			:1;
	   unsigned int     irq37		      			:1;
	   unsigned int     irq38		    			:1;
	   unsigned int     irq39		    			:1;
	   unsigned int     irq40		      			:1;
	   unsigned int     irq41		      			:1;
	   unsigned int     irq42		      			:1;
	   unsigned int     irq43		      			:1;
	   unsigned int     irq44		      			:1;
	   unsigned int     irq45		      			:1;
	   unsigned int     irq46		      			:1;
	   unsigned int     irq47		      			:1;
	   unsigned int     irq48		      			:1;
	   unsigned int     irq49		      			:1;
	   unsigned int     irq50		      			:1;
	   unsigned int     irq51		      			:1;
	   unsigned int     irq52		      			:1;
	   unsigned int     irq53		      			:1;
	   unsigned int     irq54		      			:1;
	   unsigned int     irq55		      			:1;
	   unsigned int     irq56		      			:1;
	   unsigned int     irq57		      			:1;
	   unsigned int     irq58		      			:1;
	   unsigned int     irq59		      			:1;
	   unsigned int     irq60		      			:1;
	   unsigned int     irq61		      			:1;
	   unsigned int     irq62		      			:1;
	   unsigned int     irq63		      			:1;	 
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_IRQ_Disable2_Reg_t;


typedef union __attribute__((packed)) _basic_inteerupt_enable_reg_t
{
    struct
    {
       //LSB
       unsigned int     arm_timer_irq    					:1;
	   unsigned int     arm_mailbox_irq    					:1;
	   unsigned int     arm_doorbell0_irq    				:1;
	   unsigned int     arm_doorbell1_irq    				:1;
	   unsigned int     gpu0_halted_irq		      			:1;
	   unsigned int     gpu1_halted_irq		      			:1;
	   unsigned int     access_error_type_1		    		:1;
	   unsigned int     access_error_type_0		    		:1;
	   unsigned int     reserved		      				:24;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_basic_enable_reg_t;


typedef union __attribute__((packed)) _basic_inteerupt_disable_reg_t
{
    struct
    {
       //LSB
       unsigned int     arm_timer_irq    					:1;
	   unsigned int     arm_mailbox_irq    					:1;
	   unsigned int     arm_doorbell0_irq    				:1;
	   unsigned int     arm_doorbell1_irq    				:1;
	   unsigned int     gpu0_halted_irq		      			:1;
	   unsigned int     gpu1_halted_irq		      			:1;
	   unsigned int     access_error_type_1		    		:1;
	   unsigned int     access_error_type_0		    		:1;
	   unsigned int     reserved		      				:24;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} Interrupt_basic_disable_reg_t;
