/* Program: CMDLINE */
#include "pascual.h"

PInteger I;

void pascual_main() {
  Write(&OUTPUT, 1, RwpLenPtr, 17, "This program is: ", RwpString | RwpLn | RwpEnd, PARAMSTR(0));
  for (PInteger first = 1, last = PARAMCOUNT(); first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (I = first; !done; done = I == last ? 1 : (++I, 0)) Write(&OUTPUT, 1, RwpInt, I, RwpLenPtr, 2, ": ", RwpString | RwpLn | RwpEnd, PARAMSTR(I));
    break;
  }
}
