/* Program: COMPILER */
#include "pascual.h"

typedef enum enum1 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKNUMBER, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAND, TKFALSE, TKTRUE, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH } TLXTOKENID;
typedef struct record2 {
  int ROW;
  int COL;
} TLXPOS;
typedef struct record3 {
  TLXTOKENID ID;
  PString VALUE;
  TLXPOS POS;
} TLXTOKEN;
typedef struct record4 {
  PFile SRC;
  PString NAME;
  TLXPOS POS;
} TLXINPUTFILE;

const char* enumvalues1[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKNUMBER", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAND", "TKFALSE", "TKTRUE", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH" };

struct record6 {
  PString LINE;
  TLXTOKEN TOKEN;
  TLXINPUTFILE INPUT;
  struct record5 {
    PBoolean EXISTS;
    TLXINPUTFILE INPUT;
  } PREV;
} LEXER;

PString LXTOKENNAME(TLXTOKENID ID) {
  PString return_LXTOKENNAME;
  PString NAME;
  NAME = to_str_e(ID, enumvalues1);
  return_LXTOKENNAME = NAME;
  return return_LXTOKENNAME;
}

PString LXPOSSTR(TLXPOS POS) {
  PString return_LXPOSSTR;
  PString ROW;
  PString COL;
  ROW = to_str_i(POS.ROW);
  COL = to_str_i(POS.COL);
  return_LXPOSSTR = cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(4, "row "), ROW), str_make(5, " col ")), COL), str_make(4, " in ")), LEXER.INPUT.NAME);
  return return_LXPOSSTR;
}

PString LXWHERESTR() {
  PString return_LXWHERESTR;
  return_LXWHERESTR = cat_ss(str_make(6, " near "), LXPOSSTR(LEXER.TOKEN.POS));
  return return_LXWHERESTR;
}

PString LXTOKENSTR() {
  PString return_LXTOKENSTR;
  return_LXTOKENSTR = cat_sc(cat_ss(cat_ss(LXTOKENNAME(LEXER.TOKEN.ID), str_make(2, " [")), LEXER.TOKEN.VALUE), ']');
  return return_LXTOKENSTR;
}

void COMPILEERROR(PString MSG) {
  write_s(&STDERR, MSG);
  write_s(&STDERR, LXWHERESTR());
  writeln(&STDERR);
  HALT(1);
}

PBoolean LXISALPHA(char CHR) {
  PBoolean return_LXISALPHA;
  return_LXISALPHA = CHR >= 'a' && CHR <= 'z' || CHR >= 'A' && CHR <= 'Z';
  return return_LXISALPHA;
}

PBoolean LXISDIGIT(char CHR) {
  PBoolean return_LXISDIGIT;
  return_LXISDIGIT = CHR >= '0' && CHR <= '9';
  return return_LXISDIGIT;
}

PBoolean LXISALPHANUM(char CHR) {
  PBoolean return_LXISALPHANUM;
  return_LXISALPHANUM = LXISALPHA(CHR) || LXISDIGIT(CHR);
  return return_LXISALPHANUM;
}

PBoolean LXISIDENTIFIERFIRST(char CHR) {
  PBoolean return_LXISIDENTIFIERFIRST;
  return_LXISIDENTIFIERFIRST = LXISALPHA(CHR) || CHR == '_';
  return return_LXISIDENTIFIERFIRST;
}

PBoolean LXISIDENTIFIERCHAR(char CHR) {
  PBoolean return_LXISIDENTIFIERCHAR;
  return_LXISIDENTIFIERCHAR = LXISALPHANUM(CHR) || CHR == '_';
  return return_LXISIDENTIFIERCHAR;
}

PBoolean LXISTOKENWAITING() {
  PBoolean return_LXISTOKENWAITING;
  do {
    while (LENGTH(LEXER.LINE) == 0 && !EOF(&LEXER.INPUT.SRC)) {
      LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
      LEXER.INPUT.POS.COL = 1;
      read_s(&LEXER.INPUT.SRC, &LEXER.LINE);
      readln(&LEXER.INPUT.SRC);
    }
    while (LENGTH(LEXER.LINE) > 0 && LEXER.LINE.chr[1] == ' ') {
      LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + 1;
      DELETE(&LEXER.LINE, 1, 1);
    }
  } while (!(EOF(&LEXER.INPUT.SRC) || LENGTH(LEXER.LINE) > 0));
  return_LXISTOKENWAITING = LENGTH(LEXER.LINE) > 0;
  return return_LXISTOKENWAITING;
}

void LXGETSYMBOL(TLXTOKENID ID, int LENGTH) {
  LEXER.TOKEN.ID = ID;
  LEXER.TOKEN.VALUE = COPY(LEXER.LINE, 1, LENGTH);
  LEXER.TOKEN.POS = LEXER.INPUT.POS;
  DELETE(&LEXER.LINE, 1, LENGTH);
  LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + LENGTH;
}

void LXGETIDENTIFIER() {
  char CHR;
  int POS;
  PBoolean INTOKEN;
  POS = 0;
  INTOKEN = 1;
  while (POS < LENGTH(LEXER.LINE) && INTOKEN) {
    CHR = LEXER.LINE.chr[POS + 1];
    INTOKEN = LXISIDENTIFIERCHAR(CHR);
    if (INTOKEN) POS = POS + 1;
  }
  LXGETSYMBOL(TKIDENTIFIER, POS);
  do {
    int first = 1;
    int last = LENGTH(LEXER.TOKEN.VALUE);
    if (first <= last) {
      POS = first;
      while (1) {
        LEXER.TOKEN.VALUE.chr[POS] = UPCASE(LEXER.TOKEN.VALUE.chr[POS]);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "AND")) == 0) LEXER.TOKEN.ID = TKAND;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "ARRAY")) == 0) LEXER.TOKEN.ID = TKARRAY;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "BEGIN")) == 0) LEXER.TOKEN.ID = TKBEGIN;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "CASE")) == 0) LEXER.TOKEN.ID = TKCASE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "CONST")) == 0) LEXER.TOKEN.ID = TKCONST;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "DIV")) == 0) LEXER.TOKEN.ID = TKDIV;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "DO")) == 0) LEXER.TOKEN.ID = TKDO;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(6, "DOWNTO")) == 0) LEXER.TOKEN.ID = TKDOWNTO;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "ELSE")) == 0) LEXER.TOKEN.ID = TKELSE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "END")) == 0) LEXER.TOKEN.ID = TKEND;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "FILE")) == 0) LEXER.TOKEN.ID = TKFILE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "FOR")) == 0) LEXER.TOKEN.ID = TKFOR;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(7, "FORWARD")) == 0) LEXER.TOKEN.ID = TKFORWARD;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(8, "FUNCTION")) == 0) LEXER.TOKEN.ID = TKFUNCTION;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "GOTO")) == 0) LEXER.TOKEN.ID = TKGOTO;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "IF")) == 0) LEXER.TOKEN.ID = TKIF;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "IN")) == 0) LEXER.TOKEN.ID = TKIN;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "LABEL")) == 0) LEXER.TOKEN.ID = TKLABEL;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "MOD")) == 0) LEXER.TOKEN.ID = TKMOD;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "NIL")) == 0) LEXER.TOKEN.ID = TKNIL;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "NOT")) == 0) LEXER.TOKEN.ID = TKNOT;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "OF")) == 0) LEXER.TOKEN.ID = TKOF;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "OR")) == 0) LEXER.TOKEN.ID = TKOR;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(6, "PACKED")) == 0) LEXER.TOKEN.ID = TKPACKED;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(9, "PROCEDURE")) == 0) LEXER.TOKEN.ID = TKPROCEDURE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(7, "PROGRAM")) == 0) LEXER.TOKEN.ID = TKPROGRAM;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(6, "RECORD")) == 0) LEXER.TOKEN.ID = TKRECORD;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(6, "REPEAT")) == 0) LEXER.TOKEN.ID = TKREPEAT;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "SET")) == 0) LEXER.TOKEN.ID = TKSET;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "THEN")) == 0) LEXER.TOKEN.ID = TKTHEN;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "TO")) == 0) LEXER.TOKEN.ID = TKTO;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "TYPE")) == 0) LEXER.TOKEN.ID = TKTYPE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "UNTIL")) == 0) LEXER.TOKEN.ID = TKUNTIL;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "VAR")) == 0) LEXER.TOKEN.ID = TKVAR;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "WHILE")) == 0) LEXER.TOKEN.ID = TKWHILE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "WITH")) == 0) LEXER.TOKEN.ID = TKWITH;
}

void LXGETNUMBER() {
  char CHR;
  int POS;
  PBoolean INTOKEN;
  POS = 0;
  INTOKEN = 1;
  while (POS < LENGTH(LEXER.LINE) && INTOKEN) {
    CHR = LEXER.LINE.chr[POS + 1];
    INTOKEN = LXISDIGIT(CHR);
    if (INTOKEN) POS = POS + 1;
  }
  LXGETSYMBOL(TKNUMBER, POS);
}

void LXGETSTRING() {
  char CHR;
  int POS;
  PBoolean INSTRING;
  POS = 1;
  INSTRING = 1;
  while (INSTRING) {
    POS = POS + 1;
    CHR = LEXER.LINE.chr[POS];
    if (CHR == '\'') {
      if (LENGTH(LEXER.LINE) > POS + 1 && LEXER.LINE.chr[POS + 1] == '\'') POS = POS + 1;
      else INSTRING = 0;
    }
  }
  LXGETSYMBOL(TKSTRING, POS);
}

void LXGETCOMMENT() {
  PBoolean DONE;
  int DELIMITERLENGTH;
  PString COMMENT;
  COMMENT = str_make(0, "");
  DONE = 0;
  if (LEXER.LINE.chr[1] == '{') DELIMITERLENGTH = 1;
  else DELIMITERLENGTH = 2;
  LXGETSYMBOL(TKCOMMENT, DELIMITERLENGTH);
  do {
    while (cmp_ss(LEXER.LINE, str_make(0, "")) == 0) {
      COMMENT = cat_sc(COMMENT, ' ');
      read_s(&LEXER.INPUT.SRC, &LEXER.LINE);
      readln(&LEXER.INPUT.SRC);
      LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
      LEXER.INPUT.POS.COL = 1;
    }
    if (DELIMITERLENGTH == 1) DONE = LEXER.LINE.chr[1] == '}';
    else DONE = LEXER.LINE.chr[1] == '*' && LEXER.LINE.chr[2] == ')';
    if (!DONE) {
      COMMENT = cat_sc(COMMENT, LEXER.LINE.chr[1]);
      DELETE(&LEXER.LINE, 1, 1);
      LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + 1;
    }
  } while (!DONE);
  DELETE(&LEXER.LINE, 1, DELIMITERLENGTH);
  LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + DELIMITERLENGTH;
  LEXER.TOKEN.VALUE = COMMENT;
}

void LXREADTOKEN() {
  char CHR;
  PString PFX;
  LEXER.TOKEN.VALUE = str_make(0, "");
  LEXER.TOKEN.ID = TKUNKNOWN;
  if (!LXISTOKENWAITING()) LEXER.TOKEN.ID = TKEOF;
  else {
    CHR = LEXER.LINE.chr[1];
    if (LENGTH(LEXER.LINE) >= 2) PFX = cat_cc(LEXER.LINE.chr[1], LEXER.LINE.chr[2]);
    else PFX = str_make(0, "");
    if (cmp_ss(PFX, str_make(2, "<>")) == 0) LXGETSYMBOL(TKNOTEQUALS, 2);
    else if (cmp_ss(PFX, str_make(2, "<=")) == 0) LXGETSYMBOL(TKLESSOREQUALS, 2);
    else if (cmp_ss(PFX, str_make(2, ">=")) == 0) LXGETSYMBOL(TKMOREOREQUALS, 2);
    else if (cmp_ss(PFX, str_make(2, ":=")) == 0) LXGETSYMBOL(TKASSIGN, 2);
    else if (cmp_ss(PFX, str_make(2, "..")) == 0) LXGETSYMBOL(TKRANGE, 2);
    else if (cmp_ss(PFX, str_make(2, "(*")) == 0) LXGETCOMMENT();
    else if (LXISIDENTIFIERFIRST(CHR)) LXGETIDENTIFIER();
    else if (LXISDIGIT(CHR)) LXGETNUMBER();
    else switch (CHR) {
      case '\'':
        LXGETSTRING();
        break;
      case '+':
        LXGETSYMBOL(TKPLUS, 1);
        break;
      case '-':
        LXGETSYMBOL(TKMINUS, 1);
        break;
      case '*':
        LXGETSYMBOL(TKASTERISK, 1);
        break;
      case '/':
        LXGETSYMBOL(TKSLASH, 1);
        break;
      case '=':
        LXGETSYMBOL(TKEQUALS, 1);
        break;
      case '<':
        LXGETSYMBOL(TKLESSTHAN, 1);
        break;
      case '>':
        LXGETSYMBOL(TKMORETHAN, 1);
        break;
      case '[':
        LXGETSYMBOL(TKLBRACKET, 1);
        break;
      case ']':
        LXGETSYMBOL(TKRBRACKET, 1);
        break;
      case '.':
        LXGETSYMBOL(TKDOT, 1);
        break;
      case ',':
        LXGETSYMBOL(TKCOMMA, 1);
        break;
      case ':':
        LXGETSYMBOL(TKCOLON, 1);
        break;
      case ';':
        LXGETSYMBOL(TKSEMICOLON, 1);
        break;
      case '^':
        LXGETSYMBOL(TKCARET, 1);
        break;
      case '(':
        LXGETSYMBOL(TKLPAREN, 1);
        break;
      case ')':
        LXGETSYMBOL(TKRPAREN, 1);
        break;
      case '{':
        LXGETCOMMENT();
        break;
      default:
        COMPILEERROR(cat_ss(str_make(17, "Could not parse ["), LEXER.LINE));
        break;
    }
  }
}

void LXRESET() {
  LEXER.LINE = str_make(0, "");
  LEXER.INPUT.SRC = INPUT;
  LEXER.INPUT.NAME = str_of('-');
  LEXER.INPUT.POS.ROW = 0;
  LEXER.INPUT.POS.COL = 0;
  LEXER.PREV.EXISTS = 0;
}

void LXOPEN(PString FILENAME) {
  LEXER.INPUT.NAME = FILENAME;
  ASSIGN(&LEXER.INPUT.SRC, LEXER.INPUT.NAME);
  RESET(&LEXER.INPUT.SRC);
}

void LXINCLUDE(PString FILENAME) {
  if (LEXER.PREV.EXISTS) COMPILEERROR(str_make(33, "Include files cannot be recursive"));
  LEXER.PREV.EXISTS = 1;
  LEXER.PREV.INPUT = LEXER.INPUT;
  LEXER.INPUT.POS.ROW = 0;
  LEXER.INPUT.POS.COL = 0;
  LXOPEN(FILENAME);
}

typedef enum enum7 { TSFREAD, TSFREADLN, TSFWRITE, TSFWRITELN, TSFSTR, TSFNEW, TSFDISPOSE } TPSSPECIALFUNCTION;
typedef struct record29 *TPSTYPEINDEX;
typedef struct record30 *TPSENUMINDEX;
typedef struct record32 *TPSRECORDINDEX;
typedef struct record33 *TPSARRAYINDEX;
typedef struct record34 *TPSCONSTANTINDEX;
typedef struct record35 *TPSVARIABLEINDEX;
typedef struct record36 *TPSFUNCTIONINDEX;
typedef struct record37 *TPSWITHVARINDEX;
typedef struct record39 *TPSNAMEINDEX;
typedef struct record26 *TEXPRESSION;
typedef enum enum8 { XICNIL, XICBOOLEAN, XICINTEGER, XICCHAR, XICSTRING, XICENUM } TEXIMMEDIATECLASS;
typedef struct record9 {
  TEXIMMEDIATECLASS CLS;
  union {
    struct {
      PBoolean BOOLEANVALUE;
    };
    struct {
      int INTEGERVALUE;
    };
    struct {
      char CHARVALUE;
    };
    struct {
      PString STRINGVALUE;
    };
    struct {
      int ENUMORDINAL;
    };
  };
} TEXIMMEDIATE;
typedef struct record10 {
  struct record26 *PARENT;
} TEXTOSTRING;
typedef struct record11 {
  struct record35 *VARIABLEINDEX;
} TEXVARIABLEACCESS;
typedef struct record12 {
  struct record26 *PARENT;
  int FIELDNUMBER;
} TEXFIELDACCESS;
typedef struct record13 {
  struct record26 *PARENT;
  struct record26 *SUBSCRIPT;
} TEXARRAYACCESS;
typedef struct record14 {
  struct record26 *PARENT;
} TEXPOINTERACCESS;
typedef struct record15 {
  struct record26 *PARENT;
  struct record26 *SUBSCRIPT;
} TEXSTRINGCHAR;
typedef struct record16 {
  struct record36 *FUNCTIONINDEX;
} TEXFUNCTIONREF;
typedef struct record17 {
  int SIZE;
  struct record26 *VALUES[4];
} TEXFUNCTIONARGS;
typedef struct record18 {
  struct record26 *FUNCTIONREF;
  TEXFUNCTIONARGS ARGS;
} TEXFUNCTIONCALL;
typedef struct record19 {
  TPSSPECIALFUNCTION SPECIALFUNCTION;
} TEXSPECIALFUNCTIONREF;
typedef struct record20 {
  struct record26 *ARG;
  struct record20 *NEXT;
} TEXREADARGS;
typedef struct record21 {
  struct record26 *ARG;
  struct record21 *NEXT;
} TEXWRITEARGS;
typedef struct record22 {
  struct record26 *SRC;
  struct record26 *DST;
  struct record26 *PTR;
  TPSSPECIALFUNCTION SPECIALFUNCTION;
  union {
    struct {
      struct record20 *READARGS;
    };
    struct {
      struct record21 *WRITEARGS;
    };
  };
} TEXSPECIALFUNCTIONCALL;
typedef struct record23 {
  struct record26 *PARENT;
  TLXTOKENID OP;
} TEXUNARYOP;
typedef struct record24 {
  struct record26 *LEFT;
  struct record26 *RIGHT;
  TLXTOKENID OP;
} TEXBINARYOP;
typedef enum enum25 { XCIMMEDIATE, XCTOSTRING, XCVARIABLEACCESS, XCFIELDACCESS, XCARRAYACCESS, XCPOINTERACCESS, XCSTRINGCHAR, XCFUNCTIONREF, XCFUNCTIONCALL, XCSPECIALFUNCTIONREF, XCSPECIALFUNCTIONCALL, XCUNARYOP, XCBINARYOP } TEXPRESSIONCLASS;
typedef struct record26 {
  struct record29 *TYPEINDEX;
  PBoolean ISCONSTANT;
  PBoolean ISASSIGNABLE;
  PBoolean ISFUNCTIONRESULT;
  TEXPRESSIONCLASS CLS;
  union {
    struct {
      TEXIMMEDIATE IMMEDIATEEX;
    };
    struct {
      TEXTOSTRING TOSTRINGEX;
    };
    struct {
      TEXVARIABLEACCESS VARIABLEEX;
    };
    struct {
      TEXFIELDACCESS FIELDEX;
    };
    struct {
      TEXARRAYACCESS ARRAYEX;
    };
    struct {
      TEXPOINTERACCESS POINTEREX;
    };
    struct {
      TEXSTRINGCHAR STRINGCHAREX;
    };
    struct {
      TEXFUNCTIONREF FUNCTIONEX;
    };
    struct {
      TEXFUNCTIONCALL CALLEX;
    };
    struct {
      TEXSPECIALFUNCTIONREF SPECIALFUNCTIONEX;
    };
    struct {
      TEXSPECIALFUNCTIONCALL SPECIALFUNCTIONCALLEX;
    };
    struct {
      TEXUNARYOP UNARYEX;
    };
    struct {
      TEXBINARYOP BINARYEX;
    };
  };
} TEXPRESSIONOBJ;
typedef struct record27 {
  PString NAME;
} TPSIDENTIFIER;
typedef enum enum28 { TTCBOOLEAN, TTCINTEGER, TTCCHAR, TTCSTRING, TTCTEXT, TTCENUM, TTCRECORD, TTCARRAY, TTCPOINTER, TTCNIL, TTCPOINTERUNKNOWN } TPSTYPECLASS;
typedef struct record29 {
  PString NAME;
  struct record29 *ALIASFOR;
  TPSTYPECLASS CLS;
  union {
    struct {
      struct record30 *ENUMINDEX;
    };
    struct {
      struct record32 *RECORDINDEX;
    };
    struct {
      struct record33 *ARRAYINDEX;
    };
    struct {
      struct record29 *POINTEDTYPEINDEX;
    };
    struct {
      PString *TARGETNAME;
    };
  };
} TPSTYPE;
typedef struct record30 {
  int SIZE;
  PString VALUES[128];
  int ID;
  PBoolean HASBEENDEFINED;
} TPSENUMDEF;
typedef struct record31 {
  PString NAME;
  struct record29 *TYPEINDEX;
} TPSRECORDFIELD;
typedef struct record32 {
  int SIZE;
  TPSRECORDFIELD FIELDS[32];
  int NUMVARIANTS;
  int VARIANTBOUNDS[32];
  int ID;
  PBoolean HASBEENDEFINED;
} TPSRECORDDEF;
typedef struct record33 {
  struct record26 *LOWBOUND;
  struct record26 *HIGHBOUND;
  struct record29 *TYPEINDEX;
} TPSARRAYDEF;
typedef struct record34 {
  PString NAME;
  TLXTOKEN REPLACEMENT;
} TPSCONSTANT;
typedef struct record35 {
  PString NAME;
  struct record29 *TYPEINDEX;
  PBoolean ISREFERENCE;
  PBoolean ISCONSTANT;
} TPSVARIABLE;
typedef struct record36 {
  PString NAME;
  int ARGCOUNT;
  TPSVARIABLE ARGS[4];
  struct record29 *RETURNTYPEINDEX;
  PBoolean ISDECLARATION;
} TPSFUNCTION;
typedef struct record37 {
  struct record35 *VARIABLEINDEX;
} TPSWITHVAR;
typedef enum enum38 { TNCTYPE, TNCVARIABLE, TNCENUMVALUE, TNCFUNCTION, TNCSPECIALFUNCTION } TPSNAMECLASS;
typedef struct record39 {
  PString NAME;
  TPSNAMECLASS CLS;
  union {
    struct {
      struct record29 *TYPEINDEX;
    };
    struct {
      struct record35 *VARIABLEINDEX;
    };
    struct {
      struct record29 *ENUMTYPEINDEX;
      int ORDINAL;
    };
    struct {
      struct record36 *FUNCTIONINDEX;
    };
    struct {
      TPSSPECIALFUNCTION SPECIALFUNCTION;
    };
  };
} TPSNAME;
typedef struct record41 *TPSDEFPTR;
typedef enum enum40 { TDCNAME, TDCTYPE, TDCENUM, TDCRECORD, TDCARRAY, TDCCONSTANT, TDCVARIABLE, TDCFUNCTION, TDCWITHVAR, TDCSCOPEBOUNDARY } TPSDEFCLASS;
typedef struct record41 {
  struct record41 *PREV;
  struct record41 *NEXT;
  TPSDEFCLASS CLS;
  union {
    struct {
      struct record39 *NAMEINDEX;
    };
    struct {
      struct record29 *TYPEINDEX;
    };
    struct {
      struct record30 *ENUMINDEX;
    };
    struct {
      struct record32 *RECORDINDEX;
    };
    struct {
      struct record33 *ARRAYINDEX;
    };
    struct {
      struct record34 *CONSTANTINDEX;
    };
    struct {
      struct record35 *VARIABLEINDEX;
    };
    struct {
      struct record36 *FUNCTIONINDEX;
    };
    struct {
      struct record37 *WITHVARINDEX;
    };
    struct {
      PBoolean TEMPORARYSCOPE;
      struct record36 *CURRENTFUNCTION;
    };
  };
} TPSDEFENTRY;
typedef struct record42 {
  struct record41 *LATEST;
  struct record36 *CURRENTFUNCTION;
  int COUNTER;
} TPSDEFS;

const char* enumvalues7[] = { "TSFREAD", "TSFREADLN", "TSFWRITE", "TSFWRITELN", "TSFSTR", "TSFNEW", "TSFDISPOSE" };
const char* enumvalues8[] = { "XICNIL", "XICBOOLEAN", "XICINTEGER", "XICCHAR", "XICSTRING", "XICENUM" };
const char* enumvalues25[] = { "XCIMMEDIATE", "XCTOSTRING", "XCVARIABLEACCESS", "XCFIELDACCESS", "XCARRAYACCESS", "XCPOINTERACCESS", "XCSTRINGCHAR", "XCFUNCTIONREF", "XCFUNCTIONCALL", "XCSPECIALFUNCTIONREF", "XCSPECIALFUNCTIONCALL", "XCUNARYOP", "XCBINARYOP" };
const char* enumvalues28[] = { "TTCBOOLEAN", "TTCINTEGER", "TTCCHAR", "TTCSTRING", "TTCTEXT", "TTCENUM", "TTCRECORD", "TTCARRAY", "TTCPOINTER", "TTCNIL", "TTCPOINTERUNKNOWN" };
const char* enumvalues38[] = { "TNCTYPE", "TNCVARIABLE", "TNCENUMVALUE", "TNCFUNCTION", "TNCSPECIALFUNCTION" };
const char* enumvalues40[] = { "TDCNAME", "TDCTYPE", "TDCENUM", "TDCRECORD", "TDCARRAY", "TDCCONSTANT", "TDCVARIABLE", "TDCFUNCTION", "TDCWITHVAR", "TDCSCOPEBOUNDARY" };

