#include "imu.h"
#include "spi.h"
#include "control.h"

int Motor[4];
<<<<<<< HEAD

//Define and Sed expected direction
Direction expected_Direction;

//Store the difference of expected direction and current direction
Direction diff_Direction;
//Store control motor information
Direction control_Direction;
//Difference integration
Direction diff_int_Direction;

//Difference integeration Value of each TimeDiv(10ms) of Yaw, Pitch, Roll, Default lenth:100
//Var integeration_div: just recent time data for integration(1s)
Diff_integration diff_integration;
int integeration_div;
float PID_P = 1.8f;
float PID_D = 2.2f;
float PID_I = 0.1f;

//Initialize the expected direction and calculate difference
void InitDirection(void){
  diff_int_Direction.Yaw = 0;
  diff_int_Direction.Roll = 0;
  diff_int_Direction.Pitch = 0;
=======
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
>>>>>>> origin/master

  diff_Direction.Yaw = expected_Direction.Yaw - imu_value.Yaw;
  diff_Direction.Roll = expected_Direction.Roll - imu_value.Roll;
  diff_Direction.Pitch = expected_Direction.Pitch - imu_value.Pitch;
  diff_Direction.Thr = expected_Direction.Thr;
<<<<<<< HEAD

  //Difference Integration
  if(integeration_div == INTEGRATION_LENGTH) integeration_div = 0;
  diff_integration.Yaw[integeration_div] = 0.01f * diff_Direction.Yaw;
  diff_integration.Pitch[integeration_div] = 0.01f * diff_Direction.Pitch;
  diff_integration.Roll[integeration_div] = 0.01f * diff_Direction.Roll;
  for(int i = 0; i < 100; i ++){
    diff_int_Direction.Yaw += diff_integration.Yaw[i];
    diff_int_Direction.Pitch += diff_integration.Pitch[i];
    diff_int_Direction.Roll += diff_integration.Roll[i];
  }
=======
>>>>>>> origin/master
}

//calculate the PWM output rate of 4 motors:
//leftButtom  Motor0  PC6
//RightButtom Motor1  PC7
//RightUpper  Motor2  PC8
//LeftUpper   Motor3  PC9
void CalPID(void){
<<<<<<< HEAD
  control_Direction.Roll = PID_D * mpu_value.Gyro[0] + PID_P * diff_Direction.Roll + PID_I * diff_int_Direction.Roll;
  control_Direction.Pitch = PID_D * mpu_value.Gyro[1] + PID_P * diff_Direction.Pitch + PID_I * diff_int_Direction.Pitch;
  control_Direction.Yaw = PID_D * mpu_value.Gyro[2] + PID_P * diff_Direction.Yaw + PID_I * diff_int_Direction.Yaw;
  control_Direction.Thr = diff_Direction.Thr;

  //Motor[2] = (int16_t)(control_Direction.Thr - control_Direction.Pitch - control_Direction.Roll - control_Direction.Yaw );    //M3
  //Motor[0] = (int16_t)(control_Direction.Thr + control_Direction.Pitch + control_Direction.Roll - control_Direction.Yaw );    //M1
  //Motor[3] = (int16_t)(control_Direction.Thr - control_Direction.Pitch + control_Direction.Roll + control_Direction.Yaw );    //M4
  //Motor[1] = (int16_t)(control_Direction.Thr + control_Direction.Pitch - control_Direction.Roll + control_Direction.Yaw );    //M2

	Motor[2] = (int16_t)(control_Direction.Thr - control_Direction.Pitch - control_Direction.Roll);    //M3
  Motor[0] = (int16_t)(control_Direction.Thr + control_Direction.Pitch + control_Direction.Roll);    //M1
  Motor[3] = (int16_t)(control_Direction.Thr - control_Direction.Pitch + control_Direction.Roll);    //M4
  Motor[1] = (int16_t)(control_Direction.Thr + control_Direction.Pitch - control_Direction.Roll);    //M2
=======
  control_Direction.Roll = PID_D * mpu_value.Gyro[0] + PID_P * diff_Direction.Roll;
  control_Direction.Pitch = PID_D * mpu_value.Gyro[1] + PID_P * diff_Direction.Pitch;
  control_Direction.Yaw = PID_D * mpu_value.Gyro[2] + PID_P * diff_Direction.Yaw;
  control_Direction.Thr = diff_Direction.Thr;
  
  Motor[2] = (int16_t)(control_Direction.Thr - control_Direction.Pitch - control_Direction.Roll - control_Direction.Yaw );    //M3
  Motor[0] = (int16_t)(control_Direction.Thr + control_Direction.Pitch + control_Direction.Roll - control_Direction.Yaw );    //M1
  Motor[3] = (int16_t)(control_Direction.Thr - control_Direction.Pitch + control_Direction.Roll + control_Direction.Yaw );    //M4
  Motor[1] = (int16_t)(control_Direction.Thr + control_Direction.Pitch - control_Direction.Roll + control_Direction.Yaw );    //M2
>>>>>>> origin/master
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
<<<<<<< HEAD

void Diff_Integration_Initialize(void){
  expected_Direction.Yaw = 0;
  expected_Direction.Roll = 0;
  expected_Direction.Pitch = 0;
  expected_Direction.Thr = 350;

  integeration_div = 0;
  for(int i = 0; i < 100; i++){
    diff_integration.Yaw[i] = 0;
    diff_integration.Pitch[i] = 0;
    diff_integration.Roll[i] = 0;
  }
}
=======
>>>>>>> origin/master
