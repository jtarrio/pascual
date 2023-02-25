/* Program: SETS */
#include "pascual.h"

void SETSYNTAX() {
  PSet16 S;
  int I;
  S = (PSet16) { 0, 0 };
  S = (PSet16) { 84, 5 };
  S = (PSet16) { 31, 0 };
  I = 2;
  S = ({ PSet16 dst = (PSet16) { 0, 0 }; set_set(subrange(I, 0, 10), subrange(I + 2, 0, 10), 0, dst.bits); dst; });
}

void BELONGSTOASET() {
  unsigned char CH;
  CH = '9';
  WRITE_b(&OUTPUT, 'a' <= CH && CH <= 'z');
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, '0' <= CH && CH <= '9' || 'A' <= CH && CH <= 'F' || 'a' <= CH && CH <= 'f');
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, '0' <= CH && CH <= '9' || 'A' <= CH && CH <= 'F' || 'a' <= CH && CH <= 'f');
  WRITELN(&OUTPUT);
}

void OPERATIONS() {
  PSet256 S1;
  PSet256 S2;
  PSet256 S3;
  S1 = (PSet256) { 0, 0, 0, 0, 0, 0, 255, 3, 254, 255, 255, 7, 254, 255, 255, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  S2 = (PSet256) { 0, 0, 0, 0, 0, 0, 255, 3, 126, 0, 0, 0, 126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  S3 = ({ PSet256 dst; set_union(S1.bits, S2.bits, dst.bits, 32); dst; });
  WRITE_b(&OUTPUT, set_in('a', 0, S3.bits));
  WRITE_c(&OUTPUT, ' ');
  WRITE_b(&OUTPUT, set_in('z', 0, S3.bits));
  WRITELN(&OUTPUT);
  S3 = ({ PSet256 dst; set_difference(S1.bits, S2.bits, dst.bits, 32); dst; });
  WRITE_b(&OUTPUT, set_in('a', 0, S3.bits));
  WRITE_c(&OUTPUT, ' ');
  WRITE_b(&OUTPUT, set_in('z', 0, S3.bits));
  WRITELN(&OUTPUT);
  S3 = ({ PSet256 dst; set_intersection(S1.bits, S2.bits, dst.bits, 32); dst; });
  WRITE_b(&OUTPUT, set_in('a', 0, S3.bits));
  WRITE_c(&OUTPUT, ' ');
  WRITE_b(&OUTPUT, set_in('z', 0, S3.bits));
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, set_equals(S1.bits, S2.bits, 32));
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, !set_equals(S1.bits, S2.bits, 32));
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, set_issubset(S1.bits, S2.bits, 32));
  WRITELN(&OUTPUT);
  WRITE_b(&OUTPUT, set_issubset(S2.bits, S1.bits, 32));
  WRITELN(&OUTPUT);
}

void MATERIALIZEDSETS() {
  PSet16 SR;
  PSet256 SC;
  PSet8 SB;
  int I;
  SR = (PSet16) { 226, 0 };
  WRITE_b(&OUTPUT, set_in(3, 0, SR.bits));
  WRITELN(&OUTPUT);
  SC = (PSet256) { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  WRITE_b(&OUTPUT, set_in('f', 0, SC.bits));
  WRITELN(&OUTPUT);
  SB = (PSet8) { 0 };
  WRITE_b(&OUTPUT, set_in(1, 0, SB.bits));
  WRITELN(&OUTPUT);
  I = 53;
  WRITE_b(&OUTPUT, 1 <= I && I <= 2000000);
  WRITELN(&OUTPUT);
}

void pascual_main() {
  SETSYNTAX();
  BELONGSTOASET();
  OPERATIONS();
  MATERIALIZEDSETS();
}
