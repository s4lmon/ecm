

#ifndef REVERSE_H
#define	REVERSE_H

#include <xc.h>
#define _XTAL_FREQ 8000000


struct {
   unsigned int forwards : 2;
   unsigned int left : 2;
   unsigned int right : 2;
} Direction ;

void initTimer0(void);
void initEncoder(void); 

#endif	

