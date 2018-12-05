#include <xc.h>
#include "reverse.h"
#include "motor.h"

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

void initEncoder(void) {
    /*
     *Enable interrupt and set as high priority 
     */
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IP = 1;
    INTCON2bits.INTEDG2 = 1; /* driven by rising edge */
    INTCON3bits.INT2IF = 0; /*flag set to 0*/
    /*
     * Set LAT and TRIS
     */
    LATCbits.LATC5 = 0;
    TRISCbits.RC5 = 0;
}

//void reverse_routine{
//    switch () {
//            // If action was forwards, reverse
//        case Direction.forwards:
//            reverse(mL, mR);
//
//            break;
//
//            // If action was a left turn, turn right
//        case Direction.left:
//            turnRight(mL, mR); // Adjust power to compensate
//            // for tendency of base to turn
//            break;
//
//            // If action was a right turn, turn left
//        case Direction.right:
//            turnLeft(mL, mR); // Adjust power to compensate
//            // for tendency of base to turn
//            break;
//
//            // Otherwise, stop the motors
//        default:
//            stop(mL, mR);
//
//            break;
//    }
//}
//
//int reverse_store(struct Direction *reverseDirection, char forwardDirection, int forwardTime,  ) 
//counter++;
//return counter;