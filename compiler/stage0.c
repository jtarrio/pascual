/* Program: PASCUAL */
#include "pascual.h"

struct record1 {
  PBoolean SUPPRESSWARNINGS;
  PBoolean CHECKBOUNDS;
} OPTIONS;

void COMPILEERROR(PString MSG);
void INTERNALERROR(PString MSG);
void COMPILEWARNING(PString MSG);

typedef enum enum1 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKINTEGER, TKREAL, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAND, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKSHL, TKSHR, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH, TKXOR } TLXTOKENID;
typedef struct record2 {
  PInteger ROW;
  PInteger COL;
} TLXPOS;
typedef struct record3 {
  TLXTOKENID ID;
  PString VALUE;
  TLXPOS POS;
} TLXTOKEN;
typedef enum enum2 { TPFABS, TPFCONCAT, TPFDISPOSE, TPFNEW, TPFORD, TPFPRED, TPFRANDOM, TPFREAD, TPFREADLN, TPFSQR, TPFSTR, TPFSUCC, TPFVAL, TPFWRITE, TPFWRITELN } TPSPSEUDOFN;
typedef struct record17 *TPSTYPEPTR;
typedef struct record18 *TPSENUMPTR;
typedef struct record22 *TPSVARPTR;
typedef struct record23 *TPSFNPTR;
typedef struct record5 *TEXSETIMMBOUNDS;
typedef struct record6 *TEXSETEXPRBOUNDS;
typedef struct record13 *TEXPRESSION;
typedef enum enum3 { XICNIL, XICBOOLEAN, XICINTEGER, XICREAL, XICCHAR, XICSTRING, XICENUM, XICSET } TEXIMMEDIATECLASS;
typedef struct record4 {
  TEXIMMEDIATECLASS CLS;
  union {
    struct {
      PBoolean BOOLEANVAL;
    };
    struct {
      PInteger INTEGERVAL;
    };
    struct {
      PReal REALVAL;
    };
    struct {
      PChar CHARVAL;
    };
    struct {
      PString STRINGVAL;
    };
    struct {
      PInteger ENUMORDINAL;
      struct record18 *ENUMPTR;
    };
    struct {
      struct record5 *SETBOUNDS;
      struct record17 *SETOFTYPEPTR;
    };
  };
} TEXIMMEDIATE;
typedef struct record5 {
  PInteger FIRST;
  PInteger LAST;
  struct record5 *NEXT;
} TEXSETIMMBOUNDSOBJ;
typedef struct record6 {
  struct record13 *FIRST;
  struct record13 *LAST;
  struct record6 *NEXT;
} TEXSETEXPRBOUNDSOBJ;
typedef struct record7 {
  PInteger SIZE;
  struct record13 *VALUES[8];
} TEXFUNCTIONARGS;
typedef struct record8 {
  struct record13 *ARG;
  struct record8 *NEXT;
} TEXREADARGS;
typedef struct record9 {
  struct record13 *ARG;
  struct record9 *NEXT;
} TEXWRITEARGS;
typedef struct record10 {
  struct record13 *ARG1;
  struct record13 *ARG2;
  struct record13 *ARG3;
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
typedef struct record11 {
  struct record13 *PARENT;
  TLXTOKENID OP;
} TEXUNARYOP;
typedef struct record12 {
  struct record13 *LEFT;
  struct record13 *RIGHT;
  TLXTOKENID OP;
} TEXBINARYOP;
typedef enum enum4 { XCIMMEDIATE, XCTOSTRING, XCTOREAL, XCWITHTMPVAR, XCSUBRANGE, XCSET, XCVARIABLE, XCFIELD, XCARRAY, XCPOINTER, XCSTRINGCHAR, XCFNREF, XCFNCALL, XCPSEUDOFNREF, XCPSEUDOFNCALL, XCUNARYOP, XCBINARYOP } TEXPRESSIONCLASS;
typedef struct record13 {
  struct record17 *TYPEPTR;
  PBoolean ISASSIGNABLE;
  PBoolean ISFUNCTIONRESULT;
  TEXPRESSIONCLASS CLS;
  union {
    struct {
      TEXIMMEDIATE IMMEDIATE;
    };
    struct {
      struct record13 *TOSTRPARENT;
    };
    struct {
      struct record13 *TOREALPARENT;
    };
    struct {
      struct record13 *TMPVAR;
      struct record13 *TMPVARVALUE;
      struct record13 *TMPVARCHILD;
    };
    struct {
      struct record13 *SUBRANGEPARENT;
    };
    struct {
      struct record13 *SETBASE;
      TEXSETEXPRBOUNDSOBJ *SETBOUNDS;
    };
    struct {
      struct record22 *VARPTR;
    };
    struct {
      struct record13 *RECEXPR;
      PInteger RECFIELDNUM;
    };
    struct {
      struct record13 *ARRAYEXPR;
      struct record13 *ARRAYINDEX;
    };
    struct {
      struct record13 *POINTEREXPR;
    };
    struct {
      struct record13 *STRINGEXPR;
      struct record13 *STRINGINDEX;
    };
    struct {
      struct record23 *FNPTR;
    };
    struct {
      struct record13 *FNEXPR;
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
  };
} TEXPRESSIONOBJ;
typedef struct record14 {
  PString NAME;
} TPSIDENTIFIER;
typedef struct record20 *TPSRECPTR;
typedef struct record21 *TPSCONSTPTR;
typedef struct record24 *TPSWITHVARPTR;
typedef struct record25 *TPSNAMEPTR;
typedef enum enum5 { TTCBOOLEAN, TTCINTEGER, TTCREAL, TTCCHAR, TTCSTRING, TTCTEXT, TTCENUM, TTCRANGE, TTCSET, TTCRECORD, TTCARRAY, TTCPOINTER, TTCNIL, TTCPOINTERUNKNOWN } TPSTYPECLASS;
typedef struct record17 {
  PString NAME;
  struct record17 *ALIASFOR;
  PBoolean WASUSED;
  TPSTYPECLASS CLS;
  union {
    struct {
      struct record18 *ENUMPTR;
    };
    struct {
      struct record15 {
        PInteger FIRST;
        PInteger LAST;
        struct record17 *BASETYPEPTR;
      } RANGEDEF;
    };
    struct {
      struct record17 *ELEMENTTYPEPTR;
    };
    struct {
      struct record20 *RECPTR;
    };
    struct {
      struct record16 {
        struct record17 *INDEXTYPEPTR;
        struct record17 *VALUETYPEPTR;
      } ARRAYDEF;
    };
    struct {
      struct record17 *POINTEDTYPEPTR;
    };
    struct {
      PString *TARGETNAME;
    };
  };
} TPSTYPE;
typedef struct record18 {
  PInteger SIZE;
  PString VALUES[128];
  PInteger ID;
  PBoolean HASBEENDEFINED;
  PBoolean VALUESHAVEBEENOUTPUT;
  PInteger REFCOUNT;
} TPSENUMDEF;
typedef struct record19 {
  PString NAME;
  TPSTYPE *TYPEPTR;
} TPSRECORDFIELD;
typedef struct record20 {
  PInteger SIZE;
  TPSRECORDFIELD FIELDS[32];
  PInteger NUMVARIANTS;
  PInteger VARIANTBOUNDS[32];
  PInteger ID;
  PBoolean HASBEENDEFINED;
  PInteger REFCOUNT;
} TPSRECORDDEF;
typedef struct record21 {
  PString NAME;
  TEXPRESSIONOBJ *VALUE;
} TPSCONSTANT;
typedef struct record22 {
  PString NAME;
  TPSTYPE *TYPEPTR;
  PBoolean ISREFERENCE;
  PBoolean ISCONSTANT;
  PBoolean WASINITIALIZED;
  PBoolean WASUSED;
} TPSVARIABLE;
typedef struct record23 {
  PString NAME;
  PInteger ARGCOUNT;
  TPSVARIABLE ARGS[8];
  TPSTYPE *RETURNTYPEPTR;
  PBoolean ISDECLARATION;
  PBoolean WASUSED;
} TPSFUNCTION;
typedef struct record24 {
  TPSVARIABLE *VARPTR;
} TPSWITHVAR;
typedef enum enum6 { TNCTYPE, TNCVARIABLE, TNCCONSTANT, TNCENUMVAL, TNCFUNCTION, TNCPSEUDOFN } TPSNAMECLASS;
typedef struct record25 {
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
      PInteger ORDINAL;
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
typedef struct record26 {
  PInteger ENUMCTR;
  PInteger RECORDCTR;
  PInteger TMPVARCTR;
} TPSCOUNTERS;
typedef struct record27 *TPSDEFPTR;
typedef enum enum8 { TDCNAME, TDCTYPE, TDCCONSTANT, TDCVARIABLE, TDCFUNCTION, TDCWITHVAR, TDCSCOPEBOUNDARY } TPSDEFCLASS;
typedef struct record27 {
  struct record27 *PREV;
  struct record27 *NEXT;
  TPSDEFCLASS CLS;
  union {
    struct {
      TPSNAME *NAMEPTR;
    };
    struct {
      TPSTYPE *TYPEPTR;
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
      PBoolean TEMPORARYSCOPE;
      TPSCOUNTERS COUNTERS;
      TPSFUNCTION *CURRENTFN;
    };
  };
} TPSDEFENTRY;
typedef struct record28 {
  TPSDEFENTRY *LATEST;
  TPSFUNCTION *CURRENTFN;
  TPSCOUNTERS COUNTERS;
} TPSDEFS;

const char* enumvalues1[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKINTEGER", "TKREAL", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAND", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKSHL", "TKSHR", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH", "TKXOR" };
const char* enumvalues2[] = { "TPFABS", "TPFCONCAT", "TPFDISPOSE", "TPFNEW", "TPFORD", "TPFPRED", "TPFRANDOM", "TPFREAD", "TPFREADLN", "TPFSQR", "TPFSTR", "TPFSUCC", "TPFVAL", "TPFWRITE", "TPFWRITELN" };
const char* enumvalues3[] = { "XICNIL", "XICBOOLEAN", "XICINTEGER", "XICREAL", "XICCHAR", "XICSTRING", "XICENUM", "XICSET" };
const char* enumvalues4[] = { "XCIMMEDIATE", "XCTOSTRING", "XCTOREAL", "XCWITHTMPVAR", "XCSUBRANGE", "XCSET", "XCVARIABLE", "XCFIELD", "XCARRAY", "XCPOINTER", "XCSTRINGCHAR", "XCFNREF", "XCFNCALL", "XCPSEUDOFNREF", "XCPSEUDOFNCALL", "XCUNARYOP", "XCBINARYOP" };
const char* enumvalues5[] = { "TTCBOOLEAN", "TTCINTEGER", "TTCREAL", "TTCCHAR", "TTCSTRING", "TTCTEXT", "TTCENUM", "TTCRANGE", "TTCSET", "TTCRECORD", "TTCARRAY", "TTCPOINTER", "TTCNIL", "TTCPOINTERUNKNOWN" };
const char* enumvalues6[] = { "TNCTYPE", "TNCVARIABLE", "TNCCONSTANT", "TNCENUMVAL", "TNCFUNCTION", "TNCPSEUDOFN" };
const char* enumvalues7[] = { "TCTENUM", "TCTRECORD", "TCTTMPVAR" };
const char* enumvalues8[] = { "TDCNAME", "TDCTYPE", "TDCCONSTANT", "TDCVARIABLE", "TDCFUNCTION", "TDCWITHVAR", "TDCSCOPEBOUNDARY" };

void DISPOSEEXPR(TEXPRESSIONOBJ **EXPR);
TEXPRESSIONOBJ *COPYEXPR(TEXPRESSIONOBJ *EXPR);
PString DESCRIBEEXPR(TEXPRESSIONOBJ *EXPR, PInteger LEVELS);
void EXMARKINITIALIZED(TEXPRESSIONOBJ *LHS);
TEXPRESSIONOBJ *EXUNARYOP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);
TEXPRESSIONOBJ *EXBINARYOP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
PInteger EXGETORDINAL(TEXPRESSIONOBJ *EXPR);
TEXPRESSIONOBJ *EXGETANTIORDINAL(PInteger ORDINAL, TPSTYPE *TYPEPTR);
TEXPRESSIONOBJ *PF_PARSE(TEXPRESSIONOBJ *FN);
PString PF_DESCRIBENAME(TEXPRESSIONOBJ *FN);
PString PF_DESCRIBECALL(TEXPRESSIONOBJ *EXPR, PInteger LEVELS);
TEXPRESSIONOBJ *PFORD(TEXPRESSIONOBJ *ARG);
TEXPRESSIONOBJ *PFPRED(TEXPRESSIONOBJ *ARG);
TEXPRESSIONOBJ *PFSUCC(TEXPRESSIONOBJ *ARG);

typedef struct record29 {
  PFile SRC;
  PString NAME;
  TLXPOS POS;
} TLXINPUTFILE;

struct record31 {
  PString LINE;
  TLXTOKEN TOKEN;
  TLXINPUTFILE INPUT;
  struct record30 {
    PBoolean EXISTS;
    TLXINPUTFILE INPUT;
  } PREV;
} LEXER;

PString LXTOKENNAME(TLXTOKENID ID) {
  PString RESULT;
  PString NAME;
  STR_e(ID, enumvalues1, 0, &NAME);
  RESULT = NAME;
  return RESULT;
}

PString LXPOSSTR(TLXPOS POS) {
  PString RESULT;
  PString ROW;
  PString COL;
  STR_i(POS.ROW, 0, &ROW);
  STR_i(POS.COL, 0, &COL);
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

PBoolean LXISALPHA(PChar CHR) {
  PBoolean RESULT;
  RESULT = 'A' <= CHR && CHR <= 'Z' || 'a' <= CHR && CHR <= 'z';
  return RESULT;
}

PBoolean LXISDIGIT(PChar CHR) {
  PBoolean RESULT;
  RESULT = '0' <= CHR && CHR <= '9';
  return RESULT;
}

PBoolean LXISHEXDIGIT(PChar CHR) {
  PBoolean RESULT;
  RESULT = '0' <= CHR && CHR <= '9' || 'A' <= CHR && CHR <= 'F' || 'a' <= CHR && CHR <= 'f';
  return RESULT;
}

PBoolean LXISALPHANUM(PChar CHR) {
  PBoolean RESULT;
  RESULT = LXISALPHA(CHR) || LXISDIGIT(CHR);
  return RESULT;
}

PBoolean LXISIDENTIFIERFIRST(PChar CHR) {
  PBoolean RESULT;
  RESULT = LXISALPHA(CHR) || CHR == '_';
  return RESULT;
}

PBoolean LXISIDENTIFIERCHAR(PChar CHR) {
  PBoolean RESULT;
  RESULT = LXISALPHANUM(CHR) || CHR == '_';
  return RESULT;
}

PBoolean LXISTOKENWAITING() {
  PBoolean RESULT;
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

void LXGETSYMBOL(TLXTOKENID ID, PInteger LENGTH) {
  LEXER.TOKEN.ID = ID;
  LEXER.TOKEN.VALUE = COPY(&LEXER.LINE, 1, LENGTH);
  LEXER.TOKEN.POS = LEXER.INPUT.POS;
  DELETE(&LEXER.LINE, 1, LENGTH);
  LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + LENGTH;
}

void LXGETIDENTIFIER() {
  PChar CHR;
  PInteger POS;
  PBoolean INTOKEN;
  POS = 0;
  INTOKEN = 1;
  while (POS < LENGTH(&LEXER.LINE) && INTOKEN) {
    CHR = LEXER.LINE.chr[POS + 1];
    INTOKEN = LXISIDENTIFIERCHAR(CHR);
    if (INTOKEN) POS = POS + 1;
  }
  LXGETSYMBOL(TKIDENTIFIER, POS);
  do {
    PInteger first = 1;
    PInteger last = LENGTH(&LEXER.TOKEN.VALUE);
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
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "SHL")) == 0) LEXER.TOKEN.ID = TKSHL;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "SHR")) == 0) LEXER.TOKEN.ID = TKSHR;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "THEN")) == 0) LEXER.TOKEN.ID = TKTHEN;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(2, "TO")) == 0) LEXER.TOKEN.ID = TKTO;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "TYPE")) == 0) LEXER.TOKEN.ID = TKTYPE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "UNTIL")) == 0) LEXER.TOKEN.ID = TKUNTIL;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "VAR")) == 0) LEXER.TOKEN.ID = TKVAR;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(5, "WHILE")) == 0) LEXER.TOKEN.ID = TKWHILE;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(4, "WITH")) == 0) LEXER.TOKEN.ID = TKWITH;
  else if (cmp_ss(LEXER.TOKEN.VALUE, str_make(3, "XOR")) == 0) LEXER.TOKEN.ID = TKXOR;
}

void LXGETNUMBER() {
  PInteger ASINT;
  PReal ASREAL;
  PInteger ASINTCODE;
  PInteger ASREALCODE;
  VAL_i(&LEXER.LINE, &ASINT, &ASINTCODE);
  VAL_r(&LEXER.LINE, &ASREAL, &ASREALCODE);
  if (ASINTCODE == 0) ASINTCODE = LENGTH(&LEXER.LINE) + 1;
  if (ASREALCODE == 0) ASREALCODE = LENGTH(&LEXER.LINE) + 1;
  if (ASREALCODE > ASINTCODE) LXGETSYMBOL(TKREAL, ASREALCODE - 1);
  else LXGETSYMBOL(TKINTEGER, ASINTCODE - 1);
}

void LXGETSTRING() {
  PChar CHR;
  PInteger POS;
  PInteger LAST;
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
      if ('@' <= CHR && CHR <= '_' || 'a' <= CHR && CHR <= 'z') {
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
  PBoolean DONE;
  PInteger DELIMITERLENGTH;
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
  PChar CHR;
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
struct record32 {
  TPSTYPE *PTNIL;
  TPSTYPE *PTBOOLEAN;
  TPSTYPE *PTINTEGER;
  TPSTYPE *PTREAL;
  TPSTYPE *PTCHAR;
  TPSTYPE *PTSTRING;
  TPSTYPE *PTTEXT;
} PRIMITIVETYPES;

PInteger DEFCOUNTER(TPSCOUNTERTYPE COUNTERTYPE) {
  PInteger RESULT;
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

TPSENUMDEF *NEWENUM(TPSENUMDEF ENUM) {
  TPSENUMDEF *RESULT;
  RESULT = malloc(sizeof(TPSENUMDEF));
  *RESULT = ENUM;
  RESULT->ID = DEFCOUNTER(TCTENUM);
  RESULT->REFCOUNT = 1;
  return RESULT;
}

void DISPOSEENUM(TPSENUMDEF *PTR) {
  PTR->REFCOUNT = PTR->REFCOUNT - 1;
  if (PTR->REFCOUNT == 0) free(PTR);
}

TPSRECORDDEF *NEWRECORD(TPSRECORDDEF REC) {
  TPSRECORDDEF *RESULT;
  RESULT = malloc(sizeof(TPSRECORDDEF));
  *RESULT = REC;
  RESULT->ID = DEFCOUNTER(TCTRECORD);
  RESULT->REFCOUNT = 1;
  return RESULT;
}

void DISPOSERECORD(TPSRECORDDEF *PTR) {
  PTR->REFCOUNT = PTR->REFCOUNT - 1;
  if (PTR->REFCOUNT == 0) free(PTR);
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

void _DISPOSETYPE(TPSTYPE **TYPEPTR) {
  if ((*TYPEPTR)->CLS == TTCENUM) DISPOSEENUM((*TYPEPTR)->ENUMPTR);
  else if ((*TYPEPTR)->CLS == TTCRECORD) DISPOSERECORD((*TYPEPTR)->RECPTR);
  free(*TYPEPTR);
}

void _DISPOSEDEF(TPSDEFENTRY *DEF) {
  switch (DEF->CLS) {
    case TDCNAME:
      free(DEF->NAMEPTR);
      break;
    case TDCTYPE:
      _DISPOSETYPE(&DEF->TYPEPTR);
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

PBoolean _DELETEDEF(TPSDEFENTRY *DELETEDDEF) {
  PBoolean RESULT;
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

void _STARTSCOPE(PBoolean TEMPORARY, TPSFUNCTION *NEWFUNCTION) {
  TPSDEFENTRY *DEF;
  DEF = _ADDDEF(TDCSCOPEBOUNDARY);
  DEF->TEMPORARYSCOPE = TEMPORARY;
  DEF->COUNTERS = DEFS.COUNTERS;
  DEF->CURRENTFN = DEFS.CURRENTFN;
  if (!TEMPORARY) DEFS.CURRENTFN = NEWFUNCTION;
}

void _CLOSESCOPE(PBoolean TEMPORARY) {
  TPSDEFENTRY DELETEDDEF;
  PBoolean DELETED;
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

TPSNAME *_FINDNAME(PString NAME, PBoolean REQUIRED, PBoolean FROMLOCALSCOPE) {
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
      INTERNALERROR(cat_ss(str_make(24, "Name class mismatch for "), NAMEPTR->NAME));
      break;
  }
  RESULT = NAMEPTR;
  return RESULT;
}

TPSNAME *FINDNAMEINLOCALSCOPE(PString NAME, PBoolean REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _FINDNAME(NAME, REQUIRED, 1);
  return RESULT;
}

TPSNAME *FINDNAMEOFCLASSINLOCALSCOPE(PString NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _CHECKNAMECLASS(FINDNAMEINLOCALSCOPE(NAME, REQUIRED), CLS);
  return RESULT;
}

TPSNAME *FINDNAME(PString NAME, PBoolean REQUIRED) {
  TPSNAME *RESULT;
  RESULT = _FINDNAME(NAME, REQUIRED, 0);
  return RESULT;
}

TPSNAME *FINDNAMEOFCLASS(PString NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
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

TPSNAME *ADDENUMVALNAME(PInteger ORDINAL, TPSTYPE *TYPEIDX) {
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
  RESULT = *TYPEPTR;
  if (RESULT.CLS == TTCPOINTERUNKNOWN) {
    RESULT.TARGETNAME = malloc(sizeof(PString));
    *RESULT.TARGETNAME = *TYPEPTR->TARGETNAME;
  }
  else if (RESULT.CLS == TTCENUM) RESULT.ENUMPTR->REFCOUNT = RESULT.ENUMPTR->REFCOUNT + 1;
  else if (RESULT.CLS == TTCRECORD) RESULT.RECPTR->REFCOUNT = RESULT.RECPTR->REFCOUNT + 1;
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

PBoolean ISINTEGERTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCINTEGER;
  return RESULT;
}

PBoolean ISREALTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCREAL;
  return RESULT;
}

PBoolean ISNUMERICTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISINTEGERTYPE(TYPEPTR) || ISREALTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISSTRINGTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCSTRING;
  return RESULT;
}

PBoolean ISCHARTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCCHAR;
  return RESULT;
}

PBoolean ISSTRINGYTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISSTRINGTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISBOOLEANTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCBOOLEAN;
  return RESULT;
}

PBoolean ISTEXTTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCTEXT;
  return RESULT;
}

PBoolean ISENUMTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCENUM;
  return RESULT;
}

PBoolean ISRANGETYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCRANGE;
  return RESULT;
}

TPSTYPE *GETFUNDAMENTALTYPE(TPSTYPE *TYPEPTR) {
  TPSTYPE *RESULT;
  while (ISRANGETYPE(TYPEPTR)) TYPEPTR = TYPEPTR->RANGEDEF.BASETYPEPTR;
  RESULT = TYPEPTR;
  return RESULT;
}

PBoolean ISSETTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCSET;
  return RESULT;
}

