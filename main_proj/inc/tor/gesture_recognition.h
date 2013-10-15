#ifndef GESTURE_RECOGNITION_H
#define GESTURE_RECOGNITION_H

#include "common_functions.h"
#include "global_variables.h"
#include "microblaze_uart_functions.h"
#include "types.h"

//Gesture_variables_begin
extern int x_coor_out;
extern int prev_x_coor_out;
extern int start_x_coor;
extern int end_x_coor;
extern int y_coor_out;
extern int prev_y_coor_out;
extern int start_y_coor;
extern int end_y_coor;
extern unsigned int recv_coor_counter;
extern unsigned char gesture_result;
extern int diff_x;
extern int diff_y;
extern unsigned char curr_direction_x;
extern unsigned char curr_direction_y;
extern unsigned char prev_direction_x;
extern unsigned char prev_direction_y;
extern unsigned char round_flag1;
extern unsigned char round_flag2;
extern unsigned char round_flag3;
extern unsigned char round_flag4;
extern unsigned char round_flag5;
extern unsigned char round_flag6;

extern unsigned char do_process_basic_gestures;
extern unsigned char long_click_done;

extern unsigned char tangenta_pushed;

extern const unsigned char error_x;
extern const unsigned char error_y;
//Gesture_variables_end

//Gestures_functions_begin
void InitGestureVariables();
void ComputeGesture(float x_coor, float y_coor);
void DoIfEnteredZone();
void DoIfLeftZone();
void ComputeBasicGestures();
void ComputeCurrentDirection();
void RealtimeGestures();
void GetPreviousCoordinates();
//Gestures_functions_end

#endif
