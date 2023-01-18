#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static const char* RtErrorMsgs[] = {"Index out of bounds"};

void rt_error(RtError err) {
  fprintf(stderr, "Run-time error: %s\n", RtErrorMsgs[err]);
  exit(1);
}