PBoolean ISRECORDTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCRECORD;
  return RESULT;
}

PBoolean ISARRAYTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
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

PBoolean ISPOINTERTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCPOINTER;
  return RESULT;
}

PBoolean ISNILTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCNIL;
  return RESULT;
}

PBoolean ISPOINTERYTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
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

PBoolean ISPOINTERUNKNOWNTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != (void*)0 && TYPEPTR->CLS == TTCPOINTERUNKNOWN;
  return RESULT;
}

PBoolean ISORDINALTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISINTEGERTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISBOUNDEDTYPE(TPSTYPE *TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

PInteger GETTYPELOWBOUND(TPSTYPE *TYPEPTR) {
  PInteger RESULT;
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
      RESULT = TYPEPTR->RANGEDEF.FIRST;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(21, "Type is not bounded: "), TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

PInteger GETTYPEHIGHBOUND(TPSTYPE *TYPEPTR) {
  PInteger RESULT;
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
      RESULT = TYPEPTR->RANGEDEF.LAST;
      break;
    default:
      COMPILEERROR(cat_ss(str_make(21, "Type is not bounded: "), TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

PInteger GETBOUNDEDTYPESIZE(TPSTYPE *TYPEPTR) {
  PInteger RESULT;
  RESULT = GETTYPEHIGHBOUND(TYPEPTR) - GETTYPELOWBOUND(TYPEPTR) + 1;
  return RESULT;
}

PBoolean ISSAMETYPE(TPSTYPE *A, TPSTYPE *B) {
  PBoolean RESULT;
  if (A == (void*)0 || B == (void*)0) RESULT = A == B;
  else {
    while (A->ALIASFOR != (void*)0) A = A->ALIASFOR;
    while (B->ALIASFOR != (void*)0) B = B->ALIASFOR;
    RESULT = A == B || ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE(A->POINTEDTYPEPTR, B->POINTEDTYPEPTR) || (ISRANGETYPE(A) && ISRANGETYPE(B) && ISSAMETYPE(GETFUNDAMENTALTYPE(A), GETFUNDAMENTALTYPE(B)) && GETTYPELOWBOUND(A) == GETTYPELOWBOUND(B) && GETTYPEHIGHBOUND(A) == GETTYPEHIGHBOUND(B) || ISSETTYPE(A) && ISSETTYPE(B) && ISSAMETYPE(A->ELEMENTTYPEPTR, B->ELEMENTTYPEPTR));
  }
  return RESULT;
}

PBoolean AREPOINTERSCOMPATIBLE(TPSTYPE *A, TPSTYPE *B) {
  PBoolean RESULT;
  RESULT = ISPOINTERYTYPE(A) && ISPOINTERYTYPE(B) && (ISNILTYPE(A) || ISNILTYPE(B) || ISSAMETYPE(A, B));
  return RESULT;
}

PString UNPARSECHAR(PChar CHR) {
  PString RESULT;
  PString CHNUM;
  if (CHR == '\'') RESULT = str_make(4, "''''");
  else if (CHR < ' ') {
    STR_i((int)CHR, 0, &CHNUM);
    RESULT = cat_cs('#', CHNUM);
  }
  else RESULT = cat_sc(cat_cc('\'', CHR), '\'');
  return RESULT;
}

PString UNPARSESTRING(PString ST) {
  PString RESULT;
  PInteger POS;
  PString CHNUM;
  PBoolean QUOTED;
  QUOTED = 0;
  RESULT = str_make(0, "");
  do {
    PInteger first = 1;
    PInteger last = LENGTH(&ST);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (ST.chr[POS] < ' ') {
            if (QUOTED) {
              QUOTED = 0;
              RESULT = cat_sc(RESULT, '\'');
            }
            STR_i((int)ST.chr[POS], 0, &CHNUM);
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

PString _ANTIORDINAL(PInteger ORDINAL, TPSTYPE *TYPEPTR) {
  PString RESULT;
  RESULT = DESCRIBEEXPR(EXGETANTIORDINAL(ORDINAL, TYPEPTR), 100);
  return RESULT;
}

PString DEEPTYPENAME(TPSTYPE *TYPEPTR, PBoolean USEORIGINAL) {
  PString RESULT;
  TPSTYPE TYP;
  PString RET;
  PInteger POS;
  do {
    TYP = *TYPEPTR;
    TYPEPTR = TYP.ALIASFOR;
  } while (!(!USEORIGINAL || TYPEPTR == (void*)0));
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) RESULT = TYP.NAME;
  else if (TYP.CLS == TTCENUM) {
    RET = str_of('(');
    do {
      PInteger first = 0;
      PInteger last = TYP.ENUMPTR->SIZE - 1;
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
    RESULT = cat_ss(cat_ss(_ANTIORDINAL(TYP.RANGEDEF.FIRST, TYP.RANGEDEF.BASETYPEPTR), str_make(2, "..")), _ANTIORDINAL(TYP.RANGEDEF.LAST, TYP.RANGEDEF.BASETYPEPTR));
  }
  else if (TYP.CLS == TTCSET) {
    if (TYP.ELEMENTTYPEPTR == (void*)0) RESULT = str_make(9, "SET OF []");
    else RESULT = cat_ss(str_make(7, "SET OF "), DEEPTYPENAME(TYP.ELEMENTTYPEPTR, 0));
  }
  else if (TYP.CLS == TTCRECORD) {
    RET = str_make(7, "RECORD ");
    do {
      PInteger first = 1;
      PInteger last = TYP.RECPTR->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 1) RET = cat_sc(RET, ',');
            RET = cat_ss(RET, DEEPTYPENAME(TYP.RECPTR->FIELDS[subrange(POS, 1, 32) - 1].TYPEPTR, 1));
            RET = cat_ss(cat_sc(RET, ':'), TYP.RECPTR->FIELDS[subrange(POS, 1, 32) - 1].NAME);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    RESULT = cat_ss(RET, str_make(4, " END"));
  }
  else if (TYP.CLS == TTCARRAY) {
    RET = cat_ss(cat_ss(cat_ss(str_make(7, "ARRAY ["), DEEPTYPENAME(TYP.ARRAYDEF.INDEXTYPEPTR, 0)), str_make(5, "] OF ")), DEEPTYPENAME(TYP.ARRAYDEF.VALUETYPEPTR, 0));
    RESULT = RET;
  }
  else if (TYP.CLS == TTCPOINTER) RESULT = cat_cs('^', DEEPTYPENAME(TYP.POINTEDTYPEPTR, 1));
  else {
    STR_e(TYP.CLS, enumvalues5, 0, &RET);
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

TPSTYPE *ADDTYPE(TPSTYPE TYP) {
  TPSTYPE *RESULT;
  TPSTYPE *TYPEPTR;
  PInteger ENUMPOS;
  TYPEPTR = _ADDDEF(TDCTYPE)->TYPEPTR;
  *TYPEPTR = TYP;
  RESULT = TYPEPTR;
  if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) {
    if (FINDNAMEINLOCALSCOPE(TYP.NAME, 0) != (void*)0) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), TYP.NAME), str_make(16, " already defined")));
    ADDTYPENAME(TYP.NAME, TYPEPTR);
  }
  if (TYP.CLS == TTCENUM && TYP.ALIASFOR == (void*)0) do {
    PInteger first = 0;
    PInteger last = TYP.ENUMPTR->SIZE - 1;
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

PBoolean ISSAMEFUNCTIONDEFINITION(TPSFUNCTION *DECLPTR, TPSFUNCTION FUN) {
  PBoolean RESULT;
  TPSFUNCTION DECL;
  PBoolean SAME;
  PInteger POS;
  DECL = *DECLPTR;
  SAME = ISSAMETYPE(DECL.RETURNTYPEPTR, FUN.RETURNTYPEPTR) && DECL.ARGCOUNT == FUN.ARGCOUNT;
  do {
    PInteger first = 1;
    PInteger last = DECL.ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        SAME = SAME && ISSAMETYPE(DECL.ARGS[subrange(POS, 1, 8) - 1].TYPEPTR, FUN.ARGS[subrange(POS, 1, 8) - 1].TYPEPTR) && DECL.ARGS[subrange(POS, 1, 8) - 1].ISREFERENCE == FUN.ARGS[subrange(POS, 1, 8) - 1].ISREFERENCE;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  RESULT = SAME;
  return RESULT;
}

PBoolean HASFORWARDDECLARATION(PString NAME) {
  PBoolean RESULT;
  TPSNAME *NAMEPTR;
  NAMEPTR = FINDNAMEOFCLASSINLOCALSCOPE(NAME, TNCFUNCTION, 0);
  RESULT = NAMEPTR != (void*)0 && NAMEPTR->FNPTR->ISDECLARATION;
  return RESULT;
}

TPSFUNCTION *ADDFUNCTION(TPSFUNCTION FUN) {
  TPSFUNCTION *RESULT;
  TPSNAME *NAMEPTR;
  TPSFUNCTION *FNPTR;
  PBoolean ISPROCEDURE;
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

PInteger FINDFIELD(TPSTYPE *TYPEPTR, PString NAME, PBoolean REQUIRED) {
  PInteger RESULT;
  PInteger POS;
  PInteger RET;
  if (TYPEPTR->CLS != TTCRECORD) COMPILEERROR(cat_ss(str_make(14, "Not a record: "), TYPENAME(TYPEPTR)));
  {
    TPSRECORDDEF *with1 = TYPEPTR->RECPTR;
    {
      RET = 0;
      POS = with1->SIZE;
      while (POS >= 1 && RET == 0) {
        if (cmp_ss(NAME, with1->FIELDS[subrange(POS, 1, 32) - 1].NAME) == 0) RET = POS;
        POS = POS - 1;
      }
    }
  }
  if (REQUIRED && RET == 0) COMPILEERROR(cat_ss(str_make(17, "Field not found: "), NAME));
  RESULT = RET;
  return RESULT;
}

TPSTYPE *FINDFIELDTYPE(TPSTYPE *TYPEPTR, PString NAME, PBoolean REQUIRED) {
  TPSTYPE *RESULT;
  PInteger POS;
  POS = FINDFIELD(TYPEPTR, NAME, REQUIRED);
  if (POS == 0) RESULT = (void*)0;
  else RESULT = TYPEPTR->RECPTR->FIELDS[subrange(POS, 1, 32) - 1].TYPEPTR;
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
  STR_i(DEFCOUNTER(TCTTMPVAR), 0, &TMPVARNUM);
  TMPVAR.NAME = cat_ss(str_make(4, "with"), TMPVARNUM);
  TMPVAR.TYPEPTR = BASE->TYPEPTR;
  TMPVAR.ISCONSTANT = 0;
  TMPVAR.ISREFERENCE = BASE->ISASSIGNABLE;
  TMPVAR.WASINITIALIZED = 1;
  TMPVAR.WASUSED = 1;
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

TPSVARIABLE *ADDTMPVARIABLE(PString PREFIX, TPSTYPE *TYPEPTR) {
  TPSVARIABLE *RESULT;
  PString VARNUM;
  STR_i(DEFCOUNTER(TCTTMPVAR), 0, &VARNUM);
  RESULT = ADDVARIABLE(MAKEVARIABLE(cat_ss(PREFIX, VARNUM), TYPEPTR));
  return RESULT;
}

TPSVARIABLE _MAKEARG(PString NAME, TPSTYPE *TYPEPTR, PBoolean ISREF, PBoolean ISCONST) {
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

TPSFUNCTION MAKEPROCEDURE0(PString NAME) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = NAME;
  RESULT.ARGCOUNT = 0;
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
void OUTFORBEGIN(TEXPRESSIONOBJ *ITER, TEXPRESSIONOBJ *FIRSTEXPR, TEXPRESSIONOBJ *LASTEXPR, PBoolean ASCENDING);
void OUTFOREND(TEXPRESSIONOBJ *ITER, PBoolean ASCENDING);
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
  if (TPFWRITE <= CALL->PSEUDOFN && CALL->PSEUDOFN <= TPFWRITELN) {
    READARG = CALL->READARGS;
    while (READARG != (void*)0) {
      NEXTREADARG = READARG->NEXT;
      DISPOSEEXPR(&READARG->ARG);
      free(READARG);
      READARG = NEXTREADARG;
    }
  }
  else if (TPFREAD <= CALL->PSEUDOFN && CALL->PSEUDOFN <= TPFREADLN) {
    WRITEARG = CALL->WRITEARGS;
    while (WRITEARG != (void*)0) {
      NEXTWRITEARG = WRITEARG->NEXT;
      DISPOSEEXPR(&WRITEARG->ARG);
      free(WRITEARG);
      WRITEARG = NEXTWRITEARG;
    }
  }
}

void _DISPOSEIMMEDIATE(TEXIMMEDIATE *IMM) {
  TEXSETIMMBOUNDSOBJ *BOUNDS;
  if (IMM->CLS == XICSET) {
    while (IMM->SETBOUNDS != (void*)0) {
      BOUNDS = IMM->SETBOUNDS->NEXT;
      free(IMM->SETBOUNDS);
      IMM->SETBOUNDS = BOUNDS;
    }
  }
}

void _DISPOSEBOUNDS(TEXSETEXPRBOUNDSOBJ *BOUNDS) {
  TEXSETEXPRBOUNDSOBJ *NEXT;
  while (BOUNDS != (void*)0) {
    NEXT = BOUNDS->NEXT;
    DISPOSEEXPR(&BOUNDS->FIRST);
    if (BOUNDS->LAST != (void*)0) DISPOSEEXPR(&BOUNDS->LAST);
    free(BOUNDS);
    BOUNDS = NEXT;
  }
}

void DISPOSEEXPR(TEXPRESSIONOBJ **EXPR) {
  PInteger POS;
  switch ((*EXPR)->CLS) {
    case XCIMMEDIATE:
      _DISPOSEIMMEDIATE(&(*EXPR)->IMMEDIATE);
      break;
    case XCTOSTRING:
      DISPOSEEXPR(&(*EXPR)->TOSTRPARENT);
      break;
    case XCTOREAL:
      DISPOSEEXPR(&(*EXPR)->TOREALPARENT);
      break;
    case XCWITHTMPVAR:
      {
        DISPOSEEXPR(&(*EXPR)->TMPVAR);
        DISPOSEEXPR(&(*EXPR)->TMPVARVALUE);
        DISPOSEEXPR(&(*EXPR)->TMPVARCHILD);
      }
      break;
    case XCSUBRANGE:
      DISPOSEEXPR(&(*EXPR)->SUBRANGEPARENT);
      break;
    case XCSET:
      {
        DISPOSEEXPR(&(*EXPR)->SETBASE);
        _DISPOSEBOUNDS((*EXPR)->SETBOUNDS);
      }
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
          PInteger first = 1;
          PInteger last = (*EXPR)->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              DISPOSEEXPR(&(*EXPR)->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]);
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
  if (TPFWRITE <= CALL->PSEUDOFN && CALL->PSEUDOFN <= TPFWRITELN) {
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
  else if (TPFREAD <= CALL->PSEUDOFN && CALL->PSEUDOFN <= TPFREADLN) {
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

TEXIMMEDIATE _COPYIMMEDIATE(const TEXIMMEDIATE *IMM) {
  TEXIMMEDIATE RESULT;
  TEXSETIMMBOUNDSOBJ *SRC;
  TEXSETIMMBOUNDSOBJ *DST;
  RESULT = *IMM;
  if (IMM->CLS == XICSET) {
    SRC = IMM->SETBOUNDS;
    RESULT.SETBOUNDS = malloc(sizeof(TEXSETIMMBOUNDSOBJ));
    DST = RESULT.SETBOUNDS;
    while (SRC != (void*)0) {
      *DST = *SRC;
      SRC = SRC->NEXT;
      if (SRC != (void*)0) {
        DST->NEXT = malloc(sizeof(TEXSETIMMBOUNDSOBJ));
        DST = DST->NEXT;
      }
    }
  }
  return RESULT;
}

TEXSETEXPRBOUNDSOBJ *_COPYBOUNDS(TEXSETEXPRBOUNDSOBJ *BOUNDS) {
  TEXSETEXPRBOUNDSOBJ *RESULT;
  TEXSETEXPRBOUNDSOBJ *SRC;
  TEXSETEXPRBOUNDSOBJ *DST;
  SRC = BOUNDS;
  DST = (void*)0;
  while (SRC != (void*)0) {
    if (DST == (void*)0) {
      DST = malloc(sizeof(TEXSETEXPRBOUNDSOBJ));
      RESULT = DST;
    }
    else {
      DST->NEXT = malloc(sizeof(TEXSETEXPRBOUNDSOBJ));
      DST = DST->NEXT;
    }
    DST->FIRST = COPYEXPR(SRC->FIRST);
    if (SRC->LAST != (void*)0) DST->LAST = COPYEXPR(SRC->LAST);
    else DST->LAST = (void*)0;
    DST->NEXT = (void*)0;
    SRC = SRC->NEXT;
  }
  return RESULT;
}

TEXPRESSIONOBJ *COPYEXPR(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *COPY;
  PInteger POS;
  COPY = _NEWEXPR(EXPR->CLS);
  COPY->TYPEPTR = EXPR->TYPEPTR;
  COPY->ISASSIGNABLE = EXPR->ISASSIGNABLE;
  COPY->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      COPY->IMMEDIATE = _COPYIMMEDIATE(&EXPR->IMMEDIATE);
      break;
    case XCTOSTRING:
      COPY->TOSTRPARENT = COPYEXPR(EXPR->TOSTRPARENT);
      break;
    case XCTOREAL:
      COPY->TOREALPARENT = COPYEXPR(EXPR->TOREALPARENT);
      break;
    case XCWITHTMPVAR:
      {
        COPY->TMPVAR = COPYEXPR(EXPR->TMPVAR);
        COPY->TMPVARVALUE = COPYEXPR(EXPR->TMPVARVALUE);
        COPY->TMPVARCHILD = COPYEXPR(EXPR->TMPVARCHILD);
      }
      break;
    case XCSUBRANGE:
      COPY->SUBRANGEPARENT = COPYEXPR(EXPR->SUBRANGEPARENT);
      break;
    case XCSET:
      {
        COPY->SETBASE = COPYEXPR(EXPR->SETBASE);
        COPY->SETBOUNDS = _COPYBOUNDS(EXPR->SETBOUNDS);
      }
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
          PInteger first = 1;
          PInteger last = EXPR->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              COPY->CALLARGS.VALUES[subrange(POS, 1, 8) - 1] = COPYEXPR(EXPR->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]);
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
      INTERNALERROR(cat_ss(str_make(24, "Cannot copy expression: "), DESCRIBEEXPR(EXPR, 2)));
      break;
  }
  RESULT = COPY;
  return RESULT;
}

PString _DESCRIBEIMMSETINTERNAL(TEXSETIMMBOUNDSOBJ *BOUNDS, TPSTYPE *SETOFTYPEPTR) {
  PString RESULT;
  RESULT = str_make(0, "");
  while (BOUNDS != (void*)0) {
    RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXGETANTIORDINAL(BOUNDS->FIRST, SETOFTYPEPTR), 100));
    if (BOUNDS->FIRST != BOUNDS->LAST) RESULT = cat_ss(cat_ss(RESULT, str_make(2, "..")), DESCRIBEEXPR(EXGETANTIORDINAL(BOUNDS->LAST, SETOFTYPEPTR), 100));
    BOUNDS = BOUNDS->NEXT;
    if (BOUNDS != (void*)0) RESULT = cat_ss(RESULT, str_make(2, ", "));
  }
  return RESULT;
}

PString _DESCRIBEIMMSET(TEXSETIMMBOUNDSOBJ *BOUNDS, TPSTYPE *SETOFTYPEPTR) {
  PString RESULT;
  RESULT = cat_sc(cat_cs('[', _DESCRIBEIMMSETINTERNAL(BOUNDS, SETOFTYPEPTR)), ']');
  return RESULT;
}

PString _DESCRIBEIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  PString RESULT;
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICNIL:
        RESULT = str_make(3, "nil");
        break;
      case XICBOOLEAN:
        STR_b(with1->BOOLEANVAL, 0, &RESULT);
        break;
      case XICINTEGER:
        STR_i(with1->INTEGERVAL, 0, &RESULT);
        break;
      case XICREAL:
        STR_r(with1->REALVAL, 0, -1, &RESULT);
        break;
      case XICCHAR:
        RESULT = UNPARSECHAR(with1->CHARVAL);
        break;
      case XICSTRING:
        RESULT = UNPARSESTRING(with1->STRINGVAL);
        break;
      case XICENUM:
        RESULT = with1->ENUMPTR->VALUES[subrange(with1->ENUMORDINAL, 0, 127)];
        break;
      case XICSET:
        RESULT = _DESCRIBEIMMSET(with1->SETBOUNDS, with1->SETOFTYPEPTR);
        break;
      default:
        INTERNALERROR(str_make(31, "Cannot describe immediate value"));
        break;
    }
  }
  return RESULT;
}

PInteger _EXPRPRECEDENCE(TEXPRESSIONOBJ *EXPR) {
  PInteger RESULT;
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
    case XCWITHTMPVAR:
      RESULT = _EXPRPRECEDENCE(EXPR->TMPVARCHILD);
      break;
    case XCSUBRANGE:
      RESULT = _EXPRPRECEDENCE(EXPR->SUBRANGEPARENT);
      break;
    case XCSET:
      RESULT = 0;
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
        case TKXOR:
          RESULT = 4;
          break;
        case TKSHL:
          RESULT = 3;
          break;
        case TKSHR:
          RESULT = 3;
          break;
        case TKIN:
          RESULT = 5;
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

PString _DESCRIBEUNARYOPEXPR(TEXPRESSIONOBJ *EXPR, PInteger LEVELS) {
  PString RESULT;
  PBoolean USEPARENS;
  switch (EXPR->UNARY.OP) {
    case TKMINUS:
      RESULT = str_of('-');
      break;
    case TKNOT:
      RESULT = str_make(4, "not ");
      break;
    default:
      INTERNALERROR(str_make(31, "Cannot describe unary operation"));
      break;
  }
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->UNARY.PARENT);
  if (USEPARENS) RESULT = cat_sc(RESULT, '(');
  RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->UNARY.PARENT, LEVELS));
  if (USEPARENS) RESULT = cat_sc(RESULT, ')');
  return RESULT;
}

PString _DESCRIBEBINARYOPEXPR(TEXPRESSIONOBJ *EXPR, PInteger LEVELS) {
  PString RESULT;
  PBoolean USEPARENS;
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
    case TKXOR:
      RESULT = cat_ss(RESULT, str_make(5, " xor "));
      break;
    case TKSHL:
      RESULT = cat_ss(RESULT, str_make(5, " shl "));
      break;
    case TKSHR:
      RESULT = cat_ss(RESULT, str_make(5, " shr "));
      break;
    case TKIN:
      RESULT = cat_ss(RESULT, str_make(4, " in "));
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
      INTERNALERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(46, "Cannot describe binary operation for operator "), LXTOKENNAME(EXPR->BINARY.OP)), str_make(14, " and operands ")), DESCRIBEEXPR(EXPR->BINARY.LEFT, 5)), str_make(5, " and ")), DESCRIBEEXPR(EXPR->BINARY.RIGHT, 5)));
      break;
  }
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->BINARY.RIGHT);
  if (USEPARENS) RESULT = cat_sc(RESULT, '(');
  RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->BINARY.RIGHT, LEVELS - 1));
  if (USEPARENS) RESULT = cat_sc(RESULT, ')');
  return RESULT;
}

