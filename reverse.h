

#ifndef REVERSE_H
#define	REVERSE_H

#include <xc.h>
#define _XTAL_FREQ 8000000


struct {
   unsigned int forwards : 2;
   unsigned int left : 2;
   unsigned int right : 2;
} Direction ;

void init_Timer0(void);
void initEncoder(void); 
int reverse_routine(int *backDirection, int *backTime, char dir, int counter);

#endif	

