#ifndef __PASCUAL_NUMBER_H
#define __PASCUAL_NUMBER_H

#include "types.h"

PInteger str_to_integer(const PString* str, PInteger* stop);
void integer_to_str(PInteger num, PString* str, PInteger width);

PReal str_to_real(const PString* str, PInteger* stop);
void real_to_str(PReal num, PString* str, PInteger width, PInteger precision);

#endif  // __PASCUAL_NUMBER_H