PString _DESCRIBEWITHTMPVAR(TEXPRESSIONOBJ *EXPR, PInteger LEVELS) {
  PString RESULT;
  RESULT = str_make(6, "{with ");
  while (EXPR->CLS == XCWITHTMPVAR) {
    RESULT = cat_ss(cat_ss(cat_ss(RESULT, DESCRIBEEXPR(EXPR->TMPVAR, LEVELS)), str_make(2, ":=")), DESCRIBEEXPR(EXPR->TMPVARVALUE, LEVELS - 1));
    EXPR = EXPR->TMPVARCHILD;
    if (EXPR->CLS == XCWITHTMPVAR) RESULT = cat_ss(RESULT, str_make(2, ", "));
  }
  RESULT = cat_ss(cat_ss(RESULT, str_make(2, "} ")), DESCRIBEEXPR(EXPR, LEVELS - 1));
  return RESULT;
}

PString _DESCRIBESET(TEXPRESSIONOBJ *EXPR, PInteger LEVELS) {
  PString RESULT;
  TEXSETEXPRBOUNDSOBJ *BOUNDS;
  RESULT = cat_cs('[', _DESCRIBEIMMSETINTERNAL(EXPR->SETBASE->IMMEDIATE.SETBOUNDS, EXPR->SETBASE->IMMEDIATE.SETOFTYPEPTR));
  if (EXPR->SETBASE->IMMEDIATE.SETBOUNDS != (void*)0) RESULT = cat_ss(RESULT, str_make(2, ", "));
  BOUNDS = EXPR->SETBOUNDS;
  while (BOUNDS != (void*)0) {
    RESULT = cat_ss(RESULT, DESCRIBEEXPR(BOUNDS->FIRST, LEVELS));
    if (BOUNDS->LAST != (void*)0) RESULT = cat_ss(cat_ss(RESULT, str_make(2, "..")), DESCRIBEEXPR(BOUNDS->LAST, LEVELS));
    BOUNDS = BOUNDS->NEXT;
    if (BOUNDS != (void*)0) RESULT = cat_ss(RESULT, str_make(2, ", "));
  }
  RESULT = cat_sc(RESULT, ']');
  return RESULT;
}

PString DESCRIBEEXPR(TEXPRESSIONOBJ *EXPR, PInteger LEVELS) {
  PString RESULT;
  PInteger POS;
  if (LEVELS < 1) RESULT = str_make(5, "(...)");
  else switch (EXPR->CLS) {
    case XCIMMEDIATE:
      RESULT = _DESCRIBEIMMEDIATE(EXPR);
      break;
    case XCTOSTRING:
      RESULT = DESCRIBEEXPR(EXPR->TOSTRPARENT, LEVELS);
      break;
    case XCTOREAL:
      RESULT = DESCRIBEEXPR(EXPR->TOREALPARENT, LEVELS);
      break;
    case XCWITHTMPVAR:
      RESULT = _DESCRIBEWITHTMPVAR(EXPR, LEVELS);
      break;
    case XCSUBRANGE:
      RESULT = DESCRIBEEXPR(EXPR->TOSTRPARENT, LEVELS);
      break;
    case XCSET:
      RESULT = _DESCRIBESET(EXPR, LEVELS);
      break;
    case XCVARIABLE:
      RESULT = EXPR->VARPTR->NAME;
      break;
    case XCFIELD:
      RESULT = cat_ss(cat_sc(DESCRIBEEXPR(EXPR->RECEXPR, LEVELS), '.'), EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[subrange(EXPR->RECFIELDNUM, 1, 32) - 1].NAME);
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
          PInteger first = 1;
          PInteger last = EXPR->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              {
                if (POS != 1) RESULT = cat_ss(RESULT, str_make(2, ", "));
                RESULT = cat_ss(RESULT, DESCRIBEEXPR(EXPR->CALLARGS.VALUES[subrange(POS, 1, 8) - 1], LEVELS - 1));
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
      RESULT = PF_DESCRIBENAME(EXPR);
      break;
    case XCPSEUDOFNCALL:
      RESULT = PF_DESCRIBECALL(EXPR, LEVELS);
      break;
    case XCUNARYOP:
      RESULT = _DESCRIBEUNARYOPEXPR(EXPR, LEVELS);
      break;
    case XCBINARYOP:
      RESULT = _DESCRIBEBINARYOPEXPR(EXPR, LEVELS);
      break;
    default:
      INTERNALERROR(str_make(26, "Cannot describe expression"));
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

TEXPRESSIONOBJ *EXBOOLEANCONSTANT(PBoolean VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICBOOLEAN);
  RESULT->IMMEDIATE.BOOLEANVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  return RESULT;
}

TEXPRESSIONOBJ *EXINTEGERCONSTANT(PInteger VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICINTEGER);
  RESULT->IMMEDIATE.INTEGERVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  return RESULT;
}

TEXPRESSIONOBJ *EXREALCONSTANT(PReal VALUE) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _EXIMMEDIATE(XICREAL);
  RESULT->IMMEDIATE.REALVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  return RESULT;
}

TEXPRESSIONOBJ *EXCHARCONSTANT(PChar VALUE) {
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

TEXPRESSIONOBJ *EXENUMCONSTANT(PInteger ORDINAL, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISENUMTYPE(TYPEPTR)) COMPILEERROR(cat_ss(str_make(25, "Not an enumeration type: "), TYPENAME(TYPEPTR)));
  if (ORDINAL < 0 || ORDINAL > TYPEPTR->ENUMPTR->SIZE - 1) COMPILEERROR(cat_ss(str_make(18, "Invalid value for "), TYPENAME(TYPEPTR)));
  RESULT = _EXIMMEDIATE(XICENUM);
  RESULT->IMMEDIATE.ENUMORDINAL = ORDINAL;
  RESULT->IMMEDIATE.ENUMPTR = TYPEPTR->ENUMPTR;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TEXPRESSIONOBJ *EXSETCONSTANT(TEXSETIMMBOUNDSOBJ *BOUNDS, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  TPSTYPE *ELEMENTTYPE;
  ELEMENTTYPE = TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMENTTYPE != (void*)0 && !ISORDINALTYPE(ELEMENTTYPE)) COMPILEERROR(cat_ss(str_make(21, "Not an ordinal type: "), TYPENAME(ELEMENTTYPE)));
  RESULT = _EXIMMEDIATE(XICSET);
  RESULT->IMMEDIATE.SETBOUNDS = BOUNDS;
  RESULT->IMMEDIATE.SETOFTYPEPTR = ELEMENTTYPE;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TEXSETIMMBOUNDSOBJ *EXSETADDBOUNDS(TEXSETIMMBOUNDSOBJ *BOUNDS, PInteger FIRST, PInteger LAST) {
  TEXSETIMMBOUNDSOBJ *RESULT;
  TEXSETIMMBOUNDSOBJ *PREV;
  TEXSETIMMBOUNDSOBJ *THIS;
  TEXSETIMMBOUNDSOBJ *NEWBOUNDS;
  PBoolean DONE;
  if (FIRST > LAST) COMPILEERROR(str_make(41, "Set bounds must appear in ascending order"));
  RESULT = BOUNDS;
  DONE = 0;
  PREV = (void*)0;
  THIS = BOUNDS;
  do {
    if (THIS == (void*)0 || LAST + 1 < THIS->FIRST) {
      NEWBOUNDS = malloc(sizeof(TEXSETIMMBOUNDSOBJ));
      NEWBOUNDS->FIRST = FIRST;
      NEWBOUNDS->LAST = LAST;
      NEWBOUNDS->NEXT = THIS;
      if (PREV == (void*)0) RESULT = NEWBOUNDS;
      else PREV->NEXT = NEWBOUNDS;
      DONE = 1;
    }
    else if (FIRST < THIS->FIRST && LAST <= THIS->LAST) {
      THIS->FIRST = FIRST;
      DONE = 1;
    }
    else if (FIRST >= THIS->FIRST && LAST <= THIS->LAST) {
      DONE = 1;
    }
    else if (FIRST <= THIS->FIRST && LAST > THIS->LAST) {
      NEWBOUNDS = THIS->NEXT;
      free(THIS);
      THIS = NEWBOUNDS;
      if (PREV == (void*)0) RESULT = NEWBOUNDS;
      else PREV->NEXT = NEWBOUNDS;
      DONE = 0;
    }
    else if (FIRST > THIS->FIRST && FIRST <= THIS->LAST + 1 && LAST > THIS->LAST) {
      FIRST = THIS->FIRST;
      NEWBOUNDS = THIS->NEXT;
      free(THIS);
      THIS = NEWBOUNDS;
      if (PREV == (void*)0) RESULT = NEWBOUNDS;
      else PREV->NEXT = NEWBOUNDS;
      DONE = 0;
    }
    else {
      PREV = THIS;
      THIS = THIS->NEXT;
      DONE = 0;
    }
  } while (!DONE);
  return RESULT;
}

PBoolean EXISIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  PBoolean RESULT;
  RESULT = EXPR->CLS == XCIMMEDIATE;
  return RESULT;
}

PBoolean EXISIMMEDIATEOFCLASS(TEXPRESSIONOBJ *EXPR, TEXIMMEDIATECLASS CLS) {
  PBoolean RESULT;
  RESULT = EXPR->CLS == XCIMMEDIATE && EXPR->IMMEDIATE.CLS == CLS;
  return RESULT;
}

TEXPRESSIONOBJ *EXSET() {
  TEXPRESSIONOBJ *RESULT;
  TPSTYPE SETTYPE;
  SETTYPE = EMPTYTYPE();
  SETTYPE.CLS = TTCSET;
  SETTYPE.ELEMENTTYPEPTR = (void*)0;
  RESULT = _EXIMMEDIATE(XICSET);
  RESULT->IMMEDIATE.SETBOUNDS = (void*)0;
  RESULT->IMMEDIATE.SETOFTYPEPTR = (void*)0;
  RESULT->TYPEPTR = ADDTYPE(SETTYPE);
  return RESULT;
}

TEXPRESSIONOBJ *EXSETADDRANGE(TEXPRESSIONOBJ *SETEXPR, TEXPRESSIONOBJ *FIRST, TEXPRESSIONOBJ *LAST) {
  TEXPRESSIONOBJ *RESULT;
  TPSTYPE *ELEMENTTYPEPTR;
  TEXPRESSIONOBJ *IMMSET;
  TEXPRESSIONOBJ *EXPRSET;
  TEXSETEXPRBOUNDSOBJ *NEWBOUNDS;
  ELEMENTTYPEPTR = SETEXPR->TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMENTTYPEPTR == (void*)0) {
    ELEMENTTYPEPTR = GETFUNDAMENTALTYPE(FIRST->TYPEPTR);
    SETEXPR->IMMEDIATE.SETOFTYPEPTR = ELEMENTTYPEPTR;
    SETEXPR->TYPEPTR->ELEMENTTYPEPTR = ELEMENTTYPEPTR;
  }
  if (!ISSAMETYPE(GETFUNDAMENTALTYPE(FIRST->TYPEPTR), ELEMENTTYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(30, "Cannot add an element of type "), TYPENAME(FIRST->TYPEPTR)), str_make(4, " to ")), TYPENAME(SETEXPR->TYPEPTR)));
  if (LAST != (void*)0 && !ISSAMETYPE(GETFUNDAMENTALTYPE(LAST->TYPEPTR), ELEMENTTYPEPTR) && !ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(30, "Cannot add an element of type "), TYPENAME(LAST->TYPEPTR)), str_make(4, " to ")), TYPENAME(SETEXPR->TYPEPTR)));
  if (EXISIMMEDIATE(SETEXPR)) {
    IMMSET = SETEXPR;
    EXPRSET = (void*)0;
  }
  else {
    IMMSET = SETEXPR->SETBASE;
    EXPRSET = SETEXPR;
  }
  if (EXISIMMEDIATE(FIRST) && LAST == (void*)0) LAST = COPYEXPR(FIRST);
  if (EXISIMMEDIATE(FIRST) && EXISIMMEDIATE(LAST)) {
    {
      TEXIMMEDIATE *with1 = &IMMSET->IMMEDIATE;
      with1->SETBOUNDS = EXSETADDBOUNDS(with1->SETBOUNDS, EXGETORDINAL(FIRST), EXGETORDINAL(LAST));
    }
    DISPOSEEXPR(&FIRST);
    DISPOSEEXPR(&LAST);
  }
  else {
    if (EXPRSET == (void*)0) {
      EXPRSET = _NEWEXPR(XCSET);
      EXPRSET->SETBASE = IMMSET;
      EXPRSET->SETBOUNDS = (void*)0;
      EXPRSET->TYPEPTR = IMMSET->TYPEPTR;
    }
    NEWBOUNDS = malloc(sizeof(TEXSETEXPRBOUNDSOBJ));
    NEWBOUNDS->FIRST = FIRST;
    NEWBOUNDS->LAST = LAST;
    NEWBOUNDS->NEXT = EXPRSET->SETBOUNDS;
    EXPRSET->SETBOUNDS = NEWBOUNDS;
  }
  if (EXPRSET != (void*)0) RESULT = EXPRSET;
  else RESULT = IMMSET;
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
  PReal VALUE;
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

TEXPRESSIONOBJ *EXWITHTMPVAR(TEXPRESSIONOBJ *TMPVAR, TEXPRESSIONOBJ *VALUE, TEXPRESSIONOBJ *CHILD) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCWITHTMPVAR);
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
  EXPR->TYPEPTR = GETFUNDAMENTALTYPE(EXPR->TYPEPTR);
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

TEXPRESSIONOBJ *EXFIELDACCESS(TEXPRESSIONOBJ *PARENT, PInteger FIELDNUM) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISRECORDTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(str_make(48, "Cannot access field of non-record value of type "), TYPENAME(PARENT->TYPEPTR)));
  if (FIELDNUM < 1 || FIELDNUM > PARENT->TYPEPTR->RECPTR->SIZE) COMPILEERROR(cat_ss(str_make(23, "Invalid field for type "), TYPENAME(PARENT->TYPEPTR)));
  RESULT = _NEWEXPR(XCFIELD);
  RESULT->RECEXPR = PARENT;
  RESULT->RECFIELDNUM = FIELDNUM;
  RESULT->TYPEPTR = PARENT->TYPEPTR->RECPTR->FIELDS[subrange(FIELDNUM, 1, 32) - 1].TYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ *EXARRAYACCESS(TEXPRESSIONOBJ *PARENT, TEXPRESSIONOBJ *SUBSCRIPT) {
  TEXPRESSIONOBJ *RESULT;
  if (!ISARRAYTYPE(PARENT->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(51, "Cannot access subscript of non-array value of type "), TYPENAME(PARENT->TYPEPTR)), str_make(2, ": ")), DESCRIBEEXPR(PARENT, 10)));
  RESULT = _NEWEXPR(XCARRAY);
  RESULT->ARRAYEXPR = PARENT;
  RESULT->ARRAYINDEX = EXCOERCE(SUBSCRIPT, PARENT->TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  RESULT->TYPEPTR = PARENT->TYPEPTR->ARRAYDEF.VALUETYPEPTR;
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
  PInteger POS;
  TEXPRESSIONOBJ *FNCALL;
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
    PInteger first = 1;
    PInteger last = ARGS->SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          FNCALL->CALLARGS.VALUES[subrange(POS, 1, 8) - 1] = EXCOERCE(ARGS->VALUES[subrange(POS, 1, 8) - 1], FNEXPR->FNPTR->ARGS[subrange(POS, 1, 8) - 1].TYPEPTR);
          if (FNEXPR->FNPTR->ARGS[subrange(POS, 1, 8) - 1].ISREFERENCE) {
            if (!FNCALL->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]->ISASSIGNABLE) {
              if (FNEXPR->FNPTR->ARGS[subrange(POS, 1, 8) - 1].ISCONSTANT) {
                RESULT = EXWITHTMPVAR(EXVARIABLE(ADDTMPVARIABLE(str_make(3, "tmp"), FNEXPR->FNPTR->ARGS[subrange(POS, 1, 8) - 1].TYPEPTR)), FNCALL->CALLARGS.VALUES[subrange(POS, 1, 8) - 1], RESULT);
                FNCALL->CALLARGS.VALUES[subrange(POS, 1, 8) - 1] = COPYEXPR(RESULT->TMPVAR);
              }
              else COMPILEERROR(cat_ss(str_make(47, "Pass-by-reference argument must be assignable: "), DESCRIBEEXPR(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 8) - 1], 10)));
            }
            else if (!FNEXPR->FNPTR->ARGS[subrange(POS, 1, 8) - 1].ISCONSTANT) EXMARKINITIALIZED(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]);
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
  if (EXPR->CLS != XCPSEUDOFNREF) INTERNALERROR(cat_ss(str_make(31, "Expected a pseudofunction, got "), DESCRIBEEXPR(EXPR, 10)));
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

