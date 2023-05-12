/* Program: TEXTFILES */
#include "pascual.h"

PFile INFILE = (PFile){.handle = PNil};
PFile OUTFILE = (PFile){.handle = PNil};

void COPYFILE(PFile* FILEIN, PFile* FILEOUT) {
  PString LINE;
  while (!Eof(FILEIN, 1)) {
    Read(FILEIN, 1, RwpString | RwpLn | RwpEnd, &LINE);
    Write(FILEOUT, 1, RwpStringPtr | RwpLn | RwpEnd, &LINE);
  }
  Close(FILEIN, 1);
  Close(FILEOUT, 1);
}

void pascual_main() {
  PString tmp1;
  tmp1 = str_make(13, "textfiles.pas");
  Assign(&INFILE, &tmp1, 1);
  Reset(&INFILE, 0, 1);
  tmp1 = str_make(23, "/tmp/textfiles-copy.pas");
  Assign(&OUTFILE, &tmp1, 1);
  Rewrite(&OUTFILE, 0, 1);
  COPYFILE(&INFILE, &OUTFILE);
}
