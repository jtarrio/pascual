{ Sets }
program Sets;

procedure SetSyntax;
var 
  s : set of 0..10;
  i : integer;
begin
  { To specify a set, write its members inside square brackets. }

  { Empty set }
  s := [];
  { Even numbers from 0 to 10 }
  s := [2, 4, 6, 8, 10];
  { You can specify ranges with .., like the numbers before 5 }
  s := [0..4];
  { You can use expressions to specify members }
  i := 2;
  s := [i..i+2];
end;

procedure BelongsToASet;
var Ch : char;
begin
  Ch := '9';

  { The 'in' operator is used to check if an element belongs to a set. }
  writeln(Ch in ['a'..'z']);
  { It can help you avoid having to write a complicated boolean condition. }
  writeln(('0' <= Ch) and (Ch <= '9')
          or ('A' <= Ch) and (Ch <= 'F') 
          or ('a' <= Ch) and (Ch <= 'f'));
  writeln(Ch in ['0'..'9', 'A'..'F', 'a'..'f']);
  { These two are equivalent, but the second one is so much simpler! }
end;

procedure Operations;
var 
  s1, s2, s3 : set of char;
begin
  s1 := ['0'..'9', 'A'..'Z', 'a'..'z'];
  s2 := ['0'..'9', 'A'..'F', 'a'..'f'];

  { Union }
  s3 := s1 + s2;
  writeln('a' in s3, ' ', 'z' in s3);
  { Difference }
  s3 := s1 - s2;
  writeln('a' in s3, ' ', 'z' in s3);
  { Intersection }
  s3 := s1 * s2;
  writeln('a' in s3, ' ', 'z' in s3);

  { Equality test }
  writeln(s1 = s2);
  writeln(s1 <> s2);
  { Is-superset }
  writeln(s1 >= s2);
  writeln(s1 <= s2);
end;

procedure MaterializedSets;
var 
  sr : set of 0..10;
  sc : set of char;
  sb : set of boolean;
  { si : set of integer; } { Does not work, see below }
  i : integer;
begin
  { You can only declare sets that can hold up to 256 elements. }
  { So you can declare a 'set of char' or a 'set of 1..100', but not a }
  { 'set of integer'. }
  sr := [1, 5..7];
  writeln(3 in sr);
  sc := ['a', '0'];
  writeln('f' in sc);
  sb := [];
  writeln(true in sb);

  { Note that, with the square bracket syntax, you can write a set that }
  { holds more than 256 elements. This is useful with the 'in' operator, }
  { but it will give you an error if you try to store it in a variable or }
  { return it from a function. }

  i := 53;
  writeln(i in [1..2000000]);
  { This set has two million elements and Pascual doesn't bat an eye. }
  { That happens because it gets turned into '1 <= i and i <= 2000000'. }
end;

begin
  SetSyntax;
  BelongsToASet;
  Operations;
  MaterializedSets
end.
