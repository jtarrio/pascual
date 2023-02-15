/* Program: HELLOWORLD */
#include "pascual.h"

void pascual_main() {
  WRITE_s(&OUTPUT, str_make(13, "Hello, World!"));
  WRITELN(&OUTPUT);
}
