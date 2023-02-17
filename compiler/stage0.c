/* Program: PASCUAL */
#include "pascual.h"

struct record1 {
  int SUPPRESSWARNINGS;
} OPTIONS;

void COMPILEERROR(PString MSG);
void COMPILEWARNING(PString MSG);

typedef enum enum1 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKINTEGER, TKREAL, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAND, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH } TLXTOKENID;
typedef struct record2 {
  int ROW;
  int COL;
} TLXPOS;
typedef struct record3 {
  TLXTOKENID ID;
  PString VALUE;
  TLXPOS POS;
} TLXTOKEN;
typedef enum enum2 { TPFCONCAT, TPFDISPOSE, TPFNEW, TPFORD, TPFPRED, TPFREAD, TPFREADLN, TPFSTR, TPFSUCC, TPFVAL, TPFWRITE, TPFWRITELN } TPSPSEUDOFN;
typedef struct record13 *TPSTYPEPTR;
typedef struct record14 *TPSENUMPTR;
typedef struct record20 *TPSVARPTR;
typedef struct record21 *TPSFNPTR;
typedef struct record11 *TEXPRESSION;
typedef enum enum3 { XICNIL, XICBOOLEAN, XICINTEGER, XICREAL, XICCHAR, XICSTRING, XICENUM } TEXIMMEDIATECLASS;
typedef struct record4 {
  TEXIMMEDIATECLASS CLS;
  union {
    struct {
      int BOOLEANVAL;
    };
    struct {
      int INTEGERVAL;
    };
    struct {
      double REALVAL;
    };
    struct {
      unsigned char CHARVAL;
    };
    struct {
      PString STRINGVAL;
    };
    struct {
      int ENUMORDINAL;
      struct record14 *ENUMPTR;
    };
  };
} TEXIMMEDIATE;
typedef struct record5 {
  int SIZE;
  struct record11 *VALUES[8];
} TEXFUNCTIONARGS;
typedef struct record6 {
  struct record11 *ARG;
  struct record6 *NEXT;
} TEXREADARGS;
typedef struct record7 {
  struct record11 *ARG;
  struct record7 *NEXT;
} TEXWRITEARGS;
typedef struct record8 {
  struct record11 *ARG1;
  struct record11 *ARG2;
  struct record11 *ARG3;
  TPSPSEUDOFN PSEUDOFN;
  union {
    struct {
      TEXREADARGS *READARGS;
    };
    struct {
      TEXWRITEARGS *WRITEARGS;
    };
  };
} TEXPSEUDOFNCALL;
typedef struct record9 {
  struct record11 *PARENT;
  TLXTOKENID OP;
} TEXUNARYOP;
typedef struct record10 {
  struct record11 *LEFT;
  struct record11 *RIGHT;
  TLXTOKENID OP;
} TEXBINARYOP;
typedef enum enum4 { XCIMMEDIATE, XCTOSTRING, XCTOREAL, XCSETTMPVAR, XCSUBRANGE, XCVARIABLE, XCFIELD, XCARRAY, XCPOINTER, XCSTRINGCHAR, XCFNREF, XCFNCALL, XCPSEUDOFNREF, XCPSEUDOFNCALL, XCUNARYOP, XCBINARYOP } TEXPRESSIONCLASS;
typedef struct record11 {
  struct record13 *TYPEPTR;
  int ISASSIGNABLE;
  int ISFUNCTIONRESULT;
  TEXPRESSIONCLASS CLS;
  union {
    struct {
      TEXIMMEDIATE IMMEDIATE;
    };
    struct {
      struct record11 *TOSTRPARENT;
    };
    struct {
      struct record11 *TOREALPARENT;
    };
    struct {
      struct record11 *SUBRANGEPARENT;
    };
    struct {
      struct record20 *VARPTR;
    };
    struct {
      struct record11 *RECEXPR;
      int RECFIELDNUM;
    };
    struct {
      struct record11 *ARRAYEXPR;
      struct record11 *ARRAYINDEX;
    };
    struct {
      struct record11 *POINTEREXPR;
    };
    struct {
      struct record11 *STRINGEXPR;
      struct record11 *STRINGINDEX;
    };
    struct {
      struct record21 *FNPTR;
    };
    struct {
      struct record11 *FNEXPR;
      TEXFUNCTIONARGS CALLARGS;
    };
    struct {
      TPSPSEUDOFN PSEUDOFN;
    };
    struct {
      TEXPSEUDOFNCALL PSEUDOFNCALL;
    };
    struct {
      TEXUNARYOP UNARY;
    };
    struct {
      TEXBINARYOP BINARY;
    };
    struct {
      struct record11 *TMPVAR;
      struct record11 *TMPVARVALUE;
      struct record11 *TMPVARCHILD;
    };
  };
} TEXPRESSIONOBJ;
typedef struct record12 {
  PString NAME;
} TPSIDENTIFIER;
typedef struct record15 *TPSRANGEPTR;
typedef struct record17 *TPSRECPTR;
typedef struct record18 *TPSARRAYPTR;
typedef struct record19 *TPSCONSTPTR;
typedef struct record22 *TPSWITHVARPTR;
typedef struct record23 *TPSNAMEPTR;
typedef enum enum5 { TTCBOOLEAN, TTCINTEGER, TTCREAL, TTCCHAR, TTCSTRING, TTCTEXT, TTCENUM, TTCRANGE, TTCRECORD, TTCARRAY, TTCPOINTER, TTCNIL, TTCPOINTERUNKNOWN } TPSTYPECLASS;
typedef struct record13 {
  PString NAME;
  struct record13 *ALIASFOR;
  int WASUSED;
  TPSTYPECLASS CLS;
  union {
    struct {
      struct record14 *ENUMPTR;
    };
    struct {
      struct record15 *RANGEPTR;
    };
    struct {
      struct record17 *RECPTR;
    };
    struct {
      struct record18 *ARRAYPTR;
    };
    struct {
      struct record13 *POINTEDTYPEPTR;
    };
    struct {
      PString *TARGETNAME;
    };
  };
} TPSTYPE;
typedef struct record14 {
  int SIZE;
  PString VALUES[128];
  int ID;
  int HASBEENDEFINED;
} TPSENUMDEF;
typedef struct record15 {
  int FIRST;
  int LAST;
  TPSTYPE *BASETYPEPTR;
} TPSRANGEDEF;
typedef struct record16 {
  PString NAME;
  TPSTYPE *TYPEPTR;
} TPSRECORDFIELD;
typedef struct record17 {
  int SIZE;
  TPSRECORDFIELD FIELDS[32];
  int NUMVARIANTS;
  int VARIANTBOUNDS[32];
  int ID;
  int HASBEENDEFINED;
} TPSRECORDDEF;
typedef struct record18 {
  TPSTYPE *INDEXTYPEPTR;
  TPSTYPE *VALUETYPEPTR;
} TPSARRAYDEF;
typedef struct record19 {
  PString NAME;
  TEXPRESSIONOBJ *VALUE;
} TPSCONSTANT;
typedef struct record20 {
  PString NAME;
  TPSTYPE *TYPEPTR;
  int ISREFERENCE;
  int ISCONSTANT;
  int WASINITIALIZED;
  int WASUSED;
} TPSVARIABLE;
typedef struct record21 {
  PString NAME;
  int ARGCOUNT;
  TPSVARIABLE ARGS[8];
  TPSTYPE *RETURNTYPEPTR;
  int ISDECLARATION;
  int WASUSED;
} TPSFUNCTION;
typedef struct record22 {
  TPSVARIABLE *VARPTR;
} TPSWITHVAR;
typedef enum enum6 { TNCTYPE, TNCVARIABLE, TNCCONSTANT, TNCENUMVAL, TNCFUNCTION, TNCPSEUDOFN } TPSNAMECLASS;
typedef struct record23 {
  PString NAME;
  TPSNAMECLASS CLS;
  union {
    struct {
      TPSTYPE *TYPEPTR;
    };
    struct {
      TPSVARIABLE *VARPTR;
    };
    struct {
      TPSCONSTANT *CONSTPTR;
    };
    struct {
      TPSTYPE *ENUMTYPEPTR;
      int ORDINAL;
    };
    struct {
      TPSFUNCTION *FNPTR;
    };
    struct {
      TPSPSEUDOFN PSEUDOFN;
    };
  };
} TPSNAME;
typedef enum enum7 { TCTENUM, TCTRECORD, TCTTMPVAR } TPSCOUNTERTYPE;
typedef struct record24 {
  int ENUMCTR;
  int RECORDCTR;
  int TMPVARCTR;
} TPSCOUNTERS;
typedef struct record25 *TPSDEFPTR;
typedef enum enum8 { TDCNAME, TDCTYPE, TDCENUM, TDCRANGE, TDCRECORD, TDCARRAY, TDCCONSTANT, TDCVARIABLE, TDCFUNCTION, TDCWITHVAR, TDCSCOPEBOUNDARY } TPSDEFCLASS;
typedef struct record25 {
  struct record25 *PREV;
  struct record25 *NEXT;
  TPSDEFCLASS CLS;
  union {
    struct {
      TPSNAME *NAMEPTR;
    };
    struct {
      TPSTYPE *TYPEPTR;
    };
    struct {
      TPSENUMDEF *ENUMPTR;
    };
    struct {
      TPSRANGEDEF *RANGEPTR;
    };
    struct {
      TPSRECORDDEF *RECPTR;
    };
    struct {
      TPSARRAYDEF *ARRAYPTR;
    };
    struct {
      TPSCONSTANT *CONSTPTR;
    };
    struct {
      TPSVARIABLE *VARPTR;
    };
    struct {
      TPSFUNCTION *FNPTR;
    };
    struct {
      TPSWITHVAR *WITHVARPTR;
    };
    struct {
      int TEMPORARYSCOPE;
      TPSCOUNTERS COUNTERS;
      TPSFUNCTION *CURRENTFN;
    };
  };
} TPSDEFENTRY;
typedef struct record26 {
  TPSDEFENTRY *LATEST;
  TPSFUNCTION *CURRENTFN;
  TPSCOUNTERS COUNTERS;
} TPSDEFS;

const char* enumvalues1[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKINTEGER", "TKREAL", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAND", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH" };
const char* enumvalues2[] = { "TPFCONCAT", "TPFDISPOSE", "TPFNEW", "TPFORD", "TPFPRED", "TPFREAD", "TPFREADLN", "TPFSTR", "TPFSUCC", "TPFVAL", "TPFWRITE", "TPFWRITELN" };
const char* enumvalues3[] = { "XICNIL", "XICBOOLEAN", "XICINTEGER", "XICREAL", "XICCHAR", "XICSTRING", "XICENUM" };
const char* enumvalues4[] = { "XCIMMEDIATE", "XCTOSTRING", "XCTOREAL", "XCSETTMPVAR", "XCSUBRANGE", "XCVARIABLE", "XCFIELD", "XCARRAY", "XCPOINTER", "XCSTRINGCHAR", "XCFNREF", "XCFNCALL", "XCPSEUDOFNREF", "XCPSEUDOFNCALL", "XCUNARYOP", "XCBINARYOP" };
const char* enumvalues5[] = { "TTCBOOLEAN", "TTCINTEGER", "TTCREAL", "TTCCHAR", "TTCSTRING", "TTCTEXT", "TTCENUM", "TTCRANGE", "TTCRECORD", "TTCARRAY", "TTCPOINTER", "TTCNIL", "TTCPOINTERUNKNOWN" };
const char* enumvalues6[] = { "TNCTYPE", "TNCVARIABLE", "TNCCONSTANT", "TNCENUMVAL", "TNCFUNCTION", "TNCPSEUDOFN" };
const char* enumvalues7[] = { "TCTENUM", "TCTRECORD", "TCTTMPVAR" };
const char* enumvalues8[] = { "TDCNAME", "TDCTYPE", "TDCENUM", "TDCRANGE", "TDCRECORD", "TDCARRAY", "TDCCONSTANT", "TDCVARIABLE", "TDCFUNCTION", "TDCWITHVAR", "TDCSCOPEBOUNDARY" };

void DISPOSEEXPR(TEXPRESSIONOBJ **EXPR);
TEXPRESSIONOBJ *COPYEXPR(TEXPRESSIONOBJ *EXPR);
PString DESCRIBEEXPR(TEXPRESSIONOBJ *EXPR, int LEVELS);
void EXMARKINITIALIZED(TEXPRESSIONOBJ *LHS);
TEXPRESSIONOBJ *PF_PARSE(TEXPRESSIONOBJ *FN);
TEXPRESSIONOBJ *PFORD(TEXPRESSIONOBJ *ARG);
TEXPRESSIONOBJ *PFPRED(TEXPRESSIONOBJ *ARG);
TEXPRESSIONOBJ *PFSUCC(TEXPRESSIONOBJ *ARG);

typedef struct record27 {
  PFile SRC;
  PString NAME;
  TLXPOS POS;
} TLXINPUTFILE;

struct record29 {
  PString LINE;
  TLXTOKEN TOKEN;
  TLXINPUTFILE INPUT;
  struct record28 {
    int EXISTS;
    TLXINPUTFILE INPUT;
  } PREV;
} LEXER;

PString LXTOKENNAME(TLXTOKENID ID) {
  PString RESULT;
  PString NAME;
  STR_e(ID, enumvalues1, &NAME);
  RESULT = NAME;
  return RESULT;
}

PString LXPOSSTR(TLXPOS POS) {
  PString RESULT;
  PString ROW;
  PString COL;
  STR_i(POS.ROW, &ROW);
  STR_i(POS.COL, &COL);
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

int LXISALPHA(unsigned char CHR) {
  int RESULT;
  RESULT = CHR >= 'a' && CHR <= 'z' || CHR >= 'A' && CHR <= 'Z';
  return RESULT;
}

int LXISDIGIT(unsigned char CHR) {
  int RESULT;
  RESULT = CHR >= '0' && CHR <= '9';
  return RESULT;
}

int LXISHEXDIGIT(unsigned char CHR) {
  int RESULT;
  RESULT = LXISDIGIT(CHR) || CHR >= 'a' && CHR <= 'f' || CHR >= 'A' && CHR <= 'F';
  return RESULT;
}

int LXISALPHANUM(unsigned char CHR) {
  int RESULT;
  RESULT = LXISALPHA(CHR) || LXISDIGIT(CHR);
  return RESULT;
}

int LXISIDENTIFIERFIRST(unsigned char CHR) {
  int RESULT;
  RESULT = LXISALPHA(CHR) || CHR == '_';
  return RESULT;
}

int LXISIDENTIFIERCHAR(unsigned char CHR) {
  int RESULT;
  RESULT = LXISALPHANUM(CHR) || CHR == '_';
  return RESULT;
}

int LXISTOKENWAITING() {
  int RESULT;
  do {
    while (LENGTH(&LEXER.LINE) == 0 && !EOF(&LEXER.INPUT.SRC)) {
      LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
      LEXER.INPUT.POS.COL = 1;
      READ_s(&LEXER.INPUT.SRC, &LEXER.LINE);
      READLN(&LEXER.INPUT.SRC);
    }
    while (LENGTH(&LEXER.LINE) > 0 && LEXER.LINE.chr[1] == ' ') {
      LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + 1;
      DELETE(&LEXER.LINE, 1, 1);
    }
  } while (!(EOF(&LEXER.INPUT.SRC) || LENGTH(&LEXER.LINE) > 0));
  RESULT = LENGTH(&LEXER.LINE) > 0;
  return RESULT;
}

void LXGETSYMBOL(TLXTOKENID ID, int LENGTH) {
  LEXER.TOKEN.ID = ID;
  LEXER.TOKEN.VALUE = COPY(&LEXER.LINE, 1, LENGTH);
  LEXER.TOKEN.POS = LEXER.INPUT.POS;
  DELETE(&LEXER.LINE, 1, LENGTH);
  LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + LENGTH;
}

void LXGETIDENTIFIER() {
  unsigned char CHR;
  int POS;
  int INTOKEN;
  POS = 0;
  INTOKEN = 1;
  while (POS < LENGTH(&LEXER.LINE) && INTOKEN) {
    CHR = LEXER.LINE.chr[POS + 1];
    INTOKEN = LXISIDENTIFIERCHAR(CHR);
    if (INTOKEN) POS = POS + 1;
  }
  LXGETSYMBOL(TKIDENTIFIER, POS);
  do {
    int first = 1;
    int last = LENGTH(&LEXER.TOKEN.VALUE);
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
  int ASINT;
  double ASREAL;
  int ASINTCODE;
  int ASREALCODE;
  VAL_i(&LEXER.LINE, &ASINT, &ASINTCODE);
  VAL_r(&LEXER.LINE, &ASREAL, &ASREALCODE);
  if (ASINTCODE == 0) ASINTCODE = LENGTH(&LEXER.LINE) + 1;
  if (ASREALCODE == 0) ASREALCODE = LENGTH(&LEXER.LINE) + 1;
  if (ASREALCODE > ASINTCODE) LXGETSYMBOL(TKREAL, ASREALCODE - 1);
  else LXGETSYMBOL(TKINTEGER, ASINTCODE - 1);
}

void LXGETSTRING() {
  unsigned char CHR;
  int POS;
  int LAST;
  enum enum9 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, CARET, DONE } STATE;
  const char* enumvalues9[] = { "NONE", "QUOTEDSTR", "HASH", "NUMCHARDEC", "NUMCHARHEX", "CARET", "DONE" };
  POS = 0;
  STATE = NONE;
  do {
    POS = POS + 1;
    CHR = LEXER.LINE.chr[POS];
    if (STATE == NONE) {
      if (CHR == '\'') STATE = QUOTEDSTR;
      else if (CHR == '#') STATE = HASH;
      else if (CHR == '^') STATE = CARET;
      else STATE = DONE;
    }
    else if (STATE == HASH) {
      if (CHR == '$') STATE = NUMCHARHEX;
      else if (LXISDIGIT(CHR)) {
        STATE = NUMCHARDEC;
        LAST = POS;
      }
      else STATE = DONE;
    }
    else if (STATE == NUMCHARDEC) {
      if (LXISDIGIT(CHR)) LAST = POS;
      else if (CHR == '\'') STATE = QUOTEDSTR;
      else if (CHR == '#') STATE = HASH;
      else if (CHR == '^') STATE = CARET;
      else STATE = DONE;
    }
    else if (STATE == NUMCHARHEX) {
      if (LXISHEXDIGIT(CHR)) LAST = POS;
      else if (CHR == '\'') STATE = QUOTEDSTR;
      else if (CHR == '#') STATE = HASH;
      else if (CHR == '^') STATE = CARET;
      else STATE = DONE;
    }
    else if (STATE == CARET) {
      if (CHR >= '@' && CHR <= '_' || CHR >= 'a' && CHR <= 'z') {
        LAST = POS;
        STATE = NONE;
      }
      else STATE = DONE;
    }
    else if (STATE == QUOTEDSTR) {
      LAST = POS;
      if (CHR == '\'') STATE = NONE;
    }
  } while (!(STATE == DONE));
  LXGETSYMBOL(TKSTRING, LAST);
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
      READ_s(&LEXER.INPUT.SRC, &LEXER.LINE);
      READLN(&LEXER.INPUT.SRC);
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
  unsigned char CHR;
  PString PFX;
  LEXER.TOKEN.VALUE = str_make(0, "");
  LEXER.TOKEN.ID = TKUNKNOWN;
  if (!LXISTOKENWAITING()) LEXER.TOKEN.ID = TKEOF;
  else {
    CHR = LEXER.LINE.chr[1];
    if (LENGTH(&LEXER.LINE) >= 2) PFX = cat_cc(LEXER.LINE.chr[1], LEXER.LINE.chr[2]);
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
      case '#':
        LXGETSTRING();
        break;
      case '$':
        LXGETNUMBER();
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
  ASSIGN(&LEXER.INPUT.SRC, &LEXER.INPUT.NAME);
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

TPSDEFS DEFS;
struct record30 {
  TPSTYPE *PTNIL;
  TPSTYPE *PTBOOLEAN;
  TPSTYPE *PTINTEGER;
  TPSTYPE *PTREAL;
  TPSTYPE *PTCHAR;
  TPSTYPE *PTSTRING;
  TPSTYPE *PTTEXT;
} PRIMITIVETYPES;

int DEFCOUNTER(TPSCOUNTERTYPE COUNTERTYPE) {
  int RESULT;
  switch (COUNTERTYPE) {
    case TCTENUM:
      {
        DEFS.COUNTERS.ENUMCTR = DEFS.COUNTERS.ENUMCTR + 1;
        RESULT = DEFS.COUNTERS.ENUMCTR;
      }
      break;
    case TCTRECORD:
      {
        DEFS.COUNTERS.RECORDCTR = DEFS.COUNTERS.RECORDCTR + 1;
        RESULT = DEFS.COUNTERS.RECORDCTR;
      }
      break;
    case TCTTMPVAR:
      {
        DEFS.COUNTERS.TMPVARCTR = DEFS.COUNTERS.TMPVARCTR + 1;
        RESULT = DEFS.COUNTERS.TMPVARCTR;
      }
      break;
    default:
      break;
  }
  return RESULT;
}

void INITDEFS() {
  DEFS.LATEST = (void*)0;
  DEFS.COUNTERS.ENUMCTR = 0;
  DEFS.COUNTERS.RECORDCTR = 0;
  DEFS.COUNTERS.TMPVARCTR = 0;
  DEFS.CURRENTFN = (void*)0;
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
      DEF->NAMEPTR = malloc(sizeof(TPSNAME));
      break;
    case TDCTYPE:
      DEF->TYPEPTR = malloc(sizeof(TPSTYPE));
      break;
    case TDCENUM:
      DEF->ENUMPTR = malloc(sizeof(TPSENUMDEF));
      break;
    case TDCRANGE:
      DEF->RANGEPTR = malloc(sizeof(TPSRANGEDEF));
      break;
    case TDCRECORD:
      DEF->RECPTR = malloc(sizeof(TPSRECORDDEF));
      break;
    case TDCARRAY:
      DEF->ARRAYPTR = malloc(sizeof(TPSARRAYDEF));
      break;
    case TDCCONSTANT:
      DEF->CONSTPTR = malloc(sizeof(TPSCONSTANT));
      break;
    case TDCVARIABLE:
      DEF->VARPTR = malloc(sizeof(TPSVARIABLE));
      break;
    case TDCFUNCTION:
      DEF->FNPTR = malloc(sizeof(TPSFUNCTION));
      break;
    case TDCWITHVAR:
      DEF->WITHVARPTR = malloc(sizeof(TPSWITHVAR));
      break;
    case TDCSCOPEBOUNDARY:
      {
        DEF->TEMPORARYSCOPE = 0;
        DEF->CURRENTFN = (void*)0;
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
      free(DEF->NAMEPTR);
      break;
    case TDCTYPE:
      free(DEF->TYPEPTR);
      break;
    case TDCENUM:
      free(DEF->ENUMPTR);
      break;
    case TDCRANGE:
      free(DEF->RANGEPTR);
      break;
    case TDCRECORD:
      free(DEF->RECPTR);
      break;
    case TDCARRAY:
      free(DEF->ARRAYPTR);
      break;
    case TDCCONSTANT:
      free(DEF->CONSTPTR);
      break;
    case TDCVARIABLE:
      free(DEF->VARPTR);
      break;
    case TDCFUNCTION:
      free(DEF->FNPTR);
      break;
    case TDCWITHVAR:
      free(DEF->WITHVARPTR);
      break;
    default:
      break;
  }
  free(DEF);
}

PString TYPENAME(TPSTYPE *TYPEPTR);

void _CHECKUNUSEDSYMBOLS(TPSDEFENTRY *DEF) {
  PString WHERE;
  if (DEFS.CURRENTFN == (void*)0) WHERE = str_make(0, "");
  else if (DEFS.CURRENTFN->RETURNTYPEPTR == (void*)0) WHERE = cat_ss(str_make(14, " in procedure "), DEFS.CURRENTFN->NAME);
  else WHERE = cat_ss(str_make(13, " in function "), DEFS.CURRENTFN->NAME);
  switch (DEF->CLS) {
    case TDCVARIABLE:
      if (!DEF->VARPTR->WASUSED) {
        if (DEF->VARPTR->ISCONSTANT) COMPILEWARNING(cat_ss(cat_ss(cat_ss(str_make(9, "Constant "), DEF->VARPTR->NAME), str_make(13, " was not used")), WHERE));
        else COMPILEWARNING(cat_ss(cat_ss(cat_ss(str_make(9, "Variable "), DEF->VARPTR->NAME), str_make(13, " was not used")), WHERE));
      }
      else if (!DEF->VARPTR->WASINITIALIZED) COMPILEWARNING(cat_ss(cat_ss(cat_ss(str_make(9, "Variable "), DEF->VARPTR->NAME), str_make(20, " was not initialized")), WHERE));
      break;
    case TDCFUNCTION:
      if (!DEF->FNPTR->WASUSED) {
        if (DEF->FNPTR->RETURNTYPEPTR == (void*)0) COMPILEWARNING(cat_ss(cat_ss(str_make(10, "Procedure "), DEF->FNPTR->NAME), str_make(13, " was not used")));
        else COMPILEWARNING(cat_ss(cat_ss(str_make(9, "Function "), DEF->FNPTR->NAME), str_make(13, " was not used")));
      }
      break;
    case TDCTYPE:
      if (cmp_ss(DEF->TYPEPTR->NAME, str_make(0, "")) != 0 && !DEF->TYPEPTR->WASUSED) COMPILEWARNING(cat_ss(cat_ss(str_make(5, "Type "), TYPENAME(DEF->TYPEPTR)), str_make(13, " was not used")));
      break;
    default:
      break;
  }
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
    _CHECKUNUSEDSYMBOLS(DEFS.LATEST);
    *DELETEDDEF = *DEFS.LATEST;
    _DISPOSEDEF(DEFS.LATEST);
    DEFS.LATEST = DELETEDDEF->PREV;
    RESULT = 1;
  }
  return RESULT;
}

void _STARTSCOPE(int TEMPORARY, TPSFUNCTION *NEWFUNCTION) {
  TPSDEFENTRY *DEF;
  DEF = _ADDDEF(TDCSCOPEBOUNDARY);
  DEF->TEMPORARYSCOPE = TEMPORARY;
  DEF->COUNTERS = DEFS.COUNTERS;
  DEF->CURRENTFN = DEFS.CURRENTFN;
  if (!TEMPORARY) DEFS.CURRENTFN = NEWFUNCTION;
}

void _CLOSESCOPE(int TEMPORARY) {
  TPSDEFENTRY DELETEDDEF;
  int DELETED;
  do {
    DELETED = _DELETEDEF(&DELETEDDEF);
  } while (!(!DELETED || DELETEDDEF.CLS == TDCSCOPEBOUNDARY && (TEMPORARY || !DELETEDDEF.TEMPORARYSCOPE)));
  DEFS.CURRENTFN = DELETEDDEF.CURRENTFN;
  DEFS.COUNTERS = DELETEDDEF.COUNTERS;
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
    if (DEF->CLS == TDCNAME && cmp_ss(NAME, DEF->NAMEPTR->NAME) == 0) RET = DEF->NAMEPTR;
    DEF = DEF->PREV;
  }
  if (REQUIRED && RET == (void*)0) COMPILEERROR(cat_ss(str_make(20, "Unknown identifier: "), NAME));
  RESULT = RET;
  return RESULT;
}

TPSNAME *_CHECKNAMECLASS(TPSNAME *NAMEPTR, TPSNAMECLASS CLS) {
  TPSNAME *RESULT;
  if (NAMEPTR != (void*)0 && NAMEPTR->CLS != CLS) switch (CLS) {
    case TNCTYPE:
      COMPILEERROR(cat_ss(str_make(12, "Not a type: "), NAMEPTR->NAME));
      break;
    case TNCVARIABLE:
      COMPILEERROR(cat_ss(str_make(16, "Not a variable: "), NAMEPTR->NAME));
      break;
    case TNCENUMVAL:
      COMPILEERROR(cat_ss(str_make(26, "Not an enumeration value: "), NAMEPTR->NAME));
      break;
    case TNCFUNCTION:
      COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), NAMEPTR->NAME));
      break;
    case TNCPSEUDOFN:
      COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), NAMEPTR->NAME));
      break;
    default:
      COMPILEERROR(str_make(35, "Internal error: name class mismatch"));
      break;
  }
  RESULT = NAMEPTR;
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
  POS = _ADDDEF(TDCNAME)->NAMEPTR;
  POS->NAME = NAME;
  POS->CLS = CLS;
  RESULT = POS;
  return RESULT;
}

