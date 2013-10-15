#ifndef SHADOW_CHARACTERISTICS_TO_COORDINATES_H
#define SHADOW_CHARACTERISTICS_TO_COORDINATES_H

#include <string.h>
#include "constants.h"
#include "types.h"
#include "settings.h"


void InitIrPars(IrParams* ir_pars);
void InitPointArray(FloatPoint* point_array);
void ProcessIntegers(unsigned int ir_number,
                     unsigned int* photo_array,
                     IrParams* ir_params,
                     unsigned char* buffer,
                     unsigned char* local_buffer,
                     float* k0,
                     float* b0,
                     FloatPoint* point_array);
void IntegersToBuffer(unsigned int* photo_array, unsigned char* buffer);
void CopyDataToLocalBuffer(unsigned int ir_number,
                           IrParams* ir_params,
                           unsigned char* buffer,
                           unsigned char* local_buffer);
unsigned int FindShadowCenter(unsigned int start_position,
                              unsigned char* local_buffer);
void ProcessIntersection(unsigned int first_line_ir_number,
                         unsigned int second_line_ir_number,
                         IrParams* ir_params,
                         FloatPoint* point_array);
void ComputeIntersection(unsigned int first_line_ir_number,
                         unsigned int second_line_ir_number,
                         IrParams* ir_params,
                         FloatPoint* point_array);
void find_line_factors(/*input*/
                       short x1,
                       short y1,
                       short x2,
                       short y2,
                       /*output*/
                       float* k,
                       float* b);
void find_intersection(/*input*/
                       float k1,
                       float b1,
                       float k2,
                       float b2,
                       /*output*/
                       float* x,
                       float* y);
void FindAveragePoint(FloatPoint* point_array, FloatPoint* average_point);

#endif
