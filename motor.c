#include "motor.h"

/*
 * To initialise motor PWM
 */
void initPWM (void)
{ 
    TRISB=0; //setting port as output TODO remove 

    PTCON0=0b00000000; 
    PTCON1=0b10000000; 
    
    PWMCON0=0b01111111; 
    PWMCON1=0x00;
    
    /*base period low and high bytes*/
    PTPERL=0b11000111; 
    PTPERH=0b0000;
}




    void setMotorPWM(struct DC_motor * m) {
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


void accelerate(struct DC_motor *m) {
    for (m->power; (m->power) <= 100; m->power++) { //increase motor power until 100
        setMotorPWM(m); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

void decelerate(struct DC_motor *m) {
    for (m->power; (m->power) > 0; m->power--) { //increase motor power until 100
        setMotorPWM(m); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

// high level functions to control left and right motor
// use setMotorPWM to perform low level update
// bring the robot base to a stop

void stop(struct DC_motor *mL, struct DC_motor *mR) {
   // mL->direction = 0;
   // mR->direction = 0;
    while (mL->power>0 || mR->power>0){
        if(mL->power>0){mL->power--;}
        if(mR->power>0){mR->power--;}
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

void turnLeft(struct DC_motor *mL, struct DC_motor *mR){
    //setMotorStop(mL);
    //setMotorStop(mR);
    mL->direction = 0;
    mR->direction = 1;
    while (mL->power<100 || mR->power<100){
        if(mL->power<100){mL->power++;}
        if(mR->power<100){mR->power++;}
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}

void turnRight(struct DC_motor *mL, struct DC_motor *mR){
    //setMotorStop(mL);
    //setMotorStop(mR);
    mL->direction = 1;
    mR->direction = 0;
    while (mL->power<100 || mR->power<100){
        if(mL->power<100){mL->power++;}
        if(mR->power<100){mR->power++;}
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}
// both motors forward and up to full power
void forwards(struct DC_motor *mL, struct DC_motor *mR){
    mL->direction = 0;
    mR->direction = 0;
    while (mL->power<100 || mR->power<100){
        if(mL->power<100){mL->power++;}
        if(mR->power<100){mR->power++;}
        setMotorPWM(mL); //pass pointer to setMotorSpeed function (not &m here)
        setMotorPWM(mR); //pass pointer to setMotorSpeed function (not &m here)
        __delay_ms(5); //delay of 5 ms (500 ms from 0 to 100 full power)
    }
}
    
void main(void) {
     
    OSCCON = 0x72; //8MHz clock
    while (!OSCCONbits.IOFS); //wait until stable
    initPWM(); //setup PWM registers
    
    struct DC_motor mL, mR; //declare two DC_motor structures
    mL.power=0; //zero power to start
    mL.direction=1; //set default motor direction
    mL.duty_low=(unsigned char *)(&PDC0L); //store address of PWM duty low byte
    mL.duty_high=(unsigned char *)(&PDC0H); //store address of PWM duty high byte
    mL.dir_pin=0; //pin RB0/PWM0 controls direction
    mL.period=199; //store PWMperiod for motor
    //same for motorR but different PWM registers and direction pin
    
    mR.power=0; //zero power to start
    mR.direction=1; //set default motor direction
    mR.duty_low=(unsigned char *)(&PDC1L); //store address of PWM duty low byte
    mR.duty_high=(unsigned char *)(&PDC1H); //store address of PWM duty high byte
    mR.dir_pin=2; //pin RB0/PWM0 controls direction
    mR.period=199; //store PWMperiod for motor
    //same for motorR but different PWM registers and direction pin


}