TPSNAME *ADDTYPENAME(PString NAME, TPSTYPE *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCTYPE);
  DEF->TYPEPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDVARIABLENAME(PString NAME, TPSVARIABLE *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCVARIABLE);
  DEF->VARPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDCONSTANTNAME(PString NAME, TPSCONSTANT *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCCONSTANT);
  DEF->CONSTPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDFUNCTIONNAME(PString NAME, TPSFUNCTION *IDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCFUNCTION);
  DEF->FNPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDENUMVALNAME(int ORDINAL, TPSTYPE *TYPEIDX) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(TYPEIDX->ENUMPTR->VALUES[subrange(ORDINAL, 0, 127)], TNCENUMVAL);
  DEF->ENUMTYPEPTR = TYPEIDX;
  DEF->ORDINAL = ORDINAL;
  RESULT = DEF;
  return RESULT;
}

TPSNAME *ADDPSEUDOFN(PString NAME, TPSPSEUDOFN FN) {
  TPSNAME *RESULT;
  TPSNAME *DEF;
  DEF = _ADDNAME(NAME, TNCPSEUDOFN);
  DEF->PSEUDOFN = FN;
  RESULT = DEF;
  return RESULT;
}

PString DEEPTYPENAME(TPSTYPE *TYPEPTR, int USEORIGINAL) {
  PString RESULT;
  TPSTYPE TYP;
  PString RET;
  PString NUMSTR;
  int POS;
  do {
    TYP = *TYPEPTR;
    TYPEPTR = TYP.ALIASFOR;
  } while (!(!USEORIGINAL || TYPEPTR == (void*)0));
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) RESULT = TYP.NAME;
  else if (TYP.CLS == TTCENUM) {
    RET = str_of('(');
    do {
      int first = 0;
      int last = TYP.ENUMPTR->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 0) RET = cat_sc(RET, ',');
            RET = cat_ss(RET, TYP.ENUMPTR->VALUES[subrange(POS, 0, 127)]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    RESULT = cat_sc(RET, ')');
  }
  else if (TYP.CLS == TTCRANGE) {
    STR_i(TYP.RANGEPTR->FIRST, &RESULT);
    STR_i(TYP.RANGEPTR->LAST, &NUMSTR);
    RESULT = cat_ss(cat_ss(RESULT, str_make(2, "..")), NUMSTR);
  }
  else if (TYP.CLS == TTCRECORD) {
    RET = str_make(7, "record ");
    do {
      int first = 1;
      int last = TYP.RECPTR->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 1) RET = cat_sc(RET, ',');
            RET = cat_ss(RET, DEEPTYPENAME(TYP.RECPTR->FIELDS[(int)subrange(POS, 1, 32) - 1].TYPEPTR, 1));
            RET = cat_ss(cat_sc(RET, ':'), TYP.RECPTR->FIELDS[(int)subrange(POS, 1, 32) - 1].NAME);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    RESULT = cat_ss(RET, str_make(4, " end"));
  }
  else if (TYP.CLS == TTCARRAY) {
    RET = cat_ss(cat_ss(cat_ss(str_make(7, "array ["), DEEPTYPENAME(TYP.ARRAYPTR->INDEXTYPEPTR, 0)), str_make(5, "] of ")), DEEPTYPENAME(TYP.ARRAYPTR->VALUETYPEPTR, 0));
    RESULT = RET;
  }
  else if (TYP.CLS == TTCPOINTER) RESULT = cat_cs('^', DEEPTYPENAME(TYP.POINTEDTYPEPTR, 1));
  else {
    STR_e(TYP.CLS, enumvalues5, &RET);
    COMPILEERROR(cat_ss(str_make(37, "Could not get name for type of class "), RET));
  }
  return RESULT;
}

PString TYPENAME(TPSTYPE *TYPEPTR) {
  PString RESULT;
  if (TYPEPTR == (void*)0) RESULT = str_make(6, "(none)");
  else RESULT = DEEPTYPENAME(TYPEPTR, 0);
  return RESULT;
}

TPSTYPE EMPTYTYPE() {
  TPSTYPE RESULT;
  TPSTYPE RET;
  RET.NAME = str_make(0, "");
  RET.CLS = TTCBOOLEAN;
  RET.ALIASFOR = (void*)0;
  RET.WASUSED = 0;
  RESULT = RET;
  return RESULT;
}

TPSTYPE COPYTYPE(TPSTYPE *TYPEPTR) {
  TPSTYPE RESULT;
  TPSTYPE NEWTYP;
  NEWTYP = *TYPEPTR;
  if (NEWTYP.CLS == TTCPOINTERUNKNOWN) {
    NEWTYP.TARGETNAME = malloc(sizeof(PString));
    *NEWTYP.TARGETNAME = *TYPEPTR->TARGETNAME;
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

int ISINTEGERTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCINTEGER;
  return RESULT;
}

int ISREALTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCREAL;
  return RESULT;
}

int ISNUMERICTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = ISINTEGERTYPE(TYPEPTR) || ISREALTYPE(TYPEPTR);
  return RESULT;
}

int ISSTRINGTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCSTRING;
  return RESULT;
}

int ISCHARTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCCHAR;
  return RESULT;
}

int ISSTRINGYTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = ISSTRINGTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR);
  return RESULT;
}

int ISBOOLEANTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCBOOLEAN;
  return RESULT;
}

int ISTEXTTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCTEXT;
  return RESULT;
}

int ISENUMTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCENUM;
  return RESULT;
}

int ISRANGETYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCRANGE;
  return RESULT;
}

int ISRECORDTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCRECORD;
  return RESULT;
}

int ISARRAYTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCARRAY;
  return RESULT;
}

TPSTYPE POINTERTYPE(TPSTYPE *TYPEPTR) {
  TPSTYPE RESULT;
  TPSTYPE TYP;
  TYP = TYPEOFCLASS(TTCPOINTER);
  TYP.POINTEDTYPEPTR = TYPEPTR;
  RESULT = TYP;
  return RESULT;
}

int ISPOINTERTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCPOINTER;
  return RESULT;
}

int ISNILTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCNIL;
  return RESULT;
}

int ISPOINTERYTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = ISPOINTERTYPE(TYPEPTR) || ISNILTYPE(TYPEPTR);
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

int ISPOINTERUNKNOWNTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCPOINTERUNKNOWN;
  return RESULT;
}

int ISORDINALTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISINTEGERTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

int ISBOUNDEDTYPE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

int GETTYPELOWBOUND(TPSTYPE *TYPEPTR) {
  int RESULT;
  switch (TYPEPTR->CLS) {
    case TTCBOOLEAN:
      RESULT = 0;
      break;
    case TTCCHAR:
      RESULT = 0;
      break;
    case TTCENUM:
      RESULT = 0;
      break;
    case TTCRANGE:
      RESULT = TYPEPTR->RANGEPTR->FIRST;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(21, "Type is not bounded: "), TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

int GETTYPEHIGHBOUND(TPSTYPE *TYPEPTR) {
  int RESULT;
  switch (TYPEPTR->CLS) {
    case TTCBOOLEAN:
      RESULT = 1;
      break;
    case TTCCHAR:
      RESULT = 255;
      break;
    case TTCENUM:
      RESULT = TYPEPTR->ENUMPTR->SIZE - 1;
      break;
    case TTCRANGE:
      RESULT = TYPEPTR->RANGEPTR->LAST;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(21, "Type is not bounded: "), TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

int GETBOUNDEDTYPESIZE(TPSTYPE *TYPEPTR) {
  int RESULT;
  RESULT = GETTYPEHIGHBOUND(TYPEPTR) - GETTYPELOWBOUND(TYPEPTR) + 1;
  return RESULT;
}

int ISSAMETYPE(TPSTYPE *A, TPSTYPE *B) {
  int RESULT;
  if (A == (void*)0 || B == (void*)0) RESULT = A == B;
  else {
    while (A->ALIASFOR != (void*)0) A = A->ALIASFOR;
    while (B->ALIASFOR != (void*)0) B = B->ALIASFOR;
    RESULT = A == B || ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE(A->POINTEDTYPEPTR, B->POINTEDTYPEPTR);
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
  TPSTYPE *TYPEPTR;
  int ENUMPOS;
  TYPEPTR = _ADDDEF(TDCTYPE)->TYPEPTR;
  *TYPEPTR = TYP;
  RESULT = TYPEPTR;
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) {
    if (FINDNAMEINLOCALSCOPE(TYP.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), TYP.NAME), str_make(16, " already defined")));
    ADDTYPENAME(TYP.NAME, TYPEPTR);
  }
  if (TYP.CLS == TTCENUM && TYP.ALIASFOR == (void*)0) do {
    int first = 0;
    int last = TYP.ENUMPTR->SIZE - 1;
    if (first <= last) {
      ENUMPOS = first;
      while (1) {
        ADDENUMVALNAME(ENUMPOS, TYPEPTR);
        if (ENUMPOS == last) break;
        ++ENUMPOS;
      }
    }
  } while(0);
  return RESULT;
}

TPSENUMDEF *ADDENUM(TPSENUMDEF ENUM) {
  TPSENUMDEF *RESULT;
  TPSENUMDEF *ENUMPTR;
  ENUMPTR = _ADDDEF(TDCENUM)->ENUMPTR;
  *ENUMPTR = ENUM;
  ENUMPTR->ID = DEFCOUNTER(TCTENUM);
  RESULT = ENUMPTR;
  return RESULT;
}

TPSRANGEDEF *ADDRANGE(TPSRANGEDEF RANGE) {
  TPSRANGEDEF *RESULT;
  RESULT = _ADDDEF(TDCRANGE)->RANGEPTR;
  *RESULT = RANGE;
  return RESULT;
}

TPSRECORDDEF *ADDRECORD(TPSRECORDDEF REC) {
  TPSRECORDDEF *RESULT;
  TPSRECORDDEF *RECPTR;
  RECPTR = _ADDDEF(TDCRECORD)->RECPTR;
  *RECPTR = REC;
  RECPTR->ID = DEFCOUNTER(TCTRECORD);
  RESULT = RECPTR;
  return RESULT;
}

TPSARRAYDEF *ADDARRAY(TPSARRAYDEF ARR) {
  TPSARRAYDEF *RESULT;
  TPSARRAYDEF *ARRAYPTR;
  ARRAYPTR = _ADDDEF(TDCARRAY)->ARRAYPTR;
  *ARRAYPTR = ARR;
  RESULT = ARRAYPTR;
  return RESULT;
}

TPSCONSTANT *ADDCONSTANT(TPSCONSTANT CONSTANT) {
  TPSCONSTANT *RESULT;
  TPSCONSTANT *CONSTPTR;
  if (FINDNAMEINLOCALSCOPE(CONSTANT.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), CONSTANT.NAME), str_make(16, " already defined")));
  CONSTPTR = _ADDDEF(TDCCONSTANT)->CONSTPTR;
  ADDCONSTANTNAME(CONSTANT.NAME, CONSTPTR);
  *CONSTPTR = CONSTANT;
  RESULT = CONSTPTR;
  return RESULT;
}

TPSVARIABLE *ADDVARIABLE(TPSVARIABLE VARDEF) {
  TPSVARIABLE *RESULT;
  TPSVARIABLE *VARPTR;
  if (FINDNAMEINLOCALSCOPE(VARDEF.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), VARDEF.NAME), str_make(16, " already defined")));
  VARPTR = _ADDDEF(TDCVARIABLE)->VARPTR;
  ADDVARIABLENAME(VARDEF.NAME, VARPTR);
  *VARPTR = VARDEF;
  RESULT = VARPTR;
  return RESULT;
}

TPSFUNCTION EMPTYFUNCTION() {
  TPSFUNCTION RESULT;
  RESULT.NAME = str_make(0, "");
  RESULT.ARGCOUNT = 0;
  RESULT.RETURNTYPEPTR = (void*)0;
  RESULT.ISDECLARATION = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

int ISSAMEFUNCTIONDEFINITION(TPSFUNCTION *DECLPTR, TPSFUNCTION FUN) {
  int RESULT;
  TPSFUNCTION DECL;
  int SAME;
  int POS;
  DECL = *DECLPTR;
  SAME = ISSAMETYPE(DECL.RETURNTYPEPTR, FUN.RETURNTYPEPTR) && DECL.ARGCOUNT == FUN.ARGCOUNT;
  do {
    int first = 1;
    int last = DECL.ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        SAME = SAME && ISSAMETYPE(DECL.ARGS[(int)subrange(POS, 1, 8) - 1].TYPEPTR, FUN.ARGS[(int)subrange(POS, 1, 8) - 1].TYPEPTR) && DECL.ARGS[(int)subrange(POS, 1, 8) - 1].ISREFERENCE == FUN.ARGS[(int)subrange(POS, 1, 8) - 1].ISREFERENCE;
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
  TPSNAME *NAMEPTR;
  NAMEPTR = FINDNAMEOFCLASSINLOCALSCOPE(NAME, TNCFUNCTION, 0);
  RESULT = NAMEPTR != (void*)0 && NAMEPTR->FNPTR->ISDECLARATION;
  return RESULT;
}

TPSFUNCTION *ADDFUNCTION(TPSFUNCTION FUN) {
  TPSFUNCTION *RESULT;
  TPSNAME *NAMEPTR;
  TPSFUNCTION *FNPTR;
  int ISPROCEDURE;
  ISPROCEDURE = FUN.RETURNTYPEPTR == (void*)0;
  NAMEPTR = FINDNAMEINLOCALSCOPE(FUN.NAME, 0);
  if (NAMEPTR == (void*)0) {
    FNPTR = _ADDDEF(TDCFUNCTION)->FNPTR;
    *FNPTR = FUN;
    ADDFUNCTIONNAME(FUN.NAME, FNPTR);
  }
  else {
    if (NAMEPTR->CLS != TNCFUNCTION || FUN.ISDECLARATION) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), FUN.NAME), str_make(16, " already defined")));
    FNPTR = NAMEPTR->FNPTR;
    if (FNPTR->ISDECLARATION) {
      if (FUN.ARGCOUNT == 0 && FUN.RETURNTYPEPTR == (void*)0 || ISSAMEFUNCTIONDEFINITION(FNPTR, FUN)) FNPTR->ISDECLARATION = 0;
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
  RESULT = FNPTR;
  return RESULT;
}

int FINDFIELD(TPSTYPE *TYPEPTR, PString NAME, int REQUIRED) {
  int RESULT;
  int POS;
  int RET;
  if (TYPEPTR->CLS != TTCRECORD) COMPILEERROR(cat_ss(str_make(14, "Not a record: "), TYPENAME(TYPEPTR)));
  {
    TPSRECORDDEF *with1 = TYPEPTR->RECPTR;
    {
      RET = 0;
      POS = with1->SIZE;
      while (POS >= 1 && RET == 0) {
        if (cmp_ss(NAME, with1->FIELDS[(int)subrange(POS, 1, 32) - 1].NAME) == 0) RET = POS;
        POS = POS - 1;
      }
    }
  }
  if (REQUIRED && RET == 0) COMPILEERROR(cat_ss(str_make(17, "Field not found: "), NAME));
  RESULT = RET;
  return RESULT;
}

TPSTYPE *FINDFIELDTYPE(TPSTYPE *TYPEPTR, PString NAME, int REQUIRED) {
  TPSTYPE *RESULT;
  int POS;
  POS = FINDFIELD(TYPEPTR, NAME, REQUIRED);
  if (POS == 0) RESULT = (void*)0;
  else RESULT = TYPEPTR->RECPTR->FIELDS[(int)subrange(POS, 1, 32) - 1].TYPEPTR;
  return RESULT;
}

TPSWITHVAR *FINDWITHVAR(PString NAME) {
  TPSWITHVAR *RESULT;
  TPSWITHVAR *RET;
  TPSDEFENTRY *DEF;
  TPSTYPE *TYPEPTR;
  RET = (void*)0;
  DEF = DEFS.LATEST;
  while (RET == (void*)0 && DEF != (void*)0 && DEF->CLS != TDCSCOPEBOUNDARY) {
    if (DEF->CLS == TDCWITHVAR) {
      TYPEPTR = DEF->WITHVARPTR->VARPTR->TYPEPTR;
      if (FINDFIELDTYPE(TYPEPTR, NAME, 0) != (void*)0) RET = DEF->WITHVARPTR;
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
  TPSVARIABLE *TMPVARPTR;
  TPSWITHVAR *WITHVARPTR;
  if (!ISRECORDTYPE(BASE->TYPEPTR)) COMPILEERROR(str_make(31, "'With' variable is not a record"));
  STR_i(DEFCOUNTER(TCTTMPVAR), &TMPVARNUM);
  TMPVAR.NAME = cat_ss(str_make(4, "with"), TMPVARNUM);
  TMPVAR.TYPEPTR = BASE->TYPEPTR;
  TMPVAR.ISCONSTANT = 0;
  TMPVAR.ISREFERENCE = BASE->ISASSIGNABLE;
  TMPVARPTR = ADDVARIABLE(TMPVAR);
  WITHVARPTR = _ADDDEF(TDCWITHVAR)->WITHVARPTR;
  WITHVARPTR->VARPTR = TMPVARPTR;
  RESULT = TMPVARPTR;
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

TPSVARIABLE MAKETYPEDCONSTANT(PString NAME, TPSTYPE *TYPEPTR) {
  TPSVARIABLE RESULT;
  TPSVARIABLE VARDEF;
  VARDEF.NAME = NAME;
  VARDEF.TYPEPTR = TYPEPTR;
  VARDEF.ISREFERENCE = 0;
  VARDEF.ISCONSTANT = 1;
  VARDEF.WASINITIALIZED = 1;
  RESULT = VARDEF;
  return RESULT;
}

TPSVARIABLE MAKEVARIABLE(PString NAME, TPSTYPE *TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT.NAME = NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = 0;
  RESULT.ISCONSTANT = 0;
  RESULT.WASINITIALIZED = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

TPSVARIABLE _MAKEARG(PString NAME, TPSTYPE *TYPEPTR, int ISREF, int ISCONST) {
  TPSVARIABLE RESULT;
  RESULT.NAME = NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = ISREF || ISCONST;
  RESULT.ISCONSTANT = ISCONST;
  RESULT.WASINITIALIZED = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

TPSVARIABLE MAKEARG(PString NAME, TPSTYPE *TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 0, 0);
  return RESULT;
}

TPSVARIABLE MAKEVARARG(PString NAME, TPSTYPE *TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 1, 0);
  return RESULT;
}

TPSVARIABLE MAKECONSTARG(PString NAME, TPSTYPE *TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 0, 1);
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE1(PString NAME, TPSVARIABLE ARG) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.ARGCOUNT = 1;
  RESULT.ARGS[0] = ARG;
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE2(PString NAME, TPSVARIABLE ARG1, TPSVARIABLE ARG2) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.ARGCOUNT = 2;
  RESULT.ARGS[0] = ARG1;
  RESULT.ARGS[1] = ARG2;
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE3(PString NAME, TPSVARIABLE ARG1, TPSVARIABLE ARG2, TPSVARIABLE ARG3) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.ARGCOUNT = 3;
  RESULT.ARGS[0] = ARG1;
  RESULT.ARGS[1] = ARG2;
  RESULT.ARGS[2] = ARG3;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION0(PString NAME, TPSTYPE *RETTYPEPTR) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION1(PString NAME, TPSTYPE *RETTYPEPTR, TPSVARIABLE ARG) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGCOUNT = 1;
  RESULT.ARGS[0] = ARG;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION2(PString NAME, TPSTYPE *RETTYPEPTR, TPSVARIABLE ARG1, TPSVARIABLE ARG2) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGCOUNT = 2;
  RESULT.ARGS[0] = ARG1;
  RESULT.ARGS[1] = ARG2;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION3(PString NAME, TPSTYPE *RETTYPEPTR, TPSVARIABLE ARG1, TPSVARIABLE ARG2, TPSVARIABLE ARG3) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGCOUNT = 3;
  RESULT.ARGS[0] = ARG1;
  RESULT.ARGS[1] = ARG2;
  RESULT.ARGS[2] = ARG3;
  return RESULT;
}

void CODEGENRESET();
void CODEGENSETOUTPUT(PString FILENAME);
void CODEGENSETCHECKBOUNDS(int CHECKBOUNDS);
void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT);
void OUTTYPEDEFINITIONSFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT);
void OUTCONSTANTARRAYBEGIN();
void OUTCONSTANTARRAYSEPARATOR();
void OUTCONSTANTARRAYEND();
void OUTVARIABLEDEFINITION(TPSVARIABLE *VARPTR);
void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE *VARPTR);
void OUTCONSTANTDEFINITIONEND();
void OUTFUNCTIONDECLARATION(TPSFUNCTION *FNPTR);
void OUTFUNCTIONDEFINITION(TPSFUNCTION *FNPTR);
void OUTFUNCTIONEND(TPSFUNCTION *FNPTR);
void OUTEXPRESSION(TEXPRESSIONOBJ *EXPR);
void OUTASSIGN(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS);
void OUTDECLAREANDASSIGN(TPSVARIABLE *VARPTR, TEXPRESSIONOBJ *RHS);
void OUTSEQUENCEBEGIN();
void OUTSEQUENCEEND();
void OUTIF(TEXPRESSIONOBJ *EXPR);
void OUTELSE();
void OUTIFEND();
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
void OUTPROGRAMHEADING(PString NAME);
void OUTPROGRAMBEGIN();
void OUTPROGRAMEND();

