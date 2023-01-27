#!/bin/bash

. ./testing.libsh

testrecord() {
  echo "program foo; var R : record $1 end;
        begin $2 end."
}

testrecord 'A : integer' \
           "R.A := 1; write(R.A)" | outputs "1"
testrecord 'A : integer' \
           "R.A := 1; write(R.A)" | outputs "1"
testrecord 'A : integer; B : string' \
           "R.A := 1; R.B := 'two'; write(R.A, ' ', R.B)" | outputs '1 two'
testrecord 'A : string; B : integer' \
           "R.A := 'one'; R.B := 2; write(R.A, ' ', R.B)" | outputs 'one 2'
testrecord 'case boolean of
              true: (A : string);
              false: (B : integer)' \
           "R.A := 'one'; write(R.A)" | outputs 'one'
testrecord 'case boolean of
              true: (A : string);
              false: (B : integer)' \
           "R.B := 2; write(R.B)" | outputs '2'
testrecord 'A : integer;
            case boolean of
              true: (B : string);
              false: (C : char)' \
           "R.A := 1; R.B := 'two'; write(R.A, ' ', R.B)" | outputs '1 two'
testrecord 'A : integer;
            case boolean of
              true: (B : string);
              false: (C : char)' \
           "R.A := 1; R.C := '2'; write(R.A, ' ', R.C)" | outputs '1 2'
# This one is technically undefined behavior, but we use it to check that the
# variants occupy the same space in memory.
testrecord 'A : integer;
            case boolean of
              true: (B : string);
              false: (C : char; D : char)' \
           "R.A := 1; R.B := 'abcdefghijklmnopqrstuvwxyzabcdefg'; write(R.A, ' ', R.C, ' ', R.D)" |
outputs '1 ! a'

echo "program foo; var A, B : record C : integer end;
      begin A.C := 1; B.C := 2; write(A.C, ' ', B.C) end." | outputs '1 2'

# With statement
echo "program foo;
      var A : record
                B : integer;
                C : string
              end;
          B : string;
      begin
        A.B := 1;
        A.C := 'two';
        B := 'three';
        write(A.B, ' ', A.C, ' ', B, ' ');
        with A do
          write(A.B, ' ', A.C, ' ', B)
      end." | outputs '1 two three 1 two 1'
echo "program foo;
      var A : record
                B : integer;
                C : record
                    A : string;
                    C : integer
                end
              end;
          B : string;
      begin
        A.B := 1;
        A.C.A := 'two';
        A.C.C := 3;
        B := 'four';
        write(A.B, ' ', A.C.A, ' ', A.C.C, ' ', B, ' ');
        with A, C do
          write(A, ' ', B)
      end." | outputs '1 two 3 four two 1'
