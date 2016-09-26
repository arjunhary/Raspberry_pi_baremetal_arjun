#ifndef __I2C__H__
#define __I2C__H__

//#define BSC0_I2C_BASE_ADDRESS 0x3F205000
#define BSC1_I2C_BASE_ADDRESS 0x3F804000

#define I2C_CLK_DIV 0x9C4*2

#define I2C_TIMEOUT_MS 2000 //2 seconds

#define BSC1_I2C_CONTROL_REG			BSC1_I2C_BASE_ADDRESS
#define BSC1_I2C_STATUS_REG				BSC1_I2C_BASE_ADDRESS+0x4
#define BSC1_I2C_DLEN_REG				BSC1_I2C_BASE_ADDRESS+0x8
#define BSC1_I2C_SLAVE_ADDR_REG			BSC1_I2C_BASE_ADDRESS+0xC
#define BSC1_I2C_DATA_FIFO_REG			BSC1_I2C_BASE_ADDRESS+0x10
#define BSC1_I2C_CLK_DIV_REG			BSC1_I2C_BASE_ADDRESS+0x14
#define BSC1_I2C_DATA_DELAY_REG			BSC1_I2C_BASE_ADDRESS+0x18
#define BSC1_I2C_CLK_STR_TMOUT_REG		BSC1_I2C_BASE_ADDRESS+0x1C



void i2c_enable(void);
void i2c_init(void);
int i2c_write(char device_addr,char* dataptr, int length);
int i2c_read(char device_addr, char* dataptr, int length);
void i2c_set_slave_addr(unsigned char slave_addr);
void i2c_set_clk_div(unsigned int clk_div);
void i2c_put_data_byte(char data);
void i2c_get_data_byte(volatile char* data);
void i2c_set_data_len(unsigned short data_length);
void i2c_get_data_len(unsigned short* data_length);
void clear_status_register(void);
void clear_fifos(void);
void start_read_transfer();
void start_write_transfer();
void i2c_stop_transfer(void);
void wait_till_transfer_done(void);
void wait_till_tx_fifo_can_accept_data(void);
int wait_till_rx_fifo_contains_data(int timeout);
void set_data_delay(unsigned short Rising_edge_delay, unsigned short falling_edge_delay);
int check_for_i2c_error(void);
void i2c_scan(void);
int i2c_write_reg(char device_addr, char reg, char* dataptr, int length);
int i2c_read_reg(char device_addr, char reg,char* dataptr, int length);

typedef union __attribute__((packed)) _BSC1_I2C_control_reg_t
{
    struct
    {
       //LSB
       unsigned int     read_write_transfer    	:1;
	   unsigned int     reserved    			:3;
	   unsigned int     clear_fifo    			:2;
	   unsigned int     reserved1    			:1;
	   unsigned int     start_transfer		    :1;
	   unsigned int     intr_gen_on_done	    :1;
	   unsigned int     intr_gen_on_tx		    :1;
	   unsigned int     intr_gen_on_rx		    :1;
	   unsigned int     reserved2    			:4;
	   unsigned int     I2C_enable    			:1;
	   unsigned int     reserved3    			:16;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_control_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_status_reg_t
{
    struct
    {
       //LSB
       unsigned int     transfer_active    			:1;
	   unsigned int     transfer_done    			:1;
	   unsigned int     txw_fifo_needs_writing    	:1;
	   unsigned int     rxr_fifo_needs_reading    	:1;
	   unsigned int     txd_fifo_can_accept_data	:1;
	   unsigned int     rxd_fifo_contains_data	    :1;
	   unsigned int     txe_fifo_empty		    	:1;
	   unsigned int     rxf_fifo_full		    	:1;
	   unsigned int     NAK_error    				:1;
	   unsigned int     clk_stretch_timeout    		:1;
	   unsigned int     reserved    				:22;
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_status_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_dlen_reg_t
{
    struct
    {
       //LSB
       unsigned int     Data_length  			:16;
	   unsigned int     Reserved		      	:16;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_dlen_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_slave_addr_reg_t
{
    struct
    {
       //LSB
       unsigned int     Slave_address  			:7;
	   unsigned int     Reserved		      	:25;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_slave_addr_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_data_fifo_reg_t
{
    struct
    {
       //LSB
       unsigned int     Data  			:8;
	   unsigned int     Reserved		:24;   
       //MSB
    } mBits; 
    unsigned int   mAsU32;
} BSC1_I2C_data_fifo_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_clk_reg_t
{
    struct
    {
       //LSB
       unsigned int     clk_div    				:16;
	   unsigned int     Reserved		      	:16;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_clk_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_data_delay_reg_t
{
    struct
    {
       //LSB
       unsigned int     Rising_edge_delay		:16;
	   unsigned int     falling_edge_delay     	:16;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_data_delay_Reg_t;

typedef union __attribute__((packed)) _BSC1_I2C_clk_str_timeout_reg_t
{
    struct
    {
       //LSB
       unsigned int     Clk_stretch_timeout_value		:16;
	   unsigned int     reserved     					:16;   
       //MSB
    } mBits;  
    unsigned int   mAsU32;
} BSC1_I2C_clk_str_timeout_Reg_t;

#endif