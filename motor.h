

#ifndef MOTOR_H
#define	MOTOR_H

#include <xc.h>
#define _XTAL_FREQ 8000000



struct DC_motor {
    char power; //motor power, out of 100
    char direction; //motor direction, forward(1), reverse(0)
    unsigned char *duty_low; //PWM duty low byte address
    unsigned char *duty_high; //PWM duty high byte address
    char dir_pin; // pin that controls direction on PORTB
    int period; //base period of PWM cycle
};


#endif	

