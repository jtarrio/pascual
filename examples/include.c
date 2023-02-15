/* Program: INCLUDE */
#include "pascual.h"

void DISPLAY() {
  WRITE_s(&OUTPUT, str_make(11, "In included"));
  WRITELN(&OUTPUT);
}

void pascual_main() {
  DISPLAY();
}
