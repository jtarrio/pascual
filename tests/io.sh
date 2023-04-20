#!/bin/bash

. ./testing.libsh

# Output
testout() {
  echo "program foo; begin $1 end."
}
testout 'writeln(true); writeln(false)' | outputs 'TRUE
FALSE
'
testout 'writeln(1); writeln(-1); writeln(maxint)' | outputs '1
-1
32767
'
testout "write('a')" | outputs 'a'
testout "write('a'); write('b')" | outputs 'ab'
testout "writeln('a')" | outputs 'a
'
testout "writeln('a'); writeln('b')" | outputs 'a
b
'
testout "write('foo')" | outputs 'foo'
testout "writeln('foo')" | outputs 'foo
'
testout "write('foo'); write('bar')" | outputs 'foobar'
testout "write('foo'); write('bar'); writeln" | outputs 'foobar
'
testout "writeln('foo'); writeln('bar')" | outputs 'foo
bar
'
testout "write('foo'); writeln; write('bar'); writeln" | outputs 'foo
bar
'
testout "write('foo', 'bar', 123)" | outputs 'foobar123'
testout "writeln('foo', 'bar', 123)" | outputs 'foobar123
'

testout "write(true:10)" | outputs '      TRUE'
testout "write(true:2)" | outputs 'TRUE'
testout "write(false:10)" | outputs '     FALSE'
testout "write(false:2)" | outputs 'FALSE'
testout "write(1234:10)" | outputs '      1234'
testout "write(1234:2)" | outputs '1234'
testout "write(12.34:20)" | outputs ' 1.234000000000E+001'
testout "write(12.34:10)" | outputs ' 1.23E+001'
testout "write(12.34:2)" | outputs ' 1E+001'
testout "write(12.34:20:5)" | outputs '        1.23400E+001'
testout "write(12.34:10:5)" | outputs ' 1.23E+001'
testout "write(12.34:2:5)" | outputs ' 1E+001'
testout "write(12.34:20:0)" | outputs '              1E+001'
testout "write(12.34:10:0)" | outputs '    1E+001'
testout "write(12.34:2:0)" | outputs ' 1E+001'
testout "write('c':10)" | outputs '         c'
testout "write('c':2)" | outputs ' c'
testout "write('hello':10)" | outputs '     hello'
testout "write('hello':2)" | outputs 'hello'

# Input
teststdio() {
  echo "program foo; var i : $1; begin $2 end."
}
teststdio string 'read(i); writeln(i); read(i); writeln(i)' |
inputs_and_outputs 'abcde
fghi' 'abcde

'
teststdio string 'read(i); writeln(i); readln(); read(i); writeln(i)' |
inputs_and_outputs 'abcde
fghi' 'abcde
fghi
'
teststdio string 'readln(i); writeln(i); readln(i); writeln(i)'  |
inputs_and_outputs 'abcde
fghi' 'abcde
fghi
'

# Text file output
testtextout() {
  echo "program foo; var i : string; f : text;
        begin assign(f, paramstr(1)); rewrite(f); $1 ; close(f) end."
}
testtextout 'readln(i); write(f, i); readln(i); write(f, i)' |
inputs_and_writes 'abcde
fghi' 'abcdefghi'
testtextout 'readln(i); write(f, i); writeln(f);
             readln(i); write(f, i); writeln(f)' |
inputs_and_writes 'abcde
fghi' 'abcde
fghi
'
testtextout 'readln(i); writeln(f, i); readln(i); writeln(f, i)' |
inputs_and_writes 'abcde
fghi' 'abcde
fghi
'

# Text file input
testtextin() {
  echo "program foo; var i : string; f : text;
        begin assign(f, paramstr(1)); reset(f); $1 ; close(f) end."
}
testtextin 'read(f, i); writeln(i); read(f, i); writeln(i)' |
reads_and_outputs 'abcde
fghi' 'abcde

'
testtextin 'read(f, i); writeln(i); readln(f); read(f, i); writeln(i)' |
reads_and_outputs 'abcde
fghi' 'abcde
fghi
'
testtextin 'readln(f, i); writeln(i); readln(f, i); writeln(i)' |
reads_and_outputs 'abcde
fghi' 'abcde
fghi
'

# Binary file input/output
testfile() {
  echo "program foo; type t = $1; var i : t; f : file of t;
        begin assign(f, 'testfile');
        rewrite(f); $2; close(f);
        reset(f); $3 ; close(f);
        erase(f) end."
}
testfile 'integer' 'write(f, 123456)' 'read(f, i); write(i)' | outputs '123456'
testfile 'integer' 'i := 123456; write(f, 123456); i := 0' \
                   'read(f, i); write(i)' | outputs '123456'

testfile 'record a:integer; b:boolean end' \
         'i.a := 123456; i.b := true; write(f, i)' \
         "i.a := 0; i.b := false; read(f, i); write(i.a, ' ', i.b)" |
outputs '123456 TRUE'
