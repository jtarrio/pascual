/* Program: CMDLINE */
#include "pascual.h"

int I;

void pascual_main() {
  WRITE_s(&OUTPUT, str_make(17, "This program is: "));
  WRITE_s(&OUTPUT, PARAMSTR(0));
  WRITELN(&OUTPUT);
  do {
    int first = 1;
    int last = PARAMCOUNT();
    if (first <= last) {
      I = first;
      while (1) {
        WRITE_i(&OUTPUT, I);
        WRITE_s(&OUTPUT, str_make(2, ": "));
        WRITE_s(&OUTPUT, PARAMSTR(I));
        WRITELN(&OUTPUT);
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
}
