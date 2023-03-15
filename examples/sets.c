/* Program: SETS */
#include "pascual.h"

void SETSYNTAX() {
  PSet16 S;
  PInteger I;
  S = (PSet16) { 0, 0 };
  S = (PSet16) { 84, 5 };
  S = (PSet16) { 31, 0 };
  I = 2;
  S = ({ PSet16 dst = (PSet16) { 0, 0 }; set_set(subrange(I, 0, 10), subrange(I + 2, 0, 10), 0, dst.bits); dst; });
}

void BELONGSTOASET() {
  PChar CH;
  CH = '9';
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, 'a' <= CH && CH <= 'z');
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, '0' <= CH && CH <= '9' || 'A' <= CH && CH <= 'F' || 'a' <= CH && CH <= 'f');
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, '0' <= CH && CH <= '9' || 'A' <= CH && CH <= 'F' || 'a' <= CH && CH <= 'f');
}

void OPERATIONS() {
  PSet256 S1;
  PSet256 S2;
  PSet256 S3;
  S1 = (PSet256) { 0, 0, 0, 0, 0, 0, 255, 3, 254, 255, 255, 7, 254, 255, 255, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  S2 = (PSet256) { 0, 0, 0, 0, 0, 0, 255, 3, 126, 0, 0, 0, 126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  S3 = ({ PSet256 dst; set_union(S1.bits, S2.bits, dst.bits, 32); dst; });
  WRITE(&OUTPUT, RwpBool, set_in('a', 0, S3.bits), RwpChar, ' ', RwpBool | RwpLn | RwpEnd, set_in('z', 0, S3.bits));
  S3 = ({ PSet256 dst; set_difference(S1.bits, S2.bits, dst.bits, 32); dst; });
  WRITE(&OUTPUT, RwpBool, set_in('a', 0, S3.bits), RwpChar, ' ', RwpBool | RwpLn | RwpEnd, set_in('z', 0, S3.bits));
  S3 = ({ PSet256 dst; set_intersection(S1.bits, S2.bits, dst.bits, 32); dst; });
  WRITE(&OUTPUT, RwpBool, set_in('a', 0, S3.bits), RwpChar, ' ', RwpBool | RwpLn | RwpEnd, set_in('z', 0, S3.bits));
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, set_equals(S1.bits, S2.bits, 32));
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, !set_equals(S1.bits, S2.bits, 32));
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, set_issuperset(S1.bits, S2.bits, 32));
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, set_issuperset(S2.bits, S1.bits, 32));
}

void MATERIALIZEDSETS() {
  PSet16 SR;
  PSet256 SC;
  PSet8 SB;
  PInteger I;
  SR = (PSet16) { 226, 0 };
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, set_in(3, 0, SR.bits));
  SC = (PSet256) { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, set_in('f', 0, SC.bits));
  SB = (PSet8) { 0 };
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, set_in(1, 0, SB.bits));
  I = 53;
  WRITE(&OUTPUT, RwpBool | RwpLn | RwpEnd, 1 <= I && I <= 2000000);
}

void pascual_main() {
  SETSYNTAX();
  BELONGSTOASET();
  OPERATIONS();
  MATERIALIZEDSETS();
}
