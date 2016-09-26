#ifndef __GPIO__H__
#define __GPIO__H__


#define GPIO_BASE 0x3F200000

#define GPIO_REG_GPFSEL0 GPIO_BASE
#define GPIO_REG_GPFSEL1 GPIO_BASE+0x4
#define GPIO_REG_GPFSEL2 GPIO_BASE+0x8
#define GPIO_REG_GPFSEL3 GPIO_BASE+0xC
#define GPIO_REG_GPFSEL4 GPIO_BASE+0x10
#define GPIO_REG_GPFSEL5 GPIO_BASE+0x14

#define GPIO_REG_GPSET0 GPIO_BASE+0x1C
#define GPIO_REG_GPSET1 GPIO_BASE+0x20

#define GPIO_REG_GPCLR0 GPIO_BASE+0x28
#define GPIO_REG_GPCLR1 GPIO_BASE+0x2C

#define GPIO_REG_GPLEV0 GPIO_BASE+0x34
#define GPIO_REG_GPLEV1 GPIO_BASE+0x38

#define GPIO_REG_GPEDS0 GPIO_BASE+0x40
#define GPIO_REG_GPEDS1 GPIO_BASE+0x44

#define GPIO_REG_GPREN0 GPIO_BASE+0x4C
#define GPIO_REG_GPREN1 GPIO_BASE+0x50

#define GPIO_REG_GPFEN0 GPIO_BASE+0x58
#define GPIO_REG_GPFEN1 GPIO_BASE+0x5C

#define GPIO_REG_GPFEN0 GPIO_BASE+0x58
#define GPIO_REG_GPFEN1 GPIO_BASE+0x5C

#define GPIO_REG_GPHEN0 GPIO_BASE+0x64
#define GPIO_REG_GPHEN1 GPIO_BASE+0x68

#define GPIO_REG_GPLEN0 GPIO_BASE+0x70
#define GPIO_REG_GPLEN1 GPIO_BASE+0x74

#define GPIO_REG_GPAREN0 GPIO_BASE+0x7C
#define GPIO_REG_GPAREN1 GPIO_BASE+0x80

#define GPIO_REG_GPAFEN0 GPIO_BASE+0x88
#define GPIO_REG_GPAFEN1 GPIO_BASE+0x8C

int LEDInit(void);
int LEDTurnon(void);
int LEDTurnoff(void);

enum Function_Select
{
	GPIO_SET_AS_INPUT = 0,
	GPIO_SET_AS_OUTPUT = 1,
	GPIO_SET_alternate_function_5=2,
	GPIO_SET_alternate_function_4=3,
	GPIO_SET_alternate_function_0=4,
	GPIO_SET_alternate_function_1=5,
	GPIO_SET_alternate_function_2=6,
	GPIO_SET_alternate_function_3=7,
};

