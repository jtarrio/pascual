type
    TestProc = procedure;
    TestError = ^TestErrorObj;
    TestErrorObj = record
        Message: String;
        Next: ^TestErrorObj
    end;

var
    TestErrors: TestError;
    LastError: TestError;

procedure RunTest(name: string; test: TestProc);
var error : TestError;
begin
    TestErrors := nil;
    LastError := nil;
    test();
    write(name, ' ');
    if TestErrors = nil then
        writeln('OK')
    else
    begin
        writeln('FAILED');
        while TestErrors <> nil do
        begin
            error := TestErrors;
            TestErrors := TestErrors^.Next;
            writeln(error^.Message);
            Dispose(error)
        end
    end
end;

procedure Fail(message: string);
var error : TestError;
begin
    New(error);
    error^.Message := message;
    error^.Next := nil;
    if TestErrors = nil then
        TestErrors := error
    else
        LastError^.Next := error;
    LastError := error
end;

procedure Assert(value: boolean; message: string);
begin
    if not value then Fail('Condition did not hold: ' + message)
end;

procedure AssertEqInteger(expected, actual: integer; message: string);
begin
    if expected <> actual then Fail(message + '; expected ' + IntToStr(expected) + ', got ' + IntToStr(actual))
end;

procedure AssertEqChar(expected, actual: char; message: string);
begin
    if expected <> actual then Fail(message + '; expected ' + EscapeChar(expected) + ', got ' + EscapeChar(actual))
end;

procedure AssertEqStr(expected, actual: string; message: string);
begin
    if expected <> actual then Fail(message + '; expected ''' + expected + ''', got ''' + actual + '''')
end;
