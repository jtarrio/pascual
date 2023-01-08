RTDIR=../runtime

all: pascual

bootstrap: stage0.c
	gcc -o bootstrap -I$(RTDIR) -L$(RTDIR) stage0.c -lpascual

pascual: bootstrap compiler.pas codegen.defs.inc.pas codegen.inc.pas expression.defs.inc.pas expression.inc.pas globals.inc.pas lexer.inc.pas parser.inc.pas progdefs.inc.pas
	PC=./bootstrap PINCDIR=$(RTDIR) PLIBDIR=$(RTDIR) ./pascualc compiler.pas -o pascual

.PHONY: clean

clean:
	rm -f *~ *.o bootstrap pascual
