/* Program: COMPILER */
#include "pascual.h"

typedef enum enum1 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKNUMBER, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAND, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH } TLXTOKENID;
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

const char* enumvalues1[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKNUMBER", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAND", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH" };

struct record6 {
  PString LINE;
  TLXTOKEN TOKEN;
  TLXINPUTFILE INPUT;
  struct record5 {
    int EXISTS;
    TLXINPUTFILE INPUT;
  } PREV;
} LEXER;

PString LXTOKENNAME(TLXTOKENID ID) {
  PString RESULT;
  PString NAME;
  NAME = to_str_e(ID, enumvalues1);
  RESULT = NAME;
  return RESULT;
}

PString LXPOSSTR(TLXPOS POS) {
  PString RESULT;
  PString ROW;
  PString COL;
  ROW = to_str_i(POS.ROW);
  COL = to_str_i(POS.COL);
  RESULT = cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(4, "row "), ROW), str_make(5, " col ")), COL), str_make(4, " in ")), LEXER.INPUT.NAME);
  return RESULT;
}

PString LXWHERESTR() {
  PString RESULT;
  RESULT = cat_ss(str_make(6, " near "), LXPOSSTR(LEXER.TOKEN.POS));
  return RESULT;
}

PString LXTOKENSTR() {
  PString RESULT;
  RESULT = cat_sc(cat_ss(cat_ss(LXTOKENNAME(LEXER.TOKEN.ID), str_make(2, " [")), LEXER.TOKEN.VALUE), ']');
  return RESULT;
}

void COMPILEERROR(PString MSG) {
  write_s(&STDERR, MSG);
  write_s(&STDERR, LXWHERESTR());
  writeln(&STDERR);
  HALT(1);
}

int LXISALPHA(char CHR) {
  int RESULT;
  RESULT = CHR >= 'a' && CHR <= 'z' || CHR >= 'A' && CHR <= 'Z';
  return RESULT;
}

int LXISDIGIT(char CHR) {
  int RESULT;
  RESULT = CHR >= '0' && CHR <= '9';
  return RESULT;
}

int LXISALPHANUM(char CHR) {
  int RESULT;
  RESULT = LXISALPHA(CHR) || LXISDIGIT(CHR);
  return RESULT;
}

int LXISIDENTIFIERFIRST(char CHR) {
  int RESULT;
  RESULT = LXISALPHA(CHR) || CHR == '_';
  return RESULT;
}

int LXISIDENTIFIERCHAR(char CHR) {
  int RESULT;
  RESULT = LXISALPHANUM(CHR) || CHR == '_';
  return RESULT;
}

int LXISTOKENWAITING() {
  int RESULT;
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
  RESULT = LENGTH(LEXER.LINE) > 0;
  return RESULT;
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
  int INTOKEN;
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
  int INTOKEN;
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
  int INSTRING;
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
  int DONE;
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

typedef enum enum7 { TPFDISPOSE, TPFNEW, TPFORD, TPFPRED, TPFREAD, TPFREADLN, TPFSTR, TPFSUCC, TPFWRITE, TPFWRITELN } TPSPSEUDOFUN;
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
      int BOOLEANVALUE;
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
  TPSPSEUDOFUN PSEUDOFUN;
} TEXPSEUDOFUNREF;
typedef struct record20 {
  struct record26 *ARG;
  struct record20 *NEXT;
} TEXREADARGS;
typedef struct record21 {
  struct record26 *ARG;
  struct record21 *NEXT;
} TEXWRITEARGS;
typedef struct record22 {
  struct record26 *ARG1;
  struct record26 *ARG2;
  TPSPSEUDOFUN PSEUDOFUN;
  union {
    struct {
      TEXREADARGS *READARGS;
    };
    struct {
      TEXWRITEARGS *WRITEARGS;
    };
  };
} TEXPSEUDOFUNCALL;
typedef struct record23 {
  struct record26 *PARENT;
  TLXTOKENID OP;
} TEXUNARYOP;
typedef struct record24 {
  struct record26 *LEFT;
  struct record26 *RIGHT;
  TLXTOKENID OP;
} TEXBINARYOP;
typedef enum enum25 { XCIMMEDIATE, XCTOSTRING, XCVARIABLEACCESS, XCFIELDACCESS, XCARRAYACCESS, XCPOINTERACCESS, XCSTRINGCHAR, XCFUNCTIONREF, XCFUNCTIONCALL, XCPSEUDOFUNREF, XCPSEUDOFUNCALL, XCUNARYOP, XCBINARYOP } TEXPRESSIONCLASS;
typedef struct record26 {
  struct record29 *TYPEINDEX;
  int ISCONSTANT;
  int ISASSIGNABLE;
  int ISFUNCTIONRESULT;
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
      TEXPSEUDOFUNREF PSEUDOFUNEX;
    };
    struct {
      TEXPSEUDOFUNCALL PSEUDOFUNCALLEX;
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
  int HASBEENDEFINED;
} TPSENUMDEF;
typedef struct record31 {
  PString NAME;
  TPSTYPE *TYPEINDEX;
} TPSRECORDFIELD;
typedef struct record32 {
  int SIZE;
  TPSRECORDFIELD FIELDS[32];
  int NUMVARIANTS;
  int VARIANTBOUNDS[32];
  int ID;
  int HASBEENDEFINED;
} TPSRECORDDEF;
typedef struct record33 {
  TEXPRESSIONOBJ *LOWBOUND;
  TEXPRESSIONOBJ *HIGHBOUND;
  TPSTYPE *TYPEINDEX;
} TPSARRAYDEF;
typedef struct record34 {
  PString NAME;
  TEXPRESSIONOBJ *VALUE;
} TPSCONSTANT;
typedef struct record35 {
  PString NAME;
  TPSTYPE *TYPEINDEX;
  int ISREFERENCE;
  int ISCONSTANT;
} TPSVARIABLE;
typedef struct record36 {
  PString NAME;
  int ARGCOUNT;
  TPSVARIABLE ARGS[4];
  TPSTYPE *RETURNTYPEINDEX;
  int ISDECLARATION;
} TPSFUNCTION;
typedef struct record37 {
  TPSVARIABLE *VARIABLEINDEX;
} TPSWITHVAR;
typedef enum enum38 { TNCTYPE, TNCVARIABLE, TNCCONSTANT, TNCENUMVALUE, TNCFUNCTION, TNCPSEUDOFUN } TPSNAMECLASS;
typedef struct record39 {
  PString NAME;
  TPSNAMECLASS CLS;
  union {
    struct {
      TPSTYPE *TYPEINDEX;
    };
    struct {
      TPSVARIABLE *VARIABLEINDEX;
    };
    struct {
      TPSCONSTANT *CONSTANTINDEX;
    };
    struct {
      TPSTYPE *ENUMTYPEINDEX;
      int ORDINAL;
    };
    struct {
      TPSFUNCTION *FUNCTIONINDEX;
    };
    struct {
      TPSPSEUDOFUN PSEUDOFUN;
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
      TPSNAME *NAMEINDEX;
    };
    struct {
      TPSTYPE *TYPEINDEX;
    };
    struct {
      TPSENUMDEF *ENUMINDEX;
    };
    struct {
      TPSRECORDDEF *RECORDINDEX;
    };
    struct {
      TPSARRAYDEF *ARRAYINDEX;
    };
    struct {
      TPSCONSTANT *CONSTANTINDEX;
    };
    struct {
      TPSVARIABLE *VARIABLEINDEX;
    };
    struct {
      TPSFUNCTION *FUNCTIONINDEX;
    };
    struct {
      TPSWITHVAR *WITHVARINDEX;
    };
    struct {
      int TEMPORARYSCOPE;
      TPSFUNCTION *CURRENTFUNCTION;
    };
  };
} TPSDEFENTRY;
typedef struct record42 {
  TPSDEFENTRY *LATEST;
  TPSFUNCTION *CURRENTFUNCTION;
  int COUNTER;
} TPSDEFS;

const char* enumvalues7[] = { "TPFDISPOSE", "TPFNEW", "TPFORD", "TPFPRED", "TPFREAD", "TPFREADLN", "TPFSTR", "TPFSUCC", "TPFWRITE", "TPFWRITELN" };
const char* enumvalues8[] = { "XICNIL", "XICBOOLEAN", "XICINTEGER", "XICCHAR", "XICSTRING", "XICENUM" };
const char* enumvalues25[] = { "XCIMMEDIATE", "XCTOSTRING", "XCVARIABLEACCESS", "XCFIELDACCESS", "XCARRAYACCESS", "XCPOINTERACCESS", "XCSTRINGCHAR", "XCFUNCTIONREF", "XCFUNCTIONCALL", "XCPSEUDOFUNREF", "XCPSEUDOFUNCALL", "XCUNARYOP", "XCBINARYOP" };
const char* enumvalues28[] = { "TTCBOOLEAN", "TTCINTEGER", "TTCCHAR", "TTCSTRING", "TTCTEXT", "TTCENUM", "TTCRECORD", "TTCARRAY", "TTCPOINTER", "TTCNIL", "TTCPOINTERUNKNOWN" };
const char* enumvalues38[] = { "TNCTYPE", "TNCVARIABLE", "TNCCONSTANT", "TNCENUMVALUE", "TNCFUNCTION", "TNCPSEUDOFUN" };
const char* enumvalues40[] = { "TDCNAME", "TDCTYPE", "TDCENUM", "TDCRECORD", "TDCARRAY", "TDCCONSTANT", "TDCVARIABLE", "TDCFUNCTION", "TDCWITHVAR", "TDCSCOPEBOUNDARY" };

TPSDEFS DEFS;
struct record43 {
  TPSTYPE *PTNIL;
  TPSTYPE *PTBOOLEAN;
  TPSTYPE *PTINTEGER;
  TPSTYPE *PTCHAR;
  TPSTYPE *PTSTRING;
  TPSTYPE *PTTEXT;
} PRIMITIVETYPES;

int DEFCOUNTER() {
  int RESULT;
  DEFS.COUNTER = DEFS.COUNTER + 1;
  RESULT = DEFS.COUNTER;
  return RESULT;
}

void INITDEFS() {
  DEFS.LATEST = (void*)0;
  DEFS.CURRENTFUNCTION = (void*)0;
  DEFS.COUNTER = 0;
}

TPSDEFENTRY *_NEWDEF(TPSDEFCLASS CLS) {
  TPSDEFENTRY *RESULT;
  TPSDEFENTRY *DEF;
  DEF = malloc(sizeof(TPSDEFENTRY));
  DEF->PREV = (void*)0;
  DEF->NEXT = (void*)0;
  DEF->CLS = CLS;
  switch (CLS) {
    case TDCNAME:
      DEF->NAMEINDEX = malloc(sizeof(TPSNAME));
      break;
    case TDCTYPE:
      DEF->TYPEINDEX = malloc(sizeof(TPSTYPE));
      break;
    case TDCENUM:
      DEF->ENUMINDEX = malloc(sizeof(TPSENUMDEF));
      break;
    case TDCRECORD:
      DEF->RECORDINDEX = malloc(sizeof(TPSRECORDDEF));
      break;
    case TDCARRAY:
      DEF->ARRAYINDEX = malloc(sizeof(TPSARRAYDEF));
      break;
    case TDCCONSTANT:
      DEF->CONSTANTINDEX = malloc(sizeof(TPSCONSTANT));
      break;
    case TDCVARIABLE:
      DEF->VARIABLEINDEX = malloc(sizeof(TPSVARIABLE));
      break;
    case TDCFUNCTION:
      DEF->FUNCTIONINDEX = malloc(sizeof(TPSFUNCTION));
      break;
    case TDCWITHVAR:
      DEF->WITHVARINDEX = malloc(sizeof(TPSWITHVAR));
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
  RESULT = DEF;
  return RESULT;
}

void _DISPOSEDEF(TPSDEFENTRY *DEF) {
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

TPSDEFENTRY *_ADDDEF(TPSDEFCLASS CLS) {
  TPSDEFENTRY *RESULT;
  TPSDEFENTRY *DEF;
  DEF = _NEWDEF(CLS);
  DEF->PREV = DEFS.LATEST;
  if (DEFS.LATEST != (void*)0) DEFS.LATEST->NEXT = DEF;
  DEFS.LATEST = DEF;
  RESULT = DEF;
  return RESULT;
}

int _DELETEDEF(TPSDEFENTRY *DELETEDDEF) {
  int RESULT;
  if (DEFS.LATEST == (void*)0) RESULT = 0;
  else {
    *DELETEDDEF = *DEFS.LATEST;
    _DISPOSEDEF(DEFS.LATEST);
    DEFS.LATEST = (*DELETEDDEF).PREV;
    RESULT = 1;
  }
  return RESULT;
}

void _STARTSCOPE(int TEMPORARY, TPSFUNCTION *NEWFUNCTION) {
  TPSDEFENTRY *DEF;
  DEF = _ADDDEF(TDCSCOPEBOUNDARY);
  DEF->TEMPORARYSCOPE = TEMPORARY;
  DEF->CURRENTFUNCTION = DEFS.CURRENTFUNCTION;
  if (!TEMPORARY) DEFS.CURRENTFUNCTION = NEWFUNCTION;
}

void _CLOSESCOPE(int TEMPORARY) {
  TPSDEFENTRY DELETEDDEF;
  int DELETED;
  do {
    DELETED = _DELETEDEF(&DELETEDDEF);
  } while (!(!DELETED || DELETEDDEF.CLS == TDCSCOPEBOUNDARY && (TEMPORARY || !DELETEDDEF.TEMPORARYSCOPE)));
  DEFS.CURRENTFUNCTION = DELETEDDEF.CURRENTFUNCTION;
}

void STARTLOCALSCOPE(TPSFUNCTION *NEWFUNCTION) {
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

TPSNAME *_FINDNAME(PString NAME, int REQUIRED, int FROMLOCALSCOPE) {
  TPSNAME *RESULT;
  TPSDEFENTRY *DEF;
  TPSNAME *RET;
  RET = (void*)0;
  DEF = DEFS.LATEST;
  while (RET == (void*)0 && DEF != (void*)0 && (!FROMLOCALSCOPE || DEF->CLS != TDCSCOPEBOUNDARY)) {
    if (DEF->CLS == TDCNAME && cmp_ss(NAME, DEF->NAMEINDEX->NAME) == 0) RET = DEF->NAMEINDEX;
    DEF = DEF->PREV;
  }
  if (REQUIRED && RET == (void*)0) COMPILEERROR(cat_ss(str_make(20, "Unknown identifier: "), NAME));
  RESULT = RET;
  return RESULT;
}

TPSNAME *_CHECKNAMECLASS(TPSNAME *NAMEINDEX, TPSNAMECLASS CLS) {
  TPSNAME *RESULT;
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
    case TNCPSEUDOFUN:
      COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), NAMEINDEX->NAME));
      break;
    default:
      COMPILEERROR(str_make(35, "Internal error: name class mismatch"));
      break;
  }
  RESULT = NAMEINDEX;
  return RESULT;
}

