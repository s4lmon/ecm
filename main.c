#pragma config OSC = IRCIO, LVP = OFF, MCLRE = OFF
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define _XTAL_FREQ 8000000
#define PWMPERIOD 199
#include "lcd.h"
#include "ir.h"
#include "motor.h"
//
//void IRScreen(int LeftIR, int RightIR) { //write IR values on screen
//    char buf[16];
//    SendLCD(0b00000001, 0); //clear screen              
//    sprintf(buf, "Left: %u", LeftIR);
//    LCD_string(buf);
//    SetLine(2);
//    sprintf(buf, "Right: %u", RightIR);
//    LCD_string(buf);
//}

void main(void) {
    ANSEL0 = 0; //Override start up analogue mode to digital instead
    ANSEL1 = 0;
    OSCCON = 0x72; //Set up timer
    while (!OSCCONbits.IOFS); //Wait till timer set up


    // initalise 
    LCD_init();
    init_capture();

    struct Sensor_ir values;
    
    while (1) {
        read_IR(&values);
  
        print_IR(&values);
        char buf[16];
        LCD_send(0b00000001, 0); //clear screen 
        LCD_line(1);
        sprintf(buf, "Left: %u", values.left);
        LCD_string(buf);
        LCD_line(2);
        sprintf(buf, "Right: %u", values.right);
        LCD_string(buf);



        //moveMotor(&motorL, &motorR,40,40,1,0);
        __delay_ms(500);
    }


}


//print_IR(&buffer_left, &buffer_right, &Sensors);
//initPWM(); //setup PWM registers



