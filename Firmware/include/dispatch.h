#ifndef 		__DISPATCH_H__
#define 		__DISPATCH_H__

extern unsigned char ResetMotorDispatchSteps;
void ResetMotorDispatch(void);

void motor1Forward(void);

void motor1Backward(void);

void motor2Forward(void);

void motor2Backward(void);

void motor2Copy(void);

#endif