/* Program: CONSTS */
#include "pascual.h"

PInteger ANARRAY[30];
const PString BCD = str_make(3, "bcd");
const PInteger TYPEDINT = 789;
const PReal TYPEDREAL =  6.02000000000000E+023;

void pascual_main() {
  WRITE(&OUTPUT, RwpLenPtr, 3, "abc", RwpChar, ' ', RwpLenPtr, 3, "abc", RwpChar, ' ', RwpInt, 30, RwpChar, ' ', RwpInt | RwpLn | RwpEnd, 10);
  WRITE(&OUTPUT, RwpStringPtr, &BCD, RwpChar, ' ', RwpInt, TYPEDINT, RwpChar, ' ', RwpReal | RwpLn | RwpEnd, TYPEDREAL);
}
