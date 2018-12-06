#pragma config OSC = IRCIO, LVP = OFF, MCLRE = OFF
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define _XTAL_FREQ 8000000
#define PWMPERIOD 199
#define TIME 100
#define DELAY 500
#include "lcd.h"
#include "ir.h"
#include "motor.h"
#include "rfid.h"
#include "reverse.h"


volatile char card_read; //value will change during the code at any time therefore it must be a volatile character 
char string_rfid[17]; //global variable because it is used in the RFID interrupt and called in the main loop 
//
volatile char count_encoder = 0;

void __interrupt(high_priority) InterruptHandlerHigh() {

    //RFID tag
    static char count = 0;
    char rx_char;

    if (PIR1bits.RCIF) { //if a character is read from the rfid tag
        rx_char = RCREG;
        if (rx_char == 0x02) { //if it is the first character of the message
            count = 0;
        }
        if (rx_char == 0x03) { //when all the message is read
            card_read = 1; //enter RFID reading and checksum functions
        }
        string_rfid[count] = rx_char; //store the character read by the RFID tag into a character string
        count++;
    }

    //    if (INTCONbits.INT0IF) {
    //        count_encoder++; // Increment the encoder counter
    //
    //        INTCONbits.INT0IF = 0; // Clear the encoder flag
    //    }

    if (INTCONbits.INT0IF) { //external interrupt flag

        if (PORTCbits.RC3 == 1) {
            if (PORTCbits.RC3 == 1) {


                card_read = 0; //re-start the searching routine

            }
        }

        INTCONbits.INT0IF = 0; //clear the interrupt flag
    }

}

void main(void) {


    char counter = 0;
    int forwardsDirection[50];
    //        int forwardsTime[50];
    //        struct Direction There;
    ANSEL0 = 0; //Override start up analogue mode to digital instead
    ANSEL1 = 0;
    OSCCON = 0x72;
    while (!OSCCONbits.IOFS);

    LCD_init();

    init_TIMER5();
    initPWM();
    init_capture();
    init_RFID();



    interrupt_EUSART();
    struct Sensor_ir Values;
    struct Motor mL, mR;
    int PWMcycle = 199;
    mL.power = 0; //zero power to start
    mL.direction = 1; //set default motor direction, forward
    mL.duty_low = (unsigned char *) (&PDC0L); //store address of PWM duty low byte
    mL.duty_high = (unsigned char *) (&PDC0H); //store address of PWM duty high byte
    mL.dir_pin = 0; //pin RB0/PWM0 controls direction
    mL.period = PWMcycle; //store PWMperiod for motor

    //same for motorR but different PWM registers and direction pin
    mR.power = 0;
    mR.direction = 1;
    mR.duty_low = (unsigned char *) (&PDC1L);
    mR.duty_high = (unsigned char *) (&PDC1H);
    mR.dir_pin = 2; //pin RB2/PWM0 controls direction
    mR.period = PWMcycle;

    forwards(&mL, &mR);
    stop(&mL, &mR);
    stop(&mL, &mR);
    INTCONbits.INT0IE = 1;

    card_read = 3;

    while (1) {
        while (card_read == 3) {
            //            stop(&mL, &mR);
            LCD_line(1);
            char buf[16];
            sprintf(buf, "Ready");
            LCD_string(buf);
            //            while (PORTCbits.RC3 == 0);
        }

        int direction = 0;
        //Searching for IR emitter 

        while (card_read == 0) { //Card read is set to 0 by the interrupt set by pressing the button
            read_IR(&Values);
            //            Values.left = measureIRLeft();
            //            Values.right = measureIRRight();

            print_IR(&Values);
                        LCD_clear();
                        char buf[16];
                        LCD_line(1);
                        sprintf(buf, "Count: %c", counter);
                        LCD_string(buf);
            //
                        __delay_ms(50);
            //            __delay_ms(50);

            int threshold = 50;
            int diff = Values.left - Values.right;
            if (Values.left > 100 | Values.right > 100) {
                threshold = 20;
            } else {
                threshold = 5;
            }

            if ((diff < -threshold)) {
                if (direction != 1) {
                    stop(&mL, &mR);
                }
                
//                stop(&mL, &mR);
                
                direction = 1;
                counter++;
                forwardsDirection[counter] = direction;
                turnLeft(&mL, &mR);
                __delay_ms(DELAY);
    
            } else if (diff > threshold) {
                if (direction != 2) {
                    stop(&mL, &mR);
                }
//                stop(&mL, &mR);
                
                direction = 2;
                counter++;
                forwardsDirection[counter] = direction;
                turnRight(&mL, &mR);
                __delay_ms(DELAY);
                
            } else { //either forwards or signal lost
                if ((Values.left > 199) | (Values.right > 199)) {
                    if (direction != 0) {
                        stop(&mL, &mR);
                    }
//                    stop(&mL, &mR);
                    
                    direction = 3;
                    counter++;
                    forwardsDirection[counter] = direction;
                    forwards(&mL, &mR);
                    __delay_ms(DELAY);

                } else {
                    if (direction != 2) {
                        stop(&mL, &mR);
                    }
//                    stop(&mL, &mR);
                    
                    direction = 2;
                    counter++;
                    forwardsDirection[counter] = direction;
                    turnRight(&mL, &mR);
                    __delay_ms(DELAY);



                }
            }
        }





        while (card_read == 1) { //the interrupt for the RFID tag sets card_read to 1 when the card is read
            stop(&mL, &mR);
            //            counter = reverse_routine(&forwardsDirection[counter], &forwardsTime[counter], direction, counter);
      
            print_RFID(&string_rfid[0], &string_rfid[0]); //sends the significant characters read from the RFID to the LCD
            __delay_ms(10);

            //        while (((TMR0H << 8) | TMR0L) <= forwardsTime[counter]) {      
            for (char ii = 0; ii < counter; ii++) {

                //                TMR0H = 0;
                //                TMR0L = 0;
                if (forwardsDirection[counter - ii] == 1) {
                    turnRight(&mL, &mR);
                } else if (forwardsDirection[counter - ii] == 2) {
                    turnLeft(&mL, &mR);
                } else if (forwardsDirection[counter - ii] == 3) {
                    backwards(&mL, &mR);
                } else {
                    stop(&mL, &mR);
                }


                __delay_ms(DELAY);
//                stop(&mL, &mR);
                card_read = 3;

            }
            stop(&mL, &mR);


        }
        //            
        //        }
        //        while (card_read == 2) {
        //        
        //            //            }
        //            LCD_clear();
        //            char buf[16];
        //            LCD_line(1);
        //            sprintf(buf, "Ready");
        //            LCD_string(buf);
        //            __delay_ms(50);
        //        }

        //            return 0;

    }
}