TEXPRESSIONOBJ *_NEWEXPR(TEXPRESSIONCLASS CLS) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = malloc(sizeof(TEXPRESSIONOBJ));
  EXPR->CLS = CLS;
  EXPR->ISASSIGNABLE = 0;
  EXPR->ISFUNCTIONRESULT = 0;
  RESULT = EXPR;
  return RESULT;
}

void _DISPOSEPSEUDOCALLEXPR(TEXPSEUDOFNCALL *CALL) {
  TEXREADARGS *READARG;
  TEXREADARGS *NEXTREADARG;
  TEXWRITEARGS *WRITEARG;
  TEXWRITEARGS *NEXTWRITEARG;
  if (CALL->ARG1 != (void*)0) DISPOSEEXPR(&CALL->ARG1);
  if (CALL->ARG2 != (void*)0) DISPOSEEXPR(&CALL->ARG2);
  if (CALL->ARG3 != (void*)0) DISPOSEEXPR(&CALL->ARG3);
  if (CALL->PSEUDOFN == TPFWRITE || CALL->PSEUDOFN == TPFWRITELN) {
    READARG = CALL->READARGS;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      DISPOSEEXPR(&READARG->ARG);
      free(READARG);
      READARG = NEXTREADARG;
    }
  }
  else if (CALL->PSEUDOFN == TPFREAD || CALL->PSEUDOFN == TPFREADLN) {
    WRITEARG = CALL->WRITEARGS;
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
      DISPOSEEXPR(&(*EXPR)->TOSTRPARENT);
      break;
    case XCTOREAL:
      DISPOSEEXPR(&(*EXPR)->TOREALPARENT);
      break;
    case XCSETTMPVAR:
      {
        DISPOSEEXPR(&(*EXPR)->TMPVAR);
        DISPOSEEXPR(&(*EXPR)->TMPVARVALUE);
        DISPOSEEXPR(&(*EXPR)->TMPVARCHILD);
      }
      break;
    case XCSUBRANGE:
      DISPOSEEXPR(&(*EXPR)->SUBRANGEPARENT);
      break;
    case XCFIELD:
      DISPOSEEXPR(&(*EXPR)->RECEXPR);
      break;
    case XCARRAY:
      {
        DISPOSEEXPR(&(*EXPR)->ARRAYEXPR);
        DISPOSEEXPR(&(*EXPR)->ARRAYINDEX);
      }
      break;
    case XCPOINTER:
      DISPOSEEXPR(&(*EXPR)->POINTEREXPR);
      break;
    case XCSTRINGCHAR:
      {
        DISPOSEEXPR(&(*EXPR)->STRINGEXPR);
        DISPOSEEXPR(&(*EXPR)->STRINGINDEX);
      }
      break;
    case XCFNCALL:
      {
        DISPOSEEXPR(&(*EXPR)->FNEXPR);
        do {
          int first = 1;
          int last = (*EXPR)->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              DISPOSEEXPR(&(*EXPR)->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]);
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
      }
      break;
    case XCPSEUDOFNCALL:
      _DISPOSEPSEUDOCALLEXPR(&(*EXPR)->PSEUDOFNCALL);
      break;
    case XCUNARYOP:
      DISPOSEEXPR(&(*EXPR)->UNARY.PARENT);
      break;
    case XCBINARYOP:
      {
        DISPOSEEXPR(&(*EXPR)->BINARY.LEFT);
        DISPOSEEXPR(&(*EXPR)->BINARY.RIGHT);
      }
      break;
    default:
      break;
  }
  free(*EXPR);
}

void _COPYPSEUDOCALLEXPR(TEXPSEUDOFNCALL *CALL, TEXPSEUDOFNCALL *COPY) {
  TEXREADARGS *READARG;
  TEXREADARGS *NEXTREADARG;
  TEXREADARGS *COPYREADARG;
  TEXWRITEARGS *WRITEARG;
  TEXWRITEARGS *NEXTWRITEARG;
  TEXWRITEARGS *COPYWRITEARG;
  COPY->PSEUDOFN = CALL->PSEUDOFN;
  if (CALL->ARG1 != (void*)0) COPY->ARG1 = COPYEXPR(CALL->ARG1);
  if (CALL->ARG2 != (void*)0) COPY->ARG2 = COPYEXPR(CALL->ARG2);
  if (CALL->ARG3 != (void*)0) COPY->ARG3 = COPYEXPR(CALL->ARG3);
  if (CALL->PSEUDOFN == TPFWRITE || CALL->PSEUDOFN == TPFWRITELN) {
    READARG = CALL->READARGS;
    COPYREADARG = (void*)0;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      if (COPYREADARG == (void*)0) {
        COPYREADARG = malloc(sizeof(TEXREADARGS));
        COPY->READARGS = COPYREADARG;
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
  else if (CALL->PSEUDOFN == TPFREAD || CALL->PSEUDOFN == TPFREADLN) {
    WRITEARG = CALL->WRITEARGS;
    COPYWRITEARG = (void*)0;
    while (WRITEARG != (void*)0) {
      NEXTWRITEARG = WRITEARG->NEXT;
      if (COPYWRITEARG == (void*)0) {
        COPYWRITEARG = malloc(sizeof(TEXWRITEARGS));
        COPY->WRITEARGS = COPYWRITEARG;
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
  COPY->TYPEPTR = EXPR->TYPEPTR;
  COPY->ISASSIGNABLE = EXPR->ISASSIGNABLE;
  COPY->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      COPY->IMMEDIATE = EXPR->IMMEDIATE;
      break;
    case XCTOSTRING:
      COPY->TOSTRPARENT = COPYEXPR(EXPR->TOSTRPARENT);
      break;
    case XCTOREAL:
      COPY->TOREALPARENT = COPYEXPR(EXPR->TOREALPARENT);
      break;
    case XCSETTMPVAR:
      {
        COPY->TMPVAR = COPYEXPR(EXPR->TMPVAR);
        COPY->TMPVARVALUE = COPYEXPR(EXPR->TMPVARVALUE);
        COPY->TMPVARCHILD = COPYEXPR(EXPR->TMPVARCHILD);
      }
      break;
    case XCSUBRANGE:
      COPY->SUBRANGEPARENT = COPYEXPR(EXPR->SUBRANGEPARENT);
      break;
    case XCVARIABLE:
      COPY->VARPTR = EXPR->VARPTR;
      break;
    case XCFIELD:
      {
        COPY->RECEXPR = COPYEXPR(EXPR->RECEXPR);
        COPY->RECFIELDNUM = EXPR->RECFIELDNUM;
      }
      break;
    case XCARRAY:
      {
        COPY->ARRAYEXPR = COPYEXPR(EXPR->ARRAYEXPR);
        COPY->ARRAYINDEX = COPYEXPR(EXPR->ARRAYINDEX);
      }
      break;
    case XCPOINTER:
      COPY->POINTEREXPR = COPYEXPR(EXPR->POINTEREXPR);
      break;
    case XCSTRINGCHAR:
      {
        COPY->STRINGEXPR = COPYEXPR(EXPR->STRINGEXPR);
        COPY->STRINGINDEX = COPYEXPR(EXPR->STRINGINDEX);
      }
      break;
    case XCFNREF:
      COPY->FNPTR = EXPR->FNPTR;
      break;
    case XCFNCALL:
      {
        COPY->FNEXPR = COPYEXPR(EXPR->FNEXPR);
        COPY->CALLARGS.SIZE = EXPR->CALLARGS.SIZE;
        do {
          int first = 1;
          int last = EXPR->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              COPY->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1] = COPYEXPR(EXPR->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]);
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
      }
      break;
    case XCPSEUDOFNREF:
      COPY->PSEUDOFN = EXPR->PSEUDOFN;
      break;
    case XCPSEUDOFNCALL:
      _COPYPSEUDOCALLEXPR(&EXPR->PSEUDOFNCALL, &COPY->PSEUDOFNCALL);
      break;
    case XCUNARYOP:
      {
        COPY->UNARY.PARENT = COPYEXPR(EXPR->UNARY.PARENT);
        COPY->UNARY.OP = EXPR->UNARY.OP;
      }
      break;
    case XCBINARYOP:
      {
        COPY->BINARY.LEFT = COPYEXPR(EXPR->BINARY.LEFT);
        COPY->BINARY.RIGHT = COPYEXPR(EXPR->BINARY.RIGHT);
        COPY->BINARY.OP = EXPR->BINARY.OP;
      }
      break;
    default:
      COMPILEERROR(cat_ss(str_make(40, "Internal error: cannot copy expression: "), DESCRIBEEXPR(EXPR, 2)));
      break;
  }
  RESULT = COPY;
  return RESULT;
}

PString _UNPARSECHAR(unsigned char CHR) {
  PString RESULT;
  PString CHNUM;
  if (CHR == '\'') RESULT = str_make(4, "''''");
  else if (CHR < ' ') {
    STR_i((int)CHR, &CHNUM);
    RESULT = cat_cs('#', CHNUM);
  }
  else RESULT = cat_sc(cat_cc('\'', CHR), '\'');
  return RESULT;
}

PString _UNPARSESTRING(PString ST) {
  PString RESULT;
  int POS;
  PString CHNUM;
  int QUOTED;
  QUOTED = 0;
  RESULT = str_make(0, "");
  do {
    int first = 1;
    int last = LENGTH(&ST);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (ST.chr[POS] < ' ') {
            if (QUOTED) {
              QUOTED = 0;
              RESULT = cat_sc(RESULT, '\'');
            }
            STR_i((int)ST.chr[POS], &CHNUM);
            RESULT = cat_ss(cat_sc(RESULT, '#'), CHNUM);
          }
          else {
            if (!QUOTED) {
              QUOTED = 1;
              RESULT = cat_sc(RESULT, '\'');
            }
            if (ST.chr[POS] == '\'') RESULT = cat_ss(RESULT, str_make(2, "''"));
            else RESULT = cat_sc(RESULT, ST.chr[POS]);
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  if (QUOTED) RESULT = cat_sc(RESULT, '\'');
  if (cmp_ss(RESULT, str_make(0, "")) == 0) RESULT = str_make(2, "''");
  return RESULT;
}

PString _DESCRIBEIMMEDIATEPR(TEXPRESSIONOBJ *EXPR) {
  PString RESULT;
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICNIL:
        RESULT = str_make(3, "nil");
        break;
      case XICBOOLEAN:
        STR_b(with1->BOOLEANVAL, &RESULT);
        break;
      case XICINTEGER:
        STR_i(with1->INTEGERVAL, &RESULT);
        break;
      case XICREAL:
        STR_r(with1->REALVAL, &RESULT);
        break;
      case XICCHAR:
        RESULT = _UNPARSECHAR(with1->CHARVAL);
        break;
      case XICSTRING:
        RESULT = _UNPARSESTRING(with1->STRINGVAL);
        break;
      case XICENUM:
        RESULT = with1->ENUMPTR->VALUES[subrange(with1->ENUMORDINAL, 0, 127)];
        break;
      default:
        COMPILEERROR(str_make(47, "Internal error: cannot describe immediate value"));
        break;
    }
  }
  return RESULT;
}

PString _DESCRIBEPSEUDOFNEXPR(TEXPRESSIONOBJ *EXPR) {
  PString RESULT;
  switch (EXPR->PSEUDOFN) {
    case TPFCONCAT:
      RESULT = str_make(6, "CONCAT");
      break;
    case TPFDISPOSE:
      RESULT = str_make(7, "DISPOSE");
      break;
    case TPFNEW:
      RESULT = str_make(3, "NEW");
      break;
    case TPFPRED:
      RESULT = str_make(4, "PRED");
      break;
    case TPFREAD:
      RESULT = str_make(4, "READ");
      break;
    case TPFREADLN:
      RESULT = str_make(6, "READLN");
      break;
    case TPFSTR:
      RESULT = str_make(3, "STR");
      break;
    case TPFSUCC:
      RESULT = str_make(4, "SUCC");
      break;
    case TPFVAL:
      RESULT = str_make(3, "VAL");
      break;
    case TPFWRITE:
      RESULT = str_make(5, "WRITE");
      break;
    case TPFWRITELN:
      RESULT = str_make(7, "WRITELN");
      break;
    default:
      COMPILEERROR(str_make(41, "Internal error: cannot describe pseudofun"));
      break;
  }
  return RESULT;
}

PString _DESCRIBEPSEUDOCALLEXPR(TEXPRESSIONOBJ *EXPR, int LEVELS) {
  PString RESULT;
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    switch (with1->PSEUDOFN) {
      case TPFDISPOSE:
        RESULT = cat_sc(cat_ss(str_make(8, "DISPOSE("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), ')');
        break;
      case TPFNEW:
        RESULT = cat_sc(cat_ss(str_make(4, "NEW("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), ')');
        break;
      case TPFPRED:
        RESULT = cat_sc(cat_ss(str_make(5, "PRED("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), ')');
        break;
      case TPFREAD:
        RESULT = str_make(9, "READ(...)");
        break;
      case TPFREADLN:
        RESULT = str_make(11, "READLN(...)");
        break;
      case TPFSTR:
        RESULT = cat_sc(cat_ss(cat_ss(cat_ss(str_make(4, "STR("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), str_make(2, ", ")), DESCRIBEEXPR(with1->ARG2, LEVELS - 1)), ')');
        break;
      case TPFSUCC:
        RESULT = cat_sc(cat_ss(str_make(5, "SUCC("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), ')');
        break;
      case TPFVAL:
        RESULT = cat_sc(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(4, "VAL("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), str_make(2, ", ")), DESCRIBEEXPR(with1->ARG2, LEVELS - 1)), str_make(2, ", ")), DESCRIBEEXPR(with1->ARG3, LEVELS - 1)), ')');
        break;
      case TPFWRITE:
        RESULT = str_make(10, "WRITE(...)");
        break;
      case TPFWRITELN:
        RESULT = str_make(12, "WRITELN(...)");
        break;
      default:
        COMPILEERROR(str_make(41, "Internal error: cannot describe pseudofun"));
        break;
    }
  }
  return RESULT;
}

int _EXPRPRECEDENCE(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      RESULT = 0;
      break;
    case XCTOSTRING:
      RESULT = _EXPRPRECEDENCE(EXPR->TOSTRPARENT);
      break;
    case XCTOREAL:
      RESULT = _EXPRPRECEDENCE(EXPR->TOREALPARENT);
      break;
    case XCSETTMPVAR:
      RESULT = _EXPRPRECEDENCE(EXPR->TMPVARCHILD);
      break;
    case XCSUBRANGE:
      RESULT = _EXPRPRECEDENCE(EXPR->SUBRANGEPARENT);
      break;
    case XCVARIABLE:
      RESULT = 0;
      break;
    case XCFIELD:
      RESULT = 1;
      break;
    case XCARRAY:
      RESULT = 1;
      break;
    case XCPOINTER:
      RESULT = 1;
      break;
    case XCSTRINGCHAR:
      RESULT = 1;
      break;
    case XCFNREF:
      RESULT = 0;
      break;
    case XCFNCALL:
      RESULT = 1;
      break;
    case XCPSEUDOFNREF:
      RESULT = 0;
      break;
    case XCPSEUDOFNCALL:
      RESULT = 1;
      break;
    case XCUNARYOP:
      switch (EXPR->UNARY.OP) {
        case TKMINUS:
          RESULT = 4;
          break;
        case TKNOT:
          RESULT = 2;
          break;
        default:
          break;
      }
      break;
    case XCBINARYOP:
      switch (EXPR->BINARY.OP) {
        case TKPLUS:
          RESULT = 4;
          break;
        case TKMINUS:
          RESULT = 4;
          break;
        case TKASTERISK:
          RESULT = 3;
          break;
        case TKDIV:
          RESULT = 3;
          break;
        case TKAND:
          RESULT = 3;
          break;
        case TKOR:
          RESULT = 4;
          break;
        case TKEQUALS:
          RESULT = 5;
          break;
        case TKNOTEQUALS:
          RESULT = 5;
          break;
        case TKLESSTHAN:
          RESULT = 5;
          break;
        case TKMORETHAN:
          RESULT = 5;
          break;
        case TKLESSOREQUALS:
          RESULT = 5;
          break;
        case TKMOREOREQUALS:
          RESULT = 5;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  return RESULT;
}

PString _DESCRIBEUNARYOPEXPR(TEXPRESSIONOBJ *EXPR, int LEVELS) {
  PString RESULT;
  int USEPARENS;
  switch (EXPR->UNARY.OP) {
    case TKMINUS:
      RESULT = str_of('-');
      break;
    case TKNOT:
      RESULT = str_make(4, "not ");
      break;
    default:
      COMPILEERROR(str_make(47, "Internal error: cannot describe unary operation"));
      break;
  }
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->UNARY.PARENT);
  if (USEPARENS) RESULT = cat_sc(RESULT, '(');
  RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->UNARY.PARENT, LEVELS));
  if (USEPARENS) RESULT = cat_sc(RESULT, ')');
  return RESULT;
}

PString _DESCRIBEBINARYOPEXPR(TEXPRESSIONOBJ *EXPR, int LEVELS) {
  PString RESULT;
  int USEPARENS;
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->BINARY.LEFT);
  if (USEPARENS) RESULT = str_of('(');
  else RESULT = str_make(0, "");
  RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->BINARY.LEFT, LEVELS - 1));
  if (USEPARENS) RESULT = cat_sc(RESULT, ')');
  switch (EXPR->BINARY.OP) {
    case TKPLUS:
      RESULT = cat_ss(RESULT, str_make(3, " + "));
      break;
    case TKMINUS:
      RESULT = cat_ss(RESULT, str_make(3, " - "));
      break;
    case TKASTERISK:
      RESULT = cat_ss(RESULT, str_make(3, " * "));
      break;
    case TKSLASH:
      RESULT = cat_ss(RESULT, str_make(3, " / "));
      break;
    case TKDIV:
      RESULT = cat_ss(RESULT, str_make(5, " div "));
      break;
    case TKAND:
      RESULT = cat_ss(RESULT, str_make(5, " and "));
      break;
    case TKOR:
      RESULT = cat_ss(RESULT, str_make(4, " or "));
      break;
    case TKEQUALS:
      RESULT = cat_ss(RESULT, str_make(3, " = "));
      break;
    case TKNOTEQUALS:
      RESULT = cat_ss(RESULT, str_make(4, " <> "));
      break;
    case TKLESSTHAN:
      RESULT = cat_ss(RESULT, str_make(3, " < "));
      break;
    case TKMORETHAN:
      RESULT = cat_ss(RESULT, str_make(3, " > "));
      break;
    case TKLESSOREQUALS:
      RESULT = cat_ss(RESULT, str_make(4, " <= "));
      break;
    case TKMOREOREQUALS:
      RESULT = cat_ss(RESULT, str_make(4, " >= "));
      break;
    default:
      COMPILEERROR(str_make(48, "Internal error: cannot describe binary operation"));
      break;
  }
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->BINARY.RIGHT);
  if (USEPARENS) RESULT = cat_sc(RESULT, '(');
  RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->BINARY.RIGHT, LEVELS - 1));
  if (USEPARENS) RESULT = cat_sc(RESULT, ')');
  return RESULT;
}

PString _DESCRIBESETTMPVAR(TEXPRESSIONOBJ *EXPR, int LEVELS) {
  PString RESULT;
  RESULT = str_make(6, "{with ");
  while (EXPR->CLS == XCSETTMPVAR) {
    RESULT = cat_ss(cat_ss(cat_ss(RESULT, DESCRIBEEXPR(EXPR->TMPVAR, LEVELS)), str_make(2, ":=")), DESCRIBEEXPR(EXPR->TMPVARVALUE, LEVELS - 1));
    EXPR = EXPR->TMPVARCHILD;
    if (EXPR->CLS == XCSETTMPVAR) RESULT = cat_ss(RESULT, str_make(2, ", "));
  }
  RESULT = cat_ss(cat_ss(RESULT, str_make(2, "} ")), DESCRIBEEXPR(EXPR, LEVELS - 1));
  return RESULT;
}

PString DESCRIBEEXPR(TEXPRESSIONOBJ *EXPR, int LEVELS) {
  PString RESULT;
  int POS;
  if (LEVELS < 1) RESULT = str_make(5, "(...)");
  else switch (EXPR->CLS) {
    case XCIMMEDIATE:
      RESULT = _DESCRIBEIMMEDIATEPR(EXPR);
      break;
    case XCTOSTRING:
      RESULT = DESCRIBEEXPR(EXPR->TOSTRPARENT, LEVELS);
      break;
    case XCTOREAL:
      RESULT = DESCRIBEEXPR(EXPR->TOREALPARENT, LEVELS);
      break;
    case XCSETTMPVAR:
      RESULT = _DESCRIBESETTMPVAR(EXPR, LEVELS);
      break;
    case XCSUBRANGE:
      RESULT = DESCRIBEEXPR(EXPR->TOSTRPARENT, LEVELS);
      break;
    case XCVARIABLE:
      RESULT = EXPR->VARPTR->NAME;
      break;
    case XCFIELD:
      RESULT = cat_ss(cat_sc(DESCRIBEEXPR(EXPR->RECEXPR, LEVELS), '.'), EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[(int)subrange(EXPR->RECFIELDNUM, 1, 32) - 1].NAME);
      break;
    case XCARRAY:
      RESULT = cat_sc(cat_ss(cat_sc(DESCRIBEEXPR(EXPR->ARRAYEXPR, LEVELS), '['), DESCRIBEEXPR(EXPR->ARRAYINDEX, LEVELS - 1)), ']');
      break;
    case XCPOINTER:
      RESULT = cat_sc(DESCRIBEEXPR(EXPR->POINTEREXPR, LEVELS), '^');
      break;
    case XCSTRINGCHAR:
      RESULT = cat_sc(cat_ss(cat_sc(DESCRIBEEXPR(EXPR->STRINGEXPR, LEVELS), '['), DESCRIBEEXPR(EXPR->STRINGINDEX, LEVELS - 1)), ']');
      break;
    case XCFNREF:
      RESULT = EXPR->FNPTR->NAME;
      break;
    case XCFNCALL:
      {
        RESULT = cat_sc(DESCRIBEEXPR(EXPR->FNEXPR, LEVELS), '(');
        if (LEVELS < 2) RESULT = cat_ss(RESULT, str_make(3, "..."));
        else do {
          int first = 1;
          int last = EXPR->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              {
                if (POS != 1) RESULT = cat_ss(RESULT, str_make(2, ", "));
                RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1], LEVELS - 1));
              }
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
        RESULT = cat_sc(RESULT, ')');
      }
      break;
    case XCPSEUDOFNREF:
      RESULT = _DESCRIBEPSEUDOFNEXPR(EXPR);
      break;
    case XCPSEUDOFNCALL:
      RESULT = _DESCRIBEPSEUDOCALLEXPR(EXPR, LEVELS);
      break;
    case XCUNARYOP:
      RESULT = _DESCRIBEUNARYOPEXPR(EXPR, LEVELS);
      break;
    case XCBINARYOP:
      RESULT = _DESCRIBEBINARYOPEXPR(EXPR, LEVELS);
      break;
    default:
      COMPILEERROR(str_make(42, "Internal error: cannot describe expression"));
      break;
  }
  return RESULT;
}

TEXPRESSIONOBJ *EXCOERCE(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEPTR);

TEXPRESSIONOBJ *_EXIMMEDIATE(TEXIMMEDIATECLASS CLS) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = _NEWEXPR(XCIMMEDIATE);
  EXPR->IMMEDIATE.CLS = CLS;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXNIL() {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICNIL);
  RESULT->TYPEPTR = PRIMITIVETYPES.PTNIL;
  return RESULT;
}

TEXPRESSIONOBJ *EXBOOLEANCONSTANT(int VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICBOOLEAN);
  RESULT->IMMEDIATE.BOOLEANVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  return RESULT;
}

