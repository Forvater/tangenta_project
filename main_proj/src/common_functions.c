#include "common_functions.h"

unsigned int Abs(int a) {
  if (a >= 0) {
    return a;
  } else {
    return (~a + 1);
  }
}

void Swap(unsigned char* a, unsigned char* b) {
  *a = *a + *b;
  *b = *a - *b;
  *a = *a - *b;
}
