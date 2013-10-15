#include "gesture_recognition.h"

//Gesture_variables_begin
int x_coor_out;
int prev_x_coor_out;
int start_x_coor;
int end_x_coor;
int y_coor_out;
int prev_y_coor_out;
int start_y_coor;
int end_y_coor;
unsigned int recv_coor_counter;
unsigned char gesture_result;
int diff_x;
int diff_y;
unsigned char curr_direction_x;
unsigned char curr_direction_y;
unsigned char prev_direction_x;
unsigned char prev_direction_y;
unsigned char round_flag1;
unsigned char round_flag2;
unsigned char round_flag3;
unsigned char round_flag4;
unsigned char round_flag5;
unsigned char round_flag6;

unsigned char do_process_basic_gestures;
unsigned char long_click_done;

unsigned char tangenta_pushed;

const unsigned char error_x = 5;
const unsigned char error_y = 5;  // was 20
//Gesture_variables_end

void InitGestureVariables() {
  x_coor_out = 0;
  prev_x_coor_out = 0;
  start_x_coor = 0;
  end_x_coor = 0;
  y_coor_out = 0;
  prev_y_coor_out = 0;
  start_y_coor = 0;
  end_y_coor = 0;
  recv_coor_counter = 0;
  gesture_result = 0;
  diff_x = 0;
  diff_y = 0;
  curr_direction_x = kNoChange;
  curr_direction_y = kNoChange;
  prev_direction_x = kNoChange;
  prev_direction_y = kNoChange;
  round_flag1 = 0;
  round_flag2 = 0;
  round_flag3 = 0;
  round_flag4 = 0;
  round_flag5 = 0;
  round_flag6 = 0;
  do_process_basic_gestures = 1;
  long_click_done = 0;
  tangenta_pushed = 0;
}

void ComputeGesture(float x_coor, float y_coor) {
  x_coor_out = (int) x_coor;
  y_coor_out = (int) y_coor;
  x_coor_out = x_coor_out / 100;
  y_coor_out = y_coor_out / 100;
  diff_x = x_coor_out - prev_x_coor_out;
  diff_y = y_coor_out - prev_y_coor_out;
  if ((x_coor_out != 0) || (y_coor_out != 0)) {  // Non zero coordinates came
    recv_coor_counter++;
    if ((prev_x_coor_out == 0) && (prev_y_coor_out == 0)) {
      DoIfEnteredZone();
    }
    RealtimeGestures();
  } else if ((x_coor_out == 0)  && (y_coor_out == 0)) {
    if ((prev_x_coor_out != 0) || (prev_y_coor_out != 0)) {
      DoIfLeftZone();
      recv_coor_counter = 0;
    }
  }
  if (gesture_result != 0) {
    SendByteToComPort(gesture_result);
/*************************************/
    switch (gesture_result) {
      case kRight:
        XGpio_DiscreteWrite(&ircom,2,1);
        XGpio_DiscreteWrite(&kt,1,1);
        delay_ms(500);
        XGpio_DiscreteWrite(&ircom,2,0);
        XGpio_DiscreteWrite(&kt,1,0);
        break;
      case kRound:
        XGpio_DiscreteWrite(&ircom,2,2);
        XGpio_DiscreteWrite(&kt,1,4);
        delay_ms(500);
        XGpio_DiscreteWrite(&ircom,2,0);
        XGpio_DiscreteWrite(&kt,1,0);
        break;
      case kRoundCounterClockWise:
        XGpio_DiscreteWrite(&ircom,2,4);
        XGpio_DiscreteWrite(&kt,1,2);
        delay_ms(500);
        XGpio_DiscreteWrite(&ircom,2,0);
        XGpio_DiscreteWrite(&kt,1,0);
        break;
      case kUp:
        XGpio_DiscreteWrite(&ircom,2,8);
        XGpio_DiscreteWrite(&kt,1,6);
        delay_ms(500);
        XGpio_DiscreteWrite(&ircom,2,0);
        XGpio_DiscreteWrite(&kt,1,0);
        break;
      case kLongClick:
        XGpio_DiscreteWrite(&ircom,2,16);
        XGpio_DiscreteWrite(&kt,1,5);
        tangenta_pushed = 1;
        break;
    }
/***************************************/
    gesture_result = 0;
  }
}

void DoIfEnteredZone() {
  long_click_done = 0;
  gesture_result = 0;
  round_flag1 = 0;
  round_flag2 = 0;
  round_flag3 = 0;
  round_flag4 = 0;
  round_flag5 = 0;
  round_flag6 = 0;
  prev_direction_x = kNoChange;
  prev_direction_y = kNoChange;
  curr_direction_x = kNoChange;
  curr_direction_y = kNoChange;
  start_x_coor = x_coor_out;
  start_y_coor = y_coor_out;
}

