#ifndef __SPI__H__
#define __SPI__H__


#define SPI_BASE_ADDRESS 0x3F204000

#define SPI_CONTROL_STATUS_REGISTER			SPI_BASE_ADDRESS
#define SPI_TX_RX_FIFO						SPI_BASE_ADDRESS+0x4
#define SPI_CLK_DIVIDER						SPI_BASE_ADDRESS+0x8
#define SPI_DATA_LENGTH						SPI_BASE_ADDRESS+0xC
#define SPI_LOSSI_MODE_TOH					SPI_BASE_ADDRESS+0x10
#define SPI_DMA_DREG_CTRL					SPI_BASE_ADDRESS+0x14


void spi_init(void);
void set_DC_low_for_command(void);
void set_DC_high_for_data(void);
void stop_spi_transfer(void);
void start_spi_transfer(void);
void wait_for_transfer_complete();
void wait_till_tx_fifo_not_full(void);
void wait_till_rx_fifo_has_data(void);
int spi_sendbytes(unsigned int number_of_bytes,unsigned char*ptr);
int spi_sendreadcommand(unsigned char command, int len,unsigned char* buf);
int spi_sendcommand(unsigned char command, int len,unsigned char* buf);
int spi_getbytes(unsigned char*ptr, int number_of_bytes);
void spi_dma_enable_and_adcs(void);
void spi_dma_disable_and_adcs(void);


/// \brief bcm2835SPIClockDivider
/// Specifies the divider used to generate the SPI clock from the system clock.
/// Figures below give the divider, clock period and clock frequency.
/// Clock divided is based on nominal base clock rate of 250MHz
/// It is reported that (contrary to the documentation) any even divider may used.
/// The frequencies shown for each divider have been confirmed by measurement
typedef enum
{
	BCM2835_SPI_CLOCK_DIVIDER_65536 = 0,       ///< 65536 = 262.144us = 3.814697260kHz
	BCM2835_SPI_CLOCK_DIVIDER_32768 = 32768,   ///< 32768 = 131.072us = 7.629394531kHz
	BCM2835_SPI_CLOCK_DIVIDER_16384 = 16384,   ///< 16384 = 65.536us = 15.25878906kHz
	BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192,    ///< 8192 = 32.768us = 30/51757813kHz
	BCM2835_SPI_CLOCK_DIVIDER_4096  = 4096,    ///< 4096 = 16.384us = 61.03515625kHz
	BCM2835_SPI_CLOCK_DIVIDER_2048  = 2048,    ///< 2048 = 8.192us = 122.0703125kHz
	BCM2835_SPI_CLOCK_DIVIDER_1024  = 1024,    ///< 1024 = 4.096us = 244.140625kHz
	BCM2835_SPI_CLOCK_DIVIDER_512   = 512,     ///< 512 = 2.048us = 488.28125kHz
	BCM2835_SPI_CLOCK_DIVIDER_256   = 256,     ///< 256 = 1.024us = 976.5625MHz
	BCM2835_SPI_CLOCK_DIVIDER_128   = 128,     ///< 128 = 512ns = = 1.953125MHz
	BCM2835_SPI_CLOCK_DIVIDER_64    = 64,      ///< 64 = 256ns = 3.90625MHz
	BCM2835_SPI_CLOCK_DIVIDER_32    = 32,      ///< 32 = 128ns = 7.8125MHz
	BCM2835_SPI_CLOCK_DIVIDER_16    = 16,      ///< 16 = 64ns = 15.625MHz
	BCM2835_SPI_CLOCK_DIVIDER_8     = 8,       ///< 8 = 32ns = 31.25MHz
	BCM2835_SPI_CLOCK_DIVIDER_4     = 4,       ///< 4 = 16ns = 62.5MHz
	BCM2835_SPI_CLOCK_DIVIDER_2     = 2,       ///< 2 = 8ns = 125MHz, fastest you can get
	BCM2835_SPI_CLOCK_DIVIDER_1     = 1,       ///< 1 = 262.144us = 3.814697260kHz, same as 0/65536
} bcm2835SPIClockDivider;

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

#endif