{ A position within a file. }
type
    TPos = record
        { Count of bytes from the beginning (0-based). }
        Offset: integer;
        { Row number (1-based). }
        Row: integer;
        { Column number (1-based). }
        Col: integer
    end;

{ Returns position zero. }
function TPos_Zero: TPos;
begin
    Result.Offset := 0;
    Result.Row := 1;
    Result.Col := 1
end;

{ Advances the position by one character. }
procedure TPos_Advance(var Pos: TPos);
begin
    Pos.Offset := Pos.Offset + 1;
    Pos.Col := Pos.Col + 1
end;

{ Advances the position by one tabulation. }
procedure TPos_Tab(var Pos: TPos);
const TabSize = 4;
begin
    Pos.Offset := Pos.Offset + 1;
    Pos.Col := 1 + TabSize * (1 + (Pos.Col - 1) div TabSize)
end;

{ Advances the position by one newline. }
procedure TPos_NewLine(var Pos: TPos);
begin
    Pos.Offset := Pos.Offset + 1;
    Pos.Row := Pos.Row + 1;
    Pos.Col := 1
end;

{ A block of bytes within a file. }
type
    TBlock = record
        { Count of bytes from the beginning (0-based). }
        Offset: integer;
        { Size of the block in bytes. }
        Size : integer;
        { Row number (1-based). }
        Row: integer;
        { Column number (1-based). }
        Col: integer;
    end;

{ Returns a zero-length block starting at position zero. }
function TBlock_Zero: TBlock;
begin
    Result.Offset := 0;
    Result.Size := 0;
    Result.Row := 1;
    Result.Col := 1
end;

{ Returns a block starting at the given position with the given size. }
function TBlock_Make(const Pos : TPos; Size: integer): TBlock;
begin
    Result.Offset := Pos.Offset;
    Result.Size := Size;
    Result.Row := Pos.Row;
    Result.Col := Pos.Col
end;