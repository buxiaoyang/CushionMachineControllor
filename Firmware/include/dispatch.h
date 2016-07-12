#ifndef 		__DISPATCH_H__
#define 		__DISPATCH_H__

extern unsigned char ResetMotorDispatchSteps;

extern unsigned long xdata lastStepPWMs1; //单步总脉冲数1
extern unsigned char xdata lastStatus1; //电机方向1

extern unsigned long xdata lastStepPWMs2; //单步总脉冲数1
extern unsigned char xdata lastStatus2; //电机方向1

void ResetMotorDispatch(void);

void motor1Forward(void);

void motor1Backward(void);

void motor2Copy(void);

void motor3Forward(void);

void motor3Backward(void);

void motor4Copy(void);

#endif