#include "init.h"

void delay_ms(unsigned int time) {
  t = time;
  XIntc_mEnableIntr(XPAR_INTC_0_BASEADDR,
                    XPAR_XPS_TIMER_0_INTERRUPT_MASK |
                    XPAR_XPS_UARTLITE_0_INTERRUPT_MASK);
  while (t > 1) {};
  XIntc_mEnableIntr(XPAR_INTC_0_BASEADDR,
                    //XPAR_XPS_TIMER_0_INTERRUPT_MASK |
                    XPAR_XPS_UARTLITE_0_INTERRUPT_MASK);
};

void ir_on(unsigned char t) {
  switch(t){
    case 0:
      XGpio_DiscreteWrite(&ircom,1, 0x0000);
      break;
    case 1:
      XGpio_DiscreteWrite(&ircom,1, 0x0001);
      break;
    case 2:
      XGpio_DiscreteWrite(&ircom,1, 0x0002);
      break;
    case 3:
      XGpio_DiscreteWrite(&ircom,1, 0x0004);
      break;
    case 4:
      XGpio_DiscreteWrite(&ircom,1, 0x0008);
      break;
    case 5:
      XGpio_DiscreteWrite(&ircom,1, 0x0010);
      break;
    case 6:
      XGpio_DiscreteWrite(&ircom,1, 0x0020);
      break;
    case 7:
      XGpio_DiscreteWrite(&ircom,1, 0x0040);
      break;
    case 8:
      XGpio_DiscreteWrite(&ircom,1, 0x0080);
      break;
    case 9:
      XGpio_DiscreteWrite(&ircom,1, 0x0100);
      break;
    case 10:
      XGpio_DiscreteWrite(&ircom,1, 0x0200);
      break;
    case 11:
      XGpio_DiscreteWrite(&ircom,1, 0x0400);
      break;
    case 12:
      XGpio_DiscreteWrite(&ircom,1, 0x0800);
      break;
    default:
      XGpio_DiscreteWrite(&ircom,1, 0x0000);
      break;
  }
};

void delay_us(unsigned int a) {
  unsigned i,j;
  for (i = 0; i < a; i++) {
    j++;
  }
}

void init_all(void) {
  XIntc_Initialize(&intc, XPAR_XPS_INTC_0_DEVICE_ID);
  microblaze_enable_interrupts();
  XIntc_mMasterEnable(XPAR_INTC_0_BASEADDR);
  XUartLite_mEnableIntr(XPAR_UARTLITE_0_BASEADDR);
  ////  ÐÅÃÈÑÒÐÀÖÈß  ÎÁÐÀÁÎÒ×ÈÊÎÂ ////////////////////////////////////////////
  XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
                        XPAR_INTC_0_UARTLITE_0_VEC_ID,
                        (XInterruptHandler)handler_RS232,
                        (void *)0);
  XIntc_RegisterHandler(XPAR_XPS_INTC_0_BASEADDR,
                        XPAR_INTC_0_TMRCTR_0_VEC_ID,
                        (XInterruptHandler)handler_Timer,
                        (void *)0);
  ///////////////////////////////////////////////////////////////////////////////
  //// ÍÀÑÒÐÎÉÊÀ  ÒÀÉÌÅÐÀ //////////////////////////////////////////////////////
  XTmrCtr_mSetLoadReg(XPAR_XPS_TIMER_0_BASEADDR,
                      0,
                      0x61a8 //(25 MHz / 25000 = 1mS
                    //0x124F8//(75 MHz : 75.000 = 1 mS
                      );
  XIntc_mEnableIntr(XPAR_INTC_0_BASEADDR,
              //    XPAR_XPS_TIMER_0_INTERRUPT_MASK |
                    XPAR_XPS_UARTLITE_0_INTERRUPT_MASK);
  XTmrCtr_mSetControlStatusReg(XPAR_XPS_TIMER_0_BASEADDR,
                               0,
                               XTC_CSR_ENABLE_TMR_MASK |
                               XTC_CSR_ENABLE_INT_MASK |
                               XTC_CSR_AUTO_RELOAD_MASK |
                               XTC_CSR_DOWN_COUNT_MASK);
  /////////////////////////////////////////////////////////////////////////////////
  ///////// ÍÀÑÒÐÎÉÊÀ  ÓÑÒÐÎÉÑÒÂ  ÂÂÎÄÀ-ÂÛÂÎÄÀ ////////////////////////////////////
  XGpio_Initialize(&photo, XPAR_XPS_GPIO_0_DEVICE_ID);
  XGpio_Initialize(&ircom, XPAR_XPS_GPIO_1_DEVICE_ID);
  XGpio_Initialize(&kt,    XPAR_XPS_GPIO_2_DEVICE_ID);

  XGpio_SetDataDirection(&photo, 1, 0xffffffff);  // ââîä
  XGpio_SetDataDirection(&photo, 2, 0x00);        // âûâîä
  XGpio_SetDataDirection(&ircom, 1, 0x00);        // âûâîä
  XGpio_SetDataDirection(&ircom, 2, 0x00);        // âûâîä
  XGpio_SetDataDirection(&kt,    1, 0x00);        // âûâîä
};