void DoIfLeftZone() {
  end_x_coor = prev_x_coor_out;
  end_y_coor = prev_y_coor_out;
   if (gesture_result != kRound) {
     if (gesture_result != kRoundCounterClockWise) {
       if (gesture_result != kLongClick) {
         if (do_process_basic_gestures == 1) {
           ComputeBasicGestures();
         }
       }
     }
   }
  do_process_basic_gestures = 1;
  if (tangenta_pushed == 1) { //Turn off diode after long click
    XGpio_DiscreteWrite(&ircom,2,0); //for tangenta
    XGpio_DiscreteWrite(&kt,1,0); //for tangenta
    tangenta_pushed = 0;
  }
}

void ComputeBasicGestures() {
  int delta_x = end_x_coor - start_x_coor;
  int delta_y = end_y_coor - start_y_coor;

  if (Abs(delta_x) > Abs(delta_y)) {
    if (delta_x > 0 ) {
      gesture_result = kRight;
    } else if (delta_x < 0) {
      gesture_result = kLeft;
    } else {
      gesture_result = 0;
    }
  } else if (Abs(delta_y) > Abs(delta_x)) {
    if (delta_y > 0 ) {
      gesture_result = kUp;
    } else if (delta_y < 0) {
      gesture_result = kDown;
    } else {
      gesture_result = 0;
    }
  }

  if ((delta_x < error_x) && (delta_x > -error_x)) {
    if ((delta_y < error_y) && (delta_y > -error_y)) {
      gesture_result = kClick;
    }
  }
}

void RealtimeGestures() {
  prev_direction_x = curr_direction_x;
  prev_direction_y = curr_direction_y;
  int delta_x = x_coor_out - start_x_coor;
  int delta_y = y_coor_out - start_y_coor;

  if (recv_coor_counter == 15) {
    if ((delta_x < 8/*error_x*/) && (delta_x > -8/*error_x*/)) {
      if ((delta_y < 8/*error_y*/) && (delta_y > -8/*error_y*/)) {
      long_click_done = 1;
      gesture_result = kLongClick;
      do_process_basic_gestures = 0;
      }
    }
  }

  if (long_click_done != 1) {
    ComputeCurrentDirection();
    // Clockwise
    if ((prev_direction_x == kRight) &&
        (curr_direction_x == kRight) &&
        (prev_direction_y == kUp) &&
        (curr_direction_y == kDown)) {
      round_flag1 = 1;
    }
    if (round_flag1 == 1) {
      if ((prev_direction_x == kRight) &&
          (curr_direction_x == kLeft) &&
          (prev_direction_y == kDown) &&
          (curr_direction_y == kDown)) {
        round_flag2 = 1;
      }
    }
    if (round_flag2 == 1){
      if ((prev_direction_x == kLeft) &&
          (curr_direction_x == kLeft) &&
          (prev_direction_y == kDown) &&
          (curr_direction_y == kUp)) {
        round_flag3 = 1;
      }
    }
    if (round_flag3 == 1) {
      if ((prev_direction_x == kLeft) &&
          (curr_direction_x == kRight) &&
          (prev_direction_y == kUp) &&
          (curr_direction_y == kUp)) {
        gesture_result = kRound;
        round_flag1 = 0;
        round_flag2 = 0;
        round_flag3 = 0;
        do_process_basic_gestures = 0;
      }
    }
    // CounterClockwise
    if ((prev_direction_x == kLeft) &&
        (curr_direction_x == kLeft) &&
        (prev_direction_y == kUp) &&
        (curr_direction_y == kDown)) {
      round_flag4 = 1;
    }
    if (round_flag4 == 1) {
      if ((prev_direction_x == kLeft) &&
          (curr_direction_x == kRight) &&
          (prev_direction_y == kDown) &&
          (curr_direction_y == kDown)) {
        round_flag5 = 1;
      }
    }
    if (round_flag5 == 1){
      if ((prev_direction_x == kRight) &&
          (curr_direction_x == kRight) &&
          (prev_direction_y == kDown) &&
          (curr_direction_y == kUp)) {
        round_flag6 = 1;
      }
    }
    if (round_flag6 == 1) {
      if ((prev_direction_x == kRight) &&
          (curr_direction_x == kLeft) &&
          (prev_direction_y == kUp) &&
          (curr_direction_y == kUp)) {
        gesture_result = kRoundCounterClockWise;
        round_flag4 = 0;
        round_flag5 = 0;
        round_flag6 = 0;
        do_process_basic_gestures = 0;
      }
    }
  }
}

void ComputeCurrentDirection() {
  if (diff_x > 0) {
    curr_direction_x = kRight;
  } else if (diff_x < 0) {
    curr_direction_x = kLeft;
  } else { //diff_x == 0
//    curr_direction_x = kNoChange;
  }

  if (diff_y > 0) {
    curr_direction_y = kUp;
  } else if (diff_y < 0) {
    curr_direction_y = kDown;
  } else { //diff_y == 0
//    curr_direction_y = kNoChange;
  }
}

void GetPreviousCoordinates() {
  prev_x_coor_out = x_coor_out;
  prev_y_coor_out = y_coor_out;
}
