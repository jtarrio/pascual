all: pascual libpascual.a

bootstrap: compiler.pas
	fpc -Mtp -g compiler.pas -obootstrap

pascual: bootstrap libpascual.a
	PC=./bootstrap ./pascualc compiler.pas -o pascual

pascual.o: pascual.h pascual.c
	gcc -c $(COPTS) -o pascual.o pascual.c

libpascual.a: pascual.o
	ar rcs libpascual.a pascual.o

.PHONY: clean

clean:
	rm -f *~ *.o bootstrap pascual runtime.o libpascual.a
