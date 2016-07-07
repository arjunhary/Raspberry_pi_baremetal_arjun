#ifndef __FT6206_CAP_TOUCH__H__
#define __FT6206_CAP_TOUCH__H__

#define FT6206_SLAVE_ADDR       0x38
#define FOCAL_TECH_ID			0x11


enum Gesture_ID
{
	NO_GESTURE 		= 0x00,
	MOVE_UP 		= 0x10,
	MOVE_RIGHT 		= 0x14,
	MOVE_DOWN 		= 0x18,
	MOVE_LEFT 		= 0x1C,
	ZOOM_IN 		= 0x48,
	ZOOM_OUT 		= 0x49,
};

enum modes
{
	ACTIVE_MODE 		= 0x00,
	MONITOR_MODE 		= 0x01,
};

enum dev_modes
{
	WORKING_MODE 		= 0x00,
	FACTORY_MODE 		= 0x01,
};


typedef union __attribute__((packed)) _TD_status_reg_t
{
    struct
    {
       //LSB
       unsigned char     number_of_touch_points    		:4;
	   unsigned char     reserved    					:4;
       //MSB
    } mBits;  
    unsigned char   mAsU8;
} TD_status_reg_t;


typedef union __attribute__((packed)) _Pn_XH_t
{
    struct
    {
       //LSB
       unsigned char     touch_X_position_11_8    		:4;
	   unsigned char     reserved    					:2;
	   unsigned char 	 event_flag						:2;
       //MSB
    } mBits;  
    unsigned char   mAsU8;
} Pn_XH_t;

typedef union __attribute__((packed)) _Pn_YH_t
{
    struct
    {
       //LSB
       unsigned char     touch_Y_position_11_8    		:4;
	   unsigned char     touch_id    					:4;
       //MSB
    } mBits;  
    unsigned char   mAsU8;
} Pn_YH_t;


//Register definitions
#define FT6206_REG_MODE 		0x00
#define FT6206_GESTURE_ID 		0x01
#define FT6206_TD_STATUS	 	0x02
#define FT6206_P1_XH		 	0x03
#define FT6206_P1_XL		 	0x04
#define FT6206_P1_YH		 	0x05
#define FT6206_P1_YL		 	0x06
#define FT6206_P1_WEIGHT	 	0x07

#define FT6206_P2_XH		 	0x09
#define FT6206_P2_XL		 	0x0A
#define FT6206_P2_YH		 	0x0B
#define FT6206_P2_YL		 	0x0C
#define FT6206_P2_WEIGHT	 	0x0D


#define FT6206_REG_THRESHHOLD 	0x80
#define FT6206_REG_CONTROL	 	0x86

#define FT6206_REG_PERIOD_ACTIVE 	0x88
#define FT6206_REG_PERIOD_MONITOR 	0x89

#define FT6206_REG_CHIP_SELECT 		0xA3
#define FT6206_REG_INTERRUPT_MODE	0xA4
#define FT6206_REG_PWRMODE 		0xA5
#define FT6206_REG_FIRMVERS 	0xA6
#define FT6206_REG_VENDID 		0xA8
#define FT6206_REG_STATE 		0xBC


int FT6206_get_id(char* id);
int FT6206_get_num_touches(void);
int FT6206_get_device_mode(void);
int FT6206_set_threshold(char value);
int FT6206_get_current_operating_state(void);
int FT6206_get_gesture_id(void);
int FT6206_get_control(void);
int FT6206_set_control(char mode);
int FT6206_set_device_mode(char dev_mode);
int FT6206_get_touch_point_1(short* x , short* y, char* pressure);
int FT6206_get_touch_point_2(short* x , short* y, char* pressure);
int FT6206_get_period_active_mode(void);
int FT6206_get_power_mode(void);
int FT6206_get_chip_select(void);
void FT6206_tests(void);
int FT6206_get_interrupt_mode(void);


#endif //__FT6206_CAP_TOUCH__H__