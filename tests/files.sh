#!/bin/bash

. ./testing.libsh

# Directory subroutines
echo "program foo;
      var cwd : string;
      begin GetDir(0, cwd); write(cwd) end." | outputs "$(pwd)"
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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
echo "program foo;
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

