program stage0(Input, Output, StdErr);

type 
  TokenId = (TkUnknown, TkEof, TkComment, TkIdentifier, TkNumber, TkString,
             TkPlus, TkMinus, TkAsterisk, TkSlash, TkEquals, TkLessthan,
             TkMorethan, TkLbracket, TkRbracket, TkDot, TkComma, TkColon,
             TkSemicolon, TkCaret, TkLparen, TkRparen, TkNotEquals,
             TkLessOrEquals, TkMoreOrEquals, TkAssign, TkRange, TkAnd, TkArray,
             TkBegin, TkCase, TkConst, TkDiv, TkDo, TkDownto, TkElse, TkEnd,
             TkFile, TkFor, TkFunction, TkGoto, TkIf, TkIn, TkLabel, TkMod,
             TkNil, TkNot, TkOf, TkOr, TkPacked, TkProcedure, TkProgram,
             TkRecord, TkRepeat, TkSet, TkThen, TkTo, TkType, TkUntil, TkVar,
             TkWhile, TkWith);
  Token = record
    Id : TokenId;
    Value: string
  end;
const 
  Symbols: array[1..16] of string = ('+', '-', '*', '/', '=', '<', '>', '[',
                                     ']', '.', ',', ':', ';', '^', '(', ')');
  Words: array[1..35] of string = ('AND', 'ARRAY', 'BEGIN', 'CASE', 'CONST',
                                   'DIV', 'DO', 'DOWNTO', 'ELSE', 'END', 'FILE',
                                   'FOR', 'FUNCTION', 'GOTO', 'IF', 'IN',
                                   'LABEL', 'MOD', 'NIL', 'NOT', 'OF', 'OR',
                                   'PACKED', 'PROCEDURE', 'PROGRAM', 'RECORD',
                                   'REPEAT', 'SET', 'THEN', 'TO', 'TYPE',
                                   'UNTIL', 'VAR', 'WHILE', 'WITH');
var 
  CurrentLine: string;
  ThisToken: Token;

function IsAlpha(Chr : char) : boolean;
begin
  IsAlpha := ((Chr >= 'a') and (Chr <= 'z')) or ((Chr >= 'A') and (Chr <= 'Z'))
end;

function IsDigit(Chr : char) : boolean;
begin
  IsDigit := (Chr >= '0') and (Chr <= '9')
end;

function IsAlphaNum(Chr : char) : boolean;
begin
  IsAlphaNum := IsAlpha(Chr) or IsDigit(Chr)
end;

function IsTokenWaiting : boolean;
var 
  Comment : (No, Braces, Parens);
begin
  Comment := No;
  repeat
    while (Length(CurrentLine) = 0) and not Eof(Input) do
    begin
      readln(Input, CurrentLine);
    end;
    while (Length(CurrentLine) > 0) and (CurrentLine[1] = ' ') do
      delete(CurrentLine, 1, 1);
    if Comment = No then
    begin
      if CurrentLine[1] = '{' then
      begin
        Comment := Braces;
        delete(CurrentLine, 1, 1)
      end
      else if (Length(CurrentLine) > 1)
              and (CurrentLine[1] = '(')
              and (CurrentLine[2] = '*') then
      begin
        Comment := Parens;
        delete(CurrentLine, 1, 2)
      end
    end;
    while (Comment = Braces) and (Length(CurrentLine) > 0) do
    begin
      if CurrentLine[1] = '}' then
        Comment := No;
      delete(CurrentLine, 1, 1)
    end;
    while (Comment = Parens) and (Length(CurrentLine) > 0) do
    begin
      if (Length(CurrentLine) > 1) and (CurrentLine[1] = '*') and (CurrentLine[2
         ] = ')') then
      begin
        Comment := No;
        delete(CurrentLine, 1, 2)
      end
      else
        delete(CurrentLine, 1, 1)
    end;
  until (Comment = No) and (Length(CurrentLine) > 0) or Eof(Input);
  IsTokenWaiting := Length(CurrentLine) > 0
end;

procedure GetSymbol(var Token : Token; Id: TokenId; Length: integer);
begin
  Token.Value := copy(CurrentLine, 1, length);
  delete(CurrentLine, 1, length);
  Token.Id := Id;
end;

procedure GetIdentifier(var Token: Token);
var 
  Chr: char;
  Pos: integer;
  InToken: boolean;
  WordId: TokenId;
