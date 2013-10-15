#ifndef INIT_H
#define INIT_H

#include "xparameters.h"
#include "xuartlite.h"
#include "xuartlite_i.h"
#include "xintc_l.h"
#include "xintc_i.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xtmrctr_i.h"

#include "handlers.h"

#include "global_variables.h"


void delay_ms(unsigned int time);
void ir_on(unsigned char t);
void delay_us(unsigned int a);
void init_all(void);

#endif
