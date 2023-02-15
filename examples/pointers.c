/* Program: POINTERS */
#include "pascual.h"

typedef PString *STRP;
typedef struct record1 {
  PString FOO;
  int BAR;
} RECTYPE;
typedef RECTYPE *RECP;

PString FOO;
PString *STR;
RECTYPE *REC;

void pascual_main() {
  FOO = str_make(3, "foo");
  STR = malloc(sizeof(PString));
  *STR = FOO;
  WRITE_s(&OUTPUT, *STR);
  WRITELN(&OUTPUT);
  *STR = str_make(3, "bar");
  FOO = *STR;
  WRITE_s(&OUTPUT, FOO);
  WRITELN(&OUTPUT);
  free(STR);
  REC = malloc(sizeof(RECTYPE));
  REC->FOO = str_make(3, "foo");
  REC->BAR = 1;
  WRITE_s(&OUTPUT, REC->FOO);
  WRITELN(&OUTPUT);
  WRITE_i(&OUTPUT, REC->BAR);
  WRITELN(&OUTPUT);
  free(REC);
}
