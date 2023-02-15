/* Program: VARIABLES */
#include "pascual.h"

const int CBOOL = 1;
const int CINT = 1234;
const unsigned char CCHR = 'x';
const PString CSTR = str_make(15, "constant string");
const int CARRAY[3] = { 4, 5, 6 };
int VBOOL;
int VINT;
unsigned char VCHR;
PString VSTR;
int VARRAY[3];

void pascual_main() {
  VBOOL = !CBOOL;
  VINT = CINT + 4444;
  VCHR = 'j';
  VSTR = str_make(15, "variable string");
  VARRAY[0] = 7;
  VARRAY[1] = 8;
  VARRAY[2] = 9;
  WRITE_b(&OUTPUT, CBOOL);
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, CINT);
  WRITE_c(&OUTPUT, ' ');
  WRITE_c(&OUTPUT, CCHR);
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, CARRAY[0]);
  WRITE_i(&OUTPUT, CARRAY[1]);
  WRITE_i(&OUTPUT, CARRAY[2]);
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, VBOOL);
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, VINT);
  WRITE_c(&OUTPUT, ' ');
  WRITE_c(&OUTPUT, VCHR);
  WRITE_c(&OUTPUT, ' ');
  WRITE_i(&OUTPUT, VARRAY[0]);
  WRITE_i(&OUTPUT, VARRAY[1]);
  WRITE_i(&OUTPUT, VARRAY[2]);
  WRITELN(&OUTPUT);
}
