/* Program: STRINGS */
#include "pascual.h"

void LITERALS() {
  WRITE_s(&OUTPUT, str_make(25, "This is a string literal."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(61, "I can add a single quote (') by doubling it, like I just did."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(24, "This is an empty string:"));
  WRITE_s(&OUTPUT, str_make(0, ""));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(55, "You can insert characters by ASCII code, like this: ABC"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(43, "The previous line printed 'ABC' at the end."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(55, "The ASCII codes can also be hexadecimal, like this: ABC"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(59, "There is also a syntax for control characters, like this: \x07"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(41, "That was the Control-G character, or BEL."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(40, "You can go from NUL (\x00) all the way to \x1f"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(62, "You can also use lowercase for your control characters, like \x07"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(50, "Character literals are just like string literals, "));
  WRITE_s(&OUTPUT, str_make(38, "but with only one character, like so: "));
  WRITE_c(&OUTPUT, 'X');
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(47, "You can also use the # and #$ syntax, like so: "));
  WRITE_c(&OUTPUT, 'A');
  WRITE_c(&OUTPUT, 'B');
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(64, "You can also specify control characters, but right now you need "));
  WRITE_s(&OUTPUT, str_make(61, "to precede it with an empty string so Pascual will parse it: "));
  WRITE_c(&OUTPUT, '\x07');
  WRITELN(&OUTPUT);
}

void OPERATIONS() {
  WRITE_s(&OUTPUT, str_make(29, "Concatenating strings is easy"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(48, "You can also concatenate chars in the same way: "));
  WRITE_s(&OUTPUT, str_make(2, "ab"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(56, "Note that, in Pascual, strings are limited to 255 bytes."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(63, "(Because of UTF-8, bytes are not the same thing as characters.)"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(45, "Use relational operators to compare strings: "));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(13, "abc = bcd -> "));
  WRITE_b(&OUTPUT, 0);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(14, "abc <> bcd -> "));
  WRITE_b(&OUTPUT, 1);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(13, "abc > bcd -> "));
  WRITE_b(&OUTPUT, 0);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(14, "abc <= bcd -> "));
  WRITE_b(&OUTPUT, 1);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(52, "Use the Length() function to know a string's length."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(32, "For example, 'Hello' has length "));
  WRITE_i(&OUTPUT, ({ PString tmp1 = str_make(5, "Hello"); LENGTH(&tmp1); }));
  WRITELN(&OUTPUT);
}

void VARIABLES() {
  PString A;
  PString B;
  A = str_make(34, "You can store strings in variables");
  B = str_make(21, "and operate with them");
  WRITE_s(&OUTPUT, cat_ss(cat_sc(A, ' '), B));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(48, "Use subscript notation to extract single bytes: "));
  WRITE_c(&OUTPUT, A.chr[1]);
  WRITE_c(&OUTPUT, A.chr[2]);
  WRITE_c(&OUTPUT, A.chr[3]);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(43, "As usual in Pascal, subscripts are 1-based."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(62, "It is not an error to access bytes past the end of the string."));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(48, "(As long as you don't try to go over 255 bytes.)"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(55, "You will most likely get garbage in the output, though."));
  WRITELN(&OUTPUT);
}

void PROCEDURESANDFUNCTIONS() {
  PString A;
  PString B;
  A = str_make(59, "You can use the function Copy() to extract part of a string");
  WRITE_s(&OUTPUT, A);
  WRITE_s(&OUTPUT, str_make(2, ": "));
  WRITE_s(&OUTPUT, COPY(&A, 13, 30));
  WRITELN(&OUTPUT);
  A = str_make(5, "abcde");
  B = str_make(2, "XX");
  INSERT(&B, &A, 3);
  WRITE_s(&OUTPUT, str_make(55, "Use Insert() to insert a string inside another string: "));
  WRITE_s(&OUTPUT, A);
  WRITELN(&OUTPUT);
  A = str_make(10, "1234567890");
  DELETE(&A, 3, 4);
  WRITE_s(&OUTPUT, str_make(54, "Use Delete() to delete some characters from a string: "));
  WRITE_s(&OUTPUT, A);
  WRITELN(&OUTPUT);
  A = str_make(8, "abcdbcde");
  WRITE_s(&OUTPUT, str_make(50, "Use Pos() to find a string inside another string: "));
  WRITE_i(&OUTPUT, ({ PString tmp1 = str_make(3, "cdb"); POS(&tmp1, &A); }));
  WRITELN(&OUTPUT);
}

void CONVERSIONS() {
  PString A;
  int B;
  int C;
  enum enum1 { ONE, TWO, THREE } D;
  int X;
  const char* enumvalues1[] = { "ONE", "TWO", "THREE" };
  WRITE_s(&OUTPUT, str_make(53, "You can use Str() to convert several types to string."));
  WRITELN(&OUTPUT);
  STR_i(42, &A);
  WRITE_s(&OUTPUT, A);
  WRITELN(&OUTPUT);
  STR_e(THREE, enumvalues1, &A);
  WRITE_s(&OUTPUT, A);
  WRITELN(&OUTPUT);
  STR_b(1, &A);
  WRITE_s(&OUTPUT, A);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(62, "Use Val() to convert a string into a number, boolean, or enum."));
  WRITELN(&OUTPUT);
  VAL_i(&str_make(2, "42"), &B, &X);
  VAL_b(&str_make(5, "false"), &C, &X);
  VAL_e(&str_make(3, "Two"), &D, 3, enumvalues1, &X);
  WRITE_i(&OUTPUT, B);
  WRITE_c(&OUTPUT, ' ');
  WRITE_b(&OUTPUT, C);
  WRITE_c(&OUTPUT, ' ');
  WRITE_e(&OUTPUT, D, enumvalues1);
  WRITELN(&OUTPUT);
}

void pascual_main() {
  LITERALS();
  OPERATIONS();
  VARIABLES();
  PROCEDURESANDFUNCTIONS();
  CONVERSIONS();
}
