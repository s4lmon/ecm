#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "lcd.h"
#include "ir.h"
#define _XTAL_FREQ 8000000




void main(void)
{
    OSCCON = 0x72;
    while (!OSCCONbits.IOFS); 
    
    
    char buffer_left[16], buffer_right[16];
    
    struct Sensor_ir *Sensors;
    Sensors.left = 0;
    Sensors.right = 0;
    print_IR(&Sensors);
            
    LCD_infrared(&buffer_left, &buffer_right, &Sensors);
}_

