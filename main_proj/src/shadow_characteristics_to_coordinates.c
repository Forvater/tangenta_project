#include "shadow_characteristics_to_coordinates.h"

void InitIrPars(IrParams* ir_pars) {
  ir_pars[0].start = 28;
  ir_pars[0].end = ir_pars[0].start + LINE_LENGTH;
  ir_pars[0].center = 0;
  ////////////////////////////////////////////////
  ir_pars[1].start = 45;
  ir_pars[1].end = ir_pars[1].start + LINE_LENGTH;
  ir_pars[1].center = 0;
  ////////////////////////////////////////////////
  ir_pars[2].start = 63;
  ir_pars[2].end = ir_pars[2].start + LINE_LENGTH;
  ir_pars[2].center = 0;
  ////////////////////////////////////////////////
  ir_pars[3].start = 82;
  ir_pars[3].end = ir_pars[3].start + LINE_LENGTH;
  ir_pars[3].center = 0;
  ////////////////////////////////////////////////
  ir_pars[4].start = 99;
  ir_pars[4].end = ir_pars[4].start + LINE_LENGTH;
  ir_pars[4].center = 0;
  ////////////////////////////////////////////////
  ir_pars[5].start = 10;
  ir_pars[5].end = ir_pars[5].start + LINE_LENGTH;
  ir_pars[5].center = 0;
  ////////////////////////////////////////////////
  ir_pars[6].start = 17;
  ir_pars[6].end = ir_pars[6].start + LINE_LENGTH;
  ir_pars[6].center = 0;
  ////////////////////////////////////////////////
  ir_pars[7].start = 35;
  ir_pars[7].end = ir_pars[7].start + LINE_LENGTH;
  ir_pars[7].center = 0;
  ////////////////////////////////////////////////
  ir_pars[8].start = 55;
  ir_pars[8].end = ir_pars[8].start + LINE_LENGTH;
  ir_pars[8].center = 0;
  ////////////////////////////////////////////////
  ir_pars[9].start = 73;
  ir_pars[9].end = ir_pars[9].start + LINE_LENGTH;
  ir_pars[9].center = 0;
  ////////////////////////////////////////////////
  ir_pars[10].start = 90;
  ir_pars[10].end = ir_pars[10].start + LINE_LENGTH;
  ir_pars[10].center = 0;
  ////////////////////////////////////////////////
  ir_pars[11].start = 0;
  ir_pars[11].end = ir_pars[11].start + LINE_LENGTH;
  ir_pars[11].center = 0;
  ////////////////////////////////////////////////
}

void InitPointArray(FloatPoint* point_array) {
  int i = 0;
  for (i = 0; i < 12; i++) {
    point_array[i].x = 0.0;
    point_array[i].y = 0.0;
  }
}

void ProcessIntegers(unsigned int ir_number,
                     unsigned int* photo_array,
                     IrParams* ir_params,
                     unsigned char* buffer,
                     unsigned char* local_buffer,
                     float* k0,
                     float* b0,
                     FloatPoint* point_array) {
  IntegersToBuffer(photo_array, buffer);
  CopyDataToLocalBuffer(ir_number, ir_params, buffer, local_buffer);
  //////////////////////////////////////////////////////////////////////
  if ((local_buffer[0] == 1) || (local_buffer[47] == 1)) {
    ir_params[ir_number].center = 255;  // No shadow
  } else {
    ir_params[ir_number].center = FindShadowCenter(ir_params[ir_number].start,
                                                   local_buffer);
  }
  //////////////////////////////////////////////////////////////////////
}

unsigned int FindShadowCenter(unsigned int start_position,
                              unsigned char* local_buffer) {
  unsigned char i;
  unsigned char count;
  unsigned int center;
  center = 0;
  count = 0;
  for (i = 0; i < 48; i++) {
    if (local_buffer[i] == 1) {
      center = center + i;
      count++;
    }
  }
  if (count > 0) {
    center = ((center*2)/count) + (start_position * 2);
    if (center >= 216) {
      center = center - 216;
    }
  } else {
    center = 255;  // No shadow
  }
  return center;
}

void IntegersToBuffer(unsigned int* photo_array, unsigned char* buffer) {
  unsigned char i = 0;
  ////////////////////////
  for (i = 0; i <= 31; i++) {
    if ((photo_array[0] >> i) & 1) {
      buffer[i] = 1;
    } else {
      buffer[i] = 0;
    }
  }
  /////////////////////////
  for (i = 0; i <= 31; i++) {
    if ((photo_array[1] >> i) & 1) {
      buffer[i+32] = 1;
    } else {
      buffer[i+32] = 0;
    }
  }
  /////////////////////////
  for (i = 0; i <= 31; i++) {
    if ((photo_array[2] >> i) & 1) {
      buffer[i+64] = 1;
    } else {
      buffer[i+64] = 0;
    }
  }
  /////////////////////////
  for (i = 0; i <= 11; i++) {
    if ((photo_array[3] >> i) & 1) {
      buffer[i+96] = 1;
    } else {
      buffer[i+96] = 0;
    }
  }
}

