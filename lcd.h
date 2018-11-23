/* 
 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 8000000

void E_TOG(void);
void LCD_out(unsigned char number);
void LCD_send(unsigned char Byte, int type);
void LCD_init(void);
void LCD_line (int line);
void LCD_string(char *string);
void LCD_clear(void);


#endif	/* LCD_H */

