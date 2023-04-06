#!/bin/bash

. ./testing.libsh

# Character literals
testchar() {
  echo "program foo; const ch : char = $1; begin write(ch) end."
}
testchar "''" | is_not_valid
testchar "' '" | outputs ' '
testchar "'a'" | outputs 'a'
testchar "'~'" | outputs '~'
testchar "''''" | outputs "'"
testchar "#1" | outputs $'\x01'
testchar "#65" | outputs 'A'
testchar "#126" | outputs '~'
testchar "#160" | outputs $'\xa0'
testchar '#$20' | outputs ' '
testchar '#$41' | outputs 'A'
testchar '#$4A' | outputs 'J'
testchar '#$4a' | outputs 'J'
testchar '#$A0' | outputs $'\xa0'
testchar '#$a0' | outputs $'\xa0'
testchar "^A" | outputs $'\x01'
testchar "^Z" | outputs $'\x1a'
testchar "^[" | outputs $'\e'
testchar "^a" | outputs $'\x01'
testchar "^z" | outputs $'\x1a'

# String literals
teststring() {
  echo "program foo; const st : string = $1; begin write(st) end."
}
teststring "''" | outputs ''
teststring "' '" | outputs ' '
teststring "'a'" | outputs 'a'
teststring "'~'" | outputs '~'
teststring "''''" | outputs "'"
teststring "#1" | outputs $'\x01'
teststring "#65" | outputs 'A'
teststring "#126" | outputs '~'
teststring "#160" | outputs $'\xa0'
teststring '#$20' | outputs ' '
teststring '#$41' | outputs 'A'
teststring '#$4A' | outputs 'J'
teststring '#$4a' | outputs 'J'
teststring '#$A0' | outputs $'\xa0'
teststring '#$a0' | outputs $'\xa0'
teststring "^A" | outputs $'\x01'
teststring "^Z" | outputs $'\x1a'
teststring "^[" | outputs $'\e'
teststring "^a" | outputs $'\x01'
teststring "^z" | outputs $'\x1a'

teststring "'abc'" | outputs 'abc'
teststring "'ab''cd'" | outputs "ab'cd"
teststring "#65#66#67'DEF'" | outputs 'ABCDEF'
teststring "'ABC'#68#69#70" | outputs 'ABCDEF'
teststring '#$41#$42#$43'"'DEF'" | outputs 'ABCDEF'
teststring "'ABC'"'#$44#$45#$46' | outputs 'ABCDEF'
teststring '#$49#$4A#$4B#$49#$4a#$4b' | outputs 'IJKIJK'
teststring "'abc'^G^g'def'" | outputs $'abc\x07\x07def'
teststring "'abc'^['def'" | outputs $'abc\edef'

# Coercing chars to strings but not the other way around
echo "program foo; var s : string; c : char; begin
      c := 'a'; s := c; write(s) end." | outputs 'a'
echo "program foo; var s : string; c : char; begin
      s := 'a'; c := s; write(c) end." | is_not_valid

# String subscripts
echo "program foo; var s : string; begin
      s := 'abcde'; write(s[5], s[4], s[3], s[2], s[1]) end." |
outputs 'edcba'

# Length
echo "program foo; var s : string; begin
      s := ''; write(Length(s), ' ');
      s := 'a'; write(Length(s), ' ');
      s := 'ab'; write(Length(s), ' ');
      s := 'abc'; write(Length(s)) end." | outputs '0 1 2 3'
echo "program foo; var c : char; begin
      c := 'a'; write(Length(c)) end." | outputs '1'