TPSNAME *FINDNAMEINLOCALSCOPE(PString NAME, int REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _FINDNAME(NAME, REQUIRED, 1);
  return RESULT;
}

TPSNAME *FINDNAMEOFCLASSINLOCALSCOPE(PString NAME, TPSNAMECLASS CLS, int REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _CHECKNAMECLASS(FINDNAMEINLOCALSCOPE(NAME, REQUIRED), CLS);
  return RESULT;
}

TPSNAME *FINDNAME(PString NAME, int REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _FINDNAME(NAME, REQUIRED, 0);
  return RESULT;
}

TPSNAME *FINDNAMEOFCLASS(PString NAME, TPSNAMECLASS CLS, int REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _CHECKNAMECLASS(FINDNAME(NAME, REQUIRED), CLS);
  return RESULT;
}

TPSNAME *_ADDNAME(PString NAME, TPSNAMECLASS CLS) {
  TPSNAME *RESULT;
  TPSNAME *POS;
  if (FINDNAMEINLOCALSCOPE(NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), NAME), str_make(16, " already defined")));
  POS = _ADDDEF(TDCNAME)->NAMEINDEX;
  POS->NAME = NAME;
  POS->CLS = CLS;
  RESULT = POS;
  return RESULT;
}

TPSNAME *ADDTYPENAME(PString NAME, TPSTYPE *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCTYPE);
  DEF->TYPEINDEX = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDVARIABLENAME(PString NAME, TPSVARIABLE *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCVARIABLE);
  DEF->VARIABLEINDEX = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDCONSTANTNAME(PString NAME, TPSCONSTANT *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCCONSTANT);
  DEF->CONSTANTINDEX = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDFUNCTIONNAME(PString NAME, TPSFUNCTION *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCFUNCTION);
  DEF->FUNCTIONINDEX = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDENUMVALUENAME(int ORDINAL, TPSTYPE *TYPEIDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(TYPEIDX->ENUMINDEX->VALUES[ORDINAL - 0], TNCENUMVALUE);
  DEF->ENUMTYPEINDEX = TYPEIDX;
  DEF->ORDINAL = ORDINAL;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDPSEUDOFUN(PString NAME, TPSPSEUDOFUN FN) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCPSEUDOFUN);
  DEF->PSEUDOFUN = FN;
  RESULT = DEF;
  return RESULT;
}

PString DEEPTYPENAME(TPSTYPE *TYPEINDEX, int USEORIGINAL) {
  PString RESULT;
  TPSTYPE TYP;
  PString RET;
  int POS;
  do {
    TYP = *TYPEINDEX;
    TYPEINDEX = TYP.ALIASFOR;
  } while (!(!USEORIGINAL || TYPEINDEX == (void*)0));
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) RESULT = TYP.NAME;
  else if (TYP.CLS == TTCENUM) {
    RET = str_of('(');
    do {
      int first = 0;
      int last = TYP.ENUMINDEX->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 0) RET = cat_sc(RET, ',');
            RET = cat_ss(RET, TYP.ENUMINDEX->VALUES[POS - 0]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    RESULT = cat_sc(RET, ')');
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
    RESULT = cat_ss(RET, str_make(4, " end"));
  }
  else if (TYP.CLS == TTCARRAY) {
    RET = cat_ss(str_make(15, "array [...] of "), DEEPTYPENAME(TYP.ARRAYINDEX->TYPEINDEX, 1));
    RESULT = RET;
  }
  else if (TYP.CLS == TTCPOINTER) RESULT = cat_cs('^', DEEPTYPENAME(TYP.POINTEDTYPEINDEX, 1));
  else {
    RET = to_str_e(TYP.CLS, enumvalues28);
    COMPILEERROR(cat_ss(str_make(37, "Could not get name for type of class "), RET));
  }
  return RESULT;
}

PString TYPENAME(TPSTYPE *TYPEINDEX) {
  PString RESULT;
  if (TYPEINDEX == (void*)0) RESULT = str_make(6, "(none)");
  else RESULT = DEEPTYPENAME(TYPEINDEX, 0);
  return RESULT;
}

TPSTYPE EMPTYTYPE() {
  TPSTYPE RESULT;
  TPSTYPE RET;
  RET.NAME = str_make(0, "");
  RET.CLS = TTCBOOLEAN;
  RET.ALIASFOR = (void*)0;
  RESULT = RET;
  return RESULT;
}

TPSTYPE COPYTYPE(TPSTYPE *TYPEINDEX) {
  TPSTYPE RESULT;
  TPSTYPE NEWTYP;
  NEWTYP = *TYPEINDEX;
  if (NEWTYP.CLS == TTCPOINTERUNKNOWN) {
    NEWTYP.TARGETNAME = malloc(sizeof(PString));
    *NEWTYP.TARGETNAME = *TYPEINDEX->TARGETNAME;
  }
  RESULT = NEWTYP;
  return RESULT;
}

TPSTYPE TYPEOFCLASS(TPSTYPECLASS CLS) {
  TPSTYPE RESULT;
  TPSTYPE RET;
  RET = EMPTYTYPE();
  RET.CLS = CLS;
  RESULT = RET;
  return RESULT;
}

TPSTYPE INTEGERTYPE() {
  TPSTYPE RESULT;
  RESULT = TYPEOFCLASS(TTCINTEGER);
  return RESULT;
}

int ISINTEGERTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCINTEGER;
  return RESULT;
}

TPSTYPE STRINGTYPE() {
  TPSTYPE RESULT;
  RESULT = TYPEOFCLASS(TTCSTRING);
  return RESULT;
}

TPSTYPE CHARTYPE() {
  TPSTYPE RESULT;
  RESULT = TYPEOFCLASS(TTCCHAR);
  return RESULT;
}

int ISSTRINGTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCSTRING;
  return RESULT;
}

int ISCHARTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCCHAR;
  return RESULT;
}

int ISSTRINGYTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = ISSTRINGTYPE(TYPEINDEX) || ISCHARTYPE(TYPEINDEX);
  return RESULT;
}

TPSTYPE BOOLEANTYPE() {
  TPSTYPE RESULT;
  RESULT = TYPEOFCLASS(TTCBOOLEAN);
  return RESULT;
}

int ISBOOLEANTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCBOOLEAN;
  return RESULT;
}

TPSTYPE TEXTTYPE() {
  TPSTYPE RESULT;
  RESULT = TYPEOFCLASS(TTCTEXT);
  return RESULT;
}

int ISTEXTTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCTEXT;
  return RESULT;
}

int ISENUMTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCENUM;
  return RESULT;
}

int ISRECORDTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCRECORD;
  return RESULT;
}

int ISARRAYTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCARRAY;
  return RESULT;
}

TPSTYPE POINTERTYPE(TPSTYPE *TYPEINDEX) {
  TPSTYPE RESULT;
  TPSTYPE TYP;
  TYP = TYPEOFCLASS(TTCPOINTER);
  TYP.POINTEDTYPEINDEX = TYPEINDEX;
  RESULT = TYP;
  return RESULT;
}

int ISPOINTERTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCPOINTER;
  return RESULT;
}

TPSTYPE NILTYPE() {
  TPSTYPE RESULT;
  RESULT = TYPEOFCLASS(TTCNIL);
  return RESULT;
}

int ISNILTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCNIL;
  return RESULT;
}

int ISPOINTERYTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = ISPOINTERTYPE(TYPEINDEX) || ISNILTYPE(TYPEINDEX);
  return RESULT;
}

TPSTYPE POINTERUNKNOWNTYPE(PString TARGETNAME) {
  TPSTYPE RESULT;
  TPSTYPE TYP;
  TYP = TYPEOFCLASS(TTCPOINTERUNKNOWN);
  TYP.TARGETNAME = malloc(sizeof(PString));
  *TYP.TARGETNAME = TARGETNAME;
  RESULT = TYP;
  return RESULT;
}

int ISPOINTERUNKNOWNTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = TYPEINDEX != (void*)0 && TYPEINDEX->CLS == TTCPOINTERUNKNOWN;
  return RESULT;
}

int ISORDINALTYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = ISBOOLEANTYPE(TYPEINDEX) || ISINTEGERTYPE(TYPEINDEX) || ISCHARTYPE(TYPEINDEX) || ISENUMTYPE(TYPEINDEX);
  return RESULT;
}

int ISSIMPLETYPE(TPSTYPE *TYPEINDEX) {
  int RESULT;
  RESULT = ISORDINALTYPE(TYPEINDEX) || ISSTRINGTYPE(TYPEINDEX);
  return RESULT;
}

int ISSAMETYPE(TPSTYPE *A, TPSTYPE *B) {
  int RESULT;
  if (A == (void*)0 || B == (void*)0) RESULT = A == B;
  else {
    while (A->ALIASFOR != (void*)0) A = A->ALIASFOR;
    while (B->ALIASFOR != (void*)0) B = B->ALIASFOR;
    RESULT = A == B || ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE(A->POINTEDTYPEINDEX, B->POINTEDTYPEINDEX);
  }
  return RESULT;
}

int AREPOINTERSCOMPATIBLE(TPSTYPE *A, TPSTYPE *B) {
  int RESULT;
  RESULT = ISPOINTERYTYPE(A) && ISPOINTERYTYPE(B) && (ISNILTYPE(A) || ISNILTYPE(B) || ISSAMETYPE(A, B));
  return RESULT;
}

TPSTYPE *ADDTYPE(TPSTYPE TYP) {
  TPSTYPE *RESULT;
  TPSTYPE *TYPEINDEX;
  int ENUMPOS;
  TYPEINDEX = _ADDDEF(TDCTYPE)->TYPEINDEX;
  *TYPEINDEX = TYP;
  RESULT = TYPEINDEX;
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) {
    if (FINDNAMEINLOCALSCOPE(TYP.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), TYP.NAME), str_make(16, " already defined")));
    ADDTYPENAME(TYP.NAME, TYPEINDEX);
  }
  if (TYP.CLS == TTCENUM && TYP.ALIASFOR == (void*)0) do {
    int first = 0;
    int last = TYP.ENUMINDEX->SIZE - 1;
    if (first <= last) {
      ENUMPOS = first;
      while (1) {
        ADDENUMVALUENAME(ENUMPOS, TYPEINDEX);
        if (ENUMPOS == last) break;
        ++ENUMPOS;
      }
    }
  } while(0);
  return RESULT;
}

TPSENUMDEF *ADDENUM(TPSENUMDEF ENUM) {
  TPSENUMDEF *RESULT;
  TPSENUMDEF *ENUMINDEX;
  ENUMINDEX = _ADDDEF(TDCENUM)->ENUMINDEX;
  *ENUMINDEX = ENUM;
  ENUMINDEX->ID = DEFCOUNTER();
  RESULT = ENUMINDEX;
  return RESULT;
}

TPSRECORDDEF *ADDRECORD(TPSRECORDDEF REC) {
  TPSRECORDDEF *RESULT;
  TPSRECORDDEF *RECORDINDEX;
  RECORDINDEX = _ADDDEF(TDCRECORD)->RECORDINDEX;
  *RECORDINDEX = REC;
  RECORDINDEX->ID = DEFCOUNTER();
  RESULT = RECORDINDEX;
  return RESULT;
}

TPSARRAYDEF *ADDARRAY(TPSARRAYDEF ARR) {
  TPSARRAYDEF *RESULT;
  TPSARRAYDEF *ARRAYINDEX;
  ARRAYINDEX = _ADDDEF(TDCARRAY)->ARRAYINDEX;
  *ARRAYINDEX = ARR;
  RESULT = ARRAYINDEX;
  return RESULT;
}

TPSCONSTANT *ADDCONSTANT(TPSCONSTANT CONSTANT) {
  TPSCONSTANT *RESULT;
  TPSCONSTANT *CONSTANTINDEX;
  if (FINDNAMEINLOCALSCOPE(CONSTANT.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), CONSTANT.NAME), str_make(16, " already defined")));
  CONSTANTINDEX = _ADDDEF(TDCCONSTANT)->CONSTANTINDEX;
  ADDCONSTANTNAME(CONSTANT.NAME, CONSTANTINDEX);
  *CONSTANTINDEX = CONSTANT;
  RESULT = CONSTANTINDEX;
  return RESULT;
}

