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
