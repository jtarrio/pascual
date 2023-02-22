all: libpascual.a

error.o: error.h error.c
file.o: file.h file.c types.h string.h
os.o: os.h os.c string.h types.h
sets.o: sets.h sets.c
string.o: string.h string.c error.h types.h
types.o: types.h types.c
pascual.o: pascual.h pascual.c error.h file.h os.h string.h types.h

libpascual.a: pascual.o error.o file.o os.o sets.o string.o types.o
	ar rcs libpascual.a pascual.o error.o file.o os.o sets.o string.o types.o

.PHONY: clean

clean:
	rm -f *~ *.o libpascual.a
