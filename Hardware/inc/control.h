#ifndef _CONTROL_H
#define _CONTROL_H

//PWM duty rate
#define MAXPWMDP 0.55f
#define MINPWMDP 0.1f
#define MAXPWMNUMBER 900
<<<<<<< HEAD
#define INTEGRATION_LENGTH 100
=======
>>>>>>> origin/master

extern int Motor[4];

typedef struct{
  float Yaw;
  float Pitch;
  float Roll;
  float Thr;
}Direction;
<<<<<<< HEAD

typedef struct{
  float Yaw[INTEGRATION_LENGTH];
  float Pitch[INTEGRATION_LENGTH];
  float Roll[INTEGRATION_LENGTH];
}Diff_integration;

=======
>>>>>>> origin/master
extern Direction expected_Direction;

void InitDirection(void);
void CalPID(void);
void RefreshMotor(void);
<<<<<<< HEAD
void Diff_Integration_Initialize(void);
=======
>>>>>>> origin/master

#endif