TPSDEFS DEFS;
struct record43 {
  struct record29 *PTNIL;
  struct record29 *PTBOOLEAN;
  struct record29 *PTINTEGER;
  struct record29 *PTCHAR;
  struct record29 *PTSTRING;
  struct record29 *PTTEXT;
} PRIMITIVETYPES;

int DEFCOUNTER() {
  int return_DEFCOUNTER;
  DEFS.COUNTER = DEFS.COUNTER + 1;
  return_DEFCOUNTER = DEFS.COUNTER;
  return return_DEFCOUNTER;
}

void INITDEFS() {
  DEFS.LATEST = (void*)0;
  DEFS.CURRENTFUNCTION = (void*)0;
  DEFS.COUNTER = 0;
}

struct record41 *_NEWDEF(TPSDEFCLASS CLS) {
  struct record41 *return__NEWDEF;
  struct record41 *DEF;
  DEF = malloc(sizeof(struct record41));
  DEF->PREV = (void*)0;
  DEF->NEXT = (void*)0;
  DEF->CLS = CLS;
  switch (CLS) {
    case TDCNAME:
      DEF->NAMEINDEX = malloc(sizeof(struct record39));
      break;
    case TDCTYPE:
      DEF->TYPEINDEX = malloc(sizeof(struct record29));
      break;
    case TDCENUM:
      DEF->ENUMINDEX = malloc(sizeof(struct record30));
      break;
    case TDCRECORD:
      DEF->RECORDINDEX = malloc(sizeof(struct record32));
      break;
    case TDCARRAY:
      DEF->ARRAYINDEX = malloc(sizeof(struct record33));
      break;
    case TDCCONSTANT:
      DEF->CONSTANTINDEX = malloc(sizeof(struct record34));
      break;
    case TDCVARIABLE:
      DEF->VARIABLEINDEX = malloc(sizeof(struct record35));
      break;
    case TDCFUNCTION:
      DEF->FUNCTIONINDEX = malloc(sizeof(struct record36));
      break;
    case TDCWITHVAR:
      DEF->WITHVARINDEX = malloc(sizeof(struct record37));
      break;
    case TDCSCOPEBOUNDARY:
      {
        DEF->TEMPORARYSCOPE = 0;
        DEF->CURRENTFUNCTION = (void*)0;
      }
      break;
    default:
      break;
  }
  return__NEWDEF = DEF;
  return return__NEWDEF;
}

void _DISPOSEDEF(struct record41 *DEF) {
  switch (DEF->CLS) {
    case TDCNAME:
      free(DEF->NAMEINDEX);
      break;
    case TDCTYPE:
      free(DEF->TYPEINDEX);
      break;
    case TDCENUM:
      free(DEF->ENUMINDEX);
      break;
    case TDCRECORD:
      free(DEF->RECORDINDEX);
      break;
    case TDCARRAY:
      free(DEF->ARRAYINDEX);
      break;
    case TDCCONSTANT:
      free(DEF->CONSTANTINDEX);
      break;
    case TDCVARIABLE:
      free(DEF->VARIABLEINDEX);
      break;
    case TDCFUNCTION:
      free(DEF->FUNCTIONINDEX);
      break;
    case TDCWITHVAR:
      free(DEF->WITHVARINDEX);
      break;
    default:
      break;
  }
  free(DEF);
}

struct record41 *_ADDDEF(TPSDEFCLASS CLS) {
  struct record41 *return__ADDDEF;
  struct record41 *DEF;
  DEF = _NEWDEF(CLS);
  DEF->PREV = DEFS.LATEST;
  if (DEFS.LATEST != (void*)0) DEFS.LATEST->NEXT = DEF;
  DEFS.LATEST = DEF;
  return__ADDDEF = DEF;
  return return__ADDDEF;
}

PBoolean _DELETEDEF(TPSDEFENTRY *DELETEDDEF) {
  PBoolean return__DELETEDEF;
  if (DEFS.LATEST == (void*)0) return__DELETEDEF = 0;
  else {
    *DELETEDDEF = *DEFS.LATEST;
    _DISPOSEDEF(DEFS.LATEST);
    DEFS.LATEST = (*DELETEDDEF).PREV;
    return__DELETEDEF = 1;
  }
  return return__DELETEDEF;
}

void _STARTSCOPE(PBoolean TEMPORARY, struct record36 *NEWFUNCTION) {
  struct record41 *DEF;
  DEF = _ADDDEF(TDCSCOPEBOUNDARY);
  DEF->TEMPORARYSCOPE = TEMPORARY;
  DEF->CURRENTFUNCTION = DEFS.CURRENTFUNCTION;
  if (!TEMPORARY) DEFS.CURRENTFUNCTION = NEWFUNCTION;
}

void _CLOSESCOPE(PBoolean TEMPORARY) {
  TPSDEFENTRY DELETEDDEF;
  PBoolean DELETED;
  do {
    DELETED = _DELETEDEF(&DELETEDDEF);
  } while (!(!DELETED || DELETEDDEF.CLS == TDCSCOPEBOUNDARY && (TEMPORARY || !DELETEDDEF.TEMPORARYSCOPE)));
  DEFS.CURRENTFUNCTION = DELETEDDEF.CURRENTFUNCTION;
}

void STARTLOCALSCOPE(struct record36 *NEWFUNCTION) {
  _STARTSCOPE(0, NEWFUNCTION);
}

void CLOSELOCALSCOPE() {
  _CLOSESCOPE(0);
}

void STARTTEMPORARYSCOPE() {
  _STARTSCOPE(1, (void*)0);
}

void CLOSETEMPORARYSCOPE() {
  _CLOSESCOPE(1);
}

struct record39 *_FINDNAME(PString NAME, PBoolean REQUIRED, PBoolean FROMLOCALSCOPE) {
  struct record39 *return__FINDNAME;
  struct record41 *DEF;
  struct record39 *RET;
  RET = (void*)0;
  DEF = DEFS.LATEST;
  while (RET == (void*)0 && DEF != (void*)0 && (!FROMLOCALSCOPE || DEF->CLS != TDCSCOPEBOUNDARY)) {
    if (DEF->CLS == TDCNAME && cmp_ss(NAME, DEF->NAMEINDEX->NAME) == 0) RET = DEF->NAMEINDEX;
    DEF = DEF->PREV;
  }
  if (REQUIRED && RET == (void*)0) COMPILEERROR(cat_ss(str_make(20, "Unknown identifier: "), NAME));
  return__FINDNAME = RET;
  return return__FINDNAME;
}

struct record39 *_CHECKNAMECLASS(struct record39 *NAMEINDEX, TPSNAMECLASS CLS) {
  struct record39 *return__CHECKNAMECLASS;
  if (NAMEINDEX != (void*)0 && NAMEINDEX->CLS != CLS) switch (NAMEINDEX->CLS) {
    case TNCTYPE:
      COMPILEERROR(cat_ss(str_make(12, "Not a type: "), NAMEINDEX->NAME));
      break;
    case TNCVARIABLE:
      COMPILEERROR(cat_ss(str_make(16, "Not a variable: "), NAMEINDEX->NAME));
      break;
    case TNCENUMVALUE:
      COMPILEERROR(cat_ss(str_make(26, "Not an enumeration value: "), NAMEINDEX->NAME));
      break;
    case TNCFUNCTION:
      COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), NAMEINDEX->NAME));
      break;
    case TNCSPECIALFUNCTION:
      COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), NAMEINDEX->NAME));
      break;
    default:
      COMPILEERROR(str_make(35, "Internal error: name class mismatch"));
      break;
  }
  return__CHECKNAMECLASS = NAMEINDEX;
  return return__CHECKNAMECLASS;
}

struct record39 *FINDNAMEINLOCALSCOPE(PString NAME, PBoolean REQUIRED) {
  struct record39 *return_FINDNAMEINLOCALSCOPE;
  return_FINDNAMEINLOCALSCOPE = _FINDNAME(NAME, REQUIRED, 1);
  return return_FINDNAMEINLOCALSCOPE;
}

struct record39 *FINDNAMEOFCLASSINLOCALSCOPE(PString NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
  struct record39 *return_FINDNAMEOFCLASSINLOCALSCOPE;
  return_FINDNAMEOFCLASSINLOCALSCOPE = _CHECKNAMECLASS(FINDNAMEINLOCALSCOPE(NAME, REQUIRED), CLS);
  return return_FINDNAMEOFCLASSINLOCALSCOPE;
}

struct record39 *FINDNAME(PString NAME, PBoolean REQUIRED) {
  struct record39 *return_FINDNAME;
  return_FINDNAME = _FINDNAME(NAME, REQUIRED, 0);
  return return_FINDNAME;
}

struct record39 *FINDNAMEOFCLASS(PString NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
  struct record39 *return_FINDNAMEOFCLASS;
  return_FINDNAMEOFCLASS = _CHECKNAMECLASS(FINDNAME(NAME, REQUIRED), CLS);
  return return_FINDNAMEOFCLASS;
}

struct record39 *_ADDNAME(PString NAME, TPSNAMECLASS CLS) {
  struct record39 *return__ADDNAME;
  struct record39 *POS;
  if (FINDNAMEINLOCALSCOPE(NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), NAME), str_make(16, " already defined")));
  POS = _ADDDEF(TDCNAME)->NAMEINDEX;
  POS->NAME = NAME;
  POS->CLS = CLS;
  return__ADDNAME = POS;
  return return__ADDNAME;
}

struct record39 *ADDTYPENAME(PString NAME, struct record29 *IDX) {
  struct record39 *return_ADDTYPENAME;
  struct record39 *DEF;
  DEF = _ADDNAME(NAME, TNCTYPE);
  DEF->TYPEINDEX = IDX;
  return_ADDTYPENAME = DEF;
  return return_ADDTYPENAME;
}

struct record39 *ADDVARIABLENAME(PString NAME, struct record35 *IDX) {
  struct record39 *return_ADDVARIABLENAME;
  struct record39 *DEF;
  DEF = _ADDNAME(NAME, TNCVARIABLE);
  DEF->VARIABLEINDEX = IDX;
  return_ADDVARIABLENAME = DEF;
  return return_ADDVARIABLENAME;
}

struct record39 *ADDFUNCTIONNAME(PString NAME, struct record36 *IDX) {
  struct record39 *return_ADDFUNCTIONNAME;
  struct record39 *DEF;
  DEF = _ADDNAME(NAME, TNCFUNCTION);
  DEF->FUNCTIONINDEX = IDX;
  return_ADDFUNCTIONNAME = DEF;
  return return_ADDFUNCTIONNAME;
}

struct record39 *ADDENUMVALUENAME(int ORDINAL, struct record29 *TYPEIDX) {
  struct record39 *return_ADDENUMVALUENAME;
  struct record39 *DEF;
  DEF = _ADDNAME(TYPEIDX->ENUMINDEX->VALUES[ORDINAL - 1], TNCENUMVALUE);
  DEF->ENUMTYPEINDEX = TYPEIDX;
  DEF->ORDINAL = ORDINAL;
  return_ADDENUMVALUENAME = DEF;
  return return_ADDENUMVALUENAME;
}

struct record39 *ADDSPECIALFUNCTIONNAME(PString NAME, TPSSPECIALFUNCTION FN) {
  struct record39 *return_ADDSPECIALFUNCTIONNAME;
  struct record39 *DEF;
  DEF = _ADDNAME(NAME, TNCSPECIALFUNCTION);
  DEF->SPECIALFUNCTION = FN;
  return_ADDSPECIALFUNCTIONNAME = DEF;
  return return_ADDSPECIALFUNCTIONNAME;
}

PString DEEPTYPENAME(struct record29 *TYPEINDEX, PBoolean USEORIGINAL) {
  PString return_DEEPTYPENAME;
  TPSTYPE TYP;
  PString RET;
  int POS;
  do {
    TYP = *TYPEINDEX;
    TYPEINDEX = TYP.ALIASFOR;
  } while (!(!USEORIGINAL || TYPEINDEX == (void*)0));
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) return_DEEPTYPENAME = TYP.NAME;
  else if (TYP.CLS == TTCENUM) {
    RET = str_of('(');
    do {
      int first = 1;
      int last = TYP.ENUMINDEX->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 1) RET = cat_sc(RET, ',');
            RET = cat_ss(RET, TYP.ENUMINDEX->VALUES[POS - 1]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    return_DEEPTYPENAME = cat_sc(RET, ')');
  }
  else if (TYP.CLS == TTCRECORD) {
    RET = str_make(7, "record ");
    do {
      int first = 1;
      int last = TYP.RECORDINDEX->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 1) RET = cat_sc(RET, ',');
            RET = cat_ss(RET, DEEPTYPENAME(TYP.RECORDINDEX->FIELDS[POS - 1].TYPEINDEX, 1));
            RET = cat_ss(cat_sc(RET, ':'), TYP.RECORDINDEX->FIELDS[POS - 1].NAME);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    return_DEEPTYPENAME = cat_ss(RET, str_make(4, " end"));
  }
  else if (TYP.CLS == TTCARRAY) {
    RET = cat_ss(str_make(15, "array [...] of "), DEEPTYPENAME(TYP.ARRAYINDEX->TYPEINDEX, 1));
    return_DEEPTYPENAME = RET;
  }
  else if (TYP.CLS == TTCPOINTER) return_DEEPTYPENAME = cat_cs('^', DEEPTYPENAME(TYP.POINTEDTYPEINDEX, 1));
  else {
    RET = to_str_e(TYP.CLS, enumvalues28);
    COMPILEERROR(cat_ss(str_make(37, "Could not get name for type of class "), RET));
  }
  return return_DEEPTYPENAME;
}

PString TYPENAME(struct record29 *TYPEINDEX) {
  PString return_TYPENAME;
  if (TYPEINDEX == (void*)0) return_TYPENAME = str_make(6, "(none)");
  else return_TYPENAME = DEEPTYPENAME(TYPEINDEX, 0);
  return return_TYPENAME;
}

TPSTYPE EMPTYTYPE() {
  TPSTYPE return_EMPTYTYPE;
  TPSTYPE RET;
  RET.NAME = str_make(0, "");
  RET.CLS = TTCBOOLEAN;
  RET.ALIASFOR = (void*)0;
  return_EMPTYTYPE = RET;
  return return_EMPTYTYPE;
}

TPSTYPE COPYTYPE(struct record29 *TYPEINDEX) {
  TPSTYPE return_COPYTYPE;
  TPSTYPE NEWTYP;
  NEWTYP = *TYPEINDEX;
  if (NEWTYP.CLS == TTCPOINTERUNKNOWN) {
    NEWTYP.TARGETNAME = malloc(sizeof(PString));
    *NEWTYP.TARGETNAME = *TYPEINDEX->TARGETNAME;
  }
  return_COPYTYPE = NEWTYP;
  return return_COPYTYPE;
}

TPSTYPE TYPEOFCLASS(TPSTYPECLASS CLS) {
  TPSTYPE return_TYPEOFCLASS;
  TPSTYPE RET;
  RET = EMPTYTYPE();
  RET.CLS = CLS;
  return_TYPEOFCLASS = RET;
  return return_TYPEOFCLASS;
}

TPSTYPE INTEGERTYPE() {
  TPSTYPE return_INTEGERTYPE;
  return_INTEGERTYPE = TYPEOFCLASS(TTCINTEGER);
  return return_INTEGERTYPE;
}

PBoolean ISINTEGERTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISINTEGERTYPE;
  return_ISINTEGERTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCINTEGER;
  return return_ISINTEGERTYPE;
}

TPSTYPE STRINGTYPE() {
  TPSTYPE return_STRINGTYPE;
  return_STRINGTYPE = TYPEOFCLASS(TTCSTRING);
  return return_STRINGTYPE;
}

TPSTYPE CHARTYPE() {
  TPSTYPE return_CHARTYPE;
  return_CHARTYPE = TYPEOFCLASS(TTCCHAR);
  return return_CHARTYPE;
}

PBoolean ISSTRINGTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISSTRINGTYPE;
  return_ISSTRINGTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCSTRING;
  return return_ISSTRINGTYPE;
}

PBoolean ISCHARTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISCHARTYPE;
  return_ISCHARTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCCHAR;
  return return_ISCHARTYPE;
}

PBoolean ISSTRINGYTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISSTRINGYTYPE;
  return_ISSTRINGYTYPE = ISSTRINGTYPE(TYPEINDEX) || ISCHARTYPE(TYPEINDEX);
  return return_ISSTRINGYTYPE;
}

TPSTYPE BOOLEANTYPE() {
  TPSTYPE return_BOOLEANTYPE;
  return_BOOLEANTYPE = TYPEOFCLASS(TTCBOOLEAN);
  return return_BOOLEANTYPE;
}

PBoolean ISBOOLEANTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISBOOLEANTYPE;
  return_ISBOOLEANTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCBOOLEAN;
  return return_ISBOOLEANTYPE;
}

TPSTYPE TEXTTYPE() {
  TPSTYPE return_TEXTTYPE;
  return_TEXTTYPE = TYPEOFCLASS(TTCTEXT);
  return return_TEXTTYPE;
}

PBoolean ISTEXTTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISTEXTTYPE;
  return_ISTEXTTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCTEXT;
  return return_ISTEXTTYPE;
}

PBoolean ISENUMTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISENUMTYPE;
  return_ISENUMTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCENUM;
  return return_ISENUMTYPE;
}

PBoolean ISRECORDTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISRECORDTYPE;
  return_ISRECORDTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCRECORD;
  return return_ISRECORDTYPE;
}

PBoolean ISARRAYTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISARRAYTYPE;
  return_ISARRAYTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCARRAY;
  return return_ISARRAYTYPE;
}

TPSTYPE POINTERTYPE(struct record29 *TYPEINDEX) {
  TPSTYPE return_POINTERTYPE;
  TPSTYPE TYP;
  TYP = TYPEOFCLASS(TTCPOINTER);
  TYP.POINTEDTYPEINDEX = TYPEINDEX;
  return_POINTERTYPE = TYP;
  return return_POINTERTYPE;
}

PBoolean ISPOINTERTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISPOINTERTYPE;
  return_ISPOINTERTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCPOINTER;
  return return_ISPOINTERTYPE;
}

TPSTYPE NILTYPE() {
  TPSTYPE return_NILTYPE;
  return_NILTYPE = TYPEOFCLASS(TTCNIL);
  return return_NILTYPE;
}

PBoolean ISNILTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISNILTYPE;
  return_ISNILTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCNIL;
  return return_ISNILTYPE;
}

PBoolean ISPOINTERYTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISPOINTERYTYPE;
  return_ISPOINTERYTYPE = ISPOINTERTYPE(TYPEINDEX) || ISNILTYPE(TYPEINDEX);
  return return_ISPOINTERYTYPE;
}

TPSTYPE POINTERUNKNOWNTYPE(PString TARGETNAME) {
  TPSTYPE return_POINTERUNKNOWNTYPE;
  TPSTYPE TYP;
  TYP = TYPEOFCLASS(TTCPOINTERUNKNOWN);
  TYP.TARGETNAME = malloc(sizeof(PString));
  *TYP.TARGETNAME = TARGETNAME;
  return_POINTERUNKNOWNTYPE = TYP;
  return return_POINTERUNKNOWNTYPE;
}

PBoolean ISPOINTERUNKNOWNTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISPOINTERUNKNOWNTYPE;
  return_ISPOINTERUNKNOWNTYPE = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCPOINTERUNKNOWN;
  return return_ISPOINTERUNKNOWNTYPE;
}

PBoolean ISORDINALTYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISORDINALTYPE;
  return_ISORDINALTYPE = ISBOOLEANTYPE(TYPEINDEX) || ISINTEGERTYPE(TYPEINDEX) || ISCHARTYPE(TYPEINDEX) || ISENUMTYPE(TYPEINDEX);
  return return_ISORDINALTYPE;
}

PBoolean ISSIMPLETYPE(struct record29 *TYPEINDEX) {
  PBoolean return_ISSIMPLETYPE;
  return_ISSIMPLETYPE = ISORDINALTYPE(TYPEINDEX) || ISSTRINGTYPE(TYPEINDEX);
  return return_ISSIMPLETYPE;
}

PBoolean ISSAMETYPE(struct record29 *A, struct record29 *B) {
  PBoolean return_ISSAMETYPE;
  if (A == (void*)0 || B == (void*)0) return_ISSAMETYPE = A == B;
  else {
    while (A->ALIASFOR != (void*)0) A = A->ALIASFOR;
    while (B->ALIASFOR != (void*)0) B = B->ALIASFOR;
    return_ISSAMETYPE = A == B || ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE(A->POINTEDTYPEINDEX, B->POINTEDTYPEINDEX);
  }
  return return_ISSAMETYPE;
}

PBoolean AREPOINTERSCOMPATIBLE(struct record29 *A, struct record29 *B) {
  PBoolean return_AREPOINTERSCOMPATIBLE;
  return_AREPOINTERSCOMPATIBLE = ISPOINTERYTYPE(A) && ISPOINTERYTYPE(B) && (ISNILTYPE(A) || ISNILTYPE(B) || ISSAMETYPE(A, B));
  return return_AREPOINTERSCOMPATIBLE;
}

struct record29 *ADDTYPE(TPSTYPE TYP) {
  struct record29 *return_ADDTYPE;
  struct record29 *TYPEINDEX;
  int ENUMPOS;
  TYPEINDEX = _ADDDEF(TDCTYPE)->TYPEINDEX;
  *TYPEINDEX = TYP;
  return_ADDTYPE = TYPEINDEX;
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) {
    if (FINDNAMEINLOCALSCOPE(TYP.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), TYP.NAME), str_make(16, " already defined")));
    ADDTYPENAME(TYP.NAME, TYPEINDEX);
  }
  if (TYP.CLS == TTCENUM && TYP.ALIASFOR == (void*)0) do {
    int first = 1;
    int last = TYP.ENUMINDEX->SIZE;
    if (first <= last) {
      ENUMPOS = first;
      while (1) {
        ADDENUMVALUENAME(ENUMPOS, TYPEINDEX);
        if (ENUMPOS == last) break;
        ++ENUMPOS;
      }
    }
  } while(0);
  return return_ADDTYPE;
}

struct record30 *ADDENUM(TPSENUMDEF ENUM) {
  struct record30 *return_ADDENUM;
  struct record30 *ENUMINDEX;
  ENUMINDEX = _ADDDEF(TDCENUM)->ENUMINDEX;
  *ENUMINDEX = ENUM;
  ENUMINDEX->ID = DEFCOUNTER();
  return_ADDENUM = ENUMINDEX;
  return return_ADDENUM;
}

struct record32 *ADDRECORD(TPSRECORDDEF REC) {
  struct record32 *return_ADDRECORD;
  struct record32 *RECORDINDEX;
  RECORDINDEX = _ADDDEF(TDCRECORD)->RECORDINDEX;
  *RECORDINDEX = REC;
  RECORDINDEX->ID = DEFCOUNTER();
  return_ADDRECORD = RECORDINDEX;
  return return_ADDRECORD;
}

struct record33 *ADDARRAY(TPSARRAYDEF ARR) {
  struct record33 *return_ADDARRAY;
  struct record33 *ARRAYINDEX;
  ARRAYINDEX = _ADDDEF(TDCARRAY)->ARRAYINDEX;
  *ARRAYINDEX = ARR;
  return_ADDARRAY = ARRAYINDEX;
  return return_ADDARRAY;
}

struct record34 *FINDCONSTANT(PString NAME) {
  struct record34 *return_FINDCONSTANT;
  struct record41 *DEF;
  struct record34 *RET;
  RET = (void*)0;
  DEF = DEFS.LATEST;
  while (RET == (void*)0 && DEF != (void*)0) {
    if (DEF->CLS == TDCCONSTANT && cmp_ss(NAME, DEF->CONSTANTINDEX->NAME) == 0) RET = DEF->CONSTANTINDEX;
    DEF = DEF->PREV;
  }
  return_FINDCONSTANT = RET;
  return return_FINDCONSTANT;
}

struct record34 *ADDCONSTANT(TPSCONSTANT CONSTANT) {
  struct record34 *return_ADDCONSTANT;
  struct record34 *CONSTANTINDEX;
  if (FINDCONSTANT(CONSTANT.NAME) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(9, "Constant "), CONSTANT.NAME), str_make(16, " already defined")));
  CONSTANTINDEX = _ADDDEF(TDCCONSTANT)->CONSTANTINDEX;
  *CONSTANTINDEX = CONSTANT;
  return_ADDCONSTANT = CONSTANTINDEX;
  return return_ADDCONSTANT;
}