TEXPRESSIONOBJ *EXINTEGERCONSTANT(int VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICINTEGER);
  RESULT->IMMEDIATE.INTEGERVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  return RESULT;
}

TEXPRESSIONOBJ *EXREALCONSTANT(double VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICREAL);
  RESULT->IMMEDIATE.REALVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  return RESULT;
}

TEXPRESSIONOBJ *EXCHARCONSTANT(unsigned char VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICCHAR);
  RESULT->IMMEDIATE.CHARVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTCHAR;
  return RESULT;
}

TEXPRESSIONOBJ *EXSTRINGCONSTANT(PString VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICSTRING);
  RESULT->IMMEDIATE.STRINGVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
  return RESULT;
}

TEXPRESSIONOBJ *EXENUMCONSTANT(int ORDINAL, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISENUMTYPE(TYPEPTR)) COMPILEERROR(cat_ss(str_make(25, "Not an enumeration type: "), TYPENAME(TYPEPTR)));
  if (ORDINAL < 0 || ORDINAL > TYPEPTR->ENUMPTR->SIZE - 1) COMPILEERROR(cat_ss(str_make(18, "Invalid value for "), TYPENAME(TYPEPTR)));
  RESULT = _EXIMMEDIATE(XICENUM);
  RESULT->IMMEDIATE.ENUMORDINAL = ORDINAL;
  RESULT->IMMEDIATE.ENUMPTR = TYPEPTR->ENUMPTR;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

int EXISIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  RESULT = EXPR->CLS == XCIMMEDIATE;
  return RESULT;
}

int EXISIMMEDIATEOFCLASS(TEXPRESSIONOBJ *EXPR, TEXIMMEDIATECLASS CLS) {
  int RESULT;
  RESULT = EXPR->CLS == XCIMMEDIATE && EXPR->IMMEDIATE.CLS == CLS;
  return RESULT;
}

TEXPRESSIONOBJ *EXTOSTRING(TEXPRESSIONOBJ *PARENT) {
  TEXPRESSIONOBJ *RESULT;
  PString STR;
  if (ISCHARTYPE(PARENT->TYPEPTR)) {
    if (EXISIMMEDIATE(PARENT)) {
      STR = str_of(PARENT->IMMEDIATE.CHARVAL);
      PARENT->IMMEDIATE.CLS = XICSTRING;
      PARENT->IMMEDIATE.STRINGVAL = STR;
      RESULT = PARENT;
    }
    else {
      RESULT = _NEWEXPR(XCTOSTRING);
      RESULT->TOSTRPARENT = PARENT;
      RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
      RESULT->ISASSIGNABLE = 0;
      RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
    }
  }
  else if (ISSTRINGTYPE(PARENT->TYPEPTR)) RESULT = PARENT;
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(29, "Cannot convert value of type "), TYPENAME(PARENT->TYPEPTR)), str_make(12, " to string: ")), DESCRIBEEXPR(PARENT, 10)));
  return RESULT;
}

TEXPRESSIONOBJ *EXTOREAL(TEXPRESSIONOBJ *PARENT) {
  TEXPRESSIONOBJ *RESULT;
  double VALUE;
  if (EXISIMMEDIATE(PARENT)) {
    VALUE = (double)PARENT->IMMEDIATE.INTEGERVAL;
    PARENT->IMMEDIATE.CLS = XICREAL;
    PARENT->IMMEDIATE.REALVAL = VALUE;
    PARENT->TYPEPTR = PRIMITIVETYPES.PTREAL;
    RESULT = PARENT;
  }
  else {
    RESULT = _NEWEXPR(XCTOREAL);
    RESULT->TOREALPARENT = PARENT;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
    RESULT->ISASSIGNABLE = 0;
    RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  }
  return RESULT;
}

TEXPRESSIONOBJ *EXSETTMPVAR(TEXPRESSIONOBJ *TMPVAR, TEXPRESSIONOBJ *VALUE, TEXPRESSIONOBJ *CHILD) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCSETTMPVAR);
  RESULT->TMPVAR = TMPVAR;
  RESULT->TMPVARVALUE = VALUE;
  RESULT->TMPVARCHILD = CHILD;
  RESULT->TYPEPTR = CHILD->TYPEPTR;
  RESULT->ISASSIGNABLE = 1;
  RESULT->ISFUNCTIONRESULT = 0;
  TMPVAR->VARPTR->WASINITIALIZED = 1;
  TMPVAR->VARPTR->WASUSED = 1;
  return RESULT;
}

TEXPRESSIONOBJ *EXSUBRANGE(TEXPRESSIONOBJ *PARENT, TPSTYPE *TYPEPTR);

TEXPRESSIONOBJ *EXOUTRANGE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *TMPEXPR;
  while (EXPR->CLS == XCSUBRANGE) {
    TMPEXPR = COPYEXPR(EXPR->SUBRANGEPARENT);
    DISPOSEEXPR(&EXPR);
    EXPR = TMPEXPR;
  }
  while (ISRANGETYPE(EXPR->TYPEPTR)) EXPR->TYPEPTR = EXPR->TYPEPTR->RANGEPTR->BASETYPEPTR;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXVARIABLE(TPSVARIABLE *VARPTR) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCVARIABLE);
  RESULT->VARPTR = VARPTR;
  RESULT->TYPEPTR = VARPTR->TYPEPTR;
  RESULT->ISASSIGNABLE = !VARPTR->ISCONSTANT;
  return RESULT;
}

TEXPRESSIONOBJ *EXFIELDACCESS(TEXPRESSIONOBJ *PARENT, int FIELDNUM) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISRECORDTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(str_make(48, "Cannot access field of non-record value of type "), TYPENAME(PARENT->TYPEPTR)));
  if (FIELDNUM < 1 || FIELDNUM > PARENT->TYPEPTR->RECPTR->SIZE) COMPILEERROR(cat_ss(str_make(23, "Invalid field for type "), TYPENAME(PARENT->TYPEPTR)));
  RESULT = _NEWEXPR(XCFIELD);
  RESULT->RECEXPR = PARENT;
  RESULT->RECFIELDNUM = FIELDNUM;
  RESULT->TYPEPTR = PARENT->TYPEPTR->RECPTR->FIELDS[(int)subrange(FIELDNUM, 1, 32) - 1].TYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ *EXARRAYACCESS(TEXPRESSIONOBJ *PARENT, TEXPRESSIONOBJ *SUBSCRIPT) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISARRAYTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(51, "Cannot access subscript of non-array value of type "), TYPENAME(PARENT->TYPEPTR)), str_make(2, ": ")), DESCRIBEEXPR(PARENT, 10)));
  RESULT = _NEWEXPR(XCARRAY);
  RESULT->ARRAYEXPR = PARENT;
  RESULT->ARRAYINDEX = EXCOERCE(SUBSCRIPT, PARENT->TYPEPTR->ARRAYPTR->INDEXTYPEPTR);
  RESULT->TYPEPTR = PARENT->TYPEPTR->ARRAYPTR->VALUETYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ *EXPOINTERACCESS(TEXPRESSIONOBJ *PARENT) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISPOINTERTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(45, "Cannot dereference non-pointer value of type "), TYPENAME(PARENT->TYPEPTR)), str_make(2, ": ")), DESCRIBEEXPR(PARENT, 10)));
  RESULT = _NEWEXPR(XCPOINTER);
  RESULT->POINTEREXPR = PARENT;
  RESULT->TYPEPTR = PARENT->TYPEPTR->POINTEDTYPEPTR;
  RESULT->ISASSIGNABLE = 1;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ *EXSTRINGCHAR(TEXPRESSIONOBJ *PARENT, TEXPRESSIONOBJ *SUBSCRIPT) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISSTRINGYTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(52, "Cannot access subscript of non-string value of type "), TYPENAME(PARENT->TYPEPTR)), str_make(2, ": ")), DESCRIBEEXPR(PARENT, 10)));
  if (!ISINTEGERTYPE(SUBSCRIPT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(38, "Invalid type for subscript of string: "), TYPENAME(SUBSCRIPT->TYPEPTR)), str_make(2, ": ")), DESCRIBEEXPR(SUBSCRIPT, 10)));
  RESULT = _NEWEXPR(XCSTRINGCHAR);
  RESULT->ARRAYEXPR = EXTOSTRING(PARENT);
  RESULT->ARRAYINDEX = SUBSCRIPT;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTCHAR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ *EXFNREF(TPSFUNCTION *FNPTR) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCFNREF);
  RESULT->FNPTR = FNPTR;
  RESULT->TYPEPTR = (void*)0;
  return RESULT;
}

TEXPRESSIONOBJ *EXFUNCTIONCALL(TEXPRESSIONOBJ *FNEXPR, TEXFUNCTIONARGS *ARGS) {
  TEXPRESSIONOBJ *RESULT;
  int POS;
  TEXPRESSIONOBJ *FNCALL;
  PString TMPVARNUM;
  if (FNEXPR->CLS != XCFNREF) COMPILEERROR(str_make(24, "Cannot call non-function"));
  if (ARGS->SIZE != FNEXPR->FNPTR->ARGCOUNT) COMPILEERROR(cat_ss(str_make(37, "Wrong number of arguments in call to "), FNEXPR->FNPTR->NAME));
  FNEXPR->FNPTR->WASUSED = 1;
  FNCALL = _NEWEXPR(XCFNCALL);
  FNCALL->FNEXPR = FNEXPR;
  FNCALL->CALLARGS.SIZE = ARGS->SIZE;
  FNCALL->TYPEPTR = FNEXPR->FNPTR->RETURNTYPEPTR;
  FNCALL->ISASSIGNABLE = 0;
  FNCALL->ISFUNCTIONRESULT = 1;
  RESULT = FNCALL;
  do {
    int first = 1;
    int last = ARGS->SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          FNCALL->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1] = EXCOERCE(ARGS->VALUES[(int)subrange(POS, 1, 8) - 1], FNEXPR->FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1].TYPEPTR);
          if (FNEXPR->FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1].ISREFERENCE) {
            if (!FNCALL->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]->ISASSIGNABLE) {
              if (FNEXPR->FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1].ISCONSTANT) {
                STR_i(DEFCOUNTER(TCTTMPVAR), &TMPVARNUM);
                RESULT = EXSETTMPVAR(EXVARIABLE(ADDVARIABLE(MAKEVARIABLE(cat_ss(str_make(3, "tmp"), TMPVARNUM), FNEXPR->FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1].TYPEPTR))), FNCALL->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1], RESULT);
                FNCALL->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1] = COPYEXPR(RESULT->TMPVAR);
              }
              else COMPILEERROR(cat_ss(str_make(47, "Pass-by-reference argument must be assignable: "), DESCRIBEEXPR(FNCALL->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1], 10)));
            }
            else if (!FNEXPR->FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1].ISCONSTANT) EXMARKINITIALIZED(FNCALL->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]);
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return RESULT;
}

TEXPRESSIONOBJ *EXPSEUDOFN(TPSPSEUDOFN SPECIALFN) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCPSEUDOFNREF);
  RESULT->PSEUDOFN = SPECIALFN;
  return RESULT;
}

TEXPRESSIONOBJ *EXPSEUDOFNCALL(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  TPSPSEUDOFN FN;
  if (EXPR->CLS != XCPSEUDOFNREF) COMPILEERROR(cat_ss(str_make(47, "Internal error: Expected a pseudofunction, got "), DESCRIBEEXPR(EXPR, 10)));
  FN = EXPR->PSEUDOFN;
  EXPR->CLS = XCPSEUDOFNCALL;
  EXPR->PSEUDOFNCALL.PSEUDOFN = FN;
  EXPR->PSEUDOFNCALL.ARG1 = (void*)0;
  EXPR->PSEUDOFNCALL.ARG2 = (void*)0;
  EXPR->PSEUDOFNCALL.ARG3 = (void*)0;
  EXPR->PSEUDOFNCALL.READARGS = (void*)0;
  EXPR->PSEUDOFNCALL.WRITEARGS = (void*)0;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPIMM(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXUNOPCMP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);

TEXPRESSIONOBJ *EXUNARYOP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (OP == TKMINUS || OP == TKPLUS) {
    if (!ISNUMERICTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(17, "Invalid type for "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(PARENT->TYPEPTR)));
  }
  else if (OP == TKNOT) {
    if (!ISBOOLEANTYPE(PARENT->TYPEPTR) && !ISINTEGERTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(17, "Invalid type for "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(PARENT->TYPEPTR)));
  }
  else COMPILEERROR(cat_ss(str_make(24, "Invalid unary operator: "), LXTOKENNAME(OP)));
  if (EXISIMMEDIATE(PARENT)) RESULT = _EXUNOPIMM(PARENT, OP);
  else RESULT = _EXUNOPCMP(PARENT, OP);
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPIMM(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (OP == TKMINUS && EXISIMMEDIATEOFCLASS(PARENT, XICINTEGER)) PARENT->IMMEDIATE.INTEGERVAL = -PARENT->IMMEDIATE.INTEGERVAL;
  else if (OP == TKPLUS && EXISIMMEDIATEOFCLASS(PARENT, XICINTEGER)) ;
  else if (OP == TKMINUS && EXISIMMEDIATEOFCLASS(PARENT, XICREAL)) PARENT->IMMEDIATE.REALVAL = -PARENT->IMMEDIATE.REALVAL;
  else if (OP == TKPLUS && EXISIMMEDIATEOFCLASS(PARENT, XICREAL)) ;
  else if (OP == TKNOT && EXISIMMEDIATEOFCLASS(PARENT, XICBOOLEAN)) PARENT->IMMEDIATE.BOOLEANVAL = !PARENT->IMMEDIATE.BOOLEANVAL;
  else if (OP == TKNOT && EXISIMMEDIATEOFCLASS(PARENT, XICINTEGER)) PARENT->IMMEDIATE.INTEGERVAL = ~PARENT->IMMEDIATE.INTEGERVAL;
  else COMPILEERROR(str_make(49, "Internal error: invalid immediate unary operation"));
  RESULT = PARENT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPCMP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCUNARYOP);
  RESULT->UNARY.PARENT = PARENT;
  RESULT->UNARY.OP = OP;
  RESULT->TYPEPTR = PARENT->TYPEPTR;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPINTIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPNUMIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPSTRIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPENUMIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPBOOLCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPINTCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPNUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPSTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPENUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPPTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
int _EXBINOPSHORTCUT(TEXPRESSIONOBJ **LEFT, TEXPRESSIONOBJ **RIGHT, TLXTOKENID OP);

TEXPRESSIONOBJ *EXBINARYOP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int IMMEDIATE;
  LEFT = EXOUTRANGE(LEFT);
  RIGHT = EXOUTRANGE(RIGHT);
  IMMEDIATE = EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT);
  if (_EXBINOPSHORTCUT(&LEFT, &RIGHT, OP)) {
    RESULT = LEFT;
  }
  else if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) {
    if (IMMEDIATE) RESULT = _EXBINOPBOOLIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPBOOLCMP(LEFT, RIGHT, OP);
  }
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) {
    if (IMMEDIATE) RESULT = _EXBINOPINTIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPINTCMP(LEFT, RIGHT, OP);
  }
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) {
    if (IMMEDIATE) RESULT = _EXBINOPNUMIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPNUMCMP(LEFT, RIGHT, OP);
  }
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) {
    if (IMMEDIATE) RESULT = _EXBINOPSTRIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPSTRCMP(LEFT, RIGHT, OP);
  }
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) {
    if (IMMEDIATE) RESULT = _EXBINOPENUMIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPENUMCMP(LEFT, RIGHT, OP);
  }
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXBINOPPTRCMP(LEFT, RIGHT, OP);
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(27, "Type mismatch for operator "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(LEFT->TYPEPTR)), str_make(5, " and ")), TYPENAME(RIGHT->TYPEPTR)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int LT;
  int RT;
  LT = LEFT->IMMEDIATE.BOOLEANVAL;
  RT = RIGHT->IMMEDIATE.BOOLEANVAL;
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
  LEFT->IMMEDIATE.BOOLEANVAL = LT;
  LEFT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  LEFT->ISASSIGNABLE = 0;
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPINTIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int LT;
  int RT;
  int BO;
  LT = LEFT->IMMEDIATE.INTEGERVAL;
  RT = RIGHT->IMMEDIATE.INTEGERVAL;
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
        LEFT->IMMEDIATE.CLS = XICBOOLEAN;
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
  if (EXISIMMEDIATEOFCLASS(LEFT, XICINTEGER)) {
    LEFT->IMMEDIATE.INTEGERVAL = LT;
    LEFT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  }
  else {
    LEFT->IMMEDIATE.BOOLEANVAL = BO;
    LEFT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  }
  LEFT->ISASSIGNABLE = 0;
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPNUMIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  double LT;
  double RT;
  int BO;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  LT = LEFT->IMMEDIATE.REALVAL;
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  RT = RIGHT->IMMEDIATE.REALVAL;
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
    case TKSLASH:
      LT = LT / RT;
      break;
    default:
      {
        LEFT->IMMEDIATE.CLS = XICBOOLEAN;
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
            COMPILEERROR(cat_ss(str_make(23, "Invalid real operator: "), LXTOKENNAME(OP)));
            break;
        }
      }
      break;
  }
  if (EXISIMMEDIATEOFCLASS(LEFT, XICREAL)) {
    LEFT->IMMEDIATE.REALVAL = LT;
    LEFT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  }
  else {
    LEFT->IMMEDIATE.BOOLEANVAL = BO;
    LEFT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  }
  LEFT->ISASSIGNABLE = 0;
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPSTRIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  PString LT;
  PString RT;
  int BO;
  if (EXISIMMEDIATEOFCLASS(LEFT, XICCHAR)) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
  else LT = LEFT->IMMEDIATE.STRINGVAL;
  if (EXISIMMEDIATEOFCLASS(RIGHT, XICCHAR)) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
  else RT = RIGHT->IMMEDIATE.STRINGVAL;
  DISPOSEEXPR(&RIGHT);
  if (OP == TKPLUS) {
    LEFT->IMMEDIATE.CLS = XICSTRING;
    LT = cat_ss(LT, RT);
  }
  else {
    LEFT->IMMEDIATE.CLS = XICBOOLEAN;
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
  if (EXISIMMEDIATEOFCLASS(LEFT, XICSTRING)) {
    LEFT->IMMEDIATE.STRINGVAL = LT;
    LEFT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
  }
  else {
    LEFT->IMMEDIATE.BOOLEANVAL = BO;
    LEFT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  }
  LEFT->ISASSIGNABLE = 0;
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPENUMIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  int LT;
  int RT;
  int BO;
  LT = LEFT->IMMEDIATE.ENUMORDINAL;
  RT = RIGHT->IMMEDIATE.ENUMORDINAL;
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
  LEFT->IMMEDIATE.CLS = XICBOOLEAN;
  LEFT->IMMEDIATE.BOOLEANVAL = BO;
  LEFT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  LEFT->ISASSIGNABLE = 0;
  RESULT = LEFT;
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (OP == TKAND || OP == TKOR || OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) {
    RESULT = _NEWEXPR(XCBINARYOP);
    RESULT->BINARY.LEFT = LEFT;
    RESULT->BINARY.RIGHT = RIGHT;
    RESULT->BINARY.OP = OP;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    RESULT->ISASSIGNABLE = 0;
    RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  }
  else COMPILEERROR(cat_ss(str_make(26, "Invalid boolean operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

int _EXISZERO(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICINTEGER) && EXPR->IMMEDIATE.INTEGERVAL == 0 || EXISIMMEDIATEOFCLASS(EXPR, XICREAL) && (double)EXPR->IMMEDIATE.INTEGERVAL == 0;
  return RESULT;
}

int _EXISONE(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICINTEGER) && EXPR->IMMEDIATE.INTEGERVAL == 1 || EXISIMMEDIATEOFCLASS(EXPR, XICREAL) && (double)EXPR->IMMEDIATE.INTEGERVAL == 1;
  return RESULT;
}

int _EXISTRUE(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICBOOLEAN) && EXPR->IMMEDIATE.BOOLEANVAL;
  return RESULT;
}

int _EXISFALSE(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICBOOLEAN) && !EXPR->IMMEDIATE.BOOLEANVAL;
  return RESULT;
}

int _EXBINOPSHORTCUT(TEXPRESSIONOBJ **LEFT, TEXPRESSIONOBJ **RIGHT, TLXTOKENID OP) {
  int RESULT;
  RESULT = 0;
  if (OP == TKPLUS || OP == TKMINUS) {
    if (_EXISZERO(*LEFT)) {
      DISPOSEEXPR(LEFT);
      *LEFT = *RIGHT;
      RESULT = 1;
    }
    else if (_EXISZERO(*RIGHT)) {
      DISPOSEEXPR(RIGHT);
      RESULT = 1;
    }
  }
  else if (OP == TKASTERISK) {
    if (_EXISZERO(*LEFT) || _EXISZERO(*RIGHT)) {
      DISPOSEEXPR(LEFT);
      DISPOSEEXPR(RIGHT);
      *LEFT = EXINTEGERCONSTANT(0);
      RESULT = 1;
    }
    else if (_EXISONE(*LEFT)) {
      DISPOSEEXPR(LEFT);
      *LEFT = *RIGHT;
      RESULT = 1;
    }
    else if (_EXISONE(*RIGHT)) {
      DISPOSEEXPR(RIGHT);
      RESULT = 1;
    }
  }
  else if (OP == TKDIV || OP == TKSLASH) {
    if (_EXISZERO(*LEFT) || _EXISONE(*RIGHT)) {
      DISPOSEEXPR(RIGHT);
      RESULT = 1;
    }
  }
  else if (OP == TKAND) {
    if (_EXISFALSE(*LEFT)) {
      DISPOSEEXPR(LEFT);
      DISPOSEEXPR(RIGHT);
      *LEFT = EXBOOLEANCONSTANT(0);
      RESULT = 1;
    }
    else if (_EXISTRUE(*LEFT)) {
      DISPOSEEXPR(LEFT);
      *LEFT = *RIGHT;
      RESULT = 1;
    }
    else if (_EXISTRUE(*RIGHT)) {
      DISPOSEEXPR(RIGHT);
      RESULT = 1;
    }
  }
  else if (OP == TKOR) {
    if (_EXISTRUE(*LEFT)) {
      DISPOSEEXPR(LEFT);
      DISPOSEEXPR(RIGHT);
      *LEFT = EXBOOLEANCONSTANT(1);
      RESULT = 1;
    }
    else if (_EXISFALSE(*LEFT)) {
      DISPOSEEXPR(LEFT);
      *LEFT = *RIGHT;
      RESULT = 1;
    }
    else if (_EXISFALSE(*RIGHT)) {
      DISPOSEEXPR(RIGHT);
      RESULT = 1;
    }
  }
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPINTCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS || OP == TKMINUS || OP == TKASTERISK || OP == TKDIV || OP == TKMOD || OP == TKAND || OP == TKOR) RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(26, "Invalid integer operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPNUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RESULT->BINARY.RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS || OP == TKMINUS || OP == TKASTERISK || OP == TKSLASH) RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(23, "Invalid real operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPSTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKPLUS) RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
  else if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPENUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKEQUALS || OP == TKNOTEQUALS || OP == TKLESSTHAN || OP == TKMORETHAN || OP == TKLESSOREQUALS || OP == TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(23, "Invalid enum operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPPTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (OP == TKEQUALS || OP == TKNOTEQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  else COMPILEERROR(cat_ss(str_make(25, "Invalid string operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

int EXGETORDINAL(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  if (!EXISIMMEDIATE(EXPR)) COMPILEERROR(cat_ss(str_make(29, "Expression is not immediate: "), DESCRIBEEXPR(EXPR, 5)));
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICBOOLEAN:
        RESULT = (int)with1->BOOLEANVAL;
        break;
      case XICINTEGER:
        RESULT = with1->INTEGERVAL;
        break;
      case XICCHAR:
        RESULT = (int)with1->CHARVAL;
        break;
      case XICENUM:
        RESULT = with1->ENUMORDINAL;
        break;
      default:
        COMPILEERROR(cat_ss(str_make(47, "Expression does not belong to an ordinal type: "), DESCRIBEEXPR(EXPR, 5)));
        break;
    }
  }
  return RESULT;
}