void _EXSETCOERCETOCOMMON(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT) {
  TPSTYPE *LEFTTYPE;
  TPSTYPE *RIGHTTYPE;
  LEFTTYPE = LEFT->TYPEPTR;
  RIGHTTYPE = RIGHT->TYPEPTR;
  if (LEFTTYPE->ELEMENTTYPEPTR == (void*)0 || EXISIMMEDIATE(LEFT)) LEFTTYPE->ELEMENTTYPEPTR = RIGHTTYPE->ELEMENTTYPEPTR;
  else if (RIGHTTYPE->ELEMENTTYPEPTR == (void*)0 || EXISIMMEDIATE(RIGHT)) RIGHTTYPE->ELEMENTTYPEPTR = LEFTTYPE->ELEMENTTYPEPTR;
  else if (!ISSAMETYPE(LEFTTYPE, RIGHTTYPE)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(30, "Type mismatch: cannot combine "), TYPENAME(LEFT->TYPEPTR)), str_make(6, " with ")), TYPENAME(RIGHT->TYPEPTR)));
}

TEXPRESSIONOBJ *_EXSETUNION(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT) {
  TEXPRESSIONOBJ *RESULT;
  TEXSETIMMBOUNDSOBJ *NEWBDS;
  TEXSETIMMBOUNDSOBJ *OLDBDS;
  _EXSETCOERCETOCOMMON(LEFT, RIGHT);
  NEWBDS = (void*)0;
  OLDBDS = LEFT->IMMEDIATE.SETBOUNDS;
  while (OLDBDS != (void*)0) {
    NEWBDS = EXSETADDBOUNDS(NEWBDS, OLDBDS->FIRST, OLDBDS->LAST);
    OLDBDS = OLDBDS->NEXT;
  }
  OLDBDS = RIGHT->IMMEDIATE.SETBOUNDS;
  while (OLDBDS != (void*)0) {
    NEWBDS = EXSETADDBOUNDS(NEWBDS, OLDBDS->FIRST, OLDBDS->LAST);
    OLDBDS = OLDBDS->NEXT;
  }
  RESULT = EXSETCONSTANT(NEWBDS, LEFT->TYPEPTR);
  DISPOSEEXPR(&LEFT);
  DISPOSEEXPR(&RIGHT);
  return RESULT;
}

TEXPRESSIONOBJ *_EXSETDIFFERENCE(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT) {
  TEXPRESSIONOBJ *RESULT;
  TEXSETIMMBOUNDSOBJ *LTBDS;
  TEXSETIMMBOUNDSOBJ *RTBDS;
  TEXSETIMMBOUNDSOBJ *NEWBDS;
  _EXSETCOERCETOCOMMON(LEFT, RIGHT);
  LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
  RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
  NEWBDS = (void*)0;
  while (LTBDS != (void*)0 && RTBDS != (void*)0) {
    if (RTBDS->LAST < LTBDS->FIRST) RTBDS = RTBDS->NEXT;
    else if (RTBDS->FIRST <= LTBDS->FIRST && RTBDS->LAST < LTBDS->LAST) LTBDS->FIRST = RTBDS->LAST + 1;
    else if (RTBDS->FIRST <= LTBDS->FIRST && RTBDS->LAST >= LTBDS->LAST) LTBDS = LTBDS->NEXT;
    else if (RTBDS->FIRST > LTBDS->FIRST && RTBDS->LAST < LTBDS->LAST) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, RTBDS->FIRST - 1);
      LTBDS->FIRST = RTBDS->LAST + 1;
    }
    else if (RTBDS->FIRST < LTBDS->LAST && RTBDS->LAST >= LTBDS->LAST) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, RTBDS->FIRST - 1);
      LTBDS = LTBDS->NEXT;
    }
    else {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, LTBDS->LAST);
      LTBDS = LTBDS->NEXT;
    }
  }
  while (LTBDS != (void*)0) {
    NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, LTBDS->LAST);
    LTBDS = LTBDS->NEXT;
  }
  RESULT = EXSETCONSTANT(NEWBDS, LEFT->TYPEPTR);
  DISPOSEEXPR(&LEFT);
  DISPOSEEXPR(&RIGHT);
  return RESULT;
}

TEXPRESSIONOBJ *_EXSETINTERSECTION(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT) {
  TEXPRESSIONOBJ *RESULT;
  TEXSETIMMBOUNDSOBJ *LTBDS;
  TEXSETIMMBOUNDSOBJ *RTBDS;
  TEXSETIMMBOUNDSOBJ *NEWBDS;
  _EXSETCOERCETOCOMMON(LEFT, RIGHT);
  LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
  RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
  NEWBDS = (void*)0;
  while (LTBDS != (void*)0 && RTBDS != (void*)0) {
    if (RTBDS->LAST < LTBDS->FIRST) RTBDS = RTBDS->NEXT;
    else if (RTBDS->FIRST <= LTBDS->FIRST && RTBDS->LAST < LTBDS->LAST) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, RTBDS->LAST);
      RTBDS = RTBDS->NEXT;
    }
    else if (RTBDS->FIRST <= LTBDS->FIRST && RTBDS->LAST >= LTBDS->LAST) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, LTBDS->LAST);
      LTBDS = LTBDS->NEXT;
    }
    else if (RTBDS->FIRST > LTBDS->FIRST && RTBDS->LAST < LTBDS->LAST) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, RTBDS->FIRST, RTBDS->LAST);
      RTBDS = RTBDS->NEXT;
    }
    else if (RTBDS->FIRST < LTBDS->LAST && RTBDS->LAST >= LTBDS->LAST) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, RTBDS->FIRST, LTBDS->LAST);
      LTBDS = LTBDS->NEXT;
    }
    else LTBDS = LTBDS->NEXT;
  }
  RESULT = EXSETCONSTANT(NEWBDS, LEFT->TYPEPTR);
  DISPOSEEXPR(&LEFT);
  DISPOSEEXPR(&RIGHT);
  return RESULT;
}

TEXPRESSIONOBJ *_EXSETEQUALS(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, PBoolean NEGATE) {
  TEXPRESSIONOBJ *RESULT;
  TEXSETIMMBOUNDSOBJ *LTBDS;
  TEXSETIMMBOUNDSOBJ *RTBDS;
  PBoolean EQUALS;
  _EXSETCOERCETOCOMMON(LEFT, RIGHT);
  LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
  RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
  EQUALS = 1;
  while (LTBDS != (void*)0 && RTBDS != (void*)0 && EQUALS) {
    EQUALS = RTBDS->FIRST == LTBDS->FIRST && RTBDS->LAST == LTBDS->LAST;
    LTBDS = LTBDS->NEXT;
    RTBDS = RTBDS->NEXT;
  }
  EQUALS = EQUALS && LTBDS == (void*)0 && RTBDS == (void*)0;
  if (NEGATE) EQUALS = !EQUALS;
  RESULT = EXBOOLEANCONSTANT(EQUALS);
  DISPOSEEXPR(&LEFT);
  DISPOSEEXPR(&RIGHT);
  return RESULT;
}

TEXPRESSIONOBJ *_EXSETSUBSET(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT) {
  TEXPRESSIONOBJ *RESULT;
  TEXSETIMMBOUNDSOBJ *LTBDS;
  TEXSETIMMBOUNDSOBJ *RTBDS;
  PBoolean SUBSET;
  _EXSETCOERCETOCOMMON(LEFT, RIGHT);
  LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
  RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
  SUBSET = 1;
  while (LTBDS != (void*)0 && RTBDS != (void*)0 && SUBSET) {
    if (LTBDS->LAST < RTBDS->FIRST) LTBDS = LTBDS->NEXT;
    else if (LTBDS->FIRST <= RTBDS->FIRST && LTBDS->LAST >= RTBDS->LAST) RTBDS = RTBDS->NEXT;
    else SUBSET = 0;
  }
  SUBSET = SUBSET && RTBDS == (void*)0;
  RESULT = EXBOOLEANCONSTANT(SUBSET);
  DISPOSEEXPR(&LEFT);
  DISPOSEEXPR(&RIGHT);
  return RESULT;
}

TEXPRESSIONOBJ *_EXSETIN(TEXPRESSIONOBJ *NEEDLE, TEXPRESSIONOBJ *HAYSTACK) {
  TEXPRESSIONOBJ *RESULT;
  TPSTYPE *ELEMTYPE;
  TEXPRESSIONOBJ *IMMSET;
  TEXPRESSIONOBJ *EXPRSET;
  TEXSETIMMBOUNDSOBJ *IMMBOUNDS;
  TEXSETEXPRBOUNDSOBJ *EXPRBOUNDS;
  TEXPRESSIONOBJ *COND;
  TPSVARIABLE *TMPVAR;
  TEXPRESSIONOBJ *WANTED;
  ELEMTYPE = HAYSTACK->TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMTYPE == (void*)0) ELEMTYPE = NEEDLE->TYPEPTR;
  else NEEDLE = EXCOERCE(NEEDLE, ELEMTYPE);
  if (NEEDLE->ISFUNCTIONRESULT) {
    TMPVAR = ADDTMPVARIABLE(str_make(4, "elem"), ELEMTYPE);
    WANTED = EXVARIABLE(TMPVAR);
  }
  else {
    TMPVAR = (void*)0;
    WANTED = NEEDLE;
  }
  RESULT = EXBOOLEANCONSTANT(0);
  if (EXISIMMEDIATE(HAYSTACK)) {
    IMMSET = HAYSTACK;
    EXPRSET = (void*)0;
  }
  else {
    IMMSET = HAYSTACK->SETBASE;
    EXPRSET = HAYSTACK;
  }
  if (IMMSET != (void*)0) {
    IMMBOUNDS = IMMSET->IMMEDIATE.SETBOUNDS;
    while (IMMBOUNDS != (void*)0) {
      if (IMMBOUNDS->FIRST == IMMBOUNDS->LAST) COND = EXBINARYOP(COPYEXPR(WANTED), EXGETANTIORDINAL(IMMBOUNDS->FIRST, ELEMTYPE), TKEQUALS);
      else COND = EXBINARYOP(EXBINARYOP(EXGETANTIORDINAL(IMMBOUNDS->FIRST, ELEMTYPE), COPYEXPR(WANTED), TKLESSOREQUALS), EXBINARYOP(COPYEXPR(WANTED), EXGETANTIORDINAL(IMMBOUNDS->LAST, ELEMTYPE), TKLESSOREQUALS), TKAND);
      RESULT = EXBINARYOP(RESULT, COND, TKOR);
      IMMBOUNDS = IMMBOUNDS->NEXT;
    }
  }
  if (EXPRSET != (void*)0) {
    EXPRBOUNDS = EXPRSET->SETBOUNDS;
    while (EXPRBOUNDS != (void*)0) {
      if (EXPRBOUNDS->LAST == (void*)0) COND = EXBINARYOP(COPYEXPR(WANTED), COPYEXPR(EXPRBOUNDS->FIRST), TKEQUALS);
      else COND = EXBINARYOP(EXBINARYOP(COPYEXPR(EXPRBOUNDS->FIRST), COPYEXPR(WANTED), TKLESSOREQUALS), EXBINARYOP(COPYEXPR(WANTED), COPYEXPR(EXPRBOUNDS->LAST), TKLESSOREQUALS), TKAND);
      RESULT = EXBINARYOP(RESULT, COND, TKOR);
      EXPRBOUNDS = EXPRBOUNDS->NEXT;
    }
  }
  if (TMPVAR != (void*)0) {
    RESULT = EXWITHTMPVAR(WANTED, NEEDLE, RESULT);
  }
  else DISPOSEEXPR(&NEEDLE);
  DISPOSEEXPR(&HAYSTACK);
  return RESULT;
}

TEXPRESSIONOBJ *_EXUNOPIMM(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXUNOPCMP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP);

TEXPRESSIONOBJ *EXUNARYOP(TEXPRESSIONOBJ *PARENT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (TKPLUS <= OP && OP <= TKMINUS) {
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
  else INTERNALERROR(str_make(33, "Invalid immediate unary operation"));
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
TEXPRESSIONOBJ *_EXBINOPSETIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPBOOLCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPINTCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPNUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPSTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPENUMCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPPTRCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
TEXPRESSIONOBJ *_EXBINOPSETCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP);
PBoolean _EXBINOPSHORTCUT(TEXPRESSIONOBJ **LEFT, TEXPRESSIONOBJ **RIGHT, TLXTOKENID OP);

TEXPRESSIONOBJ *EXBINARYOP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  PBoolean IMMEDIATE;
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
  else if (ISSETTYPE(RIGHT->TYPEPTR)) {
    if (EXISIMMEDIATE(RIGHT) && (EXISIMMEDIATE(LEFT) || !ISSETTYPE(LEFT->TYPEPTR))) RESULT = _EXBINOPSETIMM(LEFT, RIGHT, OP);
    else if (OP == TKIN && RIGHT->CLS == XCSET) RESULT = _EXBINOPSETIMM(LEFT, RIGHT, OP);
    else RESULT = _EXBINOPSETCMP(LEFT, RIGHT, OP);
  }
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(27, "Type mismatch for operator "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(LEFT->TYPEPTR)), str_make(5, " and ")), TYPENAME(RIGHT->TYPEPTR)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  PBoolean LT;
  PBoolean RT;
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
    case TKXOR:
      LT = LT != RT;
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
  PInteger LT;
  PInteger RT;
  PBoolean BO;
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
    case TKXOR:
      LT = LT ^ RT;
      break;
    case TKSHL:
      LT = LT << RT;
      break;
    case TKSHR:
      LT = LT >> RT;
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
  PReal LT;
  PReal RT;
  PBoolean BO;
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
  PBoolean BO;
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
  PInteger LT;
  PInteger RT;
  PBoolean BO;
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

TEXPRESSIONOBJ *_EXBINOPSETIMM(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) switch (OP) {
    case TKPLUS:
      RESULT = _EXSETUNION(LEFT, RIGHT);
      break;
    case TKMINUS:
      RESULT = _EXSETDIFFERENCE(LEFT, RIGHT);
      break;
    case TKASTERISK:
      RESULT = _EXSETINTERSECTION(LEFT, RIGHT);
      break;
    case TKEQUALS:
      RESULT = _EXSETEQUALS(LEFT, RIGHT, 0);
      break;
    case TKNOTEQUALS:
      RESULT = _EXSETEQUALS(LEFT, RIGHT, 1);
      break;
    case TKMOREOREQUALS:
      RESULT = _EXSETSUBSET(LEFT, RIGHT);
      break;
    case TKLESSOREQUALS:
      RESULT = _EXSETSUBSET(RIGHT, LEFT);
      break;
    default:
      COMPILEERROR(cat_ss(str_make(22, "Invalid set operator: "), LXTOKENNAME(OP)));
      break;
  }
  else if (OP == TKIN) RESULT = _EXSETIN(LEFT, RIGHT);
  else COMPILEERROR(cat_ss(str_make(22, "Invalid set operator: "), LXTOKENNAME(OP)));
  return RESULT;
}

TEXPRESSIONOBJ *_EXBINOPBOOLCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  if (TKEQUALS <= OP && OP <= TKMORETHAN || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS || OP == TKAND || OP == TKOR || OP == TKXOR) {
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

TEXPRESSIONOBJ *_EXBINOPINTCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  if (TKPLUS <= OP && OP <= TKASTERISK || OP == TKAND || OP == TKDIV || OP == TKMOD || OP == TKOR || TKSHL <= OP && OP <= TKSHR || OP == TKXOR) RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  else if (TKEQUALS <= OP && OP <= TKMORETHAN || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
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
  if (TKPLUS <= OP && OP <= TKSLASH) RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  else if (TKEQUALS <= OP && OP <= TKMORETHAN || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
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
  else if (TKEQUALS <= OP && OP <= TKMORETHAN || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
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
  if (TKEQUALS <= OP && OP <= TKMORETHAN || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS) RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
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

TEXPRESSIONOBJ *_EXBINOPSETCMP(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  if (OP == TKIN) {
    LEFT = EXCOERCE(LEFT, RIGHT->TYPEPTR->ELEMENTTYPEPTR);
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  }
  else if (OP == TKEQUALS || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS) {
    _EXSETCOERCETOCOMMON(LEFT, RIGHT);
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  }
  else {
    _EXSETCOERCETOCOMMON(LEFT, RIGHT);
    RESULT->TYPEPTR = LEFT->TYPEPTR;
  }
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  return RESULT;
}

PBoolean _EXISZERO(TEXPRESSIONOBJ *EXPR) {
  PBoolean RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICINTEGER) && EXPR->IMMEDIATE.INTEGERVAL == 0 || EXISIMMEDIATEOFCLASS(EXPR, XICREAL) && (double)EXPR->IMMEDIATE.INTEGERVAL ==  0.00000000000000E+000;
  return RESULT;
}

PBoolean _EXISONE(TEXPRESSIONOBJ *EXPR) {
  PBoolean RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICINTEGER) && EXPR->IMMEDIATE.INTEGERVAL == 1 || EXISIMMEDIATEOFCLASS(EXPR, XICREAL) && (double)EXPR->IMMEDIATE.INTEGERVAL ==  1.00000000000000E+000;
  return RESULT;
}

PBoolean _EXISTRUE(TEXPRESSIONOBJ *EXPR) {
  PBoolean RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICBOOLEAN) && EXPR->IMMEDIATE.BOOLEANVAL;
  return RESULT;
}

PBoolean _EXISFALSE(TEXPRESSIONOBJ *EXPR) {
  PBoolean RESULT;
  RESULT = EXISIMMEDIATEOFCLASS(EXPR, XICBOOLEAN) && !EXPR->IMMEDIATE.BOOLEANVAL;
  return RESULT;
}

PBoolean _EXBINOPSHORTCUT(TEXPRESSIONOBJ **LEFT, TEXPRESSIONOBJ **RIGHT, TLXTOKENID OP) {
  PBoolean RESULT;
  enum enum9 { USELEFT, USERIGHT, KEEP } USE;
  const char* enumvalues9[] = { "USELEFT", "USERIGHT", "KEEP" };
  USE = KEEP;
  switch (OP) {
    case TKPLUS:
      if (_EXISZERO(*LEFT)) USE = USERIGHT;
      else if (_EXISZERO(*RIGHT)) USE = USELEFT;
      break;
    case TKMINUS:
      if (_EXISZERO(*RIGHT)) USE = USELEFT;
      break;
    case TKASTERISK:
      if (_EXISONE(*LEFT)) USE = USERIGHT;
      else if (_EXISONE(*RIGHT)) USE = USELEFT;
      break;
    case TKSLASH:
      if (_EXISONE(*RIGHT)) USE = USELEFT;
      break;
    case TKDIV:
      if (_EXISONE(*RIGHT)) USE = USELEFT;
      break;
    case TKAND:
      if (_EXISFALSE(*LEFT) || _EXISTRUE(*RIGHT)) USE = USELEFT;
      else if (_EXISTRUE(*LEFT)) USE = USERIGHT;
      break;
    case TKOR:
      if (_EXISTRUE(*LEFT) || _EXISFALSE(*RIGHT)) USE = USELEFT;
      else if (_EXISFALSE(*LEFT)) USE = USERIGHT;
      break;
    case TKSHL:
      if (_EXISZERO(*RIGHT)) USE = USELEFT;
      break;
    case TKSHR:
      if (_EXISZERO(*RIGHT)) USE = USELEFT;
      break;
    default:
      break;
  }
  switch (USE) {
    case USELEFT:
      {
        DISPOSEEXPR(RIGHT);
        RESULT = 1;
      }
      break;
    case USERIGHT:
      {
        DISPOSEEXPR(LEFT);
        *LEFT = *RIGHT;
        RESULT = 1;
      }
      break;
    case KEEP:
      RESULT = 0;
      break;
    default:
      break;
  }
  return RESULT;
}

PInteger EXGETORDINAL(TEXPRESSIONOBJ *EXPR) {
  PInteger RESULT;
  if (!EXISIMMEDIATE(EXPR)) COMPILEERROR(cat_ss(str_make(29, "Expression is not immediate: "), DESCRIBEEXPR(EXPR, 5)));
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICBOOLEAN:
        RESULT = with1->BOOLEANVAL;
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

TEXPRESSIONOBJ *EXGETANTIORDINAL(PInteger ORDINAL, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  TYPEPTR = GETFUNDAMENTALTYPE(TYPEPTR);
  switch (TYPEPTR->CLS) {
    case TTCBOOLEAN:
      if (ORDINAL == 0) RESULT = EXBOOLEANCONSTANT(0);
      else RESULT = EXBOOLEANCONSTANT(1);
      break;
    case TTCINTEGER:
      RESULT = EXINTEGERCONSTANT(ORDINAL);
      break;
    case TTCCHAR:
      RESULT = EXCHARCONSTANT(CHR(ORDINAL));
      break;
    case TTCENUM:
      RESULT = EXENUMCONSTANT(ORDINAL, TYPEPTR);
      break;
    default:
      INTERNALERROR(cat_ss(str_make(32, "Cannot compute anti-ordinal for "), TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

TEXPRESSIONOBJ *EXSUBRANGE(TEXPRESSIONOBJ *PARENT, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  PInteger ORDINAL;
  if (EXISIMMEDIATE(PARENT)) {
    ORDINAL = EXGETORDINAL(PARENT);
    if (ORDINAL < TYPEPTR->RANGEDEF.FIRST || ORDINAL > TYPEPTR->RANGEDEF.LAST) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(6, "Value "), DESCRIBEEXPR(PARENT, 2)), str_make(19, " out of bounds for ")), TYPENAME(TYPEPTR)));
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
  if (TYPEPTR->RANGEDEF.FIRST <= EXPR->TYPEPTR->RANGEDEF.FIRST && EXPR->TYPEPTR->RANGEDEF.LAST <= TYPEPTR->RANGEDEF.LAST) {
    EXPR->TYPEPTR = TYPEPTR;
    RESULT = EXPR;
  }
  else RESULT = EXSUBRANGE(EXOUTRANGE(EXPR), TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ *_EXCOERCESET(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  enum enum9 { PASS, REJECT, REPLACE } OUTCOME;
  TPSTYPE *EXPRELEMTYPE;
  TPSTYPE *DESTELEMTYPE;
  const char* enumvalues9[] = { "PASS", "REJECT", "REPLACE" };
  EXPRELEMTYPE = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  DESTELEMTYPE = TYPEPTR->ELEMENTTYPEPTR;
  if (EXPRELEMTYPE == (void*)0) OUTCOME = REPLACE;
  else if (!ISSAMETYPE(GETFUNDAMENTALTYPE(EXPRELEMTYPE), GETFUNDAMENTALTYPE(DESTELEMTYPE))) OUTCOME = REJECT;
  else if (EXISIMMEDIATE(EXPR)) OUTCOME = REPLACE;
  else if (EXPR->CLS == XCSET) OUTCOME = REPLACE;
  else if (GETTYPELOWBOUND(EXPRELEMTYPE) == GETTYPELOWBOUND(DESTELEMTYPE) && GETTYPEHIGHBOUND(EXPRELEMTYPE) == GETTYPEHIGHBOUND(DESTELEMTYPE)) OUTCOME = PASS;
  else OUTCOME = REJECT;
  switch (OUTCOME) {
    case REJECT:
      COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(15, "Type mismatch: "), TYPENAME(EXPR->TYPEPTR)), str_make(23, " cannot be assigned to ")), TYPENAME(TYPEPTR)));
      break;
    case REPLACE:
      {
        EXPR->TYPEPTR = TYPEPTR;
        if (EXPR->CLS == XCSET) EXPR->SETBASE->TYPEPTR = TYPEPTR;
      }
      break;
    case PASS:
      ;
      break;
    default:
      break;
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ *EXCOERCE(TEXPRESSIONOBJ *EXPR, TPSTYPE *TYPEPTR) {
  TEXPRESSIONOBJ *RESULT;
  if (ISRANGETYPE(EXPR->TYPEPTR) && ISSAMETYPE(TYPEPTR, GETFUNDAMENTALTYPE(EXPR->TYPEPTR))) RESULT = EXOUTRANGE(EXPR);
  else if (ISRANGETYPE(TYPEPTR) && ISSAMETYPE(GETFUNDAMENTALTYPE(TYPEPTR), EXPR->TYPEPTR)) RESULT = EXSUBRANGE(EXPR, TYPEPTR);
  else if (ISRANGETYPE(EXPR->TYPEPTR) && ISRANGETYPE(TYPEPTR) && ISSAMETYPE(GETFUNDAMENTALTYPE(EXPR->TYPEPTR), GETFUNDAMENTALTYPE(TYPEPTR))) RESULT = EXRERANGE(EXPR, TYPEPTR);
  else if (ISCHARTYPE(EXPR->TYPEPTR) && ISSTRINGTYPE(TYPEPTR)) RESULT = EXTOSTRING(EXPR);
  else if (ISINTEGERTYPE(EXPR->TYPEPTR) && ISREALTYPE(TYPEPTR)) RESULT = EXTOREAL(EXPR);
  else if (ISSAMETYPE(EXPR->TYPEPTR, TYPEPTR)) RESULT = EXPR;
  else if (ISNILTYPE(EXPR->TYPEPTR) && ISPOINTERYTYPE(TYPEPTR)) RESULT = EXPR;
  else if (ISSETTYPE(EXPR->TYPEPTR) && ISSETTYPE(TYPEPTR)) {
    RESULT = _EXCOERCESET(EXPR, TYPEPTR);
  }
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(24, "Type mismatch: expected "), TYPENAME(TYPEPTR)), str_make(6, ", got ")), TYPENAME(EXPR->TYPEPTR)), str_make(4, " in ")), DESCRIBEEXPR(EXPR, 5)));
  return RESULT;
}

