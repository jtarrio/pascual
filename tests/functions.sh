#!/bin/bash

. ./lib.sh

# Basic function calls.
echo "program foo;
      function Fun : string;
      begin Fun := 'fun' end;
      begin write(Fun) end." | outputs 'fun'
echo "program foo;
      function Fun : string;
      begin Fun := 'fun' end;
      begin write(Fun()) end." | outputs 'fun'
echo "program foo;
      function Fun(a : string; b : string) : string;
      begin Fun := 'fun ' + a + ' ' + b end;
      begin write(Fun('1', '2')) end." | outputs 'fun 1 2'

# Arguments are passed by value
echo "program foo;
      var i : integer;
      function Fun(a : integer) : integer;
      begin
        a := 10;
        Fun := a + 5
      end;
      begin
        i := 1;
        write(Fun(i), ' ', i)
      end." | outputs '15 1'
# Arguments can be passed by reference
echo "program foo;
      var i : integer;
      function Fun(var a : integer) : integer;
      begin
        a := 10;
        Fun := a + 5
      end;
      begin
        i := 1;
        write(Fun(i), ' ', i)
      end." | outputs '15 10'
# Variables and types can be defined inside the function
echo "program foo;
      function Fun(a : integer) : integer;
      type T = integer;
      var b : T;
      begin
        b := a + 1;
        Fun := 2 * b
      end;
      begin write(Fun(1)) end." | outputs '4'
# Variables and types are scoped and locals override globals
echo "program foo;
      type T = boolean;
      var b : char;
      var c : T;
      function Fun(a : integer) : integer;
      type T = integer;
      var b : T;
      begin
        b := a + 1;
        Fun := 2 * b
      end;
      begin
        b := 'b';
        c := true;
        write(b, ' ', c, ' ');
        write(Fun(1));
        write(' ', b, ' ', c)
      end." | outputs 'b TRUE 4 b TRUE'
# Functions can be called recursively
echo "program foo;
      function Fib(n : integer) : integer;
      var a : integer;
      begin
        if n <= 1 then Fib := 1
        else Fib := Fib(n - 1) + Fib(n - 2)
      end;
      begin write(Fib(5)) end." | outputs '8'
# Functions can be forward declared
echo "program foo;
      function Fun1(n : integer) : integer; forward;
      function Fun2(n : integer) : integer;
      begin
        if n > 1 then Fun2 := 1 + Fun1(n - 1)
        else Fun2 := 1
      end;
      function Fun1(n : integer) : integer;
      begin
        if n > 1 then Fun1 := 2 * Fun2(n - 1)
        else Fun1 := 1
      end;
      begin write(Fun1(4)) end." |
outputs '6'
echo "program foo;
      function Fun1(n : integer) : integer; forward;
      function Fun2(n : integer) : integer;
      begin
        if n > 1 then Fun2 := 1 + Fun1(n - 1)
        else Fun2 := 1
      end;
      function Fun1;
      begin
        if n > 1 then Fun1 := 2 * Fun2(n - 1)
        else Fun1 := 1
      end;
      begin write(Fun1(4)) end." |
outputs '6'
# Signature of implementation must match forward declaration
echo "program foo;
      function Fun(n : integer) : integer; forward;
      function Fun(n : string) : integer;
      begin end;
      begin end." | is_not_valid
echo "program foo;
      function Fun(n : integer) : integer; forward;
      function Fun(n : integer) : string;
      begin end;
      begin end." | is_not_valid
