#ifndef __PASCUAL_OS_H
#define __PASCUAL_OS_H

#include "types.h"

void HALT(PInteger code);

PInteger PARAMCOUNT();
PString PARAMSTR(PInteger i);

void InitOs(int argc, const char** argv);

#endif  // __PASCUAL_OS_H
