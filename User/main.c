#include "stm32f10x.h"
#include "timer.h"
#include "spi.h"

int OutputPWM[4] = {810, 810, 810, 810}; //四路PWM输出, 0~900

int main(void){	
	 //Initialization
	TIM3_PWM_Init(899,0);	 //不分频。PWM频率=72000000/900=80Khz
	spi_Init();
	Init_MPU9250();
		while(1){
			TIM_SetCompare1(TIM3,OutputPWM[0]);
			TIM_SetCompare2(TIM3,OutputPWM[1]);
			TIM_SetCompare3(TIM3,OutputPWM[2]);
			TIM_SetCompare4(TIM3,OutputPWM[3]);
			READ_MPU9250_MAG();
			READ_MPU9250_ACCEL();
			READ_MPU9250_GYRO();
	}	 
 }
