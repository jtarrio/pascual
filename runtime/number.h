#ifndef __PASCUAL_NUMBER_H
#define __PASCUAL_NUMBER_H

#include "types.h"

int str_to_int(const PString* str, int* stop);
void int_to_str(int num, PString* str, int width);

double str_to_real(const PString* str, int* stop);
void real_to_str(double num, PString* str, int width, int precision);

#endif  // __PASCUAL_NUMBER_H
