#include "global_variables.h"

XIntc intc;
XGpio photo;
XGpio ircom;
XGpio kt;
XTmrCtr tmr;

unsigned int i, j, k, k2, l;
unsigned char coms, nomer, ir_t, rab, sost_rs, sost_timer, ir_nomer, ir_start;
unsigned char ir_nomer_start;
unsigned int ir_delta, ir_delay,er, photo_1,photo_2,photo_3,photo_4;
unsigned int volatile t;
