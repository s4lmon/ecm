

#ifndef MOTOR_H
#define	MOTOR_H

#include <xc.h>
#define _XTAL_FREQ 8000000
struct Motor {
    char power; //motor power, out of 100
    char direction; //motor direction, forward(1), reverse(0)
    unsigned char *duty_low; //PWM duty low byte address
    unsigned char *duty_high; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB
    int period; //base period of PWM cycle
};


void initTimer0(void);
void initPWM(void);
void setMotorPWM(struct Motor *m);
void accelerate(struct Motor *m);
void decelerate(struct Motor *m);
void stop(struct Motor *mL, struct Motor *mR) ;
void turnLeft(struct Motor *mL, struct Motor *mR);
void turnRight(struct Motor *mL, struct Motor *mR);
void forwards(struct Motor *mL, struct Motor *mR);
void backwards(struct Motor *mL, struct Motor *mR);
void turnRightSlow(struct Motor *mL, struct Motor *mR) ;
void smoothLeft(struct Motor *mL, struct Motor *mR);
void smoothRight(struct Motor *mL, struct Motor *mR);


#endif	// 
//struct P
//unsigned int :1;
//j}
//flags;

