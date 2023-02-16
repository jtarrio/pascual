{ Showing how to use untyped and typed constants. }
program consts;

{ Untyped constants don't have a type attached to them, only a value. }
{ They don't reside in memory. Instead, Pascual replaces them with their value }
{ wherever they appear. }
{ Therefore, you can use them anywhere you can also use an immediate value. }
const 
  Abc = 'abc';
  { You can define an untyped constant using another untyped constant. }
  AnotherAbc = Abc;
  Limit = 30;
  { You can even use math and expressions that yield constants! }
  Value = 5 * 8 - Limit;

var 
  { See how we can use the 'Limit' untyped constant to define the array size. }
  AnArray : array[1..Limit] of integer;

{ Typed constants work like variables whose value you can't change. }
{ They reside in memory and you can pass them by reference. }
{ However, you cannot use them as immediate values. }
const 
  Bcd : string = 'bcd';
  TypedInt : integer = 789;
  TypedReal : real = 6.02e23;

begin
  { You can inspect the generated C code to see how the two kinds of constants }
  { are treated differently. }

  { Print out the untyped constants we defined above. }
  writeln(Abc, ' ', AnotherAbc, ' ', Limit, ' ', Value);

  { Print out the typed constants we defined above. }
  writeln(Bcd, ' ', TypedInt, ' ', TypedReal)
end.
