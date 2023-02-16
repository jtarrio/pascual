{ Examples of types }
program types;

procedure BasicTypes;
var 
  b : boolean;
  i : integer;
  r : real;
  c : char;
  s : string;
begin
  { Booleans can take the values true and false }
  b := true;
  b := false;
  { Integers are 32-bit signed }
  i := 12345678;
  i := -12345678;
  { Hexadecimal integers are also supported with a $ syntax. }
  i := $CAFE;
  { Reals are double precision floating point }
  r := 6.02e23;
  r := 1e-10;
  r := -42.0;
  { Chars are 8-bits. }
  { They should be called 'bytes', but in the 70s there wasn't a difference. }
  c := 'a';
  c := 'Z';
  { Strings contain up to 255 bytes. }
  s := 'Hello, world';
  s := '';
  s := 'En un lugar de La Mancha, de cuyo nombre no quiero acordarme...'
end;

procedure EnumeratedTypes;
type 
  Suits = (Hearts, Diamonds, Clubs, Spades);
  Directions = (North, East, South, West);
var 
  Suit : Suits;
  Direction : Directions;
  Gear : (Park, Reverse, Neutral, Low, Direct);
begin
  { The enumerated types only contains the values for each type. }
  Suit := Diamonds;
  Direction := West;
  Gear := Neutral;

  writeln(Suit, ' ', Direction, ' ', Gear);

  { You can compare the values of each enumerated type. }
  { The values for a type have the same order they were declared in. }
  writeln('Suit > Hearts  -> ', Suit > Hearts);
  writeln('Direction = West  -> ', Direction = West);
end;

procedure Subranges;
type 
  FirstHundred = 1..100;
  Suits = (Hearts, Diamonds, Clubs, Spades);
  Reds = Hearts..Diamonds;
  Blacks = Clubs..Spades;
var 
  Num : FirstHundred;
  RedSuit : Reds;
  BlackSuit : Blacks;
  Letter : 'A'..'Z';
begin
  { Subranges create new types that are a restricted subset of another type. }
  { If bounds checking is enabled, trying to assign an out-of-range value }
  { causes a run-time error. If it can be detected during compilation, it }
  { always causes a compilation-time error. }
  Num := 27;
  { Num := 102; } { Would cause a compile-time out-of-bounds error.}
  { Num := Num + 100; } { Would cause a run-time out-of-bounds error. }
  RedSuit := Hearts;
  BlackSuit := Spades;
  Letter := 'X';
end;

procedure Arrays;
type 
  Suits = (Hearts, Diamonds, Clubs, Spades);
  Piece = (King, Queen, Bishop, Knight, Rook, Pawn);
var 
  { Arrays indices belong to a bounded type. }
  Names : array[1..12] of string;
  { It is common for Pascal arrays to be 1-based, but it's not mandatory. }
  TemperatureCounts : array[-100..100] of integer;
  { Arrays are generally indexed by a subrange, but other bounded types }
  { are available: booleans, chars, and enumerated types. }
  CharacterCounts : array[char] of integer;
  LetterCounts : array['a'..'z'] of integer;
  SuitCounts : array[Suits] of integer;
  BooleanCounts : array[boolean] of integer;

  { You can have arrays of anything other than integers and strings, }
  { even arrays of arrays. }
  ChessBoard : array[1..8] of array[1..8] of Piece;
  { We have a nice comma syntax for arrays of arrays. }
  { This is equivalent to the above. }
  OtherChessBoard : array[1..8, 1..8] of Piece;
  { And, of course, any combination. }
  Mashup : array[20..32, 'a'..'z', boolean] of real;

begin
  { Accessing an array }
  Names[1] := 'Mark';
  { If bounds checking is enabled, trying to access an out-of-range element }
  { causes a run-time error. If it can be detected during compilation, it }
  { always causes a compilation-time error. }
  { Names[0] := 'Foo'; } { Would cause a compilation-time error. }

  SuitCounts[Diamonds] := 3;

  { There are two ways to access arrays of arrays. }
  ChessBoard[3][2] := Pawn;
  ChessBoard[3, 2] := Pawn;
  { Both are equivalent. }

  { And, of course: }
  Mashup[21, 'j', true] := 3.1416;
  Mashup[21]['j'][true] := 3.1416;
end;

procedure Records;
type 
  Suits = (Hearts, Diamonds, Clubs, Spades);
type
  { This is how you declare a simple record type. }
  R = record
    A : integer;
    B, C : real;
    D : string
  end;
var 
  One : R;
  Two : R;
  { You can declare tagged variant types. }
  Three : record
    A : integer;
    B : real;
    case C : Suits of 
      Hearts : ( D : string);
      Diamonds : ( E : string; F : integer);
      Clubs, Spades : ( G : boolean);
  end;
  { The above basically means that there is a field 'C' of type Suits that }
  { determines which one of the variants is valid. In practice, Pascual does }
  { not check that the value of field C is the correct one for the variant }
  { being accessed. }
  { There are anonymous variants too. }
  Four : record
    A : integer;
    B : real;
    case Suits of 
      Hearts : ( D : string);
      Diamonds : ( E : string; F : integer);
      Clubs, Spades : ( G : boolean);
  end;
  { You can nest records in records, etc. }
begin
  { How you access the fields: }
  One.A := 123;
  One.B := 12.34;
  { You can copy records with the assignment operator. }
  Two := One;
  { Accessing variant fields has exactly the same syntax. }
  Three.D := 'These are hearts';
end;

begin
  BasicTypes;
  EnumeratedTypes;
  Subranges;
  Arrays;
  Records
end.