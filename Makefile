dist:
	mkdir dist/
	make -C runtime libpascual.a && cp runtime/libpascual.a runtime/pascual.h dist
	make -C compiler pascual && cp compiler/pascualc compiler/pascual dist

.PHONY: clean

clean:
	rm -fR dist/
	make -C runtime clean
	make -C compiler clean