TPSVARIABLE *ADDVARIABLE(TPSVARIABLE VARDEF) {
  TPSVARIABLE *RESULT;
  TPSVARIABLE *VARIABLEINDEX;
  if (FINDNAMEINLOCALSCOPE(VARDEF.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), VARDEF.NAME), str_make(16, " already defined")));
  VARIABLEINDEX = _ADDDEF(TDCVARIABLE)->VARIABLEINDEX;
  ADDVARIABLENAME(VARDEF.NAME, VARIABLEINDEX);
  *VARIABLEINDEX = VARDEF;
  RESULT = VARIABLEINDEX;
  return RESULT;
}

TPSFUNCTION EMPTYFUNCTION() {
  TPSFUNCTION RESULT;
  TPSFUNCTION RET;
  RET.NAME = str_make(0, "");
  RET.ARGCOUNT = 0;
  RET.RETURNTYPEINDEX = (void*)0;
  RET.ISDECLARATION = 0;
  RESULT = RET;
  return RESULT;
}

int ISEMPTYFUNCTION(TPSFUNCTION FN) {
  int RESULT;
  RESULT = cmp_ss(FN.NAME, str_make(0, "")) == 0;
  return RESULT;
}

int ISSAMEFUNCTIONDEFINITION(TPSFUNCTION *DECLINDEX, TPSFUNCTION FUN) {
  int RESULT;
  TPSFUNCTION DECL;
  int SAME;
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
  RESULT = SAME;
  return RESULT;
}

int HASFORWARDDECLARATION(PString NAME) {
  int RESULT;
  TPSNAME *NAMEINDEX;
  NAMEINDEX = FINDNAMEOFCLASSINLOCALSCOPE(NAME, TNCFUNCTION, 0);
  RESULT = NAMEINDEX != (void*)0 && NAMEINDEX->FUNCTIONINDEX->ISDECLARATION;
  return RESULT;
}

TPSFUNCTION *ADDFUNCTION(TPSFUNCTION FUN) {
  TPSFUNCTION *RESULT;
  TPSNAME *NAMEINDEX;
  TPSFUNCTION *FNINDEX;
  int ISPROCEDURE;
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
  RESULT = FNINDEX;
  return RESULT;
}

int FINDFIELD(TPSTYPE *TYPEINDEX, PString NAME, int REQUIRED) {
  int RESULT;
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
  RESULT = RET;
  return RESULT;
}

TPSTYPE *FINDFIELDTYPE(TPSTYPE *TYPEINDEX, PString NAME, int REQUIRED) {
  TPSTYPE *RESULT;
  int POS;
  POS = FINDFIELD(TYPEINDEX, NAME, REQUIRED);
  if (POS == 0) RESULT = (void*)0;
  else RESULT = TYPEINDEX->RECORDINDEX->FIELDS[POS - 1].TYPEINDEX;
  return RESULT;
}

TPSWITHVAR *FINDWITHVAR(PString NAME) {
  TPSWITHVAR *RESULT;
  TPSWITHVAR *RET;
  TPSDEFENTRY *DEF;
  TPSTYPE *TYPEINDEX;
  RET = (void*)0;
  DEF = DEFS.LATEST;
  while (RET == (void*)0 && DEF != (void*)0 && DEF->CLS != TDCSCOPEBOUNDARY) {
    if (DEF->CLS == TDCWITHVAR) {
      TYPEINDEX = DEF->WITHVARINDEX->VARIABLEINDEX->TYPEINDEX;
      if (FINDFIELDTYPE(TYPEINDEX, NAME, 0) != (void*)0) RET = DEF->WITHVARINDEX;
    }
    DEF = DEF->PREV;
  }
  RESULT = RET;
  return RESULT;
}

TPSVARIABLE *ADDWITHVAR(TEXPRESSIONOBJ *BASE) {
  TPSVARIABLE *RESULT;
  PString TMPVARNUM;
  TPSVARIABLE TMPVAR;
  TPSVARIABLE *TMPVARINDEX;
  TPSWITHVAR *WITHVARINDEX;
  if (!ISRECORDTYPE(BASE->TYPEINDEX)) COMPILEERROR(str_make(31, "'With' variable is not a record"));
  TMPVARNUM = to_str_i(DEFCOUNTER());
  TMPVAR.NAME = cat_ss(str_make(4, "with"), TMPVARNUM);
  TMPVAR.TYPEINDEX = BASE->TYPEINDEX;
  TMPVAR.ISCONSTANT = BASE->ISCONSTANT;
  TMPVAR.ISREFERENCE = 1;
  TMPVARINDEX = ADDVARIABLE(TMPVAR);
  WITHVARINDEX = _ADDDEF(TDCWITHVAR)->WITHVARINDEX;
  WITHVARINDEX->VARIABLEINDEX = TMPVARINDEX;
  RESULT = TMPVARINDEX;
  return RESULT;
}

TPSTYPE MAKETYPE(PString NAME, TPSTYPECLASS CLS) {
  TPSTYPE RESULT;
  TPSTYPE TYP;
  TYP = EMPTYTYPE();
  TYP.NAME = NAME;
  TYP.CLS = CLS;
  RESULT = TYP;
  return RESULT;
}

TPSCONSTANT MAKECONSTANT(PString NAME, TEXPRESSIONOBJ *VALUE) {
  TPSCONSTANT RESULT;
  TPSCONSTANT CONSTANT;
  CONSTANT.NAME = NAME;
  CONSTANT.VALUE = VALUE;
  RESULT = CONSTANT;
  return RESULT;
}

TPSVARIABLE MAKETYPEDCONSTANT(PString NAME, TPSTYPE *TYPEINDEX) {
  TPSVARIABLE RESULT;
  TPSVARIABLE VARDEF;
  VARDEF.NAME = NAME;
  VARDEF.TYPEINDEX = TYPEINDEX;
  VARDEF.ISREFERENCE = 0;
  VARDEF.ISCONSTANT = 1;
  RESULT = VARDEF;
  return RESULT;
}

TPSVARIABLE MAKEVARIABLE(PString NAME, TPSTYPE *TYPEINDEX, int ISREF) {
  TPSVARIABLE RESULT;
  TPSVARIABLE VARDEF;
  VARDEF.NAME = NAME;
  VARDEF.TYPEINDEX = TYPEINDEX;
  VARDEF.ISREFERENCE = ISREF;
  VARDEF.ISCONSTANT = 0;
  RESULT = VARDEF;
  return RESULT;
}

void CODEGENRESET();
void CODEGENSETOUTPUT(PString FILENAME);
void OUTBEGIN();
void OUTEND();
void OUTENUMVALUES(TPSENUMDEF *ENUMINDEX);
void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT);
PString OUTVARIABLENAME(PString NAME, int ISREFERENCE);
void OUTTYPEREFERENCE(TPSTYPE *TYPEINDEX);
void OUTNAMEANDTYPE(PString NAME, TPSTYPE *TYPEINDEX);
void OUTTYPEDEFINITION(TPSTYPE *TYPEINDEX);
void OUTTYPEDEFINITIONSFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT);
void OUTCONSTANTARRAYBEGIN();
void OUTCONSTANTARRAYSEPARATOR();
void OUTCONSTANTARRAYEND();
void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF);
void OUTVARIABLEDEFINITION(TPSVARIABLE *VARINDEX);
void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE *VARINDEX);
void OUTCONSTANTDEFINITIONEND();
void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF);
void OUTFUNCTIONDECLARATION(TPSFUNCTION *FNINDEX);
void OUTFUNCTIONDEFINITION(TPSFUNCTION *FNINDEX);
void OUTFUNCTIONEND(TPSFUNCTION *FNINDEX);
void OUTPROGRAMHEADING(PString NAME);
void OUTEXPRESSION(TEXPRESSIONOBJ *EXPR);
void OUTASSIGN(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS);
void OUTASSIGNRETURNVALUE(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS);
void OUTASSIGNTOREFERENCE(TPSVARIABLE *VARINDEX, TEXPRESSIONOBJ *RHS);
void OUTIF(TEXPRESSIONOBJ *EXPR);
void OUTELSE();
void OUTIFEND();
void OUTSEQUENCEBEGIN();
void OUTSEQUENCEEND();
void OUTCASEBEGIN(TEXPRESSIONOBJ *CASEINDEX);
void OUTCASESTATEMENTBEGIN(TEXPRESSIONOBJ *CASELABEL);
void OUTCASESTATEMENTEND();
void OUTCASEELSEBEGIN();
void OUTCASEELSEEND();
void OUTCASEEND();
void OUTREPEATBEGIN();
void OUTREPEATEND(TEXPRESSIONOBJ *EXPR);
void OUTWHILEBEGIN(TEXPRESSIONOBJ *EXPR);
void OUTWHILEEND();
void OUTFORBEGIN(TEXPRESSIONOBJ *ITER, TEXPRESSIONOBJ *FIRSTEXPR, TEXPRESSIONOBJ *LASTEXPR, int ASCENDING);
void OUTFOREND(TEXPRESSIONOBJ *ITER, int ASCENDING);
void OUTPROCEDURECALL(TEXPRESSIONOBJ *EXPR);
void OUTPSEUDOPROCCALL(TEXPRESSIONOBJ *EXPR);
void OUTEMPTYSTATEMENT();
void OUTPROGRAMBEGIN();
void OUTPROGRAMEND();

TEXPRESSIONOBJ *_NEWEXPR(TEXPRESSIONCLASS CLS) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = malloc(sizeof(TEXPRESSIONOBJ));
  EXPR->CLS = CLS;
  EXPR->ISCONSTANT = 0;
  EXPR->ISASSIGNABLE = 0;
  EXPR->ISFUNCTIONRESULT = 0;
  RESULT = EXPR;
  return RESULT;
}

void DISPOSEEXPR(TEXPRESSIONOBJ **EXPR);

void _DISPOSEPSEUDOCALLEXPR(TEXPSEUDOFUNCALL *CALL) {
  TEXREADARGS *READARG;
  TEXREADARGS *NEXTREADARG;
  TEXWRITEARGS *WRITEARG;
  TEXWRITEARGS *NEXTWRITEARG;
  if ((*CALL).ARG1 != (void*)0) DISPOSEEXPR(&(*CALL).ARG1);
  if ((*CALL).ARG2 != (void*)0) DISPOSEEXPR(&(*CALL).ARG2);
  if ((*CALL).PSEUDOFUN == TPFWRITE || (*CALL).PSEUDOFUN == TPFWRITELN) {
    READARG = (*CALL).READARGS;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      DISPOSEEXPR(&READARG->ARG);
      free(READARG);
      READARG = NEXTREADARG;
    }
  }
  else if ((*CALL).PSEUDOFUN == TPFREAD || (*CALL).PSEUDOFUN == TPFREADLN) {
    WRITEARG = (*CALL).WRITEARGS;
    while (WRITEARG != (void*)0) {
      NEXTWRITEARG = WRITEARG->NEXT;
      DISPOSEEXPR(&WRITEARG->ARG);
      free(WRITEARG);
      WRITEARG = NEXTWRITEARG;
    }
  }
}

void DISPOSEEXPR(TEXPRESSIONOBJ **EXPR) {
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
    case XCPSEUDOFUNCALL:
      _DISPOSEPSEUDOCALLEXPR(&(*EXPR)->PSEUDOFUNCALLEX);
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

TEXPRESSIONOBJ *COPYEXPR(TEXPRESSIONOBJ *EXPR);

void _COPYPSEUDOCALLEXPR(TEXPSEUDOFUNCALL *CALL, TEXPSEUDOFUNCALL *COPY) {
  TEXREADARGS *READARG;
  TEXREADARGS *NEXTREADARG;
  TEXREADARGS *COPYREADARG;
  TEXWRITEARGS *WRITEARG;
  TEXWRITEARGS *NEXTWRITEARG;
  TEXWRITEARGS *COPYWRITEARG;
  (*COPY).PSEUDOFUN = (*CALL).PSEUDOFUN;
  if ((*CALL).ARG1 != (void*)0) (*COPY).ARG1 = COPYEXPR((*CALL).ARG1);
  if ((*CALL).ARG2 != (void*)0) (*COPY).ARG2 = COPYEXPR((*CALL).ARG2);
  if ((*CALL).PSEUDOFUN == TPFWRITE || (*CALL).PSEUDOFUN == TPFWRITELN) {
    READARG = (*CALL).READARGS;
    COPYREADARG = (void*)0;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      if (COPYREADARG == (void*)0) {
        COPYREADARG = malloc(sizeof(TEXREADARGS));
        (*COPY).READARGS = COPYREADARG;
      }
      else {
        COPYREADARG->NEXT = malloc(sizeof(TEXREADARGS));
        COPYREADARG = COPYREADARG->NEXT;
      }
      COPYREADARG->NEXT = (void*)0;
      COPYREADARG->ARG = COPYEXPR(READARG->ARG);
      READARG = NEXTREADARG;
    }
  }
  else if ((*CALL).PSEUDOFUN == TPFREAD || (*CALL).PSEUDOFUN == TPFREADLN) {
    WRITEARG = (*CALL).WRITEARGS;
    while (WRITEARG != (void*)0) {
      NEXTWRITEARG = WRITEARG->NEXT;
      if (COPYWRITEARG == (void*)0) {
        COPYWRITEARG = malloc(sizeof(TEXWRITEARGS));
        (*COPY).WRITEARGS = COPYWRITEARG;
      }
      else {
        COPYWRITEARG->NEXT = malloc(sizeof(TEXWRITEARGS));
        COPYWRITEARG = COPYWRITEARG->NEXT;
      }
      COPYWRITEARG->NEXT = (void*)0;
      COPYWRITEARG->ARG = COPYEXPR(WRITEARG->ARG);
      WRITEARG = NEXTWRITEARG;
    }
  }
}

