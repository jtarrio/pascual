#!/bin/bash

. ./testing.libsh

# Nil
echo "program foo; var P : ^integer; begin P := nil end." | outputs ''

# New/dispose
echo "program foo; var P : ^integer;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3
echo "program foo; type TP = ^integer; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3
echo "program foo; type T = integer; TP = ^T; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3

# Getmem/freemem
echo "program foo; var P : ^integer;
      begin getmem(P, sizeof(integer)); P^ := 3; write(P^); freemem(P, sizeof(integer)) end." | outputs 3
echo "program foo; type TP = ^integer; var P : TP;
      begin getmem(P, sizeof(integer)); P^ := 3; write(P^); freemem(P, sizeof(integer)) end." | outputs 3
echo "program foo; type T = integer; TP = ^T; var P : TP;
      begin getmem(P, sizeof(T)); P^ := 3; write(P^); freemem(P, sizeof(T)) end." | outputs 3

# Address operator
echo "program foo; var I : integer; P : ^integer;
      begin I := 3; P := @I; write(P^) end." | outputs 3
echo "program foo; type IP = ^integer; var I : integer; P : IP; PP : ^IP;
      begin I := 3; P := @I; PP := @P; write(PP^^) end." | outputs 3
echo "program foo; type IP = ^integer; var R : record A : integer end; P : IP;
      begin R.A := 3; P := @R.A; write(P^) end." | outputs 3

# Can declare a pointer of a type defined in the same block.
echo "program foo; type TP = ^T; T = integer; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | outputs 3
echo "program foo; type TP = ^T; type T = integer; var P : TP;
      begin new(P); P^ := 3; write(P^); dispose(P) end." | is_not_valid

# Placeholders do not alter the order of definitions.
echo "program foo;
      type TP = ^T;
           E = (One, Two);
           T = record
               A : E
           end;
      begin end." | outputs ''

# Untyped pointers
echo "program foo;
      function RetAsCh(a : pointer) : char;
      var ch : ^char absolute a;
      begin Result := ch^ end;
      var i : integer;
      begin i := 65; write(RetAsCh(@i)) end." | outputs 'A'
echo "program foo;
      function RetAsCh(var a : pointer) : char;
      var ch : ^char absolute a;
      begin Result := ch^ end;
      var i : integer;
          p : pointer;
      begin i := 65; p := @i; write(RetAsCh(p)) end." | outputs 'A'

# Pointer arithmetic
echo "program foo; var A : array[1..5] of integer; P : ^integer; I : integer;
      begin A[1] := 10; A[2] := 21; A[3] := 32; A[4] := 43; A[5] := 54;
            P := @A[1]; for I := 1 to 5 do begin write(P^); P := P + 1 end end." | outputs "1021324354"
echo "program foo; var A : array[1..5] of integer; P : ^integer; I : integer;
      begin A[1] := 10; A[2] := 21; A[3] := 32; A[4] := 43; A[5] := 54;
            P := @A[1]; for I := 1 to 5 do begin write(P^); P := succ(p) end end." | outputs "1021324354"
echo "program foo; var A : array[1..5] of integer; P : ^integer; I : integer;
      begin A[1] := 10; A[2] := 21; A[3] := 32; A[4] := 43; A[5] := 54;
            P := @A[5]; for I := 1 to 5 do begin write(P^); P := P - 1 end end." | outputs "5443322110"
echo "program foo; var A : array[1..5] of integer; P : ^integer; I : integer;
      begin A[1] := 10; A[2] := 21; A[3] := 32; A[4] := 43; A[5] := 54;
            P := @A[5]; for I := 1 to 5 do begin write(P^); P := pred(p) end end." | outputs "5443322110"

# Longer program showing pointer manipulation.
echo "program foo;
      type TP = ^T;
      T = record
        value : integer;
        prev, next : TP
      end;
      var root, last : TP;
      procedure add(v : integer);
      begin
        if root = nil then
        begin
          new(root);
          root^.prev := nil;
          root^.next := nil;
          last := root
        end
        else
        begin
          new(last^.next);
          last^.next^.prev := last;
          last := last^.next
        end;
        last^.value := v
      end;
      function remove : integer;
      var
        prev : TP;
      begin
        if last = nil then remove := 0
        else
        begin
          remove := last^.value;
          prev := last^.prev;
          dispose(last);
          last := prev;
          if last = nil then root := nil
        end;
      end;

      begin
        root := nil;
        add(1); add(2); add(3); add(5); add(8);
        while root <> nil do
          write(remove, ' ')
      end." | outputs '8 5 3 2 1 '