void EXMARKINITIALIZED(TEXPRESSIONOBJ *LHS) {
  PBoolean ISTERMINAL;
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
  ENUM.VALUESHAVEBEENOUTPUT = 0;
  do {
    ENUM.SIZE = ENUM.SIZE + 1;
    if (ENUM.SIZE > 128) COMPILEERROR(str_make(23, "Too many values in enum"));
    ENUM.VALUES[subrange(ENUM.SIZE - 1, 0, 127)] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRPAREN));
  TYP = EMPTYTYPE();
  TYP.CLS = TTCENUM;
  TYP.ENUMPTR = NEWENUM(ENUM);
  TYP.WASUSED = 1;
  RESULT = ADDTYPE(TYP);
  SKIPTOKEN(TKRPAREN);
  return RESULT;
}

void PSRECORDFIELD(TPSRECORDDEF *REC, TLXTOKENID DELIMITER) {
  PString NAME;
  PInteger LASTFIELD;
  PInteger FIELD;
  TPSTYPE *TYPEPTR;
  LASTFIELD = REC->SIZE;
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    do {
      PInteger first = 1;
      PInteger last = REC->SIZE;
      if (first <= last) {
        FIELD = first;
        while (1) {
          {
            if (cmp_ss(REC->FIELDS[subrange(FIELD, 1, 32) - 1].NAME, NAME) == 0) COMPILEERROR(cat_ss(cat_ss(str_make(14, "A field named "), NAME), str_make(25, " has already been defined")));
          }
          if (FIELD == last) break;
          ++FIELD;
        }
      }
    } while(0);
    REC->SIZE = REC->SIZE + 1;
    if (REC->SIZE > 32) COMPILEERROR(str_make(25, "Too many fields in record"));
    REC->FIELDS[subrange(REC->SIZE, 1, 32) - 1].NAME = NAME;
    WANTTOKEN2(TKCOMMA, TKCOLON);
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKCOLON));
  WANTTOKENANDREAD(TKCOLON);
  TYPEPTR = PSTYPEDENOTER();
  do {
    PInteger first = LASTFIELD + 1;
    PInteger last = REC->SIZE;
    if (first <= last) {
      FIELD = first;
      while (1) {
        REC->FIELDS[subrange(FIELD, 1, 32) - 1].TYPEPTR = TYPEPTR;
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
    REC->FIELDS[subrange(REC->SIZE, 1, 32) - 1].NAME = TAG.NAME;
    REC->FIELDS[subrange(REC->SIZE, 1, 32) - 1].TYPEPTR = TAGTYPE;
  }
  else TAGTYPE = FINDNAMEOFCLASS(TAG.NAME, TNCTYPE, 1)->TYPEPTR;
  if (!ISORDINALTYPE(TAGTYPE)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
  WANTTOKENANDREAD(TKOF);
  do {
    REC->NUMVARIANTS = REC->NUMVARIANTS + 1;
    REC->VARIANTBOUNDS[subrange(REC->NUMVARIANTS, 1, 32) - 1] = REC->SIZE + 1;
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
  TYP.RECPTR = NEWRECORD(REC);
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
  TPSTYPE *TYPEPTR;
  TPSTYPE *NEWTYPEPTR;
  TYP = TYPEOFCLASS(TTCARRAY);
  TYP.ARRAYDEF.INDEXTYPEPTR = (void*)0;
  TYP.ARRAYDEF.VALUETYPEPTR = (void*)0;
  TYPEPTR = ADDTYPE(TYP);
  RESULT = TYPEPTR;
  WANTTOKENANDREAD(TKARRAY);
  WANTTOKENANDREAD(TKLBRACKET);
  do {
    TYPEPTR->ARRAYDEF.INDEXTYPEPTR = PSTYPEDENOTER();
    TYPEPTR->ARRAYDEF.INDEXTYPEPTR->WASUSED = 1;
    if (!ISBOUNDEDTYPE(TYPEPTR->ARRAYDEF.INDEXTYPEPTR)) COMPILEERROR(str_make(51, "Array indices must belong to a bounded ordinal type"));
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    if (LEXER.TOKEN.ID == TKCOMMA) {
      NEWTYPEPTR = ADDTYPE(TYP);
      TYPEPTR->ARRAYDEF.VALUETYPEPTR = NEWTYPEPTR;
      TYPEPTR->ARRAYDEF.VALUETYPEPTR->WASUSED = 1;
      TYPEPTR = NEWTYPEPTR;
    }
    SKIPTOKEN(TKCOMMA);
  } while (!(LEXER.TOKEN.ID == TKRBRACKET));
  WANTTOKENANDREAD(TKRBRACKET);
  WANTTOKENANDREAD(TKOF);
  TYPEPTR->ARRAYDEF.VALUETYPEPTR = PSTYPEDENOTER();
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
  FIRST = PSIMMEDIATE();
  WANTTOKENANDREAD(TKRANGE);
  LAST = PSIMMEDIATE();
  if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) COMPILEERROR(str_make(53, "The bounds of a subrange must belong to the same type"));
  if (!ISORDINALTYPE(FIRST->TYPEPTR)) COMPILEERROR(str_make(55, "The bounds of a subrange must belong to an ordinal type"));
  TYP = TYPEOFCLASS(TTCRANGE);
  TYP.RANGEDEF.FIRST = EXGETORDINAL(FIRST);
  TYP.RANGEDEF.LAST = EXGETORDINAL(LAST);
  TYP.RANGEDEF.BASETYPEPTR = FIRST->TYPEPTR;
  RESULT = ADDTYPE(TYP);
  if (TYP.RANGEDEF.FIRST > TYP.RANGEDEF.LAST) COMPILEERROR(str_make(51, "The bounds of a subrange must be in ascending order"));
  DISPOSEEXPR(&FIRST);
  DISPOSEEXPR(&LAST);
  return RESULT;
}

TPSTYPE *PSSETTYPE() {
  TPSTYPE *RESULT;
  TPSTYPE TYP;
  WANTTOKENANDREAD(TKSET);
  WANTTOKENANDREAD(TKOF);
  TYP = TYPEOFCLASS(TTCSET);
  TYP.ELEMENTTYPEPTR = PSTYPEDENOTER();
  RESULT = ADDTYPE(TYP);
  if (!ISBOUNDEDTYPE(TYP.ELEMENTTYPEPTR)) COMPILEERROR(str_make(47, "Set element types must be bounded ordinal types"));
  if (GETBOUNDEDTYPESIZE(TYP.ELEMENTTYPEPTR) > 256) COMPILEERROR(str_make(54, "Set element types may not contain more than 256 values"));
  return RESULT;
}

TPSTYPE *PSTYPEDENOTER() {
  TPSTYPE *RESULT;
  TPSNAME *IDX;
  RESULT = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) RESULT = PSENUMERATEDTYPE();
  else if (LEXER.TOKEN.ID == TKSET) RESULT = PSSETTYPE();
  else if (LEXER.TOKEN.ID == TKRECORD) RESULT = PSRECORDTYPE();
  else if (LEXER.TOKEN.ID == TKARRAY) RESULT = PSARRAYTYPE();
  else if (LEXER.TOKEN.ID == TKCARET) RESULT = PSPOINTERTYPE();
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) {
    IDX = FINDNAME(LEXER.TOKEN.VALUE, 0);
    if (IDX == (void*)0) ;
    else if (IDX->CLS == TNCTYPE) RESULT = PSTYPEIDENTIFIER();
    else if (TNCCONSTANT <= IDX->CLS && IDX->CLS <= TNCENUMVAL) RESULT = PSRANGETYPE();
  }
  else if (LEXER.TOKEN.ID == TKINTEGER || TKSTRING <= LEXER.TOKEN.ID && LEXER.TOKEN.ID <= TKMINUS) RESULT = PSRANGETYPE();
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
    TYPEPTR = TYPEPTR->ARRAYDEF.VALUETYPEPTR;
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
  PInteger NUMNAMES;
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
      NAMES[subrange(NUMNAMES, 1, 8) - 1] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      WANTTOKEN2(TKCOMMA, TKCOLON);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    WANTTOKENANDREAD(TKCOLON);
    TYPEPTR = PSTYPEDENOTER();
    WANTTOKENANDREAD(TKSEMICOLON);
    do {
      PInteger first = 1;
      PInteger last = NUMNAMES;
      if (first <= last) {
        NUMNAMES = first;
        while (1) {
          OUTVARIABLEDEFINITION(ADDVARIABLE(MAKEVARIABLE(NAMES[subrange(NUMNAMES, 1, 8) - 1], TYPEPTR)));
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
  PInteger POS;
  TPSDEFENTRY *CHECKPOINT;
  TPSVARIABLE *RESULTPTR;
  STARTLOCALSCOPE(FNPTR);
  CHECKPOINT = DEFS.LATEST;
  do {
    PInteger first = 1;
    PInteger last = FNPTR->ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        ADDVARIABLE(FNPTR->ARGS[subrange(POS, 1, 8) - 1]);
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
  PBoolean ISCONST;
  PBoolean ISVAR;
  PInteger LASTARG;
  PInteger ARG;
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
      DEF->ARGS[subrange(DEF->ARGCOUNT, 1, 8) - 1].NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      DEF->ARGS[subrange(DEF->ARGCOUNT, 1, 8) - 1].ISCONSTANT = ISCONST;
      DEF->ARGS[subrange(DEF->ARGCOUNT, 1, 8) - 1].ISREFERENCE = ISVAR || ISCONST;
      DEF->ARGS[subrange(DEF->ARGCOUNT, 1, 8) - 1].WASINITIALIZED = 1;
      WANTTOKEN2(TKCOLON, TKCOMMA);
      SKIPTOKEN(TKCOMMA);
    } while (!(LEXER.TOKEN.ID == TKCOLON));
    SKIPTOKEN(TKCOLON);
    TYPEPTR = PSTYPEIDENTIFIER();
    do {
      PInteger first = LASTARG + 1;
      PInteger last = DEF->ARGCOUNT;
      if (first <= last) {
        ARG = first;
        while (1) {
          DEF->ARGS[subrange(ARG, 1, 8) - 1].TYPEPTR = TYPEPTR;
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
        ARGS.VALUES[subrange(ARGS.SIZE, 1, 8) - 1] = PSEXPRESSION();
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
  PBoolean DONE;
  if (EXPR->CLS == XCVARIABLE && (LEXER.TOKEN.ID == TKLBRACKET || LEXER.TOKEN.ID == TKDOT || LEXER.TOKEN.ID == TKCARET)) EXPR->VARPTR->WASUSED = 1;
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

PBoolean ISOPADDING(TLXTOKEN TOK) {
  PBoolean RESULT;
  RESULT = TKPLUS <= TOK.ID && TOK.ID <= TKMINUS || TOK.ID == TKOR || TOK.ID == TKXOR;
  return RESULT;
}

PBoolean ISOPMULTIPLYING(TLXTOKEN TOK) {
  PBoolean RESULT;
  RESULT = TKASTERISK <= TOK.ID && TOK.ID <= TKSLASH || TOK.ID == TKAND || TOK.ID == TKDIV || TOK.ID == TKMOD || TKSHL <= TOK.ID && TOK.ID <= TKSHR;
  return RESULT;
}

PBoolean ISOPRELATIONAL(TLXTOKEN TOK) {
  PBoolean RESULT;
  RESULT = TKEQUALS <= TOK.ID && TOK.ID <= TKMORETHAN || TKNOTEQUALS <= TOK.ID && TOK.ID <= TKMOREOREQUALS || TOK.ID == TKIN;
  return RESULT;
}

PString PARSESTRING(PString PSTR) {
  PString RESULT;
  enum enum9 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, NUMCHARREADY, CARET, ERROR, DONE } STATE;
  PInteger POS;
  PChar CH;
  PInteger CHNUM;
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
      if ('@' <= CH && CH <= '_') RESULT = cat_sc(RESULT, CHR((int)CH - 64));
      else if ('a' <= CH && CH <= 'z') RESULT = cat_sc(RESULT, CHR((int)CH - 96));
      else STATE = ERROR;
    }
    else if (STATE == ERROR) COMPILEERROR(cat_ss(str_make(29, "Invalid character in string: "), PSTR));
  }
  if (STATE == QUOTEDSTR || STATE == CARET) COMPILEERROR(cat_ss(str_make(26, "String is not terminated: "), PSTR));
  if (NUMCHARDEC <= STATE && STATE <= NUMCHARHEX) RESULT = cat_sc(RESULT, CHR(CHNUM));
  return RESULT;
}

PInteger PARSEINT(PString PSTR) {
  PInteger RESULT;
  PInteger CODE;
  VAL_i(&PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(cat_ss(str_make(25, "Could not parse integer: "), PSTR));
  return RESULT;
}

PReal PARSEREAL(PString PSTR) {
  PReal RESULT;
  PInteger CODE;
  VAL_r(&PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(cat_ss(str_make(29, "Could not parse real number: "), PSTR));
  return RESULT;
}

TEXPRESSIONOBJ *PSSETCONSTRUCTOR() {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *FIRST;
  TEXPRESSIONOBJ *LAST;
  RESULT = EXSET();
  WANTTOKENANDREAD(TKLBRACKET);
  while (LEXER.TOKEN.ID != TKRBRACKET) {
    FIRST = PSEXPRESSION();
    if (!ISORDINALTYPE(FIRST->TYPEPTR)) COMPILEERROR(cat_ss(str_make(47, "Set elements must belong to ordinal types, got "), TYPENAME(FIRST->TYPEPTR)));
    if (LEXER.TOKEN.ID == TKRANGE) {
      WANTTOKENANDREAD(TKRANGE);
      LAST = PSEXPRESSION();
      if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(63, "Set element range boundaries must belong to the same type, got "), TYPENAME(FIRST->TYPEPTR)), str_make(5, " and ")), TYPENAME(LAST->TYPEPTR)));
    }
    else LAST = (void*)0;
    RESULT = EXSETADDRANGE(RESULT, FIRST, LAST);
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    SKIPTOKEN(TKCOMMA);
  }
  WANTTOKENANDREAD(TKRBRACKET);
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
  else if (LEXER.TOKEN.ID == TKLBRACKET) EXPR = PSSETCONSTRUCTOR();
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
  PBoolean NEGATIVE;
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
  PBoolean USESTMPVARS;
  LHS = PSVARIABLE();
  if (LHS->CLS != XCFNREF || LHS->FNPTR != DEFS.CURRENTFN || LEXER.TOKEN.ID != TKASSIGN) LHS = PSVARIABLEORFUNCTIONEXTENSION(LHS);
  if (LEXER.TOKEN.ID == TKASSIGN) {
    WANTTOKENANDREAD(TKASSIGN);
    PSASSIGN(LHS, PSEXPRESSION());
  }
  else {
    ORIGLHS = LHS;
    USESTMPVARS = 0;
    while (LHS->CLS == XCWITHTMPVAR) {
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
  } while (!(TKELSE <= LEXER.TOKEN.ID && LEXER.TOKEN.ID <= TKEND));
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
  PBoolean ASCENDING;
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
  else if (LENGTH(&DIR) == 3 && DIR.chr[2] == 'R' && (DIR.chr[3] == '+' || DIR.chr[3] == '-')) OPTIONS.CHECKBOUNDS = DIR.chr[3] == '+';
}

void READTOKEN() {
  PBoolean STOP;
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
  ADDCONSTANT(MAKECONSTANT(str_make(2, "PI"), EXREALCONSTANT( 3.14159265358979E+000)));
  ADDVARIABLE(MAKEVARIABLE(str_make(5, "INPUT"), PRIMITIVETYPES.PTTEXT));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "OUTPUT"), PRIMITIVETYPES.PTTEXT));
  ADDVARIABLE(MAKEVARIABLE(str_make(6, "STDERR"), PRIMITIVETYPES.PTTEXT));
  ADDPSEUDOFN(str_make(3, "ORD"), TPFORD);
  ADDPSEUDOFN(str_make(4, "PRED"), TPFPRED);
  ADDPSEUDOFN(str_make(4, "SUCC"), TPFSUCC);
  ADDPSEUDOFN(str_make(3, "ABS"), TPFABS);
  ADDPSEUDOFN(str_make(3, "SQR"), TPFSQR);
  ADDFUNCTION(MAKEFUNCTION1(str_make(5, "ABS_i"), PRIMITIVETYPES.PTINTEGER, MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(5, "ABS_r"), PRIMITIVETYPES.PTREAL, MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(6, "ARCTAN"), PRIMITIVETYPES.PTREAL, MAKEARG(str_make(3, "TAN"), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "COS"), PRIMITIVETYPES.PTREAL, MAKEARG(str_make(5, "ANGLE"), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "EXP"), PRIMITIVETYPES.PTREAL, MAKEARG(str_make(3, "POW"), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(4, "FRAC"), PRIMITIVETYPES.PTREAL, MAKEARG(str_of('X'), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "INT"), PRIMITIVETYPES.PTREAL, MAKEARG(str_of('X'), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(2, "LN"), PRIMITIVETYPES.PTREAL, MAKEARG(str_of('X'), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "ODD"), PRIMITIVETYPES.PTBOOLEAN, MAKEARG(str_of('X'), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(5, "ROUND"), PRIMITIVETYPES.PTINTEGER, MAKEARG(str_of('X'), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "SIN"), PRIMITIVETYPES.PTREAL, MAKEARG(str_make(5, "ANGLE"), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(5, "SQR_i"), PRIMITIVETYPES.PTINTEGER, MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(5, "SQR_r"), PRIMITIVETYPES.PTREAL, MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(4, "SQRT"), PRIMITIVETYPES.PTREAL, MAKEARG(str_of('X'), PRIMITIVETYPES.PTREAL)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(5, "TRUNC"), PRIMITIVETYPES.PTINTEGER, MAKEARG(str_of('X'), PRIMITIVETYPES.PTREAL)));
  ADDPSEUDOFN(str_make(7, "DISPOSE"), TPFDISPOSE);
  ADDPSEUDOFN(str_make(3, "NEW"), TPFNEW);
  ADDPSEUDOFN(str_make(4, "READ"), TPFREAD);
  ADDPSEUDOFN(str_make(6, "READLN"), TPFREADLN);
  ADDPSEUDOFN(str_make(5, "WRITE"), TPFWRITE);
  ADDPSEUDOFN(str_make(7, "WRITELN"), TPFWRITELN);
  ADDFUNCTION(MAKEPROCEDURE2(str_make(6, "ASSIGN"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT), MAKECONSTARG(str_make(4, "NAME"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "CLOSE"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(3, "EOF"), PRIMITIVETYPES.PTBOOLEAN, MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(4, "EOLN"), PRIMITIVETYPES.PTBOOLEAN, MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(7, "FILEPOS"), PRIMITIVETYPES.PTINTEGER, MAKECONSTARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(8, "FILESIZE"), PRIMITIVETYPES.PTINTEGER, MAKECONSTARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "FLUSH"), MAKECONSTARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION0(str_make(8, "IORESULT"), PRIMITIVETYPES.PTINTEGER));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "RESET"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(7, "REWRITE"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE2(str_make(4, "SEEK"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT), MAKEARG(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(7, "SEEKEOF"), PRIMITIVETYPES.PTBOOLEAN, MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEFUNCTION1(str_make(8, "SEEKEOLN"), PRIMITIVETYPES.PTBOOLEAN, MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "CHDIR"), MAKECONSTARG(str_make(3, "DIR"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "ERASE"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT)));
  ADDFUNCTION(MAKEPROCEDURE2(str_make(6, "GETDIR"), MAKEARG(str_make(5, "DRIVE"), PRIMITIVETYPES.PTINTEGER), MAKEVARARG(str_make(3, "DIR"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "MKDIR"), MAKECONSTARG(str_make(3, "DIR"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEPROCEDURE2(str_make(6, "RENAME"), MAKEVARARG(str_of('F'), PRIMITIVETYPES.PTTEXT), MAKECONSTARG(str_make(4, "NAME"), PRIMITIVETYPES.PTSTRING)));
  ADDFUNCTION(MAKEPROCEDURE1(str_make(5, "RMDIR"), MAKECONSTARG(str_make(3, "DIR"), PRIMITIVETYPES.PTSTRING)));
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
  ADDPSEUDOFN(str_make(6, "RANDOM"), TPFRANDOM);
  ADDFUNCTION(MAKEPROCEDURE1(str_make(4, "HALT"), MAKEARG(str_make(4, "CODE"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION0(str_make(10, "PARAMCOUNT"), PRIMITIVETYPES.PTINTEGER));
  ADDFUNCTION(MAKEFUNCTION1(str_make(8, "PARAMSTR"), PRIMITIVETYPES.PTSTRING, MAKEARG(str_of('I'), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEFUNCTION0(str_make(8, "RANDOM_n"), PRIMITIVETYPES.PTREAL));
  ADDFUNCTION(MAKEFUNCTION1(str_make(8, "RANDOM_i"), PRIMITIVETYPES.PTINTEGER, MAKEARG(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER)));
  ADDFUNCTION(MAKEPROCEDURE0(str_make(9, "RANDOMIZE")));
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

struct record33 {
  PFile OUTPUT;
  PBoolean ISMULTISTATEMENT;
  PInteger INDENT;
  PBoolean NEWLINE;
  TOUTPUTTYPE LASTOUT;
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
  PInteger CT;
  if (CODEGEN.NEWLINE) do {
    PInteger first = 1;
    PInteger last = CODEGEN.INDENT;
    if (first <= last) {
      CT = first;
      while (1) {
        WRITE_s(&CODEGEN.OUTPUT, str_make(2, "  "), 0);
        if (CT == last) break;
        ++CT;
      }
    }
  } while(0);
  CODEGEN.NEWLINE = 0;
}

void OUTBEGIN() {
  CODEGEN.ISMULTISTATEMENT = 1;
  WRITE_c(&CODEGEN.OUTPUT, '{', 0);
  _OUTNEWLINE();
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
}

void OUTEND() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  WRITE_c(&CODEGEN.OUTPUT, '}', 0);
  _OUTNEWLINE();
}

void OUTENDSAMELINE() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  WRITE_c(&CODEGEN.OUTPUT, '}', 0);
}

void _OUTESCAPEDCHAR(PChar CH) {
  PInteger CODE;
  PInteger N1;
  PInteger N2;
  CODE = (int)CH;
  N1 = CODE / 16;
  N2 = CODE % 16;
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\x"), 0);
  if (N1 < 10) WRITE_c(&CODEGEN.OUTPUT, CHR(N1 + 48), 0);
  else WRITE_c(&CODEGEN.OUTPUT, CHR(N1 + 87), 0);
  if (N2 < 10) WRITE_c(&CODEGEN.OUTPUT, CHR(N2 + 48), 0);
  else WRITE_c(&CODEGEN.OUTPUT, CHR(N2 + 87), 0);
}

void _OUTCHAR(PChar CHR) {
  WRITE_c(&CODEGEN.OUTPUT, '\'', 0);
  if (CHR == '\'') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\'"), 0);
  else if (CHR == '\\') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\\\"), 0);
  else if (' ' <= CHR && CHR <= '~') WRITE_c(&CODEGEN.OUTPUT, CHR, 0);
  else _OUTESCAPEDCHAR(CHR);
  WRITE_c(&CODEGEN.OUTPUT, '\'', 0);
}

void _OUTCSTRING(PString *STR) {
  PInteger POS;
  PChar CH;
  WRITE_c(&CODEGEN.OUTPUT, '"', 0);
  do {
    PInteger first = 1;
    PInteger last = LENGTH(STR);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          CH = (*STR).chr[POS];
          if (CH < ' ' || CH > '~') {
            _OUTESCAPEDCHAR(CH);
            if (POS < LENGTH(STR) && LXISHEXDIGIT((*STR).chr[POS + 1])) WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\"\""), 0);
          }
          else {
            if (CH == '"') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\\""), 0);
            else if (CH == '\\') WRITE_s(&CODEGEN.OUTPUT, str_make(2, "\\\\"), 0);
            else WRITE_c(&CODEGEN.OUTPUT, CH, 0);
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_c(&CODEGEN.OUTPUT, '"', 0);
}

void _OUTSTRING(PString *STR) {
  if (LENGTH(STR) == 1) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(7, "str_of("), 0);
    _OUTCHAR((*STR).chr[1]);
    WRITE_c(&CODEGEN.OUTPUT, ')', 0);
  }
  else {
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, "str_make("), 0);
    WRITE_i(&CODEGEN.OUTPUT, LENGTH(STR), 0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTCSTRING(STR);
    WRITE_c(&CODEGEN.OUTPUT, ')', 0);
  }
}

PInteger _BINOPPREC(TEXPRESSIONOBJ *EXPR) {
  PInteger RESULT;
  PBoolean ISSETLEFT;
  PBoolean ISSETRIGHT;
  ISSETLEFT = ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR);
  ISSETRIGHT = ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR);
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
    case TKXOR:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 7;
      else RESULT = 9;
      break;
    case TKSHL:
      RESULT = 5;
      break;
    case TKSHR:
      RESULT = 5;
      break;
    case TKIN:
      if (EXISIMMEDIATE(EXPR->BINARY.RIGHT)) RESULT = 12;
      else RESULT = 1;
      break;
    case TKEQUALS:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 7;
      break;
    case TKNOTEQUALS:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 7;
      break;
    case TKLESSTHAN:
      RESULT = 6;
      break;
    case TKMORETHAN:
      RESULT = 6;
      break;
    case TKLESSOREQUALS:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 6;
      break;
    case TKMOREOREQUALS:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 6;
      break;
    default:
      INTERNALERROR(cat_ss(str_make(35, "Unknown precedence for operator in "), DESCRIBEEXPR(EXPR, 5)));
      break;
  }
  return RESULT;
}

PInteger _PRECEDENCE(TEXPRESSIONOBJ *EXPR) {
  PInteger RESULT;
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
    case XCWITHTMPVAR:
      RESULT = 0;
      break;
    case XCSUBRANGE:
      RESULT = 0;
      break;
    case XCSET:
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
      INTERNALERROR(cat_ss(str_make(23, "Unknown precedence for "), DESCRIBEEXPR(EXPR, 5)));
      break;
  }
  return RESULT;
}

void _OUTEXPRESSIONPARENSPREC(TEXPRESSIONOBJ *EXPR, PInteger PREC) {
  PBoolean USEPARENS;
  USEPARENS = _PRECEDENCE(EXPR) > PREC;
  if (USEPARENS) WRITE_c(&CODEGEN.OUTPUT, '(', 0);
  OUTEXPRESSION(EXPR);
  if (USEPARENS) WRITE_c(&CODEGEN.OUTPUT, ')', 0);
}

void _OUTEXPRESSIONPARENS(TEXPRESSIONOBJ *EXPR, TEXPRESSIONOBJ *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF));
}

void _OUTEXPRESSIONPARENSEXTRA(TEXPRESSIONOBJ *EXPR, TEXPRESSIONOBJ *REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF) - 1);
}

void _OUTSETTYPENAME(TPSTYPE *TYPEPTR) {
  PInteger NUMBYTES;
  NUMBYTES = GETTYPEHIGHBOUND(TYPEPTR->ELEMENTTYPEPTR) / 8 - GETTYPELOWBOUND(TYPEPTR->ELEMENTTYPEPTR) / 8 + 1;
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "PSet"), 0);
  WRITE_i(&CODEGEN.OUTPUT, 8 * NUMBYTES, 0);
}

void _OUTSETIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  PInteger LOWBOUND;
  PInteger HIGHBOUND;
  PInteger LOWBOUNDBYTE;
  PInteger SETSIZE;
  TEXSETIMMBOUNDSOBJ *BOUNDS;
  TPSTYPE *ELEMTYPEPTR;
  PInteger SETELEMS[32];
  PInteger POS;
  PInteger BYTENUM;
  PInteger BITNUM;
  BOUNDS = EXPR->IMMEDIATE.SETBOUNDS;
  ELEMTYPEPTR = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  LOWBOUND = GETTYPELOWBOUND(ELEMTYPEPTR);
  HIGHBOUND = GETTYPEHIGHBOUND(ELEMTYPEPTR);
  LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMTYPEPTR) / 8;
  SETSIZE = HIGHBOUND / 8 - LOWBOUND / 8 + 1;
  do {
    PInteger first = 1;
    PInteger last = SETSIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        SETELEMS[subrange(POS, 1, 32) - 1] = 0;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  while (BOUNDS != (void*)0) {
    if (BOUNDS->FIRST < LOWBOUND || BOUNDS->LAST > HIGHBOUND) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(str_make(4, "Set "), DESCRIBEEXPR(EXPR, 1)), str_make(19, " contains elements ")), str_make(27, "that are out of bounds for ")), TYPENAME(EXPR->TYPEPTR)));
    do {
      PInteger first = BOUNDS->FIRST;
      PInteger last = BOUNDS->LAST;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            BYTENUM = 1 + POS / 8 - LOWBOUNDBYTE;
            BITNUM = POS % 8;
            SETELEMS[subrange(BYTENUM, 1, 32) - 1] = SETELEMS[subrange(BYTENUM, 1, 32) - 1] | 1 << BITNUM;
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    BOUNDS = BOUNDS->NEXT;
  }
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "(PSet"), 0);
  WRITE_i(&CODEGEN.OUTPUT, SETSIZE * 8, 0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, ") { "), 0);
  do {
    PInteger first = 1;
    PInteger last = SETSIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
          WRITE_i(&CODEGEN.OUTPUT, SETELEMS[subrange(POS, 1, 32) - 1], 0);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, " }"), 0);
}

