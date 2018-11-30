/* 
 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h>
#include <stdio.h>

#include "ir.h"
#define _XTAL_FREQ 8000000

void E_TOG(void);
void LCD_out(unsigned char number);
void LCD_send(unsigned char Byte, char type);
void LCD_init(void);
void LCD_line (char line);
void LCD_string(char *string);
void LCD_clear(void);
//void LCD_infrared(char *buffer_left, char *buffer_right, struct Sensor_ir *Values);

#endif	/* LCD_H */