TEXPRESSIONOBJ *EXSUBRANGE(TEXPRESSIONOBJ *PARENT, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  int ORDINAL;
  if (EXISIMMEDIATE(PARENT)) {
    ORDINAL = EXGETORDINAL(PARENT);
    if (ORDINAL < TYPEPTR->RANGEPTR->FIRST || ORDINAL > TYPEPTR->RANGEPTR->LAST) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(6, "Value "), DESCRIBEEXPR(PARENT, 2)), str_make(19, " out of bounds for ")), TYPENAME(TYPEPTR)));
    PARENT->TYPEPTR = TYPEPTR;
    RESULT = PARENT;
  }
  else {
    RESULT = _NEWEXPR(XCSUBRANGE);
    RESULT->SUBRANGEPARENT = PARENT;
    RESULT->TYPEPTR = TYPEPTR;
  }
  return RESULT;
}

TEXPRESSIONOBJ *EXRERANGE(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  if (TYPEPTR->RANGEPTR->FIRST <= EXPR->TYPEPTR->RANGEPTR->FIRST && EXPR->TYPEPTR->RANGEPTR->LAST <= TYPEPTR->RANGEPTR->LAST) {
    EXPR->TYPEPTR = TYPEPTR;
    RESULT = EXPR;
  }
  else RESULT = EXSUBRANGE(EXOUTRANGE(EXPR), TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ *EXCOERCE(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  if (ISRANGETYPE(EXPR->TYPEPTR) && ISSAMETYPE(TYPEPTR, EXPR->TYPEPTR->RANGEPTR->BASETYPEPTR)) RESULT = EXOUTRANGE(EXPR);
  else if (ISRANGETYPE(TYPEPTR) && ISSAMETYPE(TYPEPTR->RANGEPTR->BASETYPEPTR, EXPR->TYPEPTR)) RESULT = EXSUBRANGE(EXPR, TYPEPTR);
  else if (ISRANGETYPE(EXPR->TYPEPTR) && ISRANGETYPE(TYPEPTR) && ISSAMETYPE(EXPR->TYPEPTR->RANGEPTR->BASETYPEPTR, TYPEPTR->RANGEPTR->BASETYPEPTR)) RESULT = EXRERANGE(EXPR, TYPEPTR);
  else if (ISCHARTYPE(EXPR->TYPEPTR) && ISSTRINGTYPE(TYPEPTR)) RESULT = EXTOSTRING(EXPR);
  else if (ISINTEGERTYPE(EXPR->TYPEPTR) && ISREALTYPE(TYPEPTR)) RESULT = EXTOREAL(EXPR);
  else if (ISSAMETYPE(EXPR->TYPEPTR, TYPEPTR)) RESULT = EXPR;
  else if (ISNILTYPE(EXPR->TYPEPTR) && ISPOINTERYTYPE(TYPEPTR)) RESULT = EXPR;
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(24, "Type mismatch: expected "), TYPENAME(TYPEPTR)), str_make(6, ", got ")), TYPENAME(EXPR->TYPEPTR)));
  return RESULT;
}

void EXMARKINITIALIZED(TEXPRESSIONOBJ *LHS) {
  int ISTERMINAL;
  ISTERMINAL = 0;
  while (!ISTERMINAL) switch (LHS->CLS) {
    case XCFIELD:
      LHS = LHS->RECEXPR;
      break;
    case XCARRAY:
      LHS = LHS->ARRAYEXPR;
      break;
    case XCSTRINGCHAR:
      LHS = LHS->STRINGEXPR;
      break;
    default:
      ISTERMINAL = 1;
      break;
  }
  if (LHS->CLS == XCVARIABLE) LHS->VARPTR->WASINITIALIZED = 1;
}

void READTOKEN();

void WANTTOKEN(TLXTOKENID ID) {
  if (LEXER.TOKEN.ID != ID) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID)), str_make(8, ", found ")), LXTOKENSTR()));
}

void WANTTOKEN2(TLXTOKENID ID1, TLXTOKENID ID2) {
  if (LEXER.TOKEN.ID != ID1 && LEXER.TOKEN.ID != ID2) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID1)), str_make(4, " or ")), LXTOKENNAME(ID2)), str_make(8, ", found ")), LXTOKENSTR()));
}

void WANTTOKEN3(TLXTOKENID ID1, TLXTOKENID ID2, TLXTOKENID ID3) {
  if (LEXER.TOKEN.ID != ID1 && LEXER.TOKEN.ID != ID2 && LEXER.TOKEN.ID != ID3) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID1)), str_make(2, ", ")), LXTOKENNAME(ID2)), str_make(5, ", or ")), LXTOKENNAME(ID3)), str_make(8, ", found ")), LXTOKENSTR()));
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
  RESULT = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 1)->TYPEPTR;
  RESULT->WASUSED = 1;
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
    ENUM.VALUES[subrange(ENUM.SIZE - 1, 0, 127)] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRPAREN));
  TYP = EMPTYTYPE();
  TYP.CLS = TTCENUM;
  TYP.ENUMPTR = ADDENUM(ENUM);
  TYP.WASUSED = 1;
  RESULT = ADDTYPE(TYP);
  SKIPTOKEN(TKRPAREN);
  return RESULT;
}

void PSRECORDFIELD(TPSRECORDDEF *REC, TLXTOKENID DELIMITER) {
  PString NAME;
  int LASTFIELD;
  int FIELD;
  TPSTYPE *TYPEPTR;
  LASTFIELD = REC->SIZE;
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    do {
      int first = 1;
      int last = REC->SIZE;
      if (first <= last) {
        FIELD = first;
        while (1) {
          {
            if (cmp_ss(REC->FIELDS[(int)subrange(FIELD, 1, 32) - 1].NAME, NAME) == 0) COMPILEERROR(cat_ss(cat_ss(str_make(14, "A field named "), NAME), str_make(25, " has already been defined")));
          }
          if (FIELD == last) break;
          ++FIELD;
        }
      }
    } while(0);
    REC->SIZE = REC->SIZE + 1;
    if (REC->SIZE > 32) COMPILEERROR(str_make(25, "Too many fields in record"));
    REC->FIELDS[(int)subrange(REC->SIZE, 1, 32) - 1].NAME = NAME;
    WANTTOKEN2(TKCOMMA, TKCOLON);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKCOLON));
  WANTTOKENANDREAD(TKCOLON);
  TYPEPTR = PSTYPEDENOTER();
  do {
    int first = LASTFIELD + 1;
    int last = REC->SIZE;
    if (first <= last) {
      FIELD = first;
      while (1) {
        REC->FIELDS[(int)subrange(FIELD, 1, 32) - 1].TYPEPTR = TYPEPTR;
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
    REC->SIZE = REC->SIZE + 1;
    REC->FIELDS[(int)subrange(REC->SIZE, 1, 32) - 1].NAME = TAG.NAME;
    REC->FIELDS[(int)subrange(REC->SIZE, 1, 32) - 1].TYPEPTR = TAGTYPE;
  }
  else TAGTYPE = FINDNAMEOFCLASS(TAG.NAME, TNCTYPE, 1)->TYPEPTR;
  if (!ISORDINALTYPE(TAGTYPE)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
  WANTTOKENANDREAD(TKOF);
  do {
    REC->NUMVARIANTS = REC->NUMVARIANTS + 1;
    REC->VARIANTBOUNDS[(int)subrange(REC->NUMVARIANTS, 1, 32) - 1] = REC->SIZE + 1;
    do {
      CASELABEL = EXCOERCE(PSEXPRESSION(), TAGTYPE);
      if (!EXISIMMEDIATE(CASELABEL)) COMPILEERROR(str_make(48, "The label of the case statement is not immediate"));
      DISPOSEEXPR(&CASELABEL);
      WANTTOKEN2(TKCOMMA, TKCOLON);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    WANTTOKENANDREAD(TKCOLON);
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) PSRECORDFIELD(REC, TKRPAREN);
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
  TYP.RECPTR = ADDRECORD(REC);
  RESULT = ADDTYPE(TYP);
  return RESULT;
}

TEXPRESSIONOBJ *PSIMMEDIATE() {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *EXPR;
  EXPR = PSEXPRESSION();
  if (!EXISIMMEDIATE(EXPR)) COMPILEERROR(str_make(32, "Expected an immediate expression"));
  RESULT = EXPR;
  return RESULT;
}

TPSTYPE *PSARRAYTYPE() {
  TPSTYPE *RESULT;
  TPSTYPE TYP;
  TPSARRAYDEF ARR;
  TPSTYPE *TYPEPTR;
  TPSARRAYDEF *ARRPTR;
  ARR.INDEXTYPEPTR = (void*)0;
  ARR.VALUETYPEPTR = (void*)0;
  ARRPTR = ADDARRAY(ARR);
  TYP = TYPEOFCLASS(TTCARRAY);
  TYP.ARRAYPTR = ARRPTR;
  TYPEPTR = ADDTYPE(TYP);
  RESULT = TYPEPTR;
  WANTTOKENANDREAD(TKARRAY);
  WANTTOKENANDREAD(TKLBRACKET);
  do {
    ARRPTR->INDEXTYPEPTR = PSTYPEDENOTER();
    ARRPTR->INDEXTYPEPTR->WASUSED = 1;
    if (!ISBOUNDEDTYPE(ARRPTR->INDEXTYPEPTR)) COMPILEERROR(str_make(51, "Array indices must belong to a bounded ordinal type"));
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    if (LEXER.TOKEN.ID == TKCOMMA) {
      TYPEPTR = ADDTYPE(TYP);
      ARRPTR->VALUETYPEPTR = TYPEPTR;
      ARRPTR->VALUETYPEPTR->WASUSED = 1;
      ARRPTR = ADDARRAY(ARR);
      TYPEPTR->ARRAYPTR = ARRPTR;
    }
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRBRACKET));
  WANTTOKENANDREAD(TKRBRACKET);
  WANTTOKENANDREAD(TKOF);
  ARRPTR->VALUETYPEPTR = PSTYPEDENOTER();
  return RESULT;
}

TPSTYPE *PSPOINTERTYPE() {
  TPSTYPE *RESULT;
  TPSTYPE TYP;
  TPSNAME *NAMEPTR;
  WANTTOKENANDREAD(TKCARET);
  WANTTOKEN(TKIDENTIFIER);
  NAMEPTR = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 0);
  if (NAMEPTR == (void*)0) TYP = POINTERUNKNOWNTYPE(LEXER.TOKEN.VALUE);
  else {
    TYP = POINTERTYPE(NAMEPTR->TYPEPTR);
    NAMEPTR->TYPEPTR->WASUSED = 1;
  }
  READTOKEN();
  RESULT = ADDTYPE(TYP);
  return RESULT;
}

TPSTYPE *PSRANGETYPE() {
  TPSTYPE *RESULT;
  TEXPRESSIONOBJ *FIRST;
  TEXPRESSIONOBJ *LAST;
  TPSTYPE TYP;
  TPSRANGEDEF RANGE;
  FIRST = PSIMMEDIATE();
  WANTTOKENANDREAD(TKRANGE);
  LAST = PSIMMEDIATE();
  if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) COMPILEERROR(str_make(53, "The bounds of a subrange must belong to the same type"));
  if (!ISORDINALTYPE(FIRST->TYPEPTR)) COMPILEERROR(str_make(55, "The bounds of a subrange must belong to an ordinal type"));
  RANGE.FIRST = EXGETORDINAL(FIRST);
  RANGE.LAST = EXGETORDINAL(LAST);
  RANGE.BASETYPEPTR = FIRST->TYPEPTR;
  DISPOSEEXPR(&FIRST);
  DISPOSEEXPR(&LAST);
  if (RANGE.FIRST > RANGE.LAST) COMPILEERROR(str_make(51, "The bounds of a subrange must be in ascending order"));
  TYP = TYPEOFCLASS(TTCRANGE);
  TYP.RANGEPTR = ADDRANGE(RANGE);
  RESULT = ADDTYPE(TYP);
  return RESULT;
}

TPSTYPE *PSTYPEDENOTER() {
  TPSTYPE *RESULT;
  TPSNAME *IDX;
  RESULT = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) RESULT = PSENUMERATEDTYPE();
  else if (LEXER.TOKEN.ID == TKRECORD) RESULT = PSRECORDTYPE();
  else if (LEXER.TOKEN.ID == TKARRAY) RESULT = PSARRAYTYPE();
  else if (LEXER.TOKEN.ID == TKCARET) RESULT = PSPOINTERTYPE();
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) {
    IDX = FINDNAME(LEXER.TOKEN.VALUE, 0);
    if (IDX == (void*)0) ;
    else if (IDX->CLS == TNCTYPE) RESULT = PSTYPEIDENTIFIER();
    else if (IDX->CLS == TNCCONSTANT || IDX->CLS == TNCENUMVAL) RESULT = PSRANGETYPE();
  }
  else if (LEXER.TOKEN.ID == TKINTEGER || LEXER.TOKEN.ID == TKSTRING || LEXER.TOKEN.ID == TKMINUS || LEXER.TOKEN.ID == TKPLUS) RESULT = PSRANGETYPE();
  if (RESULT == (void*)0) COMPILEERROR(cat_ss(str_make(29, "Expected type denoter, found "), LXTOKENSTR()));
  return RESULT;
}

void _RESOLVEPOINTERUNKNOWN(TPSTYPE *TYPEPTR) {
  TPSTYPE *TARGETPTR;
  if (ISPOINTERUNKNOWNTYPE(TYPEPTR)) {
    TARGETPTR = FINDNAMEOFCLASS(*TYPEPTR->TARGETNAME, TNCTYPE, 1)->TYPEPTR;
    free(TYPEPTR->TARGETNAME);
    TYPEPTR->CLS = TTCPOINTER;
    TYPEPTR->POINTEDTYPEPTR = TARGETPTR;
    TARGETPTR->WASUSED = 1;
  }
}

void PSTYPEDEFINITIONS() {
  PString NAME;
  TPSTYPE *TYPEPTR;
  TPSTYPE NEWTYPE;
  TPSDEFENTRY *CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKTYPE);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKENANDREAD(TKEQUALS);
    TYPEPTR = PSTYPEDENOTER();
    NEWTYPE = COPYTYPE(TYPEPTR);
    NEWTYPE.NAME = NAME;
    NEWTYPE.ALIASFOR = TYPEPTR;
    TYPEPTR = ADDTYPE(NEWTYPE);
    WANTTOKENANDREAD(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID != TKIDENTIFIER));
  OUTTYPEDEFINITIONSFROMCHECKPOINT(CHECKPOINT);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSCONSTANT(PString NAME) {
  TPSCONSTANT CONSTANT;
  WANTTOKENANDREAD(TKEQUALS);
  CONSTANT.NAME = NAME;
  CONSTANT.VALUE = PSIMMEDIATE();
  ADDCONSTANT(CONSTANT);
}

void PSCONSTANTVALUE(TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *EXPR;
  if (ISARRAYTYPE(TYPEPTR)) {
    WANTTOKENANDREAD(TKLPAREN);
    TYPEPTR = TYPEPTR->ARRAYPTR->VALUETYPEPTR;
    OUTCONSTANTARRAYBEGIN();
    while (LEXER.TOKEN.ID != TKRPAREN) {
      PSCONSTANTVALUE(TYPEPTR);
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      if (LEXER.TOKEN.ID == TKCOMMA) OUTCONSTANTARRAYSEPARATOR();
      SKIPTOKEN(TKCOMMA);
    }
    OUTCONSTANTARRAYEND();
    WANTTOKENANDREAD(TKRPAREN);
  }
  else {
    EXPR = EXCOERCE(PSIMMEDIATE(), TYPEPTR);
    OUTEXPRESSION(EXPR);
    DISPOSEEXPR(&EXPR);
  }
}

void PSTYPEDCONSTANT(PString NAME) {
  TPSTYPE *TYPEPTR;
  WANTTOKENANDREAD(TKCOLON);
  TYPEPTR = PSTYPEDENOTER();
  WANTTOKENANDREAD(TKEQUALS);
  OUTCONSTANTDEFINITIONBEGIN(ADDVARIABLE(MAKETYPEDCONSTANT(NAME, TYPEPTR)));
  PSCONSTANTVALUE(TYPEPTR);
  OUTCONSTANTDEFINITIONEND();
}

