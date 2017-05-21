#ifndef _IMU_H
#define _IMU_H

<<<<<<< HEAD
#define Kp 1.2f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.12f   // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f

typedef struct{
  float Roll;    //X axis
  float Pitch;   //Y axis
  float Yaw;     //Z axis
  float Quat[4];
  float Integrate[3];  // integrate of X, Y, Z
}IMU_value;

extern IMU_value imu_value;

=======
#define Kp 1.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.1f   // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f

>>>>>>> origin/master
void IMU_init(void);
void IMU_AHRSupdate(void);
void IMU_getYawPitchRoll(void);

#endif
