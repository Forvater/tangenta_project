#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "xgpio.h"
#include "xintc.h"
#include "xtmrctr.h"

extern XIntc intc;
extern XGpio photo;
extern XGpio ircom;
extern XGpio kt;
extern XTmrCtr tmr;

extern unsigned int i, j, k, k2, l;
extern unsigned char coms, nomer, ir_t, rab, sost_rs, sost_timer, ir_nomer, ir_start;
extern unsigned char ir_nomer_start;
extern unsigned int ir_delta, ir_delay,er, photo_1,photo_2,photo_3,photo_4;
extern unsigned int volatile t;

#endif