# Delete
testdelete() {
  echo "program foo; var s : string; begin
        s := '$1'; Delete(s, $2, $3); write(s) end."
}
testdelete 'abcde' 1 0 | outputs 'abcde'
testdelete 'abcde' 1 1 | outputs 'bcde'
testdelete 'abcde' 1 2 | outputs 'cde'
testdelete 'abcde' 1 3 | outputs 'de'
testdelete 'abcde' 1 4 | outputs 'e'
testdelete 'abcde' 1 5 | outputs ''
testdelete 'abcde' 1 6 | outputs ''
testdelete 'abcde' 2 0 | outputs 'abcde'
testdelete 'abcde' 2 1 | outputs 'acde'
testdelete 'abcde' 2 2 | outputs 'ade'
testdelete 'abcde' 2 5 | outputs 'a'
testdelete 'abcde' 3 1 | outputs 'abde'
testdelete 'abcde' 3 5 | outputs 'ab'
testdelete 'abcde' 5 1 | outputs 'abcd'
testdelete 'abcde' 6 1 | outputs 'abcde'
testdelete 'abcde' 0 1 | aborts
testdelete 'abcde' 256 1 | aborts

# Insert
testinsert() {
  echo "program foo; var s : string; begin
        s := '$1'; Insert('$2', s, $3); write(s) end."
}
testinsert 'abcde' 'xx' 1 | outputs 'xxabcde'
testinsert 'abcde' 'xx' 2 | outputs 'axxbcde'
testinsert 'abcde' 'xx' 3 | outputs 'abxxcde'
testinsert 'abcde' 'xx' 4 | outputs 'abcxxde'
testinsert 'abcde' 'xx' 5 | outputs 'abcdxxe'
testinsert 'abcde' 'xx' 6 | outputs 'abcdexx'
testinsert 'abcde' 'xx' 7 | outputs 'abcdexx'
testinsert 'abcde' 'xx' 8 | outputs 'abcdexx'
testinsert 'abcde' 'xx' 0 | aborts
testinsert 'abcde' 'xx' 256 | aborts

echo "program foo; var s : string; i : integer; begin
      for i := 1 to 255 do s := s + chr(65 + (i - 1) mod 26);
      Insert('abcde', s, 253); write(s[252], s[253], s[254], s[255]) end." |
outputs 'Rabc'

# Str
teststr() {
  echo "program foo; var s : string; const v : $1 = $2; begin
        Str(v, s); write(s) end."
}
teststr boolean true | outputs 'TRUE'
teststr boolean false | outputs 'FALSE'
teststr integer 0 | outputs '0'
teststr integer 1 | outputs '1'
teststr integer 42 | outputs '42'
teststr integer -42 | outputs '-42'
teststr real 0 | outputs ' 0.00000000000000E+000'
teststr real 3.1416 | outputs ' 3.14160000000000E+000'
teststr real 6.02e+23 | outputs ' 6.02000000000000E+023'
teststr real 9.1e-31 | outputs ' 9.10000000000000E-031'
teststr real -2.718 | outputs '-2.71800000000000E+000'
teststr '(One, Two, Three)' 'Two' | outputs 'TWO'

teststrw() {
  echo "program foo; var s : string; const v : $1 = $2; begin
        Str(v:$3, s); write(s) end."
}
teststrw boolean true 10 | outputs '      TRUE'
teststrw boolean true 2 | outputs 'TRUE'
teststrw integer 42 10 | outputs '        42'
teststrw integer 42 1 | outputs '42'
teststrw integer -42 10 | outputs '       -42'
teststrw integer -42 2 | outputs '-42'
teststrw real 3.1416 10 | outputs ' 3.14E+000'
teststrw real 3.1416 2 | outputs ' 3E+000'
teststrw real -2.718 10 | outputs '-2.72E+000'
teststrw real -2.718 2 | outputs '-3E+000'
teststrw '(One, Two, Three)' 'Two' 10 | outputs '       TWO'
teststrw '(One, Two, Three)' 'Two' 2 | outputs 'TWO'

