type
    TPos = record
        Offset: integer;
        Row: integer;
        Col: integer
    end;

procedure TPos_Reset(var Pos: TPos);
begin
    Pos.Offset := 0;
    Pos.Row := 1;
    Pos.Col := 1
end;

procedure TPos_Advance(var Pos: TPos);
begin
    Pos.Offset := Pos.Offset + 1;
    Pos.Col := Pos.Col + 1
end;

procedure TPos_Tab(var Pos: TPos);
const TabSize = 4;
begin
    Pos.Offset := Pos.Offset + 1;
    Pos.Col := 1 + TabSize * (1 + (Pos.Col - 1) div TabSize)
end;

procedure TPos_NewLine(var Pos: TPos);
begin
    Pos.Offset := Pos.Offset + 1;
    Pos.Row := Pos.Row + 1;
    Pos.Col := 1
end;
