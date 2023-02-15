/* Program: CONSTS */
#include "pascual.h"

void pascual_main() {
  if (1) {
    WRITE_s(&OUTPUT, str_make(3, "abc"));
    WRITE_c(&OUTPUT, ' ');
    WRITE_s(&OUTPUT, str_make(3, "def"));
    WRITE_c(&OUTPUT, ' ');
    WRITE_b(&OUTPUT, 1);
    WRITE_c(&OUTPUT, ' ');
    WRITE_i(&OUTPUT, 123);
    WRITELN(&OUTPUT);
  }
}
