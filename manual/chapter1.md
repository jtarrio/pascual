# Getting started with Pascual

Pascual's home page resides at https://github.com/jtarrio/pascual, and you can download Pascual from there.

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

To build Pascual, you will need to have Make and the GNU C compiler installed. After installing them, use the following command:
```
$ make
```
The built Pascual compiler will be available in the `dist` subdirectory.

To check if Pascual works, let's write a small program and compile it:
```
$ cat > helloworld.pas
program Check;
begin
  writeln('It works!)
end.
^D
$ dist/pascualc check.pas
$ ./check
It works!
$
```

Once you have Pascual running, proceed to chapter 2.