void _OUTEXSET(TEXPRESSIONOBJ *EXPR) {
  TPSTYPE *ELEMENTTYPEPTR;
  TEXSETEXPRBOUNDSOBJ *BOUNDS;
  TEXPRESSIONOBJ *FIRST;
  TEXPRESSIONOBJ *LAST;
  PInteger LOWBOUNDBYTE;
  ELEMENTTYPEPTR = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMENTTYPEPTR) / 8;
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "({ "), 0);
  _OUTSETTYPENAME(EXPR->TYPEPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, " dst = "), 0);
  _OUTSETIMMEDIATE(EXPR->SETBASE);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, "; "), 0);
  BOUNDS = EXPR->SETBOUNDS;
  while (BOUNDS != (void*)0) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(8, "set_set("), 0);
    FIRST = PFORD(EXCOERCE(COPYEXPR(BOUNDS->FIRST), ELEMENTTYPEPTR));
    if (BOUNDS->LAST == (void*)0) LAST = COPYEXPR(FIRST);
    else LAST = PFORD(EXCOERCE(COPYEXPR(BOUNDS->LAST), ELEMENTTYPEPTR));
    OUTEXPRESSION(FIRST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    OUTEXPRESSION(LAST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    WRITE_i(&CODEGEN.OUTPUT, LOWBOUNDBYTE, 0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(13, ", dst.bits); "), 0);
    DISPOSEEXPR(&FIRST);
    DISPOSEEXPR(&LAST);
    BOUNDS = BOUNDS->NEXT;
  }
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, "dst; })"), 0);
}

void _OUTEXIMMEDIATE(TEXPRESSIONOBJ *EXPR) {
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICNIL:
        WRITE_s(&CODEGEN.OUTPUT, str_make(8, "(void*)0"), 0);
        break;
      case XICBOOLEAN:
        if (with1->BOOLEANVAL) WRITE_c(&CODEGEN.OUTPUT, '1', 0);
        else WRITE_c(&CODEGEN.OUTPUT, '0', 0);
        break;
      case XICINTEGER:
        WRITE_i(&CODEGEN.OUTPUT, with1->INTEGERVAL, 0);
        break;
      case XICREAL:
        WRITE_r(&CODEGEN.OUTPUT, with1->REALVAL, 0, -1);
        break;
      case XICCHAR:
        _OUTCHAR(with1->CHARVAL);
        break;
      case XICSTRING:
        _OUTSTRING(&with1->STRINGVAL);
        break;
      case XICENUM:
        WRITE_s(&CODEGEN.OUTPUT, with1->ENUMPTR->VALUES[subrange(with1->ENUMORDINAL, 0, 127)], 0);
        break;
      case XICSET:
        _OUTSETIMMEDIATE(EXPR);
        break;
      default:
        break;
    }
  }
}

void _OUTBOUNDS(TPSTYPE *TYPEPTR) {
  WRITE_i(&CODEGEN.OUTPUT, GETTYPELOWBOUND(TYPEPTR), 0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
  WRITE_i(&CODEGEN.OUTPUT, GETTYPEHIGHBOUND(TYPEPTR), 0);
}

void _OUTARRAYINDEX(TEXPRESSIONOBJ *INDEX, TPSTYPE *TYPEPTR) {
  PInteger LOWBOUND;
  TEXPRESSIONOBJ *SIZE;
  LOWBOUND = GETTYPELOWBOUND(TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  SIZE = EXBINARYOP(PFORD(COPYEXPR(INDEX)), EXINTEGERCONSTANT(LOWBOUND), TKMINUS);
  OUTEXPRESSION(SIZE);
  DISPOSEEXPR(&SIZE);
}

void _OUTADDRESS(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->CLS == XCPOINTER) OUTEXPRESSION(EXPR->POINTEREXPR);
  else if (EXPR->CLS == XCVARIABLE && EXPR->VARPTR->ISREFERENCE) WRITE_s(&CODEGEN.OUTPUT, EXPR->VARPTR->NAME, 0);
  else {
    WRITE_c(&CODEGEN.OUTPUT, '&', 0);
    _OUTEXPRESSIONPARENSPREC(EXPR, 2);
  }
}

void _OUTEXSUBRANGE(TEXPRESSIONOBJ *EXPR) {
  if (!OPTIONS.CHECKBOUNDS) OUTEXPRESSION(EXPR->SUBRANGEPARENT);
  else {
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, "subrange("), 0);
    OUTEXPRESSION(EXPR->SUBRANGEPARENT);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTBOUNDS(EXPR->TYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, ')', 0);
  }
}

void _OUTEXVARIABLE(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->VARPTR->ISREFERENCE) {
    WRITE_c(&CODEGEN.OUTPUT, '*', 0);
    WRITE_s(&CODEGEN.OUTPUT, EXPR->VARPTR->NAME, 0);
  }
  else WRITE_s(&CODEGEN.OUTPUT, EXPR->VARPTR->NAME, 0);
}

void _OUTEXFIELDACCESS(TEXPRESSIONOBJ *EXPR) {
  {
    TEXPRESSIONOBJ *with1 = EXPR->RECEXPR;
    {
      if (with1->CLS == XCPOINTER) {
        _OUTEXPRESSIONPARENS(with1->POINTEREXPR, EXPR);
        WRITE_s(&CODEGEN.OUTPUT, str_make(2, "->"), 0);
      }
      else if (with1->CLS == XCVARIABLE && with1->VARPTR->ISREFERENCE) {
        WRITE_s(&CODEGEN.OUTPUT, with1->VARPTR->NAME, 0);
        WRITE_s(&CODEGEN.OUTPUT, str_make(2, "->"), 0);
      }
      else {
        _OUTEXPRESSIONPARENS(EXPR->RECEXPR, EXPR);
        WRITE_c(&CODEGEN.OUTPUT, '.', 0);
      }
      WRITE_s(&CODEGEN.OUTPUT, EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[subrange(EXPR->RECFIELDNUM, 1, 32) - 1].NAME, 0);
    }
  }
}

void _OUTEXFUNCTIONCALL(TEXPRESSIONOBJ *EXPR) {
  PInteger POS;
  _OUTEXPRESSIONPARENS(EXPR->FNEXPR, EXPR);
  WRITE_c(&CODEGEN.OUTPUT, '(', 0);
  do {
    PInteger first = 1;
    PInteger last = EXPR->CALLARGS.SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
          if (EXPR->FNEXPR->FNPTR->ARGS[subrange(POS, 1, 8) - 1].ISREFERENCE) {
            if (!EXPR->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]->ISASSIGNABLE) COMPILEERROR(str_make(45, "Pass-by-reference argument must be assignable"));
            _OUTADDRESS(EXPR->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]);
          }
          else OUTEXPRESSION(EXPR->CALLARGS.VALUES[subrange(POS, 1, 8) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_c(&CODEGEN.OUTPUT, ')', 0);
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
      INTERNALERROR(cat_ss(str_make(31, "Unimplemented special function "), DESCRIBEEXPR(EXPR, 5)));
      break;
  }
}

void _OUTEXUNARYOP(TEXPRESSIONOBJ *EXPR) {
  if (EXPR->UNARY.OP == TKMINUS) WRITE_c(&CODEGEN.OUTPUT, '-', 0);
  else if (EXPR->UNARY.OP == TKNOT && ISBOOLEANTYPE(EXPR->TYPEPTR)) WRITE_c(&CODEGEN.OUTPUT, '!', 0);
  else if (EXPR->UNARY.OP == TKNOT && ISINTEGERTYPE(EXPR->TYPEPTR)) WRITE_c(&CODEGEN.OUTPUT, '~', 0);
  _OUTEXPRESSIONPARENS(EXPR->UNARY.PARENT, EXPR);
}

