#include "microblaze_uart_functions.h"

void SendByteToComPort(unsigned char byte){
  XUartLite_SendByte (XPAR_UARTLITE_0_BASEADDR, byte);
}
