RTDIR=../runtime

all: pascual

bootstrap: stage0.c $(RTDIR)/libpascual.a
	$(CC) $(CFLAGS) -o bootstrap -Os -I$(RTDIR) -L$(RTDIR) stage0.c -lpascual

pascual: bootstrap $(RTDIR)/libpascual.a compiler.pas \
         ast.inc.pas containers.inc.pas \
				 errors.inc.pas expression.forward.inc.pas expression.inc.pas \
				 globals.inc.pas lexer.inc.pas parser.inc.pas \
				 progdefs.forward.inc.pas progdefs.inc.pas \
				 pseudofuns.inc.pas statement.inc.pas \
				 modules/*.pas \
				 operations/*.pas \
				 codegen/*.pas
	PC=./bootstrap PINCDIR=$(RTDIR) PLIBDIR=$(RTDIR) ./pascualc compiler.pas -o pascual

.PHONY: clean

clean:
	rm -f *~ *.o bootstrap pascual
