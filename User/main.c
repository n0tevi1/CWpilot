#include "stm32f10x.h"
#include "timer.h"
#include "spi.h"
#include "imu.h"
<<<<<<< HEAD
#include "delay.h"
#include "control.h"
#include "led.h"

//LED1: ON after initialized, and blink while reading MPU9250.
//LED2: Blink while prepare MPU9250 magnetometer data, OFF after prepare well.
//LED3: Blink during control loop
=======

int OutputPWM[4] = {810, 810, 810, 810}; //��·PWM����, 0~900
>>>>>>> origin/master

int main(void){
	 //Initialization
	TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM4_Init();
	IMU_init();
<<<<<<< HEAD
	LED_GPIO_Configuration();
	Diff_Integration_Initialize();
	LED1ON;

	//wait for MPU9250 magnetometer data
	while(mpu_value.Mag[0] == 0){
		LED2ON;
=======
	while(mpu_value.Mag[0] == 0){
>>>>>>> origin/master
		spi_Init();
		Init_MPU9250();
		READ_MPU9250_MAG();
		READ_MPU9250_ACCEL();
		READ_MPU9250_GYRO();
<<<<<<< HEAD
		delay_ms(100);
		LED2OFF;
	}
	while(1){
		if(loop50HzFlag){
			LED1OFF;
			loop50HzFlag = 0;
			READ_MPU9250_MAG();
			READ_MPU9250_ACCEL();
			//delay_ms(1);
			READ_MPU9250_GYRO();
			LED1ON;
		}

		if(loop100HzFlag){
			LED3OFF;
 			loop100HzFlag = 0;
			IMU_AHRSupdate();
			IMU_getYawPitchRoll();
			InitDirection();
			CalPID();
			RefreshMotor();
			LED3ON;
=======
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
>>>>>>> origin/master
		}
	}
 }
