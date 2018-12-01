//#ifndef _RFID_H
//#define _RFID_H
//
//#include <xc.h>
//#include "lcd.h"
//#define _XTAL_FREQ 8000000 //define _XTAL_FREQ so delay routines work
//
////Refer to RFID.c for function description
//void RFID_init(void);
//void Serial_string(char *string);
//char getCharSerial(void);
//void sendCharSerial(char charToSend) ;
//
//#endif

#ifndef _RFID_H
#define _RFID_H

#include <xc.h>
#include "lcd.h"
#define _XTAL_FREQ 8000000 //define _XTAL_FREQ so delay routines work

//Refer to RFID.c for function description
void init_RFID(void);
void interrupt_EUSART(void);

void print_RFID(char *pa,char *dis);
void check_sum(char *str, char *arr_check, int *bit16, int btc);

#endif