struct record35 *ADDVARIABLE(TPSVARIABLE VARDEF) {
  struct record35 *return_ADDVARIABLE;
  struct record35 *VARIABLEINDEX;
  if (FINDNAMEINLOCALSCOPE(VARDEF.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), VARDEF.NAME), str_make(16, " already defined")));
  VARIABLEINDEX = _ADDDEF(TDCVARIABLE)->VARIABLEINDEX;
  ADDVARIABLENAME(VARDEF.NAME, VARIABLEINDEX);
  *VARIABLEINDEX = VARDEF;
  return_ADDVARIABLE = VARIABLEINDEX;
  return return_ADDVARIABLE;
}

TPSFUNCTION EMPTYFUNCTION() {
  TPSFUNCTION return_EMPTYFUNCTION;
  TPSFUNCTION RET;
  RET.NAME = str_make(0, "");
  RET.ARGCOUNT = 0;
  RET.RETURNTYPEINDEX = (void*)0;
  RET.ISDECLARATION = 0;
  return_EMPTYFUNCTION = RET;
  return return_EMPTYFUNCTION;
}

PBoolean ISEMPTYFUNCTION(TPSFUNCTION FN) {
  PBoolean return_ISEMPTYFUNCTION;
  return_ISEMPTYFUNCTION = cmp_ss(FN.NAME, str_make(0, "")) == 0;
  return return_ISEMPTYFUNCTION;
}

PBoolean ISSAMEFUNCTIONDEFINITION(struct record36 *DECLINDEX, TPSFUNCTION FUN) {
  PBoolean return_ISSAMEFUNCTIONDEFINITION;
  TPSFUNCTION DECL;
  PBoolean SAME;
  int POS;
  DECL = *DECLINDEX;
  SAME = ISSAMETYPE(DECL.RETURNTYPEINDEX, FUN.RETURNTYPEINDEX) && DECL.ARGCOUNT == FUN.ARGCOUNT;
  do {
    int first = 1;
    int last = DECL.ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        SAME = SAME && ISSAMETYPE(DECL.ARGS[POS - 1].TYPEINDEX, FUN.ARGS[POS - 1].TYPEINDEX) && DECL.ARGS[POS - 1].ISREFERENCE == FUN.ARGS[POS - 1].ISREFERENCE;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return_ISSAMEFUNCTIONDEFINITION = SAME;
  return return_ISSAMEFUNCTIONDEFINITION;
}

PBoolean HASFORWARDDECLARATION(PString NAME) {
  PBoolean return_HASFORWARDDECLARATION;
  struct record39 *NAMEINDEX;
  NAMEINDEX = FINDNAMEOFCLASSINLOCALSCOPE(NAME, TNCFUNCTION, 0);
  return_HASFORWARDDECLARATION = NAMEINDEX != (void*)0 && NAMEINDEX->FUNCTIONINDEX->ISDECLARATION;
  return return_HASFORWARDDECLARATION;
}

struct record36 *ADDFUNCTION(TPSFUNCTION FUN) {
  struct record36 *return_ADDFUNCTION;
  struct record39 *NAMEINDEX;
  struct record36 *FNINDEX;
  PBoolean ISPROCEDURE;
  ISPROCEDURE = FUN.RETURNTYPEINDEX == (void*)0;
  NAMEINDEX = FINDNAMEINLOCALSCOPE(FUN.NAME, 0);
  if (NAMEINDEX == (void*)0) {
    FNINDEX = _ADDDEF(TDCFUNCTION)->FUNCTIONINDEX;
    *FNINDEX = FUN;
    ADDFUNCTIONNAME(FUN.NAME, FNINDEX);
  }
  else {
    if (NAMEINDEX->CLS != TNCFUNCTION || FUN.ISDECLARATION) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), FUN.NAME), str_make(16, " already defined")));
    FNINDEX = NAMEINDEX->FUNCTIONINDEX;
    if (FNINDEX->ISDECLARATION) {
      if (FUN.ARGCOUNT == 0 && FUN.RETURNTYPEINDEX == (void*)0 || ISSAMEFUNCTIONDEFINITION(FNINDEX, FUN)) FNINDEX->ISDECLARATION = 0;
      else {
        if (ISPROCEDURE) COMPILEERROR(cat_ss(cat_ss(str_make(10, "Procedure "), FUN.NAME), str_make(42, " incompatible with its forward declaration")));
        else COMPILEERROR(cat_ss(cat_ss(str_make(9, "Function "), FUN.NAME), str_make(42, " incompatible with its forward declaration")));
      }
    }
    else {
      if (ISPROCEDURE) COMPILEERROR(cat_ss(cat_ss(str_make(10, "Procedure "), FUN.NAME), str_make(16, " already defined")));
      else COMPILEERROR(cat_ss(cat_ss(str_make(9, "Function "), FUN.NAME), str_make(16, " already defined")));
    }
  }
  return_ADDFUNCTION = FNINDEX;
  return return_ADDFUNCTION;
}

int FINDFIELD(struct record29 *TYPEINDEX, PString NAME, PBoolean REQUIRED) {
  int return_FINDFIELD;
  int POS;
  int RET;
  if (TYPEINDEX->CLS != TTCRECORD) COMPILEERROR(cat_ss(str_make(14, "Not a record: "), TYPEINDEX->NAME));
  {
    TPSRECORDDEF *with44 = &*TYPEINDEX->RECORDINDEX;
    {
      RET = 0;
      POS = (*with44).SIZE;
      while (POS >= 1 && RET == 0) {
        if (cmp_ss(NAME, (*with44).FIELDS[POS - 1].NAME) == 0) RET = POS;
        POS = POS - 1;
      }
    }
  }
  if (REQUIRED && RET == 0) COMPILEERROR(cat_ss(str_make(17, "Field not found: "), NAME));
  return_FINDFIELD = RET;
  return return_FINDFIELD;
}

struct record29 *FINDFIELDTYPE(struct record29 *TYPEINDEX, PString NAME, PBoolean REQUIRED) {
  struct record29 *return_FINDFIELDTYPE;
  int POS;
  POS = FINDFIELD(TYPEINDEX, NAME, REQUIRED);
  if (POS == 0) return_FINDFIELDTYPE = (void*)0;
  else return_FINDFIELDTYPE = TYPEINDEX->RECORDINDEX->FIELDS[POS - 1].TYPEINDEX;
  return return_FINDFIELDTYPE;
}

struct record37 *FINDWITHVAR(PString NAME) {
  struct record37 *return_FINDWITHVAR;
  struct record37 *RET;
  struct record41 *DEF;
  struct record29 *TYPEINDEX;
  RET = (void*)0;
  DEF = DEFS.LATEST;
  while (RET == (void*)0 && DEF != (void*)0 && DEF->CLS != TDCSCOPEBOUNDARY) {
    if (DEF->CLS == TDCWITHVAR) {
      TYPEINDEX = DEF->WITHVARINDEX->VARIABLEINDEX->TYPEINDEX;
      if (FINDFIELDTYPE(TYPEINDEX, NAME, 0) != (void*)0) RET = DEF->WITHVARINDEX;
    }
    DEF = DEF->PREV;
  }
  return_FINDWITHVAR = RET;
  return return_FINDWITHVAR;
}

struct record35 *ADDWITHVAR(struct record26 *BASE) {
  struct record35 *return_ADDWITHVAR;
  PString TMPVARNUM;
  TPSVARIABLE TMPVAR;
  struct record35 *TMPVARINDEX;
  struct record37 *WITHVARINDEX;
  if (!ISRECORDTYPE(BASE->TYPEINDEX)) COMPILEERROR(str_make(31, "'With' variable is not a record"));
  TMPVARNUM = to_str_i(DEFCOUNTER());
  TMPVAR.NAME = cat_ss(str_make(4, "with"), TMPVARNUM);
  TMPVAR.TYPEINDEX = BASE->TYPEINDEX;
  TMPVAR.ISCONSTANT = BASE->ISCONSTANT;
  TMPVAR.ISREFERENCE = 1;
  TMPVARINDEX = ADDVARIABLE(TMPVAR);
  WITHVARINDEX = _ADDDEF(TDCWITHVAR)->WITHVARINDEX;
  WITHVARINDEX->VARIABLEINDEX = TMPVARINDEX;
  return_ADDWITHVAR = TMPVARINDEX;
  return return_ADDWITHVAR;
}

TPSTYPE MAKETYPE(PString NAME, TPSTYPECLASS CLS) {
  TPSTYPE return_MAKETYPE;
  TPSTYPE TYP;
  TYP = EMPTYTYPE();
  TYP.NAME = NAME;
  TYP.CLS = CLS;
  return_MAKETYPE = TYP;
  return return_MAKETYPE;
}

TPSCONSTANT MAKECONSTANT(PString NAME, TLXTOKENID TOKENID, PString TOKENVALUE) {
  TPSCONSTANT return_MAKECONSTANT;
  TPSCONSTANT CONSTANT;
  CONSTANT.NAME = NAME;
  CONSTANT.REPLACEMENT.ID = TOKENID;
  CONSTANT.REPLACEMENT.VALUE = TOKENVALUE;
  return_MAKECONSTANT = CONSTANT;
  return return_MAKECONSTANT;
}

TPSVARIABLE MAKETYPEDCONSTANT(PString NAME, struct record29 *TYPEINDEX) {
  TPSVARIABLE return_MAKETYPEDCONSTANT;
  TPSVARIABLE VARDEF;
  VARDEF.NAME = NAME;
  VARDEF.TYPEINDEX = TYPEINDEX;
  VARDEF.ISREFERENCE = 0;
  VARDEF.ISCONSTANT = 1;
  return_MAKETYPEDCONSTANT = VARDEF;
  return return_MAKETYPEDCONSTANT;
}

TPSVARIABLE MAKEVARIABLE(PString NAME, struct record29 *TYPEINDEX, PBoolean ISREF) {
  TPSVARIABLE return_MAKEVARIABLE;
  TPSVARIABLE VARDEF;
  VARDEF.NAME = NAME;
  VARDEF.TYPEINDEX = TYPEINDEX;
  VARDEF.ISREFERENCE = ISREF;
  VARDEF.ISCONSTANT = 0;
  return_MAKEVARIABLE = VARDEF;
  return return_MAKEVARIABLE;
}

void CODEGENRESET();
void CODEGENSETOUTPUT(PString FILENAME);
void OUTBEGIN();
void OUTEND();
void OUTENUMVALUES(struct record30 *ENUMINDEX);
void OUTENUMVALUESFROMCHECKPOINT(struct record41 *CHECKPOINT);
PString OUTVARIABLENAME(PString NAME, PBoolean ISREFERENCE);
PString OUTRETURNVARIABLENAME(PString NAME);
void OUTTYPEREFERENCE(struct record29 *TYPEINDEX);
void OUTNAMEANDTYPE(PString NAME, struct record29 *TYPEINDEX);
void OUTTYPEDEFINITION(struct record29 *TYPEINDEX);
void OUTTYPEDEFINITIONSFROMCHECKPOINT(struct record41 *CHECKPOINT);
void OUTCONSTANTARRAYBEGIN();
void OUTCONSTANTARRAYSEPARATOR();
void OUTCONSTANTARRAYEND();
void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF);
void OUTVARIABLEDEFINITION(struct record35 *VARINDEX);
void OUTCONSTANTDEFINITIONBEGIN(struct record35 *VARINDEX);
void OUTCONSTANTDEFINITIONEND();
void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF);
void OUTFUNCTIONDECLARATION(struct record36 *FNINDEX);
void OUTFUNCTIONDEFINITION(struct record36 *FNINDEX);
void OUTFUNCTIONEND(struct record36 *FNINDEX);
void OUTPROGRAMHEADING(PString NAME);
void OUTEXPRESSION(struct record26 *EXPR);
void OUTASSIGN(struct record26 *LHS, struct record26 *RHS);
void OUTASSIGNRETURNVALUE(struct record26 *LHS, struct record26 *RHS);
void OUTASSIGNTOREFERENCE(struct record35 *VARINDEX, struct record26 *RHS);
void OUTIF(struct record26 *EXPR);
void OUTELSE();
void OUTIFEND();
void OUTSEQUENCEBEGIN();
void OUTSEQUENCEEND();
void OUTCASEBEGIN(struct record26 *CASEINDEX);
void OUTCASESTATEMENTBEGIN(struct record26 *CASELABEL);
void OUTCASESTATEMENTEND();
void OUTCASEELSEBEGIN();
void OUTCASEELSEEND();
void OUTCASEEND();
void OUTREPEATBEGIN();
void OUTREPEATEND(struct record26 *EXPR);
void OUTWHILEBEGIN(struct record26 *EXPR);
void OUTWHILEEND();
void OUTFORBEGIN(struct record26 *ITER, struct record26 *FIRSTEXPR, struct record26 *LASTEXPR, PBoolean ASCENDING);
void OUTFOREND(struct record26 *ITER, PBoolean ASCENDING);
void OUTPROCEDURECALL(struct record26 *EXPR);
void OUTSPECIALPROCEDURECALL(struct record26 *EXPR);
void OUTEMPTYSTATEMENT();
void OUTPROGRAMBEGIN();
void OUTPROGRAMEND();

struct record26 *_NEWEXPR(TEXPRESSIONCLASS CLS) {
  struct record26 *return__NEWEXPR;
  struct record26 *EXPR;
  EXPR = malloc(sizeof(struct record26));
  EXPR->CLS = CLS;
  EXPR->ISCONSTANT = 0;
  EXPR->ISASSIGNABLE = 0;
  EXPR->ISFUNCTIONRESULT = 0;
  return__NEWEXPR = EXPR;
  return return__NEWEXPR;
}

void DISPOSEEXPR(struct record26 **EXPR);

void _DISPOSESPECIALCALLEXPR(TEXSPECIALFUNCTIONCALL *CALL) {
  struct record20 *READARG;
  struct record20 *NEXTREADARG;
  struct record21 *WRITEARG;
  struct record21 *NEXTWRITEARG;
  if ((*CALL).SRC != (void*)0) DISPOSEEXPR(&(*CALL).SRC);
  if ((*CALL).DST != (void*)0) DISPOSEEXPR(&(*CALL).DST);
  if ((*CALL).PTR != (void*)0) DISPOSEEXPR(&(*CALL).PTR);
  if ((*CALL).SPECIALFUNCTION == TSFWRITE || (*CALL).SPECIALFUNCTION == TSFWRITELN) {
    READARG = (*CALL).READARGS;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      DISPOSEEXPR(&READARG->ARG);
      free(READARG);
      READARG = NEXTREADARG;
    }
  }
  else if ((*CALL).SPECIALFUNCTION == TSFREAD || (*CALL).SPECIALFUNCTION == TSFREADLN) {
    WRITEARG = (*CALL).WRITEARGS;
    while (WRITEARG != (void*)0) {
      NEXTWRITEARG = WRITEARG->NEXT;
      DISPOSEEXPR(&WRITEARG->ARG);
      free(WRITEARG);
      WRITEARG = NEXTWRITEARG;
    }
  }
}

void DISPOSEEXPR(struct record26 **EXPR) {
  int POS;
  switch ((*EXPR)->CLS) {
    case XCTOSTRING:
      DISPOSEEXPR(&(*EXPR)->TOSTRINGEX.PARENT);
      break;
    case XCFIELDACCESS:
      DISPOSEEXPR(&(*EXPR)->FIELDEX.PARENT);
      break;
    case XCARRAYACCESS:
      {
        DISPOSEEXPR(&(*EXPR)->ARRAYEX.PARENT);
        DISPOSEEXPR(&(*EXPR)->ARRAYEX.SUBSCRIPT);
      }
      break;
    case XCPOINTERACCESS:
      DISPOSEEXPR(&(*EXPR)->POINTEREX.PARENT);
      break;
    case XCSTRINGCHAR:
      {
        DISPOSEEXPR(&(*EXPR)->STRINGCHAREX.PARENT);
        DISPOSEEXPR(&(*EXPR)->STRINGCHAREX.SUBSCRIPT);
      }
      break;
    case XCFUNCTIONCALL:
      {
        DISPOSEEXPR(&(*EXPR)->CALLEX.FUNCTIONREF);
        do {
          int first = 1;
          int last = (*EXPR)->CALLEX.ARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              DISPOSEEXPR(&(*EXPR)->CALLEX.ARGS.VALUES[POS - 1]);
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
      }
      break;
    case XCSPECIALFUNCTIONCALL:
      _DISPOSESPECIALCALLEXPR(&(*EXPR)->SPECIALFUNCTIONCALLEX);
      break;
    case XCUNARYOP:
      DISPOSEEXPR(&(*EXPR)->UNARYEX.PARENT);
      break;
    case XCBINARYOP:
      {
        DISPOSEEXPR(&(*EXPR)->BINARYEX.LEFT);
        DISPOSEEXPR(&(*EXPR)->BINARYEX.RIGHT);
      }
      break;
    default:
      break;
  }
  free(*EXPR);
}

struct record26 *COPYEXPR(struct record26 *EXPR);

void _COPYSPECIALCALLEXPR(TEXSPECIALFUNCTIONCALL *CALL, TEXSPECIALFUNCTIONCALL *COPY) {
  struct record20 *READARG;
  struct record20 *NEXTREADARG;
  struct record20 *COPYREADARG;
  struct record21 *WRITEARG;
  struct record21 *NEXTWRITEARG;
  struct record21 *COPYWRITEARG;
  (*COPY).SPECIALFUNCTION = (*CALL).SPECIALFUNCTION;
  if ((*CALL).SRC != (void*)0) (*COPY).SRC = COPYEXPR((*CALL).SRC);
  if ((*CALL).DST != (void*)0) (*COPY).DST = COPYEXPR((*CALL).DST);
  if ((*CALL).PTR != (void*)0) (*COPY).PTR = COPYEXPR((*CALL).PTR);
  if ((*CALL).SPECIALFUNCTION == TSFWRITE || (*CALL).SPECIALFUNCTION == TSFWRITELN) {
    READARG = (*CALL).READARGS;
    COPYREADARG = (void*)0;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      if (COPYREADARG == (void*)0) {
        COPYREADARG = malloc(sizeof(struct record20));
        (*COPY).READARGS = COPYREADARG;
      }
      else {
        COPYREADARG->NEXT = malloc(sizeof(struct record20));
        COPYREADARG = COPYREADARG->NEXT;
      }
      COPYREADARG->NEXT = (void*)0;
      COPYREADARG->ARG = COPYEXPR(READARG->ARG);
      READARG = NEXTREADARG;
    }
  }
  else if ((*CALL).SPECIALFUNCTION == TSFREAD || (*CALL).SPECIALFUNCTION == TSFREADLN) {
    WRITEARG = (*CALL).WRITEARGS;
    while (WRITEARG != (void*)0) {
      NEXTWRITEARG = WRITEARG->NEXT;
      if (COPYWRITEARG == (void*)0) {
        COPYWRITEARG = malloc(sizeof(struct record21));
        (*COPY).WRITEARGS = COPYWRITEARG;
      }
      else {
        COPYWRITEARG->NEXT = malloc(sizeof(struct record21));
        COPYWRITEARG = COPYWRITEARG->NEXT;
      }
      COPYWRITEARG->NEXT = (void*)0;
      COPYWRITEARG->ARG = COPYEXPR(WRITEARG->ARG);
      WRITEARG = NEXTWRITEARG;
    }
  }
}

struct record26 *COPYEXPR(struct record26 *EXPR) {
  struct record26 *return_COPYEXPR;
  struct record26 *COPY;
  int POS;
  COPY = _NEWEXPR(EXPR->CLS);
  COPY->TYPEINDEX = EXPR->TYPEINDEX;
  COPY->ISCONSTANT = EXPR->ISCONSTANT;
  COPY->ISASSIGNABLE = EXPR->ISASSIGNABLE;
  COPY->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      COPY->IMMEDIATEEX = EXPR->IMMEDIATEEX;
      break;
    case XCTOSTRING:
      COPY->TOSTRINGEX.PARENT = COPYEXPR(EXPR->TOSTRINGEX.PARENT);
      break;
    case XCVARIABLEACCESS:
      COPY->VARIABLEEX = EXPR->VARIABLEEX;
      break;
    case XCFIELDACCESS:
      {
        COPY->FIELDEX.PARENT = COPYEXPR(EXPR->FIELDEX.PARENT);
        COPY->FIELDEX.FIELDNUMBER = EXPR->FIELDEX.FIELDNUMBER;
      }
      break;
    case XCARRAYACCESS:
      {
        COPY->ARRAYEX.PARENT = COPYEXPR(EXPR->ARRAYEX.PARENT);
        COPY->ARRAYEX.SUBSCRIPT = COPYEXPR(EXPR->ARRAYEX.SUBSCRIPT);
      }
      break;
    case XCPOINTERACCESS:
      COPY->POINTEREX.PARENT = COPYEXPR(EXPR->POINTEREX.PARENT);
      break;
    case XCSTRINGCHAR:
      {
        COPY->STRINGCHAREX.PARENT = COPYEXPR(EXPR->STRINGCHAREX.PARENT);
        COPY->STRINGCHAREX.SUBSCRIPT = COPYEXPR(EXPR->STRINGCHAREX.SUBSCRIPT);
      }
      break;
    case XCFUNCTIONREF:
      COPY->FUNCTIONEX.FUNCTIONINDEX = EXPR->FUNCTIONEX.FUNCTIONINDEX;
      break;
    case XCFUNCTIONCALL:
      {
        COPY->CALLEX.FUNCTIONREF = COPYEXPR(EXPR->CALLEX.FUNCTIONREF);
        COPY->CALLEX.ARGS.SIZE = EXPR->CALLEX.ARGS.SIZE;
        do {
          int first = 1;
          int last = EXPR->CALLEX.ARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              COPY->CALLEX.ARGS.VALUES[POS - 1] = COPYEXPR(EXPR->CALLEX.ARGS.VALUES[POS - 1]);
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
      }
      break;
    case XCSPECIALFUNCTIONREF:
      COPY->SPECIALFUNCTIONEX = EXPR->SPECIALFUNCTIONEX;
      break;
    case XCSPECIALFUNCTIONCALL:
      _COPYSPECIALCALLEXPR(&EXPR->SPECIALFUNCTIONCALLEX, &COPY->SPECIALFUNCTIONCALLEX);
      break;
    case XCUNARYOP:
      {
        COPY->UNARYEX.PARENT = COPYEXPR(EXPR->UNARYEX.PARENT);
        COPY->UNARYEX.OP = EXPR->UNARYEX.OP;
      }
      break;
    case XCBINARYOP:
      {
        COPY->BINARYEX.LEFT = COPYEXPR(EXPR->BINARYEX.LEFT);
        COPY->BINARYEX.RIGHT = COPYEXPR(EXPR->BINARYEX.RIGHT);
        COPY->BINARYEX.OP = EXPR->BINARYEX.OP;
      }
      break;
    default:
      break;
  }
  return_COPYEXPR = COPY;
  return return_COPYEXPR;
}

struct record26 *EXCOERCE(struct record26 *EXPR, struct record29 *TYPEINDEX);

struct record26 *_EXIMMEDIATE(TEXIMMEDIATECLASS CLS) {
  struct record26 *return__EXIMMEDIATE;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCIMMEDIATE);
  EXPR->ISCONSTANT = 1;
  EXPR->IMMEDIATEEX.CLS = CLS;
  return__EXIMMEDIATE = EXPR;
  return return__EXIMMEDIATE;
}

struct record26 *EXNIL() {
  struct record26 *return_EXNIL;
  struct record26 *EXPR;
  EXPR = _EXIMMEDIATE(XICNIL);
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTNIL;
  return_EXNIL = EXPR;
  return return_EXNIL;
}

struct record26 *EXBOOLEANCONSTANT(PBoolean VALUE) {
  struct record26 *return_EXBOOLEANCONSTANT;
  struct record26 *EXPR;
  EXPR = _EXIMMEDIATE(XICBOOLEAN);
  EXPR->IMMEDIATEEX.BOOLEANVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  return_EXBOOLEANCONSTANT = EXPR;
  return return_EXBOOLEANCONSTANT;
}

struct record26 *EXINTEGERCONSTANT(int VALUE) {
  struct record26 *return_EXINTEGERCONSTANT;
  struct record26 *EXPR;
  EXPR = _EXIMMEDIATE(XICINTEGER);
  EXPR->IMMEDIATEEX.INTEGERVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  return_EXINTEGERCONSTANT = EXPR;
  return return_EXINTEGERCONSTANT;
}

struct record26 *EXCHARCONSTANT(char VALUE) {
  struct record26 *return_EXCHARCONSTANT;
  struct record26 *EXPR;
  EXPR = _EXIMMEDIATE(XICCHAR);
  EXPR->IMMEDIATEEX.CHARVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTCHAR;
  return_EXCHARCONSTANT = EXPR;
  return return_EXCHARCONSTANT;
}

