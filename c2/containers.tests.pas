{ Unit tests for bytebuffer.pas }
program containers_tests;

{$I utils.inc.pas}
{$I containers.inc.pas}
{$I tests.inc.pas}

procedure Test_TList;
type
    testnode = record
        next : ^testnode;
        value : integer;
    end;
var
    list, elem : ^testnode;
    i : integer;
begin
    TList_New(list);
    Assert(list = nil, 'list pointer was not nil after ''new'', got ' + PtrToStr(list));
    TList_First(list, elem);
    Assert(elem = nil, 'first element was not nil, got ' + PtrToStr(elem));

    for i := 1 to 10 do
    begin
        New(elem);
        elem^.value := i;
        TList_Push(list, elem)
    end;
    i := 1;
    TList_First(list, elem);
    while elem <> nil do
    begin
        AssertEqInteger(i, elem^.value, 'iterated list element mismatch');
        TList_Next(list, elem);
        i := Succ(i)
    end;
    AssertEqInteger(11, i, 'wrong number of elements in list');

    i := 1;
    TList_Shift(list, elem);
    while elem <> nil do
    begin
        AssertEqInteger(i, elem^.value, 'shifted list element mismatch');
        Dispose(elem);
        i := Succ(i);
        TList_Shift(list, elem)
    end;

    Assert(list = nil, 'list pointer was not nil after shifts, got ' + PtrToStr(list));
end;

begin
    RunTest('Test_TList', @Test_TList);
end.
