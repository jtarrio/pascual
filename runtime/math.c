#include "math.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "error.h"

void RANDOMIZE(void) { srand(time(0)); }

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

int ABS_i(int num) {
  if (num < 0) return -num;
  return num;
}

double ABS_r(double num) {
  if (num < 0) return -num;
  return num;
}

int SQR_i(int num) { return num * num; }
double SQR_r(double num) { return num * num; }

double SIN(double angle) { return sin(angle); }
double COS(double angle) { return cos(angle); }
double EXP(double pow) { return exp(pow); }
double LN(double x) {
  if (x <= 0) rt_error(reMath);
  return log(x);
}
double SQRT(double x) {
  if (x < 0) rt_error(reMath);
  return sqrt(x);
}
double ARCTAN(double tan) { return atan(tan); }

double FRAC(double x) { return x - trunc(x); }

double INT(double x) { return trunc(x); }

double ROUND(double x) { return round(x); }

double TRUNC(double x) { return floor(x); }

int ODD(int x) { return x % 2; }
