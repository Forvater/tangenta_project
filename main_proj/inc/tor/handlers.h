#ifndef HANDLERS_H
#define HANDLERS_H

#include "xparameters.h"
#include "xuartlite.h"
#include "xuartlite_i.h"
#include "xtmrctr.h"
#include "xtmrctr_i.h"
#include "global_variables.h"

void handler_RS232(void *arg2);
void handler_Timer(void *arg2);

#endif