struct record26 *EXSTRINGCONSTANT(PString VALUE) {
  struct record26 *return_EXSTRINGCONSTANT;
  struct record26 *EXPR;
  EXPR = _EXIMMEDIATE(XICSTRING);
  EXPR->IMMEDIATEEX.STRINGVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
  return_EXSTRINGCONSTANT = EXPR;
  return return_EXSTRINGCONSTANT;
}

struct record26 *EXENUMCONSTANT(int ORDINAL, struct record29 *TYPEINDEX) {
  struct record26 *return_EXENUMCONSTANT;
  struct record26 *EXPR;
  if (!ISENUMTYPE(TYPEINDEX)) COMPILEERROR(cat_ss(str_make(25, "Not an enumeration type: "), TYPENAME(TYPEINDEX)));
  if (ORDINAL < 1 || ORDINAL > TYPEINDEX->ENUMINDEX->SIZE) COMPILEERROR(cat_ss(str_make(18, "Invalid value for "), TYPENAME(TYPEINDEX)));
  EXPR = _EXIMMEDIATE(XICENUM);
  EXPR->IMMEDIATEEX.ENUMORDINAL = ORDINAL;
  EXPR->TYPEINDEX = TYPEINDEX;
  return_EXENUMCONSTANT = EXPR;
  return return_EXENUMCONSTANT;
}

struct record26 *EXTOSTRING(struct record26 *PARENT) {
  struct record26 *return_EXTOSTRING;
  struct record26 *EXPR;
  PString STR;
  if (ISCHARTYPE(PARENT->TYPEINDEX)) {
    if (PARENT->CLS == XCIMMEDIATE) {
      STR = str_of(PARENT->IMMEDIATEEX.CHARVALUE);
      PARENT->IMMEDIATEEX.CLS = XICSTRING;
      PARENT->IMMEDIATEEX.STRINGVALUE = STR;
      return_EXTOSTRING = PARENT;
    }
    else {
      EXPR = _NEWEXPR(XCTOSTRING);
      EXPR->TOSTRINGEX.PARENT = PARENT;
      EXPR->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
      EXPR->ISASSIGNABLE = 0;
      EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
      return_EXTOSTRING = EXPR;
    }
  }
  else if (ISSTRINGTYPE(PARENT->TYPEINDEX)) return_EXTOSTRING = PARENT;
  else COMPILEERROR(cat_ss(str_make(47, "Cannot convert a value of this type to string: "), TYPENAME(PARENT->TYPEINDEX)));
  return return_EXTOSTRING;
}

struct record26 *EXVARIABLE(struct record35 *VARIABLEINDEX) {
  struct record26 *return_EXVARIABLE;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCVARIABLEACCESS);
  EXPR->VARIABLEEX.VARIABLEINDEX = VARIABLEINDEX;
  EXPR->TYPEINDEX = VARIABLEINDEX->TYPEINDEX;
  EXPR->ISCONSTANT = VARIABLEINDEX->ISCONSTANT;
  EXPR->ISASSIGNABLE = 1;
  return_EXVARIABLE = EXPR;
  return return_EXVARIABLE;
}

struct record26 *EXFIELDACCESS(struct record26 *PARENT, int FIELDNUMBER) {
  struct record26 *return_EXFIELDACCESS;
  struct record26 *EXPR;
  if (!ISRECORDTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(39, "Cannot access field of non-record type "), TYPENAME(PARENT->TYPEINDEX)));
  if (FIELDNUMBER < 1 || FIELDNUMBER > PARENT->TYPEINDEX->RECORDINDEX->SIZE) COMPILEERROR(cat_ss(str_make(18, "Invalid field for "), TYPENAME(PARENT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCFIELDACCESS);
  EXPR->FIELDEX.PARENT = PARENT;
  EXPR->FIELDEX.FIELDNUMBER = FIELDNUMBER;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX->RECORDINDEX->FIELDS[FIELDNUMBER - 1].TYPEINDEX;
  EXPR->ISCONSTANT = PARENT->ISCONSTANT;
  EXPR->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return_EXFIELDACCESS = EXPR;
  return return_EXFIELDACCESS;
}

struct record26 *EXARRAYACCESS(struct record26 *PARENT, struct record26 *SUBSCRIPT) {
  struct record26 *return_EXARRAYACCESS;
  struct record26 *EXPR;
  if (!ISARRAYTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(42, "Cannot access subscript of non-array type "), TYPENAME(PARENT->TYPEINDEX)));
  if (!ISINTEGERTYPE(SUBSCRIPT->TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(30, "Invalid type for subscript of "), TYPENAME(PARENT->TYPEINDEX)), str_make(2, ": ")), TYPENAME(SUBSCRIPT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCARRAYACCESS);
  EXPR->ARRAYEX.PARENT = PARENT;
  EXPR->ARRAYEX.SUBSCRIPT = SUBSCRIPT;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX->ARRAYINDEX->TYPEINDEX;
  EXPR->ISCONSTANT = PARENT->ISCONSTANT;
  EXPR->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return_EXARRAYACCESS = EXPR;
  return return_EXARRAYACCESS;
}

struct record26 *EXPOINTERACCESS(struct record26 *PARENT) {
  struct record26 *return_EXPOINTERACCESS;
  struct record26 *EXPR;
  if (!ISPOINTERTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(36, "Cannot dereference non-pointer type "), TYPENAME(PARENT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCPOINTERACCESS);
  EXPR->POINTEREX.PARENT = PARENT;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX->POINTEDTYPEINDEX;
  EXPR->ISCONSTANT = 0;
  EXPR->ISASSIGNABLE = 1;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return_EXPOINTERACCESS = EXPR;
  return return_EXPOINTERACCESS;
}

struct record26 *EXSTRINGCHAR(struct record26 *PARENT, struct record26 *SUBSCRIPT) {
  struct record26 *return_EXSTRINGCHAR;
  struct record26 *EXPR;
  if (!ISSTRINGYTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(43, "Cannot access subscript of non-string type "), TYPENAME(PARENT->TYPEINDEX)));
  if (!ISINTEGERTYPE(SUBSCRIPT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(38, "Invalid type for subscript of string: "), TYPENAME(SUBSCRIPT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCSTRINGCHAR);
  EXPR->ARRAYEX.PARENT = EXTOSTRING(PARENT);
  EXPR->ARRAYEX.SUBSCRIPT = SUBSCRIPT;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTCHAR;
  EXPR->ISCONSTANT = PARENT->ISCONSTANT;
  EXPR->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return_EXSTRINGCHAR = EXPR;
  return return_EXSTRINGCHAR;
}

struct record26 *EXFUNCTIONREF(struct record36 *FUNCTIONINDEX) {
  struct record26 *return_EXFUNCTIONREF;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCFUNCTIONREF);
  EXPR->FUNCTIONEX.FUNCTIONINDEX = FUNCTIONINDEX;
  EXPR->TYPEINDEX = (void*)0;
  EXPR->ISCONSTANT = 1;
  return_EXFUNCTIONREF = EXPR;
  return return_EXFUNCTIONREF;
}

struct record26 *EXFUNCTIONCALL(struct record26 *FUNCTIONREF, TEXFUNCTIONARGS *ARGS) {
  struct record26 *return_EXFUNCTIONCALL;
  struct record26 *EXPR;
  int POS;
  if (FUNCTIONREF->CLS != XCFUNCTIONREF) COMPILEERROR(str_make(24, "Cannot call non-function"));
  {
    TEXFUNCTIONREF *with45 = &FUNCTIONREF->FUNCTIONEX;
    {
      if ((*ARGS).SIZE != (*with45).FUNCTIONINDEX->ARGCOUNT) COMPILEERROR(str_make(42, "Wrong number of arguments in function call"));
      EXPR = _NEWEXPR(XCFUNCTIONCALL);
      EXPR->CALLEX.FUNCTIONREF = FUNCTIONREF;
      EXPR->CALLEX.ARGS.SIZE = (*ARGS).SIZE;
      do {
        int first = 1;
        int last = (*ARGS).SIZE;
        if (first <= last) {
          POS = first;
          while (1) {
            {
              EXPR->CALLEX.ARGS.VALUES[POS - 1] = EXCOERCE((*ARGS).VALUES[POS - 1], (*with45).FUNCTIONINDEX->ARGS[POS - 1].TYPEINDEX);
              if ((*with45).FUNCTIONINDEX->ARGS[POS - 1].ISREFERENCE && (EXPR->CALLEX.ARGS.VALUES[POS - 1]->ISCONSTANT || !EXPR->CALLEX.ARGS.VALUES[POS - 1]->ISASSIGNABLE)) COMPILEERROR(str_make(45, "Pass-by-reference argument must be assignable"));
            }
            if (POS == last) break;
            ++POS;
          }
        }
      } while(0);
      EXPR->TYPEINDEX = (*with45).FUNCTIONINDEX->RETURNTYPEINDEX;
    }
  }
  EXPR->ISCONSTANT = 0;
  EXPR->ISASSIGNABLE = 0;
  EXPR->ISFUNCTIONRESULT = 1;
  return_EXFUNCTIONCALL = EXPR;
  return return_EXFUNCTIONCALL;
}

struct record26 *EXSPECIALFUNCTION(TPSSPECIALFUNCTION SPECIALFN) {
  struct record26 *return_EXSPECIALFUNCTION;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCSPECIALFUNCTIONREF);
  EXPR->SPECIALFUNCTIONEX.SPECIALFUNCTION = SPECIALFN;
  return_EXSPECIALFUNCTION = EXPR;
  return return_EXSPECIALFUNCTION;
}

struct record26 *EXSPECIALFUNCTIONCALL(struct record26 *EXPR) {
  struct record26 *return_EXSPECIALFUNCTIONCALL;
  TPSSPECIALFUNCTION FN;
  if (EXPR->CLS != XCSPECIALFUNCTIONREF) COMPILEERROR(str_make(27, "Expected a special function"));
  FN = EXPR->SPECIALFUNCTIONEX.SPECIALFUNCTION;
  EXPR->CLS = XCSPECIALFUNCTIONCALL;
  EXPR->SPECIALFUNCTIONCALLEX.SPECIALFUNCTION = FN;
  EXPR->SPECIALFUNCTIONCALLEX.SRC = (void*)0;
  EXPR->SPECIALFUNCTIONCALLEX.DST = (void*)0;
  EXPR->SPECIALFUNCTIONCALLEX.PTR = (void*)0;
  EXPR->SPECIALFUNCTIONCALLEX.READARGS = (void*)0;
  EXPR->SPECIALFUNCTIONCALLEX.WRITEARGS = (void*)0;
  return_EXSPECIALFUNCTIONCALL = EXPR;
  return return_EXSPECIALFUNCTIONCALL;
}

struct record26 *_EXUNOPIMM(struct record26 *PARENT, TLXTOKENID OP);
struct record26 *_EXUNOPCMP(struct record26 *PARENT, TLXTOKENID OP);

struct record26 *EXUNARYOP(struct record26 *PARENT, TLXTOKENID OP) {
  struct record26 *return_EXUNARYOP;
  if (OP == TKMINUS || OP == TKPLUS) {
    if (!ISINTEGERTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(17, "Invalid type for "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(PARENT->TYPEINDEX)));
  }
  else if (OP == TKNOT) {
    if (!ISBOOLEANTYPE(PARENT->TYPEINDEX) && !ISINTEGERTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(17, "Invalid type for "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(PARENT->TYPEINDEX)));
  }
  else COMPILEERROR(cat_ss(str_make(24, "Invalid unary operator: "), LXTOKENNAME(OP)));
  if (PARENT->CLS == XCIMMEDIATE) return_EXUNARYOP = _EXUNOPIMM(PARENT, OP);
  else return_EXUNARYOP = _EXUNOPCMP(PARENT, OP);
  return return_EXUNARYOP;
}

struct record26 *_EXUNOPIMM(struct record26 *PARENT, TLXTOKENID OP) {
  struct record26 *return__EXUNOPIMM;
  if (OP == TKMINUS && PARENT->IMMEDIATEEX.CLS == XICINTEGER) PARENT->IMMEDIATEEX.INTEGERVALUE = -PARENT->IMMEDIATEEX.INTEGERVALUE;
  else if (OP == TKPLUS && PARENT->IMMEDIATEEX.CLS == XICINTEGER) ;
  else if (OP == TKNOT && PARENT->IMMEDIATEEX.CLS == XICBOOLEAN) PARENT->IMMEDIATEEX.BOOLEANVALUE = !PARENT->IMMEDIATEEX.BOOLEANVALUE;
  else if (OP == TKNOT && PARENT->IMMEDIATEEX.CLS == XICINTEGER) PARENT->IMMEDIATEEX.INTEGERVALUE = ~PARENT->IMMEDIATEEX.INTEGERVALUE;
  else COMPILEERROR(str_make(49, "Internal error: invalid immediate unary operation"));
  return__EXUNOPIMM = PARENT;
  return return__EXUNOPIMM;
}

struct record26 *_EXUNOPCMP(struct record26 *PARENT, TLXTOKENID OP) {
  struct record26 *return__EXUNOPCMP;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCUNARYOP);
  EXPR->UNARYEX.PARENT = PARENT;
  EXPR->UNARYEX.OP = OP;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX;
  EXPR->ISCONSTANT = 1;
  EXPR->ISASSIGNABLE = 0;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return__EXUNOPCMP = EXPR;
  return return__EXUNOPCMP;
}

struct record26 *_EXBINOPBOOLIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPINTIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPSTRIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPENUMIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPBOOLCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPINTCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPSTRCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPENUMCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);
struct record26 *_EXBINOPPTRCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP);

struct record26 *EXBINARYOP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return_EXBINARYOP;
  PBoolean IMMEDIATE;
  IMMEDIATE = LEFT->CLS == XCIMMEDIATE && RIGHT->CLS == XCIMMEDIATE;
  if (ISBOOLEANTYPE(LEFT->TYPEINDEX) && ISBOOLEANTYPE(RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) return_EXBINARYOP = _EXBINOPBOOLIMM(LEFT, RIGHT, OP);
    else return_EXBINARYOP = _EXBINOPBOOLCMP(LEFT, RIGHT, OP);
  }
  else if (ISINTEGERTYPE(LEFT->TYPEINDEX) && ISINTEGERTYPE(RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) return_EXBINARYOP = _EXBINOPINTIMM(LEFT, RIGHT, OP);
    else return_EXBINARYOP = _EXBINOPINTCMP(LEFT, RIGHT, OP);
  }
  else if (ISSTRINGYTYPE(LEFT->TYPEINDEX) && ISSTRINGYTYPE(RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) return_EXBINARYOP = _EXBINOPSTRIMM(LEFT, RIGHT, OP);
    else return_EXBINARYOP = _EXBINOPSTRCMP(LEFT, RIGHT, OP);
  }
  else if (ISENUMTYPE(LEFT->TYPEINDEX) && ISSAMETYPE(LEFT->TYPEINDEX, RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) return_EXBINARYOP = _EXBINOPENUMIMM(LEFT, RIGHT, OP);
    else return_EXBINARYOP = _EXBINOPENUMCMP(LEFT, RIGHT, OP);
  }
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEINDEX, RIGHT->TYPEINDEX)) return_EXBINARYOP = _EXBINOPPTRCMP(LEFT, RIGHT, OP);
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(27, "Type mismatch for operator "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(LEFT->TYPEINDEX)), str_make(5, " and ")), TYPENAME(RIGHT->TYPEINDEX)));
  return return_EXBINARYOP;
}

struct record26 *_EXBINOPBOOLIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPBOOLIMM;
  PBoolean LT;
  PBoolean RT;
  LT = LEFT->IMMEDIATEEX.BOOLEANVALUE;
  RT = RIGHT->IMMEDIATEEX.BOOLEANVALUE;
  DISPOSEEXPR(&RIGHT);
  switch (OP) {
    case TKAND:
      LT = LT && RT;
      break;
    case TKOR:
      LT = LT || RT;
      break;
    case TKEQUALS:
      LT = LT == RT;
      break;
    case TKNOTEQUALS:
      LT = LT != RT;
      break;
    case TKLESSTHAN:
      LT = LT < RT;
      break;
    case TKMORETHAN:
      LT = LT > RT;
      break;
    case TKLESSOREQUALS:
      LT = LT <= RT;
      break;
    case TKMOREOREQUALS:
      LT = LT >= RT;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(26, "Invalid boolean operator: "), LXTOKENNAME(OP)));
      break;
  }
  LEFT->IMMEDIATEEX.BOOLEANVALUE = LT;
  LEFT->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  return__EXBINOPBOOLIMM = LEFT;
  return return__EXBINOPBOOLIMM;
}

struct record26 *_EXBINOPINTIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPINTIMM;
  int LT;
  int RT;
  PBoolean BO;
  LT = LEFT->IMMEDIATEEX.INTEGERVALUE;
  RT = RIGHT->IMMEDIATEEX.INTEGERVALUE;
  DISPOSEEXPR(&RIGHT);
  switch (OP) {
    case TKPLUS:
      LT = LT + RT;
      break;
    case TKMINUS:
      LT = LT - RT;
      break;
    case TKASTERISK:
      LT = LT * RT;
      break;
    case TKDIV:
      LT = LT / RT;
      break;
    case TKMOD:
      LT = LT % RT;
      break;
    case TKAND:
      LT = LT & RT;
      break;
    case TKOR:
      LT = LT | RT;
      break;
    default:
      {
        LEFT->IMMEDIATEEX.CLS = XICBOOLEAN;
        switch (OP) {
          case TKEQUALS:
            BO = LT == RT;
            break;
          case TKNOTEQUALS:
            BO = LT != RT;
            break;
          case TKLESSTHAN:
            BO = LT < RT;
            break;
          case TKMORETHAN:
            BO = LT > RT;
            break;
          case TKLESSOREQUALS:
            BO = LT <= RT;
            break;
          case TKMOREOREQUALS:
            BO = LT >= RT;
            break;
          default:
            COMPILEERROR(cat_ss(str_make(26, "Invalid integer operator: "), LXTOKENNAME(OP)));
            break;
        }
      }
      break;
  }
  if (LEFT->IMMEDIATEEX.CLS == XICINTEGER) {
    LEFT->IMMEDIATEEX.INTEGERVALUE = LT;
    LEFT->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  }
  else {
    LEFT->IMMEDIATEEX.BOOLEANVALUE = BO;
    LEFT->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  }
  return__EXBINOPINTIMM = LEFT;
  return return__EXBINOPINTIMM;
}

struct record26 *_EXBINOPSTRIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPSTRIMM;
  PString LT;
  PString RT;
  PBoolean BO;
  if (LEFT->IMMEDIATEEX.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATEEX.CHARVALUE);
  else LT = LEFT->IMMEDIATEEX.STRINGVALUE;
  if (RIGHT->IMMEDIATEEX.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATEEX.CHARVALUE);
  else RT = RIGHT->IMMEDIATEEX.STRINGVALUE;
  DISPOSEEXPR(&RIGHT);
  if (OP == TKPLUS) {
    LEFT->IMMEDIATEEX.CLS = XICSTRING;
    LT = cat_ss(LT, RT);
  }
  else {
    LEFT->IMMEDIATEEX.CLS = XICBOOLEAN;
    switch (OP) {
      case TKEQUALS:
        BO = cmp_ss(LT, RT) == 0;
        break;
      case TKNOTEQUALS:
        BO = cmp_ss(LT, RT) != 0;
        break;
      case TKLESSTHAN:
        BO = cmp_ss(LT, RT) < 0;
        break;
      case TKMORETHAN:
        BO = cmp_ss(LT, RT) > 0;
        break;
      case TKLESSOREQUALS:
        BO = cmp_ss(LT, RT) <= 0;
        break;
      case TKMOREOREQUALS:
        BO = cmp_ss(LT, RT) >= 0;
        break;
      default:
        COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
        break;
    }
  }
  if (LEFT->IMMEDIATEEX.CLS == XICSTRING) {
    LEFT->IMMEDIATEEX.STRINGVALUE = LT;
    LEFT->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
  }
  else {
    LEFT->IMMEDIATEEX.BOOLEANVALUE = BO;
    LEFT->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  }
  return__EXBINOPSTRIMM = LEFT;
  return return__EXBINOPSTRIMM;
}

struct record26 *_EXBINOPENUMIMM(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPENUMIMM;
  int LT;
  int RT;
  PBoolean BO;
  LT = LEFT->IMMEDIATEEX.ENUMORDINAL;
  RT = RIGHT->IMMEDIATEEX.ENUMORDINAL;
  DISPOSEEXPR(&RIGHT);
  switch (OP) {
    case TKEQUALS:
      BO = LT == RT;
      break;
    case TKNOTEQUALS:
      BO = LT != RT;
      break;
    case TKLESSTHAN:
      BO = LT < RT;
      break;
    case TKMORETHAN:
      BO = LT > RT;
      break;
    case TKLESSOREQUALS:
      BO = LT <= RT;
      break;
    case TKMOREOREQUALS:
      BO = LT >= RT;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(26, "Invalid integer operator: "), LXTOKENNAME(OP)));
      break;
  }
  LEFT->IMMEDIATEEX.CLS = XICBOOLEAN;
  LEFT->IMMEDIATEEX.BOOLEANVALUE = BO;
  LEFT->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  return__EXBINOPENUMIMM = LEFT;
  return return__EXBINOPENUMIMM;
}

struct record26 *_EXBINOPBOOLCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPBOOLCMP;
  struct record26 *EXPR;
  if (OP == TKAND || OP == TKOR || OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) {
    EXPR = _NEWEXPR(XCBINARYOP);
    EXPR->BINARYEX.LEFT = LEFT;
    EXPR->BINARYEX.RIGHT = RIGHT;
    EXPR->BINARYEX.OP = OP;
    EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
    EXPR->ISCONSTANT = 1;
    EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
    return__EXBINOPBOOLCMP = EXPR;
  }
  else COMPILEERROR(cat_ss(str_make(26, "Invalid boolean operator: "), LXTOKENNAME(OP)));
  return return__EXBINOPBOOLCMP;
}

struct record26 *_EXBINOPINTCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPINTCMP;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS || OP == TKMINUS || OP == TKASTERISK || OP == TKDIV || OP == TKMOD || OP == TKAND || OP == TKOR) EXPR->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(26, "Invalid integer operator: "), LXTOKENNAME(OP)));
  return__EXBINOPINTCMP = EXPR;
  return return__EXBINOPINTCMP;
}

struct record26 *_EXBINOPSTRCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPSTRCMP;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
  return__EXBINOPSTRCMP = EXPR;
  return return__EXBINOPSTRCMP;
}

struct record26 *_EXBINOPENUMCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPENUMCMP;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(23, "Invalid enum operator: "), LXTOKENNAME(OP)));
  return__EXBINOPENUMCMP = EXPR;
  return return__EXBINOPENUMCMP;
}

struct record26 *_EXBINOPPTRCMP(struct record26 *LEFT, struct record26 *RIGHT, TLXTOKENID OP) {
  struct record26 *return__EXBINOPPTRCMP;
  struct record26 *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKEQUALS || OP == TKNOTEQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
  return__EXBINOPPTRCMP = EXPR;
  return return__EXBINOPPTRCMP;
}

struct record26 *EXCOERCE(struct record26 *EXPR, struct record29 *TYPEINDEX) {
  struct record26 *return_EXCOERCE;
  if (ISCHARTYPE(EXPR->TYPEINDEX) && ISSTRINGTYPE(TYPEINDEX)) return_EXCOERCE = EXTOSTRING(EXPR);
  else if (ISSAMETYPE(EXPR->TYPEINDEX, TYPEINDEX)) return_EXCOERCE = EXPR;
  else if (ISNILTYPE(EXPR->TYPEINDEX) && ISPOINTERYTYPE(TYPEINDEX)) return_EXCOERCE = EXPR;
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(24, "Type mismatch: expected "), TYPENAME(TYPEINDEX)), str_make(6, ", got ")), TYPENAME(EXPR->TYPEINDEX)));
  return return_EXCOERCE;
}

void READTOKEN();

void WANTTOKEN(TLXTOKENID ID) {
  if (LEXER.TOKEN.ID != ID) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID)), str_make(8, ", found ")), LXTOKENSTR()));
}

void WANTTOKEN2(TLXTOKENID ID1, TLXTOKENID ID2) {
  if (LEXER.TOKEN.ID != ID1 && LEXER.TOKEN.ID != ID2) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID1)), str_make(4, " or ")), LXTOKENNAME(ID2)), str_make(8, ", found ")), LXTOKENSTR()));
}

void WANTTOKEN3(TLXTOKENID ID1, TLXTOKENID ID2, TLXTOKENID ID3) {
  if (LEXER.TOKEN.ID != ID1 && LEXER.TOKEN.ID != ID2 && LEXER.TOKEN.ID != ID3) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID1)), str_make(2, ", ")), LXTOKENNAME(ID2)), str_make(5, ", or ")), LXTOKENNAME(ID3)), str_make(7, " found ")), LXTOKENSTR()));
}

