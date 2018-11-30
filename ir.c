#include "ir.h"


/*
 * Timer 5 for input capture, initialisation
 */
void init_TIMER5(void)
{
    /*need to reset TMR5
     *see page 139
     */
     
    T5CON = 0;
    T5CONbits.T5SEN = 0;        /*disable during sleep            */
    T5CONbits.RESEN = 0;        /*disable special event trigger   */
    T5CONbits.T5PS = 0b10;      /*prescale, unsure TODO           */
    T5CONbits.T5MOD = 0;      /*prescale, unsure TODO           */
    T5CONbits.T5SYNC = 0;       /*continuous count                */
    T5CONbits.TMR5CS = 0;       /*internal clock                  */
    T5CONbits.TMR5ON = 1;       /*turn on                         */
    
}

/*
 * Detect falling edge to rising edge from IR detector
 */
void init_capture(void)
{

//    LATA = 0;
//    TRISA = 0b00001100;
//    /*set up capture pins 1 and 2 from PIC*/
//    CAP1CONbits.CAP1REN = 1;
//    CAP1CONbits.CAP1M = 0b0110;
//    CAP1CONbits.CAP1REN = 1;
//    CAP1CONbits.CAP1M = 0b0110;
//    ANSEL0 = 0; /*configuring digital input */
//    ANSEL1 = 0;
//    DFLTCON = 0b00011000;
    
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
void read_IR(struct Sensor_ir *values)
{
    values->left  = ((CAP2BUFH << 8) | (CAP2BUFL))/1; //left is cap2, i.e. lower
    values->right = ((CAP1BUFH << 8) | (CAP1BUFL))/1;
}