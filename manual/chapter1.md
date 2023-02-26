# Getting started with Pascual

Pascual runs on Linux and requires GNU Make and the GNU C compiler. Other configurations may work, but are untested.

Pascual's home page resides at https://github.com/jtarrio/pascual.

To download Pascual:
* You can _Git clone_ it using the following commands:
```
$ git clone https://github.com/jtarrio/pascual.git
$ cd pascual
```
* If you prefer to download it as a ZIP file, you can use the following commands:
```
$ curl -L -O https://github.com/jtarrio/pascual/archive/refs/heads/main.zip
$ unzip main.zip
$ cd pascual-main
```

To build Pascual, use the following command:
```
$ make
```
The Pascual compiler will be available in the `dist` subdirectory.

To check if Pascual works, let's write a small program and compile it:
```
$ echo "program p; begin writeln('It works!') end." > check.pas
$ dist/pascualc check.pas
$ ./check
It works!
$
```

Once you have Pascual running, proceed to [chapter 2](chapter2.md).
