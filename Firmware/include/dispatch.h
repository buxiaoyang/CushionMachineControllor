#ifndef 		__DISPATCH_H__
#define 		__DISPATCH_H__

extern unsigned char ResetMotorDispatchSteps;

extern unsigned long xdata lastStepPWMs1; //������������1
extern unsigned char xdata lastStatus1; //�������1

extern unsigned long xdata lastStepPWMs2; //������������1
extern unsigned char xdata lastStatus2; //�������1

void ResetMotorDispatch(void);

void motor1Forward(void);

void motor1Backward(void);

void motor2Copy(void);

void motor3Forward(void);

void motor3Backward(void);

void motor4Copy(void);

#endif