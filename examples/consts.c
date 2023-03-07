/* Program: CONSTS */
#include "pascual.h"

PInteger ANARRAY[30];
const PString BCD = str_make(3, "bcd");
const PInteger TYPEDINT = 789;
const PReal TYPEDREAL =  6.02000000000000E+023;

void pascual_main() {
  WRITE_s(&OUTPUT, str_make(3, "abc"));
  WRITE_c(&OUTPUT, ' ');
  WRITE_s(&OUTPUT, str_make(3, "abc"));
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, 30);
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, 10);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, BCD);
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, TYPEDINT);
  WRITE_c(&OUTPUT, ' ');
  WRITE_r(&OUTPUT, TYPEDREAL);
  WRITELN(&OUTPUT);
}
