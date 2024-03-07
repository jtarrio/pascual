program lexer_tests;

{$I ../types.inc.pas}
{$I ../utils.inc.pas}
{$I lexer.inc.pas}
{$I ../tests.inc.pas}

procedure AssertEof(lexer: TLexer);
begin
    Assert(_TLexer_Eof(lexer), 'Expected EOF');
end;

procedure AssertNotEof(lexer: TLexer);
begin
    Assert(not _TLexer_Eof(lexer), 'Expected not EOF');
end;

procedure AssertGetChar(lexer: TLexer; c: char; row, col, offset: integer);
begin
    AssertEqChar(_TLexer_GetChar(lexer), c, '_TLexer_GetChar returned the wrong value');
    AssertEqInteger(lexer^.Pos.Row, row, 'Row does not match');
    AssertEqInteger(lexer^.Pos.Col, col, 'Column does not match');
    AssertEqInteger(lexer^.Pos.Offset, offset, 'Offset does not match')
end;

procedure AssertPeekChar(lexer: TLexer; c: char);
begin
    AssertEqChar(_TLexer_PeekChar(lexer), c, '_TLexer_PeekChar returned the wrong value')
end;

procedure AssertToken(lexer: TLexer; TokenId: TTokenId; Content : string);
var
    tkn : TToken;
    exp, act : string;
begin
    TLexer_NextToken(lexer);
    tkn := TLexer_Token(lexer);
    if TokenId <> tkn.Id then
    begin
        Str(TokenId, exp);
        Str(tkn.Id, act);
        if tkn.Id = TkError then
            Fail('Mismatch in token type; expected ' + exp + ', got ' + act + ': ' + lexer^.Error)
        else
            Fail('Mismatch in token type; expected ' + exp + ', got ' + act)
    end
    else
    begin
        act := _TLexer_ReadToken(lexer, tkn);
        AssertEqStr(Content, act, 'Mismatch in token content')
    end
end;

procedure ReadEmptyFile;
var lexer: TLexer;
begin
    lexer := TLexer_New('testdata/empty.txt');
    AssertEof(lexer);
    TLexer_Dispose(lexer)
end;

procedure ReadOneCharacter;
var lexer: TLexer;
begin
    lexer := TLexer_New('testdata/onechar.txt');
    AssertNotEof(lexer);
    _TLexer_NextChar(lexer);
    AssertEof(lexer);
    TLexer_Dispose(lexer)
end;

procedure GetCharacters;
var lexer: TLexer;
begin
    lexer := TLexer_New('testdata/content.txt');
    AssertNotEof(lexer);
    AssertGetChar(lexer, 'a', 1, 1, 0);
    AssertPeekChar(lexer, 'b');
    AssertPeekChar(lexer, 'b');
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, 'b', 1, 2, 1);
    AssertPeekChar(lexer, 'c');
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, 'c', 1, 3, 2);
    AssertPeekChar(lexer, #10);
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, #10, 1, 4, 3);
    AssertPeekChar(lexer, 'd');
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, 'd', 2, 1, 4);
    AssertPeekChar(lexer, #9);
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, #9, 2, 2, 5);
    AssertPeekChar(lexer, 'e');
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, 'e', 2, 5, 6);
    AssertPeekChar(lexer, #10);
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, #10, 2, 6, 7);
    AssertPeekChar(lexer, 'f');
    _TLexer_NextChar(lexer);
    AssertGetChar(lexer, 'f', 3, 1, 8);
    AssertPeekChar(lexer, #0);
    _TLexer_NextChar(lexer);
    AssertEof(lexer);
    TLexer_Dispose(lexer)
end;

procedure ReadTokens;
var lexer: TLexer;
begin
    lexer := TLexer_New('testdata/tokens.txt');
    AssertToken(lexer, TkInteger, '123');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkInteger, '$12aBfF');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkReal, '123.45');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkReal, '1.23e45');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkReal, '1.23e-45');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkReal, '1e+23');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkIdentifier, 'foo');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIdentifier, 'bar');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIdentifier, 'alondra_123FOOBAR');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkInteger, '123');
    AssertToken(lexer, TkIdentifier, 'foobar');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkString, '''foo''');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkString, '''b''''ar''');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkString, '''''''bar''''''');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkString, '#10');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkString, '#13#10');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkString, '#$0d#$0A');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkString, '''foo''^n''bar''');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkPlus, '+');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkMinus, '-');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkAsterisk, '*');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkSlash, '/');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkLparen, '(');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkRparen, ')');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkEquals, '=');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkLessthan, '<');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkMorethan, '>');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkLessOrEquals, '<=');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkMoreOrEquals, '>=');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkNotEquals, '<>');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkAssign, ':=');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkLbracket, '[');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkRbracket, ']');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkDot, '.');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkCaret, '^');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkAt, '@');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkRange, '..');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkComma, ',');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkColon, ':');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkSemicolon, ';');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkComment, '(* comment *)');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkComment, '{ comment }');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkAbsolute, 'ABSOLUTE');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkAnd, 'and');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkArray, 'ARRAY');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkBegin, 'begin');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkCase, 'CASE');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkConst, 'Const');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkDiv, 'Div');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkDo, 'DO');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkDownto, 'downto');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkElse, 'ELSE');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkEnd, 'end');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkFile, 'FILE');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkFor, 'for');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkForward, 'fOrWaRd');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkFunction, 'function');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkGoto, 'goto');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIf, 'IF');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIn, 'IN');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkLabel, 'label');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkMod, 'mod');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkNil, 'nil');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkNot, 'NOT');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkOf, 'OF');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkOr, 'OR');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkPacked, 'PACKED');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkProcedure, 'PROCEDURE');
    AssertToken(lexer, TkBlank, #10);
    AssertToken(lexer, TkProgram, 'PROGRAM');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkRecord, 'RECORD');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkRepeat, 'REPEAT');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkSet, 'SET');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkShl, 'SHL');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkShr, 'SHR');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkThen, 'THEN');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkTo, 'TO');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkType, 'TYPE');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkUntil, 'UNTIL');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkVar, 'VAR');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkWhile, 'WHILE');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkWith, 'WITH');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkXor, 'XOR');
    AssertToken(lexer, TkBlank, #10'   '#10);
    AssertToken(lexer, TkEnd, 'END');
    AssertToken(lexer, TkDot, '.');
    AssertToken(lexer, TkEof, '');
end;

begin
    RunTest('ReadEmptyFile', @ReadEmptyFile);
    RunTest('ReadOneCharacter', @ReadOneCharacter);
    RunTest('GetCharacters', @GetCharacters);
    RunTest('ReadTokens', @ReadTokens);
end.