PBoolean _ISARITHMETICOP(TLXTOKENID OP) {
  PBoolean RESULT;
  RESULT = TKPLUS <= OP && OP <= TKSLASH || OP == TKDIV || OP == TKMOD;
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

PBoolean _ISLOGICALORBITWISEOP(TLXTOKENID OP) {
  PBoolean RESULT;
  RESULT = OP == TKAND || OP == TKOR || OP == TKXOR;
  return RESULT;
}

PBoolean _ISBITWISEOP(TLXTOKENID OP) {
  PBoolean RESULT;
  RESULT = TKSHL <= OP && OP <= TKSHR;
  return RESULT;
}

PString _GETLOGICALOP(TLXTOKENID OP) {
  PString RESULT;
  if (OP == TKAND) RESULT = str_make(2, "&&");
  else if (OP == TKOR) RESULT = str_make(2, "||");
  else if (OP == TKXOR) RESULT = str_make(2, "!=");
  else INTERNALERROR(cat_ss(str_make(31, "Unimplemented logical operator "), LXTOKENNAME(OP)));
  return RESULT;
}

PString _GETBITWISEOP(TLXTOKENID OP) {
  PString RESULT;
  if (OP == TKAND) RESULT = str_of('&');
  else if (OP == TKOR) RESULT = str_of('|');
  else if (OP == TKXOR) RESULT = str_of('^');
  else if (OP == TKSHL) RESULT = str_make(2, "<<");
  else if (OP == TKSHR) RESULT = str_make(2, ">>");
  else INTERNALERROR(cat_ss(str_make(31, "Unimplemented bitwise operator "), LXTOKENNAME(OP)));
  return RESULT;
}

PBoolean _ISRELATIONALOP(TLXTOKENID OP) {
  PBoolean RESULT;
  RESULT = TKEQUALS <= OP && OP <= TKMORETHAN || TKNOTEQUALS <= OP && OP <= TKMOREOREQUALS;
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

void _OUTEXSETOPERATION(TEXPRESSIONOBJ *LEFT, TEXPRESSIONOBJ *RIGHT, TLXTOKENID OP) {
  TPSTYPE *ELEMTYPEPTR;
  PInteger LOWBOUND;
  PInteger HIGHBOUND;
  PInteger LOWBOUNDBYTE;
  PInteger SETSIZE;
  ELEMTYPEPTR = RIGHT->TYPEPTR->ELEMENTTYPEPTR;
  if (OP == TKLESSOREQUALS) _OUTEXSETOPERATION(RIGHT, LEFT, TKMOREOREQUALS);
  else if (OP == TKNOTEQUALS) {
    WRITE_c(&CODEGEN.OUTPUT, '!', 0);
    _OUTEXSETOPERATION(LEFT, RIGHT, TKEQUALS);
  }
  else if (OP == TKIN) {
    LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMTYPEPTR) / 8;
    WRITE_s(&CODEGEN.OUTPUT, str_make(7, "set_in("), 0);
    OUTEXPRESSION(LEFT);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    WRITE_i(&CODEGEN.OUTPUT, LOWBOUNDBYTE, 0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, ".bits)"), 0);
  }
  else {
    LOWBOUND = GETTYPELOWBOUND(ELEMTYPEPTR);
    HIGHBOUND = GETTYPEHIGHBOUND(ELEMTYPEPTR);
    SETSIZE = HIGHBOUND / 8 - LOWBOUND / 8 + 1;
    if (OP == TKEQUALS) {
      WRITE_s(&CODEGEN.OUTPUT, str_make(11, "set_equals("), 0);
      _OUTEXPRESSIONPARENSPREC(LEFT, 1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(7, ".bits, "), 0);
      _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(7, ".bits, "), 0);
      WRITE_i(&CODEGEN.OUTPUT, SETSIZE, 0);
      WRITE_c(&CODEGEN.OUTPUT, ')', 0);
    }
    else if (OP == TKMOREOREQUALS) {
      WRITE_s(&CODEGEN.OUTPUT, str_make(15, "set_issuperset("), 0);
      _OUTEXPRESSIONPARENSPREC(LEFT, 1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(7, ".bits, "), 0);
      _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(7, ".bits, "), 0);
      WRITE_i(&CODEGEN.OUTPUT, SETSIZE, 0);
      WRITE_c(&CODEGEN.OUTPUT, ')', 0);
    }
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(7, "({ PSet"), 0);
      WRITE_i(&CODEGEN.OUTPUT, SETSIZE * 8, 0);
      WRITE_s(&CODEGEN.OUTPUT, str_make(6, " dst; "), 0);
      switch (OP) {
        case TKPLUS:
          WRITE_s(&CODEGEN.OUTPUT, str_make(10, "set_union("), 0);
          break;
        case TKMINUS:
          WRITE_s(&CODEGEN.OUTPUT, str_make(15, "set_difference("), 0);
          break;
        case TKASTERISK:
          WRITE_s(&CODEGEN.OUTPUT, str_make(17, "set_intersection("), 0);
          break;
        default:
          INTERNALERROR(cat_ss(str_make(44, "Materialized set operation not implemented: "), LXTOKENNAME(OP)));
          break;
      }
      _OUTEXPRESSIONPARENSPREC(LEFT, 1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(7, ".bits, "), 0);
      _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(17, ".bits, dst.bits, "), 0);
      WRITE_i(&CODEGEN.OUTPUT, SETSIZE, 0);
      WRITE_s(&CODEGEN.OUTPUT, str_make(10, "); dst; })"), 0);
    }
  }
}

void _OUTEXBINARYOP(TEXPRESSIONOBJ *EXPR) {
  PChar LTYPE;
  PChar RTYPE;
  {
    TEXBINARYOP *with1 = &EXPR->BINARY;
    {
      if (ISBOOLEANTYPE(with1->LEFT->TYPEPTR) && ISBOOLEANTYPE(with1->RIGHT->TYPEPTR)) {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISLOGICALORBITWISEOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          WRITE_s(&CODEGEN.OUTPUT, _GETLOGICALOP(with1->OP), 0);
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
        }
        else if (_ISRELATIONALOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP), 0);
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
        }
        else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
      else if (ISNUMERICTYPE(with1->LEFT->TYPEPTR) && ISNUMERICTYPE(with1->RIGHT->TYPEPTR)) {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISARITHMETICOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          WRITE_s(&CODEGEN.OUTPUT, _GETARITHMETICOP(with1->OP), 0);
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
        }
        else if (_ISLOGICALORBITWISEOP(with1->OP) || _ISBITWISEOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          WRITE_s(&CODEGEN.OUTPUT, _GETBITWISEOP(with1->OP), 0);
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
        }
        else if (_ISRELATIONALOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP), 0);
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
        }
        else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
      else if (ISSTRINGYTYPE(with1->LEFT->TYPEPTR) && ISSTRINGYTYPE(with1->RIGHT->TYPEPTR)) {
        if (ISCHARTYPE(with1->LEFT->TYPEPTR)) LTYPE = 'c';
        else LTYPE = 's';
        if (ISCHARTYPE(with1->RIGHT->TYPEPTR)) RTYPE = 'c';
        else RTYPE = 's';
        if (with1->OP == TKPLUS) {
          WRITE_s(&CODEGEN.OUTPUT, str_make(4, "cat_"), 0);
          WRITE_c(&CODEGEN.OUTPUT, LTYPE, 0);
          WRITE_c(&CODEGEN.OUTPUT, RTYPE, 0);
          WRITE_c(&CODEGEN.OUTPUT, '(', 0);
          OUTEXPRESSION(with1->LEFT);
          WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
          OUTEXPRESSION(with1->RIGHT);
          WRITE_c(&CODEGEN.OUTPUT, ')', 0);
        }
        else if (ISCHARTYPE(with1->LEFT->TYPEPTR) && ISCHARTYPE(with1->RIGHT->TYPEPTR)) {
          _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
          if (_ISRELATIONALOP(with1->OP)) {
            WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
            WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP), 0);
            WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          }
          else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
          _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
        }
        else {
          WRITE_s(&CODEGEN.OUTPUT, str_make(4, "cmp_"), 0);
          WRITE_c(&CODEGEN.OUTPUT, LTYPE, 0);
          WRITE_c(&CODEGEN.OUTPUT, RTYPE, 0);
          WRITE_c(&CODEGEN.OUTPUT, '(', 0);
          OUTEXPRESSION(with1->LEFT);
          WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
          OUTEXPRESSION(with1->RIGHT);
          if (_ISRELATIONALOP(with1->OP)) {
            WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "), 0);
            WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP), 0);
            WRITE_s(&CODEGEN.OUTPUT, str_make(2, " 0"), 0);
          }
          else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        }
      }
      else if (ISSETTYPE(with1->RIGHT->TYPEPTR)) _OUTEXSETOPERATION(with1->LEFT, with1->RIGHT, with1->OP);
      else {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISRELATIONALOP(with1->OP)) {
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
          WRITE_s(&CODEGEN.OUTPUT, _GETRELATIONALOP(with1->OP), 0);
          WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
        }
        else COMPILEERROR(cat_ss(str_make(22, "Not a valid operator: "), LXTOKENNAME(with1->OP)));
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
    }
  }
}

void _OUTEXWITHTMPVAR(TEXPRESSIONOBJ *EXPR) {
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "({ "), 0);
  while (EXPR->CLS == XCWITHTMPVAR) {
    OUTVARIABLEDECLARATION(*EXPR->TMPVAR->VARPTR);
    WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "), 0);
    OUTEXPRESSION(EXPR->TMPVARVALUE);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, "; "), 0);
    EXPR = EXPR->TMPVARCHILD;
  }
  OUTEXPRESSION(EXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "; })"), 0);
}

void OUTEXPRESSION(TEXPRESSIONOBJ *EXPR) {
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      _OUTEXIMMEDIATE(EXPR);
      break;
    case XCTOSTRING:
      {
        WRITE_s(&CODEGEN.OUTPUT, str_make(7, "str_of("), 0);
        OUTEXPRESSION(EXPR->TOSTRPARENT);
        WRITE_c(&CODEGEN.OUTPUT, ')', 0);
      }
      break;
    case XCTOREAL:
      {
        WRITE_s(&CODEGEN.OUTPUT, str_make(8, "(double)"), 0);
        OUTEXPRESSION(EXPR->TOREALPARENT);
      }
      break;
    case XCWITHTMPVAR:
      _OUTEXWITHTMPVAR(EXPR);
      break;
    case XCSUBRANGE:
      _OUTEXSUBRANGE(EXPR);
      break;
    case XCSET:
      _OUTEXSET(EXPR);
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
        WRITE_c(&CODEGEN.OUTPUT, '[', 0);
        _OUTARRAYINDEX(EXPR->ARRAYINDEX, EXPR->ARRAYEXPR->TYPEPTR);
        WRITE_c(&CODEGEN.OUTPUT, ']', 0);
      }
      break;
    case XCPOINTER:
      {
        WRITE_c(&CODEGEN.OUTPUT, '*', 0);
        _OUTEXPRESSIONPARENS(EXPR->POINTEREXPR, EXPR);
      }
      break;
    case XCSTRINGCHAR:
      {
        _OUTEXPRESSIONPARENS(EXPR->STRINGEXPR, EXPR);
        WRITE_s(&CODEGEN.OUTPUT, str_make(5, ".chr["), 0);
        OUTEXPRESSION(EXPR->STRINGINDEX);
        WRITE_c(&CODEGEN.OUTPUT, ']', 0);
      }
      break;
    case XCFNREF:
      WRITE_s(&CODEGEN.OUTPUT, EXPR->FNPTR->NAME, 0);
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
  PInteger POSINENUM;
  _OUTBLANKLINE(TOTENUMVAL);
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(22, "const char* enumvalues"), 0);
  WRITE_i(&CODEGEN.OUTPUT, ENUMPTR->ID, 0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, "[] = { "), 0);
  do {
    PInteger first = 0;
    PInteger last = ENUMPTR->SIZE - 1;
    if (first <= last) {
      POSINENUM = first;
      while (1) {
        {
          if (POSINENUM != 0) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
          WRITE_c(&CODEGEN.OUTPUT, '"', 0);
          WRITE_s(&CODEGEN.OUTPUT, ENUMPTR->VALUES[subrange(POSINENUM, 0, 127)], 0);
          WRITE_c(&CODEGEN.OUTPUT, '"', 0);
        }
        if (POSINENUM == last) break;
        ++POSINENUM;
      }
    }
  } while(0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " };"), 0);
  _OUTNEWLINE();
}

void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY *CHECKPOINT) {
  TPSDEFENTRY *DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != (void*)0) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCENUM) {
      if (!DEF->TYPEPTR->ENUMPTR->VALUESHAVEBEENOUTPUT) OUTENUMVALUES(DEF->TYPEPTR->ENUMPTR);
      DEF->TYPEPTR->ENUMPTR->VALUESHAVEBEENOUTPUT = 1;
    }
    DEF = DEF->NEXT;
  }
}

PString OUTVARIABLENAME(PString NAME, PBoolean ISREFERENCE) {
  PString RESULT;
  if (ISREFERENCE) RESULT = cat_cs('*', NAME);
  else RESULT = NAME;
  return RESULT;
}

void OUTTYPEREFERENCE(TPSTYPE *TYPEPTR) {
  if (TYPEPTR == (void*)0) WRITE_s(&CODEGEN.OUTPUT, str_make(4, "void"), 0);
  else if (TYPEPTR->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEPTR->POINTEDTYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, '*', 0);
  }
  else if (TYPEPTR->CLS == TTCBOOLEAN) WRITE_s(&CODEGEN.OUTPUT, str_make(8, "PBoolean"), 0);
  else if (TYPEPTR->CLS == TTCINTEGER) WRITE_s(&CODEGEN.OUTPUT, str_make(8, "PInteger"), 0);
  else if (TYPEPTR->CLS == TTCREAL) WRITE_s(&CODEGEN.OUTPUT, str_make(5, "PReal"), 0);
  else if (TYPEPTR->CLS == TTCCHAR) WRITE_s(&CODEGEN.OUTPUT, str_make(5, "PChar"), 0);
  else if (TYPEPTR->CLS == TTCSTRING) WRITE_s(&CODEGEN.OUTPUT, str_make(7, "PString"), 0);
  else if (TYPEPTR->CLS == TTCTEXT) WRITE_s(&CODEGEN.OUTPUT, str_make(5, "PFile"), 0);
  else if (TYPEPTR->CLS == TTCENUM) {
    if (TYPEPTR->ENUMPTR->HASBEENDEFINED && cmp_ss(TYPEPTR->NAME, str_make(0, "")) != 0) WRITE_s(&CODEGEN.OUTPUT, TYPEPTR->NAME, 0);
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"), 0);
      WRITE_i(&CODEGEN.OUTPUT, TYPEPTR->ENUMPTR->ID, 0);
    }
  }
  else if (TYPEPTR->CLS == TTCRANGE) OUTTYPEREFERENCE(GETFUNDAMENTALTYPE(TYPEPTR));
  else if (TYPEPTR->CLS == TTCSET) _OUTSETTYPENAME(TYPEPTR);
  else if (TYPEPTR->CLS == TTCRECORD) {
    if (TYPEPTR->RECPTR->HASBEENDEFINED && cmp_ss(TYPEPTR->NAME, str_make(0, "")) != 0) WRITE_s(&CODEGEN.OUTPUT, TYPEPTR->NAME, 0);
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(13, "struct record"), 0);
      WRITE_i(&CODEGEN.OUTPUT, TYPEPTR->RECPTR->ID, 0);
    }
  }
  else if (TYPEPTR->CLS == TTCARRAY) {
    OUTTYPEREFERENCE(TYPEPTR->ARRAYDEF.VALUETYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, '*', 0);
  }
  else COMPILEERROR(cat_ss(str_make(30, "Error writing type reference: "), TYPENAME(TYPEPTR)));
}

void OUTNAMEANDTYPE(PString NAME, TPSTYPE *TYPEPTR);

void OUTNAMEANDRECORD(PString NAME, TPSRECORDDEF *RECPTR) {
  PInteger POS;
  PInteger NUMVARIANT;
  NUMVARIANT = 0;
  WRITE_s(&CODEGEN.OUTPUT, str_make(13, "struct record"), 0);
  WRITE_i(&CODEGEN.OUTPUT, RECPTR->ID, 0);
  if (!RECPTR->HASBEENDEFINED) {
    WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
    OUTBEGIN();
    do {
      PInteger first = 1;
      PInteger last = RECPTR->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (RECPTR->NUMVARIANTS > NUMVARIANT && RECPTR->VARIANTBOUNDS[subrange(NUMVARIANT + 1, 1, 32) - 1] == POS) {
              NUMVARIANT = NUMVARIANT + 1;
              if (NUMVARIANT == 1) {
                _OUTINDENT();
                WRITE_s(&CODEGEN.OUTPUT, str_make(6, "union "), 0);
                OUTBEGIN();
              }
              else {
                OUTENDSAMELINE();
                WRITE_c(&CODEGEN.OUTPUT, ';', 0);
                _OUTNEWLINE();
              }
              _OUTINDENT();
              WRITE_s(&CODEGEN.OUTPUT, str_make(7, "struct "), 0);
              OUTBEGIN();
            }
            _OUTINDENT();
            OUTNAMEANDTYPE(RECPTR->FIELDS[subrange(POS, 1, 32) - 1].NAME, RECPTR->FIELDS[subrange(POS, 1, 32) - 1].TYPEPTR);
            WRITE_c(&CODEGEN.OUTPUT, ';', 0);
            _OUTNEWLINE();
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    if (NUMVARIANT > 0) {
      OUTENDSAMELINE();
      WRITE_c(&CODEGEN.OUTPUT, ';', 0);
      _OUTNEWLINE();
      OUTENDSAMELINE();
      WRITE_c(&CODEGEN.OUTPUT, ';', 0);
      _OUTNEWLINE();
    }
    OUTENDSAMELINE();
    RECPTR->HASBEENDEFINED = 1;
  }
  WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
  WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
}

void OUTNAMEANDENUM(PString NAME, TPSENUMDEF *ENUMPTR) {
  PInteger POS;
  WRITE_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"), 0);
  WRITE_i(&CODEGEN.OUTPUT, ENUMPTR->ID, 0);
  if (!ENUMPTR->HASBEENDEFINED) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(3, " { "), 0);
    do {
      PInteger first = 0;
      PInteger last = ENUMPTR->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS > 0) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
            WRITE_s(&CODEGEN.OUTPUT, ENUMPTR->VALUES[subrange(POS, 0, 127)], 0);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, " }"), 0);
    ENUMPTR->HASBEENDEFINED = 1;
  }
  WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
  WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
}

void OUTNAMEANDARRAY(PString NAME, TPSTYPE *TYPEPTR) {
  TPSTYPE *THETYPE;
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
  OUTNAMEANDTYPE(NAME, THETYPE);
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) {
    WRITE_c(&CODEGEN.OUTPUT, '[', 0);
    WRITE_i(&CODEGEN.OUTPUT, GETBOUNDEDTYPESIZE(TYPEPTR->ARRAYDEF.INDEXTYPEPTR), 0);
    WRITE_c(&CODEGEN.OUTPUT, ']', 0);
    THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
  }
}

void OUTNAMEANDTYPE(PString NAME, TPSTYPE *TYPEPTR) {
  if (TYPEPTR == (void*)0) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, "void "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEPTR->POINTEDTYPEPTR);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, " *"), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->ALIASFOR != (void*)0 && cmp_ss(TYPEPTR->NAME, str_make(0, "")) != 0) {
    WRITE_s(&CODEGEN.OUTPUT, TYPEPTR->NAME, 0);
    WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCBOOLEAN) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, "PBoolean "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCINTEGER) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, "PInteger "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCREAL) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "PReal "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCCHAR) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "PChar "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCSTRING) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(8, "PString "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCTEXT) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "PFile "), 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
  else if (TYPEPTR->CLS == TTCENUM) OUTNAMEANDENUM(NAME, TYPEPTR->ENUMPTR);
  else if (TYPEPTR->CLS == TTCRANGE) OUTNAMEANDTYPE(NAME, GETFUNDAMENTALTYPE(TYPEPTR));
  else if (TYPEPTR->CLS == TTCSET) {
    _OUTSETTYPENAME(TYPEPTR);
    WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
    WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  }
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
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, "typedef "), 0);
  OUTNAMEANDTYPE(NAME, TYPEPTR->ALIASFOR);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
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
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, "{ "), 0);
}

void OUTCONSTANTARRAYSEPARATOR() {
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
}

void OUTCONSTANTARRAYEND() {
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, " }"), 0);
}

void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF) {
  if (VARDEF.ISCONSTANT) WRITE_s(&CODEGEN.OUTPUT, str_make(6, "const "), 0);
  OUTNAMEANDTYPE(OUTVARIABLENAME(VARDEF.NAME, VARDEF.ISREFERENCE), VARDEF.TYPEPTR);
}

void OUTVARIABLEDEFINITION(TPSVARIABLE *VARPTR) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE *VARPTR) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "), 0);
}

void OUTCONSTANTDEFINITIONEND() {
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF) {
  PInteger POS;
  _OUTINDENT();
  OUTNAMEANDTYPE(DEF.NAME, DEF.RETURNTYPEPTR);
  WRITE_c(&CODEGEN.OUTPUT, '(', 0);
  do {
    PInteger first = 1;
    PInteger last = DEF.ARGCOUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          OUTVARIABLEDECLARATION(DEF.ARGS[subrange(POS, 1, 8) - 1]);
          if (POS != DEF.ARGCOUNT) WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  WRITE_c(&CODEGEN.OUTPUT, ')', 0);
}

void OUTFUNCTIONDECLARATION(TPSFUNCTION *FNPTR) {
  _OUTBLANKLINE(TOTFUNDEC);
  OUTFUNCTIONPROTOTYPE(*FNPTR);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTFUNCTIONDEFINITION(TPSFUNCTION *FNPTR) {
  _OUTBLANKLINE(TOTFUNDEF);
  OUTFUNCTIONPROTOTYPE(*FNPTR);
  WRITE_c(&CODEGEN.OUTPUT, ' ', 0);
  OUTBEGIN();
}

void OUTFUNCTIONEND(TPSFUNCTION *FNPTR) {
  if (FNPTR->RETURNTYPEPTR != (void*)0) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(14, "return RESULT;"), 0);
    _OUTNEWLINE();
  }
  OUTEND();
}

void OUTPROGRAMHEADING(PString NAME) {
  WRITE_s(&CODEGEN.OUTPUT, str_make(12, "/* Program: "), 0);
  WRITE_s(&CODEGEN.OUTPUT, NAME, 0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " */"), 0);
  _OUTNEWLINE();
  WRITE_s(&CODEGEN.OUTPUT, str_make(20, "#include \"pascual.h\""), 0);
  _OUTNEWLINE();
}

