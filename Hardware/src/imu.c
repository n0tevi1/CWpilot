#include "imu.h"
#include "spi.h"
#include <math.h>

<<<<<<< HEAD
=======
typedef struct{
  float Roll;    //X axis
  float Pitch;   //Y axis
  float Yaw;     //Z axis
  float Quat[4];
  float Integrate[3];  // integrate of X, Y, Z
}IMU_value;

>>>>>>> origin/master
IMU_value imu_value;

void IMU_init(void){
	imu_value.Quat[0] = 1.0f;
	imu_value.Quat[1] = 0.0f;
	imu_value.Quat[2] = 0.0f;
	imu_value.Quat[3] = 0.0f;
	imu_value.Integrate[0] = 0.0;
	imu_value.Integrate[1] = 0.0;
	imu_value.Integrate[2] = 0.0;
}

void IMU_AHRSupdate(void) {
  float norm;
  float hx, hy, hz, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez;

  // ÏÈ°ÑÕâÐ©ÓÃµÃµ½µÄÖµËãºÃ
  float q0q0 = imu_value.Quat[0]*imu_value.Quat[0];
  float q0q1 = imu_value.Quat[0]*imu_value.Quat[1];
  float q0q2 = imu_value.Quat[0]*imu_value.Quat[2];
  float q0q3 = imu_value.Quat[0]*imu_value.Quat[3];
  float q1q1 = imu_value.Quat[1]*imu_value.Quat[1];
  float q1q2 = imu_value.Quat[1]*imu_value.Quat[2];
  float q1q3 = imu_value.Quat[1]*imu_value.Quat[3];
  float q2q2 = imu_value.Quat[2]*imu_value.Quat[2];
  float q2q3 = imu_value.Quat[2]*imu_value.Quat[3];
  float q3q3 = imu_value.Quat[3]*imu_value.Quat[3];

  norm = sqrt(mpu_value.Accel[0]*mpu_value.Accel[0] + mpu_value.Accel[1]*mpu_value.Accel[1] + mpu_value.Accel[2]*mpu_value.Accel[2]);
  mpu_value.Accel[0] = mpu_value.Accel[0] / norm;
  mpu_value.Accel[1] = mpu_value.Accel[1] / norm;
  mpu_value.Accel[2] = mpu_value.Accel[2] / norm;

  norm = sqrt(mpu_value.Mag[0]*mpu_value.Mag[0] + mpu_value.Mag[1]*mpu_value.Mag[1] + mpu_value.Mag[2]*mpu_value.Mag[2]);
  mpu_value.Mag[0] = mpu_value.Mag[0] / norm;
  mpu_value.Mag[1] = mpu_value.Mag[1] / norm;
  mpu_value.Mag[2] = mpu_value.Mag[2] / norm;

  // compute reference direction of flux
  hx = 2*mpu_value.Mag[0]*(0.5f - q2q2 - q3q3) + 2*mpu_value.Mag[1]*(q1q2 - q0q3) + 2*mpu_value.Mag[2]*(q1q3 + q0q2);
  hy = 2*mpu_value.Mag[0]*(q1q2 + q0q3) + 2*mpu_value.Mag[1]*(0.5f - q1q1 - q3q3) + 2*mpu_value.Mag[2]*(q2q3 - q0q1);
  hz = 2*mpu_value.Mag[0]*(q1q3 - q0q2) + 2*mpu_value.Mag[1]*(q2q3 + q0q1) + 2*mpu_value.Mag[2]*(0.5f - q1q1 - q2q2);
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz;

  // estimated direction of gravity and flux (v and w)
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
  wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
  wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (mpu_value.Accel[1]*vz - mpu_value.Accel[2]*vy) + (mpu_value.Mag[1]*wz - mpu_value.Mag[2]*wy);
  ey = (mpu_value.Accel[2]*vx - mpu_value.Accel[0]*vz) + (mpu_value.Mag[2]*wx - mpu_value.Mag[0]*wz);
  ez = (mpu_value.Accel[0]*vy - mpu_value.Accel[1]*vx) + (mpu_value.Mag[0]*wy - mpu_value.Mag[1]*wx);

  if(ex != 0.0f && ey != 0.0f && ez != 0.0f){
    imu_value.Integrate[0] = imu_value.Integrate[0] + ex * Ki * halfT;
    imu_value.Integrate[1] = imu_value.Integrate[1] + ey * Ki * halfT;
    imu_value.Integrate[2] = imu_value.Integrate[2] + ez * Ki * halfT;

    // adjusted gyroscope measurements
    mpu_value.Gyro[0] = mpu_value.Gyro[0] + Kp*ex + imu_value.Integrate[0];
    mpu_value.Gyro[1] = mpu_value.Gyro[1] + Kp*ey + imu_value.Integrate[1];
    mpu_value.Gyro[2] = mpu_value.Gyro[2] + Kp*ez + imu_value.Integrate[2];
    }

  // integrate quaternion rate and normalise
  imu_value.Quat[0] = imu_value.Quat[0] + (-imu_value.Quat[1]*mpu_value.Gyro[0] - imu_value.Quat[2]*mpu_value.Gyro[1] - imu_value.Quat[3]*mpu_value.Gyro[2])*halfT;
  imu_value.Quat[1] = imu_value.Quat[1] + (imu_value.Quat[0]*mpu_value.Gyro[0] + imu_value.Quat[2]*mpu_value.Gyro[2] - imu_value.Quat[3]*mpu_value.Gyro[1])*halfT;
  imu_value.Quat[2] = imu_value.Quat[2] + (imu_value.Quat[0]*mpu_value.Gyro[1] - imu_value.Quat[1]*mpu_value.Gyro[2] + imu_value.Quat[3]*mpu_value.Gyro[0])*halfT;
  imu_value.Quat[3] = imu_value.Quat[3] + (imu_value.Quat[0]*mpu_value.Gyro[2] + imu_value.Quat[1]*mpu_value.Gyro[1] - imu_value.Quat[2]*mpu_value.Gyro[0])*halfT;

  // normalise quaternion
  norm = sqrt(imu_value.Quat[0]*imu_value.Quat[0] + imu_value.Quat[1]*imu_value.Quat[1] + imu_value.Quat[2]*imu_value.Quat[2] + imu_value.Quat[3]*imu_value.Quat[3]);
  imu_value.Quat[0] = imu_value.Quat[0] / norm;
  imu_value.Quat[1] = imu_value.Quat[1] / norm;
  imu_value.Quat[2] = imu_value.Quat[2] / norm;
  imu_value.Quat[3] = imu_value.Quat[3] / norm;
}

