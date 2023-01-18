#ifndef __PASCUAL_OS_H
#define __PASCUAL_OS_H

#include "types.h"

void HALT(int code);

int PARAMCOUNT();
PString PARAMSTR(int i);

void InitOs(int argc, const char** argv);

#endif  // __PASCUAL_OS_H
