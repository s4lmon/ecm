

#ifndef IR_H
#define	IR_H

#include <xc.h>
#define _XTAL_FREQ 8000000

struct Sensor_ir {
    unsigned int left; 
    unsigned int right; 
   
};

void init_TIMER5(void);
void init_capture(void);
void read_IR(struct Sensor_ir *values);

#endif	

