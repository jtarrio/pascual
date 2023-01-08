dist: runtime compiler
	make -C runtime libpascual.a
	make -C compiler pascual
	mkdir -p dist/
	cp runtime/libpascual.a runtime/pascual.h dist
	cp compiler/pascualc compiler/pascual dist

.PHONY: clean dist

clean:
	rm -fR dist/
	make -C runtime clean
	make -C compiler clean