void WANTTOKENANDREAD(TLXTOKENID ID) {
  WANTTOKEN(ID);
  READTOKEN();
}

PString GETTOKENVALUEANDREAD(TLXTOKENID ID) {
  PString return_GETTOKENVALUEANDREAD;
  WANTTOKEN(ID);
  return_GETTOKENVALUEANDREAD = LEXER.TOKEN.VALUE;
  READTOKEN();
  return return_GETTOKENVALUEANDREAD;
}

void SKIPTOKEN(TLXTOKENID ID) {
  if (LEXER.TOKEN.ID == ID) READTOKEN();
}

struct record29 *PSTYPEDENOTER();

struct record29 *PSTYPEIDENTIFIER() {
  struct record29 *return_PSTYPEIDENTIFIER;
  WANTTOKEN(TKIDENTIFIER);
  return_PSTYPEIDENTIFIER = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 1)->TYPEINDEX;
  READTOKEN();
  return return_PSTYPEIDENTIFIER;
}

TPSIDENTIFIER PSIDENTIFIER() {
  TPSIDENTIFIER return_PSIDENTIFIER;
  TPSIDENTIFIER IDENT;
  IDENT.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  return_PSIDENTIFIER = IDENT;
  return return_PSIDENTIFIER;
}

typedef enum enum46 { LHS, RHS } EXPRESSIONTYPE;

const char* enumvalues46[] = { "LHS", "RHS" };

struct record26 *PSEXPRESSION();

struct record29 *PSENUMERATEDTYPE() {
  struct record29 *return_PSENUMERATEDTYPE;
  TPSTYPE TYP;
  TPSENUMDEF ENUM;
  WANTTOKENANDREAD(TKLPAREN);
  ENUM.SIZE = 0;
  ENUM.HASBEENDEFINED = 0;
  do {
    ENUM.SIZE = ENUM.SIZE + 1;
    if (ENUM.SIZE > 128) COMPILEERROR(str_make(23, "Too many values in enum"));
    ENUM.VALUES[ENUM.SIZE - 1] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRPAREN));
  TYP = EMPTYTYPE();
  TYP.CLS = TTCENUM;
  TYP.ENUMINDEX = ADDENUM(ENUM);
  return_PSENUMERATEDTYPE = ADDTYPE(TYP);
  SKIPTOKEN(TKRPAREN);
  return return_PSENUMERATEDTYPE;
}

void PSRECORDFIELD(TPSRECORDDEF *REC, TLXTOKENID DELIMITER) {
  PString NAME;
  int LASTFIELD;
  int FIELD;
  struct record29 *TYPEINDEX;
  LASTFIELD = (*REC).SIZE;
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    do {
      int first = 1;
      int last = (*REC).SIZE;
      if (first <= last) {
        FIELD = first;
        while (1) {
          {
            if (cmp_ss((*REC).FIELDS[FIELD - 1].NAME, NAME) == 0) COMPILEERROR(cat_ss(cat_ss(str_make(14, "A field named "), NAME), str_make(25, " has already been defined")));
          }
          if (FIELD == last) break;
          ++FIELD;
        }
      }
    } while(0);
    (*REC).SIZE = (*REC).SIZE + 1;
    if ((*REC).SIZE > 32) COMPILEERROR(str_make(25, "Too many fields in record"));
    (*REC).FIELDS[(*REC).SIZE - 1].NAME = NAME;
    WANTTOKEN2(TKCOMMA, TKCOLON);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKCOLON));
  WANTTOKENANDREAD(TKCOLON);
  TYPEINDEX = PSTYPEDENOTER();
  do {
    int first = LASTFIELD + 1;
    int last = (*REC).SIZE;
    if (first <= last) {
      FIELD = first;
      while (1) {
        (*REC).FIELDS[FIELD - 1].TYPEINDEX = TYPEINDEX;
        if (FIELD == last) break;
        ++FIELD;
      }
    }
  } while(0);
  WANTTOKEN2(TKSEMICOLON, DELIMITER);
  SKIPTOKEN(TKSEMICOLON);
}

void PSRECORDVARIANTS(TPSRECORDDEF *REC) {
  TPSIDENTIFIER TAG;
  struct record29 *TAGTYPE;
  struct record26 *CASELABEL;
  WANTTOKENANDREAD(TKCASE);
  TAG = PSIDENTIFIER();
  WANTTOKEN2(TKCOLON, TKOF);
  if (LEXER.TOKEN.ID == TKCOLON) {
    READTOKEN();
    TAGTYPE = PSTYPEIDENTIFIER();
    (*REC).SIZE = (*REC).SIZE + 1;
    (*REC).FIELDS[(*REC).SIZE - 1].NAME = TAG.NAME;
    (*REC).FIELDS[(*REC).SIZE - 1].TYPEINDEX = TAGTYPE;
  }
  else TAGTYPE = FINDNAMEOFCLASS(TAG.NAME, TNCTYPE, 1)->TYPEINDEX;
  if (!ISORDINALTYPE(TAGTYPE)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
  WANTTOKENANDREAD(TKOF);
  do {
    (*REC).NUMVARIANTS = (*REC).NUMVARIANTS + 1;
    (*REC).VARIANTBOUNDS[(*REC).NUMVARIANTS - 1] = (*REC).SIZE + 1;
    do {
      CASELABEL = EXCOERCE(PSEXPRESSION(), TAGTYPE);
      if (!CASELABEL->ISCONSTANT) COMPILEERROR(str_make(47, "The label of the case statement is not constant"));
      DISPOSEEXPR(&CASELABEL);
      WANTTOKEN2(TKCOMMA, TKCOLON);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    WANTTOKENANDREAD(TKCOLON);
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) PSRECORDFIELD(&*REC, TKRPAREN);
    WANTTOKENANDREAD(TKRPAREN);
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID == TKEND));
}

struct record29 *PSRECORDTYPE() {
  struct record29 *return_PSRECORDTYPE;
  TPSTYPE TYP;
  TPSRECORDDEF REC;
  WANTTOKENANDREAD(TKRECORD);
  REC.SIZE = 0;
  REC.NUMVARIANTS = 0;
  REC.HASBEENDEFINED = 0;
  while (LEXER.TOKEN.ID != TKCASE && LEXER.TOKEN.ID != TKEND) PSRECORDFIELD(&REC, TKEND);
  if (LEXER.TOKEN.ID == TKCASE) PSRECORDVARIANTS(&REC);
  WANTTOKENANDREAD(TKEND);
  TYP = TYPEOFCLASS(TTCRECORD);
  TYP.RECORDINDEX = ADDRECORD(REC);
  return_PSRECORDTYPE = ADDTYPE(TYP);
  return return_PSRECORDTYPE;
}

struct record26 *PSIMMEDIATE() {
  struct record26 *return_PSIMMEDIATE;
  struct record26 *EXPR;
  EXPR = PSEXPRESSION();
  if (EXPR->CLS != XCIMMEDIATE) COMPILEERROR(str_make(27, "Expected an immediate value"));
  return_PSIMMEDIATE = EXPR;
  return return_PSIMMEDIATE;
}

struct record29 *PSARRAYTYPE() {
  struct record29 *return_PSARRAYTYPE;
  TPSTYPE TYP;
  TPSARRAYDEF ARR;
  WANTTOKENANDREAD(TKARRAY);
  WANTTOKENANDREAD(TKLBRACKET);
  ARR.LOWBOUND = PSIMMEDIATE();
  WANTTOKENANDREAD(TKRANGE);
  ARR.HIGHBOUND = PSIMMEDIATE();
  WANTTOKENANDREAD(TKRBRACKET);
  WANTTOKENANDREAD(TKOF);
  ARR.TYPEINDEX = PSTYPEDENOTER();
  TYP = TYPEOFCLASS(TTCARRAY);
  TYP.ARRAYINDEX = ADDARRAY(ARR);
  return_PSARRAYTYPE = ADDTYPE(TYP);
  return return_PSARRAYTYPE;
}

struct record29 *PSPOINTERTYPE() {
  struct record29 *return_PSPOINTERTYPE;
  TPSTYPE TYP;
  struct record39 *NAMEINDEX;
  WANTTOKENANDREAD(TKCARET);
  WANTTOKEN(TKIDENTIFIER);
  NAMEINDEX = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 0);
  if (NAMEINDEX == (void*)0) TYP = POINTERUNKNOWNTYPE(LEXER.TOKEN.VALUE);
  else TYP = POINTERTYPE(NAMEINDEX->TYPEINDEX);
  READTOKEN();
  return_PSPOINTERTYPE = ADDTYPE(TYP);
  return return_PSPOINTERTYPE;
}

struct record29 *PSTYPEDENOTER() {
  struct record29 *return_PSTYPEDENOTER;
  struct record29 *TYPEINDEX;
  TYPEINDEX = (void*)0;
  if (LEXER.TOKEN.ID == TKIDENTIFIER) TYPEINDEX = PSTYPEIDENTIFIER();
  else if (LEXER.TOKEN.ID == TKLPAREN) TYPEINDEX = PSENUMERATEDTYPE();
  else if (LEXER.TOKEN.ID == TKRECORD) TYPEINDEX = PSRECORDTYPE();
  else if (LEXER.TOKEN.ID == TKARRAY) TYPEINDEX = PSARRAYTYPE();
  else if (LEXER.TOKEN.ID == TKCARET) TYPEINDEX = PSPOINTERTYPE();
  else COMPILEERROR(cat_ss(str_make(30, "Wanted type definition, found "), LXTOKENSTR()));
  return_PSTYPEDENOTER = TYPEINDEX;
  return return_PSTYPEDENOTER;
}

void _RESOLVEPOINTERUNKNOWN(struct record29 *TYPEINDEX) {
  struct record29 *TARGETINDEX;
  if (TYPEINDEX->CLS == TTCPOINTERUNKNOWN) {
    TARGETINDEX = FINDNAMEOFCLASS(*TYPEINDEX->TARGETNAME, TNCTYPE, 1)->TYPEINDEX;
    free(TYPEINDEX->TARGETNAME);
    TYPEINDEX->CLS = TTCPOINTER;
    TYPEINDEX->POINTEDTYPEINDEX = TARGETINDEX;
  }
}

void PSTYPEDEFINITIONS() {
  PString NAME;
  struct record29 *TYPEINDEX;
  TPSTYPE NEWTYPE;
  struct record41 *CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKTYPE);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKENANDREAD(TKEQUALS);
    TYPEINDEX = PSTYPEDENOTER();
    NEWTYPE = COPYTYPE(TYPEINDEX);
    NEWTYPE.NAME = NAME;
    NEWTYPE.ALIASFOR = TYPEINDEX;
    TYPEINDEX = ADDTYPE(NEWTYPE);
    WANTTOKENANDREAD(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID != TKIDENTIFIER));
  OUTTYPEDEFINITIONSFROMCHECKPOINT(CHECKPOINT);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSCONSTANT(PString NAME) {
  TPSCONSTANT CONSTANT;
  WANTTOKENANDREAD(TKEQUALS);
  CONSTANT.NAME = NAME;
  if (LEXER.TOKEN.ID == TKFALSE || LEXER.TOKEN.ID == TKTRUE || LEXER.TOKEN.ID == TKNUMBER || LEXER.TOKEN.ID == TKSTRING) CONSTANT.REPLACEMENT = LEXER.TOKEN;
  else COMPILEERROR(cat_ss(str_make(31, "Expected constant value, found "), LXTOKENSTR()));
  ADDCONSTANT(CONSTANT);
  READTOKEN();
}

void PSCONSTANTVALUE(struct record29 *TYPEINDEX) {
  struct record26 *EXPR;
  if (ISARRAYTYPE(TYPEINDEX)) {
    WANTTOKENANDREAD(TKLPAREN);
    TYPEINDEX = TYPEINDEX->ARRAYINDEX->TYPEINDEX;
    OUTCONSTANTARRAYBEGIN();
    while (LEXER.TOKEN.ID != TKRPAREN) {
      PSCONSTANTVALUE(TYPEINDEX);
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      if (LEXER.TOKEN.ID == TKCOMMA) OUTCONSTANTARRAYSEPARATOR();
      SKIPTOKEN(TKCOMMA);
    }
    OUTCONSTANTARRAYEND();
    WANTTOKENANDREAD(TKRPAREN);
  }
  else {
    EXPR = EXCOERCE(PSIMMEDIATE(), TYPEINDEX);
    OUTEXPRESSION(EXPR);
    DISPOSEEXPR(&EXPR);
  }
}

void PSTYPEDCONSTANT(PString NAME) {
  struct record29 *TYPEINDEX;
  WANTTOKENANDREAD(TKCOLON);
  TYPEINDEX = PSTYPEDENOTER();
  WANTTOKENANDREAD(TKEQUALS);
  OUTCONSTANTDEFINITIONBEGIN(ADDVARIABLE(MAKETYPEDCONSTANT(NAME, TYPEINDEX)));
  PSCONSTANTVALUE(TYPEINDEX);
  OUTCONSTANTDEFINITIONEND();
}

void PSCONSTDEFINITIONS() {
  PString NAME;
  WANTTOKENANDREAD(TKCONST);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKEQUALS, TKCOLON);
    if (LEXER.TOKEN.ID == TKEQUALS) PSCONSTANT(NAME);
    else PSTYPEDCONSTANT(NAME);
    WANTTOKENANDREAD(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID != TKIDENTIFIER));
}

