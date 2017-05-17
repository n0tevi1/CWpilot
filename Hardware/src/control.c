#include "imu.h"
#include "spi.h"
#include "control.h"

int Motor[4];
Direction expected_Direction;
Direction diff_Direction;
Direction control_Direction;
float PID_P = 1;
float PID_D = 1;

//Initialize the expected direction and calculate difference
void InitDirection(void){
  expected_Direction.Yaw = 0;
  expected_Direction.Roll = 0;
  expected_Direction.Pitch = 0;
  expected_Direction.Thr = 500;

  diff_Direction.Yaw = expected_Direction.Yaw - imu_value.Yaw;
  diff_Direction.Roll = expected_Direction.Roll - imu_value.Roll;
  diff_Direction.Pitch = expected_Direction.Pitch - imu_value.Pitch;
  diff_Direction.Thr = expected_Direction.Thr;
}

//calculate the PWM output rate of 4 motors:
//leftButtom  Motor0  PC6
//RightButtom Motor1  PC7
//RightUpper  Motor2  PC8
//LeftUpper   Motor3  PC9
void CalPID(void){
  control_Direction.Roll = PID_D * mpu_value.Gyro[0] + PID_P * diff_Direction.Roll;
  control_Direction.Pitch = PID_D * mpu_value.Gyro[1] + PID_P * diff_Direction.Pitch;
  control_Direction.Yaw = PID_D * mpu_value.Gyro[2] + PID_P * diff_Direction.Yaw;
  control_Direction.Thr = diff_Direction.Thr;
  
  Motor[2] = (int16_t)(control_Direction.Thr - control_Direction.Pitch - control_Direction.Roll - control_Direction.Yaw );    //M3
  Motor[0] = (int16_t)(control_Direction.Thr + control_Direction.Pitch + control_Direction.Roll - control_Direction.Yaw );    //M1
  Motor[3] = (int16_t)(control_Direction.Thr - control_Direction.Pitch + control_Direction.Roll + control_Direction.Yaw );    //M4
  Motor[1] = (int16_t)(control_Direction.Thr + control_Direction.Pitch - control_Direction.Roll + control_Direction.Yaw );    //M2
}

//Output PWM wave
void RefreshMotor(void){
  //amplitude limitation, see control.h to modify the max PWM duty rate and min PWM duty rate
  for(int i = 0; i < 4; i++){
    if(Motor[i] >= (int16_t)MAXPWMNUMBER * MAXPWMDP) Motor[i] = (int16_t)MAXPWMNUMBER * MAXPWMDP;
    if(Motor[i] <= (int16_t)MAXPWMNUMBER * MINPWMDP) Motor[i] = (int16_t)MAXPWMNUMBER * MINPWMDP;
  }

  TIM_SetCompare1(TIM3,Motor[0]);
  TIM_SetCompare2(TIM3,Motor[1]);
  TIM_SetCompare3(TIM3,Motor[2]);
  TIM_SetCompare4(TIM3,Motor[3]);
}