void IMU_getYawPitchRoll(void) {
  imu_value.Yaw = -atan2(2 * imu_value.Quat[1] * imu_value.Quat[2] + 2 * imu_value.Quat[0] * imu_value.Quat[3], -2 * imu_value.Quat[2]*imu_value.Quat[2] - 2 * imu_value.Quat[3] * imu_value.Quat[3] + 1)* 180/PI; // yaw
<<<<<<< HEAD
  imu_value.Pitch = -asin(-2 * imu_value.Quat[1] * imu_value.Quat[3] + 2 * imu_value.Quat[0] * imu_value.Quat[2]) 	* 180/PI; // pitch
=======
  imu_value.Pitch = -asin(-2 * imu_value.Quat[1] * imu_value.Quat[3] + 2 * imu_value.Quat[0] * imu_value.Quat[2])* 180/PI; // pitch
>>>>>>> origin/master
  imu_value.Roll = atan2(2 * imu_value.Quat[2] * imu_value.Quat[3] + 2 * imu_value.Quat[0] * imu_value.Quat[1], -2 * imu_value.Quat[1] * imu_value.Quat[1] - 2 * imu_value.Quat[2] * imu_value.Quat[2] + 1)* 180/PI; // roll
  //if(imu_value.Yaw<0)imu_value.Yaw+=360.0f;
}
