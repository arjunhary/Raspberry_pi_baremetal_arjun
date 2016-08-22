#define MINI_UART_BASE 0x3F215040

#define UART_BUF_SIZE 132
#define XMODEM_PAYLOAD_SIZE 128

#define AUX_ENABLE_REG 				0x3F215004

#define AUX_MU_IO_REG 				MINI_UART_BASE
#define AUX_MU_IER_REG				MINI_UART_BASE+0x4
#define AUX_MU_IIR_REG				MINI_UART_BASE+0x8
#define AUX_MU_LCR_REG				MINI_UART_BASE+0xc
#define AUX_MU_MCR_REG				MINI_UART_BASE+0x10
#define AUX_MU_LSR_REG				MINI_UART_BASE+0x14
#define AUX_MU_MSR_REG				MINI_UART_BASE+0x18
#define AUX_MU_CNTL_REG				MINI_UART_BASE+0x20
#define AUX_MU_STAT_REG				MINI_UART_BASE+0x24
#define AUX_MU_BAUD_REGISTER		MINI_UART_BASE+0x28

int UARTInit(void);
int xmodem_recv(void*ptr);
int xmodem_send(void*ptr, int byte_count);
int uart_recvchar(unsigned char* data,unsigned int timeout_ms);
void uart_putchar(unsigned char c);
void uart_print_string(char* str);
void uart_print_number(int num);
void uart_print_number_int_hex(unsigned int num);
void uart_print_string_newline(char* str);
int UART_disable_transmitter_and_receiver(void);
int UART_enable_transmitter_and_receiver(void);
int UART_clear_FIFOs(void);
int wait_till_transmitter_done(void);
void flush_uart_rx_buffer(void);
int uart_tests(void);
void init_uart_buf(void);


//xmodem enums
enum xmodem_chars
{
	XMODEM_SOH = 0x01,
	XMODEM_EOT = 0x04,
	XMODEM_ACK = 0x06,
	XMODEM_NAK = 0x15,
	XMODEM_ETB = 0x17,
	XMODEM_CAN = 0x18,
};

typedef union __attribute__((packed)) _AUX_Enable_Reg_t
{
    struct
    {
       //LSB
       unsigned int     mini_UART_enable    	:1;
       unsigned int     SPI_1_enable			:1;
	   unsigned int     SPI_2_enable	      	:1;
	   unsigned int	  	reserved				:29;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_Enable_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_IO_Reg_t
{
    struct
    {
       //LSB
       unsigned int     received_data    			:8;
	   unsigned int     Reserved		      	  	:24;
       //MSB
    } receive_data;
	struct
    {
       //LSB
       unsigned int     transmit_data    			:8;
	   unsigned int     Reserved		      	  	:24;
       //MSB
    } transmit_data; 
	struct
    {
       //LSB
       unsigned int     baud_rate	    			:8;
	   unsigned int     Reserved		      	  	:24;
       //MSB
    } LSB_baud_rate; 
    unsigned int   mAsU32;
} AUX_MU_IO_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_IER_Reg_t
{
    struct
    {
       //LSB
       unsigned int     Enable_transmit_intr    	:1;
       unsigned int     Enable_receive_intr			:1;
	   unsigned int     Reserved		      	  	:6;
	   unsigned int	  	Reserved1					:24;
       //MSB
    } Interrupt_bits;  
	struct
    {
       //LSB
       unsigned int     MSB_baud_rate    	:8;
	   unsigned int	  	Reserved			:24;
       //MSB
    } MSB_baud_rate;  
    unsigned int   mAsU32;
} AUX_MU_IER_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_IIR_Reg_t
{
    struct
    {
       //LSB
       unsigned int     Interrupt_pending    	:1;
       unsigned int     Interrupt_ID			:2;
	   unsigned int     Reserved		      	:3;
	   unsigned int	  	fifo_enables			:2;
	   unsigned int	  	reserved1				:24;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_MU_IIR_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_LCR_Reg_t
{
    struct
    {
       //LSB
       unsigned int     data_size    	:2;
	   unsigned int     Reserved		:4;
	   unsigned int	  	break_bit		:1;
	   unsigned int	  	dlab_acess		:1;
	   unsigned int 	reserved1		:24;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_MU_LCR_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_LSR_Reg_t
{
    struct
    {
       //LSB
       unsigned int     data_ready    			:1;
	   unsigned int     receiver_overrun		:1;
	   unsigned int	  	reserved				:2;
	   unsigned int	  	trasmitter_empty		:1;
	   unsigned int 	transmitter_idle		:1;
	   unsigned int 	reserved1				:25;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_MU_LSR_Reg_t;


typedef union __attribute__((packed)) _AUX_MU_CNTL_Reg_t
{
    struct
    {
       //LSB
       unsigned int     receiver_enable    			:1;
	   unsigned int     transmitter_enable			:1;
	   unsigned int	  	enable_receive_autoflow		:1;
	   unsigned int	  	enable_transmit_autoflow	:1;
	   unsigned int 	rts_auto_flow_level			:2;
	   unsigned int 	rts_level_assert			:1;
	   unsigned int 	cts_level_assert			:1;
	   unsigned int 	reserved					:24;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_MU_CNTL_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_STAT_Reg_t
{
    struct
    {
       //LSB
	   unsigned int     symbol_available    			:1;
       unsigned int     transmit_space_available    	:1;
	   unsigned int     receiver_idle					:1;
	   unsigned int	  	transmitter_idle				:1;
	   unsigned int	  	receiver_overrun				:1;
	   unsigned int 	transmit_fifo_full				:1;
	   unsigned int 	rts_status						:1;
	   unsigned int 	cts_status						:1;
	   unsigned int 	transmit_fifo_empty				:1;
	   unsigned int 	transmitter_done				:1;
	   unsigned int 	reserved						:6;
	   unsigned int 	receive_fifo_fill_level			:4;
	   unsigned int 	reserved1						:4;
	   unsigned int 	transmit_fifo_fill_level		:4;
	   unsigned int 	reserved2						:4;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_MU_STAT_Reg_t;

typedef union __attribute__((packed)) _AUX_MU_BAUD_RATE_Reg_t
{
    struct
    {
       //LSB
       unsigned int     baud_rate   		:16;
	   unsigned int 	reserved			:16;
       //MSB
    } mbits;  
    unsigned int   mAsU32;
} AUX_MU_BAUD_RATE_Reg_t;

