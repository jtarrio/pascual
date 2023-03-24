/* Program: HELLOWORLD2 */
#include "pascual.h"

PString NAME;

void pascual_main() {
  Write(&OUTPUT, 1, RwpLenPtr | RwpEnd, 19, "What is your name? ");
  Read(&INPUT, 1, RwpString | RwpLn | RwpEnd, &NAME);
  Write(&OUTPUT, 1, RwpLenPtr, 7, "Hello, ", RwpStringPtr, &NAME, RwpChar | RwpLn | RwpEnd, '!');
}
