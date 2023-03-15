/* Program: STRINGS */
#include "pascual.h"

void LITERALS() {
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 25, "This is a string literal.");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 61, "I can add a single quote (') by doubling it, like I just did.");
  WRITE(&OUTPUT, RwpLenPtr, 24, "This is an empty string:", RwpLenPtr | RwpLn | RwpEnd, 0, "");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 55, "You can insert characters by ASCII code, like this: ABC");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 43, "The previous line printed 'ABC' at the end.");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 55, "The ASCII codes can also be hexadecimal, like this: ABC");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 59, "There is also a syntax for control characters, like this: \x07");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 41, "That was the Control-G character, or BEL.");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 40, "You can go from NUL (\x00) all the way to \x1f");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 62, "You can also use lowercase for your control characters, like \x07");
  WRITE(&OUTPUT, RwpLenPtr, 50, "Character literals are just like string literals, ", RwpLenPtr, 38, "but with only one character, like so: ", RwpChar | RwpLn | RwpEnd, 'X');
  WRITE(&OUTPUT, RwpLenPtr, 47, "You can also use the # and #$ syntax, like so: ", RwpChar, 'A', RwpChar | RwpLn | RwpEnd, 'B');
  WRITE(&OUTPUT, RwpLenPtr, 64, "You can also specify control characters, but right now you need ", RwpLenPtr, 61, "to precede it with an empty string so Pascual will parse it: ", RwpChar | RwpLn | RwpEnd, '\x07');
}

void OPERATIONS() {
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 29, "Concatenating strings is easy");
  WRITE(&OUTPUT, RwpLenPtr, 48, "You can also concatenate chars in the same way: ", RwpLenPtr | RwpLn | RwpEnd, 2, "ab");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 56, "Note that, in Pascual, strings are limited to 255 bytes.");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 63, "(Because of UTF-8, bytes are not the same thing as characters.)");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 45, "Use relational operators to compare strings: ");
  WRITE(&OUTPUT, RwpLenPtr, 13, "abc = bcd -> ", RwpBool | RwpLn | RwpEnd, 0);
  WRITE(&OUTPUT, RwpLenPtr, 14, "abc <> bcd -> ", RwpBool | RwpLn | RwpEnd, 1);
  WRITE(&OUTPUT, RwpLenPtr, 13, "abc > bcd -> ", RwpBool | RwpLn | RwpEnd, 0);
  WRITE(&OUTPUT, RwpLenPtr, 14, "abc <= bcd -> ", RwpBool | RwpLn | RwpEnd, 1);
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 52, "Use the Length() function to know a string's length.");
  WRITE(&OUTPUT, RwpLenPtr, 32, "For example, 'Hello' has length ", RwpInt | RwpLn | RwpEnd, ({ PString tmp1 = str_make(5, "Hello"); LENGTH(&tmp1); }));
}

void VARIABLES() {
  PString A;
  PString B;
  A = str_make(34, "You can store strings in variables");
  B = str_make(21, "and operate with them");
  WRITE(&OUTPUT, RwpString | RwpLn | RwpEnd, CONCAT(CpStringPtr, &A, CpChar, ' ', CpEnd | CpStringPtr, &B));
  WRITE(&OUTPUT, RwpLenPtr, 48, "Use subscript notation to extract single bytes: ", RwpChar, A.chr[1], RwpChar, A.chr[2], RwpChar | RwpLn | RwpEnd, A.chr[3]);
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 43, "As usual in Pascal, subscripts are 1-based.");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 62, "It is not an error to access bytes past the end of the string.");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 48, "(As long as you don't try to go over 255 bytes.)");
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 55, "You will most likely get garbage in the output, though.");
}

void PROCEDURESANDFUNCTIONS() {
  PString A;
  PString B;
  A = str_make(59, "You can use the function Copy() to extract part of a string");
  WRITE(&OUTPUT, RwpStringPtr, &A, RwpLenPtr, 2, ": ", RwpString | RwpLn | RwpEnd, COPY(&A, 13, 30));
  A = str_make(5, "abcde");
  B = str_make(2, "XX");
  INSERT(&B, &A, 3);
  WRITE(&OUTPUT, RwpLenPtr, 55, "Use Insert() to insert a string inside another string: ", RwpStringPtr | RwpLn | RwpEnd, &A);
  A = str_make(10, "1234567890");
  DELETE(&A, 3, 4);
  WRITE(&OUTPUT, RwpLenPtr, 54, "Use Delete() to delete some characters from a string: ", RwpStringPtr | RwpLn | RwpEnd, &A);
  A = str_make(8, "abcdbcde");
  WRITE(&OUTPUT, RwpLenPtr, 50, "Use Pos() to find a string inside another string: ", RwpInt | RwpLn | RwpEnd, ({ PString tmp1 = str_make(3, "cdb"); POS(&tmp1, &A); }));
}

void CONVERSIONS() {
  PString A;
  PInteger B;
  PBoolean C;
  enum enum1 { ONE, TWO, THREE } D;
  PInteger X;
  const char* enumvalues1[] = { "ONE", "TWO", "THREE" };
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 53, "You can use Str() to convert several types to string.");
  STR_i(42, 0, &A);
  WRITE(&OUTPUT, RwpStringPtr | RwpLn | RwpEnd, &A);
  STR_e(THREE, enumvalues1, 0, &A);
  WRITE(&OUTPUT, RwpStringPtr | RwpLn | RwpEnd, &A);
  STR_b(1, 0, &A);
  WRITE(&OUTPUT, RwpStringPtr | RwpLn | RwpEnd, &A);
  WRITE(&OUTPUT, RwpLenPtr | RwpLn | RwpEnd, 62, "Use Val() to convert a string into a number, boolean, or enum.");
  VAL_i(&str_make(2, "42"), &B, &X);
  VAL_b(&str_make(5, "false"), &C, &X);
  VAL_e(&str_make(3, "Two"), &D, 3, enumvalues1, &X);
  WRITE(&OUTPUT, RwpInt, B, RwpChar, ' ', RwpBool, C, RwpChar, ' ', RwpEnum | RwpLn | RwpEnd, D, enumvalues1);
}

void pascual_main() {
  LITERALS();
  OPERATIONS();
  VARIABLES();
  PROCEDURESANDFUNCTIONS();
  CONVERSIONS();
}
