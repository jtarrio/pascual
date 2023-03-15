/* Program: CMDLINE */
#include "pascual.h"

PInteger I;

void pascual_main() {
  WRITE(&OUTPUT, RwpLenPtr, 17, "This program is: ", RwpString | RwpLn | RwpEnd, PARAMSTR(0));
  do {
    PInteger first = 1;
    PInteger last = PARAMCOUNT();
    if (first <= last) {
      I = first;
      while (1) {
        WRITE(&OUTPUT, RwpInt, I, RwpLenPtr, 2, ": ", RwpString | RwpLn | RwpEnd, PARAMSTR(I));
        if (I == last) break;
        ++I;
      }
    }
  } while(0);
}