TEXPRESSIONOBJ *COPYEXPR(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *COPY;
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
    case XCPSEUDOFUNREF:
      COPY->PSEUDOFUNEX = EXPR->PSEUDOFUNEX;
      break;
    case XCPSEUDOFUNCALL:
      _COPYPSEUDOCALLEXPR(&EXPR->PSEUDOFUNCALLEX, &COPY->PSEUDOFUNCALLEX);
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
  RESULT = COPY;
  return RESULT;
}

TEXPRESSIONOBJ *EXCOERCE(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEINDEX);

TEXPRESSIONOBJ *_EXIMMEDIATE(TEXIMMEDIATECLASS CLS) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCIMMEDIATE);
  EXPR->ISCONSTANT = 1;
  EXPR->IMMEDIATEEX.CLS = CLS;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXNIL() {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _EXIMMEDIATE(XICNIL);
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTNIL;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXBOOLEANCONSTANT(int VALUE) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _EXIMMEDIATE(XICBOOLEAN);
  EXPR->IMMEDIATEEX.BOOLEANVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXINTEGERCONSTANT(int VALUE) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _EXIMMEDIATE(XICINTEGER);
  EXPR->IMMEDIATEEX.INTEGERVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXCHARCONSTANT(char VALUE) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _EXIMMEDIATE(XICCHAR);
  EXPR->IMMEDIATEEX.CHARVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTCHAR;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXSTRINGCONSTANT(PString VALUE) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _EXIMMEDIATE(XICSTRING);
  EXPR->IMMEDIATEEX.STRINGVALUE = VALUE;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXENUMCONSTANT(int ORDINAL, TPSTYPE *TYPEINDEX) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  if (!ISENUMTYPE(TYPEINDEX)) COMPILEERROR(cat_ss(str_make(25, "Not an enumeration type: "), TYPENAME(TYPEINDEX)));
  if (ORDINAL < 0 || ORDINAL > TYPEINDEX->ENUMINDEX->SIZE - 1) COMPILEERROR(cat_ss(str_make(18, "Invalid value for "), TYPENAME(TYPEINDEX)));
  EXPR = _EXIMMEDIATE(XICENUM);
  EXPR->IMMEDIATEEX.ENUMORDINAL = ORDINAL;
  EXPR->TYPEINDEX = TYPEINDEX;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXTOSTRING(TEXPRESSIONOBJ *PARENT) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  PString STR;
  if (ISCHARTYPE(PARENT->TYPEINDEX)) {
    if (PARENT->CLS == XCIMMEDIATE) {
      STR = str_of(PARENT->IMMEDIATEEX.CHARVALUE);
      PARENT->IMMEDIATEEX.CLS = XICSTRING;
      PARENT->IMMEDIATEEX.STRINGVALUE = STR;
      RESULT = PARENT;
    }
    else {
      EXPR = _NEWEXPR(XCTOSTRING);
      EXPR->TOSTRINGEX.PARENT = PARENT;
      EXPR->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
      EXPR->ISASSIGNABLE = 0;
      EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
      RESULT = EXPR;
    }
  }
  else if (ISSTRINGTYPE(PARENT->TYPEINDEX)) RESULT = PARENT;
  else COMPILEERROR(cat_ss(str_make(47, "Cannot convert a value of this type to string: "), TYPENAME(PARENT->TYPEINDEX)));
  return RESULT;
}

TEXPRESSIONOBJ *EXVARIABLE(TPSVARIABLE *VARIABLEINDEX) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCVARIABLEACCESS);
  EXPR->VARIABLEEX.VARIABLEINDEX = VARIABLEINDEX;
  EXPR->TYPEINDEX = VARIABLEINDEX->TYPEINDEX;
  EXPR->ISCONSTANT = VARIABLEINDEX->ISCONSTANT;
  EXPR->ISASSIGNABLE = 1;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXFIELDACCESS(TEXPRESSIONOBJ *PARENT, int FIELDNUMBER) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  if (!ISRECORDTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(39, "Cannot access field of non-record type "), TYPENAME(PARENT->TYPEINDEX)));
  if (FIELDNUMBER < 1 || FIELDNUMBER > PARENT->TYPEINDEX->RECORDINDEX->SIZE) COMPILEERROR(cat_ss(str_make(18, "Invalid field for "), TYPENAME(PARENT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCFIELDACCESS);
  EXPR->FIELDEX.PARENT = PARENT;
  EXPR->FIELDEX.FIELDNUMBER = FIELDNUMBER;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX->RECORDINDEX->FIELDS[FIELDNUMBER - 1].TYPEINDEX;
  EXPR->ISCONSTANT = PARENT->ISCONSTANT;
  EXPR->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXARRAYACCESS(TEXPRESSIONOBJ *PARENT, TEXPRESSIONOBJ *SUBSCRIPT) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  if (!ISARRAYTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(42, "Cannot access subscript of non-array type "), TYPENAME(PARENT->TYPEINDEX)));
  if (!ISINTEGERTYPE(SUBSCRIPT->TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(30, "Invalid type for subscript of "), TYPENAME(PARENT->TYPEINDEX)), str_make(2, ": ")), TYPENAME(SUBSCRIPT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCARRAYACCESS);
  EXPR->ARRAYEX.PARENT = PARENT;
  EXPR->ARRAYEX.SUBSCRIPT = SUBSCRIPT;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX->ARRAYINDEX->TYPEINDEX;
  EXPR->ISCONSTANT = PARENT->ISCONSTANT;
  EXPR->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXPOINTERACCESS(TEXPRESSIONOBJ *PARENT) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  if (!ISPOINTERTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(36, "Cannot dereference non-pointer type "), TYPENAME(PARENT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCPOINTERACCESS);
  EXPR->POINTEREX.PARENT = PARENT;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX->POINTEDTYPEINDEX;
  EXPR->ISCONSTANT = 0;
  EXPR->ISASSIGNABLE = 1;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXSTRINGCHAR(TEXPRESSIONOBJ *PARENT, TEXPRESSIONOBJ *SUBSCRIPT) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  if (!ISSTRINGYTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(43, "Cannot access subscript of non-string type "), TYPENAME(PARENT->TYPEINDEX)));
  if (!ISINTEGERTYPE(SUBSCRIPT->TYPEINDEX)) COMPILEERROR(cat_ss(str_make(38, "Invalid type for subscript of string: "), TYPENAME(SUBSCRIPT->TYPEINDEX)));
  EXPR = _NEWEXPR(XCSTRINGCHAR);
  EXPR->ARRAYEX.PARENT = EXTOSTRING(PARENT);
  EXPR->ARRAYEX.SUBSCRIPT = SUBSCRIPT;
  EXPR->TYPEINDEX = PRIMITIVETYPES.PTCHAR;
  EXPR->ISCONSTANT = PARENT->ISCONSTANT;
  EXPR->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXFUNCTIONREF(TPSFUNCTION *FUNCTIONINDEX) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCFUNCTIONREF);
  EXPR->FUNCTIONEX.FUNCTIONINDEX = FUNCTIONINDEX;
  EXPR->TYPEINDEX = (void*)0;
  EXPR->ISCONSTANT = 1;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXFUNCTIONCALL(TEXPRESSIONOBJ *FUNCTIONREF, TEXFUNCTIONARGS *ARGS) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
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
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXPSEUDOFUN(TPSPSEUDOFUN SPECIALFN) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCPSEUDOFUNREF);
  EXPR->PSEUDOFUNEX.PSEUDOFUN = SPECIALFN;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXPSEUDOFUNCALL(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  TPSPSEUDOFUN FN;
  if (EXPR->CLS != XCPSEUDOFUNREF) COMPILEERROR(str_make(25, "Expected a pseudofunction"));
  FN = EXPR->PSEUDOFUNEX.PSEUDOFUN;
  EXPR->CLS = XCPSEUDOFUNCALL;
  EXPR->PSEUDOFUNCALLEX.PSEUDOFUN = FN;
  EXPR->PSEUDOFUNCALLEX.ARG1 = (void*)0;
  EXPR->PSEUDOFUNCALLEX.ARG2 = (void*)0;
  EXPR->PSEUDOFUNCALLEX.READARGS = (void*)0;
  EXPR->PSEUDOFUNCALLEX.WRITEARGS = (void*)0;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXPFCUNIMM(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG);
TEXPRESSIONOBJ *_EXPFCUNCMP(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG);

TEXPRESSIONOBJ *EXPSEUDOFUNCALLUNARY(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG) {
  TEXPRESSIONOBJ *RESULT;
  if (ARG->CLS == XCIMMEDIATE) RESULT = _EXPFCUNIMM(FNEXPR, ARG);
  else RESULT = _EXPFCUNCMP(FNEXPR, ARG);
  return RESULT;
}

TEXPRESSIONOBJ *_EXPFCBIIMM(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG1, TEXPRESSIONOBJ *ARG2);
TEXPRESSIONOBJ *_EXPFCBICMP(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG1, TEXPRESSIONOBJ *ARG2);

TEXPRESSIONOBJ *EXPSEUDOFUNCALLBINARY(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG1, TEXPRESSIONOBJ *ARG2) {
  TEXPRESSIONOBJ *RESULT;
  if (ARG1->CLS == XCIMMEDIATE && ARG2->CLS == XCIMMEDIATE) RESULT = _EXPFCBIIMM(FNEXPR, ARG1, ARG2);
  else RESULT = _EXPFCBICMP(FNEXPR, ARG1, ARG2);
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPIMM(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXUNOPCMP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);

TEXPRESSIONOBJ *EXUNARYOP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (OP == TKMINUS || OP == TKPLUS) {
    if (!ISINTEGERTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(17, "Invalid type for "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(PARENT->TYPEINDEX)));
  }
  else if (OP == TKNOT) {
    if (!ISBOOLEANTYPE(PARENT->TYPEINDEX) && !ISINTEGERTYPE(PARENT->TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(17, "Invalid type for "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(PARENT->TYPEINDEX)));
  }
  else COMPILEERROR(cat_ss(str_make(24, "Invalid unary operator: "), LXTOKENNAME(OP)));
  if (PARENT->CLS == XCIMMEDIATE) RESULT = _EXUNOPIMM(PARENT, OP);
  else RESULT = _EXUNOPCMP(PARENT, OP);
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPIMM(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (OP == TKMINUS && PARENT->IMMEDIATEEX.CLS == XICINTEGER) PARENT->IMMEDIATEEX.INTEGERVALUE = -PARENT->IMMEDIATEEX.INTEGERVALUE;
  else if (OP == TKPLUS && PARENT->IMMEDIATEEX.CLS == XICINTEGER) ;
  else if (OP == TKNOT && PARENT->IMMEDIATEEX.CLS == XICBOOLEAN) PARENT->IMMEDIATEEX.BOOLEANVALUE = !PARENT->IMMEDIATEEX.BOOLEANVALUE;
  else if (OP == TKNOT && PARENT->IMMEDIATEEX.CLS == XICINTEGER) PARENT->IMMEDIATEEX.INTEGERVALUE = ~PARENT->IMMEDIATEEX.INTEGERVALUE;
  else COMPILEERROR(str_make(49, "Internal error: invalid immediate unary operation"));
  RESULT = PARENT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPCMP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCUNARYOP);
  EXPR->UNARYEX.PARENT = PARENT;
  EXPR->UNARYEX.OP = OP;
  EXPR->TYPEINDEX = PARENT->TYPEINDEX;
  EXPR->ISCONSTANT = 1;
  EXPR->ISASSIGNABLE = 0;
  EXPR->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPINTIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPSTRIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPENUMIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPBOOLCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPINTCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPSTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPENUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPPTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);

TEXPRESSIONOBJ *EXBINARYOP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int IMMEDIATE;
  IMMEDIATE = LEFT->CLS == XCIMMEDIATE && RIGHT->CLS == XCIMMEDIATE;
  if (ISBOOLEANTYPE(LEFT->TYPEINDEX) && ISBOOLEANTYPE(RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) RESULT = _EXBINOPBOOLIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPBOOLCMP(LEFT, RIGHT, OP);
  }
  else if (ISINTEGERTYPE(LEFT->TYPEINDEX) && ISINTEGERTYPE(RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) RESULT = _EXBINOPINTIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPINTCMP(LEFT, RIGHT, OP);
  }
  else if (ISSTRINGYTYPE(LEFT->TYPEINDEX) && ISSTRINGYTYPE(RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) RESULT = _EXBINOPSTRIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPSTRCMP(LEFT, RIGHT, OP);
  }
  else if (ISENUMTYPE(LEFT->TYPEINDEX) && ISSAMETYPE(LEFT->TYPEINDEX, RIGHT->TYPEINDEX)) {
    if (IMMEDIATE) RESULT = _EXBINOPENUMIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPENUMCMP(LEFT, RIGHT, OP);
  }
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEINDEX, RIGHT->TYPEINDEX)) RESULT = _EXBINOPPTRCMP(LEFT, RIGHT, OP);
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(27, "Type mismatch for operator "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(LEFT->TYPEINDEX)), str_make(5, " and ")), TYPENAME(RIGHT->TYPEINDEX)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int LT;
  int RT;
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
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPINTIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int LT;
  int RT;
  int BO;
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
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPSTRIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  PString LT;
  PString RT;
  int BO;
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
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPENUMIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int LT;
  int RT;
  int BO;
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
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  if (OP == TKAND || OP == TKOR || OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) {
    EXPR = _NEWEXPR(XCBINARYOP);
    EXPR->BINARYEX.LEFT = LEFT;
    EXPR->BINARYEX.RIGHT = RIGHT;
    EXPR->BINARYEX.OP = OP;
    EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
    EXPR->ISCONSTANT = 1;
    EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
    RESULT = EXPR;
  }
  else COMPILEERROR(cat_ss(str_make(26, "Invalid boolean operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPINTCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS || OP == TKMINUS || OP == TKASTERISK || OP == TKDIV || OP == TKMOD || OP == TKAND || OP == TKOR) EXPR->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(26, "Invalid integer operator: "), LXTOKENNAME(OP)));
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPSTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTSTRING;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPENUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(23, "Invalid enum operator: "), LXTOKENNAME(OP)));
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPPTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCBINARYOP);
  EXPR->BINARYEX.LEFT = LEFT;
  EXPR->BINARYEX.RIGHT = RIGHT;
  EXPR->BINARYEX.OP = OP;
  EXPR->ISCONSTANT = 1;
  EXPR->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKEQUALS || OP == TKNOTEQUALS) EXPR->TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXPFCUNIMM(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG) {
  TEXPRESSIONOBJ *RESULT;
  TPSPSEUDOFUN PF;
  int OUTOFBOUNDS;
  OUTOFBOUNDS = 0;
  PF = FNEXPR->PSEUDOFUNEX.PSEUDOFUN;
  DISPOSEEXPR(&FNEXPR);
  if (PF == TPFNEW || PF == TPFDISPOSE) COMPILEERROR(str_make(16, "Invalid argument"));
  else if (PF == TPFORD) {
    {
      TEXIMMEDIATE *with46 = &ARG->IMMEDIATEEX;
      {
        switch ((*with46).CLS) {
          case XICBOOLEAN:
            if ((*with46).BOOLEANVALUE) (*with46).INTEGERVALUE = 1;
            else (*with46).INTEGERVALUE = 0;
            break;
          case XICINTEGER:
            ;
            break;
          case XICCHAR:
            (*with46).INTEGERVALUE = ORD((*with46).CHARVALUE);
            break;
          case XICENUM:
            (*with46).INTEGERVALUE = (*with46).ENUMORDINAL;
            break;
          default:
            COMPILEERROR(str_make(12, "Invalid type"));
            break;
        }
        ARG->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
        (*with46).CLS = XICINTEGER;
      }
    }
  }
  else if (PF == TPFPRED) {
    {
      TEXIMMEDIATE *with47 = &ARG->IMMEDIATEEX;
      {
        switch ((*with47).CLS) {
          case XICBOOLEAN:
            if ((*with47).BOOLEANVALUE) (*with47).BOOLEANVALUE = 0;
            else OUTOFBOUNDS = 1;
            break;
          case XICINTEGER:
            (*with47).INTEGERVALUE = (*with47).INTEGERVALUE - 1;
            break;
          case XICCHAR:
            if (ORD((*with47).CHARVALUE) > 0) (*with47).CHARVALUE = CHR(ORD((*with47).CHARVALUE) - 1);
            else OUTOFBOUNDS = 1;
            break;
          case XICENUM:
            if ((*with47).ENUMORDINAL > 0) (*with47).ENUMORDINAL = (*with47).ENUMORDINAL - 1;
            else OUTOFBOUNDS = 1;
            break;
          default:
            COMPILEERROR(str_make(12, "Invalid type"));
            break;
        }
      }
    }
  }
  else if (PF == TPFSUCC) {
    {
      TEXIMMEDIATE *with48 = &ARG->IMMEDIATEEX;
      {
        switch ((*with48).CLS) {
          case XICBOOLEAN:
            if (!(*with48).BOOLEANVALUE) (*with48).BOOLEANVALUE = 1;
            else OUTOFBOUNDS = 1;
            break;
          case XICINTEGER:
            (*with48).INTEGERVALUE = (*with48).INTEGERVALUE + 1;
            break;
          case XICCHAR:
            if (ORD((*with48).CHARVALUE) < 255) (*with48).CHARVALUE = CHR(ORD((*with48).CHARVALUE) + 1);
            else OUTOFBOUNDS = 1;
            break;
          case XICENUM:
            if ((*with48).ENUMORDINAL < ARG->TYPEINDEX->ENUMINDEX->SIZE - 1) (*with48).ENUMORDINAL = (*with48).ENUMORDINAL + 1;
            else OUTOFBOUNDS = 1;
            break;
          default:
            COMPILEERROR(str_make(12, "Invalid type"));
            break;
        }
      }
    }
  }
  if (OUTOFBOUNDS) COMPILEERROR(str_make(13, "Out of bounds"));
  RESULT = ARG;
  return RESULT;
}

