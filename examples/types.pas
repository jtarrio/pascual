(* Tests for types *)
program types;

type
  Boo = Boolean;
  Numbers = (One, Two, Three);
  Nums = Numbers;
  Rec = record
    Num : integer;
    Str : string
  end;
  Rec2 = record
    Bol : boolean;
    Rec : Rec
  end;
  Arr1 = array[1..10] of integer;
  Arr2 = array[1..5] of Rec;

var
  Scared : Boo;
  ANumber : Numbers;
  ANum : Nums;
  ARec : Rec;
  ARec2 : Rec2;
  ALetter : (LetterA, LetterB, LetterC, LetterD);
  AName : record
    First : string;
    Last : string;
  end;
  AArr1 : Arr1;
  AArr3 : array[1..20] of string;

procedure Proc;
var
  Fr : (Un, Deux, Trois);
begin
  writeln(Deux)
end;

function Fun : integer;
var
  De : (Eins, Zwei, Drei);
begin
  writeln(Drei);
  Fun := 0
end;

procedure Arrays;
var
  Pos : integer;
  BArr2 : Arr2;
  BArr4 : array[1..10] of boolean;
begin
  for Pos := 1 to 10 do
    AArr1[Pos] := Pos;
  for Pos := 1 to 10 do
  begin
    Str(AArr1[Pos], AArr3[Pos]);
    Str(10 + AArr1[Pos] * 2, AArr3[Pos + 10])
  end;
  BArr2[1].Str := 'In an array';
  for Pos := 1 to 10 do
    BArr4[Pos] := (AArr1[Pos] mod 2) = 0;

  for Pos := 1 to 10 do
    writeln(AArr3[Pos], ' ', AArr1[Pos], ' ', BArr4[Pos]);
  for Pos := 11 to 20 do
    writeln(AArr3[Pos]);
  writeln(BArr2[1].Str);
end;


begin
  Scared := True;
  ANumber := Two;
  ANum := Three;
  ARec.Num := 123;
  ARec.Str := 'four five six';
  ARec2.Bol := True;
  ARec2.Rec.Num := 456;
  ARec2.Rec.Str := 'seven eight nine';
  ALetter := LetterD;
  AName.First := 'Sherlock';
  AName.Last := 'Holmes';

  writeln(Scared);
  writeln(ANumber);
  writeln(ANum);
  writeln(ARec.Num);
  writeln(ARec.Str);
  writeln(ARec2.Bol);
  writeln(ARec2.Rec.Num);
  writeln(ARec2.Rec.Str);
  writeln(ALetter);
  writeln(AName.First);
  writeln(AName.Last);

  Proc;
  writeln(Fun);

  Arrays
end.