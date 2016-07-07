#define SPI_BASE_ADDRESS 0x3F204000

#define SPI_CONTROL_STATUS_REGISTER			SPI_BASE_ADDRESS
#define SPI_TX_RX_FIFO						SPI_BASE_ADDRESS+0x4
#define SPI_CLK_DIVIDER						SPI_BASE_ADDRESS+0x8
#define SPI_DATA_LENGTH						SPI_BASE_ADDRESS+0xC
#define SPI_LOSSI_MODE_TOH					SPI_BASE_ADDRESS+0x10
#define SPI_DMA_DREG_CTRL					SPI_BASE_ADDRESS+0x14

typedef union __attribute__((packed)) _spi_control_reg_t
{
    struct
    {
       //LSB
       unsigned int     chip_select    				:2;
	   unsigned int     clock_phase    				:1;
	   unsigned int     clcock_polarity    			:1;
	   unsigned int     clear_fifo    				:2;
	   unsigned int     chip_sel_polarity    		:1;
	   unsigned int     transfer_active    			:1;
	   unsigned int     dma_enable    				:1;
	   unsigned int     intr_gen_on_done    		:1;
	   unsigned int     intr_gen_on_rxr    			:1;
	   unsigned int     auto_desel_cs    			:1;
	   unsigned int     read_en    					:1;
	   unsigned int     len_sossi_enable    		:1;	   
	   unsigned int     unused		      	  		:2;
	   unsigned int     transfer_done    			:1;
	   unsigned int     rx_fifo_empty    			:1;
	   unsigned int     tx_fifo_full   				:1;
	   unsigned int     rx_fifo_needs_reading   	:1;
	   unsigned int     rx_fifo_full   				:1;
	   unsigned int     cs0_polarity   				:1;
	   unsigned int     cs1_polarity   				:1;
	   unsigned int     cs2_polarity   				:1;
	   unsigned int     dma_lossi_en   				:1;
	   unsigned int     lossi_en_word   			:1;
	   unsigned int     Reserved		      	  	:6;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} SPI_control_Reg_t;

typedef union __attribute__((packed)) _spi_clk_reg_t
{
    struct
    {
       //LSB
       unsigned int     clk_div    				:16;
	   unsigned int     Reserved		      	:16;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} SPI_clk_Reg_t;

typedef union __attribute__((packed)) _spi_dlen_reg_t
{
    struct
    {
       //LSB
       unsigned int     dma_data_length 		:16;
	   unsigned int     Reserved		      	:16;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} SPI_dlen_Reg_t;