TEXPRESSIONOBJ *_EXPFCUNCMP(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG) {
  TEXPRESSIONOBJ *RESULT;
  FNEXPR = EXPSEUDOFUNCALL(FNEXPR);
  FNEXPR->PSEUDOFUNCALLEX.ARG1 = ARG;
  switch (FNEXPR->PSEUDOFUNCALLEX.PSEUDOFUN) {
    case TPFORD:
      FNEXPR->TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
      break;
    case TPFPRED:
      FNEXPR->TYPEINDEX = ARG->TYPEINDEX;
      break;
    case TPFSUCC:
      FNEXPR->TYPEINDEX = ARG->TYPEINDEX;
      break;
    default:
      break;
  }
  RESULT = FNEXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXPFCBIIMM(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG1, TEXPRESSIONOBJ *ARG2) {
  TEXPRESSIONOBJ *RESULT;
  TPSPSEUDOFUN PF;
  PF = FNEXPR->PSEUDOFUNEX.PSEUDOFUN;
  if (PF == TPFSTR) RESULT = _EXPFCBICMP(FNEXPR, ARG1, ARG2);
  return RESULT;
}

TEXPRESSIONOBJ *_EXPFCBICMP(TEXPRESSIONOBJ *FNEXPR, TEXPRESSIONOBJ *ARG1, TEXPRESSIONOBJ *ARG2) {
  TEXPRESSIONOBJ *RESULT;
  FNEXPR = EXPSEUDOFUNCALL(FNEXPR);
  FNEXPR->PSEUDOFUNCALLEX.ARG1 = ARG1;
  FNEXPR->PSEUDOFUNCALLEX.ARG2 = ARG2;
  RESULT = FNEXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXCOERCE(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEINDEX) {
  TEXPRESSIONOBJ *RESULT;
  if (ISCHARTYPE(EXPR->TYPEINDEX) && ISSTRINGTYPE(TYPEINDEX)) RESULT = EXTOSTRING(EXPR);
  else if (ISSAMETYPE(EXPR->TYPEINDEX, TYPEINDEX)) RESULT = EXPR;
  else if (ISNILTYPE(EXPR->TYPEINDEX) && ISPOINTERYTYPE(TYPEINDEX)) RESULT = EXPR;
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(24, "Type mismatch: expected "), TYPENAME(TYPEINDEX)), str_make(6, ", got ")), TYPENAME(EXPR->TYPEINDEX)));
  return RESULT;
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
  PString RESULT;
  WANTTOKEN(ID);
  RESULT = LEXER.TOKEN.VALUE;
  READTOKEN();
  return RESULT;
}

void SKIPTOKEN(TLXTOKENID ID) {
  if (LEXER.TOKEN.ID == ID) READTOKEN();
}

TPSTYPE *PSTYPEDENOTER();

TPSTYPE *PSTYPEIDENTIFIER() {
  TPSTYPE *RESULT;
  WANTTOKEN(TKIDENTIFIER);
  RESULT = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 1)->TYPEINDEX;
  READTOKEN();
  return RESULT;
}

TPSIDENTIFIER PSIDENTIFIER() {
  TPSIDENTIFIER RESULT;
  TPSIDENTIFIER IDENT;
  IDENT.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  RESULT = IDENT;
  return RESULT;
}

typedef enum enum49 { LHS, RHS } EXPRESSIONTYPE;

const char* enumvalues49[] = { "LHS", "RHS" };

TEXPRESSIONOBJ *PSEXPRESSION();

TPSTYPE *PSENUMERATEDTYPE() {
  TPSTYPE *RESULT;
  TPSTYPE TYP;
  TPSENUMDEF ENUM;
  WANTTOKENANDREAD(TKLPAREN);
  ENUM.SIZE = 0;
  ENUM.HASBEENDEFINED = 0;
  do {
    ENUM.SIZE = ENUM.SIZE + 1;
    if (ENUM.SIZE > 128) COMPILEERROR(str_make(23, "Too many values in enum"));
    ENUM.VALUES[ENUM.SIZE - 1 - 0] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRPAREN));
  TYP = EMPTYTYPE();
  TYP.CLS = TTCENUM;
  TYP.ENUMINDEX = ADDENUM(ENUM);
  RESULT = ADDTYPE(TYP);
  SKIPTOKEN(TKRPAREN);
  return RESULT;
}

void PSRECORDFIELD(TPSRECORDDEF *REC, TLXTOKENID DELIMITER) {
  PString NAME;
  int LASTFIELD;
  int FIELD;
  TPSTYPE *TYPEINDEX;
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
  TPSTYPE *TAGTYPE;
  TEXPRESSIONOBJ *CASELABEL;
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

TPSTYPE *PSRECORDTYPE() {
  TPSTYPE *RESULT;
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
  RESULT = ADDTYPE(TYP);
  return RESULT;
}

TEXPRESSIONOBJ *PSIMMEDIATE() {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = PSEXPRESSION();
  if (EXPR->CLS != XCIMMEDIATE) COMPILEERROR(str_make(32, "Expected an immediate expression"));
  RESULT = EXPR;
  return RESULT;
}

TPSTYPE *PSARRAYTYPE() {
  TPSTYPE *RESULT;
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
  RESULT = ADDTYPE(TYP);
  return RESULT;
}

TPSTYPE *PSPOINTERTYPE() {
  TPSTYPE *RESULT;
  TPSTYPE TYP;
  TPSNAME *NAMEINDEX;
  WANTTOKENANDREAD(TKCARET);
  WANTTOKEN(TKIDENTIFIER);
  NAMEINDEX = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 0);
  if (NAMEINDEX == (void*)0) TYP = POINTERUNKNOWNTYPE(LEXER.TOKEN.VALUE);
  else TYP = POINTERTYPE(NAMEINDEX->TYPEINDEX);
  READTOKEN();
  RESULT = ADDTYPE(TYP);
  return RESULT;
}

TPSTYPE *PSTYPEDENOTER() {
  TPSTYPE *RESULT;
  TPSTYPE *TYPEINDEX;
  TYPEINDEX = (void*)0;
  if (LEXER.TOKEN.ID == TKIDENTIFIER) TYPEINDEX = PSTYPEIDENTIFIER();
  else if (LEXER.TOKEN.ID == TKLPAREN) TYPEINDEX = PSENUMERATEDTYPE();
  else if (LEXER.TOKEN.ID == TKRECORD) TYPEINDEX = PSRECORDTYPE();
  else if (LEXER.TOKEN.ID == TKARRAY) TYPEINDEX = PSARRAYTYPE();
  else if (LEXER.TOKEN.ID == TKCARET) TYPEINDEX = PSPOINTERTYPE();
  else COMPILEERROR(cat_ss(str_make(30, "Wanted type definition, found "), LXTOKENSTR()));
  RESULT = TYPEINDEX;
  return RESULT;
}

void _RESOLVEPOINTERUNKNOWN(TPSTYPE *TYPEINDEX) {
  TPSTYPE *TARGETINDEX;
  if (TYPEINDEX->CLS == TTCPOINTERUNKNOWN) {
    TARGETINDEX = FINDNAMEOFCLASS(*TYPEINDEX->TARGETNAME, TNCTYPE, 1)->TYPEINDEX;
    free(TYPEINDEX->TARGETNAME);
    TYPEINDEX->CLS = TTCPOINTER;
    TYPEINDEX->POINTEDTYPEINDEX = TARGETINDEX;
  }
}

void PSTYPEDEFINITIONS() {
  PString NAME;
  TPSTYPE *TYPEINDEX;
  TPSTYPE NEWTYPE;
  TPSDEFENTRY *CHECKPOINT;
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
  TEXPRESSIONOBJ *VALUE;
  WANTTOKENANDREAD(TKEQUALS);
  CONSTANT.NAME = NAME;
  CONSTANT.VALUE = PSIMMEDIATE();
  ADDCONSTANT(CONSTANT);
}

