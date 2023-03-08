#include "os.h"

#include <stdlib.h>

#include "string.h"

void HALT(PInteger code) { exit(code); }

static PInteger param_count;
static const char** param_str;

PInteger PARAMCOUNT() { return param_count; }

PString PARAMSTR(PInteger i) {
  if ((i < 0) || (i > param_count)) return (PString){.len = 0};
  return str_of_pchar(param_str[i], 0);
}

void InitOs(int argc, const char** argv) {
  param_count = argc - 1;
  param_str = argv;
}
