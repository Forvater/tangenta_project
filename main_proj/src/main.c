#include <string.h>
#include "gesture_recognition.h"
#include "init.h"
#include "settings.h"
#include "shadow_characteristics_to_coordinates.h"

void SendToComPort(float* x_coor, float* y_coor);
void SortArray(unsigned char* a);
void ReverseArray(unsigned char* a);

////////////////////////////////////////////////////////////////////////////////

int main (void) {
  //Gestures_variables_init_begin
  InitGestureVariables();
  //Gestures_variables_init_end

  unsigned char out_array[12];
  unsigned char buffer[108];
  unsigned char local_buffer[48];
  unsigned int photo_array[4];
  float x_coor = 0.0; //for gestures
  float y_coor = 0.0; //for gestures
  float old_x_coor = 0.0; //for gestures
  float old_y_coor = 0.0; //for gestures
  FloatPoint average_point;
  average_point.x = 0.0;
  average_point.y = 0.0;
  float k0 = 0.0; // line factors for the first line
  float b0 = 0.0; // line factors for the first line
  float x_curr = 0.0; // coordinates for output
  float y_curr = 0.0; // coordinates for output
  memset(photo_array, 0, sizeof(unsigned int)*4);
  memset(out_array, 0, sizeof(unsigned char)*12);
  memset(buffer, 0, sizeof(unsigned char)*108);
  memset(local_buffer, 0, sizeof(unsigned char)*48);
  IrParams ir_pars[12];
  FloatPoint point_array[12];
  InitIrPars(ir_pars);
  InitPointArray(point_array);
  init_all();
  XGpio_DiscreteWrite(&kt,1,1);
  delay_ms(1000);
  XGpio_DiscreteWrite(&kt,1,0);

  while (1) {
    average_point.x = 0.0;
    average_point.y = 0.0;
    XGpio_DiscreteWrite(&photo,2, 0xF000);
    delay_ms(40);
//    XUartLite_SendByte (XPAR_UARTLITE_0_BASEADDR, 0x55);
//    XUartLite_SendByte (XPAR_UARTLITE_0_BASEADDR, 0x54);
    InitPointArray(point_array);
    x_curr = 0.0;
    y_curr = 0.0;
    for (j = 0; j < NUMBER_OF_ACTIVE_EMITTERS; j++) {
      memset(photo_array,0,sizeof(unsigned int)*4);
      memset(buffer, 0, sizeof(unsigned char)*108);
      memset(local_buffer, 0, sizeof(unsigned char)*48);
      ir_on(j+1);
      XGpio_DiscreteWrite(&photo,2, 0xF000);
      delay_us(66);
      XGpio_DiscreteWrite(&photo,2, 0xFFFE);
      photo_array[0] = XGpio_DiscreteRead(&photo,1);
      XGpio_DiscreteWrite(&photo,2, 0xFFFD);
      photo_array[1] = XGpio_DiscreteRead(&photo,1);
      XGpio_DiscreteWrite(&photo,2, 0xFFFB);
      photo_array[2] = XGpio_DiscreteRead(&photo,1);
      XGpio_DiscreteWrite(&photo,2, 0xFFF7);
      photo_array[3] = XGpio_DiscreteRead(&photo,1);
      ir_on(0);
      XGpio_DiscreteWrite(&photo,2, 0xF000);
      delay_us(333);
      ProcessIntegers(j,
                      photo_array,
                      ir_pars,
                      buffer,
                      local_buffer,
                      &k0,
                      &b0,
                      point_array);
    }

    for (j = 0; j < NUMBER_OF_ACTIVE_EMITTERS; j++) {
      if ((j == 5) || (j == 11)) {
        ProcessIntersection(j, (j-5), ir_pars, point_array);
      } else {
        ProcessIntersection(j, (j+1), ir_pars, point_array);
      }
    }

    //Gestures_getting_prev_coordinates_begin
    GetPreviousCoordinates();
    //Gestures_getting_prev_coordinates_end

    for(j = 0; j < NUMBER_OF_ACTIVE_EMITTERS; j++) {
      if ((point_array[j].x != 0.0) && (point_array[j].y != 0.0)) {
        x_curr = point_array[j].x;
        y_curr = point_array[j].y;
        break;
      } else {
        x_curr = 0.0;
        y_curr = 0.0;
      }
    }

    ComputeGesture(x_curr, y_curr); // main gesture processing function
  }
  return (0);
}

void SendToComPort(float* x_coor, float* y_coor) {
  int i = 0;
  unsigned char* aa = NULL;
  static unsigned int a[3];
  memset(a, 0, sizeof(unsigned int)*3);
  a[0] = 500;
  a[1] = (int) (*x_coor);
  a[2] = (int) (*y_coor);
  aa = (unsigned char*) a;
  SortArray(aa);
  for(i = 0; i < 12; i++) {
    SendByteToComPort(aa[i]);
  }
}

void SortArray(unsigned char* a) {
  static int i = 0;
  for (i = 0; i < 12; i = i + 4) {
    ReverseArray(&(a[i]));
  }
}

void ReverseArray(unsigned char* a) {
  static int j = 0;
  for (j = 0; j < 2; j++) {
    Swap(&(a[j]) , &(a[3-j]));
  }
}
