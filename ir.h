

#ifndef IR_H
#define	IR_H

#include <xc.h>
#define _XTAL_FREQ 8000000

struct Sensor_ir {
    int left; 
    int right; 
   
};

void init_TIMER5();
void init_capture();
void print_IR();

#endif	