typedef union __attribute__((packed)) _GPIO_Function_Select0_t
{
    struct
    {
       //LSB
       unsigned int     FunctionSelGPIO0        :3;
       unsigned int     FunctionSelGPIO1		:3;
       unsigned int     FunctionSelGPIO2        :3;
	   unsigned int     FunctionSelGPIO3        :3;
	   unsigned int     FunctionSelGPIO4        :3;
       unsigned int     FunctionSelGPIO5		:3;
       unsigned int     FunctionSelGPIO6        :3;
	   unsigned int     FunctionSelGPIO7        :3;
       unsigned int     FunctionSelGPIO8		:3;
       unsigned int     FunctionSelGPIO9        :3;
	   unsigned int     Reserved		      	:2;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
}GPIO_Funtion_Select0_t;

typedef union __attribute__((packed)) _GPIO_Function_Select1_t
{
    struct
    {
       //LSB
       unsigned int     FunctionSelGPIO10        	:3;
       unsigned int     FunctionSelGPIO11		  	:3;
       unsigned int     FunctionSelGPIO12        	:3;
	   unsigned int     FunctionSelGPIO13        	:3;
	   unsigned int     FunctionSelGPIO14        	:3;
       unsigned int     FunctionSelGPIO15		  	:3;
       unsigned int     FunctionSelGPIO16       	:3;
	   unsigned int     FunctionSelGPIO17        	:3;
       unsigned int     FunctionSelGPIO18		  	:3;
       unsigned int     FunctionSelGPIO19        	:3;
	   unsigned int     Reserved		      	  	:2;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Funtion_Select1_t;

typedef union __attribute__((packed)) _GPIO_Function_Select2_t
{
    struct
    {
       //LSB
       unsigned int     FunctionSelGPIO20       	 :3;
       unsigned int     FunctionSelGPIO21		 	 :3;
       unsigned int     FunctionSelGPIO22       	 :3;
	   unsigned int     FunctionSelGPIO23       	 :3;
	   unsigned int     FunctionSelGPIO24       	 :3;
       unsigned int     FunctionSelGPIO25		 	 :3;
       unsigned int     FunctionSelGPIO26      	 	 :3;
	   unsigned int     FunctionSelGPIO27       	 :3;
       unsigned int     FunctionSelGPIO28		 	 :3;
       unsigned int     FunctionSelGPIO29       	 :3;
	   unsigned int     Reserved		      	  	 :2;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Funtion_Select2_t;

typedef union __attribute__((packed)) _GPIO_Function_Select3_t
{
    struct
    {
       //LSB
       unsigned int     FunctionSelGPIO30       	 :3;
       unsigned int     FunctionSelGPIO31		 	 :3;
       unsigned int     FunctionSelGPIO32       	 :3;
	   unsigned int     FunctionSelGPIO33       	 :3;
	   unsigned int     FunctionSelGPIO34       	 :3;
       unsigned int     FunctionSelGPIO35		 	 :3;
       unsigned int     FunctionSelGPIO36      	 	 :3;
	   unsigned int     FunctionSelGPIO37       	 :3;
       unsigned int     FunctionSelGPIO38		 	 :3;
       unsigned int     FunctionSelGPIO39       	 :3;
	   unsigned int     Reserved		      	  	 :2;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Funtion_Select3_t;

typedef union __attribute__((packed)) _GPIO_Function_Select4_t
{
    struct
    {
       //LSB
       unsigned int     FunctionSelGPIO40       	 :3;
       unsigned int     FunctionSelGPIO41		 	 :3;
       unsigned int     FunctionSelGPIO42       	 :3;
	   unsigned int     FunctionSelGPIO43       	 :3;
	   unsigned int     FunctionSelGPIO44       	 :3;
       unsigned int     FunctionSelGPIO45		 	 :3;
       unsigned int     FunctionSelGPIO46      	 	 :3;
	   unsigned int     FunctionSelGPIO47       	 :3;
       unsigned int     FunctionSelGPIO48		 	 :3;
       unsigned int     FunctionSelGPIO49       	 :3;
	   unsigned int     Reserved		      	  	 :2;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Funtion_Select4_t;

typedef union __attribute__((packed)) _GPIO_Function_Select5_t
{
    struct
    {
       //LSB
       unsigned int     FunctionSelGPIO50       	 :3;
       unsigned int     FunctionSelGPIO51		 	 :3;
       unsigned int     FunctionSelGPIO52       	 :3;
	   unsigned int     FunctionSelGPIO53       	 :3;
	   unsigned int     Reserved		      	  	 :20;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Funtion_Select5_t;

typedef union __attribute__((packed)) _GPIO_Output_Set_Register0_t
{
    struct
    {
       //LSB
       unsigned int     SetGPIO0        :1;
       unsigned int     SetGPIO1		:1;
       unsigned int     SetGPIO2        :1;
	   unsigned int     SetGPIO3        :1;
	   unsigned int     SetGPIO4        :1;
       unsigned int     SetGPIO5        :1;
       unsigned int     SetGPIO6        :1;
	   unsigned int     SetGPIO7        :1;
       unsigned int     SetGPIO8        :1;
       unsigned int     SetGPIO9        :1;
	   unsigned int     SetGPIO10       :1;
	   unsigned int     SetGPIO11       :1;
	   unsigned int     SetGPIO12       :1;
	   unsigned int     SetGPIO13       :1;
	   unsigned int     SetGPIO14       :1;
	   unsigned int     SetGPIO15       :1;
	   unsigned int     SetGPIO16       :1;
	   unsigned int     SetGPIO17       :1;
	   unsigned int     SetGPIO18       :1;
	   unsigned int     SetGPIO19       :1;
	   unsigned int     SetGPIO20       :1;
	   unsigned int     SetGPIO21       :1;
	   unsigned int     SetGPIO22       :1;
	   unsigned int     SetGPIO23       :1;
	   unsigned int     SetGPIO24       :1;
	   unsigned int     SetGPIO25       :1;
	   unsigned int     SetGPIO26       :1;
	   unsigned int     SetGPIO27       :1;
	   unsigned int     SetGPIO28       :1;
	   unsigned int     SetGPIO29       :1;
	   unsigned int     SetGPIO30       :1;
	   unsigned int     SetGPIO31       :1;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Output_Set_Register0_t;

typedef union __attribute__((packed)) _GPIO_Output_Set_Register1_t
{
    struct
    {
       //LSB
       unsigned int     SetGPIO32       :1;
       unsigned int     SetGPIO33		:1;
       unsigned int     SetGPIO34       :1;
	   unsigned int     SetGPIO35       :1;
	   unsigned int     SetGPIO36       :1;
       unsigned int     SetGPIO37       :1;
       unsigned int     SetGPIO38       :1;
	   unsigned int     SetGPIO39       :1;
       unsigned int     SetGPIO40       :1;
       unsigned int     SetGPIO41       :1;
	   unsigned int     SetGPIO42       :1;
	   unsigned int     SetGPIO43       :1;
	   unsigned int     SetGPIO44       :1;
	   unsigned int     SetGPIO45       :1;
	   unsigned int     SetGPIO46       :1;
	   unsigned int     SetGPIO47       :1;
	   unsigned int     SetGPIO48       :1;
	   unsigned int     SetGPIO49       :1;
	   unsigned int     SetGPIO50       :1;
	   unsigned int     SetGPIO51       :1;
	   unsigned int     SetGPIO52       :1;
	   unsigned int     SetGPIO53       :1;
	   unsigned int 	Reserved		:10;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Output_Set_Register1_t;

typedef union __attribute__((packed)) _GPIO_Output_Clear_Register0_t
{
    struct
    {
       //LSB
       unsigned int     ClearGPIO0        :1;
       unsigned int     ClearGPIO1		  :1;
       unsigned int     ClearGPIO2        :1;
	   unsigned int     ClearGPIO3        :1;
	   unsigned int     ClearGPIO4        :1;
       unsigned int     ClearGPIO5        :1;
       unsigned int     ClearGPIO6        :1;
	   unsigned int     ClearGPIO7        :1;
       unsigned int     ClearGPIO8        :1;
       unsigned int     ClearGPIO9        :1;
	   unsigned int     ClearGPIO10       :1;
	   unsigned int     ClearGPIO11       :1;
	   unsigned int     ClearGPIO12       :1;
	   unsigned int     ClearGPIO13       :1;
	   unsigned int     ClearGPIO14       :1;
	   unsigned int     ClearGPIO15       :1;
	   unsigned int     ClearGPIO16       :1;
	   unsigned int     ClearGPIO17       :1;
	   unsigned int     ClearGPIO18       :1;
	   unsigned int     ClearGPIO19       :1;
	   unsigned int     ClearGPIO20       :1;
	   unsigned int     ClearGPIO21       :1;
	   unsigned int     ClearGPIO22       :1;
	   unsigned int     ClearGPIO23       :1;
	   unsigned int     ClearGPIO24       :1;
	   unsigned int     ClearGPIO25       :1;
	   unsigned int     ClearGPIO26       :1;
	   unsigned int     ClearGPIO27       :1;
	   unsigned int     ClearGPIO28       :1;
	   unsigned int     ClearGPIO29       :1;
	   unsigned int     ClearGPIO30       :1;
	   unsigned int     ClearGPIO31       :1;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Output_Clear_Register0_t;

typedef union __attribute__((packed)) _GPIO_Output_Clear_Register1_t
{
    struct
    {
       //LSB
       unsigned int     ClearGPIO32       :1;
       unsigned int     ClearGPIO33		  :1;
       unsigned int     ClearGPIO34       :1;
	   unsigned int     ClearGPIO35       :1;
	   unsigned int     ClearGPIO36       :1;
       unsigned int     ClearGPIO37       :1;
       unsigned int     ClearGPIO38       :1;
	   unsigned int     ClearGPIO39       :1;
       unsigned int     ClearGPIO40       :1;
       unsigned int     ClearGPIO41       :1;
	   unsigned int     ClearGPIO42       :1;
	   unsigned int     ClearGPIO43       :1;
	   unsigned int     ClearGPIO44       :1;
	   unsigned int     ClearGPIO45       :1;
	   unsigned int     ClearGPIO46       :1;
	   unsigned int     ClearGPIO47       :1;
	   unsigned int     ClearGPIO48       :1;
	   unsigned int     ClearGPIO49       :1;
	   unsigned int     ClearGPIO50       :1;
	   unsigned int     ClearGPIO51       :1;
	   unsigned int     ClearGPIO52       :1;
	   unsigned int     ClearGPIO53       :1;
	   unsigned int 	Reserved		  :10;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_Output_Clear_Register1_t;

typedef union __attribute__((packed)) _GPIO_0_31_struct
{
    struct
    {
       //LSB
       unsigned int     GPIO0        :1;
       unsigned int     GPIO1		 :1;
       unsigned int     GPIO2        :1;
	   unsigned int     GPIO3        :1;
	   unsigned int     GPIO4        :1;
       unsigned int     GPIO5        :1;
       unsigned int     GPIO6        :1;
	   unsigned int     GPIO7        :1;
       unsigned int     GPIO8        :1;
       unsigned int     GPIO9        :1;
	   unsigned int     GPIO10       :1;
	   unsigned int     GPIO11       :1;
	   unsigned int     GPIO12       :1;
	   unsigned int     GPIO13       :1;
	   unsigned int     GPIO14       :1;
	   unsigned int     GPIO15       :1;
	   unsigned int     GPIO16       :1;
	   unsigned int     GPIO17       :1;
	   unsigned int     GPIO18       :1;
	   unsigned int     GPIO19       :1;
	   unsigned int     GPIO20       :1;
	   unsigned int     GPIO21       :1;
	   unsigned int     GPIO22       :1;
	   unsigned int     GPIO23       :1;
	   unsigned int     GPIO24       :1;
	   unsigned int     GPIO25       :1;
	   unsigned int     GPIO26       :1;
	   unsigned int     GPIO27       :1;
	   unsigned int     GPIO28       :1;
	   unsigned int     GPIO29       :1;
	   unsigned int     GPIO30       :1;
	   unsigned int     GPIO31       :1;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} GPIO_0_31_t;

#endif
