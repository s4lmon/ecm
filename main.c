#pragma config OSC = IRCIO, LVP = OFF, MCLRE = OFF
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define _XTAL_FREQ 8000000
#define PWMPERIOD 199
#include "lcd.h"
#include "ir.h"
#include "motor.h"
#include "rfid.h"



volatile char card_read; //value will change during the code at any time therefore it must be a volatile character 
char string_rfid[17]; //global variable because it is used in the RFID interrupt and called in the main loop 
//

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

}

void main(void) {
    ANSEL0 = 0; //Override start up analogue mode to digital instead
    ANSEL1 = 0;
    OSCCON = 0x72;
    while (!OSCCONbits.IOFS);
    card_read = 0;
    LCD_init();


    init_capture();
    init_RFID();

    interrupt_EUSART();
    struct Sensor_ir Values;

    while (1) {


        //Searching for IR emitter 

        while (card_read == 0) { //Card read is set to 0 by the interrupt set by pressing the button
                        read_IR(&Values);
                        print_IR(&Values);
        }


        if (card_read == 1) { //the interrupt for the RFID tag sets card_read to 1 when the card is read

            print_RFID(&string_rfid[0], &string_rfid[0]); //sends the significant characters read from the RFID to the LCD
            __delay_ms(10);
        }
    }
}









