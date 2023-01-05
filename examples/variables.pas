program variables;

const 
  cBool : boolean = true;
  cInt : integer = 1234;
  cChr : char = 'x';
  cStr : string = 'constant string';
  cArray : array[1..3] of integer = (4, 5, 6);

var 
  vBool : boolean;
  vInt : integer;
  vChr : char;
  vStr : string;
  vArray : array[1..3] of integer;

begin
  vBool := not cBool;
  vInt := cInt + 4444;
  vChr := 'j';
  vStr := 'variable string';
  vArray[1] := 7;
  vArray[2] := 8;
  vArray[3] := 9;

  writeln(cBool, ' ', cInt, ' ', cChr, ' ', cArray[1], cArray[2], cArray[3]);
  writeln(vBool, ' ', vInt, ' ', vChr, ' ', vArray[1], vArray[2], vArray[3]);
end.