void PSVARDEFINITIONS() {
  int NUMNAMES;
  PString NAMES[8];
  struct record29 *TYPEINDEX;
  struct record41 *CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKVAR);
  do {
    NUMNAMES = 0;
    do {
      NUMNAMES = NUMNAMES + 1;
      if (NUMNAMES > 8) COMPILEERROR(str_make(37, "Too many names in variable definition"));
      NAMES[NUMNAMES - 1] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      WANTTOKEN2(TKCOMMA, TKCOLON);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    WANTTOKENANDREAD(TKCOLON);
    TYPEINDEX = PSTYPEDENOTER();
    WANTTOKENANDREAD(TKSEMICOLON);
    do {
      int first = 1;
      int last = NUMNAMES;
      if (first <= last) {
        NUMNAMES = first;
        while (1) {
          OUTVARIABLEDEFINITION(ADDVARIABLE(MAKEVARIABLE(NAMES[NUMNAMES - 1], TYPEINDEX, 0)));
          if (NUMNAMES == last) break;
          ++NUMNAMES;
        }
      }
    } while(0);
  } while (!(LEXER.TOKEN.ID != TKIDENTIFIER));
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSSTATEMENT();
void PSDEFINITIONS();

void PSFUNCTIONBODY(struct record36 *FNINDEX) {
  int POS;
  struct record41 *CHECKPOINT;
  STARTLOCALSCOPE(FNINDEX);
  CHECKPOINT = DEFS.LATEST;
  do {
    int first = 1;
    int last = FNINDEX->ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        ADDVARIABLE(FNINDEX->ARGS[POS - 1]);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  OUTFUNCTIONDEFINITION(FNINDEX);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
  PSDEFINITIONS();
  WANTTOKENANDREAD(TKBEGIN);
  while (LEXER.TOKEN.ID != TKEND) {
    PSSTATEMENT();
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  }
  WANTTOKENANDREAD(TKEND);
  WANTTOKENANDREAD(TKSEMICOLON);
  OUTFUNCTIONEND(FNINDEX);
  CLOSELOCALSCOPE();
}

void PSARGUMENTS(TPSFUNCTION *DEF) {
  PBoolean ISREFERENCE;
  int LASTARG;
  int ARG;
  struct record29 *TYPEINDEX;
  WANTTOKENANDREAD(TKLPAREN);
  (*DEF).ARGCOUNT = 0;
  do {
    ISREFERENCE = LEXER.TOKEN.ID == TKVAR;
    SKIPTOKEN(TKVAR);
    LASTARG = (*DEF).ARGCOUNT;
    do {
      (*DEF).ARGCOUNT = (*DEF).ARGCOUNT + 1;
      if ((*DEF).ARGCOUNT > 4) COMPILEERROR(cat_ss(str_make(41, "Too many arguments declared for function "), (*DEF).NAME));
      (*DEF).ARGS[(*DEF).ARGCOUNT - 1].NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      (*DEF).ARGS[(*DEF).ARGCOUNT - 1].ISREFERENCE = ISREFERENCE;
      (*DEF).ARGS[(*DEF).ARGCOUNT - 1].ISCONSTANT = 0;
      WANTTOKEN2(TKCOLON, TKCOMMA);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    SKIPTOKEN(TKCOLON);
    TYPEINDEX = PSTYPEIDENTIFIER();
    do {
      int first = LASTARG + 1;
      int last = (*DEF).ARGCOUNT;
      if (first <= last) {
        ARG = first;
        while (1) {
          (*DEF).ARGS[ARG - 1].TYPEINDEX = TYPEINDEX;
          if (ARG == last) break;
          ++ARG;
        }
      }
    } while(0);
    WANTTOKEN2(TKSEMICOLON, TKRPAREN);
    SKIPTOKEN(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID == TKRPAREN));
  SKIPTOKEN(TKRPAREN);
}

void PSPROCEDUREDEFINITION() {
  TPSFUNCTION DEF;
  DEF = EMPTYFUNCTION();
  WANTTOKENANDREAD(TKPROCEDURE);
  DEF.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  WANTTOKEN2(TKLPAREN, TKSEMICOLON);
  if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&DEF);
  WANTTOKENANDREAD(TKSEMICOLON);
  if (LEXER.TOKEN.ID == TKFORWARD) {
    SKIPTOKEN(TKFORWARD);
    WANTTOKENANDREAD(TKSEMICOLON);
    DEF.ISDECLARATION = 1;
    OUTFUNCTIONDECLARATION(ADDFUNCTION(DEF));
  }
  else PSFUNCTIONBODY(ADDFUNCTION(DEF));
}

struct record29 *PSRESULTTYPE() {
  struct record29 *return_PSRESULTTYPE;
  return_PSRESULTTYPE = PSTYPEIDENTIFIER();
  return return_PSRESULTTYPE;
}

void PSFUNCTIONDEFINITION() {
  TPSFUNCTION DEF;
  DEF = EMPTYFUNCTION();
  WANTTOKENANDREAD(TKFUNCTION);
  DEF.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  if (LEXER.TOKEN.ID == TKSEMICOLON && HASFORWARDDECLARATION(DEF.NAME)) DEF.RETURNTYPEINDEX = (void*)0;
  else {
    WANTTOKEN2(TKLPAREN, TKCOLON);
    if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&DEF);
    WANTTOKENANDREAD(TKCOLON);
    DEF.RETURNTYPEINDEX = PSRESULTTYPE();
  }
  WANTTOKENANDREAD(TKSEMICOLON);
  if (LEXER.TOKEN.ID == TKFORWARD) {
    SKIPTOKEN(TKFORWARD);
    WANTTOKENANDREAD(TKSEMICOLON);
    DEF.ISDECLARATION = 1;
    OUTFUNCTIONDECLARATION(ADDFUNCTION(DEF));
  }
  else PSFUNCTIONBODY(ADDFUNCTION(DEF));
}

void PSDEFINITIONS() {
  PBoolean DONE;
  DONE = 0;
  do {
    if (LEXER.TOKEN.ID == TKTYPE) PSTYPEDEFINITIONS();
    else if (LEXER.TOKEN.ID == TKCONST) PSCONSTDEFINITIONS();
    else if (LEXER.TOKEN.ID == TKVAR) PSVARDEFINITIONS();
    else if (LEXER.TOKEN.ID == TKPROCEDURE) PSPROCEDUREDEFINITION();
    else if (LEXER.TOKEN.ID == TKFUNCTION) PSFUNCTIONDEFINITION();
    else DONE = 1;
  } while (!DONE);
}

void PSPROGRAMHEADING() {
  WANTTOKENANDREAD(TKPROGRAM);
  OUTPROGRAMHEADING(GETTOKENVALUEANDREAD(TKIDENTIFIER));
  if (LEXER.TOKEN.ID == TKLPAREN) {
    do {
      READTOKEN();
      WANTTOKEN2(TKIDENTIFIER, TKRPAREN);
      SKIPTOKEN(TKIDENTIFIER);
      WANTTOKEN2(TKCOMMA, TKRPAREN);
    } while (!(LEXER.TOKEN.ID == TKRPAREN));
    SKIPTOKEN(TKRPAREN);
  }
  WANTTOKENANDREAD(TKSEMICOLON);
}

struct record26 *PSPOINTERDEREF(struct record26 *PTR) {
  struct record26 *return_PSPOINTERDEREF;
  WANTTOKENANDREAD(TKCARET);
  return_PSPOINTERDEREF = EXPOINTERACCESS(PTR);
  return return_PSPOINTERDEREF;
}

struct record26 *PSREAD(struct record26 *FNEXPR) {
  struct record26 *return_PSREAD;
  struct record26 *EXPR;
  PBoolean FIRST;
  struct record26 *OUTVAR;
  struct record20 *READARG;
  EXPR = EXSPECIALFUNCTIONCALL(FNEXPR);
  EXPR->SPECIALFUNCTIONCALLEX.SRC = EXVARIABLE(FINDNAMEOFCLASS(str_make(5, "INPUT"), TNCVARIABLE, 1)->VARIABLEINDEX);
  READARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTVAR = PSEXPRESSION();
      if (FIRST && OUTVAR->ISASSIGNABLE && ISTEXTTYPE(OUTVAR->TYPEINDEX)) {
        DISPOSEEXPR(&EXPR->SPECIALFUNCTIONCALLEX.SRC);
        EXPR->SPECIALFUNCTIONCALLEX.SRC = OUTVAR;
      }
      else {
        if (!OUTVAR->ISASSIGNABLE || !ISSTRINGYTYPE(OUTVAR->TYPEINDEX)) COMPILEERROR(str_make(36, "Invalid expression for read argument"));
        if (READARG == (void*)0) {
          EXPR->SPECIALFUNCTIONCALLEX.READARGS = malloc(sizeof(struct record20));
          READARG = EXPR->SPECIALFUNCTIONCALLEX.READARGS;
        }
        else {
          READARG->NEXT = malloc(sizeof(struct record20));
          READARG = READARG->NEXT;
        }
        READARG->NEXT = (void*)0;
        READARG->ARG = OUTVAR;
      }
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
      FIRST = 0;
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  return_PSREAD = EXPR;
  return return_PSREAD;
}

struct record26 *PSWRITE(struct record26 *FNEXPR) {
  struct record26 *return_PSWRITE;
  struct record26 *EXPR;
  PBoolean FIRST;
  struct record26 *OUTEXPR;
  struct record21 *WRITEARG;
  EXPR = EXSPECIALFUNCTIONCALL(FNEXPR);
  EXPR->SPECIALFUNCTIONCALLEX.DST = EXVARIABLE(FINDNAMEOFCLASS(str_make(6, "OUTPUT"), TNCVARIABLE, 1)->VARIABLEINDEX);
  WRITEARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTEXPR = PSEXPRESSION();
      if (FIRST && OUTEXPR->ISASSIGNABLE && ISTEXTTYPE(OUTEXPR->TYPEINDEX)) {
        DISPOSEEXPR(&EXPR->SPECIALFUNCTIONCALLEX.DST);
        EXPR->SPECIALFUNCTIONCALLEX.DST = OUTEXPR;
      }
      else {
        if (WRITEARG == (void*)0) {
          EXPR->SPECIALFUNCTIONCALLEX.WRITEARGS = malloc(sizeof(struct record21));
          WRITEARG = EXPR->SPECIALFUNCTIONCALLEX.WRITEARGS;
        }
        else {
          WRITEARG->NEXT = malloc(sizeof(struct record21));
          WRITEARG = WRITEARG->NEXT;
        }
        WRITEARG->NEXT = (void*)0;
        WRITEARG->ARG = OUTEXPR;
      }
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
      FIRST = 0;
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  return_PSWRITE = EXPR;
  return return_PSWRITE;
}

struct record26 *PSSTR(struct record26 *FNEXPR) {
  struct record26 *return_PSSTR;
  struct record26 *EXPR;
  struct record26 *SRC;
  struct record26 *DEST;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = PSEXPRESSION();
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  if (!DEST->ISASSIGNABLE || !ISSTRINGTYPE(DEST->TYPEINDEX)) COMPILEERROR(str_make(45, "Destination argument is not a string variable"));
  WANTTOKENANDREAD(TKRPAREN);
  EXPR = EXSPECIALFUNCTIONCALL(FNEXPR);
  EXPR->SPECIALFUNCTIONCALLEX.SRC = SRC;
  EXPR->SPECIALFUNCTIONCALLEX.DST = DEST;
  return_PSSTR = EXPR;
  return return_PSSTR;
}

struct record26 *PSNEW(struct record26 *FNEXPR) {
  struct record26 *return_PSNEW;
  struct record26 *EXPR;
  struct record26 *PTR;
  WANTTOKENANDREAD(TKLPAREN);
  PTR = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  if (!PTR->ISASSIGNABLE || !ISPOINTERTYPE(PTR->TYPEINDEX)) COMPILEERROR(str_make(25, "Argument is not a pointer"));
  EXPR = EXSPECIALFUNCTIONCALL(FNEXPR);
  EXPR->SPECIALFUNCTIONCALLEX.PTR = PTR;
  return_PSNEW = EXPR;
  return return_PSNEW;
}

struct record26 *PSDISPOSE(struct record26 *FNEXPR) {
  struct record26 *return_PSDISPOSE;
  struct record26 *EXPR;
  struct record26 *PTR;
  WANTTOKENANDREAD(TKLPAREN);
  PTR = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  if (!PTR->ISASSIGNABLE || !ISPOINTERTYPE(PTR->TYPEINDEX)) COMPILEERROR(str_make(25, "Argument is not a pointer"));
  EXPR = EXSPECIALFUNCTIONCALL(FNEXPR);
  EXPR->SPECIALFUNCTIONCALLEX.PTR = PTR;
  return_PSDISPOSE = EXPR;
  return return_PSDISPOSE;
}

struct record26 *PSFUNCTIONCALL(struct record26 *FN) {
  struct record26 *return_PSFUNCTIONCALL;
  TEXFUNCTIONARGS ARGS;
  if (FN->CLS == XCFUNCTIONREF) {
    ARGS.SIZE = 0;
    if (LEXER.TOKEN.ID == TKLPAREN) {
      WANTTOKENANDREAD(TKLPAREN);
      while (LEXER.TOKEN.ID != TKRPAREN) {
        ARGS.SIZE = ARGS.SIZE + 1;
        ARGS.VALUES[ARGS.SIZE - 1] = PSEXPRESSION();
        WANTTOKEN2(TKCOMMA, TKRPAREN);
        SKIPTOKEN(TKCOMMA);
      }
      WANTTOKENANDREAD(TKRPAREN);
    }
    return_PSFUNCTIONCALL = EXFUNCTIONCALL(FN, &ARGS);
  }
  else if (FN->CLS == XCSPECIALFUNCTIONREF) {
    switch (FN->SPECIALFUNCTIONEX.SPECIALFUNCTION) {
      case TSFREAD:
        return_PSFUNCTIONCALL = PSREAD(FN);
        break;
      case TSFREADLN:
        return_PSFUNCTIONCALL = PSREAD(FN);
        break;
      case TSFWRITE:
        return_PSFUNCTIONCALL = PSWRITE(FN);
        break;
      case TSFWRITELN:
        return_PSFUNCTIONCALL = PSWRITE(FN);
        break;
      case TSFSTR:
        return_PSFUNCTIONCALL = PSSTR(FN);
        break;
      case TSFNEW:
        return_PSFUNCTIONCALL = PSNEW(FN);
        break;
      case TSFDISPOSE:
        return_PSFUNCTIONCALL = PSDISPOSE(FN);
        break;
      default:
        break;
    }
  }
  return return_PSFUNCTIONCALL;
}

struct record26 *PSARRAYACCESS(struct record26 *ARR) {
  struct record26 *return_PSARRAYACCESS;
  struct record26 *IDX;
  WANTTOKENANDREAD(TKLBRACKET);
  IDX = PSEXPRESSION();
  WANTTOKENANDREAD(TKRBRACKET);
  if (ISSTRINGYTYPE(ARR->TYPEINDEX)) return_PSARRAYACCESS = EXSTRINGCHAR(ARR, IDX);
  else return_PSARRAYACCESS = EXARRAYACCESS(ARR, IDX);
  return return_PSARRAYACCESS;
}

struct record26 *PSFIELDACCESS(struct record26 *REC) {
  struct record26 *return_PSFIELDACCESS;
  TPSIDENTIFIER FLD;
  WANTTOKENANDREAD(TKDOT);
  FLD = PSIDENTIFIER();
  return_PSFIELDACCESS = EXFIELDACCESS(REC, FINDFIELD(REC->TYPEINDEX, FLD.NAME, 1));
  return return_PSFIELDACCESS;
}

struct record26 *PSVARIABLE() {
  struct record26 *return_PSVARIABLE;
  TPSIDENTIFIER ID;
  struct record37 *WITHVARINDEX;
  TPSNAME FOUND;
  struct record26 *EXPR;
  PBoolean DONE;
  ID = PSIDENTIFIER();
  WITHVARINDEX = FINDWITHVAR(ID.NAME);
  if (WITHVARINDEX != (void*)0) {
    EXPR = EXVARIABLE(WITHVARINDEX->VARIABLEINDEX);
    EXPR = EXFIELDACCESS(EXPR, FINDFIELD(EXPR->TYPEINDEX, ID.NAME, 1));
  }
  else {
    FOUND = *FINDNAME(ID.NAME, 1);
    if (FOUND.CLS == TNCVARIABLE) EXPR = EXVARIABLE(FOUND.VARIABLEINDEX);
    else if (FOUND.CLS == TNCFUNCTION) EXPR = EXFUNCTIONREF(FOUND.FUNCTIONINDEX);
    else if (FOUND.CLS == TNCENUMVALUE) EXPR = EXENUMCONSTANT(FOUND.ORDINAL, FOUND.ENUMTYPEINDEX);
    else if (FOUND.CLS == TNCSPECIALFUNCTION) EXPR = EXSPECIALFUNCTION(FOUND.SPECIALFUNCTION);
    else COMPILEERROR(cat_ss(str_make(20, "Invalid identifier: "), ID.NAME));
  }
  return_PSVARIABLE = EXPR;
  return return_PSVARIABLE;
}

struct record26 *PSVARIABLEORFUNCTIONEXTENSION(struct record26 *EXPR) {
  struct record26 *return_PSVARIABLEORFUNCTIONEXTENSION;
  PBoolean DONE;
  DONE = 0;
  do {
    if (EXPR->CLS == XCSPECIALFUNCTIONREF) EXPR = PSFUNCTIONCALL(EXPR);
    else if (EXPR->CLS == XCFUNCTIONREF) EXPR = PSFUNCTIONCALL(EXPR);
    else if (LEXER.TOKEN.ID == TKDOT) EXPR = PSFIELDACCESS(EXPR);
    else if (LEXER.TOKEN.ID == TKLBRACKET) EXPR = PSARRAYACCESS(EXPR);
    else if (LEXER.TOKEN.ID == TKCARET) EXPR = PSPOINTERDEREF(EXPR);
    else DONE = 1;
  } while (!DONE);
  return_PSVARIABLEORFUNCTIONEXTENSION = EXPR;
  return return_PSVARIABLEORFUNCTIONEXTENSION;
}

PBoolean ISOPADDING(TLXTOKEN TOK) {
  PBoolean return_ISOPADDING;
  return_ISOPADDING = TOK.ID == TKPLUS || TOK.ID == TKMINUS || TOK.ID == TKOR;
  return return_ISOPADDING;
}

PBoolean ISOPMULTIPLYING(TLXTOKEN TOK) {
  PBoolean return_ISOPMULTIPLYING;
  return_ISOPMULTIPLYING = TOK.ID == TKASTERISK || TOK.ID == TKSLASH || TOK.ID == TKDIV || TOK.ID == TKMOD || TOK.ID == TKAND;
  return return_ISOPMULTIPLYING;
}

PBoolean ISOPRELATIONAL(TLXTOKEN TOK) {
  PBoolean return_ISOPRELATIONAL;
  return_ISOPRELATIONAL = TOK.ID == TKEQUALS || TOK.ID == TKNOTEQUALS || TOK.ID == TKLESSTHAN || TOK.ID == TKMORETHAN || TOK.ID == TKLESSOREQUALS || TOK.ID == TKMOREOREQUALS || TOK.ID == TKIN;
  return return_ISOPRELATIONAL;
}

PString PARSESTRING(PString PSTR) {
  PString return_PARSESTRING;
  PBoolean INSTR;
  int POS;
  char CHR;
  PString STR;
  STR = str_make(0, "");
  INSTR = 0;
  do {
    int first = 1;
    int last = LENGTH(PSTR);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          CHR = PSTR.chr[POS];
          if (CHR == '\'') {
            INSTR = !INSTR;
            if (INSTR && POS > 1 && PSTR.chr[POS - 1] == '\'') STR = cat_sc(STR, '\'');
          }
          else if (INSTR) STR = cat_sc(STR, CHR);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return_PARSESTRING = STR;
  return return_PARSESTRING;
}

int PARSEINT(PString PSTR) {
  int return_PARSEINT;
  int RET;
  int POS;
  RET = 0;
  do {
    int first = 1;
    int last = LENGTH(PSTR);
    if (first <= last) {
      POS = first;
      while (1) {
        RET = RET * 10 + ORD(PSTR.chr[POS]) - 48;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return_PARSEINT = RET;
  return return_PARSEINT;
}

struct record26 *PSFACTOR() {
  struct record26 *return_PSFACTOR;
  struct record26 *EXPR;
  PString STR;
  if (LEXER.TOKEN.ID == TKNIL) {
    EXPR = EXNIL();
    READTOKEN();
  }
  else if (LEXER.TOKEN.ID == TKFALSE || LEXER.TOKEN.ID == TKTRUE) {
    EXPR = EXBOOLEANCONSTANT(LEXER.TOKEN.ID == TKTRUE);
    READTOKEN();
  }
  else if (LEXER.TOKEN.ID == TKSTRING) {
    STR = PARSESTRING(GETTOKENVALUEANDREAD(TKSTRING));
    if (LENGTH(STR) == 1) EXPR = EXCHARCONSTANT(STR.chr[1]);
    else EXPR = EXSTRINGCONSTANT(STR);
  }
  else if (LEXER.TOKEN.ID == TKNUMBER) EXPR = EXINTEGERCONSTANT(PARSEINT(GETTOKENVALUEANDREAD(TKNUMBER)));
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) EXPR = PSVARIABLEORFUNCTIONEXTENSION(PSVARIABLE());
  else if (LEXER.TOKEN.ID == TKLPAREN) {
    WANTTOKENANDREAD(TKLPAREN);
    EXPR = PSEXPRESSION();
    WANTTOKENANDREAD(TKRPAREN);
  }
  else if (LEXER.TOKEN.ID == TKNOT) {
    WANTTOKENANDREAD(TKNOT);
    EXPR = EXUNARYOP(PSFACTOR(), TKNOT);
  }
  else COMPILEERROR(cat_ss(str_make(29, "Invalid token in expression: "), LXTOKENSTR()));
  return_PSFACTOR = EXPR;
  return return_PSFACTOR;
}

struct record26 *PSTERM() {
  struct record26 *return_PSTERM;
  TLXTOKENID OP;
  struct record26 *EXPR;
  EXPR = PSFACTOR();
  while (ISOPMULTIPLYING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = EXBINARYOP(EXPR, PSFACTOR(), OP);
  }
  return_PSTERM = EXPR;
  return return_PSTERM;
}

struct record26 *PSSIMPLEEXPRESSION() {
  struct record26 *return_PSSIMPLEEXPRESSION;
  PBoolean NEGATIVE;
  TLXTOKENID OP;
  struct record26 *EXPR;
  NEGATIVE = LEXER.TOKEN.ID == TKMINUS;
  if (NEGATIVE) READTOKEN();
  EXPR = PSTERM();
  if (NEGATIVE) EXPR = EXUNARYOP(EXPR, TKMINUS);
  while (ISOPADDING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = EXBINARYOP(EXPR, PSTERM(), OP);
  }
  return_PSSIMPLEEXPRESSION = EXPR;
  return return_PSSIMPLEEXPRESSION;
}

struct record26 *PSEXPRESSION() {
  struct record26 *return_PSEXPRESSION;
  TLXTOKENID OP;
  struct record26 *EXPR;
  EXPR = PSSIMPLEEXPRESSION();
  while (ISOPRELATIONAL(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = EXBINARYOP(EXPR, PSSIMPLEEXPRESSION(), OP);
  }
  return_PSEXPRESSION = EXPR;
  return return_PSEXPRESSION;
}

void PSASSIGN(struct record26 *LHS, struct record26 *RHS) {
  if (LHS->CLS == XCFUNCTIONREF) {
    if (LHS->FUNCTIONEX.FUNCTIONINDEX != DEFS.CURRENTFUNCTION) COMPILEERROR(str_make(35, "Cannot assign a value to a function"));
    RHS = EXCOERCE(RHS, LHS->FUNCTIONEX.FUNCTIONINDEX->RETURNTYPEINDEX);
    OUTASSIGNRETURNVALUE(LHS, RHS);
  }
  else {
    RHS = EXCOERCE(RHS, LHS->TYPEINDEX);
    if (!LHS->ISASSIGNABLE || LHS->ISCONSTANT) {
      if (LHS->ISFUNCTIONRESULT) COMPILEERROR(str_make(41, "Cannot assign to the result of a function"));
      else COMPILEERROR(str_make(33, "Cannot assign to a constant value"));
    }
    OUTASSIGN(LHS, RHS);
  }
  DISPOSEEXPR(&LHS);
  DISPOSEEXPR(&RHS);
}

void PSSTATEMENTSEQUENCE() {
  OUTSEQUENCEBEGIN();
  SKIPTOKEN(TKBEGIN);
  while (LEXER.TOKEN.ID != TKEND) {
    PSSTATEMENT();
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  }
  OUTSEQUENCEEND();
  SKIPTOKEN(TKEND);
}

void PSIDENTIFIERSTATEMENT() {
  struct record26 *LHS;
  LHS = PSVARIABLE();
  if (LHS->CLS != XCFUNCTIONREF || LHS->FUNCTIONEX.FUNCTIONINDEX != DEFS.CURRENTFUNCTION || LEXER.TOKEN.ID != TKASSIGN) LHS = PSVARIABLEORFUNCTIONEXTENSION(LHS);
  if (LEXER.TOKEN.ID == TKASSIGN) {
    WANTTOKENANDREAD(TKASSIGN);
    PSASSIGN(LHS, PSEXPRESSION());
  }
  else {
    if (LHS->CLS == XCFUNCTIONCALL) {
      OUTPROCEDURECALL(LHS);
      DISPOSEEXPR(&LHS);
    }
    else if (LHS->CLS == XCSPECIALFUNCTIONCALL) {
      OUTSPECIALPROCEDURECALL(LHS);
      DISPOSEEXPR(&LHS);
    }
    else if (LHS->CLS == XCBINARYOP) COMPILEERROR(str_make(56, "Invalid statement (maybe you wrote '=' instead of ':='?)"));
    else COMPILEERROR(str_make(17, "Invalid statement"));
  }
}

void PSIFSTATEMENT() {
  struct record26 *COND;
  WANTTOKENANDREAD(TKIF);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  OUTIF(COND);
  DISPOSEEXPR(&COND);
  WANTTOKENANDREAD(TKTHEN);
  if (LEXER.TOKEN.ID == TKELSE) OUTEMPTYSTATEMENT();
  else PSSTATEMENT();
  if (LEXER.TOKEN.ID == TKELSE) {
    WANTTOKENANDREAD(TKELSE);
    OUTELSE();
    PSSTATEMENT();
  }
  OUTIFEND();
}

void PSCASESTATEMENT() {
  struct record26 *CASEINDEX;
  struct record29 *CASETYPEINDEX;
  struct record26 *CASELABEL;
  WANTTOKENANDREAD(TKCASE);
  CASEINDEX = PSEXPRESSION();
  CASETYPEINDEX = CASEINDEX->TYPEINDEX;
  if (!ISORDINALTYPE(CASETYPEINDEX)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
  OUTCASEBEGIN(CASEINDEX);
  DISPOSEEXPR(&CASEINDEX);
  WANTTOKENANDREAD(TKOF);
  do {
    CASELABEL = EXCOERCE(PSEXPRESSION(), CASETYPEINDEX);
    if (!CASELABEL->ISCONSTANT) COMPILEERROR(str_make(47, "The label of the case statement is not constant"));
    WANTTOKENANDREAD(TKCOLON);
    OUTCASESTATEMENTBEGIN(CASELABEL);
    DISPOSEEXPR(&CASELABEL);
    PSSTATEMENT();
    OUTCASESTATEMENTEND();
    WANTTOKEN3(TKSEMICOLON, TKELSE, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID == TKELSE || LEXER.TOKEN.ID == TKEND));
  OUTCASEELSEBEGIN();
  if (LEXER.TOKEN.ID == TKELSE) do {
    READTOKEN();
    PSSTATEMENT();
    WANTTOKEN2(TKSEMICOLON, TKEND);
  } while (!(LEXER.TOKEN.ID == TKEND));
  OUTCASEELSEEND();
  OUTCASEEND();
  WANTTOKENANDREAD(TKEND);
}

void PSREPEATSTATEMENT() {
  struct record26 *COND;
  WANTTOKENANDREAD(TKREPEAT);
  OUTREPEATBEGIN();
  while (LEXER.TOKEN.ID != TKUNTIL) {
    PSSTATEMENT();
    WANTTOKEN2(TKSEMICOLON, TKUNTIL);
    SKIPTOKEN(TKSEMICOLON);
  }
  WANTTOKENANDREAD(TKUNTIL);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  OUTREPEATEND(COND);
  DISPOSEEXPR(&COND);
}

void PSWHILESTATEMENT() {
  struct record26 *COND;
  WANTTOKENANDREAD(TKWHILE);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  OUTWHILEBEGIN(COND);
  DISPOSEEXPR(&COND);
  WANTTOKENANDREAD(TKDO);
  PSSTATEMENT();
  OUTWHILEEND();
}

void PSFORSTATEMENT() {
  struct record26 *ITER;
  struct record26 *FIRST;
  struct record26 *LAST;
  PBoolean ASCENDING;
  WANTTOKENANDREAD(TKFOR);
  ITER = PSEXPRESSION();
  if (!ITER->ISASSIGNABLE) COMPILEERROR(str_make(36, "Iterator variable must be assignable"));
  if (ITER->ISCONSTANT) COMPILEERROR(str_make(31, "Iterator must not be a constant"));
  if (ITER->ISFUNCTIONRESULT) COMPILEERROR(str_make(45, "Iterator must not be the result of a function"));
  if (!ISORDINALTYPE(ITER->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(33, "Type of iterator is not ordinal: "), TYPENAME(ITER->TYPEINDEX)));
  WANTTOKENANDREAD(TKASSIGN);
  FIRST = PSEXPRESSION();
  WANTTOKEN2(TKTO, TKDOWNTO);
  ASCENDING = LEXER.TOKEN.ID == TKTO;
  READTOKEN();
  LAST = PSEXPRESSION();
  WANTTOKENANDREAD(TKDO);
  OUTFORBEGIN(ITER, FIRST, LAST, ASCENDING);
  PSSTATEMENT();
  OUTFOREND(ITER, ASCENDING);
  DISPOSEEXPR(&ITER);
  DISPOSEEXPR(&FIRST);
  DISPOSEEXPR(&LAST);
}

void PSWITHSTATEMENT() {
  struct record26 *BASE;
  struct record35 *VARINDEX;
  WANTTOKEN(TKWITH);
  STARTTEMPORARYSCOPE();
  OUTSEQUENCEBEGIN();
  do {
    READTOKEN();
    BASE = PSEXPRESSION();
    VARINDEX = ADDWITHVAR(BASE);
    OUTASSIGNTOREFERENCE(VARINDEX, BASE);
    DISPOSEEXPR(&BASE);
    WANTTOKEN2(TKCOMMA, TKDO);
  } while (!(LEXER.TOKEN.ID == TKDO));
  WANTTOKENANDREAD(TKDO);
  PSSTATEMENT();
  OUTSEQUENCEEND();
  CLOSETEMPORARYSCOPE();
}

void PSSTATEMENT() {
  if (LEXER.TOKEN.ID == TKSEMICOLON) OUTEMPTYSTATEMENT();
  else if (LEXER.TOKEN.ID == TKBEGIN) PSSTATEMENTSEQUENCE();
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) PSIDENTIFIERSTATEMENT();
  else if (LEXER.TOKEN.ID == TKIF) PSIFSTATEMENT();
  else if (LEXER.TOKEN.ID == TKCASE) PSCASESTATEMENT();
  else if (LEXER.TOKEN.ID == TKREPEAT) PSREPEATSTATEMENT();
  else if (LEXER.TOKEN.ID == TKWHILE) PSWHILESTATEMENT();
  else if (LEXER.TOKEN.ID == TKFOR) PSFORSTATEMENT();
  else if (LEXER.TOKEN.ID == TKWITH) PSWITHSTATEMENT();
  else COMPILEERROR(cat_ss(str_make(17, "Unexpected token "), LXTOKENSTR()));
}

void PSPROGRAMBLOCK() {
  PSDEFINITIONS();
  WANTTOKENANDREAD(TKBEGIN);
  OUTPROGRAMBEGIN();
  while (LEXER.TOKEN.ID != TKEND) {
    PSSTATEMENT();
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  }
  OUTPROGRAMEND();
  WANTTOKENANDREAD(TKEND);
}

void EXECUTEDIRECTIVE(PString DIR) {
  if (LENGTH(DIR) > 3 && DIR.chr[2] == 'I' && DIR.chr[3] == ' ') LXINCLUDE(COPY(DIR, 4, 255));
}

void READTOKEN() {
  struct record34 *CONSTINDEX;
  TLXPOS TOKENPOS;
  PBoolean STOP;
  do {
    LXREADTOKEN();
    STOP = LEXER.TOKEN.ID != TKCOMMENT;
    if (LEXER.TOKEN.ID == TKIDENTIFIER) {
      CONSTINDEX = FINDCONSTANT(LEXER.TOKEN.VALUE);
      if (CONSTINDEX != (void*)0) {
        TOKENPOS = LEXER.TOKEN.POS;
        LEXER.TOKEN = CONSTINDEX->REPLACEMENT;
        LEXER.TOKEN.POS = TOKENPOS;
      }
    }
    if (LEXER.TOKEN.ID == TKCOMMENT) if (LENGTH(LEXER.TOKEN.VALUE) >= 2 && LEXER.TOKEN.VALUE.chr[1] == '$') EXECUTEDIRECTIVE(LEXER.TOKEN.VALUE);
    if (LEXER.TOKEN.ID == TKEOF && LEXER.PREV.EXISTS) {
      LEXER.INPUT = LEXER.PREV.INPUT;
      LEXER.PREV.EXISTS = 0;
      STOP = 0;
    }
  } while (!STOP);
}

void PARSEPROGRAM() {
  READTOKEN();
  PSPROGRAMHEADING();
  PSPROGRAMBLOCK();
  WANTTOKENANDREAD(TKDOT);
  WANTTOKEN(TKEOF);
}

void STARTGLOBALSCOPE() {
  TPSFUNCTION FUN;
  INITDEFS();
  PRIMITIVETYPES.PTNIL = ADDTYPE(MAKETYPE(str_make(3, "NIL"), TTCNIL));
  PRIMITIVETYPES.PTBOOLEAN = ADDTYPE(MAKETYPE(str_make(7, "BOOLEAN"), TTCBOOLEAN));
  PRIMITIVETYPES.PTINTEGER = ADDTYPE(MAKETYPE(str_make(7, "INTEGER"), TTCINTEGER));
  PRIMITIVETYPES.PTCHAR = ADDTYPE(MAKETYPE(str_make(4, "CHAR"), TTCCHAR));
  PRIMITIVETYPES.PTSTRING = ADDTYPE(MAKETYPE(str_make(6, "STRING"), TTCSTRING));
  PRIMITIVETYPES.PTTEXT = ADDTYPE(MAKETYPE(str_make(4, "TEXT"), TTCTEXT));
  ADDCONSTANT(MAKECONSTANT(str_make(5, "FALSE"), TKFALSE, str_make(5, "FALSE")));
  ADDCONSTANT(MAKECONSTANT(str_make(4, "TRUE"), TKTRUE, str_make(4, "TRUE")));
  ADDCONSTANT(MAKECONSTANT(str_make(6, "MAXINT"), TKNUMBER, str_make(10, "2147483647")));
  ADDVARIABLE(MAKEVARIABLE(str_make(5, "INPUT"), PRIMITIVETYPES.PTTEXT, 0));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "OUTPUT"), PRIMITIVETYPES.PTTEXT, 0));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "STDERR"), PRIMITIVETYPES.PTTEXT, 0));
  ADDSPECIALFUNCTIONNAME(str_make(4, "READ"), TSFREAD);
  ADDSPECIALFUNCTIONNAME(str_make(6, "READLN"), TSFREADLN);
  ADDSPECIALFUNCTIONNAME(str_make(5, "WRITE"), TSFWRITE);
  ADDSPECIALFUNCTIONNAME(str_make(7, "WRITELN"), TSFWRITELN);
  ADDSPECIALFUNCTIONNAME(str_make(3, "STR"), TSFSTR);
  ADDSPECIALFUNCTIONNAME(str_make(3, "NEW"), TSFNEW);
  ADDSPECIALFUNCTIONNAME(str_make(7, "DISPOSE"), TSFDISPOSE);
  FUN.NAME = str_make(6, "ASSIGN");
  FUN.ARGCOUNT = 2;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
  FUN.ARGS[1] = MAKEVARIABLE(str_make(4, "NAME"), PRIMITIVETYPES.PTSTRING, 0);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(3, "CHR");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 1);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTCHAR;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(5, "CLOSE");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(4, "COPY");
  FUN.ARGCOUNT = 3;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING, 0);
  FUN.ARGS[1] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 0);
  FUN.ARGS[2] = MAKEVARIABLE(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER, 0);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTSTRING;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(6, "DELETE");
  FUN.ARGCOUNT = 3;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING, 1);
  FUN.ARGS[1] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 0);
  FUN.ARGS[2] = MAKEVARIABLE(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER, 0);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(3, "EOF");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(4, "HALT");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(4, "CODE"), PRIMITIVETYPES.PTINTEGER, 0);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(6, "LENGTH");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING, 0);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(3, "ORD");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "CHR"), PRIMITIVETYPES.PTCHAR, 0);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(10, "PARAMCOUNT");
  FUN.ARGCOUNT = 0;
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(8, "PARAMSTR");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('I'), PRIMITIVETYPES.PTINTEGER, 0);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTSTRING;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(5, "RESET");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(7, "REWRITE");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(6, "UPCASE");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "CHR"), PRIMITIVETYPES.PTCHAR, 0);
  FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTCHAR;
  ADDFUNCTION(FUN);
}

