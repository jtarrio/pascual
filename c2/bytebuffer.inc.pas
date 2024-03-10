{ Dynamically-sized buffers that contain bytes. }

type
    { A piece of memory that contains bytes. }
    ByteBuffer = record
        { Pointer to the memory. }
        Ptr : ^char;
        { Size of the buffer. }
        Size : integer;
    end;

{ Creates a new buffer with the given size. The buffer is NOT cleared. }
function ByteBuffer_New(Size : integer) : ByteBuffer;
begin
    Result.Size := Size;
    GetMem(Result.Ptr, Size)
end;

{ Copies the given string into a new buffer. }
function ByteBuffer_FromString(const Data : string) : ByteBuffer;
var
    Ptr : ^char;
    i : integer;
begin
    Result := ByteBuffer_New(Length(Data));
    Ptr := Result.Ptr;
    for i := 1 to Result.Size do
    begin
        Ptr^ := Data[i];
        Ptr := Succ(Ptr)
    end
end;

{ Releases the memory used by the given buffer. }
procedure ByteBuffer_Dispose(var bb : ByteBuffer);
begin
    FreeMem(bb.Ptr, bb.Size);
    bb.Size := 0;
end;

{ Reads a character from the buffer.
  Offset: the position in the buffer (the first byte has position 0).
  If the offset goes beyond bounds, this function returns #0. }
function ByteBuffer_GetChar(const bb : ByteBuffer; Offset : integer) : char;
var Ptr : ^char;
begin
    if (Offset < 0) or (Offset >= bb.Size) then
        Result := #0
    else
    begin
        Ptr := bb.Ptr + Offset;
        Result := Ptr^
    end
end;

{ Writes a character into the buffer.
  Chr: the character to write.
  Offset: the position in the buffer (the first byte has position 0).
  If the offset goes beyond bounds, nothing is written. }
procedure ByteBuffer_SetChar(var bb : ByteBuffer; Chr : char; Offset : integer);
var Ptr : ^char;
begin
    if (Offset >= 0) and (Offset < bb.Size) then
    begin
        Ptr := bb.Ptr + Offset;
        Ptr^ := Chr
    end
end;

{ Reads a string from the buffer.
  Offset: the position in the buffer (the first byte has position 0).
  Size: the size of the string to read.
  If the offset or size go beyond bounds, the out-of-bounds bytes are not read.
  For example, if you set Offset=-3 and Size=10, it is the same as Offset=0 and Size=7. }
function ByteBuffer_GetString(const bb : ByteBuffer; Offset, Size : integer) : string;
var
    Ptr : ^char;
    i : integer;
begin
    if Offset < 0 then
    begin
        Size := Size + Offset;
        Offset := 0
    end;
    if Offset > bb.Size then Offset := bb.Size;
    if Size < 0 then Size := 0;
    if Size > 255 then Size := 255;
    if Offset + Size > bb.Size then Size := bb.Size - Offset;
    Result[0] := Chr(Size);
    Ptr := bb.Ptr + Offset;
    for i := 1 to Size do
    begin
        Result[i] := Ptr^;
        Ptr := Succ(Ptr)
    end
end;

{ Writes a string to the buffer.
  Str: the string to write.
  Offset: the position in the buffer (the first byte has position 0).
  If the offset or size go beyond bounds, the out-of-bounds bytes are not written.
  For example, if you set Offset=-3 and Size=10, it is the same as Offset=0 and Size=7. }
procedure ByteBuffer_SetString(var bb : ByteBuffer; const Str : string; Offset : integer);
var
    Ptr : ^char;
    Size, Start, i : integer;
begin
    Start := 1;
    Size := Length(Str);
    if Offset < 0 then
    begin
        Size := Size + Offset;
        Start := 1 - Offset;
        Offset := 0
    end;
    if Offset > bb.Size then Offset := bb.Size;
    if Size < 0 then Size := 0;
    if Size > 255 then Size := 255;
    if Offset + Size > bb.Size then Size := bb.Size - Offset;
    Ptr := bb.Ptr + Offset;
    for i := Start to Start + Size - 1 do
    begin
        Ptr^ := Str[i];
        Ptr := Succ(Ptr)
    end
end;

type
    { A builder for ByteBuffers, that lets you create a large one piecewise without knowing its final size. }
    ByteBufferBuilder = ^ByteBufferBuilderObj;
    ByteBufferBuilderObj = record
        { This piece of data. }
        Piece : ByteBuffer;
        { The size of the entire buffer. }
        Size : integer;
        { Pointer to the first piece. }
        First : ByteBufferBuilder;
        { Pointer to the next piece. }
        Next : ByteBufferBuilder;
    end;

{ Returns a new builder. }
function ByteBufferBuilder_New : ByteBufferBuilder;
begin
    Result := nil
end;

{ Adds a string to the builder. }
procedure ByteBufferBuilder_AddString(var bb : ByteBufferBuilder; Data : string);
var Next : ByteBufferBuilder;
begin
    New(Next);
    Next^.Piece := ByteBuffer_FromString(Data);
    Next^.Next := nil;
    if bb = nil then
    begin
        Next^.First := Next;
        Next^.Size := Next^.Piece.Size
    end
    else
    begin
        Next^.First := bb^.First;
        Next^.Size := bb^.Size + Next^.Piece.Size;
        bb^.Next := Next;
    end;
    bb := Next
end;

{ Returns a ByteBuffer built from all the pieces. }
function ByteBufferBuilder_Build(var bb : ByteBufferBuilder) : ByteBuffer;
var
    Prev, Next : ByteBufferBuilder;
    InPtr, OutPtr : ^char;
    i : integer;
begin
    if bb = nil then Result := ByteBuffer_New(0)
    else
    begin
        Result := ByteBuffer_New(bb^.Size);
        OutPtr := Result.Ptr;
        Next := bb^.First;
        repeat
            InPtr := Next^.Piece.Ptr;
            for i := 1 to Next^.Piece.Size do
            begin
                OutPtr^ := InPtr^;
                OutPtr := Succ(OutPtr);
                InPtr := Succ(InPtr)
            end;
            ByteBuffer_Dispose(Next^.Piece);
            Prev := Next;
            Next := Next^.Next;
            dispose(Prev)
        until Next = nil;
    end
end;

type
    { Dummy type to pass a binary file as an argument. }
    _bbBinaryFile = file of char;

{ Reads the given binary file into a new buffer. }
function ByteBuffer_FromBinaryFile(f : _bbBinaryFile) : ByteBuffer;
var
    i : integer;
    Ptr : ^char;
    Err : boolean;
begin
    Err := false;
    Result := ByteBuffer_New(FileSize(f));
    Ptr := Result.Ptr;
    for i := 1 to Result.Size do
    begin
        if not err then
        begin
            Read(f, Ptr^);
            err := err or (IoResult <> 0)
        end;
        Ptr := Succ(Ptr)
    end
end;

{ Reads the given text file into a new buffer. }
function ByteBuffer_FromTextFile(f : text) : ByteBuffer;
var
    bbb : ByteBufferBuilder;
    line : string;
    Err : boolean;
begin
    Err := false;
    bbb := ByteBufferBuilder_New;
    while (not Eof(f)) and (not Err) do
    begin
        Readln(f, line);
        err := err or (IoResult <> 0);
        if not err then
            ByteBufferBuilder_AddString(bbb, line + #10)
    end;
    Result := ByteBufferBuilder_Build(bbb)
end;
