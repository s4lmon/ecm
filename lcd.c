#include "lcd.h"
/*To toggle enable bit on then off*/
void E_TOG(void)
{
    PORTCbits.RC0=1;
    __delay_us(5);
    PORTCbits.RC0=0;
    __delay_us(5);
}
 
/*remake*/
void time (int t)
{
    for(int k=0;k<t;k++) 
    {__delay_ms(90);
    }
}
/*To create 4-bit data stream to the LCD*/
void LCD_out(unsigned char number)

{

    PORTCbits.RC1=(number&0b0001);
    PORTCbits.RC2=(number&0b0010)>>1;
    PORTDbits.RD0=(number&0b0100)>>2;
    PORTDbits.RD1=(number&0b1000)>>3;
    //toggle the enable bit to send data
    E_TOG();
    __delay_us(5); 
}
 

/*
 * function to send data over 4 bit interface
 */
void LCD_send (unsigned char Byte,char type)
{
    // set RS pin whether it is a Command (0) or Data/Char (1)
    // using type as the argument
    /*TODO needs to be redone, no need for command anymore*/
    PORTAbits.RA6=type;
    
    // send high bits of Byte using LCDout function
    LCD_out(Byte>>4);
    __delay_us(10); //10us delay
    // send low bits of Byte using LCDout function
    LCD_out(Byte);
    __delay_us(50); 
    
}
 
void LCD_init(void)
{

    /* Configuring the outputs, else unknown*/
    LATA=0;
    LATC=0;
    LATD=0;
    
    /*Initialise pins for LCD from PIC, TODO redo and move LCD*/
    TRISA=0;  
    TRISC=0; //set the data direction registers to output on all pins
    TRISD=0;  
 
    
    /*Initialisation sequence, page TODO of datasheet*/
    __delay_ms(15); 
    LCD_out(0b0011);
    __delay_ms(5);  
    LCD_out(0b0011); 
    __delay_us(200);
    LCD_out(0b0011);
    __delay_us(50); 
    LCD_out(0b0010); 
    __delay_us(50);

    LCD_send(0b00101000,0); /*4 bit mode, 2 line, 5 x 7 grid*/
    __delay_ms(2); 
    LCD_send(0b00001000,0); 
    __delay_ms(2); 
    LCD_send(0b00000001,0);
    __delay_ms(2); 
    LCD_send(0b00000110,0);
    __delay_ms(2); 
    LCD_send(0b00001100,0);
    __delay_ms(2); 
 
}
/*
 * To set cursor to correct line (of two)
 */
void LCD_line (char line)
{
    if (line == 1){
    LCD_send(0x80,0); /*DDRAM address for line 1*/
    }
    else if (line == 2){
    LCD_send(0xC0,0);
    }
    __delay_us(50); //50us delay
}
/*
 * To print character by character to screen
 */
void LCD_string (char *string)
{
while(*string != 0)
    {
    //Send out the current byte pointed to
    //and increment the pointer
    LCD_send(*string++,1);
    __delay_us(50); //so we can see each character
    //being printed in turn (remove delay if you want
    //your message to appear almost instantly)
    }
}
/*
 * To clear LCD
 */
void LCD_clear(void)
{
    LCD_send(0b00000001,0);
    __delay_ms(2);
}
/*
 * To print IR values to LCD
 */
void LCD_infrared(char *buffer_left, char *buffer_right, struct Sensor_ir)
{
    LCD_clear();
    
    sprintf(buffer_left);
    sprintf(buffer_right);
    
    LCD_line(1);
    LCD_string(buffer_right);
    LCD_line(2);
    LCD_string(buffer_left);
    __delay_ms(50);
    __delay_ms(50);
    __delay_ms(50);
    __delay_ms(50);

}