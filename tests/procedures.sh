#!/bin/bash

. ./testing.libsh

# Basic procedure calls.
echo "program foo;
      procedure Proc;
      begin write('proc') end;
      begin Proc end." | outputs 'proc'
echo "program foo;
      procedure Proc;
      begin write('proc') end;
      begin Proc() end." | outputs 'proc'
echo "program foo;
      procedure Proc(a : integer; b : integer);
      begin write('proc ', a, ' ', b) end;
      begin Proc(1, 2) end." | outputs 'proc 1 2'
echo "program foo;
      procedure Proc(a : string; b, c : integer; d : string);
      begin write('proc ', a, ' ', b, ' ', c, ' ', d) end;
      begin Proc('1', 2, 3, '4') end." | outputs 'proc 1 2 3 4'

# Arguments are passed by value
echo "program foo;
      var i : integer;
      procedure Proc(a : integer);
      begin
        a := 10;
        write('proc ', a)
      end;
      begin
        i := 1;
        Proc(i);
        write(' ', i)
      end." | outputs 'proc 10 1'
# Arguments can be passed by reference
echo "program foo;
      var i : integer;
      procedure Proc(var a : integer);
      begin
        a := 10;
        write('proc ', a)
      end;
      begin
        i := 1;
        Proc(i);
        write(' ', i)
      end." | outputs 'proc 10 10'
echo "program foo;
      var i, j : integer;
      procedure Swap(var a, b : integer);
      var i : integer;
      begin
        i := a; a := b; b := i
      end;
      begin
        i := 1; j := 2;
        Swap(i, j);
        write(i, ' ', j)
      end." | outputs '2 1'
# Arguments can be passed by const reference
echo "program foo;
      var i : integer;
      procedure Proc(const a : integer);
      begin
        write('proc ', a)
      end;
      begin
        i := 1;
        Proc(i);
        write(' ', i)
      end." | outputs 'proc 1 1'
echo "program foo;
      var i : integer;
      procedure Proc(const a : integer);
      begin
        a := 10;
        write('proc ', a)
      end;
      begin
        i := 1;
        Proc(i);
        write(' ', i)
      end." | is_not_valid
echo "program foo;
      procedure Proc(const a : integer);
      begin
        write('proc ', a)
      end;
      begin
        Proc(1)
      end." | outputs 'proc 1'
echo "program foo;
      procedure Proc(const a, b : string);
      begin
        write('proc ', a, ' ', b)
      end;
      begin
        Proc('foo', 'bar')
      end." | outputs 'proc foo bar'
# Variables and types can be defined inside the procedure
echo "program foo;
      procedure Proc(a : integer);
      type T = integer;
      var b : T;
      begin
        b := a + 1;
        write('proc ', a, ' ', b)
      end;
      begin Proc(1) end." | outputs 'proc 1 2'
# Variables and types are scoped and locals override globals
echo "program foo;
      type T = boolean;
      var b : char;
      var c : T;
      procedure Proc(a : integer);
      type T = integer;
      var b : T;
      begin
        b := a + 1;
        write('proc ', a, ' ', b)
      end;
      begin
        b := 'b';
        c := true;
        Proc(1);
        write(' ', b, ' ', c)
      end." | outputs 'proc 1 2 b TRUE'
# Procedures can be called recursively
echo "program foo;
      procedure Proc(n : integer);
      var a : integer;
      begin
        a := n;
        write('n=', n, ' ');
        if n > 1 then Proc(n - 1);
        write('a=', a, ' ');
      end;
      begin Proc(3) end." | outputs 'n=3 n=2 n=1 a=1 a=2 a=3 '
# Procedures can be forward declared
echo "program foo;
      procedure Proc1(n : integer); forward;
      procedure Proc2(n : integer);
      begin
        write('>p2 ', n, ' ');
        if n > 1 then Proc1(n - 1);
        write('<p2 ', n, ' ')
      end;
      procedure Proc1(n : integer);
      begin
        write('>p1 ', n, ' ');
        if n > 1 then Proc2(n - 1);
        write('<p1 ', n, ' ')
      end;
      begin Proc1(4) end." |
outputs '>p1 4 >p2 3 >p1 2 >p2 1 <p2 1 <p1 2 <p2 3 <p1 4 '
echo "program foo;
      procedure Proc1(n : integer); forward;
      procedure Proc2(n : integer);
      begin
        write('>p2 ', n, ' ');
        if n > 1 then Proc1(n - 1);
        write('<p2 ', n, ' ')
      end;
      procedure Proc1;
      begin
        write('>p1 ', n, ' ');
        if n > 1 then Proc2(n - 1);
        write('<p1 ', n, ' ')
      end;
      begin Proc1(4) end." |
outputs '>p1 4 >p2 3 >p1 2 >p2 1 <p2 1 <p1 2 <p2 3 <p1 4 '
# Signature of implementation must match forward declaration
echo "program foo;
      procedure Proc(n : integer); forward;
      procedure Proc(n : string);
      begin end;
      begin end." | is_not_valid

# Items passed by reference must be assignable
echo "program foo;
      function Make : integer;
      begin Make := 1 end;
      procedure Change(var t : integer);
      begin t := 2 end;
      begin Change(Make) end." | is_not_valid
echo "program foo;
      const x : integer = 1;
      procedure Change(var t : integer);
      begin t := 2 end;
      begin Change(x) end." | is_not_valid