void PSCONSTDEFINITIONS() {
  PString NAME;
  TPSDEFENTRY *CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKCONST);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKEQUALS, TKCOLON);
    if (LEXER.TOKEN.ID == TKEQUALS) PSCONSTANT(NAME);
    else PSTYPEDCONSTANT(NAME);
    WANTTOKENANDREAD(TKSEMICOLON);
  } while (!(LEXER.TOKEN.ID != TKIDENTIFIER));
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSVARDEFINITIONS() {
  int NUMNAMES;
  PString NAMES[8];
  TPSTYPE *TYPEPTR;
  TPSDEFENTRY *CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKVAR);
  do {
    NUMNAMES = 0;
    do {
      NUMNAMES = NUMNAMES + 1;
      if (NUMNAMES > 8) COMPILEERROR(str_make(37, "Too many names in variable definition"));
      NAMES[(int)subrange(NUMNAMES, 1, 8) - 1] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      WANTTOKEN2(TKCOMMA, TKCOLON);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    WANTTOKENANDREAD(TKCOLON);
    TYPEPTR = PSTYPEDENOTER();
    WANTTOKENANDREAD(TKSEMICOLON);
    do {
      int first = 1;
      int last = NUMNAMES;
      if (first <= last) {
        NUMNAMES = first;
        while (1) {
          OUTVARIABLEDEFINITION(ADDVARIABLE(MAKEVARIABLE(NAMES[(int)subrange(NUMNAMES, 1, 8) - 1], TYPEPTR)));
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

void PSFUNCTIONBODY(TPSFUNCTION *FNPTR) {
  int POS;
  TPSDEFENTRY *CHECKPOINT;
  TPSVARIABLE *RESULTPTR;
  STARTLOCALSCOPE(FNPTR);
  CHECKPOINT = DEFS.LATEST;
  do {
    int first = 1;
    int last = FNPTR->ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        ADDVARIABLE(FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1]);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  OUTFUNCTIONDEFINITION(FNPTR);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
  if (FNPTR->RETURNTYPEPTR != (void*)0) {
    RESULTPTR = ADDVARIABLE(MAKEVARIABLE(str_make(6, "RESULT"), FNPTR->RETURNTYPEPTR));
    RESULTPTR->WASUSED = 1;
    OUTVARIABLEDEFINITION(RESULTPTR);
  }
  PSDEFINITIONS();
  WANTTOKENANDREAD(TKBEGIN);
  while (LEXER.TOKEN.ID != TKEND) {
    PSSTATEMENT();
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  }
  WANTTOKENANDREAD(TKEND);
  WANTTOKENANDREAD(TKSEMICOLON);
  OUTFUNCTIONEND(FNPTR);
  CLOSELOCALSCOPE();
}

void PSARGUMENTS(TPSFUNCTION *DEF) {
  int ISCONST;
  int ISVAR;
  int LASTARG;
  int ARG;
  TPSTYPE *TYPEPTR;
  WANTTOKENANDREAD(TKLPAREN);
  DEF->ARGCOUNT = 0;
  do {
    ISCONST = LEXER.TOKEN.ID == TKCONST;
    ISVAR = LEXER.TOKEN.ID == TKVAR;
    if (ISCONST) WANTTOKENANDREAD(TKCONST);
    if (ISVAR) WANTTOKENANDREAD(TKVAR);
    LASTARG = DEF->ARGCOUNT;
    do {
      DEF->ARGCOUNT = DEF->ARGCOUNT + 1;
      if (DEF->ARGCOUNT > 8) COMPILEERROR(cat_ss(str_make(41, "Too many arguments declared for function "), DEF->NAME));
      DEF->ARGS[(int)subrange(DEF->ARGCOUNT, 1, 8) - 1].NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      DEF->ARGS[(int)subrange(DEF->ARGCOUNT, 1, 8) - 1].ISCONSTANT = ISCONST;
      DEF->ARGS[(int)subrange(DEF->ARGCOUNT, 1, 8) - 1].ISREFERENCE = ISVAR || ISCONST;
      DEF->ARGS[(int)subrange(DEF->ARGCOUNT, 1, 8) - 1].WASINITIALIZED = 1;
      WANTTOKEN2(TKCOLON, TKCOMMA);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    SKIPTOKEN(TKCOLON);
    TYPEPTR = PSTYPEIDENTIFIER();
    do {
      int first = LASTARG + 1;
      int last = DEF->ARGCOUNT;
      if (first <= last) {
        ARG = first;
        while (1) {
          DEF->ARGS[(int)subrange(ARG, 1, 8) - 1].TYPEPTR = TYPEPTR;
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
  if (LEXER.TOKEN.ID == TKSEMICOLON && HASFORWARDDECLARATION(DEF.NAME)) DEF.RETURNTYPEPTR = (void*)0;
  else {
    WANTTOKEN2(TKLPAREN, TKCOLON);
    if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&DEF);
    WANTTOKENANDREAD(TKCOLON);
    DEF.RETURNTYPEPTR = PSRESULTTYPE();
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

TEXPRESSIONOBJ *PSFUNCTIONCALL(TEXPRESSIONOBJ *FN) {
  TEXPRESSIONOBJ *RESULT;
  TEXFUNCTIONARGS ARGS;
  if (FN->CLS == XCFNREF) {
    ARGS.SIZE = 0;
    if (LEXER.TOKEN.ID == TKLPAREN) {
      WANTTOKENANDREAD(TKLPAREN);
      while (LEXER.TOKEN.ID != TKRPAREN) {
        ARGS.SIZE = ARGS.SIZE + 1;
        ARGS.VALUES[(int)subrange(ARGS.SIZE, 1, 8) - 1] = PSEXPRESSION();
        WANTTOKEN2(TKCOMMA, TKRPAREN);
        SKIPTOKEN(TKCOMMA);
      }
      WANTTOKENANDREAD(TKRPAREN);
    }
    RESULT = EXFUNCTIONCALL(FN, &ARGS);
  }
  else if (FN->CLS == XCPSEUDOFNREF) RESULT = PF_PARSE(FN);
  return RESULT;
}

TEXPRESSIONOBJ *PSARRAYACCESS(TEXPRESSIONOBJ *ARR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *IDX;
  WANTTOKENANDREAD(TKLBRACKET);
  do {
    IDX = PSEXPRESSION();
    if (ISSTRINGYTYPE(ARR->TYPEPTR)) ARR = EXSTRINGCHAR(ARR, IDX);
    else ARR = EXARRAYACCESS(ARR, IDX);
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    if (LEXER.TOKEN.ID == TKCOMMA && !ISARRAYTYPE(ARR->TYPEPTR) && !ISSTRINGYTYPE(ARR->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(41, "Array element is not an array or string: "), DESCRIBEEXPR(ARR, 10)), str_make(14, "; it has type ")), TYPENAME(ARR->TYPEPTR)));
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRBRACKET));
  WANTTOKENANDREAD(TKRBRACKET);
  RESULT = ARR;
  return RESULT;
}

TEXPRESSIONOBJ *PSFIELDACCESS(TEXPRESSIONOBJ *REC) {
  TEXPRESSIONOBJ *RESULT;
  TPSIDENTIFIER FLD;
  WANTTOKENANDREAD(TKDOT);
  FLD = PSIDENTIFIER();
  RESULT = EXFIELDACCESS(REC, FINDFIELD(REC->TYPEPTR, FLD.NAME, 1));
  return RESULT;
}

TEXPRESSIONOBJ *PSVARIABLE() {
  TEXPRESSIONOBJ *RESULT;
  TPSIDENTIFIER ID;
  TPSWITHVAR *WITHVARPTR;
  TPSNAME FOUND;
  TEXPRESSIONOBJ *EXPR;
  ID = PSIDENTIFIER();
  WITHVARPTR = FINDWITHVAR(ID.NAME);
  if (WITHVARPTR != (void*)0) {
    EXPR = EXVARIABLE(WITHVARPTR->VARPTR);
    EXPR = EXFIELDACCESS(EXPR, FINDFIELD(EXPR->TYPEPTR, ID.NAME, 1));
  }
  else {
    FOUND = *FINDNAME(ID.NAME, 1);
    if (FOUND.CLS == TNCVARIABLE) EXPR = EXVARIABLE(FOUND.VARPTR);
    else if (FOUND.CLS == TNCCONSTANT) EXPR = COPYEXPR(FOUND.CONSTPTR->VALUE);
    else if (FOUND.CLS == TNCFUNCTION) EXPR = EXFNREF(FOUND.FNPTR);
    else if (FOUND.CLS == TNCENUMVAL) EXPR = EXENUMCONSTANT(FOUND.ORDINAL, FOUND.ENUMTYPEPTR);
    else if (FOUND.CLS == TNCPSEUDOFN) EXPR = EXPSEUDOFN(FOUND.PSEUDOFN);
    else COMPILEERROR(cat_ss(str_make(20, "Invalid identifier: "), ID.NAME));
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *PSVARIABLEORFUNCTIONEXTENSION(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  int DONE;
  if (EXPR->CLS == XCVARIABLE && (LEXER.TOKEN.ID == TKDOT || LEXER.TOKEN.ID == TKLBRACKET || LEXER.TOKEN.ID == TKCARET)) EXPR->VARPTR->WASUSED = 1;
  DONE = 0;
  do {
    if (EXPR->CLS == XCFNREF || EXPR->CLS == XCPSEUDOFNREF) EXPR = PSFUNCTIONCALL(EXPR);
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
  enum enum9 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, NUMCHARREADY, CARET, ERROR, DONE } STATE;
  int POS;
  unsigned char CH;
  int CHNUM;
  const char* enumvalues9[] = { "NONE", "QUOTEDSTR", "HASH", "NUMCHARDEC", "NUMCHARHEX", "NUMCHARREADY", "CARET", "ERROR", "DONE" };
  RESULT = str_make(0, "");
  STATE = NONE;
  POS = 1;
  while (POS <= LENGTH(&PSTR)) {
    CH = PSTR.chr[POS];
    if (STATE == NONE) {
      POS = POS + 1;
      if (CH == '\'') {
        STATE = QUOTEDSTR;
        if (POS > 2 && PSTR.chr[POS - 2] == '\'') RESULT = cat_sc(RESULT, '\'');
      }
      else if (CH == '#') STATE = HASH;
      else if (CH == '^') STATE = CARET;
      else STATE = ERROR;
    }
    else if (STATE == QUOTEDSTR) {
      POS = POS + 1;
      if (CH == '\'') STATE = NONE;
      else RESULT = cat_sc(RESULT, CH);
    }
    else if (STATE == HASH) {
      CHNUM = 0;
      if (LXISDIGIT(CH)) STATE = NUMCHARDEC;
      else if (CH == '$') {
        STATE = NUMCHARHEX;
        POS = POS + 1;
      }
    }
    else if (STATE == NUMCHARDEC) {
      if (LXISDIGIT(CH)) {
        POS = POS + 1;
        CHNUM = CHNUM * 10 + (int)CH - 48;
      }
      else STATE = NUMCHARREADY;
    }
    else if (STATE == NUMCHARHEX) {
      if (LXISHEXDIGIT(CH)) {
        POS = POS + 1;
        if (CH <= '9') CHNUM = CHNUM * 16 + (int)CH - 48;
        else if (CH <= 'F') CHNUM = CHNUM * 16 + (int)CH - 55;
        else if (CH <= 'f') CHNUM = CHNUM * 16 + (int)CH - 87;
      }
      else STATE = NUMCHARREADY;
    }
    else if (STATE == NUMCHARREADY) {
      RESULT = cat_sc(RESULT, CHR(CHNUM));
      STATE = NONE;
    }
    else if (STATE == CARET) {
      POS = POS + 1;
      STATE = NONE;
      if (CH >= '@' && CH <= '_') RESULT = cat_sc(RESULT, CHR((int)CH - 64));
      else if (CH >= 'a' && CH <= 'z') RESULT = cat_sc(RESULT, CHR((int)CH - 96));
      else STATE = ERROR;
    }
    else if (STATE == ERROR) COMPILEERROR(cat_ss(str_make(29, "Invalid character in string: "), PSTR));
  }
  if (STATE == QUOTEDSTR || STATE == CARET) COMPILEERROR(cat_ss(str_make(26, "String is not terminated: "), PSTR));
  if (STATE == NUMCHARDEC || STATE == NUMCHARHEX) RESULT = cat_sc(RESULT, CHR(CHNUM));
  return RESULT;
}

int PARSEINT(PString PSTR) {
  int RESULT;
  int CODE;
  VAL_i(&PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(cat_ss(str_make(25, "Could not parse integer: "), PSTR));
  return RESULT;
}

double PARSEREAL(PString PSTR) {
  double RESULT;
  int CODE;
  VAL_r(&PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(cat_ss(str_make(29, "Could not parse real number: "), PSTR));
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
    if (LENGTH(&STR) == 1) EXPR = EXCHARCONSTANT(STR.chr[1]);
    else EXPR = EXSTRINGCONSTANT(STR);
  }
  else if (LEXER.TOKEN.ID == TKINTEGER) EXPR = EXINTEGERCONSTANT(PARSEINT(GETTOKENVALUEANDREAD(TKINTEGER)));
  else if (LEXER.TOKEN.ID == TKREAL) EXPR = EXREALCONSTANT(PARSEREAL(GETTOKENVALUEANDREAD(TKREAL)));
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) {
    EXPR = PSVARIABLE();
    if (EXPR->CLS == XCVARIABLE) EXPR->VARPTR->WASUSED = 1;
    EXPR = PSVARIABLEORFUNCTIONEXTENSION(EXPR);
  }
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
  else SKIPTOKEN(TKPLUS);
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
  TPSVARIABLE *RESULTVARPTR;
  if (LHS->CLS == XCFNREF) {
    if (LHS->FNPTR != DEFS.CURRENTFN) COMPILEERROR(str_make(35, "Cannot assign a value to a function"));
    RESULTVARPTR = FINDNAMEOFCLASS(str_make(6, "RESULT"), TNCVARIABLE, 1)->VARPTR;
    DISPOSEEXPR(&LHS);
    LHS = EXVARIABLE(RESULTVARPTR);
  }
  RHS = EXCOERCE(RHS, LHS->TYPEPTR);
  if (!LHS->ISASSIGNABLE) {
    if (LHS->ISFUNCTIONRESULT) COMPILEERROR(cat_ss(str_make(43, "Cannot assign to the result of a function: "), DESCRIBEEXPR(LHS, 5)));
    else COMPILEERROR(cat_ss(str_make(28, "Cannot assign to a constant:"), DESCRIBEEXPR(LHS, 5)));
  }
  EXMARKINITIALIZED(LHS);
  OUTASSIGN(LHS, RHS);
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
  TEXPRESSIONOBJ *ORIGLHS;
  int USESTMPVARS;
  LHS = PSVARIABLE();
  if (LHS->CLS != XCFNREF || LHS->FNPTR != DEFS.CURRENTFN || LEXER.TOKEN.ID != TKASSIGN) LHS = PSVARIABLEORFUNCTIONEXTENSION(LHS);
  if (LEXER.TOKEN.ID == TKASSIGN) {
    WANTTOKENANDREAD(TKASSIGN);
    PSASSIGN(LHS, PSEXPRESSION());
  }
  else {
    ORIGLHS = LHS;
    USESTMPVARS = 0;
    while (LHS->CLS == XCSETTMPVAR) {
      if (!USESTMPVARS) {
        USESTMPVARS = 1;
        STARTTEMPORARYSCOPE();
        OUTSEQUENCEBEGIN();
      }
      OUTDECLAREANDASSIGN(LHS->TMPVAR->VARPTR, LHS->TMPVARVALUE);
      LHS = LHS->TMPVARCHILD;
    }
    if (LHS->CLS == XCFNCALL) OUTPROCEDURECALL(LHS);
    else if (LHS->CLS == XCPSEUDOFNCALL) OUTPSEUDOPROCCALL(LHS);
    else if (LHS->CLS == XCBINARYOP) COMPILEERROR(str_make(56, "Invalid statement (maybe you wrote '=' instead of ':='?)"));
    else COMPILEERROR(str_make(17, "Invalid statement"));
    DISPOSEEXPR(&ORIGLHS);
    if (USESTMPVARS) {
      OUTSEQUENCEEND();
      CLOSETEMPORARYSCOPE();
    }
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
  TEXPRESSIONOBJ *CASEPTR;
  TPSTYPE *CASETYPEPTR;
  TEXPRESSIONOBJ *CASELABEL;
  WANTTOKENANDREAD(TKCASE);
  CASEPTR = PSEXPRESSION();
  CASETYPEPTR = CASEPTR->TYPEPTR;
  if (!ISORDINALTYPE(CASETYPEPTR)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
  OUTCASEBEGIN(CASEPTR);
  DISPOSEEXPR(&CASEPTR);
  WANTTOKENANDREAD(TKOF);
  do {
    CASELABEL = EXCOERCE(PSEXPRESSION(), CASETYPEPTR);
    if (!EXISIMMEDIATE(CASELABEL)) COMPILEERROR(str_make(48, "The label of the case statement is not immediate"));
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
  if (ITER->ISFUNCTIONRESULT) COMPILEERROR(str_make(45, "Iterator must not be the result of a function"));
  if (!ISORDINALTYPE(ITER->TYPEPTR)) COMPILEERROR(cat_ss(str_make(33, "Type of iterator is not ordinal: "), TYPENAME(ITER->TYPEPTR)));
  if (ITER->CLS == XCVARIABLE) {
    ITER->VARPTR->WASINITIALIZED = 1;
    ITER->VARPTR->WASUSED = 1;
  }
  WANTTOKENANDREAD(TKASSIGN);
  FIRST = EXCOERCE(PSEXPRESSION(), ITER->TYPEPTR);
  WANTTOKEN2(TKTO, TKDOWNTO);
  ASCENDING = LEXER.TOKEN.ID == TKTO;
  READTOKEN();
  LAST = EXCOERCE(PSEXPRESSION(), ITER->TYPEPTR);
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
  TPSVARIABLE *VARPTR;
  WANTTOKEN(TKWITH);
  STARTTEMPORARYSCOPE();
  OUTSEQUENCEBEGIN();
  do {
    READTOKEN();
    BASE = PSEXPRESSION();
    VARPTR = ADDWITHVAR(BASE);
    OUTDECLAREANDASSIGN(VARPTR, BASE);
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
  if (LENGTH(&DIR) > 3 && DIR.chr[2] == 'I' && DIR.chr[3] == ' ') LXINCLUDE(COPY(&DIR, 4, 255));
  else if (LENGTH(&DIR) == 3 && DIR.chr[2] == 'R' && (DIR.chr[3] == '-' || DIR.chr[3] == '+')) CODEGENSETCHECKBOUNDS(DIR.chr[3] == '+');
}

void READTOKEN() {
  int STOP;
  do {
    LXREADTOKEN();
    STOP = LEXER.TOKEN.ID != TKCOMMENT;
    if (LEXER.TOKEN.ID == TKCOMMENT) if (LENGTH(&LEXER.TOKEN.VALUE) >= 2 && LEXER.TOKEN.VALUE.chr[1] == '$') EXECUTEDIRECTIVE(LEXER.TOKEN.VALUE);
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
  TPSDEFENTRY *DEF;
  INITDEFS();
  PRIMITIVETYPES.PTNIL = ADDTYPE(MAKETYPE(str_make(3, "NIL"), TTCNIL));
  PRIMITIVETYPES.PTBOOLEAN = ADDTYPE(MAKETYPE(str_make(7, "BOOLEAN"), TTCBOOLEAN));
  PRIMITIVETYPES.PTINTEGER = ADDTYPE(MAKETYPE(str_make(7, "INTEGER"), TTCINTEGER));
  PRIMITIVETYPES.PTREAL = ADDTYPE(MAKETYPE(str_make(4, "REAL"), TTCREAL));
  PRIMITIVETYPES.PTCHAR = ADDTYPE(MAKETYPE(str_make(4, "CHAR"), TTCCHAR));
  PRIMITIVETYPES.PTSTRING = ADDTYPE(MAKETYPE(str_make(6, "STRING"), TTCSTRING));
  PRIMITIVETYPES.PTTEXT = ADDTYPE(MAKETYPE(str_make(4, "TEXT"), TTCTEXT));
  ADDCONSTANT(MAKECONSTANT(str_make(5, "FALSE"), EXBOOLEANCONSTANT(0)));
  ADDCONSTANT(MAKECONSTANT(str_make(4, "TRUE"), EXBOOLEANCONSTANT(1)));
  ADDCONSTANT(MAKECONSTANT(str_make(6, "MAXINT"), EXINTEGERCONSTANT(32767)));
  ADDVARIABLE(MAKEVARIABLE(str_make(5, "INPUT"), PRIMITIVETYPES.PTTEXT));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "OUTPUT"), PRIMITIVETYPES.PTTEXT));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "STDERR"), PRIMITIVETYPES.PTTEXT));
  ADDPSEUDOFN(str_make(3, "ORD"), TPFORD);
  ADDPSEUDOFN(str_make(4, "PRED"), TPFPRED);
  ADDPSEUDOFN(str_make(4, "SUCC"), TPFSUCC);
  ADDPSEUDOFN(str_make(7, "DISPOSE"), TPFDISPOSE);
  ADDPSEUDOFN(str_make(3, "NEW"), TPFNEW);
  ADDPSEUDOFN(str_make(4, "READ"), TPFREAD);
  ADDPSEUDOFN(str_make(6, "READLN"), TPFREADLN);
  ADDPSEUDOFN(str_make(5, "WRITE"), TPFWRITE);
  ADDPSEUDOFN(str_make(7, "WRITELN"), TPFWRITELN);
  ADDFUNCTION(MAKEPROCEDURE2(str_make(6, "ASSIGN"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT), MAKECONSTARG(str_make(4, "NAME"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "CLOSE"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "EOF"), PRIMITIVETYPES.PTBOOLEAN, MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "RESET"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(7, "REWRITE"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDPSEUDOFN(str_make(6, "CONCAT"), TPFCONCAT);
  ADDPSEUDOFN(str_make(3, "STR"), TPFSTR);
  ADDPSEUDOFN(str_make(3, "VAL"), TPFVAL);
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "CHR"), PRIMITIVETYPES.PTCHAR, MAKEARG(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION3(str_make(4, "COPY"), PRIMITIVETYPES.PTSTRING, MAKECONSTARG(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING), MAKEARG(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER), MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEPROCEDURE3(str_make(6, "DELETE"), MAKEVARARG(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING), MAKEARG(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER), MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEPROCEDURE3(str_make(6, "INSERT"), MAKECONSTARG(str_make(3, "INS"), PRIMITIVETYPES.PTSTRING), MAKEVARARG(str_make(6, "TARGET"), PRIMITIVETYPES.PTSTRING), MAKEARG(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(6, "LENGTH"), PRIMITIVETYPES.PTINTEGER, MAKECONSTARG(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEFUNCTION2(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, MAKECONSTARG(str_make(6, "NEEDLE"), PRIMITIVETYPES.PTSTRING), MAKECONSTARG(str_make(8, "HAYSTACK"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(6, "UPCASE"), PRIMITIVETYPES.PTCHAR, MAKEARG(str_make(3, "CHR"), PRIMITIVETYPES.PTCHAR)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(4, "HALT"), MAKEARG(str_make(4, "CODE"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION0(str_make(10, "PARAMCOUNT"), PRIMITIVETYPES.PTINTEGER));
  ADDFUNCTION(MAKEFUNCTION1(str_make(8, "PARAMSTR"), PRIMITIVETYPES.PTSTRING, MAKEARG(str_of('I'), PRIMITIVETYPES.PTINTEGER)));
  DEF = DEFS.LATEST;
  while (DEF != (void*)0) {
    switch (DEF->CLS) {
      case TDCVARIABLE:
        {
          DEF->VARPTR->WASINITIALIZED = 1;
          DEF->VARPTR->WASUSED = 1;
        }
        break;
      case TDCFUNCTION:
        DEF->FNPTR->WASUSED = 1;
        break;
      case TDCTYPE:
        DEF->TYPEPTR->WASUSED = 1;
        break;
      default:
        break;
    }
    DEF = DEF->PREV;
  }
  STARTTEMPORARYSCOPE();
}

typedef enum enum9 { TOTNONE, TOTTYPE, TOTVAR, TOTENUMVAL, TOTFUNDEC, TOTFUNDEF } TOUTPUTTYPE;

const char* enumvalues9[] = { "TOTNONE", "TOTTYPE", "TOTVAR", "TOTENUMVAL", "TOTFUNDEC", "TOTFUNDEF" };

struct record31 {
  PFile OUTPUT;
  int ISMULTISTATEMENT;
  int INDENT;
  int NEWLINE;
  TOUTPUTTYPE LASTOUT;
  int CHECKBOUNDS;
} CODEGEN;

void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF);

void _OUTNEWLINE() {
  WRITELN(&CODEGEN.OUTPUT);
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
        WRITE_s(&CODEGEN.OUTPUT, str_make(2, "  "));
        if (CT == last) break;
        ++CT;
      }
    }
  } while(0);
  CODEGEN.NEWLINE = 0;
}

void OUTBEGIN() {
  CODEGEN.ISMULTISTATEMENT = 1;
  WRITE_c(&CODEGEN.OUTPUT, '{');
  _OUTNEWLINE();
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
}

void OUTEND() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  WRITE_c(&CODEGEN.OUTPUT, '}');
  _OUTNEWLINE();
}

void OUTENDSAMELINE() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  WRITE_c(&CODEGEN.OUTPUT, '}');
}

void _OUTESCAPEDCHAR(unsigned char CH) {
  int CODE;
  int N1;
  int N2;
  CODE = (int)CH;
  N1 = CODE / 16;
  N2 = CODE % 16;
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\x"));
  if (N1 < 10) WRITE_c(&CODEGEN.OUTPUT, CHR(N1 + 48));
  else WRITE_c(&CODEGEN.OUTPUT, CHR(N1 + 87));
  if (N2 < 10) WRITE_c(&CODEGEN.OUTPUT, CHR(N2 + 48));
  else WRITE_c(&CODEGEN.OUTPUT, CHR(N2 + 87));
}

void _OUTCHAR(unsigned char CHR) {
  WRITE_c(&CODEGEN.OUTPUT, '\'');
  if (CHR == '\'') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\'"));
  else if (CHR == '\\') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\\\"));
  else if (CHR >= ' ' && CHR <= '~') WRITE_c(&CODEGEN.OUTPUT, CHR);
  else _OUTESCAPEDCHAR(CHR);
  WRITE_c(&CODEGEN.OUTPUT, '\'');
}

void _OUTCSTRING(PString *STR) {
  int POS;
  unsigned char CH;
  WRITE_c(&CODEGEN.OUTPUT, '"');
  do {
    int first = 1;
    int last = LENGTH(STR);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          CH = (*STR).chr[POS];
          if (CH < ' ' || CH > '~') {
            _OUTESCAPEDCHAR(CH);
            if (POS < LENGTH(STR) && LXISHEXDIGIT((*STR).chr[POS + 1])) WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\"\""));
          }
          else {
            if (CH == '"') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\\""));
            else if (CH == '\\') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\\\"));
            else WRITE_c(&CODEGEN.OUTPUT, CH);
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_c(&CODEGEN.OUTPUT, '"');
}

void _OUTSTRING(PString *STR) {
  if (LENGTH(STR) == 1) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(7, "str_of("));
    _OUTCHAR((*STR).chr[1]);
    WRITE_c(&CODEGEN.OUTPUT, ')');
  }
  else {
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, "str_make("));
    WRITE_i(&CODEGEN.OUTPUT, LENGTH(STR));
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTCSTRING(STR);
    WRITE_c(&CODEGEN.OUTPUT, ')');
  }
}

int _BINOPPREC(TEXPRESSIONOBJ *EXPR) {
  int RESULT;
  switch (EXPR->BINARY.OP) {
    case TKPLUS:
      if (ISSTRINGYTYPE(EXPR->TYPEPTR)) RESULT = 1;
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
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 11;
      else RESULT = 8;
      break;
    case TKOR:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 12;
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
      COMPILEERROR(cat_ss(str_make(48, "Internal error: unknown precedence for operator "), LXTOKENNAME(EXPR->BINARY.OP)));
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
    case XCTOREAL:
      RESULT = 2;
      break;
    case XCSETTMPVAR:
      RESULT = 0;
      break;
    case XCSUBRANGE:
      RESULT = 0;
      break;
    case XCVARIABLE:
      if (EXPR->VARPTR->ISREFERENCE) RESULT = 2;
      else RESULT = 0;
      break;
    case XCFIELD:
      RESULT = 1;
      break;
    case XCARRAY:
      RESULT = 1;
      break;
    case XCPOINTER:
      RESULT = 2;
      break;
    case XCSTRINGCHAR:
      RESULT = 1;
      break;
    case XCFNREF:
      RESULT = 0;
      break;
    case XCFNCALL:
      RESULT = 1;
      break;
    case XCPSEUDOFNREF:
      RESULT = 0;
      break;
    case XCPSEUDOFNCALL:
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
  if (USEPARENS) WRITE_c(&CODEGEN.OUTPUT, '(');
  OUTEXPRESSION(EXPR);
  if (USEPARENS) WRITE_c(&CODEGEN.OUTPUT, ')');
}

void _OUTEXPRESSIONPARENS(TEXPRESSIONOBJ *EXPR, TEXPRESSIONOBJ *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF));
}

void _OUTEXPRESSIONPARENSEXTRA(TEXPRESSIONOBJ *EXPR, TEXPRESSIONOBJ *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF) - 1);
}

void _OUTEXIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICNIL:
        WRITE_s(&CODEGEN.OUTPUT, str_make(8, "(void*)0"));
        break;
      case XICBOOLEAN:
        if (with1->BOOLEANVAL) WRITE_c(&CODEGEN.OUTPUT, '1');
        else WRITE_c(&CODEGEN.OUTPUT, '0');
        break;
      case XICINTEGER:
        WRITE_i(&CODEGEN.OUTPUT, with1->INTEGERVAL);
        break;
      case XICREAL:
        WRITE_r(&CODEGEN.OUTPUT, with1->REALVAL);
        break;
      case XICCHAR:
        _OUTCHAR(with1->CHARVAL);
        break;
      case XICSTRING:
        _OUTSTRING(&with1->STRINGVAL);
        break;
      case XICENUM:
        WRITE_s(&CODEGEN.OUTPUT, with1->ENUMPTR->VALUES[subrange(with1->ENUMORDINAL, 0, 127)]);
        break;
      default:
        break;
    }
  }
}

void _OUTBOUNDS(TPSTYPE *TYPEPTR) {
  WRITE_i(&CODEGEN.OUTPUT, GETTYPELOWBOUND(TYPEPTR));
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
  WRITE_i(&CODEGEN.OUTPUT, GETTYPEHIGHBOUND(TYPEPTR));
}

void _OUTARRAYINDEX(TEXPRESSIONOBJ *INDEX, TPSTYPE *TYPEPTR) {
  int LOWBOUND;
  TEXPRESSIONOBJ *SIZE;
  LOWBOUND = GETTYPELOWBOUND(TYPEPTR->ARRAYPTR->INDEXTYPEPTR);
  SIZE = EXBINARYOP(PFORD(COPYEXPR(INDEX)), EXINTEGERCONSTANT(LOWBOUND), TKMINUS);
  OUTEXPRESSION(SIZE);
  DISPOSEEXPR(&SIZE);
}

void _OUTADDRESS(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->CLS == XCPOINTER) OUTEXPRESSION(EXPR->POINTEREXPR);
  else if (EXPR->CLS == XCVARIABLE && EXPR->VARPTR->ISREFERENCE) WRITE_s(&CODEGEN.OUTPUT, EXPR->VARPTR->NAME);
  else {
    WRITE_c(&CODEGEN.OUTPUT, '&');
    _OUTEXPRESSIONPARENSPREC(EXPR, 2);
  }
}

void _OUTEXSUBRANGE(TEXPRESSIONOBJ *EXPR) {
  if (!CODEGEN.CHECKBOUNDS) OUTEXPRESSION(EXPR->SUBRANGEPARENT);
  else {
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, "subrange("));
    OUTEXPRESSION(EXPR->SUBRANGEPARENT);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTBOUNDS(EXPR->TYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, ')');
  }
}

void _OUTEXVARIABLE(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->VARPTR->ISREFERENCE) {
    WRITE_c(&CODEGEN.OUTPUT, '*');
    WRITE_s(&CODEGEN.OUTPUT, EXPR->VARPTR->NAME);
  }
  else WRITE_s(&CODEGEN.OUTPUT, EXPR->VARPTR->NAME);
}

void _OUTEXFIELDACCESS(TEXPRESSIONOBJ *EXPR) {
  {
    TEXPRESSIONOBJ *with1 = EXPR->RECEXPR;
    {
      if (with1->CLS == XCPOINTER) {
        _OUTEXPRESSIONPARENS(with1->POINTEREXPR, EXPR);
        WRITE_s(&CODEGEN.OUTPUT, str_make(2, "->"));
      }
      else if (with1->CLS == XCVARIABLE && with1->VARPTR->ISREFERENCE) {
        WRITE_s(&CODEGEN.OUTPUT, with1->VARPTR->NAME);
        WRITE_s(&CODEGEN.OUTPUT, str_make(2, "->"));
      }
      else {
        _OUTEXPRESSIONPARENS(EXPR->RECEXPR, EXPR);
        WRITE_c(&CODEGEN.OUTPUT, '.');
      }
      WRITE_s(&CODEGEN.OUTPUT, EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[(int)subrange(EXPR->RECFIELDNUM, 1, 32) - 1].NAME);
    }
  }
}

