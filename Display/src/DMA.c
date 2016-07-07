#include "./../include/DMA.h"
#include "./../include/utility.h"
#include "./../include/display.h"
#include "./../include/SPI.h"


int DMA_wait_for_transfer_complete(int channel)
{
	volatile DMA_control_Reg_t dmactrlreg;
	dmactrlreg.mAsU32 = 0;
	unsigned int dma_channel_base_address = 0;
	switch (channel)
	{
		case 0:
			dma_channel_base_address = DMA_CHANNEL_0_BASE_ADDRESS;
			break;
		case 1:
			dma_channel_base_address = DMA_CHANNEL_1_BASE_ADDRESS;
			break;
		case 2:
			dma_channel_base_address = DMA_CHANNEL_2_BASE_ADDRESS;
			break;
		case 3:
			dma_channel_base_address = DMA_CHANNEL_3_BASE_ADDRESS;
			break;
		case 4:
			dma_channel_base_address = DMA_CHANNEL_4_BASE_ADDRESS;
			break;
		case 5:
			dma_channel_base_address = DMA_CHANNEL_5_BASE_ADDRESS;
			break;
		case 6:
			dma_channel_base_address = DMA_CHANNEL_6_BASE_ADDRESS;
			break;
		case 7:
			dma_channel_base_address = DMA_CHANNEL_7_BASE_ADDRESS;
			break;
		default:
			return -1;
	}
	while(1)
	{
		dmactrlreg = (DMA_control_Reg_t) GET32((dma_channel_base_address+DMA_CONTROL_AND_STATUS_REG_OFFSET));
		if(dmactrlreg.mBits.DMA_end_flag == 1)
		{
			break;
		}
	}
	//Clear the end flag
	dmactrlreg = (DMA_control_Reg_t) GET32((dma_channel_base_address+DMA_CONTROL_AND_STATUS_REG_OFFSET));
	dmactrlreg.mBits.DMA_end_flag = 1;
	PUT32((dma_channel_base_address+DMA_CONTROL_AND_STATUS_REG_OFFSET),dmactrlreg.mAsU32);
	return 0;
	
}

int DMA_channel_reset(int channel)
{
	unsigned int dma_channel_base_address = 0;
	switch (channel)
	{
		case 0:
			dma_channel_base_address = DMA_CHANNEL_0_BASE_ADDRESS;
			break;
		case 1:
			dma_channel_base_address = DMA_CHANNEL_1_BASE_ADDRESS;
			break;
		case 2:
			dma_channel_base_address = DMA_CHANNEL_2_BASE_ADDRESS;
			break;
		case 3:
			dma_channel_base_address = DMA_CHANNEL_3_BASE_ADDRESS;
			break;
		case 4:
			dma_channel_base_address = DMA_CHANNEL_4_BASE_ADDRESS;
			break;
		case 5:
			dma_channel_base_address = DMA_CHANNEL_5_BASE_ADDRESS;
			break;
		case 6:
			dma_channel_base_address = DMA_CHANNEL_6_BASE_ADDRESS;
			break;
		case 7:
			dma_channel_base_address = DMA_CHANNEL_7_BASE_ADDRESS;
			break;
		default:
			return -1;
	}
	PUT32((dma_channel_base_address+DMA_CONTROL_AND_STATUS_REG_OFFSET),0x80000000);
	return 0;
}

int DMA_Start_transfer(int channel)
{
	unsigned int dma_channel_base_address = 0;
	switch (channel)
	{
		case 0:
			dma_channel_base_address = DMA_CHANNEL_0_BASE_ADDRESS;
			break;
		case 1:
			dma_channel_base_address = DMA_CHANNEL_1_BASE_ADDRESS;
			break;
		case 2:
			dma_channel_base_address = DMA_CHANNEL_2_BASE_ADDRESS;
			break;
		case 3:
			dma_channel_base_address = DMA_CHANNEL_3_BASE_ADDRESS;
			break;
		case 4:
			dma_channel_base_address = DMA_CHANNEL_4_BASE_ADDRESS;
			break;
		case 5:
			dma_channel_base_address = DMA_CHANNEL_5_BASE_ADDRESS;
			break;
		case 6:
			dma_channel_base_address = DMA_CHANNEL_6_BASE_ADDRESS;
			break;
		case 7:
			dma_channel_base_address = DMA_CHANNEL_7_BASE_ADDRESS;
			break;
		default:
			return -1;
	}
	PUT32((dma_channel_base_address+DMA_CONTROL_AND_STATUS_REG_OFFSET),0x00000003);
	return 0;
}

volatile DMA_Control_Block_Data_structure_t DMA_CB[2] __attribute__ ((aligned (32)));