PChar SHORTTYPENAME(TPSTYPE *TYPEPTR) {
  PChar RESULT;
  TYPEPTR = GETFUNDAMENTALTYPE(TYPEPTR);
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = 'b';
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = 'i';
  else if (ISREALTYPE(TYPEPTR)) RESULT = 'r';
  else if (ISCHARTYPE(TYPEPTR)) RESULT = 'c';
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = 's';
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(5, "Type "), TYPENAME(TYPEPTR)), str_make(26, " is not representable for ")), str_make(24, "READ, WRITE, STR, or VAL")));
  return RESULT;
}

void _OUTREAD(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *SRC;
  TEXREADARGS *READARG;
  PBoolean LINEFEED;
  PBoolean BRACES;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  LINEFEED = EXPR->PSEUDOFNCALL.PSEUDOFN == TPFREADLN;
  READARG = EXPR->PSEUDOFNCALL.READARGS;
  BRACES = !CODEGEN.ISMULTISTATEMENT && READARG != (void*)0 && (READARG->NEXT != (void*)0 || LINEFEED);
  if (BRACES) OUTBEGIN();
  while (READARG != (void*)0) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, "READ_"), 0);
    WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(READARG->ARG->TYPEPTR), 0);
    WRITE_c(&CODEGEN.OUTPUT, '(', 0);
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTADDRESS(READARG->ARG);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
    _OUTNEWLINE();
    READARG = READARG->NEXT;
  }
  if (LINEFEED) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(7, "READLN("), 0);
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
    _OUTNEWLINE();
  }
  if (BRACES) OUTEND();
}

void _OUTWRITE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *DST;
  TEXWRITEARGS *WRITEARG;
  PBoolean LINEFEED;
  PBoolean BRACES;
  TPSTYPE *TYPEPTR;
  DST = EXPR->PSEUDOFNCALL.ARG1;
  LINEFEED = EXPR->PSEUDOFNCALL.PSEUDOFN == TPFWRITELN;
  WRITEARG = EXPR->PSEUDOFNCALL.WRITEARGS;
  BRACES = !CODEGEN.ISMULTISTATEMENT && WRITEARG != (void*)0 && (WRITEARG->NEXT != (void*)0 || LINEFEED);
  if (BRACES) OUTBEGIN();
  while (WRITEARG != (void*)0) {
    TYPEPTR = GETFUNDAMENTALTYPE(WRITEARG->ARG->TYPEPTR);
    if (ISENUMTYPE(TYPEPTR)) {
      _OUTINDENT();
      WRITE_s(&CODEGEN.OUTPUT, str_make(8, "WRITE_e("), 0);
      _OUTADDRESS(DST);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
      OUTEXPRESSION(WRITEARG->ARG);
      WRITE_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"), 0);
      WRITE_i(&CODEGEN.OUTPUT, TYPEPTR->ENUMPTR->ID, 0);
      WRITE_s(&CODEGEN.OUTPUT, str_make(5, ", 0);"), 0);
      _OUTNEWLINE();
    }
    else if (ISREALTYPE(TYPEPTR)) {
      _OUTINDENT();
      WRITE_s(&CODEGEN.OUTPUT, str_make(8, "WRITE_r("), 0);
      _OUTADDRESS(DST);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
      OUTEXPRESSION(WRITEARG->ARG);
      WRITE_s(&CODEGEN.OUTPUT, str_make(9, ", 0, -1);"), 0);
      _OUTNEWLINE();
    }
    else {
      _OUTINDENT();
      WRITE_s(&CODEGEN.OUTPUT, str_make(6, "WRITE_"), 0);
      WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(TYPEPTR), 0);
      WRITE_c(&CODEGEN.OUTPUT, '(', 0);
      _OUTADDRESS(DST);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
      OUTEXPRESSION(WRITEARG->ARG);
      WRITE_s(&CODEGEN.OUTPUT, str_make(5, ", 0);"), 0);
      _OUTNEWLINE();
    }
    WRITEARG = WRITEARG->NEXT;
  }
  if (LINEFEED) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(8, "WRITELN("), 0);
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
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
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "STR_e("), 0);
    OUTEXPRESSION(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"), 0);
    WRITE_i(&CODEGEN.OUTPUT, SRC->TYPEPTR->ENUMPTR->ID, 0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, ", 0, "), 0);
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
    _OUTNEWLINE();
  }
  else if (ISREALTYPE(SRC->TYPEPTR)) {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "STR_r("), 0);
    OUTEXPRESSION(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(9, ", 0, -1, "), 0);
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(4, "STR_"), 0);
    WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(SRC->TYPEPTR), 0);
    WRITE_c(&CODEGEN.OUTPUT, '(', 0);
    OUTEXPRESSION(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, ", 0, "), 0);
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
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
    WRITE_s(&CODEGEN.OUTPUT, str_make(6, "VAL_e("), 0);
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    TMPEXPR = EXINTEGERCONSTANT(DST->TYPEPTR->ENUMPTR->SIZE);
    OUTEXPRESSION(TMPEXPR);
    DISPOSEEXPR(&TMPEXPR);
    WRITE_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"), 0);
    WRITE_i(&CODEGEN.OUTPUT, DST->TYPEPTR->ENUMPTR->ID, 0);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTADDRESS(CODE);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    WRITE_s(&CODEGEN.OUTPUT, str_make(4, "VAL_"), 0);
    WRITE_c(&CODEGEN.OUTPUT, SHORTTYPENAME(DST->TYPEPTR), 0);
    WRITE_c(&CODEGEN.OUTPUT, '(', 0);
    _OUTADDRESS(SRC);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTADDRESS(DST);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
    _OUTADDRESS(CODE);
    WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
    _OUTNEWLINE();
  }
}

void _OUTNEW(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *PTR;
  PTR = EXPR->PSEUDOFNCALL.ARG1;
  _OUTINDENT();
  OUTEXPRESSION(PTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(17, " = malloc(sizeof("), 0);
  OUTTYPEREFERENCE(PTR->TYPEPTR->POINTEDTYPEPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "));"), 0);
  _OUTNEWLINE();
}

void _OUTDISPOSE(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *PTR;
  PTR = EXPR->PSEUDOFNCALL.ARG1;
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "free("), 0);
  OUTEXPRESSION(PTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
  _OUTNEWLINE();
}

void _OUTORD(TEXPRESSIONOBJ *EXPR) {
  if (ISCHARTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    WRITE_s(&CODEGEN.OUTPUT, str_make(5, "(int)"), 0);
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
    if (!OPTIONS.CHECKBOUNDS) {
      TMPEXPR = EXBINARYOP(PFORD(COPYEXPR(EXPR->PSEUDOFNCALL.ARG1)), EXINTEGERCONSTANT(1), TKMINUS);
      OUTEXPRESSION(TMPEXPR);
      DISPOSEEXPR(&TMPEXPR);
    }
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(5, "pred("), 0);
      OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
      _OUTBOUNDS(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR);
      WRITE_c(&CODEGEN.OUTPUT, ')', 0);
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
    if (!OPTIONS.CHECKBOUNDS) {
      TMPEXPR = EXBINARYOP(PFORD(COPYEXPR(EXPR->PSEUDOFNCALL.ARG1)), EXINTEGERCONSTANT(1), TKPLUS);
      OUTEXPRESSION(TMPEXPR);
      DISPOSEEXPR(&TMPEXPR);
    }
    else {
      WRITE_s(&CODEGEN.OUTPUT, str_make(5, "succ("), 0);
      OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
      WRITE_s(&CODEGEN.OUTPUT, str_make(2, ", "), 0);
      _OUTBOUNDS(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR);
      WRITE_c(&CODEGEN.OUTPUT, ')', 0);
    }
  }
  else COMPILEERROR(cat_ss(str_make(30, "Expected an ordinal type, got "), TYPENAME(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)));
}

void OUTASSIGN(TEXPRESSIONOBJ *LHS, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  OUTEXPRESSION(LHS);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "), 0);
  OUTEXPRESSION(RHS);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTDECLAREANDASSIGN(TPSVARIABLE *VARPTR, TEXPRESSIONOBJ *RHS) {
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "), 0);
  if (VARPTR->ISREFERENCE) _OUTADDRESS(RHS);
  else OUTEXPRESSION(RHS);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTIF(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "if ("), 0);
  OUTEXPRESSION(EXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "), 0);
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTELSE() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "else "), 0);
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
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, "switch ("), 0);
  OUTEXPRESSION(CASEINDEX);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "), 0);
  OUTBEGIN();
}

void OUTCASESTATEMENTBEGIN(TEXPRESSIONOBJ *CASELABEL) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(5, "case "), 0);
  OUTEXPRESSION(CASELABEL);
  WRITE_c(&CODEGEN.OUTPUT, ':', 0);
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASESTATEMENTEND() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(6, "break;"), 0);
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEELSEBEGIN() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, "default:"), 0);
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASEELSEEND() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(6, "break;"), 0);
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEEND() {
  OUTEND();
}

void OUTREPEATBEGIN() {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "do "), 0);
  OUTBEGIN();
}

void OUTREPEATEND(TEXPRESSIONOBJ *EXPR) {
  TEXPRESSIONOBJ *TMPEXPR;
  OUTENDSAMELINE();
  WRITE_s(&CODEGEN.OUTPUT, str_make(8, " while ("), 0);
  TMPEXPR = EXUNARYOP(COPYEXPR(EXPR), TKNOT);
  OUTEXPRESSION(TMPEXPR);
  DISPOSEEXPR(&TMPEXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ");"), 0);
  _OUTNEWLINE();
}

void OUTWHILEBEGIN(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, "while ("), 0);
  OUTEXPRESSION(EXPR);
  WRITE_s(&CODEGEN.OUTPUT, str_make(2, ") "), 0);
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTWHILEEND() {
  CODEGEN.ISMULTISTATEMENT = 1;
}

void OUTFORBEGIN(TEXPRESSIONOBJ *ITER, TEXPRESSIONOBJ *FIRSTEXPR, TEXPRESSIONOBJ *LASTEXPR, PBoolean ASCENDING) {
  TPSTYPE *LIMITTYPE;
  TPSVARIABLE FIRST;
  TPSVARIABLE LAST;
  LIMITTYPE = ITER->TYPEPTR;
  if (ISENUMTYPE(LIMITTYPE)) LIMITTYPE = PRIMITIVETYPES.PTINTEGER;
  FIRST = MAKEVARIABLE(str_make(5, "first"), LIMITTYPE);
  LAST = MAKEVARIABLE(str_make(4, "last"), LIMITTYPE);
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, "do "), 0);
  OUTBEGIN();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(FIRST);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "), 0);
  OUTEXPRESSION(FIRSTEXPR);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(LAST);
  WRITE_s(&CODEGEN.OUTPUT, str_make(3, " = "), 0);
  OUTEXPRESSION(LASTEXPR);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(10, "if (first "), 0);
  if (ASCENDING) WRITE_s(&CODEGEN.OUTPUT, str_make(2, "<="), 0);
  else WRITE_s(&CODEGEN.OUTPUT, str_make(2, ">="), 0);
  WRITE_s(&CODEGEN.OUTPUT, str_make(7, " last) "), 0);
  OUTBEGIN();
  _OUTINDENT();
  OUTEXPRESSION(ITER);
  WRITE_s(&CODEGEN.OUTPUT, str_make(9, " = first;"), 0);
  _OUTNEWLINE();
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(10, "while (1) "), 0);
  OUTBEGIN();
}

void OUTFOREND(TEXPRESSIONOBJ *ITER, PBoolean ASCENDING) {
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(4, "if ("), 0);
  OUTEXPRESSION(ITER);
  WRITE_s(&CODEGEN.OUTPUT, str_make(16, " == last) break;"), 0);
  _OUTNEWLINE();
  _OUTINDENT();
  if (ASCENDING) WRITE_s(&CODEGEN.OUTPUT, str_make(2, "++"), 0);
  else WRITE_s(&CODEGEN.OUTPUT, str_make(2, "--"), 0);
  OUTEXPRESSION(ITER);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
  OUTEND();
  OUTEND();
  OUTENDSAMELINE();
  WRITE_s(&CODEGEN.OUTPUT, str_make(10, " while(0);"), 0);
  _OUTNEWLINE();
}

void OUTPROCEDURECALL(TEXPRESSIONOBJ *EXPR) {
  _OUTINDENT();
  OUTEXPRESSION(EXPR);
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTPSEUDOPROCCALL(TEXPRESSIONOBJ *EXPR) {
  _OUTEXPSEUDOFNCALL(EXPR);
}

void OUTEMPTYSTATEMENT() {
  _OUTINDENT();
  WRITE_c(&CODEGEN.OUTPUT, ';', 0);
  _OUTNEWLINE();
}

void OUTPROGRAMBEGIN() {
  _OUTBLANKLINE(TOTFUNDEF);
  _OUTINDENT();
  WRITE_s(&CODEGEN.OUTPUT, str_make(20, "void pascual_main() "), 0);
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
  ASSIGN(&CODEGEN.OUTPUT, &FILENAME);
  REWRITE(&CODEGEN.OUTPUT);
}

TEXPRESSIONOBJ *_PF_UNARY_PARSE() {
  TEXPRESSIONOBJ *RESULT;
  WANTTOKENANDREAD(TKLPAREN);
  RESULT = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  return RESULT;
}

PString _PF_FUN_OVERLOAD(PString NAMEPREFIX, TPSTYPE *TYPEPTR) {
  PString RESULT;
  TYPEPTR = GETFUNDAMENTALTYPE(TYPEPTR);
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = cat_ss(NAMEPREFIX, str_make(2, "_b"));
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = cat_ss(NAMEPREFIX, str_make(2, "_i"));
  else if (ISREALTYPE(TYPEPTR)) RESULT = cat_ss(NAMEPREFIX, str_make(2, "_r"));
  else if (ISCHARTYPE(TYPEPTR)) RESULT = cat_ss(NAMEPREFIX, str_make(2, "_c"));
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = cat_ss(NAMEPREFIX, str_make(2, "_s"));
  else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(str_make(5, "Type "), TYPENAME(TYPEPTR)), str_make(26, " is not representable for ")), str_make(12, "overload of ")), NAMEPREFIX));
  return RESULT;
}

TEXPRESSIONOBJ *_PF_OVERLOAD_PARSE(TEXPRESSIONOBJ *FNEXPR, PString NAMEPREFIX) {
  TEXPRESSIONOBJ *RESULT;
  TEXPRESSIONOBJ *ARG;
  TEXFUNCTIONARGS ARGS;
  TPSFUNCTION *FNPTR;
  DISPOSEEXPR(&FNEXPR);
  ARG = (void*)0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    WANTTOKENANDREAD(TKLPAREN);
    if (LEXER.TOKEN.ID != TKRPAREN) ARG = PSEXPRESSION();
    WANTTOKENANDREAD(TKRPAREN);
  }
  if (ARG == (void*)0) {
    FNPTR = FINDNAMEOFCLASS(cat_ss(NAMEPREFIX, str_make(2, "_n")), TNCFUNCTION, 1)->FNPTR;
    ARGS.SIZE = 0;
    RESULT = EXFUNCTIONCALL(EXFNREF(FNPTR), &ARGS);
  }
  else {
    FNPTR = FINDNAMEOFCLASS(_PF_FUN_OVERLOAD(NAMEPREFIX, ARG->TYPEPTR), TNCFUNCTION, 1)->FNPTR;
    ARGS.SIZE = 1;
    ARGS.VALUES[0] = ARG;
    RESULT = EXFUNCTIONCALL(EXFNREF(FNPTR), &ARGS);
  }
  return RESULT;
}

TEXPRESSIONOBJ *PFABS_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _PF_OVERLOAD_PARSE(FNEXPR, str_make(3, "ABS"));
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
  PBoolean OUTOFBOUNDS;
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

TEXPRESSIONOBJ *PFRANDOM_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _PF_OVERLOAD_PARSE(FNEXPR, str_make(6, "RANDOM"));
  return RESULT;
}

TEXPRESSIONOBJ *PFREAD_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  PBoolean FIRST;
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
        if (!OUTVAR->ISASSIGNABLE) COMPILEERROR(str_make(25, "Invalid argument for READ"));
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

TEXPRESSIONOBJ *PFSQR_PARSE(TEXPRESSIONOBJ *FNEXPR) {
  TEXPRESSIONOBJ *RESULT;
  RESULT = _PF_OVERLOAD_PARSE(FNEXPR, str_make(3, "SQR"));
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
  PBoolean OUTOFBOUNDS;
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
  PBoolean FIRST;
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
    case TPFABS:
      RESULT = PFABS_PARSE(FN);
      break;
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
    case TPFRANDOM:
      RESULT = PFRANDOM_PARSE(FN);
      break;
    case TPFREAD:
      RESULT = PFREAD_PARSE(FN);
      break;
    case TPFREADLN:
      RESULT = PFREAD_PARSE(FN);
      break;
    case TPFSQR:
      RESULT = PFSQR_PARSE(FN);
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
      INTERNALERROR(cat_ss(str_make(31, "Unimplemented special function "), DESCRIBEEXPR(FN, 5)));
      break;
  }
  return RESULT;
}

PString PF_DESCRIBENAME(TEXPRESSIONOBJ *FN) {
  PString RESULT;
  switch (FN->PSEUDOFN) {
    case TPFABS:
      RESULT = str_make(3, "ABS");
      break;
    case TPFCONCAT:
      RESULT = str_make(6, "CONCAT");
      break;
    case TPFDISPOSE:
      RESULT = str_make(7, "DISPOSE");
      break;
    case TPFNEW:
      RESULT = str_make(3, "NEW");
      break;
    case TPFORD:
      RESULT = str_make(3, "ORD");
      break;
    case TPFPRED:
      RESULT = str_make(4, "PRED");
      break;
    case TPFRANDOM:
      RESULT = str_make(6, "RANDOM");
      break;
    case TPFREAD:
      RESULT = str_make(4, "READ");
      break;
    case TPFREADLN:
      RESULT = str_make(6, "READLN");
      break;
    case TPFSQR:
      RESULT = str_make(3, "SQR");
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
      INTERNALERROR(str_make(25, "Cannot describe pseudofun"));
      break;
  }
  return RESULT;
}

PString PF_DESCRIBECALL(TEXPRESSIONOBJ *EXPR, PInteger LEVELS) {
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
      case TPFORD:
        RESULT = cat_sc(cat_ss(str_make(4, "ORD("), DESCRIBEEXPR(with1->ARG1, LEVELS - 1)), ')');
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
        INTERNALERROR(str_make(25, "Cannot describe pseudofun"));
        break;
    }
  }
  return RESULT;
}

void COMPILEERROR(PString MSG) {
  WRITE_s(&STDERR, MSG, 0);
  WRITE_s(&STDERR, LXWHERESTR(), 0);
  WRITELN(&STDERR);
  HALT(1);
}

void INTERNALERROR(PString MSG) {
  WRITE_s(&STDERR, str_make(17, "Internal error : "), 0);
  WRITE_s(&STDERR, MSG, 0);
  WRITE_s(&STDERR, LXWHERESTR(), 0);
  WRITELN(&STDERR);
  HALT(1);
}

void COMPILEWARNING(PString MSG) {
  if (!OPTIONS.SUPPRESSWARNINGS) {
    WRITE_s(&STDERR, str_make(9, "Warning: "), 0);
    WRITE_s(&STDERR, MSG, 0);
    WRITE_s(&STDERR, LXWHERESTR(), 0);
    WRITELN(&STDERR);
  }
}

void USAGE(PString MSG) {
  if (cmp_ss(MSG, str_make(0, "")) != 0) {
    WRITE_s(&OUTPUT, MSG, 0);
    WRITELN(&OUTPUT);
    WRITELN(&OUTPUT);
  }
  WRITE_s(&OUTPUT, str_make(6, "Usage:"), 0);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, PARAMSTR(0), 0);
  WRITE_s(&OUTPUT, str_make(33, " input.pas [-o output.c] [-Wnone]"), 0);
  WRITELN(&OUTPUT);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(48, "If you specify \"-\" as the input or output file, "), 0);
  WRITE_s(&OUTPUT, str_make(26, "stdin/stdout will be used."), 0);
  WRITELN(&OUTPUT);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(8, "Options:"), 0);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(46, "   -Wnone    :- Suppress all warning messages."), 0);
  WRITELN(&OUTPUT);
  HALT(0);
}

PString REPLACEEXTENSION(PString STR, PString OLD, PString NEW) {
  PString RESULT;
  PInteger BASELEN;
  PInteger POS;
  PBoolean MATCHES;
  RESULT = str_make(0, "");
  BASELEN = LENGTH(&STR) - LENGTH(&OLD);
  if (BASELEN > 0) {
    MATCHES = 1;
    do {
      PInteger first = 1;
      PInteger last = LENGTH(&OLD);
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
  PInteger POS;
  PString INPUTFILE;
  PString OUTPUTFILE;
  PBoolean SUPPRESSWARNINGS;
  enum enum10 { FLAGNONE, FLAGOUTPUT } FLAG;
  PString PARAM;
  const char* enumvalues10[] = { "FLAGNONE", "FLAGOUTPUT" };
  INPUTFILE = str_make(0, "");
  OUTPUTFILE = str_make(0, "");
  SUPPRESSWARNINGS = 0;
  FLAG = FLAGNONE;
  do {
    PInteger first = 1;
    PInteger last = PARAMCOUNT();
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
  OPTIONS.CHECKBOUNDS = 1;
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
