#include "types.h"

#include "error.h"

PInteger subrange(PInteger value, PInteger low, PInteger high) {
  if (value < low || value > high) rt_error(reOutOfBounds);
  return value;
}

PInteger pred(PInteger value, PInteger low, PInteger high) {
  if (value == low) rt_error(reOutOfBounds);
  return value - 1;
}

PInteger succ(PInteger value, PInteger low, PInteger high) {
  if (value == high) rt_error(reOutOfBounds);
  return value + 1;
}