int DMA_setup_CB(unsigned int channel,unsigned int source_addr, unsigned int dest_addr, unsigned int length,unsigned int transfer_information,unsigned int CB_index)
{
	
	memset((void*)&DMA_CB[CB_index],0x00,sizeof(DMA_Control_Block_Data_structure_t));
	
	unsigned int dma_channel_base_address = 0;
	unsigned int control_block_address = (0xC0000000+(unsigned int)&DMA_CB[CB_index]);
	
	switch (channel)
	{
		case 0:
			dma_channel_base_address = DMA_CHANNEL_0_BASE_ADDRESS;
			break;
		case 1:
			dma_channel_base_address = DMA_CHANNEL_1_BASE_ADDRESS;
			break;
		case 2:
			dma_channel_base_address = DMA_CHANNEL_2_BASE_ADDRESS;
			break;
		case 3:
			dma_channel_base_address = DMA_CHANNEL_3_BASE_ADDRESS;
			break;
		case 4:
			dma_channel_base_address = DMA_CHANNEL_4_BASE_ADDRESS;
			break;
		case 5:
			dma_channel_base_address = DMA_CHANNEL_5_BASE_ADDRESS;
			break;
		case 6:
			dma_channel_base_address = DMA_CHANNEL_6_BASE_ADDRESS;
			break;
		case 7:
			dma_channel_base_address = DMA_CHANNEL_7_BASE_ADDRESS;
			break;
		default:
			return -1;
	}
	
	//Clear the DMA debug register
	DMA_Debug_Reg_t dma_debug;
	dma_debug.mAsU32 = 0;
	dma_debug.mBits.DMA_Read_last_not_set_error	= 1;
	dma_debug.mBits.DMA_fifo_error				= 1;
	dma_debug.mBits.DMA_slave_read_error		= 1;
	PUT32((dma_channel_base_address+DMA_DEBUG_REG_OFFSET),dma_debug.mAsU32);
	
	DMA_CB[CB_index].source_address 			=	 source_addr;
	DMA_CB[CB_index].dest_address				=	 dest_addr;
	DMA_CB[CB_index].transfer_length			=	 length;	
	DMA_CB[CB_index].transfer_information		=	 transfer_information;
	DMA_CB[CB_index].td_mode_stride			= 	 0;
	DMA_CB[CB_index].next_control_block_ptr	= 	 0;
	
	
	PUT32((dma_channel_base_address+DMA_CONTROL_BLOCK_ADDRESS_REG_OFFSET),control_block_address);
	
	return 0;
}

extern unsigned char TFT_Screen_frambuffer[TFT_DISPLAY_WIDTH*TFT_DISPLAY_HEIGHT*BYTES_PER_PIXEL];
unsigned int spi_dummy_receive = 0;
int DMA_SPI_Write_test(void)
{
	//Set GPIO25
	//set_DC_high_for_data();
	PUT32(SPI_DATA_LENGTH,3);
	PUT32(SPI_CONTROL_STATUS_REGISTER,0x00000930);
	
	//PUT32(SPI_TX_RX_FIFO,0x80);
	//PUT32(SPI_TX_RX_FIFO,0x29);
	
	unsigned int* source_addr = (unsigned int*)TFT_Screen_frambuffer;
	unsigned int* dest_addr   = (unsigned int*)(0xFF204004);
	
	DMA_Transfer_Information_Reg_t dma_transfer_spi_tx_information;
	dma_transfer_spi_tx_information.mAsU32 = 0;
	dma_transfer_spi_tx_information.mBits.DMA_source_address_increment = 1;
	dma_transfer_spi_tx_information.mBits.DMA_wait_for_write_response = 1;
	dma_transfer_spi_tx_information.mBits.DMA_Destination_address_increment = 0;
	dma_transfer_spi_tx_information.mBits.DMA_source_PERMAP = 6;
	dma_transfer_spi_tx_information.mBits.DMA_destination_DREQ = 1;
	
	DMA_Transfer_Information_Reg_t dma_transfer_spi_rx_information;
	dma_transfer_spi_rx_information.mAsU32 = 0;
	dma_transfer_spi_rx_information.mBits.DMA_source_address_increment = 0;
	dma_transfer_spi_rx_information.mBits.DMA_Destination_address_increment = 0;
	dma_transfer_spi_rx_information.mBits.DMA_source_PERMAP = 7;
	dma_transfer_spi_rx_information.mBits.DMA_source_DREQ = 1;

	memset(TFT_Screen_frambuffer,0xFF,((320*240*3)+4));
	TFT_Screen_frambuffer[0] = 0x8F;
	TFT_Screen_frambuffer[1] = 0x8F;
	TFT_Screen_frambuffer[2] = 0x8F;
	TFT_Screen_frambuffer[3] = 0x8F;

	
	DMA_setup_CB(1,(unsigned int)dest_addr,(unsigned int)&spi_dummy_receive,3,dma_transfer_spi_rx_information.mAsU32,1);
	DMA_setup_CB(0,(unsigned int)source_addr,(unsigned int)dest_addr,3,dma_transfer_spi_tx_information.mAsU32,0);
	DMA_Start_transfer(1);
	DMA_Start_transfer(0);
	DMA_wait_for_transfer_complete(0);
	//DMA_wait_for_transfer_complete(1);
	
	return 0;
}

int DMA_test()
{
	unsigned int* source_addr =  (unsigned int*)0x00100000;
	unsigned int* dest_addr   =  (unsigned int*)0x00101000;
	unsigned int* source_addr1 = (unsigned int*)0x00102000;
	unsigned int* dest_addr1   = (unsigned int*)0x00103000;
	
	DMA_Transfer_Information_Reg_t dma_transfer_information;
	dma_transfer_information.mAsU32 = 0;
	dma_transfer_information.mBits.DMA_source_address_increment = 1;
	dma_transfer_information.mBits.DMA_Destination_address_increment = 1;

	memset((void*)0x00100000,0xDE,512);
	memset(dest_addr,0x00,512);
	memset((void*)0x00102000,0xAB,512);
	memset(dest_addr1,0x00,512);
	
	DMA_setup_CB(1,(unsigned int)source_addr1,(unsigned int)dest_addr1,512,dma_transfer_information.mAsU32,0);
	DMA_Start_transfer(1);
	DMA_wait_for_transfer_complete(1);
	DMA_setup_CB(0,(unsigned int)source_addr,(unsigned int)dest_addr,512,dma_transfer_information.mAsU32,1);
	DMA_Start_transfer(0);
	DMA_wait_for_transfer_complete(0);
	
	
	return 0;
}