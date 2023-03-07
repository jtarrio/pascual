#ifndef __PASCUAL_MATH_H
#define __PASCUAL_MATH_H

#include "types.h"

void RANDOMIZE(void);
PReal RANDOM_n(void);
PInteger RANDOM_i(PInteger num);

PInteger ABS_i(PInteger num);
PReal ABS_r(PReal num);
PInteger SQR_i(PInteger num);
PReal SQR_r(PReal num);
PReal SIN(PReal angle);
PReal COS(PReal angle);
PReal EXP(PReal pow);
PReal LN(PReal x);
PReal SQRT(PReal x);
PReal ARCTAN(PReal tan);

PReal FRAC(PReal x);
PReal INT(PReal x);
PInteger ROUND(PReal x);
PInteger TRUNC(PReal x);

PBoolean ODD(PInteger x);

#endif  // __PASCUAL_MATH_H
