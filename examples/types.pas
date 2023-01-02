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

  Proc();
  writeln(Fun())
end.