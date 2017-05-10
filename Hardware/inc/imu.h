#ifndef _IMU_H
#define _IMU_H

#define Kp 1.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.1f   // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f

void IMU_init(void);
void IMU_AHRSupdate(void);
void IMU_getYawPitchRoll(void);

#endif
