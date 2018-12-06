#include "motor.h"
#define POWER 70
#define POWER2 70
#define POWER3 80
#define DELAYTIME 300

/*
 * To initialise motor PWM
 */
void initPWM(void) {
    TRISB = 0; //setting port as output TODO remove 

    PTCON0 = 0b00000000;
    PTCON1 = 0b10000000;

    PWMCON0 = 0b01111111;
    PWMCON1 = 0x00;

    /*base period low and high bytes*/
    PTPERL = 0b11000111;
    PTPERH = 0b0000;
}

void setMotorPWM(struct Motor *m) {
    int PWMduty; //tmp variable to store PWM duty cycle
    if (m->direction) { //if forward
        // low time increases with power
        PWMduty = m->period - ((int) (m->power)*(m->period)) / 100;
    } else { //if reverse
        // high time increases with power
        PWMduty = ((int) (m->power)*(m->period)) / 100;
    }
    PWMduty = (PWMduty << 2); // two LSBs are reserved for other things
    *(m->duty_low) = PWMduty & 0xFF; //set low duty cycle byte
    *(m->duty_high) = (PWMduty >> 8) & 0x3F; //set high duty cycle byte

    if (m->direction) { // if direction is high,
        LATB = LATB | (1 << (m->dir_pin)); // set dir_pin bit in LATB to high
    } else { // if direction is low,
        LATB = LATB & (~(1 << (m->dir_pin))); // set dir_pin bit in LATB to low
    }
}

//Control which motor turns on and which direction it rotates

void accelerate(struct Motor *m) {
    for (m->power; (m->power) <= POWER; m->power++) { //increase motor power until 100
        setMotorPWM(m); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

void accelerate2(struct Motor *m) {
    for (m->power; (m->power) <= POWER2; m->power++) { //increase motor power until 100
        setMotorPWM(m); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

void decelerate(struct Motor *m) {
    for (m->power; (m->power) > 0; m->power--) { //increase motor power until 100
        setMotorPWM(m); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

// high level functions to control left and right motor
// use setMotorPWM to perform low level update
// bring the robot base to a stop

void stop(struct Motor *mL, struct Motor *mR)
{
    // While either motor is more than zero
    // ----------------------------------------------------------------------
    while (((mR->power) > 0)||((mL->power) > 0))
    {
        // Check to see if either motor is zero, and decrement respectively
        // ------------------------------------------------------------------
//        if ((mR->power) > 0)  // If right still more than zero
//        {
//            (mR->power)--;      
//        }
//        if ((mL->power) > 0)  // If left still more than zero
//        {
//            (mL->power)--;
//        }   
        mL->power = 0;
        mR->power = 0;
        // Update PWM for each
        // ------------------------------------------------------------------
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(1);
    }
}

void turnLeft(struct Motor *mL, struct Motor *mR) {
    //setMotorStop(mL);
    //setMotorStop(mR);
    mL->direction = 0;
    mR->direction = 1;
    while (mL->power < POWER2 || mR->power < POWER2) {
        if (mL->power < POWER2) {
            mL->power++;
        }
        if (mR->power < POWER2) {
            mR->power++;
        }
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }

}

void turnRight(struct Motor *mL, struct Motor *mR) {
    //setMotorStop(mL);
    //setMotorStop(mR);
    mL->direction = 1;
    mR->direction = 0;
    while (mL->power < POWER2 || mR->power < POWER2) {
        if (mL->power < POWER2) {
            mL->power++;
        }
        if (mR->power < POWER2) {
            mR->power++;
        }
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

//void turnRightSlow(struct Motor *mL, struct Motor *mR) {
//    //setMotorStop(mL);
//    //setMotorStop(mR);
//    mL->direction = 1;
//    mR->direction = 0;
//    while (mL->power < POWER3 || mR->power < POWER3) {
//        if (mL->power < POWER3) {
//            mL->power++;
//        }
//        if (mR->power < POWER3
//                ) {
//            mR->power++;
//        }
//        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
//        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
//        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
//    }
//}
// both motors forward and up to full power

void forwards(struct Motor *mL, struct Motor *mR) {
    mL->direction = 0;
    mR->direction = 0;
    while (mL->power < POWER || mR->power < POWER) {
        if (mL->power < POWER) {
            mL->power++;
        }
        if (mR->power < POWER) {
            mR->power++;
        }
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
        //    }
        //        mR->direction = 0;
        //        while (mL->power < POWER2 || mR->power < POWER2) {
        //            if (mL->power < POWER2) {
        //                mL->power++;
        //            }
        //            if (mR->power < POWER2) {
        //                mR->power++;
        //            }
        //            setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        //            setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        //            __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
        //        }
    }
}

void backwards(struct Motor *mL, struct Motor *mR) {
    mL->direction = 1;
    mR->direction = 1;
    while (mL->power < POWER || mR->power < POWER) {
        if (mL->power < POWER) {
            mL->power++;
        }
        if (mR->power < POWER) {
            mR->power++;
        }
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)

    }
}



