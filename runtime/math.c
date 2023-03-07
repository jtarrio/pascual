#include "math.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "error.h"

void RANDOMIZE(void) { srand(time(0)); }

PReal RANDOM_n(void) { return (PReal)rand() / RAND_MAX; }

PInteger RANDOM_i(PInteger num) {
  PInteger max = RAND_MAX - RAND_MAX % num;
  if (max == 0) max = RAND_MAX;
  PInteger r;
  do {
    r = rand();
  } while (r >= max);
  return r % num;
}

PInteger ABS_i(PInteger num) {
  if (num < 0) return -num;
  return num;
}

PReal ABS_r(PReal num) {
  if (num < 0) return -num;
  return num;
}

PInteger SQR_i(PInteger num) { return num * num; }
PReal SQR_r(PReal num) { return num * num; }

PReal SIN(PReal angle) { return sin(angle); }
PReal COS(PReal angle) { return cos(angle); }
PReal EXP(PReal pow) { return exp(pow); }
PReal LN(PReal x) {
  if (x <= 0) rt_error(reMath);
  return log(x);
}
PReal SQRT(PReal x) {
  if (x < 0) rt_error(reMath);
  return sqrt(x);
}
PReal ARCTAN(PReal tan) { return atan(tan); }

PReal FRAC(PReal x) { return x - trunc(x); }

PReal INT(PReal x) { return trunc(x); }

PInteger ROUND(PReal x) { return round(x); }

PInteger TRUNC(PReal x) { return floor(x); }

PBoolean ODD(PInteger x) { return x % 2; }
