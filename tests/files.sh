#!/bin/bash

. ./testing.libsh

# Text file output
testtextout() {
  echo "program foo; var i : string; f : text;
        begin assign(f, paramstr(1)); rewrite(f); $1 ; close(f) end."
}
testtextout 'readln(i); write(f, i); readln(i); write(f, i)' |
inputs_and_writes 'abcde
fghi' 'abcdefghi'
testtextout 'readln(i); write(f, i); writeln(f);
             readln(i); write(f, i); writeln(f)' |
inputs_and_writes 'abcde
fghi' 'abcde
fghi
'
testtextout 'readln(i); writeln(f, i); readln(i); writeln(f, i)' |
inputs_and_writes 'abcde
fghi' 'abcde
fghi
'

# Text file input
testtextin() {
  echo "program foo; var i : string; f : text;
        begin assign(f, paramstr(1)); reset(f); $1 ; close(f) end."
}
testtextin 'read(f, i); writeln(i); read(f, i); writeln(i)' |
reads_and_outputs 'abcde
fghi' 'abcde

'
testtextin 'read(f, i); writeln(i); readln(f); read(f, i); writeln(i)' |
reads_and_outputs 'abcde
fghi' 'abcde
fghi
'
testtextin 'readln(f, i); writeln(i); readln(f, i); writeln(i)' |
reads_and_outputs 'abcde
fghi' 'abcde
fghi
'

# Binary file input/output
testfile() {
  echo "program foo; type t = $1; var i : t; f : file of t;
        begin assign(f, 'testfile');
        rewrite(f); $2; close(f);
        reset(f); $3 ; close(f);
        erase(f) end."
}
testfile 'integer' 'write(f, 123456)' 'read(f, i); write(i)' | outputs '123456'
testfile 'integer' 'i := 123456; write(f, 123456); i := 0' \
                   'read(f, i); write(i)' | outputs '123456'

testfile 'record a:integer; b:boolean end' \
         'i.a := 123456; i.b := true; write(f, i)' \
         "i.a := 0; i.b := false; read(f, i); write(i.a, ' ', i.b)" |
outputs '123456 TRUE'

# Directory subroutines
echo "{\$I-}
      program foo;
      var cwd : string;
      begin GetDir(0, cwd); write(cwd) end." | outputs "$(pwd)"
echo "{\$I-}
      program foo;
      var cwd : string;
      begin
        MkDir('testdir');
        ChDir('testdir');
        GetDir(0, cwd);
        ChDir('..');
        RmDir('testdir');
        write(cwd)
      end." | outputs "$(pwd)/testdir"

# Erase
echo "{\$I-}
      program foo;
      var f : text;
      begin
        assign(f, 'testfile');
        rewrite(f);
        close(f);
        reset(f);
        write(IoResult, '(0) ');
        close(f);
        erase(f);
        write(IoResult, '(0) ');
        reset(f);
        write(IoResult, '(2)')
      end." | outputs '0(0) 0(0) 2(2)'

# Rename
echo "{\$I-}
      program foo;
      var f : text;
      begin
        assign(f, 'testfile1');
        rewrite(f);
        close(f);
        rename(f, 'testfile2');
        assign(f, 'testfile1');
        reset(f);
        write(IoResult, '(2) ');
        assign(f, 'testfile2');
        reset(f);
        write(IoResult, '(0)');
        close(f);
        erase(f)
      end." | outputs '2(2) 0(0)'

# Text files
echo "{\$I-}
      program foo;
      var f : text; s : string;
      begin
        assign(f, 'testfile');
        rewrite(f);
        writeln(f, 'Text');
        writeln(f, 2);
        writeln(f, true);
        close(f);
        reset(f);
        readln(f, s);
        write(s, ' ');
        readln(f, s);
        write(s, ' ');
        readln(f, s);
        write(s);
        close(f);
        erase(f)
      end." | outputs "Text 2 TRUE"
# Eoln
echo "{\$I-}
      program foo;
      var f : text; s : string; i : integer;
      begin
        assign(f, 'testfile');
        rewrite(f);
        writeln(f, 'This is line 1.');
        write(f, '2 ');
        close(f);
        reset(f);
        write(eoln(f));
        read(f, s);
        write(eoln(f));
        readln(f);
        write(eoln(f));
        read(f, i);
        write(eoln(f));
        readln(f);
        write(eoln(f));
        close(f);
        erase(f)
      end." | outputs 'FALSETRUEFALSEFALSETRUE'
# SeekEoln
echo "{\$I-}
      program foo;
      var f : text; s : string; i : integer;
      begin
        assign(f, 'testfile');
        rewrite(f);
        writeln(f, 'This is line 1.');
        write(f, '2 ');
        close(f);
        reset(f);
        write(seekeoln(f));
        read(f, s);
        write(seekeoln(f));
        readln(f);
        write(seekeoln(f));
        read(f, i);
        write(seekeoln(f));
        readln(f);
        write(seekeoln(f));
        close(f);
        erase(f)
      end." | outputs 'FALSETRUEFALSETRUETRUE'
# Eof
echo "{\$I-}
      program foo;
      var f : text; s : string; i : integer;
      begin
        assign(f, 'testfile');
        rewrite(f);
        writeln(f, 'This is line 1.');
        write(f, '2 ');
        close(f);
        reset(f);
        write(eof(f));
        read(f, s);
        write(eof(f));
        readln(f);
        write(eof(f));
        read(f, i);
        write(eof(f));
        readln(f);
        write(eof(f));
        close(f);
        erase(f)
      end." | outputs 'FALSEFALSEFALSEFALSETRUE'
# SeekEof
echo "{\$I-}
      program foo;
      var f : text; s : string; i : integer;
      begin
        assign(f, 'testfile');
        rewrite(f);
        writeln(f, 'This is line 1.');
        write(f, '2 ');
        close(f);
        reset(f);
        write(seekeof(f));
        read(f, s);
        write(seekeof(f));
        readln(f);
        write(seekeof(f));
        read(f, i);
        write(seekeof(f));
        readln(f);
        write(seekeof(f));
        close(f);
        erase(f)
      end." | outputs 'FALSEFALSEFALSETRUETRUE'
# Seek/FilePos/FileSize
echo "{\$I-}
      program foo;
      var f : text; s : string;
      begin
        assign(f, 'testfile');
        rewrite(f);
        writeln(f, 'This is line 1.');
        write(f, 'This is line 2.');
        close(f);
        reset(f);
        write(filesize(f), '(31) ');
        write(filepos(f), '(0) ');
        readln(f, s);
        write(filepos(f), '(16) ');
        seek(f, 8);
        write(filepos(f), '(8) ');
        readln(f, s);
        write(s, '(line 1.)');
        close(f);
        erase(f)
      end." | outputs '31(31) 0(0) 16(16) 8(8) line 1.(line 1.)'

# Passive I/O error checking
echo "{\$I-}
      program foo;
      var f : text; r : integer;
      begin
        assign(f, 'testfile');
        erase(f);
        r := ioresult;
        write('Continues ', r)
      end." | outputs 'Continues 2'
# Active I/O error checking
echo "{\$I+}
      program foo;
      var f : text; r : integer;
      begin
        assign(f, 'testfile');
        erase(f);
        r := ioresult;
        write('Continues ', r)
      end." | outputs '' 2>/dev/null
