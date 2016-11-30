#ifndef __DMA__H__
#define __DMA__H__

#define DMA_BASE_ADDRESS 0x3F007000

#define DMA_CHANNEL_0_BASE_ADDRESS DMA_BASE_ADDRESS
#define DMA_CHANNEL_1_BASE_ADDRESS (DMA_BASE_ADDRESS+0x100)
#define DMA_CHANNEL_2_BASE_ADDRESS (DMA_BASE_ADDRESS+0x200)
#define DMA_CHANNEL_3_BASE_ADDRESS (DMA_BASE_ADDRESS+0x300)
#define DMA_CHANNEL_4_BASE_ADDRESS (DMA_BASE_ADDRESS+0x400)
#define DMA_CHANNEL_5_BASE_ADDRESS (DMA_BASE_ADDRESS+0x500)
#define DMA_CHANNEL_6_BASE_ADDRESS (DMA_BASE_ADDRESS+0x600)
#define DMA_CHANNEL_7_BASE_ADDRESS (DMA_BASE_ADDRESS+0x700)
#define DMA_CHANNEL_8_BASE_ADDRESS (DMA_BASE_ADDRESS+0x800)
#define DMA_CHANNEL_9_BASE_ADDRESS (DMA_BASE_ADDRESS+0x900)
#define DMA_CHANNEL_10_BASE_ADDRESS (DMA_BASE_ADDRESS+0xa00)
#define DMA_CHANNEL_11_BASE_ADDRESS (DMA_BASE_ADDRESS+0xb00)
#define DMA_CHANNEL_12_BASE_ADDRESS (DMA_BASE_ADDRESS+0xc00)
#define DMA_CHANNEL_13_BASE_ADDRESS (DMA_BASE_ADDRESS+0xd00)
#define DMA_CHANNEL_14_BASE_ADDRESS (DMA_BASE_ADDRESS+0xe00)

#define DMA_CHANNEL_INTERRUPT_STATUS DMA_BASE_ADDRESS+0xFE0
#define DMA_CHANNEL_ENABLE			 DMA_BASE_ADDRESS+0xFF0


#define DMA_CONTROL_AND_STATUS_REG_OFFSET		0x0
#define DMA_CONTROL_BLOCK_ADDRESS_REG_OFFSET	0x4
#define DMA_TRANFER_INFORMATION_REG_OFFSET		0x8
#define DMA_SOURCE_ADDRESS_REG_OFFSET			0xc
#define DMA_DESTINATION_ADDRESS_REG_OFFSET		0x10
#define DMA_TRANSFER_LENGTH_REG_OFFSET			0x14
#define DMA_2D_STRIDE_REG_OFFSET				0x18
#define DMA_NEXT_CB_REG_OFFSET					0x1c
#define DMA_DEBUG_REG_OFFSET					0x20	

#define DMA_CHANNEL_0_CONTROL_AND_STATUS 			DMA_CHANNEL_0_BASE_ADDRESS
#define DMA_CHANNEL_0_CONTROL_BLOCK_ADDRESS 		DMA_CHANNEL_0_BASE_ADDRESS+0x4
#define DMA_CHANNEL_0_TRANSFER_INFORMATION			DMA_CHANNEL_0_BASE_ADDRESS+0x8
#define DMA_CHANNEL_0_SOURCE_ADDRESS				DMA_CHANNEL_0_BASE_ADDRESS+0xC
#define DMA_CHANNEL_0_DESTINATION_ADDRESS			DMA_CHANNEL_0_BASE_ADDRESS+0x10
#define DMA_CHANNEL_0_TRANSFER_LENGTH				DMA_CHANNEL_0_BASE_ADDRESS+0x14
#define DMA_CHANNEL_0_2D_STRIDE						DMA_CHANNEL_0_BASE_ADDRESS+0x18
#define DMA_CHANNEL_0_NEXT_CONTROL_BLOCK_ADDRESS	DMA_CHANNEL_0_BASE_ADDRESS+0x1C
#define DMA_CHANNEL_0_DEBUG							DMA_CHANNEL_0_BASE_ADDRESS+0x20

#define DMA_CHANNEL_1_CONTROL_AND_STATUS 			DMA_CHANNEL_1_BASE_ADDRESS
#define DMA_CHANNEL_1_CONTROL_BLOCK_ADDRESS 		DMA_CHANNEL_1_BASE_ADDRESS+0x4
#define DMA_CHANNEL_1_TRANSFER_INFORMATION			DMA_CHANNEL_1_BASE_ADDRESS+0x8
#define DMA_CHANNEL_1_SOURCE_ADDRESS				DMA_CHANNEL_1_BASE_ADDRESS+0xC
#define DMA_CHANNEL_1_DESTINATION_ADDRESS			DMA_CHANNEL_1_BASE_ADDRESS+0x10
#define DMA_CHANNEL_1_TRANSFER_LENGTH				DMA_CHANNEL_1_BASE_ADDRESS+0x14
#define DMA_CHANNEL_1_2D_STRIDE						DMA_CHANNEL_1_BASE_ADDRESS+0x18
#define DMA_CHANNEL_1_NEXT_CONTROL_BLOCK_ADDRESS	DMA_CHANNEL_1_BASE_ADDRESS+0x1C
#define DMA_CHANNEL_1_DEBUG							DMA_CHANNEL_1_BASE_ADDRESS+0x20


