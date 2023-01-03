program comments;

{ This is a
  multiline comment. }

begin
  writeln{inline comment}('Line 1');
  writeln('Line 2') { Another
  multiline
  comment };
  (* Another style of comment *)writeln('Line 3');
  writeln((* Another inline comment. *)'Line 4');
  writeln('Line 5')(* Finally, another comment. *);
  (* Nested comments of the same type don't work
  (* *) writeln('Line 6'); (* *)
  (* Nested comments of different types work.
  { } writeln('Line 7 no'); (* *)
end.
