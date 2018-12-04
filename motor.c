#include "motor.h"
#define POWER 100
#define POWER2 50
#define POWER3 50
void initTimer0(void) {
    T0CONbits.TMR0ON = 1; //turn on timer0
    T0CONbits.T016BIT = 0; // 16bit mode
    T0CONbits.T0CS = 1; // Transition on T0CKI pin input edge
    T0CONbits.PSA = 0; //Enable prescaler
    T0CONbits.T0PS = 0b001; // set prescaler value to 4
    T0CONbits.T0SE = 0; // Increment on low-to-high transition on T0CKI pin
    TMR0L = 0; //Initialise TMR0L and TMR0H to 0, these are re-set to 0 every time the back_trace_counter step increases
    TMR0H = 0;
}

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

void decelerate(struct Motor *m) {
    for (m->power; (m->power) > 0; m->power--) { //increase motor power until 100
        setMotorPWM(m); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

// high level functions to control left and right motor
// use setMotorPWM to perform low level update
// bring the robot base to a stop

void stop(struct Motor *mL, struct Motor *mR) {
    // mL->direction = 0;
    // mR->direction = 0;
    while (mL->power > 0 || mR->power > 0) {
        if (mL->power > 0) {
            mL->power--;
        }
        if (mR->power > 0) {
            mR->power--;
        }
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
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
void turnRightSlow(struct Motor *mL, struct Motor *mR) {
    //setMotorStop(mL);
    //setMotorStop(mR);
    mL->direction = 1;
    mR->direction = 0;
    while (mL->power < POWER3 || mR->power < POWER3) {
        if (mL->power < POWER3) {
            mL->power++;
        }
        if (mR->power < POWER3
                ) {
            mR->power++;
        }
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}
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
    }
}





