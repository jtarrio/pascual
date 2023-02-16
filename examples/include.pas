

{ Showing how you can include other files.
  Note that includes cannot be recursive. }

program include;

{$I include.inc}

begin
  { The 'Display' procedure is in the 'include.inc' file. }
  Display
end.
