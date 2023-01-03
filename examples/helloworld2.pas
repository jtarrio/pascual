(* Variation of Hello World that asks the user's name. *)
program helloworld2;

var
  Name : string;
begin
  write('What is your name? ');
  readln(Name);
  writeln('Hello, ', Name, '!')
end.
