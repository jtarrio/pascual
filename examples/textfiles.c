/* Program: TEXTFILES */
#include "pascual.h"

PFile INFILE;
PFile OUTFILE;

void COPYFILE(PFile *FILEIN, PFile *FILEOUT) {
  PString LINE;
  while (!EOF(FILEIN)) {
    READ_s(FILEIN, &LINE);
    READLN(FILEIN);
    WRITE_s(FILEOUT, LINE);
    WRITELN(FILEOUT);
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
