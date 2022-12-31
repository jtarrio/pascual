all: pascual libpascual.a

bootstrap: compiler.pas
	fpc -Mtp -g compiler.pas -obootstrap

pascual: bootstrap
	cp bootstrap pascual

runtime.o: runtime.h runtime.c
	gcc -c -o runtime.o runtime.c

libpascual.a: runtime.o
	ar rcs libpascual.a runtime.o

.PHONY: clean

clean:
	rm -f *~ *.o bootstrap pascual runtime.o libpascual.a
