all: pascual libpascual.a

bootstrap: stage0.c libpascual.a
	gcc -o bootstrap -I. -L. stage0.c -lpascual

pascual: bootstrap compiler.pas lexer.inc.pas parser.inc.pas codegen.defs.inc.pas codegen.inc.pas builtins.inc.pas libpascual.a
	PC=./bootstrap ./pascualc compiler.pas -o pascual

pascual.o: pascual.h pascual.c
	gcc -c $(COPTS) -o pascual.o pascual.c

libpascual.a: pascual.o
	ar rcs libpascual.a pascual.o

.PHONY: clean

clean:
	rm -f *~ *.o bootstrap pascual runtime.o libpascual.a