typedef enum enum47 { TOTNONE, TOTTYPE, TOTVAR, TOTENUMVAL, TOTFUNDEC, TOTFUNDEF } TOUTPUTTYPE;

const char* enumvalues47[] = { "TOTNONE", "TOTTYPE", "TOTVAR", "TOTENUMVAL", "TOTFUNDEC", "TOTFUNDEF" };

struct record48 {
  PFile OUTPUT;
  PBoolean ISMULTISTATEMENT;
  int INDENT;
  PBoolean NEWLINE;
  TOUTPUTTYPE LASTOUT;
} CODEGEN;

void _OUTNEWLINE() {
  writeln(&CODEGEN.OUTPUT);
  CODEGEN.NEWLINE = 1;
}

void _OUTBLANKLINE(TOUTPUTTYPE NEWOUT) {
  if (CODEGEN.INDENT == 0 && (CODEGEN.LASTOUT != NEWOUT || NEWOUT == TOTFUNDEF)) _OUTNEWLINE();
  CODEGEN.LASTOUT = NEWOUT;
}

void _OUTINDENT() {
  int CT;
  if (CODEGEN.NEWLINE) do {
    int first = 1;
    int last = CODEGEN.INDENT;
    if (first <= last) {
      CT = first;
      while (1) {
        write_s(&CODEGEN.OUTPUT, str_make(2, "  "));
        if (CT == last) break;
        ++CT;
      }
    }
  } while(0);
  CODEGEN.NEWLINE = 0;
}

void OUTBEGIN() {
  CODEGEN.ISMULTISTATEMENT = 1;
  write_c(&CODEGEN.OUTPUT, '{');
  _OUTNEWLINE();
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
}

void OUTEND() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  write_c(&CODEGEN.OUTPUT, '}');
  _OUTNEWLINE();
}

void OUTENDSAMELINE() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  write_c(&CODEGEN.OUTPUT, '}');
}

void _OUTCHAR(char CHR) {
  if (CHR == '\'') write_s(&CODEGEN.OUTPUT, str_make(4, "'\\''"));
  else if (CHR == '\\') write_s(&CODEGEN.OUTPUT, str_make(4, "'\\\\'"));
  else if (CHR >= ' ') {
    write_c(&CODEGEN.OUTPUT, '\'');
    write_c(&CODEGEN.OUTPUT, CHR);
    write_c(&CODEGEN.OUTPUT, '\'');
  }
  else COMPILEERROR(str_make(51, "Internal error: escaped chars are not supported yet"));
}

void _OUTSTRING(PString *STR) {
  int POS;
  char CHR;
  if (LENGTH(*STR) == 1) {
    write_s(&CODEGEN.OUTPUT, str_make(7, "str_of("));
    _OUTCHAR((*STR).chr[1]);
    write_c(&CODEGEN.OUTPUT, ')');
  }
  else {
    write_s(&CODEGEN.OUTPUT, str_make(9, "str_make("));
    write_i(&CODEGEN.OUTPUT, LENGTH(*STR));
    write_s(&CODEGEN.OUTPUT, str_make(3, ", \""));
    do {
      int first = 1;
      int last = LENGTH(*STR);
      if (first <= last) {
        POS = first;
        while (1) {
          {
            CHR = (*STR).chr[POS];
            if (CHR == '"') write_s(&CODEGEN.OUTPUT, str_make(2, "\\\""));
            else if (CHR == '\\') write_s(&CODEGEN.OUTPUT, str_make(2, "\\\\"));
            else if (CHR >= ' ') write_c(&CODEGEN.OUTPUT, CHR);
            else COMPILEERROR(str_make(51, "Internal error: escaped chars are not supported yet"));
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    write_s(&CODEGEN.OUTPUT, str_make(2, "\")"));
  }
}

int _BINOPPREC(struct record26 *EXPR) {
  int return__BINOPPREC;
  switch (EXPR->BINARYEX.OP) {
    case TKPLUS:
      if (ISSTRINGYTYPE(EXPR->TYPEINDEX)) return__BINOPPREC = 1;
      else return__BINOPPREC = 4;
      break;
    case TKMINUS:
      return__BINOPPREC = 4;
      break;
    case TKASTERISK:
      return__BINOPPREC = 3;
      break;
    case TKSLASH:
      return__BINOPPREC = 3;
      break;
    case TKDIV:
      return__BINOPPREC = 3;
      break;
    case TKMOD:
      return__BINOPPREC = 3;
      break;
    case TKAND:
      if (ISBOOLEANTYPE(EXPR->TYPEINDEX)) return__BINOPPREC = 11;
      else return__BINOPPREC = 8;
      break;
    case TKOR:
      if (ISBOOLEANTYPE(EXPR->TYPEINDEX)) return__BINOPPREC = 12;
      else return__BINOPPREC = 10;
      break;
    case TKEQUALS:
      return__BINOPPREC = 7;
      break;
    case TKNOTEQUALS:
      return__BINOPPREC = 7;
      break;
    case TKLESSTHAN:
      return__BINOPPREC = 6;
      break;
    case TKMORETHAN:
      return__BINOPPREC = 6;
      break;
    case TKLESSOREQUALS:
      return__BINOPPREC = 6;
      break;
    case TKMOREOREQUALS:
      return__BINOPPREC = 6;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(48, "Internal error: unknown precedence for operator "), LXTOKENNAME(EXPR->BINARYEX.OP)));
      break;
  }
  return return__BINOPPREC;
}

int _PRECEDENCE(struct record26 *EXPR) {
  int return__PRECEDENCE;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      return__PRECEDENCE = 0;
      break;
    case XCTOSTRING:
      return__PRECEDENCE = 0;
      break;
    case XCVARIABLEACCESS:
      if (EXPR->VARIABLEEX.VARIABLEINDEX->ISREFERENCE) return__PRECEDENCE = 2;
      else return__PRECEDENCE = 0;
      break;
    case XCFIELDACCESS:
      return__PRECEDENCE = 1;
      break;
    case XCARRAYACCESS:
      return__PRECEDENCE = 1;
      break;
    case XCPOINTERACCESS:
      return__PRECEDENCE = 2;
      break;
    case XCSTRINGCHAR:
      return__PRECEDENCE = 1;
      break;
    case XCFUNCTIONREF:
      return__PRECEDENCE = 0;
      break;
    case XCFUNCTIONCALL:
      return__PRECEDENCE = 1;
      break;
    case XCUNARYOP:
      return__PRECEDENCE = 2;
      break;
    case XCBINARYOP:
      return__PRECEDENCE = _BINOPPREC(EXPR);
      break;
    default:
      COMPILEERROR(str_make(34, "Internal error: unknown precedence"));
      break;
  }
  return return__PRECEDENCE;
}

void _OUTEXPRESSIONPARENSPREC(struct record26 *EXPR, int PREC) {
  PBoolean USEPARENS;
  USEPARENS = _PRECEDENCE(EXPR) > PREC;
  if (USEPARENS) write_c(&CODEGEN.OUTPUT, '(');
  OUTEXPRESSION(EXPR);
  if (USEPARENS) write_c(&CODEGEN.OUTPUT, ')');
}

void _OUTEXPRESSIONPARENS(struct record26 *EXPR, struct record26 *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF));
}

void _OUTEXPRESSIONPARENSEXTRA(struct record26 *EXPR, struct record26 *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF) - 1);
}

void _OUTEXIMMEDIATE(struct record26 *EXPR) {
  {
    TEXIMMEDIATE *with49 = &EXPR->IMMEDIATEEX;
    switch ((*with49).CLS) {
      case XICNIL:
        write_s(&CODEGEN.OUTPUT, str_make(8, "(void*)0"));
        break;
      case XICBOOLEAN:
        if ((*with49).BOOLEANVALUE) write_c(&CODEGEN.OUTPUT, '1');
        else write_c(&CODEGEN.OUTPUT, '0');
        break;
      case XICINTEGER:
        write_i(&CODEGEN.OUTPUT, (*with49).INTEGERVALUE);
        break;
      case XICCHAR:
        _OUTCHAR((*with49).CHARVALUE);
        break;
      case XICSTRING:
        _OUTSTRING(&(*with49).STRINGVALUE);
        break;
      case XICENUM:
        write_s(&CODEGEN.OUTPUT, EXPR->TYPEINDEX->ENUMINDEX->VALUES[(*with49).ENUMORDINAL - 1]);
        break;
      default:
        break;
    }
  }
}

void _OUTEXVARIABLE(struct record26 *EXPR) {
  if (EXPR->VARIABLEEX.VARIABLEINDEX->ISREFERENCE) {
    write_c(&CODEGEN.OUTPUT, '*');
    write_s(&CODEGEN.OUTPUT, EXPR->VARIABLEEX.VARIABLEINDEX->NAME);
  }
  else write_s(&CODEGEN.OUTPUT, EXPR->VARIABLEEX.VARIABLEINDEX->NAME);
}

void _OUTEXFIELDACCESS(struct record26 *EXPR) {
  if (EXPR->FIELDEX.PARENT->CLS == XCPOINTERACCESS) {
    _OUTEXPRESSIONPARENS(EXPR->FIELDEX.PARENT->POINTEREX.PARENT, EXPR);
    write_s(&CODEGEN.OUTPUT, str_make(2, "->"));
  }
  else {
    _OUTEXPRESSIONPARENS(EXPR->FIELDEX.PARENT, EXPR);
    write_c(&CODEGEN.OUTPUT, '.');
  }
  write_s(&CODEGEN.OUTPUT, EXPR->FIELDEX.PARENT->TYPEINDEX->RECORDINDEX->FIELDS[EXPR->FIELDEX.FIELDNUMBER - 1].NAME);
}

void _OUTEXFUNCTIONCALL(struct record26 *EXPR) {
  int POS;
  _OUTEXPRESSIONPARENS(EXPR->CALLEX.FUNCTIONREF, EXPR);
  write_c(&CODEGEN.OUTPUT, '(');
  do {
    int first = 1;
    int last = EXPR->CALLEX.ARGS.SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          if (EXPR->CALLEX.FUNCTIONREF->FUNCTIONEX.FUNCTIONINDEX->ARGS[POS - 1].ISREFERENCE) {
            if (!EXPR->CALLEX.ARGS.VALUES[POS - 1]->ISASSIGNABLE) COMPILEERROR(str_make(45, "Pass-by-reference argument must be assignable"));
            write_c(&CODEGEN.OUTPUT, '&');
            _OUTEXPRESSIONPARENSPREC(EXPR->CALLEX.ARGS.VALUES[POS - 1], 2);
          }
          else OUTEXPRESSION(EXPR->CALLEX.ARGS.VALUES[POS - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  write_c(&CODEGEN.OUTPUT, ')');
}

void _OUTEXUNARYOP(struct record26 *EXPR) {
  if (EXPR->UNARYEX.OP == TKMINUS) write_c(&CODEGEN.OUTPUT, '-');
  else if (EXPR->UNARYEX.OP == TKNOT && ISBOOLEANTYPE(EXPR->TYPEINDEX)) write_c(&CODEGEN.OUTPUT, '!');
  else if (EXPR->UNARYEX.OP == TKNOT && ISINTEGERTYPE(EXPR->TYPEINDEX)) write_c(&CODEGEN.OUTPUT, '~');
  _OUTEXPRESSIONPARENS(EXPR->UNARYEX.PARENT, EXPR);
}

PBoolean _ISARITHMETICOP(TLXTOKENID OP) {
  PBoolean return__ISARITHMETICOP;
  return__ISARITHMETICOP = OP == TKPLUS || OP == TKMINUS || OP == TKASTERISK || OP == TKSLASH || OP == TKDIV || OP == TKMOD;
  return return__ISARITHMETICOP;
}

PString _GETARITHMETICOP(TLXTOKENID OP) {
  PString return__GETARITHMETICOP;
  switch (OP) {
    case TKPLUS:
      return__GETARITHMETICOP = str_of('+');
      break;
    case TKMINUS:
      return__GETARITHMETICOP = str_of('-');
      break;
    case TKASTERISK:
      return__GETARITHMETICOP = str_of('*');
      break;
    case TKSLASH:
      return__GETARITHMETICOP = str_of('/');
      break;
    case TKDIV:
      return__GETARITHMETICOP = str_of('/');
      break;
    case TKMOD:
      return__GETARITHMETICOP = str_of('%');
      break;
    default:
      break;
  }
  return return__GETARITHMETICOP;
}

PBoolean _ISLOGICALORBITWISEOP(TLXTOKENID OP) {
  PBoolean return__ISLOGICALORBITWISEOP;
  return__ISLOGICALORBITWISEOP = OP == TKAND || OP == TKOR;
  return return__ISLOGICALORBITWISEOP;
}

PString _GETLOGICALOP(TLXTOKENID OP) {
  PString return__GETLOGICALOP;
  if (OP == TKAND) return__GETLOGICALOP = str_make(2, "&&");
  else return__GETLOGICALOP = str_make(2, "||");
  return return__GETLOGICALOP;
}

PString _GETBITWISEOP(TLXTOKENID OP) {
  PString return__GETBITWISEOP;
  if (OP == TKAND) return__GETBITWISEOP = str_of('&');
  else return__GETBITWISEOP = str_of('|');
  return return__GETBITWISEOP;
}

PBoolean _ISRELATIONALOP(TLXTOKENID OP) {
  PBoolean return__ISRELATIONALOP;
  return__ISRELATIONALOP = OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS;
  return return__ISRELATIONALOP;
}

PString _GETRELATIONALOP(TLXTOKENID OP) {
  PString return__GETRELATIONALOP;
  switch (OP) {
    case TKEQUALS:
      return__GETRELATIONALOP = str_make(2, "==");
      break;
    case TKNOTEQUALS:
      return__GETRELATIONALOP = str_make(2, "!=");
      break;
    case TKLESSTHAN:
      return__GETRELATIONALOP = str_of('<');
      break;
    case TKMORETHAN:
      return__GETRELATIONALOP = str_of('>');
      break;
    case TKLESSOREQUALS:
      return__GETRELATIONALOP = str_make(2, "<=");
      break;
    case TKMOREOREQUALS:
      return__GETRELATIONALOP = str_make(2, ">=");
      break;
    default:
      break;
  }
  return return__GETRELATIONALOP;
}

void _OUTEXBINARYOP(struct record26 *EXPR) {
  char LTYPE;
  char RTYPE;
  {
    TEXBINARYOP *with50 = &EXPR->BINARYEX;
    {
      if (ISSTRINGYTYPE((*with50).LEFT->TYPEINDEX)) {
        if (ISCHARTYPE((*with50).LEFT->TYPEINDEX)) LTYPE = 'c';
        else LTYPE = 's';
        if (ISCHARTYPE((*with50).RIGHT->TYPEINDEX)) RTYPE = 'c';
        else RTYPE = 's';
        if ((*with50).OP == TKPLUS) {
          write_s(&CODEGEN.OUTPUT, str_make(4, "cat_"));
          write_c(&CODEGEN.OUTPUT, LTYPE);
          write_c(&CODEGEN.OUTPUT, RTYPE);
          write_c(&CODEGEN.OUTPUT, '(');
          OUTEXPRESSION((*with50).LEFT);
          write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          OUTEXPRESSION((*with50).RIGHT);
          write_c(&CODEGEN.OUTPUT, ')');
        }
        else if (ISCHARTYPE((*with50).LEFT->TYPEINDEX) && ISCHARTYPE((*with50).RIGHT->TYPEINDEX)) {
          _OUTEXPRESSIONPARENS((*with50).LEFT, EXPR);
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with50).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
          _OUTEXPRESSIONPARENSEXTRA((*with50).RIGHT, EXPR);
        }
        else {
          write_s(&CODEGEN.OUTPUT, str_make(4, "cmp_"));
          write_c(&CODEGEN.OUTPUT, LTYPE);
          write_c(&CODEGEN.OUTPUT, RTYPE);
          write_c(&CODEGEN.OUTPUT, '(');
          OUTEXPRESSION((*with50).LEFT);
          write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          OUTEXPRESSION((*with50).RIGHT);
          write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with50).OP));
          write_s(&CODEGEN.OUTPUT, str_make(2, " 0"));
        }
      }
      else if (ISBOOLEANTYPE((*with50).LEFT->TYPEINDEX)) {
        _OUTEXPRESSIONPARENS((*with50).LEFT, EXPR);
        if (_ISLOGICALORBITWISEOP((*with50).OP)) {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETLOGICALOP((*with50).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        else {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with50).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        _OUTEXPRESSIONPARENSEXTRA((*with50).RIGHT, EXPR);
      }
      else if (ISINTEGERTYPE((*with50).LEFT->TYPEINDEX)) {
        _OUTEXPRESSIONPARENS((*with50).LEFT, EXPR);
        if (_ISARITHMETICOP((*with50).OP)) {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETARITHMETICOP((*with50).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        else if (_ISLOGICALORBITWISEOP((*with50).OP)) {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETBITWISEOP((*with50).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        else {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with50).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        _OUTEXPRESSIONPARENSEXTRA((*with50).RIGHT, EXPR);
      }
      else {
        _OUTEXPRESSIONPARENS((*with50).LEFT, EXPR);
        write_c(&CODEGEN.OUTPUT, ' ');
        write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with50).OP));
        write_c(&CODEGEN.OUTPUT, ' ');
        _OUTEXPRESSIONPARENSEXTRA((*with50).RIGHT, EXPR);
      }
    }
  }
}

void OUTEXPRESSION(struct record26 *EXPR) {
  struct record26 *TMPEXPR;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      _OUTEXIMMEDIATE(EXPR);
      break;
    case XCTOSTRING:
      {
        write_s(&CODEGEN.OUTPUT, str_make(7, "str_of("));
        OUTEXPRESSION(EXPR->TOSTRINGEX.PARENT);
        write_c(&CODEGEN.OUTPUT, ')');
      }
      break;
    case XCVARIABLEACCESS:
      _OUTEXVARIABLE(EXPR);
      break;
    case XCFIELDACCESS:
      _OUTEXFIELDACCESS(EXPR);
      break;
    case XCARRAYACCESS:
      {
        _OUTEXPRESSIONPARENS(EXPR->ARRAYEX.PARENT, EXPR);
        write_c(&CODEGEN.OUTPUT, '[');
        TMPEXPR = EXBINARYOP(COPYEXPR(EXPR->ARRAYEX.SUBSCRIPT), COPYEXPR(EXPR->ARRAYEX.PARENT->TYPEINDEX->ARRAYINDEX->LOWBOUND), TKMINUS);
        OUTEXPRESSION(TMPEXPR);
        DISPOSEEXPR(&TMPEXPR);
        write_c(&CODEGEN.OUTPUT, ']');
      }
      break;
    case XCPOINTERACCESS:
      {
        write_c(&CODEGEN.OUTPUT, '*');
        _OUTEXPRESSIONPARENS(EXPR->POINTEREX.PARENT, EXPR);
      }
      break;
    case XCSTRINGCHAR:
      {
        _OUTEXPRESSIONPARENS(EXPR->STRINGCHAREX.PARENT, EXPR);
        write_s(&CODEGEN.OUTPUT, str_make(5, ".chr["));
        OUTEXPRESSION(EXPR->STRINGCHAREX.SUBSCRIPT);
        write_c(&CODEGEN.OUTPUT, ']');
      }
      break;
    case XCFUNCTIONREF:
      write_s(&CODEGEN.OUTPUT, EXPR->FUNCTIONEX.FUNCTIONINDEX->NAME);
      break;
    case XCFUNCTIONCALL:
      _OUTEXFUNCTIONCALL(EXPR);
      break;
    case XCUNARYOP:
      _OUTEXUNARYOP(EXPR);
      break;
    case XCBINARYOP:
      _OUTEXBINARYOP(EXPR);
      break;
    default:
      break;
  }
}

void OUTENUMVALUES(struct record30 *ENUMINDEX) {
  int POSINENUM;
  _OUTBLANKLINE(TOTENUMVAL);
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(22, "const char* enumvalues"));
  write_i(&CODEGEN.OUTPUT, ENUMINDEX->ID);
  write_s(&CODEGEN.OUTPUT, str_make(7, "[] = { "));
  do {
    int first = 1;
    int last = ENUMINDEX->SIZE;
    if (first <= last) {
      POSINENUM = first;
      while (1) {
        {
          if (POSINENUM != 1) write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          write_c(&CODEGEN.OUTPUT, '"');
          write_s(&CODEGEN.OUTPUT, ENUMINDEX->VALUES[POSINENUM - 1]);
          write_c(&CODEGEN.OUTPUT, '"');
        }
        if (POSINENUM == last) break;
        ++POSINENUM;
      }
    }
  } while(0);
  write_s(&CODEGEN.OUTPUT, str_make(3, " };"));
  _OUTNEWLINE();
}

void OUTENUMVALUESFROMCHECKPOINT(struct record41 *CHECKPOINT) {
  struct record41 *DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != (void*)0) {
    if (DEF->CLS == TDCENUM) OUTENUMVALUES(DEF->ENUMINDEX);
    DEF = DEF->NEXT;
  }
}

PString OUTVARIABLENAME(PString NAME, PBoolean ISREFERENCE) {
  PString return_OUTVARIABLENAME;
  if (ISREFERENCE) return_OUTVARIABLENAME = cat_cs('*', NAME);
  else return_OUTVARIABLENAME = NAME;
  return return_OUTVARIABLENAME;
}

PString OUTRETURNVARIABLENAME(PString NAME) {
  PString return_OUTRETURNVARIABLENAME;
  return_OUTRETURNVARIABLENAME = cat_ss(str_make(7, "return_"), NAME);
  return return_OUTRETURNVARIABLENAME;
}

void OUTTYPEREFERENCE(struct record29 *TYPEINDEX) {
  if (TYPEINDEX == (void*)0) write_s(&CODEGEN.OUTPUT, str_make(4, "void"));
  else if (TYPEINDEX->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEINDEX->POINTEDTYPEINDEX);
    write_c(&CODEGEN.OUTPUT, '*');
  }
  else if (TYPEINDEX->CLS == TTCBOOLEAN) write_s(&CODEGEN.OUTPUT, str_make(8, "PBoolean"));
  else if (TYPEINDEX->CLS == TTCINTEGER) write_s(&CODEGEN.OUTPUT, str_make(3, "int"));
  else if (TYPEINDEX->CLS == TTCCHAR) write_s(&CODEGEN.OUTPUT, str_make(4, "char"));
  else if (TYPEINDEX->CLS == TTCSTRING) write_s(&CODEGEN.OUTPUT, str_make(7, "PString"));
  else if (TYPEINDEX->CLS == TTCTEXT) write_s(&CODEGEN.OUTPUT, str_make(5, "PFile"));
  else if (TYPEINDEX->CLS == TTCENUM) {
    write_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
    write_i(&CODEGEN.OUTPUT, TYPEINDEX->ENUMINDEX->ID);
  }
  else if (TYPEINDEX->CLS == TTCRECORD) {
    write_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
    write_i(&CODEGEN.OUTPUT, TYPEINDEX->RECORDINDEX->ID);
  }
  else if (TYPEINDEX->CLS == TTCARRAY) {
    OUTTYPEREFERENCE(TYPEINDEX->ARRAYINDEX->TYPEINDEX);
    write_c(&CODEGEN.OUTPUT, '*');
  }
  else COMPILEERROR(cat_ss(str_make(30, "Error writing type reference: "), TYPENAME(TYPEINDEX)));
}

