all: pascual libpascual.a

bootstrap: compiler.pas
	fpc -Mtp -g compiler.pas -obootstrap

pascual.c: bootstrap
	./bootstrap < compiler.pas > pascual.c

pascual: pascual.c libpascual.a
	gcc -I. -L. $(COPTS) -o pascual pascual.c -lpascual

runtime.o: runtime.h runtime.c
	gcc -c $(COPTS) -o runtime.o runtime.c

libpascual.a: runtime.o
	ar rcs libpascual.a runtime.o

.PHONY: clean

clean:
	rm -f *~ *.o bootstrap pascual pascual.c runtime.o libpascual.a
