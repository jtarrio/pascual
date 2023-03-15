/* Program: TEXTFILES */
#include "pascual.h"

PFile INFILE;
PFile OUTFILE;

void COPYFILE(PFile *FILEIN, PFile *FILEOUT) {
  PString LINE;
  while (!EOF(FILEIN)) {
    READ(FILEIN, RwpString | RwpLn | RwpEnd, &LINE);
    WRITE(FILEOUT, RwpStringPtr | RwpLn | RwpEnd, &LINE);
  }
  CLOSE(FILEIN);
  CLOSE(FILEOUT);
}

void pascual_main() {
  {
    PString tmp1 = str_make(13, "textfiles.pas");
    ASSIGN(&INFILE, &tmp1);
  }
  RESET(&INFILE);
  {
    PString tmp2 = str_make(23, "/tmp/textfiles-copy.pas");
    ASSIGN(&OUTFILE, &tmp2);
  }
  REWRITE(&OUTFILE);
  COPYFILE(&INFILE, &OUTFILE);
}
