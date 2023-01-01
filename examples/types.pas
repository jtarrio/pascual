program types;

type
  Boo = Boolean;
  Numbers = (One, Two, Three);
  Nums = Numbers;
  Rec = record
    Num : integer;
    Str : string
  end;

var
  Scared : Boo;
  ANumber : Numbers;
  ANum : Nums;
  ARec : Rec;
  ALetter : (LetterA, LetterB, LetterC, LetterD);
  AName : record
    First : string;
    Last : string;
  end;

procedure Proc;
var
  Fr : (Un, Doix, Trois);
begin
  writeln(Doix)
end;

function Fun : integer;
var
  De : (Eins, Zwei, Drei);
begin
  writeln(Drei);
  Fun := 0
end;

begin
  Scared := True;
  ANumber := Two;
  ANum := Three;
  ARec.Num := 123;
  ARec.Str := 'four five six';
  ALetter := LetterD;
  AName.First := 'Sherlock';
  AName.Last := 'Holmes';

  writeln(Scared);
  writeln(ANumber);
  writeln(ANum);
  writeln(ARec.Num);
  writeln(ARec.Str);
  writeln(ALetter);
  writeln(AName.First);
  writeln(AName.Last);

  Proc();
  writeln(Fun())
end.