# Val
testval() {
  echo "program foo; const s : string = '$1'; var v : $2; c : integer; begin
        Val(s, v, c); if c <> 0 then write('error ', c) else write(v) end."
}
testval 'true' boolean | outputs 'TRUE'
testval 'FaLsE' boolean | outputs 'FALSE'
testval '0' boolean | outputs 'error 1'
testval '0' integer | outputs '0'
testval '42' integer | outputs '42'
testval ' 42' integer | outputs 'error 1'
testval '42 ' integer | outputs 'error 3'
testval '-42' integer | outputs '-42'
testval '$20' integer | outputs '32'
testval '$ff' integer | outputs '255'
testval '$FF' integer | outputs '255'
testval '-$20' integer | outputs 'error 2'
testval '3.1416' integer | outputs 'error 2'
testval '0' real | outputs ' 0.00000000000000E+000'
testval '3.1416' real | outputs ' 3.14160000000000E+000'
testval '1e3' real | outputs ' 1.00000000000000E+003'
testval '1e+3' real | outputs ' 1.00000000000000E+003'
testval '1E3' real | outputs ' 1.00000000000000E+003'
testval '1.0e3' real | outputs ' 1.00000000000000E+003'
testval '1e-3' real | outputs ' 1.00000000000000E-003'
testval '-1.23e+4' real | outputs '-1.23000000000000E+004'
testval '1.' real | outputs 'error 2'
testval '1.0e' real | outputs 'error 4'
testval '1.0e+' real | outputs 'error 4'
testval 'one' '(One, Two, Three)' | outputs 'ONE'
testval 'Two' '(One, Two, Three)' | outputs 'TWO'
testval 'THREE' '(One, Two, Three)' | outputs 'THREE'
testval 'FOUR' '(One, Two, Three)' | outputs 'error 1'

# Copy
testcopy() {
  echo "program foo; const s : string = '$1'; var d : string; begin
        d := Copy(s, $2, $3); write(d) end."
}
testcopy 'abcde' 1 0 | outputs ''
testcopy 'abcde' 1 1 | outputs 'a'
testcopy 'abcde' 1 2 | outputs 'ab'
testcopy 'abcde' 1 3 | outputs 'abc'
testcopy 'abcde' 1 4 | outputs 'abcd'
testcopy 'abcde' 1 5 | outputs 'abcde'
testcopy 'abcde' 1 6 | outputs 'abcde'
testcopy 'abcde' 1 7 | outputs 'abcde'
testcopy 'abcde' 4 0 | outputs ''
testcopy 'abcde' 4 1 | outputs 'd'
testcopy 'abcde' 4 2 | outputs 'de'
testcopy 'abcde' 4 3 | outputs 'de'
testcopy 'abcde' 4 4 | outputs 'de'
testcopy 'abcde' 6 2 | outputs ''
testcopy 'abcde' 0 1 | aborts
testcopy 'abcde' 256 1 | aborts

# Concat
testconcat() {
  echo "program foo; begin write(Concat($1)) end."
}
testconcat '' | is_not_valid
testconcat "''" | outputs ''
testconcat "'', ''" | outputs ''
testconcat "'a', 'b'" | outputs 'ab'
testconcat "'a', ''" | outputs 'a'
testconcat "'', 'b'" | outputs 'b'
testconcat "'ab', 'cd'" | outputs 'abcd'
testconcat "'ab', ''" | outputs 'ab'
testconcat "'', 'cd'" | outputs 'cd'
testconcat "'ab', 'cd', 'ef'" | outputs 'abcdef'

# Pos
testpos() {
  echo "program foo; begin write(Pos('$1', '$2')) end."
}
testpos 'abc' 'abcde' | outputs 1
testpos 'cd' 'abcde' | outputs 3
testpos 'def' 'abcde' | outputs 0
testpos 'bcd' 'abcdbcde' | outputs 2
testpos 'abc' 'ab' | outputs 0
testpos 'abc' '' | outputs 0
testpos '' 'abc' | outputs 0
testpos 'x' 'abcd' | outputs 0

# Upcase
testupcase() {
  echo "program foo; begin write(Upcase('$1')) end."
}
testupcase 'a' | outputs 'A'
testupcase 'B' | outputs 'B'
testupcase '`' | outputs '`'
testupcase '{' | outputs '{'

# Lowercase
testlowercase() {
  echo "program foo; begin write(Lowercase('$1')) end."
}
testlowercase 'a' | outputs 'a'
testlowercase 'B' | outputs 'b'
testlowercase '`' | outputs '`'
testlowercase '{' | outputs '{'
