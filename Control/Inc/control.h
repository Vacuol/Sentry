#ifndef _CONTROL_H
#define _CONTROL_H

#include "stm32f4xx_HAL.h"
#include "pid.h"

#define pitch_mid  -550					//��̨pitch���ֵ   800				-800black
#define yaw_mid -2600						//��̨yaw���ֵ			300		1000black


/*****teler*******/
struct telecon_data
{
	int16_t ch0;
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;
	uint8_t s1;
	uint8_t s2;
	
	int16_t x;
	int16_t y;
	int16_t z;
	uint8_t press_l;
	uint8_t press_r;
	
	uint16_t key;   

	uint16_t resv;
};	

///*************underpan**************//
struct underpan_parameter 
{
	uint16_t mechanical_angle;	//λ�ýǶ�
	int16_t rotation_rate;		//ת��
	int16_t motor_current;		
	uint16_t motor_temperature;
	int16_t motor_output;
	int16_t set_speed;
	int16_t i_output;
	int16_t i_interg;
	int16_t s_interg;
	int32_t sum_current;
	uint8_t current_flag;
	int16_t current_store[10];
	uint8_t current_count;
	int16_t average_current;
};

typedef struct cloud_parameter 
{
	uint16_t mechanical_angle;//��е�Ƕ�
	int16_t Bmechanical_angle;//�任��Ƕ�
	int16_t torque;//ת�ص�������
	int16_t torque_current;//ת�ص�������
	int16_t iout;

	int16_t motor_output;
	int16_t set_speed;
} cloud_parameter;

typedef struct dan_parameter 
{
	uint16_t mechanical_angle;//��е�Ƕ�
	
	int16_t speed;//ת�ص�������
	int16_t torque_current;//ת�ص�������
	
	int16_t error[3];
	int16_t motor_output;
} dan_parameter;

typedef struct CAMERA
{
    uint8_t recieve[1];
    uint8_t count;
    uint8_t transmit[1];
    int16_t x;
    int16_t y;
		int16_t x_last;
		int16_t y_last;
    uint8_t sum;
} CAMERA;

typedef struct JUDGE
{
    uint8_t recieve[1];
    uint8_t count;
    uint8_t transmit[1];
//    uint16_t x;
//    uint16_t y;
//    uint8_t sum;
} JUDGE;

typedef struct
{
    uint8_t Count;
	uint8_t Buf[20];
	uint8_t		Sum;
	uint8_t		pidReadBuf;
	PID_Regulator_t* 	pidAdjust;
} RxPID;



extern RxPID rxPID;
extern uint8_t teledata_rx[18];
extern struct telecon_data tele_data;
extern struct underpan_parameter underpan_para[4];
extern struct cloud_parameter cloud_pitch,cloud_yaw;
extern struct dan_parameter dan_42;
extern struct dan_parameter dan_17;
extern struct dan_parameter dan_42_ver;
extern struct CAMERA camera;
extern struct JUDGE judge;
extern int16_t pitch;
extern int16_t yaw;
extern float track_length;



//****************function*****************//
void telecontroller_data(void);
void underpan_pid(void);
void cloud_y_v_pid(void);
void cloud_p_v_pid(void);
void para_init(void);
void Bodan_pid(void);


#endif