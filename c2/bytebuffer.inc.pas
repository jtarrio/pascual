{ Dynamically-sized buffers that contain bytes. }

{ Requires: containers.inc.pas }

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
        _ : TListNode;
        { This piece of data. }
        Piece : ByteBuffer;
        { The size of the entire buffer. }
        Size : integer;
        { Pointer to the first piece. }
        First : ByteBufferBuilder;
    end;

{ Returns a new builder. }
function ByteBufferBuilder_New : ByteBufferBuilder;
begin
    TList_New(Result)
end;

{ Adds a string to the builder. }
procedure ByteBufferBuilder_AddString(var bb : ByteBufferBuilder; Data : string);
var Elem : ByteBufferBuilder;
begin
    New(Elem);
    Elem^.Piece := ByteBuffer_FromString(Data);
    if bb <> nil then Elem^.Size := bb^.Size;
    Elem^.Size := Elem^.Size + Elem^.Piece.Size;
    TList_Push_Back(bb, Elem)
end;

{ Returns a ByteBuffer built from all the pieces. }
function ByteBufferBuilder_Build(var bb : ByteBufferBuilder) : ByteBuffer;
var
    Elem : ByteBufferBuilder;
    InPtr, OutPtr : ^char;
    i : integer;
begin
    if bb = nil then Result := ByteBuffer_New(0)
    else
    begin
        Result := ByteBuffer_New(bb^.Size);
        OutPtr := Result.Ptr;
        TList_Pop_Front(bb, Elem);
        while Elem <> nil do
        begin
            InPtr := Elem^.Piece.Ptr;
            for i := 1 to Elem^.Piece.Size do
            begin
                OutPtr^ := InPtr^;
                OutPtr := Succ(OutPtr);
                InPtr := Succ(InPtr)
            end;
            ByteBuffer_Dispose(Elem^.Piece);
            dispose(Elem);
            TList_Pop_Front(bb, Elem)
        end
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

{ Outputs a portion of the buffer into the given text file.
  f: the output file.
  Offset: the position in the buffer (the first byte has position 0).
  Size: the size of the string to read.
  If the offset or size go beyond bounds, the out-of-bounds bytes are not read.
  For example, if you set Offset=-3 and Size=10, it is the same as Offset=0 and Size=7. }
procedure ByteBuffer_ToTextFile_Partial(const bb : ByteBuffer; f : text; Offset, Size : integer);
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
    if Offset + Size > bb.Size then Size := bb.Size - Offset;
    Ptr := bb.Ptr + Offset;
    for i := 1 to Size do
    begin
        write(f, Ptr^);
        Ptr := Succ(Ptr)
    end
end;

{ Outputs the buffer into the given text file.
  f: the output file. }
procedure ByteBuffer_ToTextFile(const bb : ByteBuffer; f : text);
begin
    ByteBuffer_ToTextFile_Partial(bb, f, 0, bb.Size)
end;
