#include "os.h"

#include <stdlib.h>

#include "string.h"

void HALT(int code) { exit(code); }

static int param_count;
static const char** param_str;

int PARAMCOUNT() { return param_count; }

PString PARAMSTR(int i) {
  if ((i < 0) || (i > param_count)) return (PString){.len = 0};
  return str_of_pchar(param_str[i]);
}

void InitOs(int argc, const char** argv) {
  param_count = argc - 1;
  param_str = argv;
}
