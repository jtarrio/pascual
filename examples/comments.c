/* Program: COMMENTS */
#include "pascual.h"

void pascual_main() {
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Line 1");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Line 2");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Line 3");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Line 4");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Line 5");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 6, "Line 6");
}
