#include "ir.h"

/*
 * Timer 5 for input capture, initialisation
 */
void init_TIMER5(void) {
    /*need to reset TMR5
     *see page 139
     */

    T5CON = 0;
    T5CONbits.T5SEN = 0; /*disable during sleep            */
    T5CONbits.RESEN = 0; /*disable special event trigger   */
    T5CONbits.T5PS = 0b10; /*prescale, unsure TODO           */
    T5CONbits.T5MOD = 0; /*prescale, unsure TODO           */
    T5CONbits.T5SYNC = 0; /*continuous count                */
    T5CONbits.TMR5CS = 0; /*internal clock                  */
    T5CONbits.TMR5ON = 1; /*turn on                         */

}

/*
 * Detect falling edge to rising edge from IR detector
 */
void init_capture(void) {

    LATA = 0; //Tensure all pins have same value initially
    TRISA = 0b00001100; //Turn on CAP1 (RA2) and CAP2 (RA3) Pins.
    CAP1CON = 0b01000110; //Time base enabled, measure PWM falling to rising 
    CAP2CON = 0b01000110;
    T5CON = 0b00001001; //Timer 5 prescaler 1:2, Timer on (overflow 65.536ms - max PWM is 50ms)
    DFLTCON = 0b00011000; //Noise filter for CAP1 and CAP2 
}

/*
 * Printing IR signal
 */
void read_IR(struct Sensor_ir *Values) {
    Values->left_prev = Values->left;
    Values->right_prev = Values->right;
    Values->left = ((CAP2BUFH << 8) | (CAP2BUFL)); //left is cap2, i.e. lower
    Values->right = ((CAP1BUFH << 8) | (CAP1BUFL));

    if ((Values->left == Values->left_prev) && (Values->left <= 200)) {
        Values->left = 0;
    }

    if ((Values->right == Values->right_prev) && (Values->right <= 200)) {
        Values->right = 0;
    }

    Values->left = Values->left / 64;
    if (Values->left > 200) {
        Values->left = 200;
    } //reduce un-required noise above 200

    Values->right = Values->right / 64;
    if (Values->right > 200) {
        Values->right = 200;
    } //reduce un-required noise above 200

    // __delay_ms(50);
    //    if ((Values->left_prev = Values->left) && (Values->left < 50)) 
    //        {
    //            Values->left = 0;
    //        }
    //    if ((Values->right_prev = Values->right) && (Values->right < 50)) {
    //            {
    //                Values->right = 0;
    //            }

    //        }
}

//int measureIRLeft(void)
//{
//    // Initialise IR level variable
//    // ----------------------------------------------------------------------
//    unsigned int IRlevel = 0;
//
//    // Take a predetermined number of readings, and sum the results
//    // ----------------------------------------------------------------------
//    for (char i = 0; i < 2; i++)
//    {
//         IRlevel += CAP1BUFL;  // Get the 8 bit LSB result
//         IRlevel += ((unsigned int)CAP1BUFH << 8); // Get 2 bit MSB result
//         __delay_ms(50);
//    }
//
//    // Reset to zero
//    // ----------------------------------------------------------------------
//    CAP1BUFL = 0;
//    CAP1BUFH = 0;
//
//    // Calculate the average
//    // ----------------------------------------------------------------------
//    IRlevel = IRlevel/3;
//
//    // Return the averaged value
//    // ----------------------------------------------------------------------
//    return IRlevel;
//}
//
//int measureIRright(void)
//{
//    // Initialise IR level variable
//    // ----------------------------------------------------------------------
//    unsigned int IRlevel = 0;
//
//    // Take a predetermined number of readings, and sum the results
//    // ----------------------------------------------------------------------
//    for (char i = 0; i < 2; i++)
//    {
//         IRlevel += CAP1BUFL;  // Get the 8 bit LSB result
//         IRlevel += ((unsigned int)CAP1BUFH << 8); // Get 2 bit MSB result
//         __delay_ms(50);
//    }
//
//    // Reset to zero
//    // ----------------------------------------------------------------------
//    CAP1BUFL = 0;
//    CAP1BUFH = 0;
//
//    // Calculate the average
//    // ----------------------------------------------------------------------
//    IRlevel = IRlevel/2;
//
//    // Return the averaged value
//    // ----------------------------------------------------------------------
//    return IRlevel;
//}