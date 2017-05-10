#include "stm32f10x.h"
#include "timer.h"
#include "spi.h"
#include "imu.h"

int OutputPWM[4] = {810, 810, 810, 810}; //��·PWM����, 0~900

int main(void){
	 //Initialization
	TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM4_Init();
	IMU_init();
	while(mpu_value.Mag[0] == 0){
		spi_Init();
		Init_MPU9250();
		READ_MPU9250_MAG();
		READ_MPU9250_ACCEL();
		READ_MPU9250_GYRO();
	}
	while(1){
		if(loop50HzFlag){
			loop50HzFlag = 0;
			READ_MPU9250_MAG();
			READ_MPU9250_ACCEL();
			READ_MPU9250_GYRO();
		}

		if(loop100HzFlag){
			loop100HzFlag = 0;
			IMU_AHRSupdate();
			IMU_getYawPitchRoll();
			TIM_SetCompare1(TIM3,OutputPWM[0]);
			TIM_SetCompare2(TIM3,OutputPWM[1]);
			TIM_SetCompare3(TIM3,OutputPWM[2]);
			TIM_SetCompare4(TIM3,OutputPWM[3]);
		}
	}
 }
