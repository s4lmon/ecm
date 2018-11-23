#include "ir.h"


/*
 * Timer 5 for input capture, initialisation
 */
void init_TIMER5(void)
{
    /*need to reset TMR5
     *see page 139
     */
     
    
    T5CONbits.T5SEN = 0;        /*disable during sleep            */
    T5CONbits.RESEN = 0;        /*disable special event trigger   */
    T5CONbits.T5PS = 0b00;      /*prescale, unsure TODO           */
    T5CONbits.T5SYNC = 0;       /*continuous count                */
    T5CONbits.TMR5CS = 0;       /*internal clock                  */
    T5CONbits.TMR5ON = 1;       /*turn on                         */
    
}

/*
 * Detect falling edge to rising edge from IR detector
 */
void init_capture(void)
{
    ANSEL0 = 0; /*configuring digital input */
    ANSEL1 = 0;
    
    /*set up capture pins 1 and 2 from PIC*/
    CAP1CONbits.CAP1REN = 1;
    CAP1CONbits.CAP1M = 0b0110;
    CAP1CONbits.CAP1REN = 1;
    CAP1CONbits.CAP1M = 0b0110;
}

/*
 * Printing IR signal
 */
void print_IR(struct Sensor_ir *value)
{
    *value.left = (CAP2BUFH << 8) | (CAP2BUFL); //left is cap2, i.e. lower
    *value.right = (CAP1BUFH << 8) | (CAP1BUFL);
}