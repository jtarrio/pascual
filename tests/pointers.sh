#!/bin/bash

. ./lib.sh

echo "program foo; var P : ^integer;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3
echo "program foo; type TP = ^integer; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3
echo "program foo; type T = integer; TP = ^T; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3

# Can declare a pointer of a type defined in the same block.
echo "program foo; type TP = ^T; T = integer; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3
