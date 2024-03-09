{ Unit tests for bytebuffer.pas }
program bytebuffer_tests;

{$I utils.inc.pas}
{$I bytebuffer.inc.pas}
{$I tests.inc.pas}

procedure Test_ByteBuffer_GetChar;
var
    bb : ByteBuffer;
    i : integer;
begin
    bb := ByteBuffer_FromString('0123456789');
    for i := 0 to 9 do
        AssertEqChar(Chr(i + Ord('0')), ByteBuffer_GetChar(bb, i), 'wrong character for ' + IntToStr(i));
    ByteBuffer_Dispose(bb)
end;

procedure Test_ByteBuffer_SetChar;
var
    bb : ByteBuffer;
    i : integer;
begin
    bb := ByteBuffer_New(10);
    for i := -2 to 12 do
        ByteBuffer_SetChar(bb, Chr(i + Ord('a')), i);
    AssertEqStr('abcdefghij', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_Dispose(bb)
end;

procedure Test_ByteBuffer_GetString;
var bb : ByteBuffer;
begin
    bb := ByteBuffer_FromString('0123456789');
    AssertEqStr('', ByteBuffer_GetString(bb, -7, 6), 'wrong string for -7, 6');
    AssertEqStr('', ByteBuffer_GetString(bb, -6, 6), 'wrong string for -6, 6');
    AssertEqStr('01', ByteBuffer_GetString(bb, -4, 6), 'wrong string for -4, 6');
    AssertEqStr('0123', ByteBuffer_GetString(bb, -2, 6), 'wrong string for -2, 6');
    AssertEqStr('012345', ByteBuffer_GetString(bb, 0, 6), 'wrong string for 0, 6');
    AssertEqStr('234567', ByteBuffer_GetString(bb, 2, 6), 'wrong string for 2, 6');
    AssertEqStr('456789', ByteBuffer_GetString(bb, 4, 6), 'wrong string for 4, 6');
    AssertEqStr('6789', ByteBuffer_GetString(bb, 6, 6), 'wrong string for 6, 6');
    AssertEqStr('89', ByteBuffer_GetString(bb, 8, 6), 'wrong string for 8, 6');
    AssertEqStr('', ByteBuffer_GetString(bb, 10, 6), 'wrong string for 10, 6');
    AssertEqStr('', ByteBuffer_GetString(bb, 11, 6), 'wrong string for 11, 6');
    ByteBuffer_Dispose(bb)
end;

procedure Test_ByteBuffer_SetString;
var bb : ByteBuffer;
begin
    bb := ByteBuffer_FromString('----------');
    ByteBuffer_SetString(bb, 'VWXYZ', -7);
    AssertEqStr('----------', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_SetString(bb, '12345', -3);
    AssertEqStr('45--------', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_SetString(bb, 'abcde', 2);
    AssertEqStr('45abcde---', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_SetString(bb, 'ABCDE', 7);
    AssertEqStr('45abcdeABC', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_SetString(bb, 'vwxyz', 12);
    AssertEqStr('45abcdeABC', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_Dispose(bb)
end;

procedure Test_ByteBufferBuilder;
var
    bbb : ByteBufferBuilder;
    bb : ByteBuffer;
begin
    bbb := ByteBufferBuilder_New;
    ByteBufferBuilder_AddString(bbb, '0123');
    ByteBufferBuilder_AddString(bbb, '4');
    ByteBufferBuilder_AddString(bbb, '56789');
    bb := ByteBufferBuilder_Build(bbb);
    AssertEqStr('0123456789', ByteBuffer_GetString(bb, 0, 10), 'wrong string');
    ByteBuffer_Dispose(bb)
end;

procedure Test_ByteBuffer_FromBinaryFile;
var
    f : file of char;
    bb : ByteBuffer;
    Actual : string;
const
    Expected : string = '{ Unit tests for bytebuffer.pas }'#10'program bytebuffer_tests;'#10;
begin
    Assign(f, 'bytebuffer.tests.pas');
    Reset(f);
    bb := ByteBuffer_FromBinaryFile(f);
    Close(f);
    Actual := ByteBuffer_GetString(bb, 0, Length(Expected));
    AssertEqStr(Expected, Actual, 'wrong start of the unit test file')
end;

procedure Test_ByteBuffer_FromTextFile;
var
    f : text;
    bb : ByteBuffer;
    Actual : string;
const
    Expected : string = '{ Unit tests for bytebuffer.pas }'#10'program bytebuffer_tests;'#10;
begin
    Assign(f, 'bytebuffer.tests.pas');
    Reset(f);
    bb := ByteBuffer_FromTextFile(f);
    Close(f);
    Actual := ByteBuffer_GetString(bb, 0, Length(Expected));
    AssertEqStr(Expected, Actual, 'wrong start of the unit test file')
end;

begin
    RunTest('Test_ByteBuffer_GetChar', @Test_ByteBuffer_GetChar);
    RunTest('Test_ByteBuffer_SetChar', @Test_ByteBuffer_SetChar);
    RunTest('Test_ByteBuffer_GetString', @Test_ByteBuffer_GetString);
    RunTest('Test_ByteBuffer_SetString', @Test_ByteBuffer_SetString);
    RunTest('Test_ByteBufferBuilder', @Test_ByteBufferBuilder);
    RunTest('Test_ByteBuffer_FromBinaryFile', @Test_ByteBuffer_FromBinaryFile);
    RunTest('Test_ByteBuffer_FromTextFile', @Test_ByteBuffer_FromTextFile);
end.