(* Examples of chars strings *)
program strings;

procedure Literals;
begin
  writeln('This is a string literal.');
  writeln('I can add a single quote ('') by doubling it, like I just did.');
  writeln('This is an empty string:', '');
  writeln('You can insert characters by ASCII code, like this: '#65#66#67);
  writeln('The previous line printed ''ABC'' at the end.');
  writeln('The ASCII codes can also be hexadecimal, like this: '#$41#$42#$43);
  writeln('There is also a syntax for control characters, like this: '^G);
  writeln('That was the Control-G character, or BEL.');
  writeln('You can go from NUL ('^@') all the way to '^_);
  writeln('You can also use lowercase for your control characters, like '^g);

  writeln('Character literals are just like string literals, ',
          'but with only one character, like so: ', 'X');
  writeln('You can also use the # and #$ syntax, like so: ', #65, #$42);
  writeln('You can also specify control characters: ', ^g)
end;

procedure Operations;
begin
  writeln('Concatenating strings ' + 'is easy');
  writeln('You can also concatenate chars in the same way: ', 'a' + 'b');
  writeln('Note that, in Pascual, strings are limited to 255 bytes.');
  writeln('(Because of UTF-8, bytes are not the same thing as characters.)');
  writeln('Use relational operators to compare strings: ');
  writeln('abc = bcd -> ', 'abc' = 'bcd');
  writeln('abc <> bcd -> ', 'abc' <> 'bcd');
  writeln('abc > bcd -> ', 'abc' > 'bcd');
  writeln('abc <= bcd -> ', 'abc' <= 'bcd');
  writeln('Use the Length() function to know a string''s length.');
  writeln('For example, ''Hello'' has length ', Length('Hello'));
end;

procedure Variables;
var a, b : string;
begin
  a := 'You can store strings in variables';
  b := 'and operate with them';
  writeln(a + ' ' + b);
  writeln('Use subscript notation to extract single bytes: ', a[1], a[2], a[3]);
  writeln('As usual in Pascal, subscripts are 1-based.');
  writeln('It is not an error to access bytes past the end of the string.');
  writeln('(As long as you don''t try to go over 255 bytes.)');
  writeln('You will most likely get garbage in the output, though.');
end;

procedure ProceduresAndFunctions;
var a, b : string;
begin
  a := 'You can use the function Copy() to extract part of a string';
  writeln(a, ': ', Copy(a, 13, 30));
  a := 'abcde';
  b := 'XX';
  Insert(b, a, 3);
  writeln('Use Insert() to insert a string inside another string: ', a);
  a := '1234567890';
  Delete(a, 3, 4);
  writeln('Use Delete() to delete some characters from a string: ', a);
  a := 'abcdbcde';
  writeln('Use Pos() to find a string inside another string: ',
          Pos('cdb', a));
end;

procedure Conversions;
var
  a : string;
  b : integer;
  c : boolean;
  d : (One, Two, Three);
  x : integer;
begin
  writeln('You can use Str() to convert several types to string.');
  Str(42, a);
  writeln(a);
  Str(Three, a);
  writeln(a);
  Str(true, a);
  writeln(a);
  writeln('Use Val() to convert a string into a number, boolean, or enum.');
  Val('42', b, x);
  Val('false', c, x);
  Val('Two', d, x);
  writeln(b, ' ', c, ' ', d);
end;

begin
  Literals;
  Operations;
  Variables;
  ProceduresAndFunctions;
  Conversions
end.
