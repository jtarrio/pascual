#ifndef __PASCUAL_OS_H
#define __PASCUAL_OS_H

#include "types.h"

void HALT(PInteger code);

PInteger PARAMCOUNT();
PString PARAMSTR(PInteger i);

PInteger ENVCOUNT();
PString ENVSTR(PInteger i);
PString GETENV(const PString* varname);

void InitOs(int argc, const char** argv, const char** env);

#endif  // __PASCUAL_OS_H
