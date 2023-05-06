type
  TCodegen = ^TCodegenBase;
  TCodegenBase = record
    SetOutputFile : procedure(This : TCodegen; const Name : string);
    Generate : procedure(This : TCodegen; AST : TSProgram);
  end;

