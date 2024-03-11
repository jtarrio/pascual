function EscapeChar(c: char): string;
begin
    if c > ' ' then
        Result := c
    else
    begin
        Str(Ord(c), Result);
        Result := '#' + Result
    end
end;

function IntToStr(i: integer): string;
begin
    Str(i, Result)
end;

function ChrToHex(c: char): string;
var n : integer;
begin
    Result := '00';
    n := Ord(c) and $0f;
    if n <= 9 then Result[2] := Chr(n + Ord('0'))
    else Result[2] := Chr(n + Ord('a') - 10);
    n := (Ord(c) shr 4) and $0f;
    if n <= 9 then Result[1] := Chr(n + Ord('0'))
    else Result[1] := Chr(n + Ord('a') - 10)
end;

function PtrToStr(p: pointer): string;
var
    value: record
        case boolean of
            true: (p: pointer);
            false: (b: array[1..8] of char);
        end;
    b: string;
    i: integer;
begin
    if p = nil then Result := 'nil'
    else
    begin
        result[0] := #16;
        value.p := p;
        for i := 1 to 8 do
        begin
            b := ChrToHex(value.b[i]);
            result[2 * (9 - i) - 1] := b[1];
            result[2 * (9 - i)] := b[2]
        end
    end
end;
