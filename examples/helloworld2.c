/* Program: HELLOWORLD2 */
#include "pascual.h"

PString NAME;

void pascual_main() {
  WRITE_s(&OUTPUT, str_make(19, "What is your name? "));
  READ_s(&INPUT, &NAME);
  READLN(&INPUT);
  WRITE_s(&OUTPUT, str_make(7, "Hello, "));
  WRITE_s(&OUTPUT, NAME);
  WRITE_c(&OUTPUT, '!');
  WRITELN(&OUTPUT);
}