typedef struct _DMA_Control_Block_Data_structure_t {
	unsigned int transfer_information;
	unsigned int source_address;
	unsigned int dest_address;
	unsigned int transfer_length;
	unsigned int td_mode_stride;
	unsigned int next_control_block_ptr;
	unsigned int reserved[2];
} DMA_Control_Block_Data_structure_t;


typedef union __attribute__((packed)) _DMA_control_reg_t
{
    struct
    {
       //LSB
       unsigned int     DMA_Activate    							:1;
	   unsigned int     DMA_end_flag    							:1;
	   unsigned int     DMA_interrupt_status    					:1;
	   unsigned int     DMA_DREQ_state    							:1;
	   unsigned int     DMA_paused_state		      	  			:1;
	   unsigned int     DMA_paused_by_DREQ	      	  				:1;
	   unsigned int     DMA_waiting_for_outstanding_writes		    :1;
	   unsigned int     reserved0		      	  					:1;
	   unsigned int     DMA_error		      	  					:1;
	   unsigned int     reserved1		      	  					:7;
	   unsigned int     DMA_priority_level		      	  			:4;
	   unsigned int     DMA_panic_priority_level		      	  	:4;
	   unsigned int     reserved2		      	  					:4;
	   unsigned int     DMA_wait_for_outstanding_writes		      	:1;
	   unsigned int     DMA_disable_debug_pause_signal		      	:1;
	   unsigned int     DMA_abort	      	  						:1;
	   unsigned int     DMA_reset	      	  						:1;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} DMA_control_Reg_t;

typedef union __attribute__((packed)) _DMA_Transfer_Information_reg_t
{
    struct
    {
       //LSB
       unsigned int     DMA_Interrupt_enable    				:1;
	   unsigned int     DMA_2D_MODE    							:1;
	   unsigned int     reserved0    							:1;
	   unsigned int     DMA_wait_for_write_response  			:1;
	   unsigned int     DMA_Destination_address_increment		:1;
	   unsigned int     DMA_Destination_Transfer_width	      	:1;
	   unsigned int     DMA_destination_DREQ 					:1;
	   unsigned int     DMA_Ignore_writes	      	  			:1;
	   unsigned int     DMA_source_address_increment		    :1;
	   unsigned int     DMA_source_transfer_width		      	:1;
	   unsigned int     DMA_source_DREQ		      	  			:1;
	   unsigned int     DMA_source_ignore	      	  			:1;
   	   unsigned int     DMA_source_burst_length		      	  	:4;
	   unsigned int     DMA_source_PERMAP		      	  		:5;
	   unsigned int     DMA_source_add_wait_cycles		      	:5;
	   unsigned int     DMA_source_no_wide_bursts		      	:1;
	   unsigned int     reserved1		      	  				:5;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} DMA_Transfer_Information_Reg_t;

typedef union __attribute__((packed)) _DMA_Transfer_Length_reg_t
{
    struct
    {
       //LSB
       unsigned int     DMA_XLENGTH  				:16;
	   unsigned int     DMA_YLENGTH     			:14;
	   unsigned int     reserved0    				:2;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} DMA_Transfer_length_Reg_t;


typedef union __attribute__((packed)) _DMA_Debug_reg_t
{
    struct
    {
       //LSB
       unsigned int     DMA_Read_last_not_set_error    		:1;
	   unsigned int     DMA_fifo_error    					:1;
	   unsigned int     DMA_slave_read_error    			:1;
	   unsigned int     reserved0   						:1;
	   unsigned int     DMA_outstanding_writes				:4;
	   unsigned int     DMA_ID	      	  					:8;
	   unsigned int     DMA_state_machine_state 			:9;
	   unsigned int     DMA_version		      	  			:3;
	   unsigned int     DMA_lite		      	  			:1;
	   unsigned int     reserved1		      	  			:3;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} DMA_Debug_Reg_t;


typedef union __attribute__((packed)) _DMA_Enable_reg_t
{
    struct
    {
       //LSB
       unsigned int     Enable_DMA_Engine_0    		:1;
	   unsigned int     Enable_DMA_Engine_1    		:1;
	   unsigned int     Enable_DMA_Engine_2    		:1;
	   unsigned int     Enable_DMA_Engine_3   		:1;
	   unsigned int     Enable_DMA_Engine_4			:1;
	   unsigned int     Enable_DMA_Engine_5	      	:1;
	   unsigned int     Enable_DMA_Engine_6 		:1;
	   unsigned int     Enable_DMA_Engine_7		    :1;
	   unsigned int     Enable_DMA_Engine_8		    :1;
	   unsigned int     Enable_DMA_Engine_9		    :1;
	   unsigned int     Enable_DMA_Engine_10	    :1;
	   unsigned int     Enable_DMA_Engine_11		:1;
	   unsigned int     Enable_DMA_Engine_12		:1;
	   unsigned int     Enable_DMA_Engine_13		:1;
	   unsigned int     Enable_DMA_Engine_14		:1;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} DMA_Enable_Reg_t;

int DMA_test(void);
int DMA_setup_CB(unsigned int channel,unsigned int source_addr, unsigned int dest_addr, unsigned int length,unsigned int transfer_information,unsigned int CB_index);
int DMA_SPI_Write_test(void);
int DMA_Start_transfer(int channel);
int DMA_channel_reset(int channel);
int DMA_wait_for_transfer_complete(int channel);

#endif