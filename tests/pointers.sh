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

# Address operator
echo "program foo; var I : integer; P : ^integer;
      begin I := 3; P := @I; write(P^) end." | outputs 3
echo "program foo; type IP = ^integer; var I : integer; P : IP; PP : ^IP;
      begin I := 3; P := @I; PP := @P; write(PP^^) end." | outputs 3

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
