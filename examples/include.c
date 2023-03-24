/* Program: INCLUDE */
#include "pascual.h"

void DISPLAY() {
  Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 11, "In included");
}

void pascual_main() {
  DISPLAY();
}