begin
  Pos := 0;
  InToken := True;
  WordId := TkAnd;
  while (Pos < Length(CurrentLine)) and InToken do
  begin
    Chr := CurrentLine[Pos + 1];
    InToken := IsAlphaNum(Chr);
    if InToken then Pos := Pos + 1
  end;
  GetSymbol(Token, TkIdentifier, Pos);
  for Pos := 1 to Length(Token.Value) do
    Token.Value[Pos] := UpCase(Token.Value[Pos]);
  for Pos := 1 to 35 do
    begin
      if Token.Value = Words[Pos] then
        Token.Id := WordId;
      WordId := succ(WordId)
    end
end;

procedure GetNumber(var Token : Token);
var 
  Chr: char;
  Pos : integer;
  Part : (None, Sign, Int, Dot, Frac, E, Mag);
begin
  Pos := 0;
  Part := Sign;
  while (Pos < Length(CurrentLine)) and (Part <> None) do
  begin
    Chr := CurrentLine[Pos + 1];
    if (Chr = '-') or (Chr = '+') then
    begin
      if Part = Sign then
        Part := Int
      else if Part = E then
             Part := Mag
      else
        Part := None
    end
    else if Chr = 'e' then
    begin
      if (Part = Int) or (Part = Frac) then
        Part := E
      else
        Part := None
    end
    else if Chr = '.' then
    begin
      if Part = Int then
        Part := Dot
      else
        Part := None
    end
    else if IsDigit(Chr) then
    begin
      if Part = Sign then
        Part := Int
      else if Part = Dot then
             Part := Frac
      else if Part = E then
             Part := Mag
    end
    else
      Part := None;
    if Part <> None then
      Pos := Pos + 1;
  end;
  if (CurrentLine[Pos] = '.') or (CurrentLine[Pos] = 'e') then
    Pos := Pos - 1;
  GetSymbol(Token, TkNumber, Pos);
end;

procedure GetString(var Token : Token);
var 
  Chr: char;
  Pos: integer;
  Instring: boolean;
begin
  Pos := 1;
  Instring := True;
  while Instring do
  begin
    Pos := Pos + 1;
    Chr := CurrentLine[Pos];
    if Chr = '''' then
    begin
      if (Pos + 1 < Length(CurrentLine)) and (CurrentLine[Pos + 1] = '''')
        then
      begin
        Pos := Pos + 2
      end
      else
        Instring := False;
    end;
  end;
  GetSymbol(Token, TkString, Pos);
end;

procedure ReadToken(var Token : Token);
var 
  Chr: char;
  Nxt: char;
  i : integer;
  SymbolId : TokenId;

