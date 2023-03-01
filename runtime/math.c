#include "math.h"

#include <stdlib.h>

double RANDOM_n(void) { return (double)rand() / RAND_MAX; }

int RANDOM_i(int num) {
  int max = RAND_MAX - RAND_MAX % num;
  if (max == 0) max = RAND_MAX;
  int r;
  do {
    r = rand();
  } while (r >= max);
  return r % num;
}
