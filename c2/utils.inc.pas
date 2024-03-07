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

