#include "types.h"
#include "error.h"

int subrange(int value, int low, int high) {
  if (value < low || value > high) rt_error(reOutOfBounds);
  return value;
}

int pred(int value, int low, int high) {
  if (value == low) rt_error(reOutOfBounds);
  return value - 1;
}

int succ(int value, int low, int high) {
  if (value == high) rt_error(reOutOfBounds);
  return value + 1;
}
