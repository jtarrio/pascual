#!/bin/bash

. ./lib.sh

# Output
testout() {
  echo "program foo; begin $1 end."
}
testout 'writeln(true); writeln(false)' | outputs 'TRUE
FALSE
'
testout 'writeln(1); writeln(-1); writeln(maxint)' | outputs '1
-1
2147483647
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
