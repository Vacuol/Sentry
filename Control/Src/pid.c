/**
  *@file timer.c
  *@date 2018-12-17
  *@author Vacuo.W
  *@brief 
  */
  
#include "pid.h"
#include "control.h"
#include "can_my.h"
#include "mpu6050.h"

/************CLOUD***************/
PID_Regulator_t cloud_pitch_speed_pid;
PID_Regulator_t cloud_pitch_position_pid;
PID_Regulator_t cloud_yaw_speed_pid;
PID_Regulator_t cloud_yaw_position_pid;
PID_Regulator_t underpan_motor[4];

void PID_Calc(PID_Regulator_t *pid)
{
	//λ��ʽ
	if (pid->type == positional)
	{
		pid->err[1] = pid->err[0];
		pid->err[0] = pid->ref - pid->fdb;
		pid->inte += pid->err[0] * pid->ki;

		if(pid->inte > pid->componentKiMax)//�Ի������������
			pid->inte = pid->componentKiMax;
		else if (pid->inte < -pid->componentKiMax)
			pid->inte = -pid->componentKiMax;

		pid->output = (
					   pid->err[0] * pid->kp					//P
					 + pid->inte								//I
					 + (pid->err[0] - pid->err[1]) * pid->kd	//D
					  );
	}
	else	
	//����ʽ
	{
		pid->err[2] = pid->err[1];
		pid->err[1] = pid->err[0];
		pid->err[0] = pid->ref - pid->fdb;

		pid->output += (
						(pid->err[0] - pid->err[1]) * pid->kp
					  + (pid->err[0]) * pid->ki
					  + (pid->err[0] - 2 * pid->err[1] + pid->err[2]) * pid->kd
					   );
	}
	//����޷�
	if(pid->output > pid->outputMax)
		pid->output = pid->outputMax;
	else if (pid->output < -pid->outputMax)
		pid->output = -pid->outputMax;	
}

void PID_Init(PID_Regulator_t *pid,float kp,float ki,float kd,float componentKiMax,float outputMax, PID_type type)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->inte = 0;

	pid->componentKiMax = componentKiMax;

	pid->outputMax = outputMax;
	pid->type = type;
}

void Cloud_Speed(void)
{
	//PITCH
	cloud_pitch_speed_pid.fdb = mpu6050.Gyro.Origin.y;					//���������������ǽ��ٶ�
	cloud_pitch_speed_pid.ref = cloud_pitch_position_pid.output;			//�趨������λ�û����
	PID_Calc(&cloud_pitch_speed_pid);
	
	//YAW
	cloud_yaw_speed_pid.fdb = mpu6050.Gyro.Origin.x;					//���������������ǽ��ٶ�
	cloud_yaw_speed_pid.ref = cloud_yaw_position_pid.output;			//�趨������λ�û����
	PID_Calc(&cloud_yaw_speed_pid);
	
}

void Cloud_Position(void)
{
	//PITCH
	cloud_pitch_position_pid.fdb = cloud_pitch.Bmechanical_angle;				//��������:��̨����Ƕ�
	cloud_pitch_position_pid.ref = pitch;										//�趨����
	PID_Calc(&cloud_pitch_position_pid);
	
	//YAW
	cloud_yaw_position_pid.fdb = cloud_yaw.Bmechanical_angle;					//������������̨����Ƕ�
	cloud_yaw_position_pid.ref = yaw;										//�趨������λ�û����
	PID_Calc(&cloud_yaw_position_pid);
	
}

void ALLPID_Init()
{
	int i;

	PID_Init(&cloud_pitch_position_pid,	-70,	-1,	5,	3000,	10000, positional);
	PID_Init(&cloud_pitch_speed_pid,	0.30, 0, -0,	2000,5000, positional);
	
	PID_Init(&cloud_yaw_position_pid,	45,	0,	-7,	1000,	12000, positional);
	PID_Init(&cloud_yaw_speed_pid	,	-0.5,-0,-0,	1000,5000, positional);

	for (i = 0;i <= 3; i++)
	{
		PID_Init(&underpan_motor[i], 5, 0.03, 0, 8000, 16000, increment);
	}
	
}