void CopyDataToLocalBuffer(unsigned int ir_number,
                           IrParams* ir_params,
                           unsigned char* buffer,
                           unsigned char* local_buffer) {
  switch (ir_number) {
    case 0:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
    case 1:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
    case 2:
      memcpy(local_buffer,
             &(buffer[ir_params[ir_number].start]),
             (108 - ir_params[ir_number].start));
      memcpy(&(local_buffer[108 - ir_params[ir_number].start]),
             buffer,
             (LINE_LENGTH - (108 - ir_params[ir_number].start)));
      break;
    case 3:
      memcpy(local_buffer,
             &(buffer[ir_params[ir_number].start]),
             (108 - ir_params[ir_number].start));
      memcpy(&(local_buffer[108 - ir_params[ir_number].start]),
             buffer,
             (LINE_LENGTH - (108 - ir_params[ir_number].start)));
      break;
    case 4:
      memcpy(local_buffer,
             &(buffer[ir_params[ir_number].start]),
             (108 - ir_params[ir_number].start));
      memcpy(&(local_buffer[108 - ir_params[ir_number].start]),
             buffer,
             (LINE_LENGTH - (108 - ir_params[ir_number].start)));
      break;
    case 5:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
    case 6:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
    case 7:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
    case 8:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
    case 9:
      memcpy(local_buffer,
             &(buffer[ir_params[ir_number].start]),
             (108 - ir_params[ir_number].start));
      memcpy(&(local_buffer[108 - ir_params[ir_number].start]),
             buffer,
             (LINE_LENGTH - (108 - ir_params[ir_number].start)));
      break;
    case 10:
      memcpy(local_buffer,
             &(buffer[ir_params[ir_number].start]),
             (108 - ir_params[ir_number].start));
      memcpy(&(local_buffer[108 - ir_params[ir_number].start]),
             buffer,
             (LINE_LENGTH - (108 - ir_params[ir_number].start)));
      break;
    case 11:
      memcpy(local_buffer, &(buffer[ir_params[ir_number].start]), LINE_LENGTH);
      break;
  }
}

void find_line_factors(/*input*/
                       short x1,
                       short y1,
                       short x2,
                       short y2,
                       /*output*/
                       float* k,
                       float* b) {
  *k  = ((float) y2 - (float) y1) / ((float) x2 - (float) x1);
  *b = (float) y1 - (*k * (float) x1);
}

void find_intersection(/*input*/
                       float k1,
                       float b1,
                       float k2,
                       float b2,
                       /*output*/
                       float* x,
                       float* y) {
  *x = ((b1 - b2) / (k2-k1));
  *y = (k2 * (*x)) + b2;
}

void FindAveragePoint(FloatPoint* point_array, FloatPoint* average_point) {
  int i;
  unsigned char count = 0;
  for (i = 0; i < 12; i++) {
    average_point->x = average_point->x + point_array[i].x;
    average_point->y = average_point->y + point_array[i].y;
    if ((point_array[i].x != 0.0) && (point_array[i].y != 0.0)) {
      count++;
    }
  }
  average_point->x = (average_point->x) / count;
  average_point->y = (average_point->y) / count;
}

void ProcessIntersection(unsigned int first_line_ir_number,
                         unsigned int second_line_ir_number,
                         IrParams* ir_params,
                         FloatPoint* point_array) {
  if ((ir_params[first_line_ir_number].center == 255) ||
      (ir_params[second_line_ir_number].center == 255)) {
    point_array[first_line_ir_number].x = 0.0;
    point_array[first_line_ir_number].y = 0.0;
  } else {
    ComputeIntersection(first_line_ir_number,
                        second_line_ir_number,
                        ir_params, point_array);
  }
}

void ComputeIntersection(unsigned int first_line_ir_number,
                         unsigned int second_line_ir_number,
                         IrParams* ir_params,
                         FloatPoint* point_array) {
  float k_first_line = 0.0;
  float b_first_line = 0.0;
  float k_second_line = 0.0;
  float b_second_line = 0.0;
  find_line_factors(vdx[first_line_ir_number],
                    vdy[first_line_ir_number],
                    ftx[ir_params[first_line_ir_number].center],
                    fty[ir_params[first_line_ir_number].center],
                    &k_first_line,
                    &b_first_line);
  find_line_factors(vdx[second_line_ir_number],
                    vdy[second_line_ir_number],
                    ftx[ir_params[second_line_ir_number].center],
                    fty[ir_params[second_line_ir_number].center],
                    &k_second_line,
                    &b_second_line);
  find_intersection(k_first_line,
                    b_first_line,
                    k_second_line,
                    b_second_line,
                    &(point_array[first_line_ir_number].x),
                    &(point_array[first_line_ir_number].y));
}