begin
  Token.Value := '';
  Token.Id := TkUnknown;

  repeat
    if not IsTokenWaiting() then
      Token.Id := TkEof
    else
    begin
      Chr := CurrentLine[1];

      if IsAlpha(Chr) then
        GetIdentifier(Token);
      if (Token.Id = TkUnknown) and IsDigit(Chr) then
        GetNumber(Token);
      if (Token.Id = TkUnknown) and (Chr = '''') then
        GetString(Token);
      if (Token.Id = TkUnknown) and (Length(CurrentLine) > 1) then
      begin
        Nxt := CurrentLine[2];
        if (Token.Id = TkUnknown) and ((Chr = '+') or (Chr = '-'))
           and IsDigit(Nxt) then
          GetNumber(Token);
        if (Token.Id = TkUnknown) and (Chr = '<') and (Nxt = '>') then
          GetSymbol(Token, TkNotEquals, 2);
        if (Token.Id = TkUnknown) and (Chr = '<') and (Nxt = '=') then
          GetSymbol(Token, TkLessOrEquals, 2);
        if (Token.Id = TkUnknown) and (Chr = '>') and (Nxt = '=') then
          GetSymbol(Token, TkMoreOrEquals, 2);
        if (Token.Id = TkUnknown) and (Chr = ':') and (Nxt = '=') then
          GetSymbol(Token, TkAssign, 2);
        if (Token.Id = TkUnknown) and (Chr = '.') and (Nxt = '.') then
          GetSymbol(Token, TkRange, 2);
      end;
      SymbolId := TkPlus;
      for i := 1 to 16 do
        begin
          if (Token.Id = TkUnknown) and (Chr = Symbols[i]) then
            GetSymbol(Token, SymbolId, 1);
          SymbolId := succ(SymbolId)
        end;
    end;
  until Token.Id <> TkComment;
end;

procedure GetNextToken;
begin
  ReadToken(ThisToken);
end;

procedure ExpectToken(Id : TokenId);
begin
  if ThisToken.Id <> Id then
  begin
    writeln(StdErr, 'Expected token: ', Id, ', found ', ThisToken.Id,
            ' (', ThisToken.Value, ')');
    halt(1)
  end
end;

procedure ExpectToken2(Id1, Id2 : TokenId);
begin
  if (ThisToken.Id <> Id1) and (ThisToken.Id <> Id2) then
  begin
    writeln(StdErr, 'Expected tokens: ', Id1, ' or ', Id2,
            ', found ', ThisToken.Id, ' (', ThisToken.Value, ')');
    halt(1)
  end
end;

procedure ExpectAndGetNextToken(Id : TokenId);
begin
  ExpectToken(Id);
  GetNextToken()
end;

function GenerateTypeDefinition(Name: string) : boolean;
forward;

procedure GenerateEnum;
begin
  write(Output, 'enum { ');
  repeat
    GetNextToken();
    ExpectToken(TkIdentifier);
    write(Output, ThisToken.Value);
    GetNextToken();
    ExpectToken2(TkComma, TkRparen);
    if ThisToken.Id = TkComma then
      write(Output, ', ');
  until ThisToken.Id = TkRparen;
  write(Output, ' }');
end;

procedure GenerateStruct;
var 
  Name: string;
begin
  write(Output, 'struct { ');
  repeat
    GetNextToken();
    ExpectToken(TkIdentifier);
    Name := ThisToken.Value;
    GetNextToken();
    ExpectAndGetNextToken(TkColon);
    if not GenerateTypeDefinition(Name) then
      write(Output, ' ', Name);
    if ThisToken.Id <> TkEnd then
      write(Output, ', ');
  until ThisToken.Id = TkEnd;
  write(Output, '}');
end;

procedure GenerateArray(Name: string);
var 
  LowBound: string;
  HighBound: string;
begin
  ExpectAndGetNextToken(TkArray);
  ExpectAndGetNextToken(TkLbracket);
  ExpectToken(TkNumber);
  LowBound := ThisToken.Value;
  GetNextToken();
  ExpectAndGetNextToken(TkRange);
  ExpectToken(TkNumber);
  HighBound := ThisToken.Value;
  GetNextToken();
  ExpectAndGetNextToken(TkRbracket);
  ExpectAndGetNextToken(TkOf);
  if not GenerateTypeDefinition(Name) then
    write(Output, ' ', Name);
  write(Output, '[/*', LowBound, ' ..*/ ', HighBound, ' + 1]');
end;

procedure GenerateString(Value: string);
var 
  InStr: boolean;
  LastQuote: boolean;
  Pos: integer;
  Chr: char;
begin
  InStr := false;
  LastQuote := false;
  write(Output, '"');
  for Pos := 1 to Length(Value) do
    begin
      Chr := Value[Pos];
      if Chr = '''' then
      begin
        InStr := not InStr;
        if InStr and LastQuote then
          write(Output, '''');
        LastQuote := not InStr
      end
      else
        write(Output, Chr);
    end;
  write(Output, '"');
end;

procedure GenerateNumber(Value: string);
begin
  write(Output, Value);
end;

procedure GenerateIdentifier(Name: string);
begin
  if Name = 'TRUE' then
    Name := 'PTrue'
  else if Name = 'FALSE' then
         Name := 'PFalse';
  write(Output, Name)
end;

procedure GenerateTypeName(Name: string);
begin
  if Name = 'INTEGER' then
    Name := 'int'
  else if Name = 'BOOLEAN' then
         Name := 'PBoolean'
  else if Name = 'STRING' then
         Name := 'PString';
  write(Output, Name)
end;

function GenerateTypeDefinition(Name: string): boolean;
begin
  GenerateTypeDefinition := false;
  if ThisToken.Id = TkLparen then
  begin
    GenerateEnum();
    GetNextToken()
  end
  else if ThisToken.Id = TkRecord then
  begin
    GenerateStruct();
    GetNextToken()
  end
  else if ThisToken.Id = TkArray then
  begin
    GenerateArray(Name);
    GenerateTypeDefinition := true;
  end
  else if ThisToken.Id = TkIdentifier then
  begin
    GenerateTypeName(ThisToken.Value);
    GetNextToken()
  end
  else
  begin
    writeln(StdErr, 'Expected tokens: ', TkLparen, ', ', TkRecord, ' or ',
            TkIdentifier,
            ', found ', ThisToken.Id, ' (', ThisToken.Value, ')');
    halt(1)
  end;
end;

procedure GenerateTypes;
var 
  Name : string;
begin
  ExpectAndGetNextToken(TkType);
  writeln(Output, '/* Types */');
  while ThisToken.Id = TkIdentifier do
  begin
    Name := ThisToken.Value;
    write(Output, 'typedef ');
    GetNextToken();
    ExpectAndGetNextToken(TkEquals);
    if not GenerateTypeDefinition(Name) then
      write(Output, ' ', Name);
    writeln(Output, ';');
    ExpectAndGetNextToken(TkSemicolon);
  end;
end;

procedure GenerateConstant;
begin
  if ThisToken.Id = TkNumber then
    GenerateNumber(ThisToken.Value)
  else if ThisToken.Id = TkString then
         GenerateString(ThisToken.Value)
  else if ThisToken.Id = TkIdentifier then
         GenerateIdentifier(ThisToken.Value)
  else
  begin
    writeln(StdErr, 'Expected token: ', TkNumber, ', ', TkString, ', or ',
            TkIdentifier, ', found ', ThisToken.Id,
            ' (', ThisToken.Value, ')');
    halt(1)
  end
end;

procedure GenerateConstantInitializer;
begin
  if ThisToken.Id = TkLparen then
  begin
    write(Output, '{ ');
    repeat
      GetNextToken();
      GenerateConstantInitializer();
      GetNextToken();
      ExpectToken2(TkComma, TkRparen);
      if ThisToken.Id = TkComma then
        write(Output, ', ');
    until ThisToken.Id = TkRparen;
    write(Output, ' }');
    GetNextToken();
  end
  else
    GenerateConstant();
end;

procedure GenerateConsts;
var 
  Name : string;
begin
  ExpectAndGetNextToken(TkConst);
  writeln(Output, '/* Consts */');
  while ThisToken.Id = TkIdentifier do
  begin
    Name := ThisToken.Value;
    write(Output, 'const ');
    GetNextToken();
    ExpectAndGetNextToken(TkColon);
    if not GenerateTypeDefinition(Name) then
      write(Output, ' ', Name);
    ExpectAndGetNextToken(TkEquals);
    write(Output, ' = ');
    GenerateConstantInitializer();
    ExpectAndGetNextToken(TkSemicolon);
    writeln(Output, ';');
  end;
end;

procedure GenerateVars;
var
  Name : string;
begin
  ExpectAndGetNextToken(TkVar);
  writeln(Output, '/* Vars */');
  while ThisToken.Id = TkIdentifier do
  begin
    Name := ThisToken.Value;
    GetNextToken();
    ExpectAndGetNextToken(TkColon);
    if not GenerateTypeDefinition(Name) then
      write(Output, ' ', Name);
    ExpectAndGetNextToken(TkSemicolon);
    writeln(Output, ';');
  end;
end;

procedure GenerateDefinitions;
begin
  while ThisToken.Id <> TkBegin do
  begin
    if ThisToken.Id = TkType then
      GenerateTypes()
    else if ThisToken.Id = TkConst then
           GenerateConsts()
    else if ThisToken.Id = TkVar then
    GenerateVars()
    else
      ExpectToken(TkBegin);
  end;
end;

procedure GenerateMainBlock;
begin
  GenerateDefinitions();
end;

procedure GenerateProgram;
begin
  ExpectAndGetNextToken(TkProgram);
  ExpectToken(TkIdentifier);
  write(Output, '/* Program: ', ThisToken.Value);
  GetNextToken();
  if ThisToken.Id = TkLparen then
  begin
    write(Output, ' (');
    repeat
      GetNextToken();
      ExpectToken(TkIdentifier);
      write(Output, ThisToken.Value);
      GetNextToken();
      ExpectToken2(TkComma, TkRparen);
      if ThisToken.Id = TkComma then
        write(Output, ', ');
    until ThisToken.Id = TkRparen;
    write(Output, ')');
  end;
  GetNextToken();
  ExpectAndGetNextToken(TkSemicolon);
  writeln(Output, ' */');

  writeln(Output, '#include "runtime.h"');

  GenerateMainBlock();
end;

begin
  GetNextToken();
  GenerateProgram();
end.