void _OUTEXFUNCTIONCALL(TEXPRESSIONOBJ *EXPR) {
  int POS;
  _OUTEXPRESSIONPARENS(EXPR->FNEXPR, EXPR);
  WRITE_c(&CODEGEN.OUTPUT, '(');
  do {
    int first = 1;
    int last = EXPR->CALLARGS.SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          if (EXPR->FNEXPR->FNPTR->ARGS[(int)subrange(POS, 1, 8) - 1].ISREFERENCE) {
            if (!EXPR->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]->ISASSIGNABLE) COMPILEERROR(str_make(45, "Pass-by-reference argument must be assignable"));
            _OUTADDRESS(EXPR->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]);
          }
          else OUTEXPRESSION(EXPR->CALLARGS.VALUES[(int)subrange(POS, 1, 8) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_c(&CODEGEN.OUTPUT, ')');
}

void _OUTDISPOSE(TEXPRESSIONOBJ *EXPR);
void _OUTNEW(TEXPRESSIONOBJ *EXPR);
void _OUTORD(TEXPRESSIONOBJ *EXPR);
void _OUTPRED(TEXPRESSIONOBJ *EXPR);
void _OUTREAD(TEXPRESSIONOBJ *EXPR);
void _OUTSTR(TEXPRESSIONOBJ *EXPR);
void _OUTSUCC(TEXPRESSIONOBJ *EXPR);
void _OUTVAL(TEXPRESSIONOBJ *EXPR);
void _OUTWRITE(TEXPRESSIONOBJ *EXPR);

void _OUTEXPSEUDOFNCALL(TEXPRESSIONOBJ *EXPR) {
  switch (EXPR->PSEUDOFNCALL.PSEUDOFN) {
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
    case TPFVAL:
      _OUTVAL(EXPR);
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
  if (EXPR->UNARY.OP == TKMINUS) WRITE_c(&CODEGEN.OUTPUT, '-');
  else if (EXPR->UNARY.OP == TKNOT && ISBOOLEANTYPE(EXPR->TYPEPTR)) WRITE_c(&CODEGEN.OUTPUT, '!');
  else if (EXPR->UNARY.OP == TKNOT && ISINTEGERTYPE(EXPR->TYPEPTR)) WRITE_c(&CODEGEN.OUTPUT, '~');
  _OUTEXPRESSIONPARENS(EXPR->UNARY.PARENT, EXPR);
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
  unsigned char LTYPE;
  unsigned char RTYPE;
  {
    TEXBINARYOP *with1 = &EXPR->BINARY;
    {
      if (ISSTRINGYTYPE(with1->LEFT->TYPEPTR)) {
        if (ISCHARTYPE(with1->LEFT->TYPEPTR)) LTYPE = 'c';
        else LTYPE = 's';
        if (ISCHARTYPE(with1->RIGHT->TYPEPTR)) RTYPE = 'c';
        else RTYPE = 's';
        if (with1->OP == TKPLUS) {
          WRITE_s(&CODEGEN.OUTPUT, str_make(4, "cat_"));
          WRITE_c(&CODEGEN.OUTPUT, LTYPE);
          WRITE_c(&CODEGEN.OUTPUT, RTYPE);
          WRITE_c(&CODEGEN.OUTPUT, '(');
          OUTEXPRESSION(with1->LEFT);
          WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          OUTEXPRESSION(with1->RIGHT);
          WRITE_c(&CODEGEN.OUTPUT, ')');
        }
        else if (ISCHARTYPE(with1->LEFT->TYPEPTR) && ISCHARTYPE(with1->RIGHT->TYPEPTR)) {
          _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
          if (_ISRELATIONALOP(with1->OP)) {
            WRITE_c(&CODEGEN.OUTPUT, ' ');
            WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP));
            WRITE_c(&CODEGEN.OUTPUT, ' ');
          }
          else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
          _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
        }
        else {
          WRITE_s(&CODEGEN.OUTPUT, str_make(4, "cmp_"));
          WRITE_c(&CODEGEN.OUTPUT, LTYPE);
          WRITE_c(&CODEGEN.OUTPUT, RTYPE);
          WRITE_c(&CODEGEN.OUTPUT, '(');
          OUTEXPRESSION(with1->LEFT);
          WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          OUTEXPRESSION(with1->RIGHT);
          if (_ISRELATIONALOP(with1->OP)) {
            WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "));
            WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP));
            WRITE_s(&CODEGEN.OUTPUT, str_make(2, " 0"));
          }
          else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        }
      }
      else if (ISBOOLEANTYPE(with1->LEFT->TYPEPTR)) {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISLOGICALORBITWISEOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ');
          WRITE_s(&CODEGEN.OUTPUT, _GETLOGICALOP(with1->OP));
          WRITE_c(&CODEGEN.OUTPUT, ' ');
        }
        else if (_ISRELATIONALOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ');
          WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP));
          WRITE_c(&CODEGEN.OUTPUT, ' ');
        }
        else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
      else if (ISNUMERICTYPE(with1->LEFT->TYPEPTR)) {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISARITHMETICOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ');
          WRITE_s(&CODEGEN.OUTPUT, _GETARITHMETICOP(with1->OP));
          WRITE_c(&CODEGEN.OUTPUT, ' ');
        }
        else if (_ISLOGICALORBITWISEOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ');
          WRITE_s(&CODEGEN.OUTPUT, _GETBITWISEOP(with1->OP));
          WRITE_c(&CODEGEN.OUTPUT, ' ');
        }
        else if (_ISRELATIONALOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ');
          WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP));
          WRITE_c(&CODEGEN.OUTPUT, ' ');
        }
        else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
      else {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISRELATIONALOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ');
          WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP));
          WRITE_c(&CODEGEN.OUTPUT, ' ');
        }
        else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
    }
  }
}

void _OUTEXSETTMPVAR(TEXPRESSIONOBJ *EXPR) {
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "({ "));
  while (EXPR->CLS == XCSETTMPVAR) {
    OUTVARIABLEDECLARATION(*EXPR->TMPVAR->VARPTR);
    WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "));
    OUTEXPRESSION(EXPR->TMPVARVALUE);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, "; "));
    EXPR = EXPR->TMPVARCHILD;
  }
  OUTEXPRESSION(EXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "; })"));
}

void OUTEXPRESSION(TEXPRESSIONOBJ *EXPR) {
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      _OUTEXIMMEDIATE(EXPR);
      break;
    case XCTOSTRING:
      {
        WRITE_s(&CODEGEN.OUTPUT, str_make(7, "str_of("));
        OUTEXPRESSION(EXPR->TOSTRPARENT);
        WRITE_c(&CODEGEN.OUTPUT, ')');
      }
      break;
    case XCTOREAL:
      {
        WRITE_s(&CODEGEN.OUTPUT, str_make(8, "(double)"));
        OUTEXPRESSION(EXPR->TOREALPARENT);
      }
      break;
    case XCSETTMPVAR:
      _OUTEXSETTMPVAR(EXPR);
      break;
    case XCSUBRANGE:
      _OUTEXSUBRANGE(EXPR);
      break;
    case XCVARIABLE:
      _OUTEXVARIABLE(EXPR);
      break;
    case XCFIELD:
      _OUTEXFIELDACCESS(EXPR);
      break;
    case XCARRAY:
      {
        _OUTEXPRESSIONPARENS(EXPR->ARRAYEXPR, EXPR);
        WRITE_c(&CODEGEN.OUTPUT, '[');
        _OUTARRAYINDEX(EXPR->ARRAYINDEX, EXPR->ARRAYEXPR->TYPEPTR);
        WRITE_c(&CODEGEN.OUTPUT, ']');
      }
      break;
    case XCPOINTER:
      {
        WRITE_c(&CODEGEN.OUTPUT, '*');
        _OUTEXPRESSIONPARENS(EXPR->POINTEREXPR, EXPR);
      }
      break;
    case XCSTRINGCHAR:
      {
        _OUTEXPRESSIONPARENS(EXPR->STRINGEXPR, EXPR);
        WRITE_s(&CODEGEN.OUTPUT, str_make(5, ".chr["));
        OUTEXPRESSION(EXPR->STRINGINDEX);
        WRITE_c(&CODEGEN.OUTPUT, ']');
      }
      break;
    case XCFNREF:
      WRITE_s(&CODEGEN.OUTPUT, EXPR->FNPTR->NAME);
      break;
    case XCFNCALL:
      _OUTEXFUNCTIONCALL(EXPR);
      break;
    case XCPSEUDOFNCALL:
      _OUTEXPSEUDOFNCALL(EXPR);
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

void OUTENUMVALUES(TPSENUMDEF *ENUMPTR) {
  int POSINENUM;
  _OUTBLANKLINE(TOTENUMVAL);
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(22, "const char* enumvalues"));
  WRITE_i(&CODEGEN.OUTPUT, ENUMPTR->ID);
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, "[] = { "));
  do {
    int first = 0;
    int last = ENUMPTR->SIZE - 1;
    if (first <= last) {
      POSINENUM = first;
      while (1) {
        {
          if (POSINENUM != 0) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
          WRITE_c(&CODEGEN.OUTPUT, '"');
          WRITE_s(&CODEGEN.OUTPUT, ENUMPTR->VALUES[subrange(POSINENUM, 0, 127)]);
          WRITE_c(&CODEGEN.OUTPUT, '"');
        }
        if (POSINENUM == last) break;
        ++POSINENUM;
      }
    }
  } while(0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " };"));
  _OUTNEWLINE();
}

void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT) {
  TPSDEFENTRY *DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != (void*)0) {
    if (DEF->CLS == TDCENUM) OUTENUMVALUES(DEF->ENUMPTR);
    DEF = DEF->NEXT;
  }
}

PString OUTVARIABLENAME(PString NAME, int ISREFERENCE) {
  PString RESULT;
  if (ISREFERENCE) RESULT = cat_cs('*', NAME);
  else RESULT = NAME;
  return RESULT;
}

void OUTTYPEREFERENCE(TPSTYPE *TYPEPTR) {
  if (TYPEPTR == (void*)0) WRITE_s(&CODEGEN.OUTPUT, str_make(4, "void"));
  else if (TYPEPTR->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEPTR->POINTEDTYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, '*');
  }
  else if (TYPEPTR->CLS == TTCBOOLEAN) WRITE_s(&CODEGEN.OUTPUT, str_make(3, "int"));
  else if (TYPEPTR->CLS == TTCINTEGER) WRITE_s(&CODEGEN.OUTPUT, str_make(3, "int"));
  else if (TYPEPTR->CLS == TTCREAL) WRITE_s(&CODEGEN.OUTPUT, str_make(6, "double"));
  else if (TYPEPTR->CLS == TTCCHAR) WRITE_s(&CODEGEN.OUTPUT, str_make(13, "unsigned char"));
  else if (TYPEPTR->CLS == TTCSTRING) WRITE_s(&CODEGEN.OUTPUT, str_make(7, "PString"));
  else if (TYPEPTR->CLS == TTCTEXT) WRITE_s(&CODEGEN.OUTPUT, str_make(5, "PFile"));
  else if (TYPEPTR->CLS == TTCENUM) {
    if (TYPEPTR->ENUMPTR->HASBEENDEFINED && cmp_ss(TYPEPTR->NAME, str_make(0, "")) != 0) WRITE_s(&CODEGEN.OUTPUT, TYPEPTR->NAME);
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
      WRITE_i(&CODEGEN.OUTPUT, TYPEPTR->ENUMPTR->ID);
    }
  }
  else if (TYPEPTR->CLS == TTCRANGE) OUTTYPEREFERENCE(TYPEPTR->RANGEPTR->BASETYPEPTR);
  else if (TYPEPTR->CLS == TTCRECORD) {
    if (TYPEPTR->RECPTR->HASBEENDEFINED && cmp_ss(TYPEPTR->NAME, str_make(0, "")) != 0) WRITE_s(&CODEGEN.OUTPUT, TYPEPTR->NAME);
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
      WRITE_i(&CODEGEN.OUTPUT, TYPEPTR->RECPTR->ID);
    }
  }
  else if (TYPEPTR->CLS == TTCARRAY) {
    OUTTYPEREFERENCE(TYPEPTR->ARRAYPTR->VALUETYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, '*');
  }
  else COMPILEERROR(cat_ss(str_make(30, "Error writing type reference: "), TYPENAME(TYPEPTR)));
}

void OUTNAMEANDTYPE(PString NAME, TPSTYPE *TYPEPTR);

void OUTNAMEANDRECORD(PString NAME, TPSRECORDDEF *RECPTR) {
  int POS;
  int NUMVARIANT;
  NUMVARIANT = 0;
  WRITE_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
  WRITE_i(&CODEGEN.OUTPUT, RECPTR->ID);
  if (!RECPTR->HASBEENDEFINED) {
    WRITE_c(&CODEGEN.OUTPUT, ' ');
    OUTBEGIN();
    do {
      int first = 1;
      int last = RECPTR->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (RECPTR->NUMVARIANTS > NUMVARIANT && RECPTR->VARIANTBOUNDS[(int)subrange(NUMVARIANT + 1, 1, 32) - 1] == POS) {
              NUMVARIANT = NUMVARIANT + 1;
              if (NUMVARIANT == 1) {
                _OUTINDENT();
                WRITE_s(&CODEGEN.OUTPUT, str_make(6, "union "));
                OUTBEGIN();
              }
              else {
                OUTENDSAMELINE();
                WRITE_c(&CODEGEN.OUTPUT, ';');
                _OUTNEWLINE();
              }
              _OUTINDENT();
              WRITE_s(&CODEGEN.OUTPUT, str_make(7, "struct "));
              OUTBEGIN();
            }
            _OUTINDENT();
            OUTNAMEANDTYPE(RECPTR->FIELDS[(int)subrange(POS, 1, 32) - 1].NAME, RECPTR->FIELDS[(int)subrange(POS, 1, 32) - 1].TYPEPTR);
            WRITE_c(&CODEGEN.OUTPUT, ';');
            _OUTNEWLINE();
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    if (NUMVARIANT > 0) {
      OUTENDSAMELINE();
      WRITE_c(&CODEGEN.OUTPUT, ';');
      _OUTNEWLINE();
      OUTENDSAMELINE();
      WRITE_c(&CODEGEN.OUTPUT, ';');
      _OUTNEWLINE();
    }
    OUTENDSAMELINE();
    RECPTR->HASBEENDEFINED = 1;
  }
  WRITE_c(&CODEGEN.OUTPUT, ' ');
  WRITE_s(&CODEGEN.OUTPUT, NAME);
}

void OUTNAMEANDENUM(PString NAME, TPSENUMDEF *ENUMPTR) {
  int POS;
  WRITE_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
  WRITE_i(&CODEGEN.OUTPUT, ENUMPTR->ID);
  if (!ENUMPTR->HASBEENDEFINED) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(3, " { "));
    do {
      int first = 0;
      int last = ENUMPTR->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS > 0) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
            WRITE_s(&CODEGEN.OUTPUT, ENUMPTR->VALUES[subrange(POS, 0, 127)]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, " }"));
    ENUMPTR->HASBEENDEFINED = 1;
  }
  WRITE_c(&CODEGEN.OUTPUT, ' ');
  WRITE_s(&CODEGEN.OUTPUT, NAME);
}

void OUTNAMEANDARRAY(PString NAME, TPSTYPE *TYPEPTR) {
  TPSTYPE *THETYPE;
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) THETYPE = THETYPE->ARRAYPTR->VALUETYPEPTR;
  OUTNAMEANDTYPE(NAME, THETYPE);
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) {
    WRITE_c(&CODEGEN.OUTPUT, '[');
    WRITE_i(&CODEGEN.OUTPUT, GETBOUNDEDTYPESIZE(TYPEPTR->ARRAYPTR->INDEXTYPEPTR));
    WRITE_c(&CODEGEN.OUTPUT, ']');
    THETYPE = THETYPE->ARRAYPTR->VALUETYPEPTR;
  }
}

void OUTNAMEANDTYPE(PString NAME, TPSTYPE *TYPEPTR) {
  if (TYPEPTR == (void*)0) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, "void "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEPTR->POINTEDTYPEPTR);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, " *"));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->ALIASFOR != (void*)0 && cmp_ss(TYPEPTR->NAME, str_make(0, "")) != 0) {
    WRITE_s(&CODEGEN.OUTPUT, TYPEPTR->NAME);
    WRITE_c(&CODEGEN.OUTPUT, ' ');
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCBOOLEAN) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(4, "int "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCINTEGER) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(4, "int "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCREAL) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(7, "double "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCCHAR) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(14, "unsigned char "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCSTRING) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(8, "PString "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCTEXT) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "PFile "));
    WRITE_s(&CODEGEN.OUTPUT, NAME);
  }
  else if (TYPEPTR->CLS == TTCENUM) OUTNAMEANDENUM(NAME, TYPEPTR->ENUMPTR);
  else if (TYPEPTR->CLS == TTCRANGE) OUTNAMEANDTYPE(NAME, TYPEPTR->RANGEPTR->BASETYPEPTR);
  else if (TYPEPTR->CLS == TTCRECORD) OUTNAMEANDRECORD(NAME, TYPEPTR->RECPTR);
  else if (TYPEPTR->CLS == TTCARRAY) OUTNAMEANDARRAY(NAME, TYPEPTR);
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(29, "Error writing name and type: "), NAME), str_make(2, ", ")), TYPENAME(TYPEPTR)));
}

void OUTTYPEDEFINITION(TPSTYPE *TYPEPTR) {
  PString NAME;
  _OUTBLANKLINE(TOTTYPE);
  _OUTINDENT();
  NAME = TYPEPTR->NAME;
  if (TYPEPTR->ALIASFOR == (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(5, "Type "), NAME), str_make(16, " is not an alias")));
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, "typedef "));
  OUTNAMEANDTYPE(NAME, TYPEPTR->ALIASFOR);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTTYPEDEFINITIONSFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT) {
  TPSDEFENTRY *DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != (void*)0) {
    if (DEF->CLS == TDCTYPE) {
      _RESOLVEPOINTERUNKNOWN(DEF->TYPEPTR);
      if (DEF->TYPEPTR->ALIASFOR != (void*)0) OUTTYPEDEFINITION(DEF->TYPEPTR);
    }
    DEF = DEF->NEXT;
  }
}

void OUTCONSTANTARRAYBEGIN() {
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, "{ "));
}

void OUTCONSTANTARRAYSEPARATOR() {
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
}

void OUTCONSTANTARRAYEND() {
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, " }"));
}

void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF) {
  if (VARDEF.ISCONSTANT) WRITE_s(&CODEGEN.OUTPUT, str_make(6, "const "));
  OUTNAMEANDTYPE(OUTVARIABLENAME(VARDEF.NAME, VARDEF.ISREFERENCE), VARDEF.TYPEPTR);
}

void OUTVARIABLEDEFINITION(TPSVARIABLE *VARPTR) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE *VARPTR) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "));
}

void OUTCONSTANTDEFINITIONEND() {
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF) {
  int POS;
  _OUTINDENT();
  OUTNAMEANDTYPE(DEF.NAME, DEF.RETURNTYPEPTR);
  WRITE_c(&CODEGEN.OUTPUT, '(');
  do {
    int first = 1;
    int last = DEF.ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          OUTVARIABLEDECLARATION(DEF.ARGS[(int)subrange(POS, 1, 8) - 1]);
          if (POS != DEF.ARGCOUNT) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_c(&CODEGEN.OUTPUT, ')');
}

void OUTFUNCTIONDECLARATION(TPSFUNCTION *FNPTR) {
  _OUTBLANKLINE(TOTFUNDEC);
  OUTFUNCTIONPROTOTYPE(*FNPTR);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONDEFINITION(TPSFUNCTION *FNPTR) {
  _OUTBLANKLINE(TOTFUNDEF);
  OUTFUNCTIONPROTOTYPE(*FNPTR);
  WRITE_c(&CODEGEN.OUTPUT, ' ');
  OUTBEGIN();
}

void OUTFUNCTIONEND(TPSFUNCTION *FNPTR) {
  if (FNPTR->RETURNTYPEPTR != (void*)0) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(14, "return RESULT;"));
    _OUTNEWLINE();
  }
  OUTEND();
}

void OUTPROGRAMHEADING(PString NAME) {
  WRITE_s(&CODEGEN.OUTPUT, str_make(12, "/* Program: "));
  WRITE_s(&CODEGEN.OUTPUT, NAME);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " */"));
  _OUTNEWLINE();
  WRITE_s(&CODEGEN.OUTPUT, str_make(20, "#include \"pascual.h\""));
  _OUTNEWLINE();
}

unsigned char SHORTTYPENAME(TPSTYPE *TYPEPTR) {
  unsigned char RESULT;
  while (ISRANGETYPE(TYPEPTR)) TYPEPTR = TYPEPTR->RANGEPTR->BASETYPEPTR;
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = 'b';
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = 'i';
  else if (ISREALTYPE(TYPEPTR)) RESULT = 'r';
  else if (ISCHARTYPE(TYPEPTR)) RESULT = 'c';
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = 's';
  else COMPILEERROR(cat_ss(str_make(30, "No short type name exists for "), TYPENAME(TYPEPTR)));
  return RESULT;
}

void _OUTREAD(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *SRC;
  TEXREADARGS *READARG;
  int LINEFEED;
  int BRACES;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  LINEFEED = EXPR->PSEUDOFNCALL.PSEUDOFN == TPFREADLN;
  READARG = EXPR->PSEUDOFNCALL.READARGS;
  BRACES = !CODEGEN.ISMULTISTATEMENT && READARG != (void*)0 && (READARG->NEXT != (void*)0 || LINEFEED);
  if (BRACES) OUTBEGIN();
  while (READARG != (void*)0) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, "READ_"));
    WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(READARG->ARG->TYPEPTR));
    WRITE_c(&CODEGEN.OUTPUT, '(');
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(READARG->ARG);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
    READARG = READARG->NEXT;
  }
  if (LINEFEED) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(7, "READLN("));
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  if (BRACES) OUTEND();
}

void _OUTWRITE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *DST;
  TEXWRITEARGS *WRITEARG;
  int LINEFEED;
  int BRACES;
  TPSTYPE *TYPEPTR;
  DST = EXPR->PSEUDOFNCALL.ARG1;
  LINEFEED = EXPR->PSEUDOFNCALL.PSEUDOFN == TPFWRITELN;
  WRITEARG = EXPR->PSEUDOFNCALL.WRITEARGS;
  BRACES = !CODEGEN.ISMULTISTATEMENT && WRITEARG != (void*)0 && (WRITEARG->NEXT != (void*)0 || LINEFEED);
  if (BRACES) OUTBEGIN();
  while (WRITEARG != (void*)0) {
    TYPEPTR = WRITEARG->ARG->TYPEPTR;
    while (ISRANGETYPE(TYPEPTR)) TYPEPTR = TYPEPTR->RANGEPTR->BASETYPEPTR;
    if (ISENUMTYPE(TYPEPTR)) {
      _OUTINDENT();
      WRITE_s(&CODEGEN.OUTPUT, str_make(8, "WRITE_e("));
      _OUTADDRESS(DST);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
      OUTEXPRESSION(WRITEARG->ARG);
      WRITE_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
      WRITE_i(&CODEGEN.OUTPUT, TYPEPTR->ENUMPTR->ID);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
      _OUTNEWLINE();
    }
    else {
      _OUTINDENT();
      WRITE_s(&CODEGEN.OUTPUT, str_make(6, "WRITE_"));
      WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(TYPEPTR));
      WRITE_c(&CODEGEN.OUTPUT, '(');
      _OUTADDRESS(DST);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
      OUTEXPRESSION(WRITEARG->ARG);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
      _OUTNEWLINE();
    }
    WRITEARG = WRITEARG->NEXT;
  }
  if (LINEFEED) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(8, "WRITELN("));
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  if (BRACES) OUTEND();
}

void _OUTSTR(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *SRC;
  TEXPRESSIONOBJ *DST;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  DST = EXPR->PSEUDOFNCALL.ARG2;
  if (ISENUMTYPE(SRC->TYPEPTR)) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "STR_e("));
    OUTEXPRESSION(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
    WRITE_i(&CODEGEN.OUTPUT, SRC->TYPEPTR->ENUMPTR->ID);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(4, "STR_"));
    WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(SRC->TYPEPTR));
    WRITE_c(&CODEGEN.OUTPUT, '(');
    OUTEXPRESSION(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
}

