#ifndef TYPES_H
#define TYPES_H

enum Directions {
  kUp = 1,
  kDown,
  kLeft,
  kRight,
  kNoChange,
  kRound,
  kRoundCounterClockWise,
  kClick,
  kLongClick
};

typedef struct {
  unsigned char start;
  unsigned char end;
  unsigned int center;
} IrParams;

typedef struct {
  float x;
  float y;
} FloatPoint;

#endif
