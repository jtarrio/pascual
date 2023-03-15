/* Program: HELLOWORLD */
#include "pascual.h"

void pascual_main() {
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 13, "Hello, World!");
}
