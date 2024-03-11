program lexer_tests;

{$I ../types.inc.pas}
{$I ../utils.inc.pas}
{$I ../containers.inc.pas}
{$I ../bytebuffer.inc.pas}
{$I lexer.inc.pas}
{$I ../tests.inc.pas}

procedure AssertToken(lexer: TLexer; TokenId: TTokenId; Content : string);
var
    tkn : TToken;
    exp, act : string;
begin
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
    end;
    TLexer_NextToken(lexer)
end;

procedure ReadEmptyFile;
var lexer: TLexer;
begin
    lexer := TLexer_New('testdata/empty.txt', ByteBuffer_New(0));
    AssertToken(lexer, TkEof, '');
    TLexer_Dispose(lexer)
end;

procedure ReadTokens;
var
    bbb: ByteBufferBuilder;
    lexer: TLexer;
begin
    bbb := ByteBufferBuilder_New;
    ByteBufferBuilder_AddString(bbb, '   '#10#9'   '#10);
    ByteBufferBuilder_AddString(bbb, '%! ');
    ByteBufferBuilder_AddString(bbb, '123 $12aBfF 123.45 1.23e45 1.23e-45 1e+23 ');
    ByteBufferBuilder_AddString(bbb, 'foo bar alondra_123FOOBAR 123foobar ');
    ByteBufferBuilder_AddString(bbb, '''foo'' ''b''''ar'' ''''''bar'''''' #10 #13#10 #$0d#$0A ''foo''^n''bar'' ');
    ByteBufferBuilder_AddString(bbb, '+ - * / ( ) ');
    ByteBufferBuilder_AddString(bbb, '= < > <= >= <> ');
    ByteBufferBuilder_AddString(bbb, ':= [ ] . ^ @ ');
    ByteBufferBuilder_AddString(bbb, '.. , : ; ');
    ByteBufferBuilder_AddString(bbb, '(* comment *) ');
    ByteBufferBuilder_AddString(bbb, '{ comment } ');
    ByteBufferBuilder_AddString(bbb, 'ABSOLUTE and ARRAY begin CASE Const Div DO downto ELSE end FILE for ');
    ByteBufferBuilder_AddString(bbb, 'fOrWaRd function goto IF IN label mod nil NOT OF OR PACKED PROCEDURE ');
    ByteBufferBuilder_AddString(bbb, 'PROGRAM RECORD REPEAT SET SHL SHR THEN TO TYPE UNTIL VAR WHILE WITH XOR ');
    ByteBufferBuilder_AddString(bbb, 'END.');

    lexer := TLexer_New('testdata/tokens.txt', ByteBufferBuilder_Build(bbb));
    AssertToken(lexer, TkBlank, '   '#10#9'   '#10);
    AssertToken(lexer, TkError, '%');
    AssertToken(lexer, TkError, '!');
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIdentifier, 'foo');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIdentifier, 'bar');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkIdentifier, 'alondra_123FOOBAR');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkInteger, '123');
    AssertToken(lexer, TkIdentifier, 'foobar');
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkRange, '..');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkComma, ',');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkColon, ':');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkSemicolon, ';');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkComment, '(* comment *)');
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkComment, '{ comment }');
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
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
    AssertToken(lexer, TkBlank, ' ');
    AssertToken(lexer, TkEnd, 'END');
    AssertToken(lexer, TkDot, '.');
    AssertToken(lexer, TkEof, '');
end;

begin
    RunTest('ReadEmptyFile', @ReadEmptyFile);
    RunTest('ReadTokens', @ReadTokens);
end.