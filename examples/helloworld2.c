/* Program: HELLOWORLD2 */
#include "pascual.h"

PString NAME;

void pascual_main() {
  WRITE(&OUTPUT, RwpLenPtr | RwpEnd, 19, "What is your name? ");
  READ(&INPUT, RwpString | RwpLn | RwpEnd, &NAME);
  WRITE(&OUTPUT, RwpLenPtr, 7, "Hello, ", RwpStringPtr, &NAME, RwpChar | RwpLn | RwpEnd, '!');
}
