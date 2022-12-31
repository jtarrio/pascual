dist:
	mkdir dist/
	make -C compiler pascual libpascual.a && cp compiler/pascualc compiler/pascual compiler/libpascual.a dist

.PHONY: clean

clean:
	rm -fR dist/
	make -C compiler clean