void OUTNAMEANDRECORD(PString NAME, struct record32 *RECORDINDEX) {
  int POS;
  int NUMVARIANT;
  NUMVARIANT = 0;
  write_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
  write_i(&CODEGEN.OUTPUT, RECORDINDEX->ID);
  if (!RECORDINDEX->HASBEENDEFINED) {
    write_c(&CODEGEN.OUTPUT, ' ');
    OUTBEGIN();
    do {
      int first = 1;
      int last = RECORDINDEX->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (RECORDINDEX->NUMVARIANTS > NUMVARIANT && RECORDINDEX->VARIANTBOUNDS[NUMVARIANT + 1 - 1] == POS) {
              NUMVARIANT = NUMVARIANT + 1;
              if (NUMVARIANT == 1) {
                _OUTINDENT();
                write_s(&CODEGEN.OUTPUT, str_make(6, "union "));
                OUTBEGIN();
              }
              else {
                OUTENDSAMELINE();
                write_c(&CODEGEN.OUTPUT, ';');
                _OUTNEWLINE();
              }
              _OUTINDENT();
              write_s(&CODEGEN.OUTPUT, str_make(7, "struct "));
              OUTBEGIN();
            }
            _OUTINDENT();
            OUTNAMEANDTYPE(RECORDINDEX->FIELDS[POS - 1].NAME, RECORDINDEX->FIELDS[POS - 1].TYPEINDEX);
            write_c(&CODEGEN.OUTPUT, ';');
            _OUTNEWLINE();
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    if (NUMVARIANT > 0) {
      OUTENDSAMELINE();
      write_c(&CODEGEN.OUTPUT, ';');
      _OUTNEWLINE();
      OUTENDSAMELINE();
      write_c(&CODEGEN.OUTPUT, ';');
      _OUTNEWLINE();
    }
    OUTENDSAMELINE();
    RECORDINDEX->HASBEENDEFINED = 1;
  }
  write_c(&CODEGEN.OUTPUT, ' ');
  write_s(&CODEGEN.OUTPUT, NAME);
}

void OUTNAMEANDENUM(PString NAME, struct record30 *ENUMINDEX) {
  int POS;
  write_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
  write_i(&CODEGEN.OUTPUT, ENUMINDEX->ID);
  if (!ENUMINDEX->HASBEENDEFINED) {
    write_s(&CODEGEN.OUTPUT, str_make(3, " { "));
    do {
      int first = 1;
      int last = ENUMINDEX->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS > 1) write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
            write_s(&CODEGEN.OUTPUT, ENUMINDEX->VALUES[POS - 1]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    write_s(&CODEGEN.OUTPUT, str_make(2, " }"));
    ENUMINDEX->HASBEENDEFINED = 1;
  }
  write_c(&CODEGEN.OUTPUT, ' ');
  write_s(&CODEGEN.OUTPUT, NAME);
}

void OUTNAMEANDTYPE(PString NAME, struct record29 *TYPEINDEX) {
  TPSARRAYDEF ARR;
  struct record26 *SIZEEXPR;
  if (TYPEINDEX == (void*)0) {
    write_s(&CODEGEN.OUTPUT, str_make(5, "void "));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEINDEX->POINTEDTYPEINDEX);
    write_s(&CODEGEN.OUTPUT, str_make(2, " *"));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->ALIASFOR != (void*)0 && cmp_ss(TYPEINDEX->NAME, str_make(0, "")) != 0) {
    write_s(&CODEGEN.OUTPUT, TYPEINDEX->NAME);
    write_c(&CODEGEN.OUTPUT, ' ');
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCBOOLEAN) {
    write_s(&CODEGEN.OUTPUT, str_make(9, "PBoolean "));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCINTEGER) {
    write_s(&CODEGEN.OUTPUT, str_make(4, "int "));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCCHAR) {
    write_s(&CODEGEN.OUTPUT, str_make(5, "char "));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCSTRING) {
    write_s(&CODEGEN.OUTPUT, str_make(8, "PString "));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCTEXT) {
    write_s(&CODEGEN.OUTPUT, str_make(6, "PFile "));
    write_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEINDEX->CLS == TTCENUM) OUTNAMEANDENUM(NAME, TYPEINDEX->ENUMINDEX);
  else if (TYPEINDEX->CLS == TTCRECORD) OUTNAMEANDRECORD(NAME, TYPEINDEX->RECORDINDEX);
  else if (TYPEINDEX->CLS == TTCARRAY) {
    ARR = *TYPEINDEX->ARRAYINDEX;
    OUTNAMEANDTYPE(NAME, ARR.TYPEINDEX);
    write_c(&CODEGEN.OUTPUT, '[');
    SIZEEXPR = EXBINARYOP(EXBINARYOP(EXINTEGERCONSTANT(1), COPYEXPR(ARR.HIGHBOUND), TKPLUS), COPYEXPR(ARR.LOWBOUND), TKMINUS);
    OUTEXPRESSION(SIZEEXPR);
    DISPOSEEXPR(&SIZEEXPR);
    write_c(&CODEGEN.OUTPUT, ']');
  }
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(29, "Error writing name and type: "), NAME), str_make(2, ", ")), TYPENAME(TYPEINDEX)));
}

void OUTTYPEDEFINITION(struct record29 *TYPEINDEX) {
  PString NAME;
  _OUTBLANKLINE(TOTTYPE);
  _OUTINDENT();
  NAME = TYPEINDEX->NAME;
  if (TYPEINDEX->ALIASFOR == (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(5, "Type "), NAME), str_make(16, " is not an alias")));
  write_s(&CODEGEN.OUTPUT, str_make(8, "typedef "));
  OUTNAMEANDTYPE(NAME, TYPEINDEX->ALIASFOR);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTTYPEDEFINITIONSFROMCHECKPOINT(struct record41 *CHECKPOINT) {
  struct record41 *DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != (void*)0) {
    if (DEF->CLS == TDCTYPE) {
      _RESOLVEPOINTERUNKNOWN(DEF->TYPEINDEX);
      if (DEF->TYPEINDEX->ALIASFOR != (void*)0) OUTTYPEDEFINITION(DEF->TYPEINDEX);
    }
    DEF = DEF->NEXT;
  }
}

void OUTCONSTANTARRAYBEGIN() {
  write_s(&CODEGEN.OUTPUT, str_make(2, "{ "));
}

void OUTCONSTANTARRAYSEPARATOR() {
  write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
}

void OUTCONSTANTARRAYEND() {
  write_s(&CODEGEN.OUTPUT, str_make(2, " }"));
}

void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF) {
  OUTNAMEANDTYPE(OUTVARIABLENAME(VARDEF.NAME, VARDEF.ISREFERENCE), VARDEF.TYPEINDEX);
}

void OUTVARIABLEDEFINITION(struct record35 *VARINDEX) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  if (VARINDEX->ISCONSTANT) write_s(&CODEGEN.OUTPUT, str_make(6, "const "));
  OUTVARIABLEDECLARATION(*VARINDEX);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTCONSTANTDEFINITIONBEGIN(struct record35 *VARINDEX) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(6, "const "));
  OUTVARIABLEDECLARATION(*VARINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
}

void OUTCONSTANTDEFINITIONEND() {
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF) {
  int POS;
  _OUTINDENT();
  OUTNAMEANDTYPE(DEF.NAME, DEF.RETURNTYPEINDEX);
  write_c(&CODEGEN.OUTPUT, '(');
  do {
    int first = 1;
    int last = DEF.ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          OUTVARIABLEDECLARATION(DEF.ARGS[POS - 1]);
          if (POS != DEF.ARGCOUNT) write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  write_c(&CODEGEN.OUTPUT, ')');
}

void OUTFUNCTIONDECLARATION(struct record36 *FNINDEX) {
  _OUTBLANKLINE(TOTFUNDEC);
  OUTFUNCTIONPROTOTYPE(*FNINDEX);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONDEFINITION(struct record36 *FNINDEX) {
  _OUTBLANKLINE(TOTFUNDEF);
  OUTFUNCTIONPROTOTYPE(*FNINDEX);
  write_c(&CODEGEN.OUTPUT, ' ');
  OUTBEGIN();
  if (FNINDEX->RETURNTYPEINDEX != (void*)0) {
    _OUTINDENT();
    OUTNAMEANDTYPE(OUTRETURNVARIABLENAME(FNINDEX->NAME), FNINDEX->RETURNTYPEINDEX);
    write_c(&CODEGEN.OUTPUT, ';');
    _OUTNEWLINE();
  }
}

void OUTFUNCTIONEND(struct record36 *FNINDEX) {
  if (FNINDEX->RETURNTYPEINDEX != (void*)0) {
    _OUTINDENT();
    write_s(&CODEGEN.OUTPUT, str_make(7, "return "));
    write_s(&CODEGEN.OUTPUT, OUTRETURNVARIABLENAME(FNINDEX->NAME));
    write_c(&CODEGEN.OUTPUT, ';');
    _OUTNEWLINE();
  }
  OUTEND();
}

void OUTPROGRAMHEADING(PString NAME) {
  write_s(&CODEGEN.OUTPUT, str_make(12, "/* Program: "));
  write_s(&CODEGEN.OUTPUT, NAME);
  write_s(&CODEGEN.OUTPUT, str_make(3, " */"));
  _OUTNEWLINE();
  write_s(&CODEGEN.OUTPUT, str_make(20, "#include \"pascual.h\""));
  _OUTNEWLINE();
}

char SHORTTYPENAME(struct record29 *TYPEINDEX) {
  char return_SHORTTYPENAME;
  if (ISBOOLEANTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'b';
  else if (ISINTEGERTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'i';
  else if (ISCHARTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'c';
  else if (ISSTRINGTYPE(TYPEINDEX)) return_SHORTTYPENAME = 's';
  else if (ISENUMTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'e';
  else COMPILEERROR(cat_ss(str_make(30, "No short type name exists for "), TYPENAME(TYPEINDEX)));
  return return_SHORTTYPENAME;
}

void _OUTREAD(struct record26 *EXPR) {
  struct record26 *SRC;
  struct record20 *READARG;
  PBoolean LINEFEED;
  PBoolean BRACES;
  SRC = EXPR->SPECIALFUNCTIONCALLEX.SRC;
  LINEFEED = EXPR->SPECIALFUNCTIONCALLEX.SPECIALFUNCTION == TSFREADLN;
  READARG = EXPR->SPECIALFUNCTIONCALLEX.READARGS;
  BRACES = !CODEGEN.ISMULTISTATEMENT && READARG != (void*)0 && (READARG->NEXT != (void*)0 || LINEFEED);
  if (BRACES) OUTBEGIN();
  while (READARG != (void*)0) {
    _OUTINDENT();
    write_s(&CODEGEN.OUTPUT, str_make(5, "read_"));
    write_c(&CODEGEN.OUTPUT, SHORTTYPENAME(READARG->ARG->TYPEINDEX));
    write_s(&CODEGEN.OUTPUT, str_make(2, "(&"));
    _OUTEXPRESSIONPARENSPREC(SRC, 2);
    write_s(&CODEGEN.OUTPUT, str_make(3, ", &"));
    _OUTEXPRESSIONPARENSPREC(READARG->ARG, 2);
    write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
    READARG = READARG->NEXT;
  }
  if (LINEFEED) {
    _OUTINDENT();
    write_s(&CODEGEN.OUTPUT, str_make(8, "readln(&"));
    _OUTEXPRESSIONPARENSPREC(SRC, 2);
    write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  if (BRACES) OUTEND();
}

void _OUTWRITE(struct record26 *EXPR) {
  struct record26 *DST;
  struct record21 *WRITEARG;
  PBoolean LINEFEED;
  PBoolean BRACES;
  DST = EXPR->SPECIALFUNCTIONCALLEX.DST;
  LINEFEED = EXPR->SPECIALFUNCTIONCALLEX.SPECIALFUNCTION == TSFWRITELN;
  WRITEARG = EXPR->SPECIALFUNCTIONCALLEX.WRITEARGS;
  BRACES = !CODEGEN.ISMULTISTATEMENT && WRITEARG != (void*)0 && (WRITEARG->NEXT != (void*)0 || LINEFEED);
  if (BRACES) OUTBEGIN();
  while (WRITEARG != (void*)0) {
    if (ISENUMTYPE(WRITEARG->ARG->TYPEINDEX)) {
      _OUTINDENT();
      write_s(&CODEGEN.OUTPUT, str_make(9, "write_e(&"));
      _OUTEXPRESSIONPARENSPREC(DST, 2);
      write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
      OUTEXPRESSION(WRITEARG->ARG);
      write_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
      write_i(&CODEGEN.OUTPUT, WRITEARG->ARG->TYPEINDEX->ENUMINDEX->ID);
      write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
      _OUTNEWLINE();
    }
    else {
      _OUTINDENT();
      write_s(&CODEGEN.OUTPUT, str_make(6, "write_"));
      write_c(&CODEGEN.OUTPUT, SHORTTYPENAME(WRITEARG->ARG->TYPEINDEX));
      write_s(&CODEGEN.OUTPUT, str_make(2, "(&"));
      _OUTEXPRESSIONPARENSPREC(DST, 2);
      write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
      OUTEXPRESSION(WRITEARG->ARG);
      write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
      _OUTNEWLINE();
    }
    WRITEARG = WRITEARG->NEXT;
  }
  if (LINEFEED) {
    _OUTINDENT();
    write_s(&CODEGEN.OUTPUT, str_make(9, "writeln(&"));
    _OUTEXPRESSIONPARENSPREC(DST, 2);
    write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  if (BRACES) OUTEND();
}

void _OUTSTR(struct record26 *EXPR) {
  struct record26 *SRC;
  struct record26 *DST;
  SRC = EXPR->SPECIALFUNCTIONCALLEX.SRC;
  DST = EXPR->SPECIALFUNCTIONCALLEX.DST;
  if (ISENUMTYPE(SRC->TYPEINDEX)) {
    _OUTINDENT();
    OUTEXPRESSION(DST);
    write_s(&CODEGEN.OUTPUT, str_make(12, " = to_str_e("));
    OUTEXPRESSION(SRC);
    write_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
    write_i(&CODEGEN.OUTPUT, SRC->TYPEINDEX->ENUMINDEX->ID);
    write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    OUTEXPRESSION(DST);
    write_s(&CODEGEN.OUTPUT, str_make(10, " = to_str_"));
    write_c(&CODEGEN.OUTPUT, SHORTTYPENAME(SRC->TYPEINDEX));
    write_c(&CODEGEN.OUTPUT, '(');
    OUTEXPRESSION(SRC);
    write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
}

void _OUTNEW(struct record26 *EXPR) {
  struct record26 *PTR;
  PTR = EXPR->SPECIALFUNCTIONCALLEX.PTR;
  _OUTINDENT();
  OUTEXPRESSION(PTR);
  write_s(&CODEGEN.OUTPUT, str_make(17, " = malloc(sizeof("));
  OUTTYPEREFERENCE(PTR->TYPEINDEX->POINTEDTYPEINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(3, "));"));
  _OUTNEWLINE();
}

void _OUTDISPOSE(struct record26 *EXPR) {
  struct record26 *PTR;
  PTR = EXPR->SPECIALFUNCTIONCALLEX.PTR;
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(5, "free("));
  OUTEXPRESSION(PTR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
  _OUTNEWLINE();
}

void OUTASSIGN(struct record26 *LHS, struct record26 *RHS) {
  _OUTINDENT();
  OUTEXPRESSION(LHS);
  write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(RHS);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTASSIGNRETURNVALUE(struct record26 *LHS, struct record26 *RHS) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(7, "return_"));
  write_s(&CODEGEN.OUTPUT, LHS->FUNCTIONEX.FUNCTIONINDEX->NAME);
  write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(RHS);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTASSIGNTOREFERENCE(struct record35 *VARINDEX, struct record26 *RHS) {
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(4, " = &"));
  _OUTEXPRESSIONPARENSPREC(RHS, 2);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTIF(struct record26 *EXPR) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(4, "if ("));
  OUTEXPRESSION(EXPR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTELSE() {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(5, "else "));
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTIFEND() {
  CODEGEN.ISMULTISTATEMENT = 1;
}

void OUTSEQUENCEBEGIN() {
  _OUTINDENT();
  OUTBEGIN();
}

void OUTSEQUENCEEND() {
  OUTEND();
}

void OUTCASEBEGIN(struct record26 *CASEINDEX) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(8, "switch ("));
  OUTEXPRESSION(CASEINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  OUTBEGIN();
}

void OUTCASESTATEMENTBEGIN(struct record26 *CASELABEL) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(5, "case "));
  OUTEXPRESSION(CASELABEL);
  write_c(&CODEGEN.OUTPUT, ':');
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASESTATEMENTEND() {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(6, "break;"));
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEELSEBEGIN() {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(8, "default:"));
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASEELSEEND() {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(6, "break;"));
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEEND() {
  OUTEND();
}

void OUTREPEATBEGIN() {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(3, "do "));
  OUTBEGIN();
}

void OUTREPEATEND(struct record26 *EXPR) {
  struct record26 *TMPEXPR;
  OUTENDSAMELINE();
  write_s(&CODEGEN.OUTPUT, str_make(8, " while ("));
  TMPEXPR = EXUNARYOP(COPYEXPR(EXPR), TKNOT);
  OUTEXPRESSION(TMPEXPR);
  DISPOSEEXPR(&TMPEXPR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
  _OUTNEWLINE();
}

void OUTWHILEBEGIN(struct record26 *EXPR) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(7, "while ("));
  OUTEXPRESSION(EXPR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTWHILEEND() {
  CODEGEN.ISMULTISTATEMENT = 1;
}

void OUTFORBEGIN(struct record26 *ITER, struct record26 *FIRSTEXPR, struct record26 *LASTEXPR, PBoolean ASCENDING) {
  struct record29 *LIMITTYPE;
  TPSVARIABLE FIRST;
  TPSVARIABLE LAST;
  LIMITTYPE = ITER->TYPEINDEX;
  if (ISENUMTYPE(LIMITTYPE)) LIMITTYPE = PRIMITIVETYPES.PTINTEGER;
  FIRST = MAKEVARIABLE(str_make(5, "first"), LIMITTYPE, 0);
  LAST = MAKEVARIABLE(str_make(4, "last"), LIMITTYPE, 0);
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(3, "do "));
  OUTBEGIN();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(FIRST);
  write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(FIRSTEXPR);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(LAST);
  write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(LASTEXPR);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(10, "if (first "));
  if (ASCENDING) write_s(&CODEGEN.OUTPUT, str_make(2, "<="));
  else write_s(&CODEGEN.OUTPUT, str_make(2, ">="));
  write_s(&CODEGEN.OUTPUT, str_make(7, " last) "));
  OUTBEGIN();
  _OUTINDENT();
  OUTEXPRESSION(ITER);
  write_s(&CODEGEN.OUTPUT, str_make(9, " = first;"));
  _OUTNEWLINE();
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(10, "while (1) "));
  OUTBEGIN();
}

void OUTFOREND(struct record26 *ITER, PBoolean ASCENDING) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(4, "if ("));
  OUTEXPRESSION(ITER);
  write_s(&CODEGEN.OUTPUT, str_make(16, " == last) break;"));
  _OUTNEWLINE();
  _OUTINDENT();
  if (ASCENDING) write_s(&CODEGEN.OUTPUT, str_make(2, "++"));
  else write_s(&CODEGEN.OUTPUT, str_make(2, "--"));
  OUTEXPRESSION(ITER);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
  OUTEND();
  OUTEND();
  OUTENDSAMELINE();
  write_s(&CODEGEN.OUTPUT, str_make(10, " while(0);"));
  _OUTNEWLINE();
}

void OUTPROCEDURECALL(struct record26 *EXPR) {
  _OUTINDENT();
  OUTEXPRESSION(EXPR);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTSPECIALPROCEDURECALL(struct record26 *EXPR) {
  switch (EXPR->SPECIALFUNCTIONCALLEX.SPECIALFUNCTION) {
    case TSFWRITE:
      _OUTWRITE(EXPR);
      break;
    case TSFWRITELN:
      _OUTWRITE(EXPR);
      break;
    case TSFREAD:
      _OUTREAD(EXPR);
      break;
    case TSFREADLN:
      _OUTREAD(EXPR);
      break;
    case TSFSTR:
      _OUTSTR(EXPR);
      break;
    case TSFNEW:
      _OUTNEW(EXPR);
      break;
    case TSFDISPOSE:
      _OUTDISPOSE(EXPR);
      break;
    default:
      break;
  }
}

void OUTEMPTYSTATEMENT() {
  _OUTINDENT();
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTPROGRAMBEGIN() {
  _OUTBLANKLINE(TOTFUNDEF);
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(20, "void pascual_main() "));
  OUTBEGIN();
}

void OUTPROGRAMEND() {
  OUTEND();
}

void CODEGENRESET() {
  CODEGEN.OUTPUT = OUTPUT;
  CODEGEN.ISMULTISTATEMENT = 0;
  CODEGEN.INDENT = 0;
  CODEGEN.NEWLINE = 1;
  CODEGEN.LASTOUT = TOTNONE;
}

void CODEGENSETOUTPUT(PString FILENAME) {
  ASSIGN(&CODEGEN.OUTPUT, FILENAME);
  REWRITE(&CODEGEN.OUTPUT);
}

void USAGE(PString MSG) {
  if (cmp_ss(MSG, str_make(0, "")) != 0) {
    write_s(&OUTPUT, MSG);
    writeln(&OUTPUT);
  }
  write_s(&OUTPUT, str_make(6, "Usage:"));
  writeln(&OUTPUT);
  write_s(&OUTPUT, PARAMSTR(0));
  write_s(&OUTPUT, str_make(24, " input.pas [-o output.c]"));
  writeln(&OUTPUT);
  writeln(&OUTPUT);
  write_s(&OUTPUT, str_make(48, "If you specify \"-\" as the input or output file, "));
  write_s(&OUTPUT, str_make(26, "stdin/stdout will be used."));
  writeln(&OUTPUT);
  HALT(0);
}

PString REPLACEEXTENSION(PString STR, PString OLD, PString NEW) {
  PString return_REPLACEEXTENSION;
  int BASELEN;
  int POS;
  PBoolean MATCHES;
  return_REPLACEEXTENSION = str_make(0, "");
  BASELEN = LENGTH(STR) - LENGTH(OLD);
  if (BASELEN > 0) {
    MATCHES = 1;
    do {
      int first = 1;
      int last = LENGTH(OLD);
      if (first <= last) {
        POS = first;
        while (1) {
          MATCHES = MATCHES && UPCASE(STR.chr[POS + BASELEN]) == UPCASE(OLD.chr[POS]);
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    if (MATCHES) return_REPLACEEXTENSION = cat_ss(COPY(STR, 1, BASELEN), NEW);
  }
  return return_REPLACEEXTENSION;
}

void PARSECMDLINE() {
  int POS;
  PString INPUTFILE;
  PString OUTPUTFILE;
  enum enum51 { FLAGNONE, FLAGOUTPUT } FLAG;
  PString PARAM;
  const char* enumvalues51[] = { "FLAGNONE", "FLAGOUTPUT" };
  INPUTFILE = str_make(0, "");
  OUTPUTFILE = str_make(0, "");
  FLAG = FLAGNONE;
  do {
    int first = 1;
    int last = PARAMCOUNT();
    if (first <= last) {
      POS = first;
      while (1) {
        {
          PARAM = PARAMSTR(POS);
          if (PARAM.chr[1] == '-' && cmp_sc(PARAM, '-') != 0) {
            if (cmp_ss(PARAM, str_make(2, "-o")) == 0) FLAG = FLAGOUTPUT;
            else if (cmp_ss(PARAM, str_make(2, "-h")) == 0) USAGE(str_make(0, ""));
            else USAGE(cat_ss(str_make(16, "Unknown option: "), PARAM));
          }
          else if (FLAG == FLAGOUTPUT) {
            if (cmp_ss(OUTPUTFILE, str_make(0, "")) != 0) USAGE(str_make(39, "Output file must be specified only once"));
            else OUTPUTFILE = PARAM;
            FLAG = FLAGNONE;
          }
          else {
            if (cmp_ss(INPUTFILE, str_make(0, "")) != 0) USAGE(str_make(38, "Input file must be specified only once"));
            else INPUTFILE = PARAM;
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  if (cmp_ss(INPUTFILE, str_make(0, "")) == 0) USAGE(str_make(28, "Input file must be specified"));
  if (cmp_ss(OUTPUTFILE, str_make(0, "")) == 0) {
    if (cmp_sc(INPUTFILE, '-') == 0) OUTPUTFILE = str_of('-');
    else OUTPUTFILE = REPLACEEXTENSION(INPUTFILE, str_make(4, ".pas"), str_make(2, ".c"));
  }
  if (cmp_ss(OUTPUTFILE, str_make(0, "")) == 0) USAGE(str_make(29, "Output file must be specified"));
  if (cmp_sc(INPUTFILE, '-') != 0) LXOPEN(INPUTFILE);
  if (cmp_sc(OUTPUTFILE, '-') != 0) CODEGENSETOUTPUT(OUTPUTFILE);
}

void CLEARSTATE() {
  LXRESET();
  CODEGENRESET();
}

void pascual_main() {
  CLEARSTATE();
  PARSECMDLINE();
  STARTGLOBALSCOPE();
  PARSEPROGRAM();
  CLOSE(&LEXER.INPUT.SRC);
  CLOSE(&CODEGEN.OUTPUT);
}
