#!/bin/bash

. ./testing.libsh

# If
testif() {
  echo "program foo; begin
        write('S ');
        $1;
        write(' E')
        end."
}
# If-then-else
testif "if true then write('true')" | outputs 'S true E'
testif "if false then write('true')" | outputs 'S  E'
testif "if true then write('true') else write('false')" | outputs 'S true E'
testif "if false then write('true') else write('false')" | outputs 'S false E'
testif "if true then else write('false')" | outputs 'S  E'
testif "if false then else write('false')" | outputs 'S false E'
# Compound then and else
testif "if true then begin write('tr'); write('ue') end
                else begin write('fal'); write('se') end" | outputs 'S true E'
testif "if false then begin write('tr'); write('ue') end
                 else begin write('fal'); write('se') end" | outputs 'S false E'
# If-then-else if
testif "if true then write('true1')
                else if true then write('true2')
                             else write('false')" | outputs 'S true1 E'
testif "if true then write('true1')
                else if false then write('true2')
                             else write('false')" | outputs 'S true1 E'
testif "if false then write('true1')
                 else if true then write('true2')
                              else write('false')" | outputs 'S true2 E'
testif "if false then write('true1')
                 else if false then write('true2')
                               else write('false')" | outputs 'S false E'
# If-then (If-then-else) (the 'else' goes with the most recent 'then')
testif "if true then if true then write('true')
                             else write('false')" | outputs 'S true E'
testif "if true then if false then write('true')
                              else write('false')" | outputs 'S false E'
testif "if false then if true then write('true')
                              else write('false')" | outputs 'S  E'
testif "if false then if false then write('true')
                               else write('false')" | outputs 'S  E'
# If-then (If-then-else) else
testif "if true then if true then write('true')
                             else write('false1')
                else write('false2')" | outputs 'S true E'
testif "if true then if false then write('true')
                              else write('false1')
                else write('false2')" | outputs 'S false1 E'
testif "if false then if true then write('true')
                              else write('false1')
                 else write('false2')" | outputs 'S false2 E'
testif "if false then if false then write('true')
                               else write('false1')
                 else write('false2')" | outputs 'S false2 E'
# The expression must be boolean
testif "if 1 then write('true') else write('false')" | is_not_valid
testif "if 'a' then write('true') else write('false')" | is_not_valid
testif "if 'abc' then write('true') else write('false')" | is_not_valid

# Case
testcase() {
  echo "program foo; var i : $1;
        begin i := $2; case i of $3 end end."
}
testcase boolean true "true : write('1x');
                       false : write('2x')" | outputs '1x'
testcase boolean false "true : write('1x');
                        false : write('2x')" | outputs '2x'
testcase char "'a'" "'a' : write('1x');
                     'b' : begin write('2a'); write('2b') end;
                     'd' : write('4x')" | outputs '1x'
testcase char "'b'" "'a' : write('1x');
                     'b' : begin write('2a'); write('2b') end;
                     'd' : write('4x')" | outputs '2a2b'
testcase char "'c'" "'a' : write('1x');
                     'b' : begin write('2a'); write('2b') end;
                     'd' : write('4x')" | outputs ''
testcase char "'d'" "'a' : write('1x');
                     'b' : begin write('2a'); write('2b') end;
                     'd' : write('4x')" | outputs '4x'
testcase integer 1 "1 : write('1x');
                    2 : begin write('2a'); write('2b') end;
                    4 : write('4x')" | outputs '1x'
testcase integer 2 "1 : write('1x');
                    2 : begin write('2a'); write('2b') end;
                    4 : write('4x')" | outputs '2a2b'
testcase integer 3 "1 : write('1x');
                    2 : begin write('2a'); write('2b') end;
                    4 : write('4x')" | outputs ''
testcase integer 4 "1 : write('1x');
                    2 : begin write('2a'); write('2b') end;
                    4 : write('4x')" | outputs '4x'
testcase "(One, Two, Three, Four)" One \
         "One : write('1x');
          Two : begin write('2a'); write('2b') end;
          Four : write('4x')" | outputs '1x'
testcase "(One, Two, Three, Four)" Two \
         "One : write('1x');
          Two : begin write('2a'); write('2b') end;
          Four : write('4x')" | outputs '2a2b'
testcase "(One, Two, Three, Four)" Three \
         "One : write('1x');
          Two : begin write('2a'); write('2b') end;
          Four : write('4x')" | outputs ''
testcase "(One, Two, Three, Four)" Four \
         "One : write('1x');
          Two : begin write('2a'); write('2b') end;
          Four : write('4x')" | outputs '4x'

# Repeat-until
testlooptype() {
  echo "program foo;
        var i : $1;
        begin
          write('S ');
          $2;
          write(' E')
        end." 
}
testloop() {
  testlooptype integer "$1"
}
testloop "i := 0;
          repeat i := i + 1; write(i, ' ') until i = 10;
          write(i)" | outputs 'S 1 2 3 4 5 6 7 8 9 10 10 E'
testloop "i := 0;
          repeat i := i + 1; write(i, ' ') until i > 0;
          write(i)" | outputs 'S 1 1 E'
testloop "i := 10;
          repeat i := i + 1; write(i, ' ') until i > 0;
          write(i)" | outputs 'S 11 11 E'
testloop "i := 0; repeat i := i + 1 until 0" | is_not_valid

# While
testloop "i := 0;
          while i < 10 do begin i := i + 1; write(i, ' ') end;
          write(i)" | outputs 'S 1 2 3 4 5 6 7 8 9 10 10 E'
testloop "i := 0;
          while i < 0 do begin i := i + 1; write(i, ' ') end;
          write(i)" | outputs 'S 0 E'
testloop "i := 0; while 0 do i := i + 1" | is_not_valid

# For
testloop "i := 99;
          for i := 0 to 10 do write(i, ' ');
          write(i)" | outputs 'S 0 1 2 3 4 5 6 7 8 9 10 10 E'
testloop "i := 99;
          for i := 0 to 0 do write(i, ' ');
          write(i)" | outputs 'S 0 0 E'
testloop "i := 99;
          for i := 0 to -10 do write(i, ' ');
          write(i)" | outputs 'S 99 E'
testloop "i := 99;
          for i := 10 downto 0 do write(i, ' ');
          write(i)" | outputs 'S 10 9 8 7 6 5 4 3 2 1 0 0 E'
testloop "i := 99;
          for i := 0 downto 0 do write(i, ' ');
          write(i)" | outputs 'S 0 0 E'
testloop "i := 99;
          for i := 0 downto 10 do write(i, ' ');
          write(i)" | outputs 'S 99 E'
testlooptype char "for i := 'a' to 'j' do write(i)" | outputs 'S abcdefghij E'
testlooptype char "for i := 'j' downto 'a' do write(i)" | outputs 'S jihgfedcba E'
testlooptype "(A,B,C,D,E,F)" "for i := B to E do write(i)" | outputs 'S BCDE E'
testlooptype boolean "for i := false to true do write(i)" |
outputs 'S FALSETRUE E'
testlooptype string "for i := 'a' to 'z' do write(i)" | is_not_valid
