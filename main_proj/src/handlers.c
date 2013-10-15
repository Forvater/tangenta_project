#include "handlers.h"

////////////  Œ¡–¿¡Œ“◊» » œ–≈–€¬¿Õ»…  //////////////////////////////////////////
void handler_RS232(void *arg2) {
  if(!XUartLite_mIsReceiveEmpty(XPAR_UARTLITE_0_BASEADDR)) {
    sost_rs =  XUartLite_RecvByte(XPAR_UARTLITE_0_BASEADDR);
  }
}

void handler_Timer(void *arg2) {
  unsigned int csr = XTmrCtr_mGetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR, 0);
  t=t-1;
  XTmrCtr_mSetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR, 0, csr);
}