void _OUTVAL(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *SRC;
  TEXPRESSIONOBJ *DST;
  TEXPRESSIONOBJ *CODE;
  TEXPRESSIONOBJ *TMPEXPR;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  DST = EXPR->PSEUDOFNCALL.ARG2;
  CODE = EXPR->PSEUDOFNCALL.ARG3;
  if (ISENUMTYPE(DST->TYPEPTR)) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "VAL_e("));
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    TMPEXPR = EXINTEGERCONSTANT(DST->TYPEPTR->ENUMPTR->SIZE);
    OUTEXPRESSION(TMPEXPR);
    DISPOSEEXPR(&TMPEXPR);
    WRITE_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
    WRITE_i(&CODEGEN.OUTPUT, DST->TYPEPTR->ENUMPTR->ID);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(CODE);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(4, "VAL_"));
    WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(DST->TYPEPTR));
    WRITE_c(&CODEGEN.OUTPUT, '(');
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
    _OUTADDRESS(CODE);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
    _OUTNEWLINE();
  }
}

void _OUTNEW(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *PTR;
  PTR = EXPR->PSEUDOFNCALL.ARG1;
  _OUTINDENT();
  OUTEXPRESSION(PTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(17, " = malloc(sizeof("));
  OUTTYPEREFERENCE(PTR->TYPEPTR->POINTEDTYPEPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "));"));
  _OUTNEWLINE();
}

void _OUTDISPOSE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *PTR;
  PTR = EXPR->PSEUDOFNCALL.ARG1;
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "free("));
  OUTEXPRESSION(PTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
  _OUTNEWLINE();
}

void _OUTORD(TEXPRESSIONOBJ *EXPR) {
  if (ISCHARTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, "(int)"));
    _OUTEXPRESSIONPARENSPREC(EXPR->PSEUDOFNCALL.ARG1, 2);
  }
  else if (ISORDINALTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)));
}

void _OUTPRED(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *TMPEXPR;
  if (ISINTEGERTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    TMPEXPR = EXBINARYOP(COPYEXPR(EXPR->PSEUDOFNCALL.ARG1), EXINTEGERCONSTANT(1), TKMINUS);
    OUTEXPRESSION(TMPEXPR);
    DISPOSEEXPR(&TMPEXPR);
  }
  else if (ISORDINALTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    if (!CODEGEN.CHECKBOUNDS) {
      TMPEXPR = EXBINARYOP(PFORD(COPYEXPR(EXPR->PSEUDOFNCALL.ARG1)), EXINTEGERCONSTANT(1), TKMINUS);
      OUTEXPRESSION(TMPEXPR);
      DISPOSEEXPR(&TMPEXPR);
    }
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(5, "pred("));
      OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
      _OUTBOUNDS(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR);
      WRITE_c(&CODEGEN.OUTPUT, ')');
    }
  }
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)));
}

void _OUTSUCC(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *TMPEXPR;
  if (ISINTEGERTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    TMPEXPR = EXBINARYOP(COPYEXPR(EXPR->PSEUDOFNCALL.ARG1), EXINTEGERCONSTANT(1), TKPLUS);
    OUTEXPRESSION(TMPEXPR);
    DISPOSEEXPR(&TMPEXPR);
  }
  else if (ISORDINALTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    if (!CODEGEN.CHECKBOUNDS) {
      TMPEXPR = EXBINARYOP(PFORD(COPYEXPR(EXPR->PSEUDOFNCALL.ARG1)), EXINTEGERCONSTANT(1), TKPLUS);
      OUTEXPRESSION(TMPEXPR);
      DISPOSEEXPR(&TMPEXPR);
    }
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(5, "succ("));
      OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "));
      _OUTBOUNDS(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR);
      WRITE_c(&CODEGEN.OUTPUT, ')');
    }
  }
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)));
}

void OUTASSIGN(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  OUTEXPRESSION(LHS);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(RHS);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTDECLAREANDASSIGN(TPSVARIABLE *VARPTR, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  if (VARPTR->ISREFERENCE) _OUTADDRESS(RHS);
  else OUTEXPRESSION(RHS);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTIF(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "if ("));
  OUTEXPRESSION(EXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTELSE() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "else "));
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
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, "switch ("));
  OUTEXPRESSION(CASEINDEX);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  OUTBEGIN();
}

void OUTCASESTATEMENTBEGIN(TEXPRESSIONOBJ *CASELABEL) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "case "));
  OUTEXPRESSION(CASELABEL);
  WRITE_c(&CODEGEN.OUTPUT, ':');
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASESTATEMENTEND() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(6, "break;"));
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEELSEBEGIN() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, "default:"));
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASEELSEEND() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(6, "break;"));
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEEND() {
  OUTEND();
}

void OUTREPEATBEGIN() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "do "));
  OUTBEGIN();
}

void OUTREPEATEND(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *TMPEXPR;
  OUTENDSAMELINE();
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, " while ("));
  TMPEXPR = EXUNARYOP(COPYEXPR(EXPR), TKNOT);
  OUTEXPRESSION(TMPEXPR);
  DISPOSEEXPR(&TMPEXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"));
  _OUTNEWLINE();
}

void OUTWHILEBEGIN(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, "while ("));
  OUTEXPRESSION(EXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "));
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTWHILEEND() {
  CODEGEN.ISMULTISTATEMENT = 1;
}

void OUTFORBEGIN(TEXPRESSIONOBJ *ITER, TEXPRESSIONOBJ *FIRSTEXPR, TEXPRESSIONOBJ *LASTEXPR, int ASCENDING) {
  TPSTYPE *LIMITTYPE;
  TPSVARIABLE FIRST;
  TPSVARIABLE LAST;
  LIMITTYPE = ITER->TYPEPTR;
  if (ISENUMTYPE(LIMITTYPE)) LIMITTYPE = PRIMITIVETYPES.PTINTEGER;
  FIRST = MAKEVARIABLE(str_make(5, "first"), LIMITTYPE);
  LAST = MAKEVARIABLE(str_make(4, "last"), LIMITTYPE);
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "do "));
  OUTBEGIN();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(FIRST);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(FIRSTEXPR);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(LAST);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "));
  OUTEXPRESSION(LASTEXPR);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(10, "if (first "));
  if (ASCENDING) WRITE_s(&CODEGEN.OUTPUT, str_make(2, "<="));
  else WRITE_s(&CODEGEN.OUTPUT, str_make(2, ">="));
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, " last) "));
  OUTBEGIN();
  _OUTINDENT();
  OUTEXPRESSION(ITER);
  WRITE_s(&CODEGEN.OUTPUT, str_make(9, " = first;"));
  _OUTNEWLINE();
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(10, "while (1) "));
  OUTBEGIN();
}

void OUTFOREND(TEXPRESSIONOBJ *ITER, int ASCENDING) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "if ("));
  OUTEXPRESSION(ITER);
  WRITE_s(&CODEGEN.OUTPUT, str_make(16, " == last) break;"));
  _OUTNEWLINE();
  _OUTINDENT();
  if (ASCENDING) WRITE_s(&CODEGEN.OUTPUT, str_make(2, "++"));
  else WRITE_s(&CODEGEN.OUTPUT, str_make(2, "--"));
  OUTEXPRESSION(ITER);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
  OUTEND();
  OUTEND();
  OUTENDSAMELINE();
  WRITE_s(&CODEGEN.OUTPUT, str_make(10, " while(0);"));
  _OUTNEWLINE();
}

void OUTPROCEDURECALL(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  OUTEXPRESSION(EXPR);
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTPSEUDOPROCCALL(TEXPRESSIONOBJ *EXPR) {
  _OUTEXPSEUDOFNCALL(EXPR);
}

void OUTEMPTYSTATEMENT() {
  _OUTINDENT();
  WRITE_c(&CODEGEN.OUTPUT, ';');
  _OUTNEWLINE();
}

void OUTPROGRAMBEGIN() {
  _OUTBLANKLINE(TOTFUNDEF);
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(20, "void pascual_main() "));
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
  CODEGEN.CHECKBOUNDS = 1;
}

void CODEGENSETOUTPUT(PString FILENAME) {
  ASSIGN(&CODEGEN.OUTPUT, &FILENAME);
  REWRITE(&CODEGEN.OUTPUT);
}

void CODEGENSETCHECKBOUNDS(int CHECKBOUNDS) {
  CODEGEN.CHECKBOUNDS = CHECKBOUNDS;
}

TEXPRESSIONOBJ *_PF_UNARY_PARSE() {
  TEXPRESSIONOBJ *RESULT;
  WANTTOKENANDREAD(TKLPAREN);
  RESULT = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  return RESULT;
}

TEXPRESSIONOBJ *PFCONCAT_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *OPERAND;
  DISPOSEEXPR(&FNEXPR);
  RESULT = (void*)0;
  WANTTOKENANDREAD(TKLPAREN);
  do {
    OPERAND = PSEXPRESSION();
    if (!ISSTRINGYTYPE(OPERAND->TYPEPTR)) COMPILEERROR(str_make(35, "Argument for CONCAT is not a string"));
    if (RESULT == (void*)0) RESULT = OPERAND;
    else RESULT = EXBINARYOP(RESULT, OPERAND, TKPLUS);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRPAREN));
  WANTTOKENANDREAD(TKRPAREN);
  return RESULT;
}

TEXPRESSIONOBJ *PFDISPOSE_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *PTR;
  PTR = _PF_UNARY_PARSE();
  if (!PTR->ISASSIGNABLE || !ISPOINTERTYPE(PTR->TYPEPTR)) COMPILEERROR(str_make(37, "Argument for DISPOSE is not a pointer"));
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = PTR;
  return RESULT;
}

TEXPRESSIONOBJ *PFNEW_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *PTR;
  PTR = _PF_UNARY_PARSE();
  if (!PTR->ISASSIGNABLE || !ISPOINTERTYPE(PTR->TYPEPTR)) COMPILEERROR(str_make(33, "Argument for NEW is not a pointer"));
  EXMARKINITIALIZED(PTR);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = PTR;
  return RESULT;
}

TEXPRESSIONOBJ *PFORD_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  DISPOSEEXPR(&FNEXPR);
  RESULT = PFORD(_PF_UNARY_PARSE());
  return RESULT;
}

TEXPRESSIONOBJ *PFORD(TEXPRESSIONOBJ *ARG) {
  TEXPRESSIONOBJ *RESULT;
  TEXIMMEDIATE IMM;
  if (!ISORDINALTYPE(ARG->TYPEPTR)) COMPILEERROR(str_make(46, "Argument for ORD does not have an ordinal type"));
  if (EXISIMMEDIATE(ARG)) {
    IMM = ARG->IMMEDIATE;
    DISPOSEEXPR(&ARG);
    switch (IMM.CLS) {
      case XICBOOLEAN:
        if (IMM.BOOLEANVAL) RESULT = EXINTEGERCONSTANT(1);
        else RESULT = EXINTEGERCONSTANT(0);
        break;
      case XICINTEGER:
        RESULT = EXINTEGERCONSTANT(IMM.INTEGERVAL);
        break;
      case XICCHAR:
        RESULT = EXINTEGERCONSTANT((int)IMM.CHARVAL);
        break;
      case XICENUM:
        RESULT = EXINTEGERCONSTANT(IMM.ENUMORDINAL);
        break;
      default:
        COMPILEERROR(str_make(20, "Invalid type for ORD"));
        break;
    }
  }
  else {
    RESULT = EXPSEUDOFNCALL(EXPSEUDOFN(TPFORD));
    RESULT->PSEUDOFNCALL.ARG1 = ARG;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  }
  return RESULT;
}

TEXPRESSIONOBJ *PFPRED_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  DISPOSEEXPR(&FNEXPR);
  RESULT = PFPRED(_PF_UNARY_PARSE());
  return RESULT;
}

TEXPRESSIONOBJ *PFPRED(TEXPRESSIONOBJ *ARG) {
  TEXPRESSIONOBJ *RESULT;
  TEXIMMEDIATE IMM;
  int OUTOFBOUNDS;
  if (!ISORDINALTYPE(ARG->TYPEPTR)) COMPILEERROR(str_make(47, "Argument for PRED does not have an ordinal type"));
  if (EXISIMMEDIATE(ARG)) {
    IMM = ARG->IMMEDIATE;
    OUTOFBOUNDS = 0;
    switch (IMM.CLS) {
      case XICBOOLEAN:
        if (IMM.BOOLEANVAL) RESULT = EXBOOLEANCONSTANT(0);
        else OUTOFBOUNDS = 1;
        break;
      case XICINTEGER:
        RESULT = EXINTEGERCONSTANT(IMM.INTEGERVAL - 1);
        break;
      case XICCHAR:
        if ((int)IMM.CHARVAL > 0) RESULT = EXCHARCONSTANT(pred(IMM.CHARVAL, 0, 255));
        break;
      case XICENUM:
        if (IMM.ENUMORDINAL > 0) RESULT = EXENUMCONSTANT(IMM.ENUMORDINAL - 1, ARG->TYPEPTR);
        break;
      default:
        COMPILEERROR(str_make(21, "Invalid type for PRED"));
        break;
    }
    DISPOSEEXPR(&ARG);
    if (OUTOFBOUNDS) COMPILEERROR(str_make(21, "Out of bounds in PRED"));
  }
  else {
    RESULT = EXPSEUDOFNCALL(EXPSEUDOFN(TPFPRED));
    RESULT->PSEUDOFNCALL.ARG1 = ARG;
    RESULT->TYPEPTR = ARG->TYPEPTR;
  }
  return RESULT;
}

TEXPRESSIONOBJ *PFREAD_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  int FIRST;
  TEXPRESSIONOBJ *OUTVAR;
  TEXREADARGS *READARG;
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = EXVARIABLE(FINDNAMEOFCLASS(str_make(5, "INPUT"), TNCVARIABLE, 1)->VARPTR);
  READARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTVAR = PSEXPRESSION();
      if (FIRST && OUTVAR->ISASSIGNABLE && ISTEXTTYPE(OUTVAR->TYPEPTR)) {
        DISPOSEEXPR(&RESULT->PSEUDOFNCALL.ARG1);
        RESULT->PSEUDOFNCALL.ARG1 = OUTVAR;
      }
      else {
        if (!OUTVAR->ISASSIGNABLE || !ISSTRINGYTYPE(OUTVAR->TYPEPTR)) COMPILEERROR(str_make(25, "Invalid argument for READ"));
        if (READARG == (void*)0) {
          RESULT->PSEUDOFNCALL.READARGS = malloc(sizeof(TEXREADARGS));
          READARG = RESULT->PSEUDOFNCALL.READARGS;
        }
        else {
          READARG->NEXT = malloc(sizeof(TEXREADARGS));
          READARG = READARG->NEXT;
        }
        READARG->NEXT = (void*)0;
        READARG->ARG = OUTVAR;
        EXMARKINITIALIZED(OUTVAR);
      }
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
      FIRST = 0;
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  return RESULT;
}

TEXPRESSIONOBJ *PFSTR_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *SRC;
  TEXPRESSIONOBJ *DEST;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = EXOUTRANGE(PSEXPRESSION());
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  if (!DEST->ISASSIGNABLE || !ISSTRINGTYPE(DEST->TYPEPTR)) COMPILEERROR(str_make(48, "Second argument for STR is not a string variable"));
  if (!ISBOOLEANTYPE(SRC->TYPEPTR) && !ISINTEGERTYPE(SRC->TYPEPTR) && !ISREALTYPE(SRC->TYPEPTR) && !ISENUMTYPE(SRC->TYPEPTR)) COMPILEERROR(cat_ss(str_make(44, "First argument for STR has an invalid type: "), TYPENAME(SRC->TYPEPTR)));
  EXMARKINITIALIZED(DEST);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = SRC;
  RESULT->PSEUDOFNCALL.ARG2 = DEST;
  return RESULT;
}

TEXPRESSIONOBJ *PFSUCC_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  DISPOSEEXPR(&FNEXPR);
  RESULT = PFSUCC(_PF_UNARY_PARSE());
  return RESULT;
}

TEXPRESSIONOBJ *PFSUCC(TEXPRESSIONOBJ *ARG) {
  TEXPRESSIONOBJ *RESULT;
  TEXIMMEDIATE IMM;
  int OUTOFBOUNDS;
  if (!ISORDINALTYPE(ARG->TYPEPTR)) COMPILEERROR(str_make(47, "Argument for SUCC does not have an ordinal type"));
  if (EXISIMMEDIATE(ARG)) {
    IMM = ARG->IMMEDIATE;
    OUTOFBOUNDS = 0;
    switch (IMM.CLS) {
      case XICBOOLEAN:
        if (!IMM.BOOLEANVAL) RESULT = EXBOOLEANCONSTANT(1);
        else OUTOFBOUNDS = 1;
        break;
      case XICINTEGER:
        RESULT = EXINTEGERCONSTANT(IMM.INTEGERVAL + 1);
        break;
      case XICCHAR:
        if ((int)IMM.CHARVAL < 255) RESULT = EXCHARCONSTANT(succ(IMM.CHARVAL, 0, 255));
        break;
      case XICENUM:
        if (IMM.ENUMORDINAL < IMM.ENUMPTR->SIZE - 1) RESULT = EXENUMCONSTANT(IMM.ENUMORDINAL + 1, ARG->TYPEPTR);
        break;
      default:
        COMPILEERROR(str_make(21, "Invalid type for SUCC"));
        break;
    }
    DISPOSEEXPR(&ARG);
    if (OUTOFBOUNDS) COMPILEERROR(str_make(21, "Out of bounds in SUCC"));
  }
  else {
    RESULT = EXPSEUDOFNCALL(EXPSEUDOFN(TPFSUCC));
    RESULT->PSEUDOFNCALL.ARG1 = ARG;
    RESULT->TYPEPTR = ARG->TYPEPTR;
  }
  return RESULT;
}

TEXPRESSIONOBJ *PFVAL_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *SRC;
  TEXPRESSIONOBJ *DEST;
  TEXPRESSIONOBJ *CODE;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = EXOUTRANGE(PSEXPRESSION());
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  WANTTOKENANDREAD(TKCOMMA);
  CODE = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  if (!ISSTRINGTYPE(SRC->TYPEPTR)) COMPILEERROR(str_make(47, "First argument for VAL is not a string variable"));
  if (!DEST->ISASSIGNABLE) COMPILEERROR(str_make(41, "Second argument for VAL is not a variable"));
  if (!ISBOOLEANTYPE(DEST->TYPEPTR) && !ISINTEGERTYPE(DEST->TYPEPTR) && !ISREALTYPE(DEST->TYPEPTR) && !ISENUMTYPE(DEST->TYPEPTR)) COMPILEERROR(cat_ss(str_make(45, "Second argument for VAL has an invalid type: "), TYPENAME(DEST->TYPEPTR)));
  if (!CODE->ISASSIGNABLE || !ISINTEGERTYPE(CODE->TYPEPTR)) COMPILEERROR(str_make(49, "Third argument for VAL is not an integer variable"));
  EXMARKINITIALIZED(DEST);
  EXMARKINITIALIZED(CODE);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = SRC;
  RESULT->PSEUDOFNCALL.ARG2 = DEST;
  RESULT->PSEUDOFNCALL.ARG3 = CODE;
  return RESULT;
}

TEXPRESSIONOBJ *PFWRITE_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  int FIRST;
  TEXPRESSIONOBJ *OUTEXPR;
  TEXWRITEARGS *WRITEARG;
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = EXVARIABLE(FINDNAMEOFCLASS(str_make(6, "OUTPUT"), TNCVARIABLE, 1)->VARPTR);
  WRITEARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTEXPR = PSEXPRESSION();
      if (FIRST && OUTEXPR->ISASSIGNABLE && ISTEXTTYPE(OUTEXPR->TYPEPTR)) {
        DISPOSEEXPR(&RESULT->PSEUDOFNCALL.ARG1);
        RESULT->PSEUDOFNCALL.ARG1 = OUTEXPR;
      }
      else {
        if (WRITEARG == (void*)0) {
          RESULT->PSEUDOFNCALL.WRITEARGS = malloc(sizeof(TEXWRITEARGS));
          WRITEARG = RESULT->PSEUDOFNCALL.WRITEARGS;
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
  return RESULT;
}

TEXPRESSIONOBJ *PF_PARSE(TEXPRESSIONOBJ *FN) {
  TEXPRESSIONOBJ *RESULT;
  switch (FN->PSEUDOFN) {
    case TPFCONCAT:
      RESULT = PFCONCAT_PARSE(FN);
      break;
    case TPFDISPOSE:
      RESULT = PFDISPOSE_PARSE(FN);
      break;
    case TPFNEW:
      RESULT = PFNEW_PARSE(FN);
      break;
    case TPFORD:
      RESULT = PFORD_PARSE(FN);
      break;
    case TPFPRED:
      RESULT = PFPRED_PARSE(FN);
      break;
    case TPFREAD:
      RESULT = PFREAD_PARSE(FN);
      break;
    case TPFREADLN:
      RESULT = PFREAD_PARSE(FN);
      break;
    case TPFSTR:
      RESULT = PFSTR_PARSE(FN);
      break;
    case TPFSUCC:
      RESULT = PFSUCC_PARSE(FN);
      break;
    case TPFVAL:
      RESULT = PFVAL_PARSE(FN);
      break;
    case TPFWRITE:
      RESULT = PFWRITE_PARSE(FN);
      break;
    case TPFWRITELN:
      RESULT = PFWRITE_PARSE(FN);
      break;
    default:
      COMPILEERROR(str_make(46, "Internal error: unimplemented special function"));
      break;
  }
  return RESULT;
}

void COMPILEERROR(PString MSG) {
  WRITE_s(&STDERR, MSG);
  WRITE_s(&STDERR, LXWHERESTR());
  WRITELN(&STDERR);
  HALT(1);
}

void COMPILEWARNING(PString MSG) {
  if (!OPTIONS.SUPPRESSWARNINGS) {
    WRITE_s(&STDERR, str_make(9, "Warning: "));
    WRITE_s(&STDERR, MSG);
    WRITE_s(&STDERR, LXWHERESTR());
    WRITELN(&STDERR);
  }
}

void USAGE(PString MSG) {
  if (cmp_ss(MSG, str_make(0, "")) != 0) {
    WRITE_s(&OUTPUT, MSG);
    WRITELN(&OUTPUT);
    WRITELN(&OUTPUT);
  }
  WRITE_s(&OUTPUT, str_make(6, "Usage:"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, PARAMSTR(0));
  WRITE_s(&OUTPUT, str_make(33, " input.pas [-o output.c] [-Wnone]"));
  WRITELN(&OUTPUT);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(48, "If you specify \"-\" as the input or output file, "));
  WRITE_s(&OUTPUT, str_make(26, "stdin/stdout will be used."));
  WRITELN(&OUTPUT);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(8, "Options:"));
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(46, "   -Wnone    :- Suppress all warning messages."));
  WRITELN(&OUTPUT);
  HALT(0);
}

PString REPLACEEXTENSION(PString STR, PString OLD, PString NEW) {
  PString RESULT;
  int BASELEN;
  int POS;
  int MATCHES;
  RESULT = str_make(0, "");
  BASELEN = LENGTH(&STR) - LENGTH(&OLD);
  if (BASELEN > 0) {
    MATCHES = 1;
    do {
      int first = 1;
      int last = LENGTH(&OLD);
      if (first <= last) {
        POS = first;
        while (1) {
          MATCHES = MATCHES && UPCASE(STR.chr[POS + BASELEN]) == UPCASE(OLD.chr[POS]);
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    if (MATCHES) RESULT = cat_ss(COPY(&STR, 1, BASELEN), NEW);
  }
  return RESULT;
}

void PARSECMDLINE() {
  int POS;
  PString INPUTFILE;
  PString OUTPUTFILE;
  int SUPPRESSWARNINGS;
  enum enum10 { FLAGNONE, FLAGOUTPUT } FLAG;
  PString PARAM;
  const char* enumvalues10[] = { "FLAGNONE", "FLAGOUTPUT" };
  INPUTFILE = str_make(0, "");
  OUTPUTFILE = str_make(0, "");
  SUPPRESSWARNINGS = 0;
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
            else if (cmp_ss(PARAM, str_make(6, "-Wnone")) == 0) SUPPRESSWARNINGS = 1;
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
  OPTIONS.SUPPRESSWARNINGS = SUPPRESSWARNINGS;
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
  CLOSELOCALSCOPE();
  CLOSE(&LEXER.INPUT.SRC);
  CLOSE(&CODEGEN.OUTPUT);
}
