/* Program: INCLUDE */
#include "pascual.h"

void DISPLAY() {
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 11, "In included");
}

void pascual_main() {
  DISPLAY();
}
