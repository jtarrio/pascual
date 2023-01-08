all: libpascual.a

pascual.o: pascual.h pascual.c
	gcc -c $(COPTS) -o pascual.o pascual.c

libpascual.a: pascual.o
	ar rcs libpascual.a pascual.o

.PHONY: clean

clean:
	rm -f *~ *.o libpascual.a
