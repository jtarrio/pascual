all: libpascual.a

error.o: error.h error.c
file.o: file.h file.c types.h
os.o: os.h os.c string.h types.h
string.o: string.h string.c error.h types.h
pascual.o: pascual.h pascual.c error.h file.h os.h string.h types.h

libpascual.a: pascual.o error.o file.o os.o string.o
	ar rcs libpascual.a pascual.o error.o file.o os.o string.o

.PHONY: clean

clean:
	rm -f *~ *.o libpascual.a
