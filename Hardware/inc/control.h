#ifndef _CONTROL_H
#define _CONTROL_H

//PWM duty rate
#define MAXPWMDP 0.55f
#define MINPWMDP 0.1f
#define MAXPWMNUMBER 900

extern int Motor[4];

typedef struct{
  float Yaw;
  float Pitch;
  float Roll;
  float Thr;
}Direction;
extern Direction expected_Direction;

void InitDirection(void);
void CalPID(void);
void RefreshMotor(void);

#endif