void PSCONSTANTVALUE(TPSTYPE *TYPEINDEX) {
  TEXPRESSIONOBJ *EXPR;
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
  TPSTYPE *TYPEINDEX;
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
  TPSTYPE *TYPEINDEX;
  TPSDEFENTRY *CHECKPOINT;
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

void PSFUNCTIONBODY(TPSFUNCTION *FNINDEX) {
  int POS;
  TPSDEFENTRY *CHECKPOINT;
  TPSVARIABLE RETURNVAR;
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
  if (FNINDEX->RETURNTYPEINDEX != (void*)0) OUTVARIABLEDEFINITION(ADDVARIABLE(MAKEVARIABLE(str_make(6, "RESULT"), FNINDEX->RETURNTYPEINDEX, 0)));
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
  int ISREFERENCE;
  int LASTARG;
  int ARG;
  TPSTYPE *TYPEINDEX;
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

TPSTYPE *PSRESULTTYPE() {
  TPSTYPE *RESULT;
  RESULT = PSTYPEIDENTIFIER();
  return RESULT;
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
  int DONE;
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

TEXPRESSIONOBJ *PSPOINTERDEREF(TEXPRESSIONOBJ *PTR) {
  TEXPRESSIONOBJ *RESULT;
  WANTTOKENANDREAD(TKCARET);
  RESULT = EXPOINTERACCESS(PTR);
  return RESULT;
}

TEXPRESSIONOBJ *PSREAD(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  int FIRST;
  TEXPRESSIONOBJ *OUTVAR;
  TEXREADARGS *READARG;
  EXPR = EXPSEUDOFUNCALL(FNEXPR);
  EXPR->PSEUDOFUNCALLEX.ARG1 = EXVARIABLE(FINDNAMEOFCLASS(str_make(5, "INPUT"), TNCVARIABLE, 1)->VARIABLEINDEX);
  READARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTVAR = PSEXPRESSION();
      if (FIRST && OUTVAR->ISASSIGNABLE && ISTEXTTYPE(OUTVAR->TYPEINDEX)) {
        DISPOSEEXPR(&EXPR->PSEUDOFUNCALLEX.ARG1);
        EXPR->PSEUDOFUNCALLEX.ARG1 = OUTVAR;
      }
      else {
        if (!OUTVAR->ISASSIGNABLE || !ISSTRINGYTYPE(OUTVAR->TYPEINDEX)) COMPILEERROR(str_make(36, "Invalid expression for read argument"));
        if (READARG == (void*)0) {
          EXPR->PSEUDOFUNCALLEX.READARGS = malloc(sizeof(TEXREADARGS));
          READARG = EXPR->PSEUDOFUNCALLEX.READARGS;
        }
        else {
          READARG->NEXT = malloc(sizeof(TEXREADARGS));
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
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSWRITE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  int FIRST;
  TEXPRESSIONOBJ *OUTEXPR;
  TEXWRITEARGS *WRITEARG;
  EXPR = EXPSEUDOFUNCALL(FNEXPR);
  EXPR->PSEUDOFUNCALLEX.ARG1 = EXVARIABLE(FINDNAMEOFCLASS(str_make(6, "OUTPUT"), TNCVARIABLE, 1)->VARIABLEINDEX);
  WRITEARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTEXPR = PSEXPRESSION();
      if (FIRST && OUTEXPR->ISASSIGNABLE && ISTEXTTYPE(OUTEXPR->TYPEINDEX)) {
        DISPOSEEXPR(&EXPR->PSEUDOFUNCALLEX.ARG1);
        EXPR->PSEUDOFUNCALLEX.ARG1 = OUTEXPR;
      }
      else {
        if (WRITEARG == (void*)0) {
          EXPR->PSEUDOFUNCALLEX.WRITEARGS = malloc(sizeof(TEXWRITEARGS));
          WRITEARG = EXPR->PSEUDOFUNCALLEX.WRITEARGS;
        }
        else {
          WRITEARG->NEXT = malloc(sizeof(TEXWRITEARGS));
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
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSSTR(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  TEXPRESSIONOBJ *SRC;
  TEXPRESSIONOBJ *DEST;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = PSEXPRESSION();
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  if (!DEST->ISASSIGNABLE || !ISSTRINGTYPE(DEST->TYPEINDEX)) COMPILEERROR(str_make(45, "Destination argument is not a string variable"));
  WANTTOKENANDREAD(TKRPAREN);
  EXPR = EXPSEUDOFUNCALLBINARY(FNEXPR, SRC, DEST);
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSORDPRECSUCC(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  TEXPRESSIONOBJ *VALUE;
  WANTTOKENANDREAD(TKLPAREN);
  VALUE = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  if (!ISORDINALTYPE(VALUE->TYPEINDEX)) COMPILEERROR(str_make(38, "Argument does not have an ordinal type"));
  EXPR = EXPSEUDOFUNCALLUNARY(FNEXPR, VALUE);
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSNEWDISPOSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  TEXPRESSIONOBJ *PTR;
  WANTTOKENANDREAD(TKLPAREN);
  PTR = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  if (!PTR->ISASSIGNABLE || !ISPOINTERTYPE(PTR->TYPEINDEX)) COMPILEERROR(str_make(25, "Argument is not a pointer"));
  EXPR = EXPSEUDOFUNCALLUNARY(FNEXPR, PTR);
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSFUNCTIONCALL(TEXPRESSIONOBJ *FN) {
  TEXPRESSIONOBJ *RESULT;
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
    RESULT = EXFUNCTIONCALL(FN, &ARGS);
  }
  else if (FN->CLS == XCPSEUDOFUNREF) {
    switch (FN->PSEUDOFUNEX.PSEUDOFUN) {
      case TPFDISPOSE:
        RESULT = PSNEWDISPOSE(FN);
        break;
      case TPFNEW:
        RESULT = PSNEWDISPOSE(FN);
        break;
      case TPFORD:
        RESULT = PSORDPRECSUCC(FN);
        break;
      case TPFPRED:
        RESULT = PSORDPRECSUCC(FN);
        break;
      case TPFREAD:
        RESULT = PSREAD(FN);
        break;
      case TPFREADLN:
        RESULT = PSREAD(FN);
        break;
      case TPFSTR:
        RESULT = PSSTR(FN);
        break;
      case TPFSUCC:
        RESULT = PSORDPRECSUCC(FN);
        break;
      case TPFWRITE:
        RESULT = PSWRITE(FN);
        break;
      case TPFWRITELN:
        RESULT = PSWRITE(FN);
        break;
      default:
        COMPILEERROR(str_make(46, "Internal error: unimplemented special function"));
        break;
    }
  }
  return RESULT;
}

TEXPRESSIONOBJ *PSARRAYACCESS(TEXPRESSIONOBJ *ARR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *IDX;
  WANTTOKENANDREAD(TKLBRACKET);
  IDX = PSEXPRESSION();
  WANTTOKENANDREAD(TKRBRACKET);
  if (ISSTRINGYTYPE(ARR->TYPEINDEX)) RESULT = EXSTRINGCHAR(ARR, IDX);
  else RESULT = EXARRAYACCESS(ARR, IDX);
  return RESULT;
}

TEXPRESSIONOBJ *PSFIELDACCESS(TEXPRESSIONOBJ *REC) {
  TEXPRESSIONOBJ *RESULT;
  TPSIDENTIFIER FLD;
  WANTTOKENANDREAD(TKDOT);
  FLD = PSIDENTIFIER();
  RESULT = EXFIELDACCESS(REC, FINDFIELD(REC->TYPEINDEX, FLD.NAME, 1));
  return RESULT;
}

TEXPRESSIONOBJ *PSVARIABLE() {
  TEXPRESSIONOBJ *RESULT;
  TPSIDENTIFIER ID;
  TPSWITHVAR *WITHVARINDEX;
  TPSNAME FOUND;
  TEXPRESSIONOBJ *EXPR;
  int DONE;
  ID = PSIDENTIFIER();
  WITHVARINDEX = FINDWITHVAR(ID.NAME);
  if (WITHVARINDEX != (void*)0) {
    EXPR = EXVARIABLE(WITHVARINDEX->VARIABLEINDEX);
    EXPR = EXFIELDACCESS(EXPR, FINDFIELD(EXPR->TYPEINDEX, ID.NAME, 1));
  }
  else {
    FOUND = *FINDNAME(ID.NAME, 1);
    if (FOUND.CLS == TNCVARIABLE) EXPR = EXVARIABLE(FOUND.VARIABLEINDEX);
    else if (FOUND.CLS == TNCCONSTANT) EXPR = COPYEXPR(FOUND.CONSTANTINDEX->VALUE);
    else if (FOUND.CLS == TNCFUNCTION) EXPR = EXFUNCTIONREF(FOUND.FUNCTIONINDEX);
    else if (FOUND.CLS == TNCENUMVALUE) EXPR = EXENUMCONSTANT(FOUND.ORDINAL, FOUND.ENUMTYPEINDEX);
    else if (FOUND.CLS == TNCPSEUDOFUN) EXPR = EXPSEUDOFUN(FOUND.PSEUDOFUN);
    else COMPILEERROR(cat_ss(str_make(20, "Invalid identifier: "), ID.NAME));
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSVARIABLEORFUNCTIONEXTENSION(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  int DONE;
  DONE = 0;
  do {
    if (EXPR->CLS == XCFUNCTIONREF || EXPR->CLS == XCPSEUDOFUNREF) EXPR = PSFUNCTIONCALL(EXPR);
    else if (LEXER.TOKEN.ID == TKDOT) EXPR = PSFIELDACCESS(EXPR);
    else if (LEXER.TOKEN.ID == TKLBRACKET) EXPR = PSARRAYACCESS(EXPR);
    else if (LEXER.TOKEN.ID == TKCARET) EXPR = PSPOINTERDEREF(EXPR);
    else DONE = 1;
  } while (!DONE);
  RESULT = EXPR;
  return RESULT;
}

int ISOPADDING(TLXTOKEN TOK) {
  int RESULT;
  RESULT = TOK.ID == TKPLUS || TOK.ID == TKMINUS || TOK.ID == TKOR;
  return RESULT;
}

int ISOPMULTIPLYING(TLXTOKEN TOK) {
  int RESULT;
  RESULT = TOK.ID == TKASTERISK || TOK.ID == TKSLASH || TOK.ID == TKDIV || TOK.ID == TKMOD || TOK.ID == TKAND;
  return RESULT;
}

int ISOPRELATIONAL(TLXTOKEN TOK) {
  int RESULT;
  RESULT = TOK.ID == TKEQUALS || TOK.ID == TKNOTEQUALS || TOK.ID == TKLESSTHAN || TOK.ID == TKMORETHAN || TOK.ID == TKLESSOREQUALS || TOK.ID == TKMOREOREQUALS || TOK.ID == TKIN;
  return RESULT;
}

PString PARSESTRING(PString PSTR) {
  PString RESULT;
  int INSTR;
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
  RESULT = STR;
  return RESULT;
}

int PARSEINT(PString PSTR) {
  int RESULT;
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
  RESULT = RET;
  return RESULT;
}

TEXPRESSIONOBJ *PSFACTOR() {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  PString STR;
  if (LEXER.TOKEN.ID == TKNIL) {
    EXPR = EXNIL();
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
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSTERM() {
  TEXPRESSIONOBJ *RESULT;
  TLXTOKENID OP;
  TEXPRESSIONOBJ *EXPR;
  EXPR = PSFACTOR();
  while (ISOPMULTIPLYING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = EXBINARYOP(EXPR, PSFACTOR(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSSIMPLEEXPRESSION() {
  TEXPRESSIONOBJ *RESULT;
  int NEGATIVE;
  TLXTOKENID OP;
  TEXPRESSIONOBJ *EXPR;
  NEGATIVE = LEXER.TOKEN.ID == TKMINUS;
  if (NEGATIVE) READTOKEN();
  EXPR = PSTERM();
  if (NEGATIVE) EXPR = EXUNARYOP(EXPR, TKMINUS);
  while (ISOPADDING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = EXBINARYOP(EXPR, PSTERM(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSEXPRESSION() {
  TEXPRESSIONOBJ *RESULT;
  TLXTOKENID OP;
  TEXPRESSIONOBJ *EXPR;
  EXPR = PSSIMPLEEXPRESSION();
  while (ISOPRELATIONAL(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = EXBINARYOP(EXPR, PSSIMPLEEXPRESSION(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

void PSASSIGN(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS) {
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
  TEXPRESSIONOBJ *LHS;
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
    else if (LHS->CLS == XCPSEUDOFUNCALL) {
      OUTPSEUDOPROCCALL(LHS);
      DISPOSEEXPR(&LHS);
    }
    else if (LHS->CLS == XCBINARYOP) COMPILEERROR(str_make(56, "Invalid statement (maybe you wrote '=' instead of ':='?)"));
    else COMPILEERROR(str_make(17, "Invalid statement"));
  }
}

void PSIFSTATEMENT() {
  TEXPRESSIONOBJ *COND;
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
  TEXPRESSIONOBJ *CASEINDEX;
  TPSTYPE *CASETYPEINDEX;
  TEXPRESSIONOBJ *CASELABEL;
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
  TEXPRESSIONOBJ *COND;
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
  TEXPRESSIONOBJ *COND;
  WANTTOKENANDREAD(TKWHILE);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  OUTWHILEBEGIN(COND);
  DISPOSEEXPR(&COND);
  WANTTOKENANDREAD(TKDO);
  PSSTATEMENT();
  OUTWHILEEND();
}

void PSFORSTATEMENT() {
  TEXPRESSIONOBJ *ITER;
  TEXPRESSIONOBJ *FIRST;
  TEXPRESSIONOBJ *LAST;
  int ASCENDING;
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
  TEXPRESSIONOBJ *BASE;
  TPSVARIABLE *VARINDEX;
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
  TLXPOS TOKENPOS;
  int STOP;
  do {
    LXREADTOKEN();
    STOP = LEXER.TOKEN.ID != TKCOMMENT;
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
  ADDCONSTANT(MAKECONSTANT(str_make(5, "FALSE"), EXBOOLEANCONSTANT(0)));
  ADDCONSTANT(MAKECONSTANT(str_make(4, "TRUE"), EXBOOLEANCONSTANT(1)));
  ADDCONSTANT(MAKECONSTANT(str_make(6, "MAXINT"), EXINTEGERCONSTANT(32767)));
  ADDVARIABLE(MAKEVARIABLE(str_make(5, "INPUT"), PRIMITIVETYPES.PTTEXT, 0));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "OUTPUT"), PRIMITIVETYPES.PTTEXT, 0));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "STDERR"), PRIMITIVETYPES.PTTEXT, 0));
  ADDPSEUDOFUN(str_make(7, "DISPOSE"), TPFDISPOSE);
  ADDPSEUDOFUN(str_make(3, "NEW"), TPFNEW);
  ADDPSEUDOFUN(str_make(3, "ORD"), TPFORD);
  ADDPSEUDOFUN(str_make(4, "PRED"), TPFPRED);
  ADDPSEUDOFUN(str_make(4, "READ"), TPFREAD);
  ADDPSEUDOFUN(str_make(6, "READLN"), TPFREADLN);
  ADDPSEUDOFUN(str_make(3, "STR"), TPFSTR);
  ADDPSEUDOFUN(str_make(4, "SUCC"), TPFSUCC);
  ADDPSEUDOFUN(str_make(5, "WRITE"), TPFWRITE);
  ADDPSEUDOFUN(str_make(7, "WRITELN"), TPFWRITELN);
  FUN.NAME = str_make(6, "ASSIGN");
  FUN.ARGCOUNT = 2;
  FUN.ARGS[0] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
  FUN.ARGS[1] = MAKEVARIABLE(str_make(4, "NAME"), PRIMITIVETYPES.PTSTRING, 0);
  FUN.RETURNTYPEINDEX = (void*)0;
  ADDFUNCTION(FUN);
  FUN.NAME = str_make(3, "CHR");
  FUN.ARGCOUNT = 1;
  FUN.ARGS[0] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 0);
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

typedef enum enum50 { TOTNONE, TOTTYPE, TOTVAR, TOTENUMVAL, TOTFUNDEC, TOTFUNDEF } TOUTPUTTYPE;

const char* enumvalues50[] = { "TOTNONE", "TOTTYPE", "TOTVAR", "TOTENUMVAL", "TOTFUNDEC", "TOTFUNDEF" };

struct record51 {
  PFile OUTPUT;
  int ISMULTISTATEMENT;
  int INDENT;
  int NEWLINE;
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

int _BINOPPREC(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  switch (EXPR->BINARYEX.OP) {
    case TKPLUS:
      if (ISSTRINGYTYPE(EXPR->TYPEINDEX)) RESULT = 1;
      else RESULT = 4;
      break;
    case TKMINUS:
      RESULT = 4;
      break;
    case TKASTERISK:
      RESULT = 3;
      break;
    case TKSLASH:
      RESULT = 3;
      break;
    case TKDIV:
      RESULT = 3;
      break;
    case TKMOD:
      RESULT = 3;
      break;
    case TKAND:
      if (ISBOOLEANTYPE(EXPR->TYPEINDEX)) RESULT = 11;
      else RESULT = 8;
      break;
    case TKOR:
      if (ISBOOLEANTYPE(EXPR->TYPEINDEX)) RESULT = 12;
      else RESULT = 10;
      break;
    case TKEQUALS:
      RESULT = 7;
      break;
    case TKNOTEQUALS:
      RESULT = 7;
      break;
    case TKLESSTHAN:
      RESULT = 6;
      break;
    case TKMORETHAN:
      RESULT = 6;
      break;
    case TKLESSOREQUALS:
      RESULT = 6;
      break;
    case TKMOREOREQUALS:
      RESULT = 6;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(48, "Internal error: unknown precedence for operator "), LXTOKENNAME(EXPR->BINARYEX.OP)));
      break;
  }
  return RESULT;
}

int _PRECEDENCE(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      RESULT = 0;
      break;
    case XCTOSTRING:
      RESULT = 0;
      break;
    case XCVARIABLEACCESS:
      if (EXPR->VARIABLEEX.VARIABLEINDEX->ISREFERENCE) RESULT = 2;
      else RESULT = 0;
      break;
    case XCFIELDACCESS:
      RESULT = 1;
      break;
    case XCARRAYACCESS:
      RESULT = 1;
      break;
    case XCPOINTERACCESS:
      RESULT = 2;
      break;
    case XCSTRINGCHAR:
      RESULT = 1;
      break;
    case XCFUNCTIONREF:
      RESULT = 0;
      break;
    case XCFUNCTIONCALL:
      RESULT = 1;
      break;
    case XCPSEUDOFUNREF:
      RESULT = 0;
      break;
    case XCPSEUDOFUNCALL:
      RESULT = 1;
      break;
    case XCUNARYOP:
      RESULT = 2;
      break;
    case XCBINARYOP:
      RESULT = _BINOPPREC(EXPR);
      break;
    default:
      COMPILEERROR(str_make(34, "Internal error: unknown precedence"));
      break;
  }
  return RESULT;
}

void _OUTEXPRESSIONPARENSPREC(TEXPRESSIONOBJ *EXPR, int PREC) {
  int USEPARENS;
  USEPARENS = _PRECEDENCE(EXPR) > PREC;
  if (USEPARENS) write_c(&CODEGEN.OUTPUT, '(');
  OUTEXPRESSION(EXPR);
  if (USEPARENS) write_c(&CODEGEN.OUTPUT, ')');
}

void _OUTEXPRESSIONPARENS(TEXPRESSIONOBJ *EXPR, TEXPRESSIONOBJ *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF));
}

void _OUTEXPRESSIONPARENSEXTRA(TEXPRESSIONOBJ *EXPR, TEXPRESSIONOBJ *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF) - 1);
}

void _OUTEXIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  {
    TEXIMMEDIATE *with52 = &EXPR->IMMEDIATEEX;
    switch ((*with52).CLS) {
      case XICNIL:
        write_s(&CODEGEN.OUTPUT, str_make(8, "(void*)0"));
        break;
      case XICBOOLEAN:
        if ((*with52).BOOLEANVALUE) write_c(&CODEGEN.OUTPUT, '1');
        else write_c(&CODEGEN.OUTPUT, '0');
        break;
      case XICINTEGER:
        write_i(&CODEGEN.OUTPUT, (*with52).INTEGERVALUE);
        break;
      case XICCHAR:
        _OUTCHAR((*with52).CHARVALUE);
        break;
      case XICSTRING:
        _OUTSTRING(&(*with52).STRINGVALUE);
        break;
      case XICENUM:
        write_s(&CODEGEN.OUTPUT, EXPR->TYPEINDEX->ENUMINDEX->VALUES[(*with52).ENUMORDINAL - 0]);
        break;
      default:
        break;
    }
  }
}

void _OUTEXVARIABLE(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->VARIABLEEX.VARIABLEINDEX->ISREFERENCE) {
    write_c(&CODEGEN.OUTPUT, '*');
    write_s(&CODEGEN.OUTPUT, EXPR->VARIABLEEX.VARIABLEINDEX->NAME);
  }
  else write_s(&CODEGEN.OUTPUT, EXPR->VARIABLEEX.VARIABLEINDEX->NAME);
}

void _OUTEXFIELDACCESS(TEXPRESSIONOBJ *EXPR) {
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

void _OUTEXFUNCTIONCALL(TEXPRESSIONOBJ *EXPR) {
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

void _OUTDISPOSE(TEXPRESSIONOBJ *EXPR);
void _OUTNEW(TEXPRESSIONOBJ *EXPR);
void _OUTORD(TEXPRESSIONOBJ *EXPR);
void _OUTPRED(TEXPRESSIONOBJ *EXPR);
void _OUTREAD(TEXPRESSIONOBJ *EXPR);
void _OUTSTR(TEXPRESSIONOBJ *EXPR);
void _OUTSUCC(TEXPRESSIONOBJ *EXPR);
void _OUTWRITE(TEXPRESSIONOBJ *EXPR);

void _OUTEXPSEUDOFUNCALL(TEXPRESSIONOBJ *EXPR) {
  switch (EXPR->PSEUDOFUNCALLEX.PSEUDOFUN) {
    case TPFDISPOSE:
      _OUTDISPOSE(EXPR);
      break;
    case TPFNEW:
      _OUTNEW(EXPR);
      break;
    case TPFORD:
      _OUTORD(EXPR);
      break;
    case TPFPRED:
      _OUTPRED(EXPR);
      break;
    case TPFREAD:
      _OUTREAD(EXPR);
      break;
    case TPFREADLN:
      _OUTREAD(EXPR);
      break;
    case TPFSTR:
      _OUTSTR(EXPR);
      break;
    case TPFSUCC:
      _OUTSUCC(EXPR);
      break;
    case TPFWRITE:
      _OUTWRITE(EXPR);
      break;
    case TPFWRITELN:
      _OUTWRITE(EXPR);
      break;
    default:
      COMPILEERROR(str_make(46, "Internal error: unimplemented special function"));
      break;
  }
}

void _OUTEXUNARYOP(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->UNARYEX.OP == TKMINUS) write_c(&CODEGEN.OUTPUT, '-');
  else if (EXPR->UNARYEX.OP == TKNOT && ISBOOLEANTYPE(EXPR->TYPEINDEX)) write_c(&CODEGEN.OUTPUT, '!');
  else if (EXPR->UNARYEX.OP == TKNOT && ISINTEGERTYPE(EXPR->TYPEINDEX)) write_c(&CODEGEN.OUTPUT, '~');
  _OUTEXPRESSIONPARENS(EXPR->UNARYEX.PARENT, EXPR);
}

int _ISARITHMETICOP(TLXTOKENID OP) {
  int RESULT;
  RESULT = OP == TKPLUS || OP == TKMINUS || OP == TKASTERISK || OP == TKSLASH || OP == TKDIV || OP == TKMOD;
  return RESULT;
}

PString _GETARITHMETICOP(TLXTOKENID OP) {
  PString RESULT;
  switch (OP) {
    case TKPLUS:
      RESULT = str_of('+');
      break;
    case TKMINUS:
      RESULT = str_of('-');
      break;
    case TKASTERISK:
      RESULT = str_of('*');
      break;
    case TKSLASH:
      RESULT = str_of('/');
      break;
    case TKDIV:
      RESULT = str_of('/');
      break;
    case TKMOD:
      RESULT = str_of('%');
      break;
    default:
      break;
  }
  return RESULT;
}

int _ISLOGICALORBITWISEOP(TLXTOKENID OP) {
  int RESULT;
  RESULT = OP == TKAND || OP == TKOR;
  return RESULT;
}

PString _GETLOGICALOP(TLXTOKENID OP) {
  PString RESULT;
  if (OP == TKAND) RESULT = str_make(2, "&&");
  else RESULT = str_make(2, "||");
  return RESULT;
}

PString _GETBITWISEOP(TLXTOKENID OP) {
  PString RESULT;
  if (OP == TKAND) RESULT = str_of('&');
  else RESULT = str_of('|');
  return RESULT;
}

int _ISRELATIONALOP(TLXTOKENID OP) {
  int RESULT;
  RESULT = OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS;
  return RESULT;
}

PString _GETRELATIONALOP(TLXTOKENID OP) {
  PString RESULT;
  switch (OP) {
    case TKEQUALS:
      RESULT = str_make(2, "==");
      break;
    case TKNOTEQUALS:
      RESULT = str_make(2, "!=");
      break;
    case TKLESSTHAN:
      RESULT = str_of('<');
      break;
    case TKMORETHAN:
      RESULT = str_of('>');
      break;
    case TKLESSOREQUALS:
      RESULT = str_make(2, "<=");
      break;
    case TKMOREOREQUALS:
      RESULT = str_make(2, ">=");
      break;
    default:
      break;
  }
  return RESULT;
}

void _OUTEXBINARYOP(TEXPRESSIONOBJ *EXPR) {
  char LTYPE;
  char RTYPE;
  {
    TEXBINARYOP *with53 = &EXPR->BINARYEX;
    {
      if (ISSTRINGYTYPE((*with53).LEFT->TYPEINDEX)) {
        if (ISCHARTYPE((*with53).LEFT->TYPEINDEX)) LTYPE = 'c';
        else LTYPE = 's';
        if (ISCHARTYPE((*with53).RIGHT->TYPEINDEX)) RTYPE = 'c';
        else RTYPE = 's';
        if ((*with53).OP == TKPLUS) {
          write_s(&CODEGEN.OUTPUT, str_make(4, "cat_"));
          write_c(&CODEGEN.OUTPUT, LTYPE);
          write_c(&CODEGEN.OUTPUT, RTYPE);
          write_c(&CODEGEN.OUTPUT, '(');
          OUTEXPRESSION((*with53).LEFT);
          write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          OUTEXPRESSION((*with53).RIGHT);
          write_c(&CODEGEN.OUTPUT, ')');
        }
        else if (ISCHARTYPE((*with53).LEFT->TYPEINDEX) && ISCHARTYPE((*with53).RIGHT->TYPEINDEX)) {
          _OUTEXPRESSIONPARENS((*with53).LEFT, EXPR);
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with53).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
          _OUTEXPRESSIONPARENSEXTRA((*with53).RIGHT, EXPR);
        }
        else {
          write_s(&CODEGEN.OUTPUT, str_make(4, "cmp_"));
          write_c(&CODEGEN.OUTPUT, LTYPE);
          write_c(&CODEGEN.OUTPUT, RTYPE);
          write_c(&CODEGEN.OUTPUT, '(');
          OUTEXPRESSION((*with53).LEFT);
          write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          OUTEXPRESSION((*with53).RIGHT);
          write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with53).OP));
          write_s(&CODEGEN.OUTPUT, str_make(2, " 0"));
        }
      }
      else if (ISBOOLEANTYPE((*with53).LEFT->TYPEINDEX)) {
        _OUTEXPRESSIONPARENS((*with53).LEFT, EXPR);
        if (_ISLOGICALORBITWISEOP((*with53).OP)) {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETLOGICALOP((*with53).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        else {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with53).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        _OUTEXPRESSIONPARENSEXTRA((*with53).RIGHT, EXPR);
      }
      else if (ISINTEGERTYPE((*with53).LEFT->TYPEINDEX)) {
        _OUTEXPRESSIONPARENS((*with53).LEFT, EXPR);
        if (_ISARITHMETICOP((*with53).OP)) {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETARITHMETICOP((*with53).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        else if (_ISLOGICALORBITWISEOP((*with53).OP)) {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETBITWISEOP((*with53).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        else {
          write_c(&CODEGEN.OUTPUT, ' ');
          write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with53).OP));
          write_c(&CODEGEN.OUTPUT, ' ');
        }
        _OUTEXPRESSIONPARENSEXTRA((*with53).RIGHT, EXPR);
      }
      else {
        _OUTEXPRESSIONPARENS((*with53).LEFT, EXPR);
        write_c(&CODEGEN.OUTPUT, ' ');
        write_s(&CODEGEN.OUTPUT, _GETRELATIONALOP((*with53).OP));
        write_c(&CODEGEN.OUTPUT, ' ');
        _OUTEXPRESSIONPARENSEXTRA((*with53).RIGHT, EXPR);
      }
    }
  }
}

void OUTEXPRESSION(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *TMPEXPR;
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
    case XCPSEUDOFUNCALL:
      _OUTEXPSEUDOFUNCALL(EXPR);
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

void OUTENUMVALUES(TPSENUMDEF *ENUMINDEX) {
  int POSINENUM;
  _OUTBLANKLINE(TOTENUMVAL);
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(22, "const char* enumvalues"));
  write_i(&CODEGEN.OUTPUT, ENUMINDEX->ID);
  write_s(&CODEGEN.OUTPUT, str_make(7, "[] = { "));
  do {
    int first = 0;
    int last = ENUMINDEX->SIZE - 1;
    if (first <= last) {
      POSINENUM = first;
      while (1) {
        {
          if (POSINENUM != 0) write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          write_c(&CODEGEN.OUTPUT, '"');
          write_s(&CODEGEN.OUTPUT, ENUMINDEX->VALUES[POSINENUM - 0]);
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

void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT) {
  TPSDEFENTRY *DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != (void*)0) {
    if (DEF->CLS == TDCENUM) OUTENUMVALUES(DEF->ENUMINDEX);
    DEF = DEF->NEXT;
  }
}

PString OUTVARIABLENAME(PString NAME, int ISREFERENCE) {
  PString RESULT;
  if (ISREFERENCE) RESULT = cat_cs('*', NAME);
  else RESULT = NAME;
  return RESULT;
}

void OUTTYPEREFERENCE(TPSTYPE *TYPEINDEX) {
  if (TYPEINDEX == (void*)0) write_s(&CODEGEN.OUTPUT, str_make(4, "void"));
  else if (TYPEINDEX->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEINDEX->POINTEDTYPEINDEX);
    write_c(&CODEGEN.OUTPUT, '*');
  }
  else if (TYPEINDEX->CLS == TTCBOOLEAN) write_s(&CODEGEN.OUTPUT, str_make(3, "int"));
  else if (TYPEINDEX->CLS == TTCINTEGER) write_s(&CODEGEN.OUTPUT, str_make(3, "int"));
  else if (TYPEINDEX->CLS == TTCCHAR) write_s(&CODEGEN.OUTPUT, str_make(4, "char"));
  else if (TYPEINDEX->CLS == TTCSTRING) write_s(&CODEGEN.OUTPUT, str_make(7, "PString"));
  else if (TYPEINDEX->CLS == TTCTEXT) write_s(&CODEGEN.OUTPUT, str_make(5, "PFile"));
  else if (TYPEINDEX->CLS == TTCENUM) {
    if (TYPEINDEX->ENUMINDEX->HASBEENDEFINED && cmp_ss(TYPEINDEX->NAME, str_make(0, "")) != 0) write_s(&CODEGEN.OUTPUT, TYPEINDEX->NAME);
    else {
      write_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
      write_i(&CODEGEN.OUTPUT, TYPEINDEX->ENUMINDEX->ID);
    }
  }
  else if (TYPEINDEX->CLS == TTCRECORD) {
    if (TYPEINDEX->RECORDINDEX->HASBEENDEFINED && cmp_ss(TYPEINDEX->NAME, str_make(0, "")) != 0) write_s(&CODEGEN.OUTPUT, TYPEINDEX->NAME);
    else {
      write_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
      write_i(&CODEGEN.OUTPUT, TYPEINDEX->RECORDINDEX->ID);
    }
  }
  else if (TYPEINDEX->CLS == TTCARRAY) {
    OUTTYPEREFERENCE(TYPEINDEX->ARRAYINDEX->TYPEINDEX);
    write_c(&CODEGEN.OUTPUT, '*');
  }
  else COMPILEERROR(cat_ss(str_make(30, "Error writing type reference: "), TYPENAME(TYPEINDEX)));
}

void OUTNAMEANDRECORD(PString NAME, TPSRECORDDEF *RECORDINDEX) {
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

void OUTNAMEANDENUM(PString NAME, TPSENUMDEF *ENUMINDEX) {
  int POS;
  write_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
  write_i(&CODEGEN.OUTPUT, ENUMINDEX->ID);
  if (!ENUMINDEX->HASBEENDEFINED) {
    write_s(&CODEGEN.OUTPUT, str_make(3, " { "));
    do {
      int first = 0;
      int last = ENUMINDEX->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS > 0) write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
            write_s(&CODEGEN.OUTPUT, ENUMINDEX->VALUES[POS - 0]);
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

void OUTNAMEANDTYPE(PString NAME, TPSTYPE *TYPEINDEX) {
  TPSARRAYDEF ARR;
  TEXPRESSIONOBJ *SIZEEXPR;
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
    write_s(&CODEGEN.OUTPUT, str_make(4, "int "));
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

void OUTTYPEDEFINITION(TPSTYPE *TYPEINDEX) {
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

void OUTTYPEDEFINITIONSFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT) {
  TPSDEFENTRY *DEF;
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

void OUTVARIABLEDEFINITION(TPSVARIABLE *VARINDEX) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  if (VARINDEX->ISCONSTANT) write_s(&CODEGEN.OUTPUT, str_make(6, "const "));
  OUTVARIABLEDECLARATION(*VARINDEX);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE *VARINDEX) {
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

void OUTFUNCTIONDECLARATION(TPSFUNCTION *FNINDEX) {
  _OUTBLANKLINE(TOTFUNDEC);
  OUTFUNCTIONPROTOTYPE(*FNINDEX);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONDEFINITION(TPSFUNCTION *FNINDEX) {
  _OUTBLANKLINE(TOTFUNDEF);
  OUTFUNCTIONPROTOTYPE(*FNINDEX);
  write_c(&CODEGEN.OUTPUT, ' ');
  OUTBEGIN();
}

void OUTFUNCTIONEND(TPSFUNCTION *FNINDEX) {
  if (FNINDEX->RETURNTYPEINDEX != (void*)0) {
    _OUTINDENT();
    write_s(&CODEGEN.OUTPUT, str_make(14, "return RESULT;"));
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

char SHORTTYPENAME(TPSTYPE *TYPEINDEX) {
  char RESULT;
  if (ISBOOLEANTYPE(TYPEINDEX)) RESULT = 'b';
  else if (ISINTEGERTYPE(TYPEINDEX)) RESULT = 'i';
  else if (ISCHARTYPE(TYPEINDEX)) RESULT = 'c';
  else if (ISSTRINGTYPE(TYPEINDEX)) RESULT = 's';
  else if (ISENUMTYPE(TYPEINDEX)) RESULT = 'e';
  else COMPILEERROR(cat_ss(str_make(30, "No short type name exists for "), TYPENAME(TYPEINDEX)));
  return RESULT;
}

void _OUTREAD(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *SRC;
  TEXREADARGS *READARG;
  int LINEFEED;
  int BRACES;
  SRC = EXPR->PSEUDOFUNCALLEX.ARG1;
  LINEFEED = EXPR->PSEUDOFUNCALLEX.PSEUDOFUN == TPFREADLN;
  READARG = EXPR->PSEUDOFUNCALLEX.READARGS;
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

void _OUTWRITE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *DST;
  TEXWRITEARGS *WRITEARG;
  int LINEFEED;
  int BRACES;
  DST = EXPR->PSEUDOFUNCALLEX.ARG1;
  LINEFEED = EXPR->PSEUDOFUNCALLEX.PSEUDOFUN == TPFWRITELN;
  WRITEARG = EXPR->PSEUDOFUNCALLEX.WRITEARGS;
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

void _OUTSTR(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *SRC;
  TEXPRESSIONOBJ *DST;
  SRC = EXPR->PSEUDOFUNCALLEX.ARG1;
  DST = EXPR->PSEUDOFUNCALLEX.ARG2;
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

void _OUTNEW(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *PTR;
  PTR = EXPR->PSEUDOFUNCALLEX.ARG1;
  _OUTINDENT();
  OUTEXPRESSION(PTR);
  write_s(&CODEGEN.OUTPUT, str_make(17, " = malloc(sizeof("));
  OUTTYPEREFERENCE(PTR->TYPEINDEX->POINTEDTYPEINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(3, "));"));
  _OUTNEWLINE();
}

void _OUTDISPOSE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *PTR;
  PTR = EXPR->PSEUDOFUNCALLEX.ARG1;
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(5, "free("));
  OUTEXPRESSION(PTR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
  _OUTNEWLINE();
}

void _OUTORD(TEXPRESSIONOBJ *EXPR) {
  if (ISORDINALTYPE(EXPR->PSEUDOFUNCALLEX.ARG1->TYPEINDEX)) {
    write_s(&CODEGEN.OUTPUT, str_make(5, "(int)"));
    _OUTEXPRESSIONPARENSPREC(EXPR->PSEUDOFUNCALLEX.ARG1, 2);
  }
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFUNCALLEX.ARG1->TYPEINDEX)));
}

void _OUTBOUNDS(TEXPRESSIONOBJ *EXPR) {
  if (ISBOOLEANTYPE(EXPR->TYPEINDEX)) write_s(&CODEGEN.OUTPUT, str_make(4, "0, 1"));
  else if (ISINTEGERTYPE(EXPR->TYPEINDEX)) write_s(&CODEGEN.OUTPUT, str_make(16, "INT_MIN, INT_MAX"));
  else if (ISCHARTYPE(EXPR->TYPEINDEX)) write_s(&CODEGEN.OUTPUT, str_make(6, "0, 255"));
  else if (ISENUMTYPE(EXPR->TYPEINDEX)) {
    write_s(&CODEGEN.OUTPUT, str_make(3, "0, "));
    write_i(&CODEGEN.OUTPUT, EXPR->TYPEINDEX->ENUMINDEX->SIZE - 1);
  }
  else COMPILEERROR(cat_ss(str_make(40, "Internal error: unknown bounds for type "), TYPENAME(EXPR->TYPEINDEX)));
}

void _OUTPRED(TEXPRESSIONOBJ *EXPR) {
  if (ISORDINALTYPE(EXPR->PSEUDOFUNCALLEX.ARG1->TYPEINDEX)) {
    write_s(&CODEGEN.OUTPUT, str_make(5, "pred("));
    OUTEXPRESSION(EXPR->PSEUDOFUNCALLEX.ARG1);
    write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTBOUNDS(EXPR->PSEUDOFUNCALLEX.ARG1);
    write_c(&CODEGEN.OUTPUT, ')');
  }
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFUNCALLEX.ARG1->TYPEINDEX)));
}

void _OUTSUCC(TEXPRESSIONOBJ *EXPR) {
  if (ISORDINALTYPE(EXPR->PSEUDOFUNCALLEX.ARG1->TYPEINDEX)) {
    write_s(&CODEGEN.OUTPUT, str_make(5, "succ("));
    OUTEXPRESSION(EXPR->PSEUDOFUNCALLEX.ARG1);
    write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTBOUNDS(EXPR->PSEUDOFUNCALLEX.ARG1);
    write_c(&CODEGEN.OUTPUT, ')');
  }
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFUNCALLEX.ARG1->TYPEINDEX)));
}

void OUTASSIGN(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  OUTEXPRESSION(LHS);
  write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(RHS);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTASSIGNRETURNVALUE(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(9, "RESULT = "));
  OUTEXPRESSION(RHS);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTASSIGNTOREFERENCE(TPSVARIABLE *VARINDEX, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(4, " = &"));
  _OUTEXPRESSIONPARENSPREC(RHS, 2);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTIF(TEXPRESSIONOBJ *EXPR) {
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

void OUTCASEBEGIN(TEXPRESSIONOBJ *CASEINDEX) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(8, "switch ("));
  OUTEXPRESSION(CASEINDEX);
  write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  OUTBEGIN();
}

void OUTCASESTATEMENTBEGIN(TEXPRESSIONOBJ *CASELABEL) {
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

void OUTREPEATEND(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *TMPEXPR;
  OUTENDSAMELINE();
  write_s(&CODEGEN.OUTPUT, str_make(8, " while ("));
  TMPEXPR = EXUNARYOP(COPYEXPR(EXPR), TKNOT);
  OUTEXPRESSION(TMPEXPR);
  DISPOSEEXPR(&TMPEXPR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
  _OUTNEWLINE();
}

void OUTWHILEBEGIN(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  write_s(&CODEGEN.OUTPUT, str_make(7, "while ("));
  OUTEXPRESSION(EXPR);
  write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTWHILEEND() {
  CODEGEN.ISMULTISTATEMENT = 1;
}

void OUTFORBEGIN(TEXPRESSIONOBJ *ITER, TEXPRESSIONOBJ *FIRSTEXPR, TEXPRESSIONOBJ *LASTEXPR, int ASCENDING) {
  TPSTYPE *LIMITTYPE;
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

void OUTFOREND(TEXPRESSIONOBJ *ITER, int ASCENDING) {
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

void OUTPROCEDURECALL(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  OUTEXPRESSION(EXPR);
  write_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTPSEUDOPROCCALL(TEXPRESSIONOBJ *EXPR) {
  _OUTEXPSEUDOFUNCALL(EXPR);
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
  PString RESULT;
  int BASELEN;
  int POS;
  int MATCHES;
  RESULT = str_make(0, "");
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
    if (MATCHES) RESULT = cat_ss(COPY(STR, 1, BASELEN), NEW);
  }
  return RESULT;
}

void PARSECMDLINE() {
  int POS;
  PString INPUTFILE;
  PString OUTPUTFILE;
  enum enum54 { FLAGNONE, FLAGOUTPUT } FLAG;
  PString PARAM;
  const char* enumvalues54[] = { "FLAGNONE", "FLAGOUTPUT" };
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
