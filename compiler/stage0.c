/* Program: PASCUAL */
#include "pascual.h"

struct record1 {
  PBoolean SUPPRESSWARNINGS;
  PBoolean CHECKBOUNDS;
  PBoolean CHECKIORESULT;
} OPTIONS;

void COMPILEERROR(PString MSG);
void INTERNALERROR(PString MSG);
void COMPILEWARNING(PString MSG);

typedef struct record15* TPSTYPEPTR;
typedef struct record16* TPSENUMPTR;
typedef struct record20* TPSVARPTR;
typedef struct record23* TPSFNPTR;
typedef struct record24* TPSPSEUDOFNPTR;
typedef struct record3* TEXSETIMMBOUNDS;
typedef struct record4* TEXSETEXPRBOUNDS;
typedef struct record11* TEXPRESSION;
typedef enum __attribute__((__packed__)) enum1 { XICNIL, XICBOOLEAN, XICINTEGER, XICREAL, XICCHAR, XICSTRING, XICENUM, XICSET } TEXIMMEDIATECLASS;
typedef struct record2 {
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
      struct record16* ENUMPTR;
    };
    struct {
      struct record3* SETBOUNDS;
      struct record15* SETOFTYPEPTR;
    };
  };
} TEXIMMEDIATE;
typedef struct record3 {
  PInteger FIRST;
  PInteger LAST;
  struct record3* NEXT;
} TEXSETIMMBOUNDSOBJ;
typedef struct record4 {
  struct record11* FIRST;
  struct record11* LAST;
  struct record4* NEXT;
} TEXSETEXPRBOUNDSOBJ;
typedef struct record5 {
  PInteger SIZE;
  struct record11* VALUES[16];
} TEXFUNCTIONARGS;
typedef struct record6 {
  struct record11* ARG;
  struct record6* NEXT;
} TEXREADARGS;
typedef struct record7 {
  struct record11* ARG;
  struct record11* WIDTH;
  struct record11* PREC;
  struct record7* NEXT;
} TEXWRITEARGS;
typedef struct record8 {
  struct record24* PSEUDOFNPTR;
  struct record11* ARG1;
  struct record11* ARG2;
  struct record11* ARG3;
  struct record11* ARG4;
  struct record15* TYPEARG;
  TEXREADARGS* READARGS;
  TEXWRITEARGS* WRITEARGS;
} TEXPSEUDOFNCALL;
typedef enum __attribute__((__packed__)) enum2 { XOADD, XOSUB, XOMUL, XODIVREAL, XODIVINT, XOMOD, XONEG, XOAND, XOOR, XOXOR, XONOT, XOSHL, XOSHR, XOIN, XOEQ, XONE, XOLT, XOGT, XOLTEQ, XOGTEQ, XOORD, XOPRED, XOSUCC } TEXOPERATOR;
typedef struct record9 {
  struct record11* PARENT;
  TEXOPERATOR OP;
} TEXUNARYOP;
typedef struct record10 {
  struct record11* LEFT;
  struct record11* RIGHT;
  TEXOPERATOR OP;
} TEXBINARYOP;
typedef enum __attribute__((__packed__)) enum3 { XCIMMEDIATE, XCTOSTRING, XCTOREAL, XCWITHTMPVAR, XCSUBRANGE, XCSET, XCVARIABLE, XCFIELD, XCARRAY, XCPOINTER, XCADDRESS, XCSTRINGCHAR, XCFNREF, XCFNCALL, XCPSEUDOFNREF, XCPSEUDOFNCALL, XCUNARYOP, XCBINARYOP } TEXPRESSIONCLASS;
typedef struct record11 {
  struct record15* TYPEPTR;
  PBoolean ISASSIGNABLE;
  PBoolean ISADDRESSABLE;
  PBoolean ISFUNCTIONRESULT;
  TEXPRESSIONCLASS CLS;
  union {
    struct {
      TEXIMMEDIATE IMMEDIATE;
    };
    struct {
      struct record11* TOSTRPARENT;
    };
    struct {
      struct record11* TOREALPARENT;
    };
    struct {
      struct record11* TMPVAR;
      struct record11* TMPVARVALUE;
      struct record11* TMPVARCHILD;
    };
    struct {
      struct record11* SUBRANGEPARENT;
    };
    struct {
      struct record11* SETBASE;
      TEXSETEXPRBOUNDSOBJ* SETBOUNDS;
    };
    struct {
      struct record20* VARPTR;
    };
    struct {
      struct record11* RECEXPR;
      PInteger RECFIELDNUM;
    };
    struct {
      struct record11* ARRAYEXPR;
      struct record11* ARRAYINDEX;
    };
    struct {
      struct record11* POINTEREXPR;
    };
    struct {
      struct record11* ADDRESSEXPR;
    };
    struct {
      struct record11* STRINGEXPR;
      struct record11* STRINGINDEX;
    };
    struct {
      struct record23* FNPTR;
    };
    struct {
      struct record11* FNEXPR;
      TEXFUNCTIONARGS CALLARGS;
    };
    struct {
      struct record24* PSEUDOFNPTR;
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
typedef struct record12 {
  PString NAME;
} TPSIDENTIFIER;
typedef struct record18* TPSRECPTR;
typedef struct record22* TPSFNDEFPTR;
typedef struct record19* TPSCONSTPTR;
typedef struct record25* TPSWITHVARPTR;
typedef struct record26* TPSNAMEPTR;
typedef enum __attribute__((__packed__)) enum4 { TTCBOOLEAN, TTCINTEGER, TTCREAL, TTCCHAR, TTCSTRING, TTCTEXT, TTCENUM, TTCRANGE, TTCSET, TTCRECORD, TTCARRAY, TTCPOINTER, TTCNIL, TTCPOINTERUNKNOWN, TTCFUNCTION } TPSTYPECLASS;
typedef struct record15 {
  PString NAME;
  struct record15* ALIASFOR;
  PBoolean WASUSED;
  TPSTYPECLASS CLS;
  union {
    struct {
      struct record16* ENUMPTR;
    };
    struct {
      struct record13 {
        PInteger FIRST;
        PInteger LAST;
        struct record15* BASETYPEPTR;
      } RANGEDEF;
    };
    struct {
      struct record15* ELEMENTTYPEPTR;
    };
    struct {
      struct record18* RECPTR;
    };
    struct {
      struct record14 {
        struct record15* INDEXTYPEPTR;
        struct record15* VALUETYPEPTR;
      } ARRAYDEF;
    };
    struct {
      struct record15* POINTEDTYPEPTR;
    };
    struct {
      PString* TARGETNAME;
    };
    struct {
      struct record22* FNDEFPTR;
    };
  };
} TPSTYPE;
typedef struct record16 {
  PInteger SIZE;
  PString VALUES[128];
  PInteger ID;
  PBoolean HASBEENDEFINED;
  PBoolean VALUESHAVEBEENOUTPUT;
  PInteger REFCOUNT;
} TPSENUMDEF;
typedef struct record17 {
  PString NAME;
  TPSTYPE* TYPEPTR;
} TPSRECORDFIELD;
typedef struct record18 {
  PInteger SIZE;
  TPSRECORDFIELD FIELDS[32];
  PInteger NUMVARIANTS;
  PInteger VARIANTBOUNDS[32];
  PBoolean ISPACKED;
  PInteger ID;
  PBoolean HASBEENDEFINED;
  PInteger REFCOUNT;
} TPSRECORDDEF;
typedef struct record19 {
  PString NAME;
  TEXPRESSIONOBJ* VALUE;
} TPSCONSTANT;
typedef struct record20 {
  PString NAME;
  TPSTYPE* TYPEPTR;
  PBoolean ISREFERENCE;
  PBoolean ISCONSTANT;
  PBoolean WASINITIALIZED;
  PBoolean WASUSED;
} TPSVARIABLE;
typedef struct record21 {
  PInteger COUNT;
  TPSVARIABLE DEFS[16];
} TPSFNARGS;
typedef struct record22 {
  TPSFNARGS ARGS;
  TPSTYPE* RETURNTYPEPTR;
  PInteger REFCOUNT;
} TPSFNDEF;
typedef struct record23 {
  PString NAME;
  PString EXTERNALNAME;
  TPSFNARGS ARGS;
  TPSTYPE* RETURNTYPEPTR;
  PBoolean ISDECLARATION;
  PBoolean WASUSED;
} TPSFUNCTION;
typedef TEXPRESSIONOBJ* (*TPSPSEUDOFNPARSER)(TEXPRESSIONOBJ* FNEXPR);
typedef PString (*TPSPSEUDOFNDESCRIPTOR)(TEXPRESSIONOBJ* FNEXPR);
typedef struct record24 {
  PString NAME;
  TPSPSEUDOFNPARSER PARSEFN;
  TPSPSEUDOFNDESCRIPTOR DESCRIBEFN;
} TPSPSEUDOFN;
typedef struct record25 {
  TPSVARIABLE* VARPTR;
} TPSWITHVAR;
typedef enum __attribute__((__packed__)) enum5 { TNCTYPE, TNCVARIABLE, TNCCONSTANT, TNCENUMVAL, TNCFUNCTION, TNCPSEUDOFN } TPSNAMECLASS;
typedef struct record26 {
  PString NAME;
  TPSNAMECLASS CLS;
  union {
    struct {
      TPSTYPE* TYPEPTR;
    };
    struct {
      TPSVARIABLE* VARPTR;
    };
    struct {
      TPSCONSTANT* CONSTPTR;
    };
    struct {
      TPSTYPE* ENUMTYPEPTR;
      PInteger ORDINAL;
    };
    struct {
      TPSFUNCTION* FNPTR;
    };
    struct {
      TPSPSEUDOFN* PSEUDOFNPTR;
    };
  };
} TPSNAME;
typedef enum __attribute__((__packed__)) enum6 { TCTENUM, TCTRECORD, TCTTMPVAR } TPSCOUNTERTYPE;
typedef struct record27 {
  PInteger ENUMCTR;
  PInteger RECORDCTR;
  PInteger TMPVARCTR;
} TPSCOUNTERS;
typedef struct record28* TPSDEFPTR;
typedef enum __attribute__((__packed__)) enum7 { TDCNAME, TDCTYPE, TDCCONSTANT, TDCVARIABLE, TDCFUNCTION, TDCPSEUDOFN, TDCWITHVAR, TDCSCOPEBOUNDARY } TPSDEFCLASS;
typedef struct record28 {
  struct record28* PREV;
  struct record28* NEXT;
  TPSDEFCLASS CLS;
  union {
    struct {
      TPSNAME* NAMEPTR;
    };
    struct {
      TPSTYPE* TYPEPTR;
    };
    struct {
      TPSCONSTANT* CONSTPTR;
    };
    struct {
      TPSVARIABLE* VARPTR;
    };
    struct {
      TPSFUNCTION* FNPTR;
    };
    struct {
      TPSPSEUDOFN* PSEUDOFNPTR;
    };
    struct {
      TPSWITHVAR* WITHVARPTR;
    };
    struct {
      PBoolean TEMPORARYSCOPE;
      TPSCOUNTERS COUNTERS;
      TPSFUNCTION* CURRENTFN;
    };
  };
} TPSDEFENTRY;
typedef struct record29 {
  TPSDEFENTRY* LATEST;
  TPSFUNCTION* CURRENTFN;
  TPSCOUNTERS COUNTERS;
} TPSDEFS;

const char* enumvalues1[] = { "XICNIL", "XICBOOLEAN", "XICINTEGER", "XICREAL", "XICCHAR", "XICSTRING", "XICENUM", "XICSET" };
const char* enumvalues2[] = { "XOADD", "XOSUB", "XOMUL", "XODIVREAL", "XODIVINT", "XOMOD", "XONEG", "XOAND", "XOOR", "XOXOR", "XONOT", "XOSHL", "XOSHR", "XOIN", "XOEQ", "XONE", "XOLT", "XOGT", "XOLTEQ", "XOGTEQ", "XOORD", "XOPRED", "XOSUCC" };
const char* enumvalues3[] = { "XCIMMEDIATE", "XCTOSTRING", "XCTOREAL", "XCWITHTMPVAR", "XCSUBRANGE", "XCSET", "XCVARIABLE", "XCFIELD", "XCARRAY", "XCPOINTER", "XCADDRESS", "XCSTRINGCHAR", "XCFNREF", "XCFNCALL", "XCPSEUDOFNREF", "XCPSEUDOFNCALL", "XCUNARYOP", "XCBINARYOP" };
const char* enumvalues4[] = { "TTCBOOLEAN", "TTCINTEGER", "TTCREAL", "TTCCHAR", "TTCSTRING", "TTCTEXT", "TTCENUM", "TTCRANGE", "TTCSET", "TTCRECORD", "TTCARRAY", "TTCPOINTER", "TTCNIL", "TTCPOINTERUNKNOWN", "TTCFUNCTION" };
const char* enumvalues5[] = { "TNCTYPE", "TNCVARIABLE", "TNCCONSTANT", "TNCENUMVAL", "TNCFUNCTION", "TNCPSEUDOFN" };
const char* enumvalues6[] = { "TCTENUM", "TCTRECORD", "TCTTMPVAR" };
const char* enumvalues7[] = { "TDCNAME", "TDCTYPE", "TDCCONSTANT", "TDCVARIABLE", "TDCFUNCTION", "TDCPSEUDOFN", "TDCWITHVAR", "TDCSCOPEBOUNDARY" };

void EXDISPOSE(TEXPRESSIONOBJ* *EXPR);
TEXPRESSIONOBJ* EXCOPY(TEXPRESSIONOBJ* EXPR);
PString EXDESCRIBEOPERATOR(TEXOPERATOR OP);
PString EXDESCRIBE(TEXPRESSIONOBJ* EXPR);
void EXMARKINITIALIZED(TEXPRESSIONOBJ* LHS);
PInteger EXGETORDINAL(TEXPRESSIONOBJ* EXPR);
TEXPRESSIONOBJ* EXGETANTIORDINAL(PInteger ORDINAL, TPSTYPE* TYPEPTR);
TEXPRESSIONOBJ* PFORD(TEXPRESSIONOBJ* ARG);
TEXPRESSIONOBJ* PFPRED(TEXPRESSIONOBJ* ARG);
TEXPRESSIONOBJ* PFSUCC(TEXPRESSIONOBJ* ARG);
PString TYPENAME(TPSTYPE* TYPEPTR);
PBoolean ISENUMTYPE(TPSTYPE* TYPEPTR);
PBoolean ISORDINALTYPE(TPSTYPE* TYPEPTR);
PBoolean ISRECORDTYPE(TPSTYPE* TYPEPTR);
PBoolean ISARRAYTYPE(TPSTYPE* TYPEPTR);
PBoolean ISPOINTERTYPE(TPSTYPE* TYPEPTR);
PBoolean ISSTRINGYTYPE(TPSTYPE* TYPEPTR);
PBoolean ISSTRINGTYPE(TPSTYPE* TYPEPTR);
PBoolean ISINTEGERTYPE(TPSTYPE* TYPEPTR);
PBoolean ISNUMERICTYPE(TPSTYPE* TYPEPTR);

PString ERRORDESCRIBEEXPR(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  RESULT = CONCAT(CpString, EXDESCRIBE(EXPR), CpLenPtr, 2, " (", CpString, TYPENAME(EXPR->TYPEPTR), CpEnd | CpChar, ')');
  return RESULT;
}

void ERRORINVALIDOPERATOR(TEXPRESSIONOBJ* EXPR, TEXOPERATOR OP) {
  COMPILEERROR(CONCAT(CpLenPtr, 10, "Operator '", CpString, EXDESCRIBEOPERATOR(OP), CpLenPtr, 29, "' is not valid in expression ", CpEnd | CpString, ERRORDESCRIBEEXPR(EXPR)));
}

void ERRORINVALIDOPERATOR2(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT, TEXOPERATOR OP) {
  COMPILEERROR(CONCAT(CpLenPtr, 18, "Invalid operator '", CpString, EXDESCRIBEOPERATOR(OP), CpLenPtr, 6, "' for ", CpString, ERRORDESCRIBEEXPR(LEFT), CpLenPtr, 5, " and ", CpEnd | CpString, ERRORDESCRIBEEXPR(RIGHT)));
}

void ERRORFORTYPE(const PString *MSG, TPSTYPE* GOT) {
  COMPILEERROR(CONCAT(CpStringPtr, MSG, CpLenPtr, 6, "; got ", CpEnd | CpString, TYPENAME(GOT)));
}

void ERRORFOREXPR(const PString *MSG, TEXPRESSIONOBJ* GOT) {
  COMPILEERROR(CONCAT(CpStringPtr, MSG, CpLenPtr, 6, "; got ", CpEnd | CpString, ERRORDESCRIBEEXPR(GOT)));
}

void ENSURERECORDTYPE(TPSTYPE* TYP) {
  if (!ISRECORDTYPE(TYP)) {
    PString tmp1 = str_make(17, "Expected a record");
    ERRORFORTYPE(&tmp1, TYP);
  }
}

void ENSUREENUMTYPE(TPSTYPE* TYP) {
  if (!ISENUMTYPE(TYP)) {
    PString tmp1 = str_make(23, "Expected an enumeration");
    ERRORFORTYPE(&tmp1, TYP);
  }
}

void ENSUREORDINALTYPE(TPSTYPE* TYP) {
  if (!ISORDINALTYPE(TYP)) {
    PString tmp1 = str_make(19, "Expected an ordinal");
    ERRORFORTYPE(&tmp1, TYP);
  }
}

void ENSURERECORDEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISRECORDTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(17, "Expected a record");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREARRAYEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISARRAYTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(17, "Expected an array");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREPOINTEREXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISPOINTERTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(18, "Expected a pointer");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURESTRINGYEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISSTRINGYTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(17, "Expected a string");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURESTRINGEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISSTRINGTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(17, "Expected a string");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURENUMERICEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISNUMERICTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(29, "Expected a numeric expression");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREINTEGEREXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISINTEGERTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(19, "Expected an integer");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREORDINALEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!ISORDINALTYPE(EXPR->TYPEPTR)) {
    PString tmp1 = str_make(19, "Expected an ordinal");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREASSIGNABLEEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!EXPR->ISASSIGNABLE) {
    PString tmp1 = str_make(19, "Expected a variable");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREADDRESSABLEEXPR(TEXPRESSIONOBJ* EXPR) {
  if (!EXPR->ISADDRESSABLE) {
    PString tmp1 = str_make(29, "Expected an addressable value");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

typedef enum __attribute__((__packed__)) enum8 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKINTEGER, TKREAL, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAT, TKAND, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKSHL, TKSHR, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH, TKXOR } TLXTOKENID;
typedef struct record30 {
  PInteger ROW;
  PInteger COL;
} TLXPOS;
typedef struct record31 {
  TLXTOKENID ID;
  PString VALUE;
  TLXPOS POS;
} TLXTOKEN;
typedef struct record32 {
  PFile SRC;
  PString NAME;
  TLXPOS POS;
} TLXINPUTFILE;
typedef struct record33* TLXINCLUDESTACK;
typedef struct record33 {
  TLXINPUTFILE INPUT;
  struct record33* PREV;
} TLXINCLUDESTACKELEM;

const char* enumvalues8[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKINTEGER", "TKREAL", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAT", "TKAND", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKSHL", "TKSHR", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH", "TKXOR" };

struct record34 {
  PString LINE;
  TLXTOKEN TOKEN;
  TLXINPUTFILE INPUT;
  TLXINCLUDESTACKELEM* INCLUDESTACK;
} LEXER;

PString LXTOKENNAME(TLXTOKENID ID) {
  PString RESULT;
  PString NAME;
  STR_e(ID, enumvalues8, 0, &NAME);
  RESULT = NAME;
  return RESULT;
}

PString LXPOSSTR(TLXPOS POS) {
  PString RESULT;
  PString ROW;
  PString COL;
  STR_i(POS.ROW, 0, &ROW);
  STR_i(POS.COL, 0, &COL);
  RESULT = CONCAT(CpLenPtr, 4, "row ", CpStringPtr, &ROW, CpLenPtr, 5, " col ", CpStringPtr, &COL, CpLenPtr, 4, " in ", CpEnd | CpStringPtr, &LEXER.INPUT.NAME);
  return RESULT;
}

PString LXWHERESTR() {
  PString RESULT;
  RESULT = CONCAT(CpLenPtr, 6, " near ", CpEnd | CpString, LXPOSSTR(LEXER.TOKEN.POS));
  return RESULT;
}

PString LXTOKENSTR() {
  PString RESULT;
  RESULT = CONCAT(CpString, LXTOKENNAME(LEXER.TOKEN.ID), CpLenPtr, 2, " [", CpStringPtr, &LEXER.TOKEN.VALUE, CpEnd | CpChar, ']');
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
    while (LENGTH(&LEXER.LINE) == 0 && !Eof(&LEXER.INPUT.SRC, 1)) {
      LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
      LEXER.INPUT.POS.COL = 1;
      Read(&LEXER.INPUT.SRC, 1, RwpString | RwpLn | RwpEnd, &LEXER.LINE);
    }
    while (LENGTH(&LEXER.LINE) > 0 && LEXER.LINE.chr[1] == ' ') {
      LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + 1;
      DELETE(&LEXER.LINE, 1, 1);
    }
  } while (!Eof(&LEXER.INPUT.SRC, 1) && LENGTH(&LEXER.LINE) <= 0);
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
  const PString KEYWORDS[39] = { str_make(3, "AND"), str_make(5, "ARRAY"), str_make(5, "BEGIN"), str_make(4, "CASE"), str_make(5, "CONST"), str_make(3, "DIV"), str_make(2, "DO"), str_make(6, "DOWNTO"), str_make(4, "ELSE"), str_make(3, "END"), str_make(4, "FILE"), str_make(3, "FOR"), str_make(7, "FORWARD"), str_make(8, "FUNCTION"), str_make(4, "GOTO"), str_make(2, "IF"), str_make(2, "IN"), str_make(5, "LABEL"), str_make(3, "MOD"), str_make(3, "NIL"), str_make(3, "NOT"), str_make(2, "OF"), str_make(2, "OR"), str_make(6, "PACKED"), str_make(9, "PROCEDURE"), str_make(7, "PROGRAM"), str_make(6, "RECORD"), str_make(6, "REPEAT"), str_make(3, "SET"), str_make(3, "SHL"), str_make(3, "SHR"), str_make(4, "THEN"), str_make(2, "TO"), str_make(4, "TYPE"), str_make(5, "UNTIL"), str_make(3, "VAR"), str_make(5, "WHILE"), str_make(4, "WITH"), str_make(3, "XOR") };
  const TLXTOKENID TOKENS[39] = { TKAND, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKSHL, TKSHR, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH, TKXOR };
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
  POS = 1;
  while (POS <= 39 && LEXER.TOKEN.ID == TKIDENTIFIER) {
    if (cmp_str(CoEq, CpStringPtr, &LEXER.TOKEN.VALUE, CpStringPtr, &KEYWORDS[subrange(POS, 1, 39) - 1])) LEXER.TOKEN.ID = TOKENS[subrange(POS, 1, 39) - 1];
    POS = POS + 1;
  }
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
  enum __attribute__((__packed__)) enum9 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, CARET, DONE } STATE;
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
  } while (STATE != DONE);
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
    while (cmp_str(CoEq, CpStringPtr, &LEXER.LINE, CpLenPtr, 0, "")) {
      COMMENT = CONCAT(CpStringPtr, &COMMENT, CpEnd | CpChar, ' ');
      Read(&LEXER.INPUT.SRC, 1, RwpString | RwpLn | RwpEnd, &LEXER.LINE);
      LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
      LEXER.INPUT.POS.COL = 1;
    }
    if (DELIMITERLENGTH == 1) DONE = LEXER.LINE.chr[1] == '}';
    else DONE = LEXER.LINE.chr[1] == '*' && LEXER.LINE.chr[2] == ')';
    if (!DONE) {
      COMMENT = CONCAT(CpStringPtr, &COMMENT, CpEnd | CpChar, LEXER.LINE.chr[1]);
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
    if (LENGTH(&LEXER.LINE) >= 2) PFX = CONCAT(CpChar, LEXER.LINE.chr[1], CpEnd | CpChar, LEXER.LINE.chr[2]);
    else PFX = str_make(0, "");
    if (cmp_str(CoEq, CpStringPtr, &PFX, CpLenPtr, 2, "<>")) LXGETSYMBOL(TKNOTEQUALS, 2);
    else if (cmp_str(CoEq, CpStringPtr, &PFX, CpLenPtr, 2, "<=")) LXGETSYMBOL(TKLESSOREQUALS, 2);
    else if (cmp_str(CoEq, CpStringPtr, &PFX, CpLenPtr, 2, ">=")) LXGETSYMBOL(TKMOREOREQUALS, 2);
    else if (cmp_str(CoEq, CpStringPtr, &PFX, CpLenPtr, 2, ":=")) LXGETSYMBOL(TKASSIGN, 2);
    else if (cmp_str(CoEq, CpStringPtr, &PFX, CpLenPtr, 2, "..")) LXGETSYMBOL(TKRANGE, 2);
    else if (cmp_str(CoEq, CpStringPtr, &PFX, CpLenPtr, 2, "(*")) LXGETCOMMENT();
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
      case '@':
        LXGETSYMBOL(TKAT, 1);
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
        COMPILEERROR(CONCAT(CpLenPtr, 17, "Could not parse [", CpStringPtr, &LEXER.LINE, CpEnd | CpChar, ']'));
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
  LEXER.INCLUDESTACK = PNil;
}

PString _LXRESOLVEFILENAME(const PString *CURRENT, const PString *NEW) {
  PString RESULT;
  PInteger SLASH;
  if (cmp_str(CoEq, CpStringPtr, CURRENT, CpChar, '-') || NEW->chr[1] == '/') RESULT = *NEW;
  else {
    SLASH = LENGTH(CURRENT);
    while (SLASH > 0 && CURRENT->chr[SLASH] != '/') SLASH = SLASH - 1;
    if (SLASH == 0) RESULT = *NEW;
    else RESULT = CONCAT(CpString, COPY(CURRENT, 1, SLASH), CpEnd | CpStringPtr, NEW);
  }
  return RESULT;
}

void LXOPEN(PString FILENAME) {
  LEXER.INPUT.NAME = FILENAME;
  Assign(&LEXER.INPUT.SRC, &LEXER.INPUT.NAME, 1);
  Reset(&LEXER.INPUT.SRC, 0, 1);
}

void LXINCLUDE(const PString *FILENAME) {
  TLXINCLUDESTACKELEM* NEWSTACK;
  NEWSTACK = malloc(sizeof(TLXINCLUDESTACKELEM));
  NEWSTACK->INPUT = LEXER.INPUT;
  NEWSTACK->PREV = LEXER.INCLUDESTACK;
  LEXER.INCLUDESTACK = NEWSTACK;
  LEXER.INPUT.POS.ROW = 0;
  LEXER.INPUT.POS.COL = 0;
  LXOPEN(_LXRESOLVEFILENAME(&LEXER.INPUT.NAME, FILENAME));
}
TPSDEFS DEFS;
struct record35 {
  TPSTYPE* PTNIL;
  TPSTYPE* PTBOOLEAN;
  TPSTYPE* PTINTEGER;
  TPSTYPE* PTREAL;
  TPSTYPE* PTCHAR;
  TPSTYPE* PTSTRING;
  TPSTYPE* PTTEXT;
  TPSTYPE* PTEMPTYSET;
} PRIMITIVETYPES;
struct record36 {
  TPSPSEUDOFN* DISPOSE;
  TPSPSEUDOFN* NEW;
  TPSPSEUDOFN* ORD;
  TPSPSEUDOFN* PRED;
  TPSPSEUDOFN* READ;
  TPSPSEUDOFN* READLN;
  TPSPSEUDOFN* SIZEOF;
  TPSPSEUDOFN* STR;
  TPSPSEUDOFN* SUCC;
  TPSPSEUDOFN* VAL;
  TPSPSEUDOFN* WRITE;
  TPSPSEUDOFN* WRITELN;
} PSEUDOFUNS;

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
  DEFS.LATEST = PNil;
  DEFS.COUNTERS.ENUMCTR = 0;
  DEFS.COUNTERS.RECORDCTR = 0;
  DEFS.COUNTERS.TMPVARCTR = 0;
  DEFS.CURRENTFN = PNil;
}

TPSENUMDEF* NEWENUM(const TPSENUMDEF *ENUM) {
  TPSENUMDEF* RESULT;
  RESULT = malloc(sizeof(TPSENUMDEF));
  *RESULT = *ENUM;
  RESULT->ID = DEFCOUNTER(TCTENUM);
  RESULT->REFCOUNT = 1;
  RESULT->HASBEENDEFINED = 0;
  RESULT->VALUESHAVEBEENOUTPUT = 0;
  return RESULT;
}

void DISPOSEENUM(TPSENUMDEF* PTR) {
  PTR->REFCOUNT = PTR->REFCOUNT - 1;
  if (PTR->REFCOUNT == 0) free(PTR);
}

TPSRECORDDEF* NEWRECORD(const TPSRECORDDEF *REC) {
  TPSRECORDDEF* RESULT;
  RESULT = malloc(sizeof(TPSRECORDDEF));
  *RESULT = *REC;
  RESULT->ID = DEFCOUNTER(TCTRECORD);
  RESULT->REFCOUNT = 1;
  RESULT->HASBEENDEFINED = 0;
  return RESULT;
}

void DISPOSERECORD(TPSRECORDDEF* PTR) {
  PTR->REFCOUNT = PTR->REFCOUNT - 1;
  if (PTR->REFCOUNT == 0) free(PTR);
}

TPSFNDEF* NEWFNDEF() {
  TPSFNDEF* RESULT;
  RESULT = malloc(sizeof(TPSFNDEF));
  RESULT->REFCOUNT = 1;
  return RESULT;
}

void DISPOSEFNDEF(TPSFNDEF* PTR) {
  PTR->REFCOUNT = PTR->REFCOUNT - 1;
  if (PTR->REFCOUNT == 0) free(PTR);
}

TPSDEFENTRY* _NEWDEF(TPSDEFCLASS CLS) {
  TPSDEFENTRY* RESULT;
  TPSDEFENTRY* DEF;
  DEF = malloc(sizeof(TPSDEFENTRY));
  DEF->PREV = PNil;
  DEF->NEXT = PNil;
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
    case TDCPSEUDOFN:
      DEF->PSEUDOFNPTR = malloc(sizeof(TPSPSEUDOFN));
      break;
    case TDCWITHVAR:
      DEF->WITHVARPTR = malloc(sizeof(TPSWITHVAR));
      break;
    case TDCSCOPEBOUNDARY:
      {
        DEF->TEMPORARYSCOPE = 0;
        DEF->CURRENTFN = PNil;
      }
      break;
    default:
      break;
  }
  RESULT = DEF;
  return RESULT;
}

void _DISPOSETYPE(TPSTYPE* *TYPEPTR) {
  if ((*TYPEPTR)->CLS == TTCENUM) DISPOSEENUM((*TYPEPTR)->ENUMPTR);
  else if ((*TYPEPTR)->CLS == TTCRECORD) DISPOSERECORD((*TYPEPTR)->RECPTR);
  else if ((*TYPEPTR)->CLS == TTCFUNCTION) DISPOSEFNDEF((*TYPEPTR)->FNDEFPTR);
  free(*TYPEPTR);
}

void _DISPOSEDEF(TPSDEFENTRY* DEF) {
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
    case TDCPSEUDOFN:
      free(DEF->PSEUDOFNPTR);
      break;
    case TDCWITHVAR:
      free(DEF->WITHVARPTR);
      break;
    default:
      break;
  }
  free(DEF);
}

void _CHECKUNUSEDSYMBOLS(TPSDEFENTRY* DEF) {
  PString WHERE;
  if (DEFS.CURRENTFN == PNil) WHERE = str_make(0, "");
  else if (DEFS.CURRENTFN->RETURNTYPEPTR == PNil) WHERE = CONCAT(CpLenPtr, 14, " in procedure ", CpEnd | CpStringPtr, &DEFS.CURRENTFN->NAME);
  else WHERE = CONCAT(CpLenPtr, 13, " in function ", CpEnd | CpStringPtr, &DEFS.CURRENTFN->NAME);
  switch (DEF->CLS) {
    case TDCVARIABLE:
      if (!DEF->VARPTR->WASUSED) {
        if (DEF->VARPTR->ISCONSTANT) COMPILEWARNING(CONCAT(CpLenPtr, 9, "Constant ", CpStringPtr, &DEF->VARPTR->NAME, CpLenPtr, 13, " was not used", CpEnd | CpStringPtr, &WHERE));
        else COMPILEWARNING(CONCAT(CpLenPtr, 9, "Variable ", CpStringPtr, &DEF->VARPTR->NAME, CpLenPtr, 13, " was not used", CpEnd | CpStringPtr, &WHERE));
      }
      else if (!DEF->VARPTR->WASINITIALIZED) COMPILEWARNING(CONCAT(CpLenPtr, 9, "Variable ", CpStringPtr, &DEF->VARPTR->NAME, CpLenPtr, 20, " was not initialized", CpEnd | CpStringPtr, &WHERE));
      break;
    case TDCFUNCTION:
      if (!DEF->FNPTR->WASUSED) {
        if (DEF->FNPTR->RETURNTYPEPTR == PNil) COMPILEWARNING(CONCAT(CpLenPtr, 10, "Procedure ", CpStringPtr, &DEF->FNPTR->NAME, CpEnd | CpLenPtr, 13, " was not used"));
        else COMPILEWARNING(CONCAT(CpLenPtr, 9, "Function ", CpStringPtr, &DEF->FNPTR->NAME, CpEnd | CpLenPtr, 13, " was not used"));
      }
      break;
    case TDCTYPE:
      if (cmp_str(CoNotEq, CpStringPtr, &DEF->TYPEPTR->NAME, CpLenPtr, 0, "") && !DEF->TYPEPTR->WASUSED) COMPILEWARNING(CONCAT(CpLenPtr, 5, "Type ", CpString, TYPENAME(DEF->TYPEPTR), CpEnd | CpLenPtr, 13, " was not used"));
      break;
    default:
      break;
  }
}

TPSDEFENTRY* _ADDDEF(TPSDEFCLASS CLS) {
  TPSDEFENTRY* RESULT;
  TPSDEFENTRY* DEF;
  DEF = _NEWDEF(CLS);
  DEF->PREV = DEFS.LATEST;
  if (DEFS.LATEST != PNil) DEFS.LATEST->NEXT = DEF;
  DEFS.LATEST = DEF;
  RESULT = DEF;
  return RESULT;
}

PBoolean _DELETEDEF(TPSDEFENTRY *DELETEDDEF) {
  PBoolean RESULT;
  if (DEFS.LATEST == PNil) RESULT = 0;
  else {
    _CHECKUNUSEDSYMBOLS(DEFS.LATEST);
    *DELETEDDEF = *DEFS.LATEST;
    _DISPOSEDEF(DEFS.LATEST);
    DEFS.LATEST = DELETEDDEF->PREV;
    RESULT = 1;
  }
  return RESULT;
}

void _STARTSCOPE(PBoolean TEMPORARY, TPSFUNCTION* NEWFUNCTION) {
  TPSDEFENTRY* DEF;
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
  } while (DELETED && (DELETEDDEF.CLS != TDCSCOPEBOUNDARY || !TEMPORARY && DELETEDDEF.TEMPORARYSCOPE));
  DEFS.CURRENTFN = DELETEDDEF.CURRENTFN;
  DEFS.COUNTERS = DELETEDDEF.COUNTERS;
}

void STARTLOCALSCOPE(TPSFUNCTION* NEWFUNCTION) {
  _STARTSCOPE(0, NEWFUNCTION);
}

void CLOSELOCALSCOPE() {
  _CLOSESCOPE(0);
}

void STARTTEMPORARYSCOPE() {
  _STARTSCOPE(1, PNil);
}

void CLOSETEMPORARYSCOPE() {
  _CLOSESCOPE(1);
}

TPSNAME* _FINDNAME(const PString *NAME, PBoolean REQUIRED, PBoolean FROMLOCALSCOPE) {
  TPSNAME* RESULT;
  TPSDEFENTRY* DEF;
  TPSNAME* RET;
  RET = PNil;
  DEF = DEFS.LATEST;
  while (RET == PNil && DEF != PNil && (!FROMLOCALSCOPE || DEF->CLS != TDCSCOPEBOUNDARY)) {
    if (DEF->CLS == TDCNAME && cmp_str(CoEq, CpStringPtr, NAME, CpStringPtr, &DEF->NAMEPTR->NAME)) RET = DEF->NAMEPTR;
    DEF = DEF->PREV;
  }
  if (REQUIRED && RET == PNil) COMPILEERROR(CONCAT(CpLenPtr, 20, "Unknown identifier: ", CpEnd | CpStringPtr, NAME));
  RESULT = RET;
  return RESULT;
}

TPSNAME* _CHECKNAMECLASS(TPSNAME* NAMEPTR, TPSNAMECLASS CLS) {
  TPSNAME* RESULT;
  if (NAMEPTR != PNil && NAMEPTR->CLS != CLS) switch (CLS) {
    case TNCTYPE:
      COMPILEERROR(CONCAT(CpLenPtr, 12, "Not a type: ", CpEnd | CpStringPtr, &NAMEPTR->NAME));
      break;
    case TNCVARIABLE:
      COMPILEERROR(CONCAT(CpLenPtr, 16, "Not a variable: ", CpEnd | CpStringPtr, &NAMEPTR->NAME));
      break;
    case TNCENUMVAL:
      COMPILEERROR(CONCAT(CpLenPtr, 26, "Not an enumeration value: ", CpEnd | CpStringPtr, &NAMEPTR->NAME));
      break;
    case TNCFUNCTION:
      COMPILEERROR(CONCAT(CpLenPtr, 29, "Not a procedure or function: ", CpEnd | CpStringPtr, &NAMEPTR->NAME));
      break;
    case TNCPSEUDOFN:
      COMPILEERROR(CONCAT(CpLenPtr, 29, "Not a procedure or function: ", CpEnd | CpStringPtr, &NAMEPTR->NAME));
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 24, "Name class mismatch for ", CpEnd | CpStringPtr, &NAMEPTR->NAME));
      break;
  }
  RESULT = NAMEPTR;
  return RESULT;
}

TPSNAME* FINDNAMEINLOCALSCOPE(const PString *NAME, PBoolean REQUIRED) {
  TPSNAME* RESULT;
  RESULT = _FINDNAME(NAME, REQUIRED, 1);
  return RESULT;
}

TPSNAME* FINDNAMEOFCLASSINLOCALSCOPE(const PString *NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
  TPSNAME* RESULT;
  RESULT = _CHECKNAMECLASS(FINDNAMEINLOCALSCOPE(NAME, REQUIRED), CLS);
  return RESULT;
}

TPSNAME* FINDNAME(const PString *NAME, PBoolean REQUIRED) {
  TPSNAME* RESULT;
  RESULT = _FINDNAME(NAME, REQUIRED, 0);
  return RESULT;
}

TPSNAME* FINDNAMEOFCLASS(const PString *NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
  TPSNAME* RESULT;
  RESULT = _CHECKNAMECLASS(FINDNAME(NAME, REQUIRED), CLS);
  return RESULT;
}

TPSNAME* _ADDNAME(const PString *NAME, TPSNAMECLASS CLS) {
  TPSNAME* RESULT;
  TPSNAME* POS;
  if (FINDNAMEINLOCALSCOPE(NAME, 0) != PNil) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, NAME, CpEnd | CpLenPtr, 16, " already defined"));
  POS = _ADDDEF(TDCNAME)->NAMEPTR;
  POS->NAME = *NAME;
  POS->CLS = CLS;
  RESULT = POS;
  return RESULT;
}

TPSNAME* ADDTYPENAME(const PString *NAME, TPSTYPE* IDX) {
  TPSNAME* RESULT;
  RESULT = _ADDNAME(NAME, TNCTYPE);
  RESULT->TYPEPTR = IDX;
  return RESULT;
}

TPSNAME* ADDVARIABLENAME(const PString *NAME, TPSVARIABLE* IDX) {
  TPSNAME* RESULT;
  TPSNAME* DEF;
  DEF = _ADDNAME(NAME, TNCVARIABLE);
  DEF->VARPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME* ADDCONSTANTNAME(const PString *NAME, TPSCONSTANT* IDX) {
  TPSNAME* RESULT;
  TPSNAME* DEF;
  DEF = _ADDNAME(NAME, TNCCONSTANT);
  DEF->CONSTPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME* ADDFUNCTIONNAME(const PString *NAME, TPSFUNCTION* IDX) {
  TPSNAME* RESULT;
  TPSNAME* DEF;
  DEF = _ADDNAME(NAME, TNCFUNCTION);
  DEF->FNPTR = IDX;
  RESULT = DEF;
  return RESULT;
}

TPSNAME* ADDENUMVALNAME(PInteger ORDINAL, TPSTYPE* TYPEIDX) {
  TPSNAME* RESULT;
  TPSNAME* DEF;
  DEF = _ADDNAME(&TYPEIDX->ENUMPTR->VALUES[subrange(ORDINAL, 0, 127)], TNCENUMVAL);
  DEF->ENUMTYPEPTR = TYPEIDX;
  DEF->ORDINAL = ORDINAL;
  RESULT = DEF;
  return RESULT;
}

TPSPSEUDOFN* ADDPSEUDOFN(const PString *NAME, TPSPSEUDOFNPARSER PARSE, TPSPSEUDOFNDESCRIPTOR DESCRIBE) {
  TPSPSEUDOFN* RESULT;
  TPSNAME* DEF;
  DEF = _ADDNAME(NAME, TNCPSEUDOFN);
  DEF->PSEUDOFNPTR = _ADDDEF(TDCPSEUDOFN)->PSEUDOFNPTR;
  DEF->PSEUDOFNPTR->NAME = *NAME;
  DEF->PSEUDOFNPTR->PARSEFN = PARSE;
  DEF->PSEUDOFNPTR->DESCRIBEFN = DESCRIBE;
  RESULT = DEF->PSEUDOFNPTR;
  return RESULT;
}

TPSTYPE COPYTYPE(TPSTYPE* TYPEPTR) {
  TPSTYPE RESULT;
  RESULT = *TYPEPTR;
  if (RESULT.CLS == TTCPOINTERUNKNOWN) {
    RESULT.TARGETNAME = malloc(sizeof(PString));
    *RESULT.TARGETNAME = *TYPEPTR->TARGETNAME;
  }
  else if (RESULT.CLS == TTCENUM) RESULT.ENUMPTR->REFCOUNT = RESULT.ENUMPTR->REFCOUNT + 1;
  else if (RESULT.CLS == TTCRECORD) RESULT.RECPTR->REFCOUNT = RESULT.RECPTR->REFCOUNT + 1;
  else if (RESULT.CLS == TTCFUNCTION) RESULT.FNDEFPTR->REFCOUNT = RESULT.FNDEFPTR->REFCOUNT + 1;
  return RESULT;
}

TPSTYPE* GETFUNDAMENTALTYPE(TPSTYPE* TYPEPTR) {
  TPSTYPE* RESULT;
  if (TYPEPTR != PNil && TYPEPTR->CLS == TTCRANGE) RESULT = TYPEPTR->RANGEDEF.BASETYPEPTR;
  else RESULT = TYPEPTR;
  return RESULT;
}

PBoolean _TYPEHASCLASS(TPSTYPE* TYPEPTR, TPSTYPECLASS CLS) {
  PBoolean RESULT;
  RESULT = TYPEPTR != PNil && GETFUNDAMENTALTYPE(TYPEPTR)->CLS == CLS;
  return RESULT;
}

PBoolean ISINTEGERTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCINTEGER);
  return RESULT;
}

PBoolean ISREALTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCREAL);
  return RESULT;
}

PBoolean ISNUMERICTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISINTEGERTYPE(TYPEPTR) || ISREALTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISSTRINGTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCSTRING);
  return RESULT;
}

PBoolean ISCHARTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCCHAR);
  return RESULT;
}

PBoolean ISSTRINGYTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISSTRINGTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISBOOLEANTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCBOOLEAN);
  return RESULT;
}

PBoolean ISTEXTTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCTEXT);
  return RESULT;
}

PBoolean ISENUMTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCENUM);
  return RESULT;
}

PBoolean ISRANGETYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != PNil && TYPEPTR->CLS == TTCRANGE;
  return RESULT;
}

PBoolean ISSETTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCSET);
  return RESULT;
}

PBoolean ISRECORDTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCRECORD);
  return RESULT;
}

PBoolean ISARRAYTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCARRAY);
  return RESULT;
}

PBoolean ISPOINTERTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCPOINTER);
  return RESULT;
}

PBoolean ISNILTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCNIL);
  return RESULT;
}

PBoolean ISPOINTERYTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISPOINTERTYPE(TYPEPTR) || ISNILTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISPOINTERUNKNOWNTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCPOINTERUNKNOWN);
  return RESULT;
}

PBoolean ISFUNCTIONTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, TTCFUNCTION);
  return RESULT;
}

PBoolean ISFUNCTIONYTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISFUNCTIONTYPE(TYPEPTR) || ISNILTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISORDINALTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISINTEGERTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISBOUNDEDTYPE(TPSTYPE* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

PInteger GETTYPELOWBOUND(TPSTYPE* TYPEPTR) {
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
      {
        PString tmp1 = str_make(21, "Expected bounded type");
        ERRORFORTYPE(&tmp1, TYPEPTR);
      }
      break;
  }
  return RESULT;
}

PInteger GETTYPEHIGHBOUND(TPSTYPE* TYPEPTR) {
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
      {
        PString tmp1 = str_make(21, "Expected bounded type");
        ERRORFORTYPE(&tmp1, TYPEPTR);
      }
      break;
  }
  return RESULT;
}

PInteger GETBOUNDEDTYPESIZE(TPSTYPE* TYPEPTR) {
  PInteger RESULT;
  RESULT = GETTYPEHIGHBOUND(TYPEPTR) - GETTYPELOWBOUND(TYPEPTR) + 1;
  return RESULT;
}

PBoolean ISSAMETYPE(TPSTYPE* A, TPSTYPE* B) {
  PBoolean RESULT;
  if (A == PNil || B == PNil) RESULT = A == B;
  else {
    while (A->ALIASFOR != PNil) A = A->ALIASFOR;
    while (B->ALIASFOR != PNil) B = B->ALIASFOR;
    RESULT = A == B || ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE(A->POINTEDTYPEPTR, B->POINTEDTYPEPTR) || ISRANGETYPE(A) && ISRANGETYPE(B) && ISSAMETYPE(GETFUNDAMENTALTYPE(A), GETFUNDAMENTALTYPE(B)) && GETTYPELOWBOUND(A) == GETTYPELOWBOUND(B) && GETTYPEHIGHBOUND(A) == GETTYPEHIGHBOUND(B) || ISSETTYPE(A) && ISSETTYPE(B) && ISSAMETYPE(A->ELEMENTTYPEPTR, B->ELEMENTTYPEPTR);
  }
  return RESULT;
}

PBoolean ISFUNDAMENTALLYSAMETYPE(TPSTYPE* A, TPSTYPE* B) {
  PBoolean RESULT;
  RESULT = ISSAMETYPE(GETFUNDAMENTALTYPE(A), GETFUNDAMENTALTYPE(B));
  return RESULT;
}

PBoolean AREPOINTERSCOMPATIBLE(TPSTYPE* A, TPSTYPE* B) {
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
    RESULT = CONCAT(CpChar, '#', CpEnd | CpStringPtr, &CHNUM);
  }
  else RESULT = CONCAT(CpChar, '\'', CpChar, CHR, CpEnd | CpChar, '\'');
  return RESULT;
}

PString UNPARSESTRING(const PString *ST) {
  PString RESULT;
  PInteger POS;
  PString CHNUM;
  PBoolean QUOTED;
  QUOTED = 0;
  RESULT = str_make(0, "");
  do {
    PInteger first = 1;
    PInteger last = LENGTH(ST);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (ST->chr[POS] < ' ') {
            if (QUOTED) {
              QUOTED = 0;
              RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '\'');
            }
            STR_i((int)ST->chr[POS], 0, &CHNUM);
            RESULT = CONCAT(CpStringPtr, &RESULT, CpChar, '#', CpEnd | CpStringPtr, &CHNUM);
          }
          else {
            if (!QUOTED) {
              QUOTED = 1;
              RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '\'');
            }
            if (ST->chr[POS] == '\'') RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, "''");
            else RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ST->chr[POS]);
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  if (QUOTED) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '\'');
  if (cmp_str(CoEq, CpStringPtr, &RESULT, CpLenPtr, 0, "")) RESULT = str_make(2, "''");
  return RESULT;
}

PString _ANTIORDINAL(PInteger ORDINAL, TPSTYPE* TYPEPTR) {
  PString RESULT;
  RESULT = EXDESCRIBE(EXGETANTIORDINAL(ORDINAL, TYPEPTR));
  return RESULT;
}

PString DEEPTYPENAME(TPSTYPE* TYPEPTR, PBoolean USEORIGINAL) {
  PString RESULT;
  TPSTYPE TYP;
  PInteger POS;
  do {
    TYP = *TYPEPTR;
    TYPEPTR = TYP.ALIASFOR;
  } while (USEORIGINAL && TYPEPTR != PNil);
  if (cmp_str(CoNotEq, CpStringPtr, &TYP.NAME, CpLenPtr, 0, "")) RESULT = TYP.NAME;
  else if (TYP.CLS == TTCENUM) {
    RESULT = str_of('(');
    do {
      PInteger first = 0;
      PInteger last = TYP.ENUMPTR->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 0) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ',');
            RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpStringPtr, &TYP.ENUMPTR->VALUES[subrange(POS, 0, 127)]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  }
  else if (TYP.CLS == TTCRANGE) RESULT = CONCAT(CpString, _ANTIORDINAL(TYP.RANGEDEF.FIRST, TYP.RANGEDEF.BASETYPEPTR), CpLenPtr, 2, "..", CpEnd | CpString, _ANTIORDINAL(TYP.RANGEDEF.LAST, TYP.RANGEDEF.BASETYPEPTR));
  else if (TYP.CLS == TTCSET) {
    if (TYP.ELEMENTTYPEPTR == PNil) RESULT = str_make(9, "SET OF []");
    else RESULT = CONCAT(CpLenPtr, 7, "SET OF ", CpEnd | CpString, DEEPTYPENAME(TYP.ELEMENTTYPEPTR, 0));
  }
  else if (TYP.CLS == TTCRECORD) {
    RESULT = str_make(7, "RECORD ");
    do {
      PInteger first = 1;
      PInteger last = TYP.RECPTR->SIZE;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS != 1) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ',');
            RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, DEEPTYPENAME(TYP.RECPTR->FIELDS[subrange(POS, 1, 32) - 1].TYPEPTR, 1));
            RESULT = CONCAT(CpStringPtr, &RESULT, CpChar, ':', CpEnd | CpStringPtr, &TYP.RECPTR->FIELDS[subrange(POS, 1, 32) - 1].NAME);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 4, " END");
  }
  else if (TYP.CLS == TTCARRAY) RESULT = CONCAT(CpLenPtr, 7, "ARRAY [", CpString, DEEPTYPENAME(TYP.ARRAYDEF.INDEXTYPEPTR, 0), CpLenPtr, 5, "] OF ", CpEnd | CpString, DEEPTYPENAME(TYP.ARRAYDEF.VALUETYPEPTR, 0));
  else if (TYP.CLS == TTCPOINTER) RESULT = CONCAT(CpChar, '^', CpEnd | CpString, DEEPTYPENAME(TYP.POINTEDTYPEPTR, 1));
  else if (TYP.CLS == TTCFUNCTION) {
    TPSFNDEF *with1 = TYP.FNDEFPTR;
    {
      if (with1->RETURNTYPEPTR == PNil) RESULT = str_make(9, "PROCEDURE");
      else RESULT = str_make(8, "FUNCTION");
      if (with1->ARGS.COUNT > 0) {
        RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '(');
        do {
          PInteger first = 1;
          PInteger last = with1->ARGS.COUNT;
          if (first <= last) {
            POS = first;
            while (1) {
              {
                if (POS != 1) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, "; ");
                if (with1->ARGS.DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 6, "CONST ");
                else if (with1->ARGS.DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 4, "VAR ");
                RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpStringPtr, &with1->ARGS.DEFS[subrange(POS, 1, 16) - 1].NAME);
                RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 3, " : ", CpEnd | CpString, DEEPTYPENAME(with1->ARGS.DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, 0));
              }
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
        RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
      }
      if (with1->RETURNTYPEPTR != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 3, " : ", CpEnd | CpString, DEEPTYPENAME(with1->RETURNTYPEPTR, 0));
    }
  }
  else {
    STR_e(TYP.CLS, enumvalues4, 0, &RESULT);
    COMPILEERROR(CONCAT(CpLenPtr, 37, "Could not get name for type of class ", CpEnd | CpStringPtr, &RESULT));
  }
  return RESULT;
}

PString TYPENAME(TPSTYPE* TYPEPTR) {
  PString RESULT;
  if (TYPEPTR == PNil) RESULT = str_make(6, "(none)");
  else RESULT = DEEPTYPENAME(TYPEPTR, 0);
  return RESULT;
}

TPSCONSTANT* ADDCONSTANT(const TPSCONSTANT *CONSTANT) {
  TPSCONSTANT* RESULT;
  TPSCONSTANT* CONSTPTR;
  if (FINDNAMEINLOCALSCOPE(&CONSTANT->NAME, 0) != PNil) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, &CONSTANT->NAME, CpEnd | CpLenPtr, 16, " already defined"));
  CONSTPTR = _ADDDEF(TDCCONSTANT)->CONSTPTR;
  ADDCONSTANTNAME(&CONSTANT->NAME, CONSTPTR);
  *CONSTPTR = *CONSTANT;
  RESULT = CONSTPTR;
  return RESULT;
}

TPSVARIABLE* ADDVARIABLE(const TPSVARIABLE *VARDEF) {
  TPSVARIABLE* RESULT;
  TPSVARIABLE* VARPTR;
  if (FINDNAMEINLOCALSCOPE(&VARDEF->NAME, 0) != PNil) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, &VARDEF->NAME, CpEnd | CpLenPtr, 16, " already defined"));
  VARPTR = _ADDDEF(TDCVARIABLE)->VARPTR;
  ADDVARIABLENAME(&VARDEF->NAME, VARPTR);
  *VARPTR = *VARDEF;
  RESULT = VARPTR;
  return RESULT;
}

TPSFUNCTION EMPTYFUNCTION() {
  TPSFUNCTION RESULT;
  RESULT.NAME = str_make(0, "");
  RESULT.EXTERNALNAME = str_make(0, "");
  RESULT.ARGS.COUNT = 0;
  RESULT.RETURNTYPEPTR = PNil;
  RESULT.ISDECLARATION = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

PBoolean ISSAMEFUNCTIONDEFINITION(TPSFUNCTION* DECLPTR, TPSFUNCTION FUN) {
  PBoolean RESULT;
  TPSFUNCTION DECL;
  PBoolean SAME;
  PInteger POS;
  DECL = *DECLPTR;
  SAME = ISSAMETYPE(DECL.RETURNTYPEPTR, FUN.RETURNTYPEPTR) && DECL.ARGS.COUNT == FUN.ARGS.COUNT;
  do {
    PInteger first = 1;
    PInteger last = DECL.ARGS.COUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        SAME = SAME && ISSAMETYPE(DECL.ARGS.DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, FUN.ARGS.DEFS[subrange(POS, 1, 16) - 1].TYPEPTR) && DECL.ARGS.DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE == FUN.ARGS.DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE;
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  RESULT = SAME;
  return RESULT;
}

PBoolean HASFORWARDDECLARATION(const PString *NAME) {
  PBoolean RESULT;
  TPSNAME* NAMEPTR;
  NAMEPTR = FINDNAMEOFCLASSINLOCALSCOPE(NAME, TNCFUNCTION, 0);
  RESULT = NAMEPTR != PNil && NAMEPTR->FNPTR->ISDECLARATION;
  return RESULT;
}

TPSFUNCTION* ADDFUNCTION(const TPSFUNCTION *FUN) {
  TPSFUNCTION* RESULT;
  TPSNAME* NAMEPTR;
  TPSFUNCTION* FNPTR;
  PBoolean ISPROCEDURE;
  ISPROCEDURE = FUN->RETURNTYPEPTR == PNil;
  NAMEPTR = FINDNAMEINLOCALSCOPE(&FUN->NAME, 0);
  if (NAMEPTR == PNil) {
    FNPTR = _ADDDEF(TDCFUNCTION)->FNPTR;
    *FNPTR = *FUN;
    ADDFUNCTIONNAME(&FUN->NAME, FNPTR);
  }
  else {
    if (NAMEPTR->CLS != TNCFUNCTION || FUN->ISDECLARATION) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, &FUN->NAME, CpEnd | CpLenPtr, 16, " already defined"));
    FNPTR = NAMEPTR->FNPTR;
    if (FNPTR->ISDECLARATION) {
      if (FUN->ARGS.COUNT == 0 && FUN->RETURNTYPEPTR == PNil || ISSAMEFUNCTIONDEFINITION(FNPTR, *FUN)) FNPTR->ISDECLARATION = 0;
      else {
        if (ISPROCEDURE) COMPILEERROR(CONCAT(CpLenPtr, 10, "Procedure ", CpStringPtr, &FUN->NAME, CpEnd | CpLenPtr, 42, " incompatible with its forward declaration"));
        else COMPILEERROR(CONCAT(CpLenPtr, 9, "Function ", CpStringPtr, &FUN->NAME, CpEnd | CpLenPtr, 42, " incompatible with its forward declaration"));
      }
    }
    else {
      if (ISPROCEDURE) COMPILEERROR(CONCAT(CpLenPtr, 10, "Procedure ", CpStringPtr, &FUN->NAME, CpEnd | CpLenPtr, 16, " already defined"));
      else COMPILEERROR(CONCAT(CpLenPtr, 9, "Function ", CpStringPtr, &FUN->NAME, CpEnd | CpLenPtr, 16, " already defined"));
    }
  }
  RESULT = FNPTR;
  return RESULT;
}

PInteger FINDFIELD(TPSTYPE* TYPEPTR, const PString *NAME, PBoolean REQUIRED) {
  PInteger RESULT;
  PInteger POS;
  PInteger RET;
  ENSURERECORDTYPE(TYPEPTR);
  {
    TPSRECORDDEF *with1 = TYPEPTR->RECPTR;
    {
      RET = 0;
      POS = with1->SIZE;
      while (POS >= 1 && RET == 0) {
        if (cmp_str(CoEq, CpStringPtr, NAME, CpStringPtr, &with1->FIELDS[subrange(POS, 1, 32) - 1].NAME)) RET = POS;
        POS = POS - 1;
      }
    }
  }
  if (REQUIRED && RET == 0) COMPILEERROR(CONCAT(CpLenPtr, 17, "Field not found: ", CpEnd | CpStringPtr, NAME));
  RESULT = RET;
  return RESULT;
}

TPSTYPE* FINDFIELDTYPE(TPSTYPE* TYPEPTR, const PString *NAME, PBoolean REQUIRED) {
  TPSTYPE* RESULT;
  PInteger POS;
  POS = FINDFIELD(TYPEPTR, NAME, REQUIRED);
  if (POS == 0) RESULT = PNil;
  else RESULT = TYPEPTR->RECPTR->FIELDS[subrange(POS, 1, 32) - 1].TYPEPTR;
  return RESULT;
}

TPSWITHVAR* FINDWITHVAR(const PString *NAME) {
  TPSWITHVAR* RESULT;
  TPSWITHVAR* RET;
  TPSDEFENTRY* DEF;
  TPSTYPE* TYPEPTR;
  RET = PNil;
  DEF = DEFS.LATEST;
  while (RET == PNil && DEF != PNil && DEF->CLS != TDCSCOPEBOUNDARY) {
    if (DEF->CLS == TDCWITHVAR) {
      TYPEPTR = DEF->WITHVARPTR->VARPTR->TYPEPTR;
      if (FINDFIELDTYPE(TYPEPTR, NAME, 0) != PNil) RET = DEF->WITHVARPTR;
    }
    DEF = DEF->PREV;
  }
  RESULT = RET;
  return RESULT;
}

TPSVARIABLE* ADDWITHVAR(TEXPRESSIONOBJ* BASE) {
  TPSVARIABLE* RESULT;
  PString TMPVARNUM;
  TPSVARIABLE TMPVAR;
  TPSVARIABLE* TMPVARPTR;
  TPSWITHVAR* WITHVARPTR;
  ENSURERECORDEXPR(BASE);
  STR_i(DEFCOUNTER(TCTTMPVAR), 0, &TMPVARNUM);
  TMPVAR.NAME = CONCAT(CpLenPtr, 4, "with", CpEnd | CpStringPtr, &TMPVARNUM);
  TMPVAR.TYPEPTR = BASE->TYPEPTR;
  TMPVAR.ISCONSTANT = 0;
  TMPVAR.ISREFERENCE = BASE->ISADDRESSABLE;
  TMPVAR.WASINITIALIZED = 1;
  TMPVAR.WASUSED = 1;
  TMPVARPTR = ADDVARIABLE(&TMPVAR);
  WITHVARPTR = _ADDDEF(TDCWITHVAR)->WITHVARPTR;
  WITHVARPTR->VARPTR = TMPVARPTR;
  RESULT = TMPVARPTR;
  return RESULT;
}

TPSCONSTANT MAKECONSTANT(const PString *NAME, TEXPRESSIONOBJ* VALUE) {
  TPSCONSTANT RESULT;
  TPSCONSTANT CONSTANT;
  CONSTANT.NAME = *NAME;
  CONSTANT.VALUE = VALUE;
  RESULT = CONSTANT;
  return RESULT;
}

TPSVARIABLE MAKETYPEDCONSTANT(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSVARIABLE RESULT;
  TPSVARIABLE VARDEF;
  VARDEF.NAME = *NAME;
  VARDEF.TYPEPTR = TYPEPTR;
  VARDEF.ISREFERENCE = 0;
  VARDEF.ISCONSTANT = 1;
  VARDEF.WASINITIALIZED = 1;
  RESULT = VARDEF;
  return RESULT;
}

TPSVARIABLE MAKEVARIABLE(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT.NAME = *NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = 0;
  RESULT.ISCONSTANT = 0;
  RESULT.WASINITIALIZED = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

TPSVARIABLE* ADDTMPVARIABLE(const PString *PREFIX, TPSTYPE* TYPEPTR) {
  TPSVARIABLE* RESULT;
  PString VARNUM;
  STR_i(DEFCOUNTER(TCTTMPVAR), 0, &VARNUM);
  RESULT = ({ TPSVARIABLE tmp2 = ({ PString tmp1 = CONCAT(CpStringPtr, PREFIX, CpEnd | CpStringPtr, &VARNUM); MAKEVARIABLE(&tmp1, TYPEPTR); }); ADDVARIABLE(&tmp2); });
  return RESULT;
}

TPSVARIABLE _MAKEARG(const PString *NAME, TPSTYPE* TYPEPTR, PBoolean ISREF, PBoolean ISCONST) {
  TPSVARIABLE RESULT;
  RESULT.NAME = *NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = ISREF || ISCONST;
  RESULT.ISCONSTANT = ISCONST;
  RESULT.WASINITIALIZED = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

TPSVARIABLE MAKEARG(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 0, 0);
  return RESULT;
}

TPSVARIABLE MAKEVARARG(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 1, 0);
  return RESULT;
}

TPSVARIABLE MAKECONSTARG(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSVARIABLE RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 0, 1);
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE0(const PString *NAME) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 0;
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE1(const PString *NAME, TPSVARIABLE ARG) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 1;
  RESULT.ARGS.DEFS[0] = ARG;
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE2(const PString *NAME, TPSVARIABLE ARG1, TPSVARIABLE ARG2) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 2;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  return RESULT;
}

TPSFUNCTION MAKEPROCEDURE3(const PString *NAME, TPSVARIABLE ARG1, TPSVARIABLE ARG2, TPSVARIABLE ARG3) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 3;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  RESULT.ARGS.DEFS[2] = ARG3;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION0(const PString *NAME, TPSTYPE* RETTYPEPTR) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION1(const PString *NAME, TPSTYPE* RETTYPEPTR, TPSVARIABLE ARG) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGS.COUNT = 1;
  RESULT.ARGS.DEFS[0] = ARG;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION2(const PString *NAME, TPSTYPE* RETTYPEPTR, TPSVARIABLE ARG1, TPSVARIABLE ARG2) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGS.COUNT = 2;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  return RESULT;
}

TPSFUNCTION MAKEFUNCTION3(const PString *NAME, TPSTYPE* RETTYPEPTR, TPSVARIABLE ARG1, TPSVARIABLE ARG2, TPSVARIABLE ARG3) {
  TPSFUNCTION RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGS.COUNT = 3;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  RESULT.ARGS.DEFS[2] = ARG3;
  return RESULT;
}

TPSTYPE* _UNALIASTYPE(TPSTYPE* TYPEPTR) {
  TPSTYPE* RESULT;
  RESULT = TYPEPTR;
  while (RESULT->ALIASFOR != PNil) RESULT = RESULT->ALIASFOR;
  return RESULT;
}

TPSTYPE* _NEWTYPE(TPSTYPECLASS CLS) {
  TPSTYPE* RESULT;
  RESULT = _ADDDEF(TDCTYPE)->TYPEPTR;
  RESULT->NAME = str_make(0, "");
  RESULT->CLS = CLS;
  RESULT->ALIASFOR = PNil;
  RESULT->WASUSED = 0;
  return RESULT;
}

TPSTYPE* MAKEBASETYPE(const PString *NAME, TPSTYPECLASS CLS) {
  TPSTYPE* RESULT;
  RESULT = _NEWTYPE(CLS);
  RESULT->NAME = *NAME;
  ADDTYPENAME(NAME, RESULT);
  return RESULT;
}

TPSTYPE* MAKEENUMTYPE(const TPSENUMDEF *ENUM) {
  TPSTYPE* RESULT;
  PInteger POS;
  RESULT = _NEWTYPE(TTCENUM);
  RESULT->ENUMPTR = NEWENUM(ENUM);
  RESULT->WASUSED = 1;
  do {
    PInteger first = 0;
    PInteger last = RESULT->ENUMPTR->SIZE - 1;
    if (first <= last) {
      POS = first;
      while (1) {
        ADDENUMVALNAME(POS, RESULT);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return RESULT;
}

TPSTYPE* MAKERECORDTYPE(const TPSRECORDDEF *REC) {
  TPSTYPE* RESULT;
  RESULT = _NEWTYPE(TTCRECORD);
  RESULT->RECPTR = NEWRECORD(REC);
  return RESULT;
}

TPSTYPE* MAKEARRAYTYPE(TPSTYPE* INDEXTYPE, TPSTYPE* VALUETYPE) {
  TPSTYPE* RESULT;
  TPSDEFENTRY* DEF;
  RESULT = PNil;
  DEF = DEFS.LATEST;
  while (DEF != PNil && RESULT == PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCARRAY && ISSAMETYPE(DEF->TYPEPTR->ARRAYDEF.INDEXTYPEPTR, INDEXTYPE) && ISSAMETYPE(DEF->TYPEPTR->ARRAYDEF.VALUETYPEPTR, VALUETYPE)) RESULT = _UNALIASTYPE(DEF->TYPEPTR);
    DEF = DEF->PREV;
  }
  if (RESULT == PNil) {
    if (!ISBOUNDEDTYPE(INDEXTYPE)) {
      PString tmp1 = str_make(51, "Array indices must belong to a bounded ordinal type");
      ERRORFORTYPE(&tmp1, INDEXTYPE);
    }
    RESULT = _NEWTYPE(TTCARRAY);
    RESULT->ARRAYDEF.INDEXTYPEPTR = INDEXTYPE;
    RESULT->ARRAYDEF.VALUETYPEPTR = VALUETYPE;
  }
  return RESULT;
}

TPSTYPE* MAKERANGETYPE(TPSTYPE* TYPEPTR, PInteger FIRST, PInteger LAST) {
  TPSTYPE* RESULT;
  TPSDEFENTRY* DEF;
  RESULT = PNil;
  DEF = DEFS.LATEST;
  while (DEF != PNil && RESULT == PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCRANGE && ISSAMETYPE(DEF->TYPEPTR->RANGEDEF.BASETYPEPTR, TYPEPTR) && FIRST == DEF->TYPEPTR->RANGEDEF.FIRST && LAST == DEF->TYPEPTR->RANGEDEF.LAST) RESULT = _UNALIASTYPE(DEF->TYPEPTR);
    DEF = DEF->PREV;
  }
  if (RESULT == PNil) {
    if (FIRST > LAST) COMPILEERROR(str_make(51, "The bounds of a subrange must be in ascending order"));
    RESULT = _NEWTYPE(TTCRANGE);
    RESULT->RANGEDEF.FIRST = FIRST;
    RESULT->RANGEDEF.LAST = LAST;
    RESULT->RANGEDEF.BASETYPEPTR = GETFUNDAMENTALTYPE(TYPEPTR);
  }
  return RESULT;
}

TPSTYPE* MAKEPOINTERTYPE(TPSTYPE* TYPEPTR) {
  TPSTYPE* RESULT;
  TPSDEFENTRY* DEF;
  RESULT = PNil;
  DEF = DEFS.LATEST;
  while (DEF != PNil && RESULT == PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCPOINTER && ISSAMETYPE(DEF->TYPEPTR->POINTEDTYPEPTR, TYPEPTR)) RESULT = _UNALIASTYPE(DEF->TYPEPTR);
    DEF = DEF->PREV;
  }
  if (RESULT == PNil) {
    RESULT = _NEWTYPE(TTCPOINTER);
    RESULT->POINTEDTYPEPTR = TYPEPTR;
  }
  return RESULT;
}

TPSTYPE* MAKEPOINTERUNKNOWNTYPE(const PString *TARGETNAME) {
  TPSTYPE* RESULT;
  TPSDEFENTRY* DEF;
  RESULT = PNil;
  DEF = DEFS.LATEST;
  while (DEF != PNil && RESULT == PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCPOINTERUNKNOWN && cmp_str(CoEq, CpStringPtr, DEF->TYPEPTR->TARGETNAME, CpStringPtr, TARGETNAME)) RESULT = _UNALIASTYPE(DEF->TYPEPTR);
    DEF = DEF->PREV;
  }
  if (RESULT == PNil) {
    RESULT = _NEWTYPE(TTCPOINTERUNKNOWN);
    RESULT->TARGETNAME = malloc(sizeof(PString));
    *RESULT->TARGETNAME = *TARGETNAME;
  }
  return RESULT;
}

TPSTYPE* MAKESETTYPE(TPSTYPE* TYPEPTR) {
  TPSTYPE* RESULT;
  TPSDEFENTRY* DEF;
  RESULT = PNil;
  DEF = DEFS.LATEST;
  while (DEF != PNil && RESULT == PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCSET && ISSAMETYPE(DEF->TYPEPTR->ELEMENTTYPEPTR, TYPEPTR)) RESULT = _UNALIASTYPE(DEF->TYPEPTR);
    DEF = DEF->PREV;
  }
  if (RESULT == PNil) {
    RESULT = _NEWTYPE(TTCSET);
    RESULT->ELEMENTTYPEPTR = TYPEPTR;
  }
  return RESULT;
}

PBoolean ARESAMEARGS(const TPSFNARGS *A, const TPSFNARGS *B) {
  PBoolean RESULT;
  PInteger POS;
  RESULT = A->COUNT == B->COUNT;
  if (RESULT) do {
    PInteger first = 1;
    PInteger last = A->COUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        RESULT = RESULT && A->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE == B->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE && A->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT == B->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT && ISSAMETYPE(A->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, B->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return RESULT;
}

TPSTYPE* MAKEFUNCTIONTYPE(const TPSFNARGS *ARGS, TPSTYPE* RETURNTYPEPTR) {
  TPSTYPE* RESULT;
  TPSDEFENTRY* DEF;
  RESULT = PNil;
  DEF = DEFS.LATEST;
  while (DEF != PNil && RESULT == PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCFUNCTION && ISSAMETYPE(DEF->TYPEPTR->FNDEFPTR->RETURNTYPEPTR, RETURNTYPEPTR) && ARESAMEARGS(&DEF->TYPEPTR->FNDEFPTR->ARGS, ARGS)) RESULT = _UNALIASTYPE(DEF->TYPEPTR);
    DEF = DEF->PREV;
  }
  if (RESULT == PNil) {
    RESULT = _NEWTYPE(TTCFUNCTION);
    RESULT->FNDEFPTR = NEWFNDEF();
    RESULT->FNDEFPTR->RETURNTYPEPTR = RETURNTYPEPTR;
    RESULT->FNDEFPTR->ARGS = *ARGS;
  }
  return RESULT;
}

TPSTYPE* MAKEALIASTYPE(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSTYPE* RESULT;
  TYPEPTR = _UNALIASTYPE(TYPEPTR);
  RESULT = _NEWTYPE(TYPEPTR->CLS);
  *RESULT = COPYTYPE(TYPEPTR);
  RESULT->NAME = *NAME;
  RESULT->ALIASFOR = TYPEPTR;
  ADDTYPENAME(NAME, RESULT);
  return RESULT;
}

void CODEGENRESET();
void CODEGENSETOUTPUT(PString FILENAME);
void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY* CHECKPOINT);
void OUTTYPEDEFINITIONSFROMCHECKPOINT(TPSDEFENTRY* CHECKPOINT);
void OUTCONSTANTARRAYBEGIN();
void OUTCONSTANTARRAYSEPARATOR();
void OUTCONSTANTARRAYEND();
void OUTVARIABLEDEFINITION(TPSVARIABLE* VARPTR);
void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE* VARPTR);
void OUTCONSTANTDEFINITIONEND();
void OUTFUNCTIONDECLARATION(TPSFUNCTION* FNPTR);
void OUTFUNCTIONDEFINITION(TPSFUNCTION* FNPTR);
void OUTFUNCTIONEND(TPSFUNCTION* FNPTR);
void OUTEXPRESSION(TEXPRESSIONOBJ* EXPR);
void OUTASSIGN(TEXPRESSIONOBJ* LHS, TEXPRESSIONOBJ* RHS);
void OUTDECLAREANDASSIGN(TPSVARIABLE* VARPTR, TEXPRESSIONOBJ* RHS);
void OUTSEQUENCEBEGIN();
void OUTSEQUENCEEND();
void OUTIF(TEXPRESSIONOBJ* EXPR);
void OUTELSE();
void OUTIFEND();
void OUTCASEBEGIN(TEXPRESSIONOBJ* CASEINDEX);
void OUTCASESTATEMENTBEGIN(TEXPRESSIONOBJ* CASELABEL);
void OUTCASESTATEMENTEND();
void OUTCASEELSEBEGIN();
void OUTCASEELSEEND();
void OUTCASEEND();
void OUTREPEATBEGIN();
void OUTREPEATEND(TEXPRESSIONOBJ* EXPR);
void OUTWHILEBEGIN(TEXPRESSIONOBJ* EXPR);
void OUTWHILEEND();
void OUTFORBEGIN(TEXPRESSIONOBJ* ITER, TEXPRESSIONOBJ* FIRSTEXPR, TEXPRESSIONOBJ* LASTEXPR, PBoolean ASCENDING);
void OUTFOREND(TEXPRESSIONOBJ* ITER, PBoolean ASCENDING);
void OUTPROCEDURECALL(TEXPRESSIONOBJ* EXPR);
void OUTPSEUDOPROCCALL(TEXPRESSIONOBJ* EXPR);
void OUTEMPTYSTATEMENT();
void OUTPROGRAMHEADING(PString NAME);
void OUTPROGRAMBEGIN();
void OUTPROGRAMEND();

TEXPRESSIONOBJ* _NEWEXPR(TEXPRESSIONCLASS CLS) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = malloc(sizeof(TEXPRESSIONOBJ));
  RESULT->CLS = CLS;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISADDRESSABLE = 0;
  RESULT->ISFUNCTIONRESULT = 0;
  return RESULT;
}

void _DISPOSEPSEUDOCALLEXPR(TEXPSEUDOFNCALL *CALL) {
  TEXREADARGS* READARG;
  TEXREADARGS* NEXTREADARG;
  TEXWRITEARGS* WRITEARG;
  TEXWRITEARGS* NEXTWRITEARG;
  if (CALL->ARG1 != PNil) EXDISPOSE(&CALL->ARG1);
  if (CALL->ARG2 != PNil) EXDISPOSE(&CALL->ARG2);
  if (CALL->ARG3 != PNil) EXDISPOSE(&CALL->ARG3);
  if (CALL->ARG4 != PNil) EXDISPOSE(&CALL->ARG4);
  READARG = CALL->READARGS;
  while (READARG != PNil) {
    NEXTREADARG = READARG->NEXT;
    EXDISPOSE(&READARG->ARG);
    free(READARG);
    READARG = NEXTREADARG;
  }
  WRITEARG = CALL->WRITEARGS;
  while (WRITEARG != PNil) {
    NEXTWRITEARG = WRITEARG->NEXT;
    EXDISPOSE(&WRITEARG->ARG);
    if (WRITEARG->WIDTH != PNil) EXDISPOSE(&WRITEARG->WIDTH);
    if (WRITEARG->PREC != PNil) EXDISPOSE(&WRITEARG->PREC);
    free(WRITEARG);
    WRITEARG = NEXTWRITEARG;
  }
}

void _DISPOSEIMMEDIATE(TEXIMMEDIATE *IMM) {
  TEXSETIMMBOUNDSOBJ* BOUNDS;
  if (IMM->CLS == XICSET) {
    while (IMM->SETBOUNDS != PNil) {
      BOUNDS = IMM->SETBOUNDS->NEXT;
      free(IMM->SETBOUNDS);
      IMM->SETBOUNDS = BOUNDS;
    }
  }
}

void _DISPOSEBOUNDS(TEXSETEXPRBOUNDSOBJ* BOUNDS) {
  TEXSETEXPRBOUNDSOBJ* NEXT;
  while (BOUNDS != PNil) {
    NEXT = BOUNDS->NEXT;
    EXDISPOSE(&BOUNDS->FIRST);
    if (BOUNDS->LAST != PNil) EXDISPOSE(&BOUNDS->LAST);
    free(BOUNDS);
    BOUNDS = NEXT;
  }
}

void EXDISPOSE(TEXPRESSIONOBJ* *EXPR) {
  PInteger POS;
  switch ((*EXPR)->CLS) {
    case XCIMMEDIATE:
      _DISPOSEIMMEDIATE(&(*EXPR)->IMMEDIATE);
      break;
    case XCTOSTRING:
      EXDISPOSE(&(*EXPR)->TOSTRPARENT);
      break;
    case XCTOREAL:
      EXDISPOSE(&(*EXPR)->TOREALPARENT);
      break;
    case XCWITHTMPVAR:
      {
        EXDISPOSE(&(*EXPR)->TMPVAR);
        EXDISPOSE(&(*EXPR)->TMPVARVALUE);
        EXDISPOSE(&(*EXPR)->TMPVARCHILD);
      }
      break;
    case XCSUBRANGE:
      EXDISPOSE(&(*EXPR)->SUBRANGEPARENT);
      break;
    case XCSET:
      {
        EXDISPOSE(&(*EXPR)->SETBASE);
        _DISPOSEBOUNDS((*EXPR)->SETBOUNDS);
      }
      break;
    case XCFIELD:
      EXDISPOSE(&(*EXPR)->RECEXPR);
      break;
    case XCARRAY:
      {
        EXDISPOSE(&(*EXPR)->ARRAYEXPR);
        EXDISPOSE(&(*EXPR)->ARRAYINDEX);
      }
      break;
    case XCPOINTER:
      EXDISPOSE(&(*EXPR)->POINTEREXPR);
      break;
    case XCADDRESS:
      EXDISPOSE(&(*EXPR)->ADDRESSEXPR);
      break;
    case XCSTRINGCHAR:
      {
        EXDISPOSE(&(*EXPR)->STRINGEXPR);
        EXDISPOSE(&(*EXPR)->STRINGINDEX);
      }
      break;
    case XCFNCALL:
      {
        EXDISPOSE(&(*EXPR)->FNEXPR);
        do {
          PInteger first = 1;
          PInteger last = (*EXPR)->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              EXDISPOSE(&(*EXPR)->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
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
      EXDISPOSE(&(*EXPR)->UNARY.PARENT);
      break;
    case XCBINARYOP:
      {
        EXDISPOSE(&(*EXPR)->BINARY.LEFT);
        EXDISPOSE(&(*EXPR)->BINARY.RIGHT);
      }
      break;
    default:
      break;
  }
  free(*EXPR);
}

void _COPYPSEUDOCALLEXPR(TEXPSEUDOFNCALL *CALL, TEXPSEUDOFNCALL *COPY) {
  TEXREADARGS* READARG;
  TEXREADARGS* NEXTREADARG;
  TEXREADARGS* COPYREADARG;
  TEXWRITEARGS* WRITEARG;
  TEXWRITEARGS* NEXTWRITEARG;
  TEXWRITEARGS* COPYWRITEARG;
  COPY->PSEUDOFNPTR = CALL->PSEUDOFNPTR;
  if (CALL->ARG1 != PNil) COPY->ARG1 = EXCOPY(CALL->ARG1);
  if (CALL->ARG2 != PNil) COPY->ARG2 = EXCOPY(CALL->ARG2);
  if (CALL->ARG3 != PNil) COPY->ARG3 = EXCOPY(CALL->ARG3);
  if (CALL->ARG4 != PNil) COPY->ARG4 = EXCOPY(CALL->ARG4);
  COPY->TYPEARG = CALL->TYPEARG;
  READARG = CALL->READARGS;
  COPYREADARG = PNil;
  while (READARG != PNil) {
    NEXTREADARG = READARG->NEXT;
    if (COPYREADARG == PNil) {
      COPYREADARG = malloc(sizeof(TEXREADARGS));
      COPY->READARGS = COPYREADARG;
    }
    else {
      COPYREADARG->NEXT = malloc(sizeof(TEXREADARGS));
      COPYREADARG = COPYREADARG->NEXT;
    }
    COPYREADARG->NEXT = PNil;
    COPYREADARG->ARG = EXCOPY(READARG->ARG);
    READARG = NEXTREADARG;
  }
  WRITEARG = CALL->WRITEARGS;
  COPYWRITEARG = PNil;
  while (WRITEARG != PNil) {
    NEXTWRITEARG = WRITEARG->NEXT;
    if (COPYWRITEARG == PNil) {
      COPYWRITEARG = malloc(sizeof(TEXWRITEARGS));
      COPY->WRITEARGS = COPYWRITEARG;
    }
    else {
      COPYWRITEARG->NEXT = malloc(sizeof(TEXWRITEARGS));
      COPYWRITEARG = COPYWRITEARG->NEXT;
    }
    COPYWRITEARG->NEXT = PNil;
    COPYWRITEARG->ARG = EXCOPY(WRITEARG->ARG);
    if (WRITEARG->WIDTH != PNil) COPYWRITEARG->WIDTH = EXCOPY(WRITEARG->WIDTH);
    if (WRITEARG->PREC != PNil) COPYWRITEARG->PREC = EXCOPY(WRITEARG->PREC);
    WRITEARG = NEXTWRITEARG;
  }
}

TEXIMMEDIATE _COPYIMMEDIATE(const TEXIMMEDIATE *IMM) {
  TEXIMMEDIATE RESULT;
  TEXSETIMMBOUNDSOBJ* SRC;
  TEXSETIMMBOUNDSOBJ* DST;
  RESULT = *IMM;
  if (IMM->CLS == XICSET) {
    SRC = IMM->SETBOUNDS;
    RESULT.SETBOUNDS = malloc(sizeof(TEXSETIMMBOUNDSOBJ));
    DST = RESULT.SETBOUNDS;
    while (SRC != PNil) {
      *DST = *SRC;
      SRC = SRC->NEXT;
      if (SRC != PNil) {
        DST->NEXT = malloc(sizeof(TEXSETIMMBOUNDSOBJ));
        DST = DST->NEXT;
      }
    }
  }
  return RESULT;
}

TEXSETEXPRBOUNDSOBJ* _COPYBOUNDS(TEXSETEXPRBOUNDSOBJ* BOUNDS) {
  TEXSETEXPRBOUNDSOBJ* RESULT;
  TEXSETEXPRBOUNDSOBJ* SRC;
  TEXSETEXPRBOUNDSOBJ* DST;
  SRC = BOUNDS;
  DST = PNil;
  while (SRC != PNil) {
    if (DST == PNil) {
      DST = malloc(sizeof(TEXSETEXPRBOUNDSOBJ));
      RESULT = DST;
    }
    else {
      DST->NEXT = malloc(sizeof(TEXSETEXPRBOUNDSOBJ));
      DST = DST->NEXT;
    }
    DST->FIRST = EXCOPY(SRC->FIRST);
    if (SRC->LAST != PNil) DST->LAST = EXCOPY(SRC->LAST);
    else DST->LAST = PNil;
    DST->NEXT = PNil;
    SRC = SRC->NEXT;
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXCOPY(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* COPY;
  PInteger POS;
  COPY = _NEWEXPR(EXPR->CLS);
  COPY->TYPEPTR = EXPR->TYPEPTR;
  COPY->ISASSIGNABLE = EXPR->ISASSIGNABLE;
  COPY->ISADDRESSABLE = EXPR->ISADDRESSABLE;
  COPY->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      COPY->IMMEDIATE = _COPYIMMEDIATE(&EXPR->IMMEDIATE);
      break;
    case XCTOSTRING:
      COPY->TOSTRPARENT = EXCOPY(EXPR->TOSTRPARENT);
      break;
    case XCTOREAL:
      COPY->TOREALPARENT = EXCOPY(EXPR->TOREALPARENT);
      break;
    case XCWITHTMPVAR:
      {
        COPY->TMPVAR = EXCOPY(EXPR->TMPVAR);
        COPY->TMPVARVALUE = EXCOPY(EXPR->TMPVARVALUE);
        COPY->TMPVARCHILD = EXCOPY(EXPR->TMPVARCHILD);
      }
      break;
    case XCSUBRANGE:
      COPY->SUBRANGEPARENT = EXCOPY(EXPR->SUBRANGEPARENT);
      break;
    case XCSET:
      {
        COPY->SETBASE = EXCOPY(EXPR->SETBASE);
        COPY->SETBOUNDS = _COPYBOUNDS(EXPR->SETBOUNDS);
      }
      break;
    case XCVARIABLE:
      COPY->VARPTR = EXPR->VARPTR;
      break;
    case XCFIELD:
      {
        COPY->RECEXPR = EXCOPY(EXPR->RECEXPR);
        COPY->RECFIELDNUM = EXPR->RECFIELDNUM;
      }
      break;
    case XCARRAY:
      {
        COPY->ARRAYEXPR = EXCOPY(EXPR->ARRAYEXPR);
        COPY->ARRAYINDEX = EXCOPY(EXPR->ARRAYINDEX);
      }
      break;
    case XCPOINTER:
      COPY->POINTEREXPR = EXCOPY(EXPR->POINTEREXPR);
      break;
    case XCADDRESS:
      COPY->ADDRESSEXPR = EXCOPY(EXPR->ADDRESSEXPR);
      break;
    case XCSTRINGCHAR:
      {
        COPY->STRINGEXPR = EXCOPY(EXPR->STRINGEXPR);
        COPY->STRINGINDEX = EXCOPY(EXPR->STRINGINDEX);
      }
      break;
    case XCFNREF:
      COPY->FNPTR = EXPR->FNPTR;
      break;
    case XCFNCALL:
      {
        COPY->FNEXPR = EXCOPY(EXPR->FNEXPR);
        COPY->CALLARGS.SIZE = EXPR->CALLARGS.SIZE;
        do {
          PInteger first = 1;
          PInteger last = EXPR->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              COPY->CALLARGS.VALUES[subrange(POS, 1, 16) - 1] = EXCOPY(EXPR->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
      }
      break;
    case XCPSEUDOFNREF:
      COPY->PSEUDOFNPTR = EXPR->PSEUDOFNPTR;
      break;
    case XCPSEUDOFNCALL:
      _COPYPSEUDOCALLEXPR(&EXPR->PSEUDOFNCALL, &COPY->PSEUDOFNCALL);
      break;
    case XCUNARYOP:
      {
        COPY->UNARY.PARENT = EXCOPY(EXPR->UNARY.PARENT);
        COPY->UNARY.OP = EXPR->UNARY.OP;
      }
      break;
    case XCBINARYOP:
      {
        COPY->BINARY.LEFT = EXCOPY(EXPR->BINARY.LEFT);
        COPY->BINARY.RIGHT = EXCOPY(EXPR->BINARY.RIGHT);
        COPY->BINARY.OP = EXPR->BINARY.OP;
      }
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 24, "Cannot copy expression: ", CpEnd | CpString, EXDESCRIBE(EXPR)));
      break;
  }
  RESULT = COPY;
  return RESULT;
}

PString _DESCRIBEIMMSETINTERNAL(TEXSETIMMBOUNDSOBJ* BOUNDS, TPSTYPE* SETOFTYPEPTR) {
  PString RESULT;
  RESULT = str_make(0, "");
  while (BOUNDS != PNil) {
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXGETANTIORDINAL(BOUNDS->FIRST, SETOFTYPEPTR)));
    if (BOUNDS->FIRST != BOUNDS->LAST) RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 2, "..", CpEnd | CpString, EXDESCRIBE(EXGETANTIORDINAL(BOUNDS->LAST, SETOFTYPEPTR)));
    BOUNDS = BOUNDS->NEXT;
    if (BOUNDS != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
  }
  return RESULT;
}

PString _DESCRIBEIMMSET(TEXSETIMMBOUNDSOBJ* BOUNDS, TPSTYPE* SETOFTYPEPTR) {
  PString RESULT;
  RESULT = CONCAT(CpChar, '[', CpString, _DESCRIBEIMMSETINTERNAL(BOUNDS, SETOFTYPEPTR), CpEnd | CpChar, ']');
  return RESULT;
}

PString _DESCRIBEIMMEDIATE(TEXPRESSIONOBJ* EXPR) {
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
        RESULT = UNPARSESTRING(&with1->STRINGVAL);
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
const PInteger _EXPRECEDENCES[18] = { 0, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, -1, -1 };
const PInteger _EXOPPRECEDENCES[23] = { 4, 4, 4, 3, 3, 3, 4, 3, 4, 4, 3, 3, 5, 5, 5, 5, 5, 5, 1, 1, 1 };
const PString _EXOPNAMES[23] = { str_of('+'), str_of('-'), str_of('*'), str_of('/'), str_make(3, "DIV"), str_make(3, "MOD"), str_of('-'), str_make(3, "AND"), str_make(2, "OR"), str_make(3, "XOR"), str_make(3, "NOT"), str_make(3, "SHL"), str_make(3, "SHR"), str_make(2, "IN"), str_of('='), str_make(2, "<>"), str_of('<'), str_of('>'), str_make(2, "<="), str_make(2, ">="), str_make(3, "ORD"), str_make(4, "PRED"), str_make(4, "SUCC") };

PInteger _EXPRPRECEDENCE(TEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  RESULT = _EXPRECEDENCES[EXPR->CLS];
  if (RESULT < 0) {
    switch (EXPR->CLS) {
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
      case XCUNARYOP:
        RESULT = _EXOPPRECEDENCES[EXPR->UNARY.OP];
        break;
      case XCBINARYOP:
        RESULT = _EXOPPRECEDENCES[EXPR->BINARY.OP];
        break;
      default:
        break;
    }
  }
  if (RESULT < 0) COMPILEERROR(str_make(33, "Unknown precedence for expression"));
  return RESULT;
}

PString EXDESCRIBEOPERATOR(TEXOPERATOR OP) {
  PString RESULT;
  RESULT = _EXOPNAMES[OP];
  return RESULT;
}

PString _DESCRIBEUNARYOPEXPR(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  PBoolean USEPARENS;
  RESULT = EXDESCRIBEOPERATOR(EXPR->UNARY.OP);
  if (EXPR->UNARY.OP != XONEG) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ' ');
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->UNARY.PARENT);
  if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '(');
  RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXPR->UNARY.PARENT));
  if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  return RESULT;
}

PString _DESCRIBEBINARYOPEXPR(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  PBoolean USEPARENS;
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->BINARY.LEFT);
  if (USEPARENS) RESULT = str_of('(');
  else RESULT = str_make(0, "");
  RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXPR->BINARY.LEFT));
  if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  RESULT = CONCAT(CpStringPtr, &RESULT, CpChar, ' ', CpString, EXDESCRIBEOPERATOR(EXPR->BINARY.OP), CpEnd | CpChar, ' ');
  USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->BINARY.RIGHT);
  if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '(');
  RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXPR->BINARY.RIGHT));
  if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  return RESULT;
}

PString _DESCRIBEWITHTMPVAR(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  RESULT = str_make(6, "{with ");
  while (EXPR->CLS == XCWITHTMPVAR) {
    RESULT = CONCAT(CpStringPtr, &RESULT, CpString, EXDESCRIBE(EXPR->TMPVAR), CpLenPtr, 2, ":=", CpEnd | CpString, EXDESCRIBE(EXPR->TMPVARVALUE));
    EXPR = EXPR->TMPVARCHILD;
    if (EXPR->CLS == XCWITHTMPVAR) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
  }
  RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 2, "} ", CpEnd | CpString, EXDESCRIBE(EXPR));
  return RESULT;
}

PString _DESCRIBESET(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  TEXSETEXPRBOUNDSOBJ* BOUNDS;
  RESULT = CONCAT(CpChar, '[', CpEnd | CpString, _DESCRIBEIMMSETINTERNAL(EXPR->SETBASE->IMMEDIATE.SETBOUNDS, EXPR->SETBASE->IMMEDIATE.SETOFTYPEPTR));
  if (EXPR->SETBASE->IMMEDIATE.SETBOUNDS != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
  BOUNDS = EXPR->SETBOUNDS;
  while (BOUNDS != PNil) {
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(BOUNDS->FIRST));
    if (BOUNDS->LAST != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 2, "..", CpEnd | CpString, EXDESCRIBE(BOUNDS->LAST));
    BOUNDS = BOUNDS->NEXT;
    if (BOUNDS != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
  }
  RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ']');
  return RESULT;
}

PString EXDESCRIBE(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  PInteger POS;
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      RESULT = _DESCRIBEIMMEDIATE(EXPR);
      break;
    case XCTOSTRING:
      RESULT = EXDESCRIBE(EXPR->TOSTRPARENT);
      break;
    case XCTOREAL:
      RESULT = EXDESCRIBE(EXPR->TOREALPARENT);
      break;
    case XCWITHTMPVAR:
      RESULT = _DESCRIBEWITHTMPVAR(EXPR);
      break;
    case XCSUBRANGE:
      RESULT = EXDESCRIBE(EXPR->TOSTRPARENT);
      break;
    case XCSET:
      RESULT = _DESCRIBESET(EXPR);
      break;
    case XCVARIABLE:
      RESULT = EXPR->VARPTR->NAME;
      break;
    case XCFIELD:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->RECEXPR), CpChar, '.', CpEnd | CpStringPtr, &EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[subrange(EXPR->RECFIELDNUM, 1, 32) - 1].NAME);
      break;
    case XCARRAY:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->ARRAYEXPR), CpChar, '[', CpString, EXDESCRIBE(EXPR->ARRAYINDEX), CpEnd | CpChar, ']');
      break;
    case XCPOINTER:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->POINTEREXPR), CpEnd | CpChar, '^');
      break;
    case XCADDRESS:
      RESULT = CONCAT(CpChar, '@', CpEnd | CpString, EXDESCRIBE(EXPR->ADDRESSEXPR));
      break;
    case XCSTRINGCHAR:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->STRINGEXPR), CpChar, '[', CpString, EXDESCRIBE(EXPR->STRINGINDEX), CpEnd | CpChar, ']');
      break;
    case XCFNREF:
      RESULT = EXPR->FNPTR->NAME;
      break;
    case XCFNCALL:
      {
        RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->FNEXPR), CpEnd | CpChar, '(');
        do {
          PInteger first = 1;
          PInteger last = EXPR->CALLARGS.SIZE;
          if (first <= last) {
            POS = first;
            while (1) {
              {
                if (POS != 1) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
                RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXPR->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]));
              }
              if (POS == last) break;
              ++POS;
            }
          }
        } while(0);
        RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
      }
      break;
    case XCPSEUDOFNREF:
      RESULT = EXPR->PSEUDOFNPTR->NAME;
      break;
    case XCPSEUDOFNCALL:
      RESULT = EXPR->PSEUDOFNPTR->DESCRIBEFN(EXPR);
      break;
    case XCUNARYOP:
      RESULT = _DESCRIBEUNARYOPEXPR(EXPR);
      break;
    case XCBINARYOP:
      RESULT = _DESCRIBEBINARYOPEXPR(EXPR);
      break;
    default:
      INTERNALERROR(str_make(26, "Cannot describe expression"));
      break;
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXCOERCE(TEXPRESSIONOBJ* EXPR, TPSTYPE* TYPEPTR);

TEXPRESSIONOBJ* _EXIMMEDIATE(TEXIMMEDIATECLASS CLS) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* EXPR;
  EXPR = _NEWEXPR(XCIMMEDIATE);
  EXPR->IMMEDIATE.CLS = CLS;
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* EXNIL() {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICNIL);
  RESULT->TYPEPTR = PRIMITIVETYPES.PTNIL;
  return RESULT;
}

TEXPRESSIONOBJ* EXBOOLEANCONSTANT(PBoolean VALUE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICBOOLEAN);
  RESULT->IMMEDIATE.BOOLEANVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  return RESULT;
}

TEXPRESSIONOBJ* EXINTEGERCONSTANT(PInteger VALUE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICINTEGER);
  RESULT->IMMEDIATE.INTEGERVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  return RESULT;
}

TEXPRESSIONOBJ* EXREALCONSTANT(PReal VALUE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICREAL);
  RESULT->IMMEDIATE.REALVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  return RESULT;
}

TEXPRESSIONOBJ* EXCHARCONSTANT(PChar VALUE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICCHAR);
  RESULT->IMMEDIATE.CHARVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTCHAR;
  return RESULT;
}

TEXPRESSIONOBJ* EXSTRINGCONSTANT(PString VALUE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICSTRING);
  RESULT->IMMEDIATE.STRINGVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
  return RESULT;
}

TEXPRESSIONOBJ* EXENUMCONSTANT(PInteger ORDINAL, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
  ENSUREENUMTYPE(TYPEPTR);
  if (ORDINAL < 0 || ORDINAL > TYPEPTR->ENUMPTR->SIZE - 1) COMPILEERROR(CONCAT(CpLenPtr, 18, "Invalid value for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
  RESULT = _EXIMMEDIATE(XICENUM);
  RESULT->IMMEDIATE.ENUMORDINAL = ORDINAL;
  RESULT->IMMEDIATE.ENUMPTR = TYPEPTR->ENUMPTR;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TEXPRESSIONOBJ* EXSETCONSTANT(TEXSETIMMBOUNDSOBJ* BOUNDS, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
  TPSTYPE* ELEMENTTYPE;
  ELEMENTTYPE = TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMENTTYPE != PNil) ENSUREORDINALTYPE(ELEMENTTYPE);
  RESULT = _EXIMMEDIATE(XICSET);
  RESULT->IMMEDIATE.SETBOUNDS = BOUNDS;
  RESULT->IMMEDIATE.SETOFTYPEPTR = ELEMENTTYPE;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TEXSETIMMBOUNDSOBJ* EXSETADDBOUNDS(TEXSETIMMBOUNDSOBJ* BOUNDS, PInteger FIRST, PInteger LAST) {
  TEXSETIMMBOUNDSOBJ* RESULT;
  TEXSETIMMBOUNDSOBJ* PREV;
  TEXSETIMMBOUNDSOBJ* THIS;
  TEXSETIMMBOUNDSOBJ* NEWBOUNDS;
  PBoolean DONE;
  if (FIRST > LAST) COMPILEERROR(str_make(41, "Set bounds must appear in ascending order"));
  RESULT = BOUNDS;
  DONE = 0;
  PREV = PNil;
  THIS = BOUNDS;
  do {
    if (THIS == PNil || LAST + 1 < THIS->FIRST) {
      NEWBOUNDS = malloc(sizeof(TEXSETIMMBOUNDSOBJ));
      NEWBOUNDS->FIRST = FIRST;
      NEWBOUNDS->LAST = LAST;
      NEWBOUNDS->NEXT = THIS;
      if (PREV == PNil) RESULT = NEWBOUNDS;
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
      if (PREV == PNil) RESULT = NEWBOUNDS;
      else PREV->NEXT = NEWBOUNDS;
      DONE = 0;
    }
    else if (FIRST > THIS->FIRST && FIRST <= THIS->LAST + 1 && LAST > THIS->LAST) {
      FIRST = THIS->FIRST;
      NEWBOUNDS = THIS->NEXT;
      free(THIS);
      THIS = NEWBOUNDS;
      if (PREV == PNil) RESULT = NEWBOUNDS;
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

PBoolean EXISIMMEDIATE(TEXPRESSIONOBJ* EXPR) {
  PBoolean RESULT;
  RESULT = EXPR->CLS == XCIMMEDIATE;
  return RESULT;
}

PBoolean EXISIMMEDIATEOFCLASS(TEXPRESSIONOBJ* EXPR, TEXIMMEDIATECLASS CLS) {
  PBoolean RESULT;
  RESULT = EXPR->CLS == XCIMMEDIATE && EXPR->IMMEDIATE.CLS == CLS;
  return RESULT;
}

TEXPRESSIONOBJ* EXSET() {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(XICSET);
  RESULT->IMMEDIATE.SETBOUNDS = PNil;
  RESULT->IMMEDIATE.SETOFTYPEPTR = PNil;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTEMPTYSET;
  return RESULT;
}

TEXPRESSIONOBJ* EXSETADDRANGE(TEXPRESSIONOBJ* SETEXPR, TEXPRESSIONOBJ* FIRST, TEXPRESSIONOBJ* LAST) {
  TEXPRESSIONOBJ* RESULT;
  TPSTYPE* ELEMENTTYPEPTR;
  TEXPRESSIONOBJ* IMMSET;
  TEXPRESSIONOBJ* EXPRSET;
  TEXSETEXPRBOUNDSOBJ* NEWBOUNDS;
  ELEMENTTYPEPTR = SETEXPR->TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMENTTYPEPTR == PNil) {
    ELEMENTTYPEPTR = GETFUNDAMENTALTYPE(FIRST->TYPEPTR);
    SETEXPR->IMMEDIATE.SETOFTYPEPTR = ELEMENTTYPEPTR;
    SETEXPR->TYPEPTR = MAKESETTYPE(FIRST->TYPEPTR);
  }
  if (!ISFUNDAMENTALLYSAMETYPE(FIRST->TYPEPTR, ELEMENTTYPEPTR)) {
    PString tmp1 = CONCAT(CpLenPtr, 26, "Cannot add element to set ", CpEnd | CpString, ERRORDESCRIBEEXPR(SETEXPR));
    ERRORFOREXPR(&tmp1, FIRST);
  }
  if (LAST != PNil && !ISFUNDAMENTALLYSAMETYPE(LAST->TYPEPTR, ELEMENTTYPEPTR) && !ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) {
    PString tmp2 = CONCAT(CpLenPtr, 26, "Cannot add element to set ", CpEnd | CpString, ERRORDESCRIBEEXPR(SETEXPR));
    ERRORFOREXPR(&tmp2, LAST);
  }
  if (EXISIMMEDIATE(SETEXPR)) {
    IMMSET = SETEXPR;
    EXPRSET = PNil;
  }
  else {
    IMMSET = SETEXPR->SETBASE;
    EXPRSET = SETEXPR;
  }
  if (EXISIMMEDIATE(FIRST) && LAST == PNil) LAST = EXCOPY(FIRST);
  if (EXISIMMEDIATE(FIRST) && EXISIMMEDIATE(LAST)) {
    {
      TEXIMMEDIATE *with3 = &IMMSET->IMMEDIATE;
      with3->SETBOUNDS = EXSETADDBOUNDS(with3->SETBOUNDS, EXGETORDINAL(FIRST), EXGETORDINAL(LAST));
    }
    EXDISPOSE(&FIRST);
    EXDISPOSE(&LAST);
  }
  else {
    if (EXPRSET == PNil) {
      EXPRSET = _NEWEXPR(XCSET);
      EXPRSET->SETBASE = IMMSET;
      EXPRSET->SETBOUNDS = PNil;
      EXPRSET->TYPEPTR = IMMSET->TYPEPTR;
    }
    NEWBOUNDS = malloc(sizeof(TEXSETEXPRBOUNDSOBJ));
    NEWBOUNDS->FIRST = FIRST;
    NEWBOUNDS->LAST = LAST;
    NEWBOUNDS->NEXT = EXPRSET->SETBOUNDS;
    EXPRSET->SETBOUNDS = NEWBOUNDS;
  }
  if (EXPRSET != PNil) RESULT = EXPRSET;
  else RESULT = IMMSET;
  return RESULT;
}

TEXPRESSIONOBJ* EXTOSTRING(TEXPRESSIONOBJ* PARENT) {
  TEXPRESSIONOBJ* RESULT;
  PString STR;
  ENSURESTRINGYEXPR(PARENT);
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
      RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
    }
  }
  else RESULT = PARENT;
  return RESULT;
}

TEXPRESSIONOBJ* EXTOREAL(TEXPRESSIONOBJ* PARENT) {
  TEXPRESSIONOBJ* RESULT;
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
    RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXWITHTMPVAR(TEXPRESSIONOBJ* TMPVAR, TEXPRESSIONOBJ* VALUE, TEXPRESSIONOBJ* CHILD) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCWITHTMPVAR);
  RESULT->TMPVAR = TMPVAR;
  RESULT->TMPVARVALUE = VALUE;
  RESULT->TMPVARCHILD = CHILD;
  RESULT->TYPEPTR = CHILD->TYPEPTR;
  RESULT->ISASSIGNABLE = CHILD->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = CHILD->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = CHILD->ISFUNCTIONRESULT;
  TMPVAR->VARPTR->WASINITIALIZED = 1;
  TMPVAR->VARPTR->WASUSED = 1;
  return RESULT;
}

TEXPRESSIONOBJ* EXSUBRANGE(TEXPRESSIONOBJ* PARENT, TPSTYPE* TYPEPTR);

TEXPRESSIONOBJ* EXOUTRANGE(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* TMPEXPR;
  while (EXPR->CLS == XCSUBRANGE) {
    TMPEXPR = EXCOPY(EXPR->SUBRANGEPARENT);
    EXDISPOSE(&EXPR);
    EXPR = TMPEXPR;
  }
  EXPR->TYPEPTR = GETFUNDAMENTALTYPE(EXPR->TYPEPTR);
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* EXVARIABLE(TPSVARIABLE* VARPTR) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCVARIABLE);
  RESULT->VARPTR = VARPTR;
  RESULT->TYPEPTR = VARPTR->TYPEPTR;
  RESULT->ISASSIGNABLE = !VARPTR->ISCONSTANT;
  RESULT->ISADDRESSABLE = 1;
  return RESULT;
}

TEXPRESSIONOBJ* EXFIELDACCESS(TEXPRESSIONOBJ* PARENT, PInteger FIELDNUM) {
  TEXPRESSIONOBJ* RESULT;
  ENSURERECORDEXPR(PARENT);
  if (FIELDNUM < 1 || FIELDNUM > PARENT->TYPEPTR->RECPTR->SIZE) COMPILEERROR(CONCAT(CpLenPtr, 23, "Invalid field for type ", CpEnd | CpString, TYPENAME(PARENT->TYPEPTR)));
  RESULT = _NEWEXPR(XCFIELD);
  RESULT->RECEXPR = PARENT;
  RESULT->RECFIELDNUM = FIELDNUM;
  RESULT->TYPEPTR = PARENT->TYPEPTR->RECPTR->FIELDS[subrange(FIELDNUM, 1, 32) - 1].TYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ* EXARRAYACCESS(TEXPRESSIONOBJ* PARENT, TEXPRESSIONOBJ* SUBSCRIPT) {
  TEXPRESSIONOBJ* RESULT;
  ENSUREARRAYEXPR(PARENT);
  RESULT = _NEWEXPR(XCARRAY);
  RESULT->ARRAYEXPR = PARENT;
  RESULT->ARRAYINDEX = EXCOERCE(SUBSCRIPT, PARENT->TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  RESULT->TYPEPTR = PARENT->TYPEPTR->ARRAYDEF.VALUETYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ* EXPOINTERACCESS(TEXPRESSIONOBJ* PARENT) {
  TEXPRESSIONOBJ* RESULT;
  ENSUREPOINTEREXPR(PARENT);
  RESULT = _NEWEXPR(XCPOINTER);
  RESULT->POINTEREXPR = PARENT;
  RESULT->TYPEPTR = PARENT->TYPEPTR->POINTEDTYPEPTR;
  RESULT->ISASSIGNABLE = 1;
  RESULT->ISADDRESSABLE = 1;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ* EXADDRESSOF(TEXPRESSIONOBJ* PARENT) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCADDRESS);
  RESULT->ADDRESSEXPR = PARENT;
  if (PARENT->CLS == XCFNREF) RESULT->TYPEPTR = MAKEFUNCTIONTYPE(&PARENT->FNPTR->ARGS, PARENT->FNPTR->RETURNTYPEPTR);
  else {
    ENSUREADDRESSABLEEXPR(PARENT);
    ENSUREASSIGNABLEEXPR(PARENT);
    if (PARENT->CLS != XCVARIABLE) {
      PString tmp1 = str_make(19, "Expected a variable");
      ERRORFOREXPR(&tmp1, PARENT);
    }
    RESULT->TYPEPTR = MAKEPOINTERTYPE(PARENT->TYPEPTR);
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXSTRINGCHAR(TEXPRESSIONOBJ* PARENT, TEXPRESSIONOBJ* SUBSCRIPT) {
  TEXPRESSIONOBJ* RESULT;
  ENSURESTRINGYEXPR(PARENT);
  ENSUREINTEGEREXPR(SUBSCRIPT);
  RESULT = _NEWEXPR(XCSTRINGCHAR);
  RESULT->ARRAYEXPR = EXTOSTRING(PARENT);
  RESULT->ARRAYINDEX = SUBSCRIPT;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTCHAR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ* EXFNREF(TPSFUNCTION* FNPTR) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCFNREF);
  RESULT->FNPTR = FNPTR;
  RESULT->TYPEPTR = PNil;
  return RESULT;
}

TEXPRESSIONOBJ* _EXFUNCTIONCALL(TEXPRESSIONOBJ* FNEXPR, const TPSFNARGS *ARGDEFS, TPSTYPE* RETURNTYPEPTR, const TEXFUNCTIONARGS *ARGS) {
  TEXPRESSIONOBJ* RESULT;
  PInteger POS;
  TEXPRESSIONOBJ* FNCALL;
  if (ARGS->SIZE != ARGDEFS->COUNT) COMPILEERROR(CONCAT(CpLenPtr, 37, "Wrong number of arguments in call to ", CpEnd | CpString, EXDESCRIBE(FNEXPR)));
  FNCALL = _NEWEXPR(XCFNCALL);
  FNCALL->FNEXPR = FNEXPR;
  FNCALL->CALLARGS.SIZE = ARGS->SIZE;
  FNCALL->TYPEPTR = RETURNTYPEPTR;
  FNCALL->ISFUNCTIONRESULT = 1;
  RESULT = FNCALL;
  do {
    PInteger first = 1;
    PInteger last = ARGS->SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1] = EXCOERCE(ARGS->VALUES[subrange(POS, 1, 16) - 1], ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR);
          if (ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE) {
            if (!FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]->ISADDRESSABLE) {
              if (ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT) {
                RESULT = EXWITHTMPVAR(EXVARIABLE(({ PString tmp1 = str_make(3, "tmp"); ADDTMPVARIABLE(&tmp1, ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR); })), FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1], RESULT);
                FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1] = EXCOPY(RESULT->TMPVAR);
              }
              else COMPILEERROR(CONCAT(CpLenPtr, 47, "Pass-by-reference argument must be assignable: ", CpEnd | CpString, EXDESCRIBE(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1])));
            }
            else if (!ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT) {
              ENSUREASSIGNABLEEXPR(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
              EXMARKINITIALIZED(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
            }
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  return RESULT;
}

TEXPRESSIONOBJ* EXFUNCTIONCALL(TEXPRESSIONOBJ* FNEXPR, const TEXFUNCTIONARGS *ARGS) {
  TEXPRESSIONOBJ* RESULT;
  if (FNEXPR->CLS == XCFNREF) {
    FNEXPR->FNPTR->WASUSED = 1;
    RESULT = _EXFUNCTIONCALL(FNEXPR, &FNEXPR->FNPTR->ARGS, FNEXPR->FNPTR->RETURNTYPEPTR, ARGS);
  }
  else if (ISFUNCTIONTYPE(FNEXPR->TYPEPTR)) RESULT = _EXFUNCTIONCALL(FNEXPR, &FNEXPR->TYPEPTR->FNDEFPTR->ARGS, FNEXPR->TYPEPTR->FNDEFPTR->RETURNTYPEPTR, ARGS);
  else {
    PString tmp1 = str_make(24, "Cannot call non-function");
    ERRORFOREXPR(&tmp1, FNEXPR);
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXPSEUDOFN(TPSPSEUDOFN* SPECIALFN) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCPSEUDOFNREF);
  RESULT->PSEUDOFNPTR = SPECIALFN;
  return RESULT;
}

TEXPRESSIONOBJ* EXPSEUDOFNCALL(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  TPSPSEUDOFN* FN;
  if (EXPR->CLS != XCPSEUDOFNREF) INTERNALERROR(CONCAT(CpLenPtr, 31, "Expected a pseudofunction, got ", CpEnd | CpString, EXDESCRIBE(EXPR)));
  FN = EXPR->PSEUDOFNPTR;
  EXPR->CLS = XCPSEUDOFNCALL;
  EXPR->PSEUDOFNCALL.PSEUDOFNPTR = FN;
  EXPR->PSEUDOFNCALL.ARG1 = PNil;
  EXPR->PSEUDOFNCALL.ARG2 = PNil;
  EXPR->PSEUDOFNCALL.ARG3 = PNil;
  EXPR->PSEUDOFNCALL.ARG4 = PNil;
  EXPR->PSEUDOFNCALL.TYPEARG = PNil;
  EXPR->PSEUDOFNCALL.READARGS = PNil;
  EXPR->PSEUDOFNCALL.WRITEARGS = PNil;
  RESULT = EXPR;
  return RESULT;
}

PInteger EXGETORDINAL(TEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  if (!EXISIMMEDIATE(EXPR)) {
    PString tmp1 = str_make(27, "Expected an immediate value");
    ERRORFOREXPR(&tmp1, EXPR);
  }
  {
    TEXIMMEDIATE *with2 = &EXPR->IMMEDIATE;
    switch (with2->CLS) {
      case XICBOOLEAN:
        RESULT = with2->BOOLEANVAL;
        break;
      case XICINTEGER:
        RESULT = with2->INTEGERVAL;
        break;
      case XICCHAR:
        RESULT = (int)with2->CHARVAL;
        break;
      case XICENUM:
        RESULT = with2->ENUMORDINAL;
        break;
      default:
        {
          PString tmp3 = str_make(19, "Expected an ordinal");
          ERRORFOREXPR(&tmp3, EXPR);
        }
        break;
    }
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXGETANTIORDINAL(PInteger ORDINAL, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
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
      INTERNALERROR(CONCAT(CpLenPtr, 32, "Cannot compute anti-ordinal for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

TEXPRESSIONOBJ* EXSUBRANGE(TEXPRESSIONOBJ* PARENT, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
  PInteger ORDINAL;
  if (EXISIMMEDIATE(PARENT)) {
    ORDINAL = EXGETORDINAL(PARENT);
    if (ORDINAL < TYPEPTR->RANGEDEF.FIRST || ORDINAL > TYPEPTR->RANGEDEF.LAST) COMPILEERROR(CONCAT(CpLenPtr, 6, "Value ", CpString, EXDESCRIBE(PARENT), CpLenPtr, 19, " out of bounds for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
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

TEXPRESSIONOBJ* EXRERANGE(TEXPRESSIONOBJ* EXPR, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
  if (TYPEPTR->RANGEDEF.FIRST <= EXPR->TYPEPTR->RANGEDEF.FIRST && EXPR->TYPEPTR->RANGEDEF.LAST <= TYPEPTR->RANGEDEF.LAST) {
    EXPR->TYPEPTR = TYPEPTR;
    RESULT = EXPR;
  }
  else RESULT = EXSUBRANGE(EXOUTRANGE(EXPR), TYPEPTR);
  return RESULT;
}

void EXSETCOERCETOCOMMON(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TPSTYPE* LEFTTYPE;
  TPSTYPE* RIGHTTYPE;
  LEFTTYPE = LEFT->TYPEPTR;
  RIGHTTYPE = RIGHT->TYPEPTR;
  if (LEFTTYPE->ELEMENTTYPEPTR == PNil || EXISIMMEDIATE(LEFT)) LEFTTYPE->ELEMENTTYPEPTR = RIGHTTYPE->ELEMENTTYPEPTR;
  else if (RIGHTTYPE->ELEMENTTYPEPTR == PNil || EXISIMMEDIATE(RIGHT)) RIGHTTYPE->ELEMENTTYPEPTR = LEFTTYPE->ELEMENTTYPEPTR;
  else if (!ISSAMETYPE(LEFTTYPE, RIGHTTYPE)) COMPILEERROR(CONCAT(CpLenPtr, 30, "Type mismatch: cannot combine ", CpString, TYPENAME(LEFT->TYPEPTR), CpLenPtr, 6, " with ", CpEnd | CpString, TYPENAME(RIGHT->TYPEPTR)));
}

TEXPRESSIONOBJ* _EXCOERCESET(TEXPRESSIONOBJ* EXPR, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
  enum __attribute__((__packed__)) enum9 { PASS, REJECT, REPLACE } OUTCOME;
  TPSTYPE* EXPRELEMTYPE;
  TPSTYPE* DESTELEMTYPE;
  const char* enumvalues9[] = { "PASS", "REJECT", "REPLACE" };
  EXPRELEMTYPE = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  DESTELEMTYPE = TYPEPTR->ELEMENTTYPEPTR;
  if (EXPRELEMTYPE == PNil) OUTCOME = REPLACE;
  else if (!ISFUNDAMENTALLYSAMETYPE(EXPRELEMTYPE, DESTELEMTYPE)) OUTCOME = REJECT;
  else if (EXISIMMEDIATE(EXPR)) OUTCOME = REPLACE;
  else if (EXPR->CLS == XCSET) OUTCOME = REPLACE;
  else if (GETTYPELOWBOUND(EXPRELEMTYPE) == GETTYPELOWBOUND(DESTELEMTYPE) && GETTYPEHIGHBOUND(EXPRELEMTYPE) == GETTYPEHIGHBOUND(DESTELEMTYPE)) OUTCOME = PASS;
  else OUTCOME = REJECT;
  switch (OUTCOME) {
    case REJECT:
      {
        PString tmp1 = CONCAT(CpLenPtr, 20, "Cannot treat set as ", CpEnd | CpString, TYPENAME(TYPEPTR));
        ERRORFOREXPR(&tmp1, EXPR);
      }
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

TEXPRESSIONOBJ* EXCOERCE(TEXPRESSIONOBJ* EXPR, TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* RESULT;
  if (ISFUNDAMENTALLYSAMETYPE(EXPR->TYPEPTR, TYPEPTR)) {
    if (ISRANGETYPE(EXPR->TYPEPTR) && ISRANGETYPE(TYPEPTR)) RESULT = EXRERANGE(EXPR, TYPEPTR);
    else if (ISRANGETYPE(EXPR->TYPEPTR)) RESULT = EXOUTRANGE(EXPR);
    else if (ISRANGETYPE(TYPEPTR)) RESULT = EXSUBRANGE(EXPR, TYPEPTR);
    else RESULT = EXPR;
  }
  else if (ISCHARTYPE(EXPR->TYPEPTR) && ISSTRINGTYPE(TYPEPTR)) RESULT = EXTOSTRING(EXPR);
  else if (ISINTEGERTYPE(EXPR->TYPEPTR) && ISREALTYPE(TYPEPTR)) RESULT = EXTOREAL(EXPR);
  else if (ISNILTYPE(EXPR->TYPEPTR) && ISPOINTERYTYPE(TYPEPTR)) RESULT = EXPR;
  else if (ISNILTYPE(EXPR->TYPEPTR) && ISFUNCTIONYTYPE(TYPEPTR)) RESULT = EXPR;
  else if (ISSETTYPE(EXPR->TYPEPTR) && ISSETTYPE(TYPEPTR)) RESULT = _EXCOERCESET(EXPR, TYPEPTR);
  else {
    PString tmp1 = CONCAT(CpLenPtr, 22, "Cannot treat value as ", CpEnd | CpString, TYPENAME(TYPEPTR));
    ERRORFOREXPR(&tmp1, EXPR);
  }
  return RESULT;
}

void EXMARKINITIALIZED(TEXPRESSIONOBJ* LHS) {
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

TEXPRESSIONOBJ* EXOPNEG(TEXPRESSIONOBJ* EXPR);
TEXPRESSIONOBJ* EXOPADD(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPSUB(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPMUL(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPDIVREAL(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPDIVINT(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPMOD(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPNOT(TEXPRESSIONOBJ* EXPR);
TEXPRESSIONOBJ* EXOPAND(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPOR(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPXOR(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPSHL(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPSHR(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPIN(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPEQ(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPNE(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPLT(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPGT(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPLTEQ(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);
TEXPRESSIONOBJ* EXOPGTEQ(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT);

TEXPRESSIONOBJ* _EXOP_MAKEUNARY(TEXPRESSIONOBJ* EXPR, TEXOPERATOR OP, TPSTYPE* RESULTTYPE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCUNARYOP);
  RESULT->UNARY.PARENT = EXPR;
  RESULT->UNARY.OP = OP;
  RESULT->TYPEPTR = RESULTTYPE;
  RESULT->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ* _EXOP_MAKEBINARY(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT, TEXOPERATOR OP, TPSTYPE* RESULTTYPE) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(XCBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->TYPEPTR = RESULTTYPE;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPADD_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.INTEGERVAL == 0) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == 0) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL + RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOADD, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPSUB_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.INTEGERVAL == 0) {
    EXDISPOSE(&LEFT);
    RESULT = EXOPNEG(RIGHT);
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == 0) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL - RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOSUB, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPMUL_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.INTEGERVAL == 1) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.INTEGERVAL == -1) {
    EXDISPOSE(&LEFT);
    RESULT = EXOPNEG(RIGHT);
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == 1) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == -1) {
    EXDISPOSE(&RIGHT);
    RESULT = EXOPNEG(LEFT);
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL * RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOMUL, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPDIV_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == 1) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == -1) {
    EXDISPOSE(&RIGHT);
    RESULT = EXOPNEG(LEFT);
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL / RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XODIVINT, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPMOD_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL % RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOMOD, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPADD_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.REALVAL ==  0.00000000000000E+000) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL ==  0.00000000000000E+000) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.REALVAL = LEFT->IMMEDIATE.REALVAL + RIGHT->IMMEDIATE.REALVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOADD, PRIMITIVETYPES.PTREAL);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPSUB_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.REALVAL ==  0.00000000000000E+000) {
    EXDISPOSE(&LEFT);
    RESULT = EXOPNEG(RIGHT);
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL ==  0.00000000000000E+000) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.REALVAL = LEFT->IMMEDIATE.REALVAL - RIGHT->IMMEDIATE.REALVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOSUB, PRIMITIVETYPES.PTREAL);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPMUL_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.REALVAL ==  1.00000000000000E+000) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.REALVAL == -1.00000000000000E+000) {
    EXDISPOSE(&LEFT);
    RESULT = EXOPNEG(RIGHT);
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL ==  1.00000000000000E+000) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL == -1.00000000000000E+000) {
    EXDISPOSE(&RIGHT);
    RESULT = EXOPNEG(LEFT);
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.REALVAL = LEFT->IMMEDIATE.REALVAL * RIGHT->IMMEDIATE.REALVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOMUL, PRIMITIVETYPES.PTREAL);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPDIV_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL ==  1.00000000000000E+000) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL == -1.00000000000000E+000) {
    EXDISPOSE(&RIGHT);
    RESULT = EXOPNEG(LEFT);
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.REALVAL = LEFT->IMMEDIATE.REALVAL / RIGHT->IMMEDIATE.REALVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XODIVREAL, PRIMITIVETYPES.PTREAL);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPADD_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATEOFCLASS(LEFT, XICSTRING) && cmp_str(CoEq, CpStringPtr, &LEFT->IMMEDIATE.STRINGVAL, CpLenPtr, 0, "")) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATEOFCLASS(RIGHT, XICSTRING) && cmp_str(CoEq, CpStringPtr, &RIGHT->IMMEDIATE.STRINGVAL, CpLenPtr, 0, "")) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.STRINGVAL = CONCAT(CpStringPtr, &LT, CpEnd | CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICSTRING;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOADD, PRIMITIVETYPES.PTSTRING);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPUNION_SETS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  TEXSETIMMBOUNDSOBJ* NEWBDS;
  TEXSETIMMBOUNDSOBJ* OLDBDS;
  EXSETCOERCETOCOMMON(LEFT, RIGHT);
  if (EXISIMMEDIATE(LEFT) && LEFT->SETBOUNDS == PNil) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->SETBOUNDS == PNil) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    NEWBDS = PNil;
    OLDBDS = LEFT->IMMEDIATE.SETBOUNDS;
    while (OLDBDS != PNil) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, OLDBDS->FIRST, OLDBDS->LAST);
      OLDBDS = OLDBDS->NEXT;
    }
    OLDBDS = RIGHT->IMMEDIATE.SETBOUNDS;
    while (OLDBDS != PNil) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, OLDBDS->FIRST, OLDBDS->LAST);
      OLDBDS = OLDBDS->NEXT;
    }
    RESULT = EXSETCONSTANT(NEWBDS, LEFT->TYPEPTR);
    EXDISPOSE(&LEFT);
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOADD, LEFT->TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPDIFFERENCE_SETS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  TEXSETIMMBOUNDSOBJ* LTBDS;
  TEXSETIMMBOUNDSOBJ* RTBDS;
  TEXSETIMMBOUNDSOBJ* NEWBDS;
  EXSETCOERCETOCOMMON(LEFT, RIGHT);
  if (EXISIMMEDIATE(LEFT) && LEFT->SETBOUNDS == PNil) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->SETBOUNDS == PNil) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
    RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
    NEWBDS = PNil;
    while (LTBDS != PNil && RTBDS != PNil) {
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
    while (LTBDS != PNil) {
      NEWBDS = EXSETADDBOUNDS(NEWBDS, LTBDS->FIRST, LTBDS->LAST);
      LTBDS = LTBDS->NEXT;
    }
    RESULT = EXSETCONSTANT(NEWBDS, LEFT->TYPEPTR);
    EXDISPOSE(&LEFT);
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOSUB, LEFT->TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPINTERSECTION_SETS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  TEXSETIMMBOUNDSOBJ* LTBDS;
  TEXSETIMMBOUNDSOBJ* RTBDS;
  TEXSETIMMBOUNDSOBJ* NEWBDS;
  EXSETCOERCETOCOMMON(LEFT, RIGHT);
  if (EXISIMMEDIATE(LEFT) && LEFT->SETBOUNDS == PNil) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(RIGHT) && RIGHT->SETBOUNDS == PNil) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
    RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
    NEWBDS = PNil;
    while (LTBDS != PNil && RTBDS != PNil) {
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
    EXDISPOSE(&LEFT);
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOMUL, LEFT->TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPNEG(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  ENSURENUMERICEXPR(EXPR);
  if (EXISIMMEDIATE(EXPR)) {
    {
      TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
      if (with1->CLS == XICINTEGER) with1->INTEGERVAL = -with1->INTEGERVAL;
      else with1->REALVAL = -with1->REALVAL;
    }
    RESULT = EXPR;
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, XONEG, EXPR->TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPADD(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPADD_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPADD_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPADD_STRINGS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPUNION_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOADD);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPSUB(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPSUB_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPSUB_NUMBERS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPDIFFERENCE_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOSUB);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPMUL(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPMUL_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPMUL_NUMBERS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPINTERSECTION_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOMUL);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPDIVREAL(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPDIV_NUMBERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XODIVREAL);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPDIVINT(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPDIV_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XODIVINT);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPMOD(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPMOD_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOMOD);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPSHL_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL << RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOSHL, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPSHR_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL >> RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOSHR, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPSHL(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPSHL_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOSHL);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPSHR(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPSHR_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOSHR);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNOT_BOOLEAN(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = EXPR;
  if (EXISIMMEDIATE(EXPR)) RESULT->IMMEDIATE.BOOLEANVAL = !EXPR->IMMEDIATE.BOOLEANVAL;
  else if (EXPR->CLS == XCUNARYOP && EXPR->UNARY.OP == XONOT) {
    RESULT = EXCOPY(EXPR->UNARY.PARENT);
    EXDISPOSE(&EXPR);
  }
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOEQ) RESULT->BINARY.OP = XONE;
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XONE) RESULT->BINARY.OP = XOEQ;
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOLT) RESULT->BINARY.OP = XOGTEQ;
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOGT) RESULT->BINARY.OP = XOLTEQ;
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOLTEQ && !ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR) && !ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR)) RESULT->BINARY.OP = XOGT;
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOGTEQ && !ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR) && !ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR)) RESULT->BINARY.OP = XOLT;
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOAND && ISBOOLEANTYPE(EXPR->TYPEPTR)) {
    RESULT = EXOPOR(EXOPNOT(EXCOPY(EXPR->BINARY.LEFT)), EXOPNOT(EXCOPY(EXPR->BINARY.RIGHT)));
    EXDISPOSE(&EXPR);
  }
  else if (EXPR->CLS == XCBINARYOP && EXPR->BINARY.OP == XOOR && ISBOOLEANTYPE(EXPR->TYPEPTR)) {
    RESULT = EXOPAND(EXOPNOT(EXCOPY(EXPR->BINARY.LEFT)), EXOPNOT(EXCOPY(EXPR->BINARY.RIGHT)));
    EXDISPOSE(&EXPR);
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, XONOT, EXPR->TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPAND_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.BOOLEANVAL == 0) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.BOOLEANVAL == 1) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL && RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOAND, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPOR_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.BOOLEANVAL == 1) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.BOOLEANVAL == 0) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL || RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOOR, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPXOR_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL != RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOXOR, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNOT_INTEGER(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(EXPR)) {
    EXPR->IMMEDIATE.INTEGERVAL = ~EXPR->IMMEDIATE.INTEGERVAL;
    RESULT = EXPR;
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, XONOT, EXPR->TYPEPTR);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPAND_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL & RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOAND, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPOR_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL | RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOOR, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPXOR_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL ^ RIGHT->IMMEDIATE.INTEGERVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOXOR, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPNOT(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = _EXOPNOT_BOOLEAN(EXPR);
  else if (ISINTEGERTYPE(EXPR->TYPEPTR)) RESULT = _EXOPNOT_INTEGER(EXPR);
  else ERRORINVALIDOPERATOR(EXPR, XONOT);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPAND(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPAND_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPAND_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOADD);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPOR(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPOR_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPOR_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOOR);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPXOR(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPXOR_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPXOR_INTEGERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOXOR);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL == RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNE_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL != RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLT_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL < RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGT_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL > RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLTEQ_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL <= RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGTEQ_BOOLEANS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL >= RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.INTEGERVAL == RIGHT->IMMEDIATE.INTEGERVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNE_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.INTEGERVAL != RIGHT->IMMEDIATE.INTEGERVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLT_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.INTEGERVAL < RIGHT->IMMEDIATE.INTEGERVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGT_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.INTEGERVAL > RIGHT->IMMEDIATE.INTEGERVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLTEQ_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.INTEGERVAL <= RIGHT->IMMEDIATE.INTEGERVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGTEQ_INTEGERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.INTEGERVAL >= RIGHT->IMMEDIATE.INTEGERVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.REALVAL == RIGHT->IMMEDIATE.REALVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNE_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.REALVAL != RIGHT->IMMEDIATE.REALVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLT_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.REALVAL < RIGHT->IMMEDIATE.REALVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGT_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.REALVAL > RIGHT->IMMEDIATE.REALVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLTEQ_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.REALVAL <= RIGHT->IMMEDIATE.REALVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGTEQ_NUMBERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.REALVAL >= RIGHT->IMMEDIATE.REALVAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = cmp_str(CoEq, CpStringPtr, &LT, CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNE_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = cmp_str(CoNotEq, CpStringPtr, &LT, CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLT_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = cmp_str(CoBefore, CpStringPtr, &LT, CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGT_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = cmp_str(CoAfter, CpStringPtr, &LT, CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLTEQ_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = cmp_str(CoBeforeOrEq, CpStringPtr, &LT, CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGTEQ_STRINGS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == XICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == XICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = cmp_str(CoAfterOrEq, CpStringPtr, &LT, CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_ENUMS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.ENUMORDINAL == RIGHT->IMMEDIATE.ENUMORDINAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNE_ENUMS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.ENUMORDINAL != RIGHT->IMMEDIATE.ENUMORDINAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLT_ENUMS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.ENUMORDINAL < RIGHT->IMMEDIATE.ENUMORDINAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGT_ENUMS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.ENUMORDINAL > RIGHT->IMMEDIATE.ENUMORDINAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGT, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPLTEQ_ENUMS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.ENUMORDINAL <= RIGHT->IMMEDIATE.ENUMORDINAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOLTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGTEQ_ENUMS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.ENUMORDINAL >= RIGHT->IMMEDIATE.ENUMORDINAL;
    RESULT->IMMEDIATE.CLS = XICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_SETS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  TEXSETIMMBOUNDSOBJ* LTBDS;
  TEXSETIMMBOUNDSOBJ* RTBDS;
  PBoolean EQUALS;
  EXSETCOERCETOCOMMON(LEFT, RIGHT);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
    RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
    EQUALS = 1;
    while (LTBDS != PNil && RTBDS != PNil && EQUALS) {
      EQUALS = RTBDS->FIRST == LTBDS->FIRST && RTBDS->LAST == LTBDS->LAST;
      LTBDS = LTBDS->NEXT;
      RTBDS = RTBDS->NEXT;
    }
    EQUALS = EQUALS && LTBDS == PNil && RTBDS == PNil;
    RESULT = EXBOOLEANCONSTANT(EQUALS);
    EXDISPOSE(&LEFT);
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPGTEQ_SETS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  TEXSETIMMBOUNDSOBJ* LTBDS;
  TEXSETIMMBOUNDSOBJ* RTBDS;
  PBoolean ISGTEQ;
  EXSETCOERCETOCOMMON(LEFT, RIGHT);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LTBDS = LEFT->IMMEDIATE.SETBOUNDS;
    RTBDS = RIGHT->IMMEDIATE.SETBOUNDS;
    ISGTEQ = 1;
    while (LTBDS != PNil && RTBDS != PNil && ISGTEQ) {
      if (LTBDS->LAST < RTBDS->FIRST) LTBDS = LTBDS->NEXT;
      else if (LTBDS->FIRST <= RTBDS->FIRST && LTBDS->LAST >= RTBDS->LAST) RTBDS = RTBDS->NEXT;
      else ISGTEQ = 0;
    }
    ISGTEQ = ISGTEQ && RTBDS == PNil;
    RESULT = EXBOOLEANCONSTANT(ISGTEQ);
    EXDISPOSE(&LEFT);
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPIN_IMPL(TEXPRESSIONOBJ* NEEDLE, TEXPRESSIONOBJ* HAYSTACK) {
  TEXPRESSIONOBJ* RESULT;
  TPSTYPE* ELEMTYPE;
  TEXPRESSIONOBJ* IMMSET;
  TEXPRESSIONOBJ* EXPRSET;
  TEXSETIMMBOUNDSOBJ* IMMBOUNDS;
  TEXSETEXPRBOUNDSOBJ* EXPRBOUNDS;
  TEXPRESSIONOBJ* COND;
  TPSVARIABLE* TMPVAR;
  TEXPRESSIONOBJ* WANTED;
  ELEMTYPE = HAYSTACK->TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMTYPE == PNil) ELEMTYPE = NEEDLE->TYPEPTR;
  else NEEDLE = EXCOERCE(NEEDLE, ELEMTYPE);
  if (NEEDLE->ISFUNCTIONRESULT) {
    TMPVAR = ({ PString tmp1 = str_make(4, "elem"); ADDTMPVARIABLE(&tmp1, ELEMTYPE); });
    WANTED = EXVARIABLE(TMPVAR);
  }
  else {
    TMPVAR = PNil;
    WANTED = NEEDLE;
  }
  RESULT = EXBOOLEANCONSTANT(0);
  if (EXISIMMEDIATE(HAYSTACK)) {
    IMMSET = HAYSTACK;
    EXPRSET = PNil;
  }
  else {
    IMMSET = HAYSTACK->SETBASE;
    EXPRSET = HAYSTACK;
  }
  if (IMMSET != PNil) {
    IMMBOUNDS = IMMSET->IMMEDIATE.SETBOUNDS;
    while (IMMBOUNDS != PNil) {
      if (IMMBOUNDS->FIRST == IMMBOUNDS->LAST) COND = EXOPEQ(EXCOPY(WANTED), EXGETANTIORDINAL(IMMBOUNDS->FIRST, ELEMTYPE));
      else COND = EXOPAND(EXOPLTEQ(EXGETANTIORDINAL(IMMBOUNDS->FIRST, ELEMTYPE), EXCOPY(WANTED)), EXOPLTEQ(EXCOPY(WANTED), EXGETANTIORDINAL(IMMBOUNDS->LAST, ELEMTYPE)));
      RESULT = EXOPOR(RESULT, COND);
      IMMBOUNDS = IMMBOUNDS->NEXT;
    }
  }
  if (EXPRSET != PNil) {
    EXPRBOUNDS = EXPRSET->SETBOUNDS;
    while (EXPRBOUNDS != PNil) {
      if (EXPRBOUNDS->LAST == PNil) COND = EXOPEQ(EXCOPY(WANTED), EXCOPY(EXPRBOUNDS->FIRST));
      else COND = EXOPAND(EXOPLTEQ(EXCOPY(EXPRBOUNDS->FIRST), EXCOPY(WANTED)), EXOPLTEQ(EXCOPY(WANTED), EXCOPY(EXPRBOUNDS->LAST)));
      RESULT = EXOPOR(RESULT, COND);
      EXPRBOUNDS = EXPRBOUNDS->NEXT;
    }
  }
  if (TMPVAR != PNil) {
    RESULT = EXWITHTMPVAR(WANTED, NEEDLE, RESULT);
  }
  else EXDISPOSE(&NEEDLE);
  EXDISPOSE(&HAYSTACK);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPEQ_POINTERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* _EXOPNE_POINTERS(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPIN(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISSETTYPE(RIGHT->TYPEPTR)) {
    if (EXISIMMEDIATE(RIGHT) || RIGHT->CLS == XCSET) RESULT = _EXOPIN_IMPL(LEFT, RIGHT);
    else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, XOIN, PRIMITIVETYPES.PTBOOLEAN);
  }
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOXOR);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPEQ(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPEQ_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPEQ_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPEQ_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPEQ_STRINGS(LEFT, RIGHT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPEQ_ENUMS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPEQ_SETS(LEFT, RIGHT);
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPEQ_POINTERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOEQ);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPNE(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPNE_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPNE_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPNE_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPNE_STRINGS(LEFT, RIGHT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPNE_ENUMS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = EXOPNOT(_EXOPEQ_SETS(LEFT, RIGHT));
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPNE_POINTERS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XONE);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPLT(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLT_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLT_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLT_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLT_STRINGS(LEFT, RIGHT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPLT_ENUMS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOLT);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPGT(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGT_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGT_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGT_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGT_STRINGS(LEFT, RIGHT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPGT_ENUMS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOGT);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPLTEQ(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLTEQ_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLTEQ_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLTEQ_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLTEQ_STRINGS(LEFT, RIGHT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPLTEQ_ENUMS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_SETS(RIGHT, LEFT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOLTEQ);
  return RESULT;
}

TEXPRESSIONOBJ* EXOPGTEQ(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT) {
  TEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_BOOLEANS(LEFT, RIGHT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_INTEGERS(LEFT, RIGHT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_NUMBERS(LEFT, RIGHT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_STRINGS(LEFT, RIGHT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_ENUMS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, XOGTEQ);
  return RESULT;
}

void READTOKEN();

void WANTTOKEN(TLXTOKENID ID) {
  if (LEXER.TOKEN.ID != ID) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
}

void WANTTOKEN2(TLXTOKENID ID1, TLXTOKENID ID2) {
  if (LEXER.TOKEN.ID != ID1 && LEXER.TOKEN.ID != ID2) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID1), CpLenPtr, 4, " or ", CpString, LXTOKENNAME(ID2), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
}

void WANTTOKEN3(TLXTOKENID ID1, TLXTOKENID ID2, TLXTOKENID ID3) {
  if (LEXER.TOKEN.ID != ID1 && LEXER.TOKEN.ID != ID2 && LEXER.TOKEN.ID != ID3) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID1), CpLenPtr, 2, ", ", CpString, LXTOKENNAME(ID2), CpLenPtr, 5, ", or ", CpString, LXTOKENNAME(ID3), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
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

TPSTYPE* PSTYPEDENOTER();

TPSTYPE* PSTYPEIDENTIFIER() {
  TPSTYPE* RESULT;
  WANTTOKEN(TKIDENTIFIER);
  RESULT = FINDNAMEOFCLASS(&LEXER.TOKEN.VALUE, TNCTYPE, 1)->TYPEPTR;
  RESULT->WASUSED = 1;
  READTOKEN();
  return RESULT;
}

TPSIDENTIFIER PSIDENTIFIER() {
  TPSIDENTIFIER RESULT;
  RESULT.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  return RESULT;
}

TEXPRESSIONOBJ* PSEXPRESSION();

TEXPRESSIONOBJ* PSIMMEDIATE() {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* EXPR;
  EXPR = PSEXPRESSION();
  if (!EXISIMMEDIATE(EXPR)) {
    PString tmp1 = str_make(32, "Expected an immediate expression");
    ERRORFOREXPR(&tmp1, EXPR);
  }
  RESULT = EXPR;
  return RESULT;
}

TPSTYPE* PSENUMERATEDTYPE() {
  TPSTYPE* RESULT;
  TPSENUMDEF ENUM;
  WANTTOKENANDREAD(TKLPAREN);
  ENUM.SIZE = 0;
  do {
    ENUM.SIZE = ENUM.SIZE + 1;
    if (ENUM.SIZE > 128) COMPILEERROR(str_make(23, "Too many values in enum"));
    ENUM.VALUES[subrange(ENUM.SIZE - 1, 0, 127)] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (LEXER.TOKEN.ID != TKRPAREN);
  WANTTOKENANDREAD(TKRPAREN);
  RESULT = MAKEENUMTYPE(&ENUM);
  return RESULT;
}

void PSRECORDFIELD(TPSRECORDDEF *REC, TLXTOKENID DELIMITER) {
  PString NAME;
  PInteger LASTFIELD;
  PInteger FIELD;
  TPSTYPE* TYPEPTR;
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
            if (cmp_str(CoEq, CpStringPtr, &REC->FIELDS[subrange(FIELD, 1, 32) - 1].NAME, CpStringPtr, &NAME)) COMPILEERROR(CONCAT(CpLenPtr, 14, "A field named ", CpStringPtr, &NAME, CpEnd | CpLenPtr, 25, " has already been defined"));
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
  } while (LEXER.TOKEN.ID != TKCOLON);
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
  TPSTYPE* TAGTYPE;
  TEXPRESSIONOBJ* CASELABEL;
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
  else TAGTYPE = FINDNAMEOFCLASS(&TAG.NAME, TNCTYPE, 1)->TYPEPTR;
  ENSUREORDINALTYPE(TAGTYPE);
  WANTTOKENANDREAD(TKOF);
  do {
    REC->NUMVARIANTS = REC->NUMVARIANTS + 1;
    REC->VARIANTBOUNDS[subrange(REC->NUMVARIANTS, 1, 32) - 1] = REC->SIZE + 1;
    do {
      CASELABEL = EXCOERCE(PSIMMEDIATE(), TAGTYPE);
      EXDISPOSE(&CASELABEL);
      WANTTOKEN2(TKCOMMA, TKCOLON);
      SKIPTOKEN(TKCOMMA);
    } while (LEXER.TOKEN.ID != TKCOLON);
    WANTTOKENANDREAD(TKCOLON);
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) PSRECORDFIELD(REC, TKRPAREN);
    WANTTOKENANDREAD(TKRPAREN);
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  } while (LEXER.TOKEN.ID != TKEND);
}

TPSTYPE* PSRECORDTYPE(PBoolean ISPACKED) {
  TPSTYPE* RESULT;
  TPSRECORDDEF REC;
  WANTTOKENANDREAD(TKRECORD);
  REC.SIZE = 0;
  REC.NUMVARIANTS = 0;
  REC.ISPACKED = ISPACKED;
  while (LEXER.TOKEN.ID != TKCASE && LEXER.TOKEN.ID != TKEND) PSRECORDFIELD(&REC, TKEND);
  if (LEXER.TOKEN.ID == TKCASE) PSRECORDVARIANTS(&REC);
  WANTTOKENANDREAD(TKEND);
  RESULT = MAKERECORDTYPE(&REC);
  return RESULT;
}

void PSARGUMENTS(TPSFNARGS *ARGS) {
  PBoolean ISCONST;
  PBoolean ISVAR;
  PInteger LASTARG;
  PInteger ARG;
  TPSTYPE* TYPEPTR;
  WANTTOKENANDREAD(TKLPAREN);
  ARGS->COUNT = 0;
  do {
    ISCONST = LEXER.TOKEN.ID == TKCONST;
    ISVAR = LEXER.TOKEN.ID == TKVAR;
    if (ISCONST) WANTTOKENANDREAD(TKCONST);
    if (ISVAR) WANTTOKENANDREAD(TKVAR);
    LASTARG = ARGS->COUNT;
    do {
      ARGS->COUNT = ARGS->COUNT + 1;
      if (ARGS->COUNT > 16) COMPILEERROR(str_make(42, "Too many arguments declared for subroutine"));
      ARGS->DEFS[subrange(ARGS->COUNT, 1, 16) - 1].NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
      ARGS->DEFS[subrange(ARGS->COUNT, 1, 16) - 1].ISCONSTANT = ISCONST;
      ARGS->DEFS[subrange(ARGS->COUNT, 1, 16) - 1].ISREFERENCE = ISVAR || ISCONST;
      ARGS->DEFS[subrange(ARGS->COUNT, 1, 16) - 1].WASINITIALIZED = 1;
      WANTTOKEN2(TKCOLON, TKCOMMA);
      SKIPTOKEN(TKCOMMA);
    } while (LEXER.TOKEN.ID != TKCOLON);
    SKIPTOKEN(TKCOLON);
    TYPEPTR = PSTYPEIDENTIFIER();
    do {
      PInteger first = LASTARG + 1;
      PInteger last = ARGS->COUNT;
      if (first <= last) {
        ARG = first;
        while (1) {
          ARGS->DEFS[subrange(ARG, 1, 16) - 1].TYPEPTR = TYPEPTR;
          if (ARG == last) break;
          ++ARG;
        }
      }
    } while(0);
    WANTTOKEN2(TKSEMICOLON, TKRPAREN);
    SKIPTOKEN(TKSEMICOLON);
  } while (LEXER.TOKEN.ID != TKRPAREN);
  SKIPTOKEN(TKRPAREN);
}

TPSTYPE* PSRESULTTYPE() {
  TPSTYPE* RESULT;
  RESULT = PSTYPEIDENTIFIER();
  return RESULT;
}

TPSTYPE* PSPROCEDURETYPE() {
  TPSTYPE* RESULT;
  TPSFNARGS ARGS;
  WANTTOKENANDREAD(TKPROCEDURE);
  if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&ARGS);
  RESULT = MAKEFUNCTIONTYPE(&ARGS, PNil);
  return RESULT;
}

TPSTYPE* PSFUNCTIONTYPE() {
  TPSTYPE* RESULT;
  TPSFNARGS ARGS;
  TPSTYPE* RETURNTYPE;
  WANTTOKENANDREAD(TKFUNCTION);
  WANTTOKEN2(TKLPAREN, TKCOLON);
  if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&ARGS);
  WANTTOKENANDREAD(TKCOLON);
  RETURNTYPE = PSRESULTTYPE();
  RESULT = MAKEFUNCTIONTYPE(&ARGS, RETURNTYPE);
  return RESULT;
}

TPSTYPE* _PSARRAYTYPEINTERNAL() {
  TPSTYPE* RESULT;
  TPSTYPE* INDEXTYPE;
  TPSTYPE* VALUETYPE;
  INDEXTYPE = PSTYPEDENOTER();
  WANTTOKEN2(TKCOMMA, TKRBRACKET);
  if (LEXER.TOKEN.ID == TKCOMMA) {
    WANTTOKENANDREAD(TKCOMMA);
    VALUETYPE = _PSARRAYTYPEINTERNAL();
  }
  else {
    WANTTOKENANDREAD(TKRBRACKET);
    WANTTOKENANDREAD(TKOF);
    VALUETYPE = PSTYPEDENOTER();
  }
  RESULT = MAKEARRAYTYPE(INDEXTYPE, VALUETYPE);
  return RESULT;
}

TPSTYPE* PSARRAYTYPE() {
  TPSTYPE* RESULT;
  WANTTOKENANDREAD(TKARRAY);
  WANTTOKENANDREAD(TKLBRACKET);
  RESULT = _PSARRAYTYPEINTERNAL();
  return RESULT;
}

TPSTYPE* PSPOINTERTYPE() {
  TPSTYPE* RESULT;
  TPSNAME* NAMEPTR;
  WANTTOKENANDREAD(TKCARET);
  WANTTOKEN(TKIDENTIFIER);
  NAMEPTR = FINDNAMEOFCLASS(&LEXER.TOKEN.VALUE, TNCTYPE, 0);
  if (NAMEPTR == PNil) RESULT = MAKEPOINTERUNKNOWNTYPE(&LEXER.TOKEN.VALUE);
  else {
    RESULT = MAKEPOINTERTYPE(NAMEPTR->TYPEPTR);
    NAMEPTR->TYPEPTR->WASUSED = 1;
  }
  READTOKEN();
  return RESULT;
}

TPSTYPE* PSRANGETYPE() {
  TPSTYPE* RESULT;
  TEXPRESSIONOBJ* FIRST;
  TEXPRESSIONOBJ* LAST;
  FIRST = PSIMMEDIATE();
  WANTTOKENANDREAD(TKRANGE);
  LAST = PSIMMEDIATE();
  ENSUREORDINALEXPR(FIRST);
  if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) {
    PString tmp1 = CONCAT(CpLenPtr, 9, "Expected ", CpEnd | CpString, TYPENAME(FIRST->TYPEPTR));
    ERRORFOREXPR(&tmp1, LAST);
  }
  RESULT = MAKERANGETYPE(FIRST->TYPEPTR, EXGETORDINAL(FIRST), EXGETORDINAL(LAST));
  EXDISPOSE(&FIRST);
  EXDISPOSE(&LAST);
  return RESULT;
}

TPSTYPE* PSSETTYPE() {
  TPSTYPE* RESULT;
  TPSTYPE* ELEMENTTYPEPTR;
  WANTTOKENANDREAD(TKSET);
  WANTTOKENANDREAD(TKOF);
  ELEMENTTYPEPTR = PSTYPEDENOTER();
  if (!ISBOUNDEDTYPE(ELEMENTTYPEPTR)) {
    PString tmp1 = str_make(47, "Set element types must be bounded ordinal types");
    ERRORFORTYPE(&tmp1, ELEMENTTYPEPTR);
  }
  if (GETBOUNDEDTYPESIZE(ELEMENTTYPEPTR) > 256) {
    PString tmp2 = str_make(54, "Set element types may not contain more than 256 values");
    ERRORFORTYPE(&tmp2, ELEMENTTYPEPTR);
  }
  RESULT = MAKESETTYPE(ELEMENTTYPEPTR);
  return RESULT;
}

TPSTYPE* PSTYPEDENOTER() {
  TPSTYPE* RESULT;
  TPSNAME* IDX;
  PBoolean ISPACKED;
  RESULT = PNil;
  ISPACKED = LEXER.TOKEN.ID == TKPACKED;
  SKIPTOKEN(TKPACKED);
  if (LEXER.TOKEN.ID == TKLPAREN) RESULT = PSENUMERATEDTYPE();
  else if (LEXER.TOKEN.ID == TKSET) RESULT = PSSETTYPE();
  else if (LEXER.TOKEN.ID == TKRECORD) RESULT = PSRECORDTYPE(ISPACKED);
  else if (LEXER.TOKEN.ID == TKARRAY) RESULT = PSARRAYTYPE();
  else if (LEXER.TOKEN.ID == TKCARET) RESULT = PSPOINTERTYPE();
  else if (LEXER.TOKEN.ID == TKPROCEDURE) RESULT = PSPROCEDURETYPE();
  else if (LEXER.TOKEN.ID == TKFUNCTION) RESULT = PSFUNCTIONTYPE();
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) {
    IDX = FINDNAME(&LEXER.TOKEN.VALUE, 0);
    if (IDX == PNil) ;
    else if (IDX->CLS == TNCTYPE) RESULT = PSTYPEIDENTIFIER();
    else if (TNCCONSTANT <= IDX->CLS && IDX->CLS <= TNCENUMVAL) RESULT = PSRANGETYPE();
  }
  else if (LEXER.TOKEN.ID == TKINTEGER || TKSTRING <= LEXER.TOKEN.ID && LEXER.TOKEN.ID <= TKMINUS) RESULT = PSRANGETYPE();
  if (RESULT == PNil) COMPILEERROR(CONCAT(CpLenPtr, 29, "Expected type denoter, found ", CpEnd | CpString, LXTOKENSTR()));
  return RESULT;
}

void _RESOLVEPOINTERUNKNOWN(TPSTYPE* TYPEPTR) {
  TPSTYPE* TARGETPTR;
  if (ISPOINTERUNKNOWNTYPE(TYPEPTR)) {
    TARGETPTR = FINDNAMEOFCLASS(TYPEPTR->TARGETNAME, TNCTYPE, 1)->TYPEPTR;
    free(TYPEPTR->TARGETNAME);
    TYPEPTR->CLS = TTCPOINTER;
    TYPEPTR->POINTEDTYPEPTR = TARGETPTR;
    TARGETPTR->WASUSED = 1;
  }
}

void PSTYPEDEFINITIONS() {
  PString NAME;
  TPSTYPE* TYPEPTR;
  TPSDEFENTRY* CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKTYPE);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKENANDREAD(TKEQUALS);
    TYPEPTR = PSTYPEDENOTER();
    WANTTOKENANDREAD(TKSEMICOLON);
    MAKEALIASTYPE(&NAME, TYPEPTR);
  } while (LEXER.TOKEN.ID == TKIDENTIFIER);
  OUTTYPEDEFINITIONSFROMCHECKPOINT(CHECKPOINT);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSCONSTANT(const PString *NAME) {
  TPSCONSTANT CONSTANT;
  WANTTOKENANDREAD(TKEQUALS);
  CONSTANT.NAME = *NAME;
  CONSTANT.VALUE = PSIMMEDIATE();
  ADDCONSTANT(&CONSTANT);
}

void PSCONSTANTVALUE(TPSTYPE* TYPEPTR) {
  TEXPRESSIONOBJ* EXPR;
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
    EXDISPOSE(&EXPR);
  }
}

void PSTYPEDCONSTANT(const PString *NAME) {
  TPSTYPE* TYPEPTR;
  WANTTOKENANDREAD(TKCOLON);
  TYPEPTR = PSTYPEDENOTER();
  WANTTOKENANDREAD(TKEQUALS);
  OUTCONSTANTDEFINITIONBEGIN(({ TPSVARIABLE tmp1 = MAKETYPEDCONSTANT(NAME, TYPEPTR); ADDVARIABLE(&tmp1); }));
  PSCONSTANTVALUE(TYPEPTR);
  OUTCONSTANTDEFINITIONEND();
}

void PSCONSTDEFINITIONS() {
  PString NAME;
  TPSDEFENTRY* CHECKPOINT;
  CHECKPOINT = DEFS.LATEST;
  WANTTOKENANDREAD(TKCONST);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKEQUALS, TKCOLON);
    if (LEXER.TOKEN.ID == TKEQUALS) PSCONSTANT(&NAME);
    else PSTYPEDCONSTANT(&NAME);
    WANTTOKENANDREAD(TKSEMICOLON);
  } while (LEXER.TOKEN.ID == TKIDENTIFIER);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSVARDEFINITIONS() {
  PInteger NUMNAMES;
  PString NAMES[8];
  TPSTYPE* TYPEPTR;
  TPSDEFENTRY* CHECKPOINT;
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
    } while (LEXER.TOKEN.ID != TKCOLON);
    WANTTOKENANDREAD(TKCOLON);
    TYPEPTR = PSTYPEDENOTER();
    WANTTOKENANDREAD(TKSEMICOLON);
    do {
      PInteger first = 1;
      PInteger last = NUMNAMES;
      if (first <= last) {
        NUMNAMES = first;
        while (1) {
          OUTVARIABLEDEFINITION(({ TPSVARIABLE tmp1 = MAKEVARIABLE(&NAMES[subrange(NUMNAMES, 1, 8) - 1], TYPEPTR); ADDVARIABLE(&tmp1); }));
          if (NUMNAMES == last) break;
          ++NUMNAMES;
        }
      }
    } while(0);
  } while (LEXER.TOKEN.ID == TKIDENTIFIER);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}

void PSSTATEMENT();
void PSDEFINITIONS();

void PSFUNCTIONBODY(TPSFUNCTION* FNPTR) {
  PInteger POS;
  TPSDEFENTRY* CHECKPOINT;
  TPSVARIABLE* RESULTPTR;
  STARTLOCALSCOPE(FNPTR);
  CHECKPOINT = DEFS.LATEST;
  do {
    PInteger first = 1;
    PInteger last = FNPTR->ARGS.COUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        ADDVARIABLE(&FNPTR->ARGS.DEFS[subrange(POS, 1, 16) - 1]);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  OUTFUNCTIONDEFINITION(FNPTR);
  OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
  if (FNPTR->RETURNTYPEPTR != PNil) {
    RESULTPTR = ({ TPSVARIABLE tmp2 = ({ PString tmp1 = str_make(6, "RESULT"); MAKEVARIABLE(&tmp1, FNPTR->RETURNTYPEPTR); }); ADDVARIABLE(&tmp2); });
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

void PSPROCEDUREDEFINITION() {
  TPSFUNCTION DEF;
  DEF = EMPTYFUNCTION();
  WANTTOKENANDREAD(TKPROCEDURE);
  DEF.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  DEF.EXTERNALNAME = DEF.NAME;
  WANTTOKEN2(TKLPAREN, TKSEMICOLON);
  if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&DEF.ARGS);
  WANTTOKENANDREAD(TKSEMICOLON);
  if (LEXER.TOKEN.ID == TKFORWARD) {
    SKIPTOKEN(TKFORWARD);
    WANTTOKENANDREAD(TKSEMICOLON);
    DEF.ISDECLARATION = 1;
    OUTFUNCTIONDECLARATION(ADDFUNCTION(&DEF));
  }
  else PSFUNCTIONBODY(ADDFUNCTION(&DEF));
}

void PSFUNCTIONDEFINITION() {
  TPSFUNCTION DEF;
  DEF = EMPTYFUNCTION();
  WANTTOKENANDREAD(TKFUNCTION);
  DEF.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  DEF.EXTERNALNAME = DEF.NAME;
  if (LEXER.TOKEN.ID == TKSEMICOLON && HASFORWARDDECLARATION(&DEF.NAME)) DEF.RETURNTYPEPTR = PNil;
  else {
    WANTTOKEN2(TKLPAREN, TKCOLON);
    if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&DEF.ARGS);
    WANTTOKENANDREAD(TKCOLON);
    DEF.RETURNTYPEPTR = PSRESULTTYPE();
  }
  WANTTOKENANDREAD(TKSEMICOLON);
  if (LEXER.TOKEN.ID == TKFORWARD) {
    SKIPTOKEN(TKFORWARD);
    WANTTOKENANDREAD(TKSEMICOLON);
    DEF.ISDECLARATION = 1;
    OUTFUNCTIONDECLARATION(ADDFUNCTION(&DEF));
  }
  else PSFUNCTIONBODY(ADDFUNCTION(&DEF));
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
    } while (LEXER.TOKEN.ID != TKRPAREN);
    SKIPTOKEN(TKRPAREN);
  }
  WANTTOKENANDREAD(TKSEMICOLON);
}

TEXPRESSIONOBJ* PSPOINTERDEREF(TEXPRESSIONOBJ* PTR) {
  TEXPRESSIONOBJ* RESULT;
  WANTTOKENANDREAD(TKCARET);
  RESULT = EXPOINTERACCESS(PTR);
  return RESULT;
}

TEXFUNCTIONARGS PSFUNCTIONARGS() {
  TEXFUNCTIONARGS RESULT;
  RESULT.SIZE = 0;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      RESULT.SIZE = RESULT.SIZE + 1;
      RESULT.VALUES[subrange(RESULT.SIZE, 1, 16) - 1] = PSEXPRESSION();
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  return RESULT;
}

TEXPRESSIONOBJ* PSFUNCTIONCALL(TEXPRESSIONOBJ* FN) {
  TEXPRESSIONOBJ* RESULT;
  if (FN->CLS == XCFNREF || ISFUNCTIONTYPE(FN->TYPEPTR)) RESULT = ({ TEXFUNCTIONARGS tmp1 = PSFUNCTIONARGS(); EXFUNCTIONCALL(FN, &tmp1); });
  else if (FN->CLS == XCPSEUDOFNREF) RESULT = FN->PSEUDOFNPTR->PARSEFN(FN);
  return RESULT;
}

TEXPRESSIONOBJ* PSARRAYACCESS(TEXPRESSIONOBJ* ARR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* IDX;
  WANTTOKENANDREAD(TKLBRACKET);
  do {
    IDX = PSEXPRESSION();
    if (ISSTRINGYTYPE(ARR->TYPEPTR)) ARR = EXSTRINGCHAR(ARR, IDX);
    else ARR = EXARRAYACCESS(ARR, IDX);
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    if (LEXER.TOKEN.ID == TKCOMMA && !ISARRAYTYPE(ARR->TYPEPTR) && !ISSTRINGYTYPE(ARR->TYPEPTR)) {
      PString tmp1 = str_make(29, "Expected an array or a string");
      ERRORFOREXPR(&tmp1, ARR);
    }
    SKIPTOKEN(TKCOMMA);
  } while (LEXER.TOKEN.ID != TKRBRACKET);
  WANTTOKENANDREAD(TKRBRACKET);
  RESULT = ARR;
  return RESULT;
}

TEXPRESSIONOBJ* PSFIELDACCESS(TEXPRESSIONOBJ* REC) {
  TEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER FLD;
  WANTTOKENANDREAD(TKDOT);
  FLD = PSIDENTIFIER();
  RESULT = EXFIELDACCESS(REC, FINDFIELD(REC->TYPEPTR, &FLD.NAME, 1));
  return RESULT;
}

TEXPRESSIONOBJ* PSVARIABLE() {
  TEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER ID;
  TPSWITHVAR* WITHVARPTR;
  TPSNAME FOUND;
  TEXPRESSIONOBJ* EXPR;
  ID = PSIDENTIFIER();
  WITHVARPTR = FINDWITHVAR(&ID.NAME);
  if (WITHVARPTR != PNil) {
    EXPR = EXVARIABLE(WITHVARPTR->VARPTR);
    EXPR = EXFIELDACCESS(EXPR, FINDFIELD(EXPR->TYPEPTR, &ID.NAME, 1));
  }
  else {
    FOUND = *FINDNAME(&ID.NAME, 1);
    if (FOUND.CLS == TNCVARIABLE) EXPR = EXVARIABLE(FOUND.VARPTR);
    else if (FOUND.CLS == TNCCONSTANT) EXPR = EXCOPY(FOUND.CONSTPTR->VALUE);
    else if (FOUND.CLS == TNCFUNCTION) EXPR = EXFNREF(FOUND.FNPTR);
    else if (FOUND.CLS == TNCENUMVAL) EXPR = EXENUMCONSTANT(FOUND.ORDINAL, FOUND.ENUMTYPEPTR);
    else if (FOUND.CLS == TNCPSEUDOFN) EXPR = EXPSEUDOFN(FOUND.PSEUDOFNPTR);
    else COMPILEERROR(CONCAT(CpLenPtr, 20, "Invalid identifier: ", CpEnd | CpStringPtr, &ID.NAME));
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* PSVARIABLEORFUNCTIONEXTENSION(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  PBoolean DONE;
  if (EXPR->CLS == XCVARIABLE && (LEXER.TOKEN.ID == TKLBRACKET || LEXER.TOKEN.ID == TKDOT || TKCARET <= LEXER.TOKEN.ID && LEXER.TOKEN.ID <= TKLPAREN)) EXPR->VARPTR->WASUSED = 1;
  DONE = 0;
  do {
    if (EXPR->CLS == XCFNREF || EXPR->CLS == XCPSEUDOFNREF) EXPR = PSFUNCTIONCALL(EXPR);
    else if (ISFUNCTIONTYPE(EXPR->TYPEPTR) && LEXER.TOKEN.ID == TKLPAREN) EXPR = PSFUNCTIONCALL(EXPR);
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

PString PARSESTRING(const PString *PSTR) {
  PString RESULT;
  enum __attribute__((__packed__)) enum9 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, NUMCHARREADY, CARET, ERROR, DONE } STATE;
  PInteger POS;
  PChar CH;
  PInteger CHNUM;
  const char* enumvalues9[] = { "NONE", "QUOTEDSTR", "HASH", "NUMCHARDEC", "NUMCHARHEX", "NUMCHARREADY", "CARET", "ERROR", "DONE" };
  RESULT = str_make(0, "");
  STATE = NONE;
  POS = 1;
  while (POS <= LENGTH(PSTR)) {
    CH = PSTR->chr[POS];
    if (STATE == NONE) {
      POS = POS + 1;
      if (CH == '\'') {
        STATE = QUOTEDSTR;
        if (POS > 2 && PSTR->chr[POS - 2] == '\'') RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '\'');
      }
      else if (CH == '#') STATE = HASH;
      else if (CH == '^') STATE = CARET;
      else STATE = ERROR;
    }
    else if (STATE == QUOTEDSTR) {
      POS = POS + 1;
      if (CH == '\'') STATE = NONE;
      else RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, CH);
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
      RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, CHR(CHNUM));
      STATE = NONE;
    }
    else if (STATE == CARET) {
      POS = POS + 1;
      STATE = NONE;
      if ('@' <= CH && CH <= '_') RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, CHR((int)CH - 64));
      else if ('a' <= CH && CH <= 'z') RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, CHR((int)CH - 96));
      else STATE = ERROR;
    }
    else if (STATE == ERROR) COMPILEERROR(CONCAT(CpLenPtr, 29, "Invalid character in string: ", CpEnd | CpStringPtr, PSTR));
  }
  if (STATE == QUOTEDSTR || STATE == CARET) COMPILEERROR(CONCAT(CpLenPtr, 26, "String is not terminated: ", CpEnd | CpStringPtr, PSTR));
  if (NUMCHARDEC <= STATE && STATE <= NUMCHARHEX) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, CHR(CHNUM));
  return RESULT;
}

PInteger PARSEINT(const PString *PSTR) {
  PInteger RESULT;
  PInteger CODE;
  VAL_i(PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(CONCAT(CpLenPtr, 25, "Could not parse integer: ", CpEnd | CpStringPtr, PSTR));
  return RESULT;
}

PReal PARSEREAL(const PString *PSTR) {
  PReal RESULT;
  PInteger CODE;
  VAL_r(PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(CONCAT(CpLenPtr, 29, "Could not parse real number: ", CpEnd | CpStringPtr, PSTR));
  return RESULT;
}

TEXPRESSIONOBJ* PSSETCONSTRUCTOR() {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* FIRST;
  TEXPRESSIONOBJ* LAST;
  RESULT = EXSET();
  WANTTOKENANDREAD(TKLBRACKET);
  while (LEXER.TOKEN.ID != TKRBRACKET) {
    FIRST = PSEXPRESSION();
    ENSUREORDINALEXPR(FIRST);
    if (LEXER.TOKEN.ID == TKRANGE) {
      WANTTOKENANDREAD(TKRANGE);
      LAST = PSEXPRESSION();
      if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) {
        PString tmp1 = str_make(53, "Set element range bounds must belong to the same type");
        ERRORFOREXPR(&tmp1, LAST);
      }
    }
    else LAST = PNil;
    RESULT = EXSETADDRANGE(RESULT, FIRST, LAST);
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    SKIPTOKEN(TKCOMMA);
  }
  WANTTOKENANDREAD(TKRBRACKET);
  return RESULT;
}

TEXPRESSIONOBJ* PSFACTOR() {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* EXPR;
  PString STR;
  if (LEXER.TOKEN.ID == TKNIL) {
    EXPR = EXNIL();
    READTOKEN();
  }
  else if (LEXER.TOKEN.ID == TKSTRING) {
    STR = ({ PString tmp1 = GETTOKENVALUEANDREAD(TKSTRING); PARSESTRING(&tmp1); });
    if (LENGTH(&STR) == 1) EXPR = EXCHARCONSTANT(STR.chr[1]);
    else EXPR = EXSTRINGCONSTANT(STR);
  }
  else if (LEXER.TOKEN.ID == TKINTEGER) EXPR = EXINTEGERCONSTANT(({ PString tmp2 = GETTOKENVALUEANDREAD(TKINTEGER); PARSEINT(&tmp2); }));
  else if (LEXER.TOKEN.ID == TKREAL) EXPR = EXREALCONSTANT(({ PString tmp3 = GETTOKENVALUEANDREAD(TKREAL); PARSEREAL(&tmp3); }));
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
    EXPR = EXOPNOT(PSFACTOR());
  }
  else if (LEXER.TOKEN.ID == TKAT) {
    WANTTOKENANDREAD(TKAT);
    EXPR = PSVARIABLE();
    if (EXPR->CLS == XCVARIABLE) EXPR->VARPTR->WASUSED = 1;
    else if (EXPR->CLS == XCFNREF) EXPR->FNPTR->WASUSED = 1;
    EXPR = EXADDRESSOF(EXPR);
  }
  else COMPILEERROR(CONCAT(CpLenPtr, 29, "Invalid token in expression: ", CpEnd | CpString, LXTOKENSTR()));
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* PSBINARYOP(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT, TLXTOKENID OP) {
  TEXPRESSIONOBJ* RESULT;
  switch (OP) {
    case TKPLUS:
      RESULT = EXOPADD(LEFT, RIGHT);
      break;
    case TKMINUS:
      RESULT = EXOPSUB(LEFT, RIGHT);
      break;
    case TKASTERISK:
      RESULT = EXOPMUL(LEFT, RIGHT);
      break;
    case TKSLASH:
      RESULT = EXOPDIVREAL(LEFT, RIGHT);
      break;
    case TKDIV:
      RESULT = EXOPDIVINT(LEFT, RIGHT);
      break;
    case TKMOD:
      RESULT = EXOPMOD(LEFT, RIGHT);
      break;
    case TKAND:
      RESULT = EXOPAND(LEFT, RIGHT);
      break;
    case TKOR:
      RESULT = EXOPOR(LEFT, RIGHT);
      break;
    case TKXOR:
      RESULT = EXOPXOR(LEFT, RIGHT);
      break;
    case TKSHL:
      RESULT = EXOPSHL(LEFT, RIGHT);
      break;
    case TKSHR:
      RESULT = EXOPSHR(LEFT, RIGHT);
      break;
    case TKIN:
      RESULT = EXOPIN(LEFT, RIGHT);
      break;
    case TKEQUALS:
      RESULT = EXOPEQ(LEFT, RIGHT);
      break;
    case TKNOTEQUALS:
      RESULT = EXOPNE(LEFT, RIGHT);
      break;
    case TKLESSTHAN:
      RESULT = EXOPLT(LEFT, RIGHT);
      break;
    case TKMORETHAN:
      RESULT = EXOPGT(LEFT, RIGHT);
      break;
    case TKLESSOREQUALS:
      RESULT = EXOPLTEQ(LEFT, RIGHT);
      break;
    case TKMOREOREQUALS:
      RESULT = EXOPGTEQ(LEFT, RIGHT);
      break;
    default:
      COMPILEERROR(CONCAT(CpLenPtr, 26, "Expected an operator, got ", CpEnd | CpString, LXTOKENNAME(OP)));
      break;
  }
  return RESULT;
}

TEXPRESSIONOBJ* PSTERM() {
  TEXPRESSIONOBJ* RESULT;
  TLXTOKENID OP;
  TEXPRESSIONOBJ* EXPR;
  EXPR = PSFACTOR();
  while (ISOPMULTIPLYING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = PSBINARYOP(EXPR, PSFACTOR(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* PSSIMPLEEXPRESSION() {
  TEXPRESSIONOBJ* RESULT;
  PBoolean NEGATIVE;
  TLXTOKENID OP;
  TEXPRESSIONOBJ* EXPR;
  NEGATIVE = LEXER.TOKEN.ID == TKMINUS;
  if (NEGATIVE) READTOKEN();
  else SKIPTOKEN(TKPLUS);
  EXPR = PSTERM();
  if (NEGATIVE) EXPR = EXOPNEG(EXPR);
  while (ISOPADDING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = PSBINARYOP(EXPR, PSTERM(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* PSEXPRESSION() {
  TEXPRESSIONOBJ* RESULT;
  TLXTOKENID OP;
  TEXPRESSIONOBJ* EXPR;
  EXPR = PSSIMPLEEXPRESSION();
  while (ISOPRELATIONAL(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = PSBINARYOP(EXPR, PSSIMPLEEXPRESSION(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

void PSASSIGN(TEXPRESSIONOBJ* LHS, TEXPRESSIONOBJ* RHS) {
  TPSVARIABLE* RESULTVARPTR;
  if (LHS->CLS == XCFNREF) {
    if (LHS->FNPTR != DEFS.CURRENTFN) {
      PString tmp1 = str_make(35, "Cannot assign a value to a function");
      ERRORFOREXPR(&tmp1, LHS);
    }
    RESULTVARPTR = ({ PString tmp2 = str_make(6, "RESULT"); FINDNAMEOFCLASS(&tmp2, TNCVARIABLE, 1); })->VARPTR;
    EXDISPOSE(&LHS);
    LHS = EXVARIABLE(RESULTVARPTR);
  }
  RHS = EXCOERCE(RHS, LHS->TYPEPTR);
  if (!LHS->ISASSIGNABLE) {
    if (LHS->ISFUNCTIONRESULT) {
      PString tmp3 = str_make(41, "Cannot assign to the result of a function");
      ERRORFOREXPR(&tmp3, LHS);
    }
    else {
      PString tmp4 = str_make(27, "Cannot assign to a constant");
      ERRORFOREXPR(&tmp4, LHS);
    }
  }
  EXMARKINITIALIZED(LHS);
  OUTASSIGN(LHS, RHS);
  EXDISPOSE(&LHS);
  EXDISPOSE(&RHS);
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
  TEXPRESSIONOBJ* LHS;
  TEXPRESSIONOBJ* ORIGLHS;
  PBoolean USESTMPVARS;
  LHS = PSVARIABLE();
  if (LHS->CLS != XCFNREF || LHS->FNPTR != DEFS.CURRENTFN || LEXER.TOKEN.ID != TKASSIGN) LHS = PSVARIABLEORFUNCTIONEXTENSION(LHS);
  if (LEXER.TOKEN.ID == TKASSIGN) {
    WANTTOKENANDREAD(TKASSIGN);
    PSASSIGN(LHS, PSEXPRESSION());
  }
  else {
    if (ISFUNCTIONTYPE(LHS->TYPEPTR)) LHS = PSFUNCTIONCALL(LHS);
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
    else if (LEXER.TOKEN.ID == TKEQUALS) COMPILEERROR(str_make(56, "Invalid statement (maybe you wrote '=' instead of ':='?)"));
    else COMPILEERROR(str_make(17, "Invalid statement"));
    EXDISPOSE(&ORIGLHS);
    if (USESTMPVARS) {
      OUTSEQUENCEEND();
      CLOSETEMPORARYSCOPE();
    }
  }
}

void PSIFSTATEMENT() {
  TEXPRESSIONOBJ* COND;
  WANTTOKENANDREAD(TKIF);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  OUTIF(COND);
  EXDISPOSE(&COND);
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
  TEXPRESSIONOBJ* CASEPTR;
  TPSTYPE* CASETYPEPTR;
  TEXPRESSIONOBJ* CASELABEL;
  WANTTOKENANDREAD(TKCASE);
  CASEPTR = PSEXPRESSION();
  CASETYPEPTR = CASEPTR->TYPEPTR;
  ENSUREORDINALEXPR(CASEPTR);
  OUTCASEBEGIN(CASEPTR);
  EXDISPOSE(&CASEPTR);
  WANTTOKENANDREAD(TKOF);
  do {
    CASELABEL = EXCOERCE(PSIMMEDIATE(), CASETYPEPTR);
    WANTTOKENANDREAD(TKCOLON);
    OUTCASESTATEMENTBEGIN(CASELABEL);
    EXDISPOSE(&CASELABEL);
    PSSTATEMENT();
    OUTCASESTATEMENTEND();
    WANTTOKEN3(TKSEMICOLON, TKELSE, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  } while (TKELSE > LEXER.TOKEN.ID || LEXER.TOKEN.ID > TKEND);
  OUTCASEELSEBEGIN();
  if (LEXER.TOKEN.ID == TKELSE) {
    READTOKEN();
    do {
      PSSTATEMENT();
      WANTTOKEN2(TKSEMICOLON, TKEND);
      SKIPTOKEN(TKSEMICOLON);
    } while (LEXER.TOKEN.ID != TKEND);
  }
  OUTCASEELSEEND();
  OUTCASEEND();
  WANTTOKENANDREAD(TKEND);
}

void PSREPEATSTATEMENT() {
  TEXPRESSIONOBJ* COND;
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
  EXDISPOSE(&COND);
}

void PSWHILESTATEMENT() {
  TEXPRESSIONOBJ* COND;
  WANTTOKENANDREAD(TKWHILE);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  OUTWHILEBEGIN(COND);
  EXDISPOSE(&COND);
  WANTTOKENANDREAD(TKDO);
  PSSTATEMENT();
  OUTWHILEEND();
}

void PSFORSTATEMENT() {
  TEXPRESSIONOBJ* ITER;
  TEXPRESSIONOBJ* FIRST;
  TEXPRESSIONOBJ* LAST;
  PBoolean ASCENDING;
  WANTTOKENANDREAD(TKFOR);
  ITER = PSEXPRESSION();
  if (ITER->ISFUNCTIONRESULT) {
    PString tmp1 = str_make(45, "Iterator must not be the result of a function");
    ERRORFOREXPR(&tmp1, ITER);
  }
  ENSUREASSIGNABLEEXPR(ITER);
  ENSUREORDINALEXPR(ITER);
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
  EXDISPOSE(&ITER);
  EXDISPOSE(&FIRST);
  EXDISPOSE(&LAST);
}

void PSWITHSTATEMENT() {
  TEXPRESSIONOBJ* BASE;
  TPSVARIABLE* VARPTR;
  WANTTOKEN(TKWITH);
  STARTTEMPORARYSCOPE();
  OUTSEQUENCEBEGIN();
  do {
    READTOKEN();
    BASE = PSEXPRESSION();
    VARPTR = ADDWITHVAR(BASE);
    OUTDECLAREANDASSIGN(VARPTR, BASE);
    EXDISPOSE(&BASE);
    WANTTOKEN2(TKCOMMA, TKDO);
  } while (LEXER.TOKEN.ID != TKDO);
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
  else COMPILEERROR(CONCAT(CpLenPtr, 17, "Unexpected token ", CpEnd | CpString, LXTOKENSTR()));
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

void EXECUTEDIRECTIVE(const PString *DIR) {
  if (LENGTH(DIR) > 3 && DIR->chr[2] == 'I' && DIR->chr[3] == ' ') {
    PString tmp1 = COPY(DIR, 4, 255);
    LXINCLUDE(&tmp1);
  }
  else if (LENGTH(DIR) == 3 && DIR->chr[2] == 'R' && (DIR->chr[3] == '+' || DIR->chr[3] == '-')) OPTIONS.CHECKBOUNDS = DIR->chr[3] == '+';
  else if (LENGTH(DIR) == 3 && DIR->chr[2] == 'I' && (DIR->chr[3] == '+' || DIR->chr[3] == '-')) OPTIONS.CHECKIORESULT = DIR->chr[3] == '+';
}

void READTOKEN() {
  PBoolean STOP;
  TLXINCLUDESTACKELEM* PREVSTACK;
  do {
    LXREADTOKEN();
    STOP = LEXER.TOKEN.ID != TKCOMMENT;
    if (LEXER.TOKEN.ID == TKCOMMENT) if (LENGTH(&LEXER.TOKEN.VALUE) >= 2 && LEXER.TOKEN.VALUE.chr[1] == '$') EXECUTEDIRECTIVE(&LEXER.TOKEN.VALUE);
    if (LEXER.TOKEN.ID == TKEOF && LEXER.INCLUDESTACK != PNil) {
      LEXER.INPUT = LEXER.INCLUDESTACK->INPUT;
      PREVSTACK = LEXER.INCLUDESTACK;
      LEXER.INCLUDESTACK = PREVSTACK->PREV;
      free(PREVSTACK);
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

TEXPRESSIONOBJ* _PF_UNARY_PARSE() {
  TEXPRESSIONOBJ* RESULT;
  WANTTOKENANDREAD(TKLPAREN);
  RESULT = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  return RESULT;
}

PString _PF_FUN_OVERLOAD(const PString *NAMEPREFIX, TPSTYPE* TYPEPTR) {
  PString RESULT;
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_b");
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_i");
  else if (ISREALTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_r");
  else if (ISCHARTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_c");
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_s");
  else COMPILEERROR(CONCAT(CpLenPtr, 5, "Type ", CpString, TYPENAME(TYPEPTR), CpLenPtr, 26, " is not representable for ", CpLenPtr, 12, "overload of ", CpEnd | CpStringPtr, NAMEPREFIX));
  return RESULT;
}

TEXPRESSIONOBJ* PF_OVERLOAD_PARSE(TEXPRESSIONOBJ* FNEXPR, PString NAMEPREFIX) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* ARG;
  TEXFUNCTIONARGS ARGS;
  TPSFUNCTION* FNPTR;
  EXDISPOSE(&FNEXPR);
  ARG = PNil;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    WANTTOKENANDREAD(TKLPAREN);
    if (LEXER.TOKEN.ID != TKRPAREN) ARG = PSEXPRESSION();
    WANTTOKENANDREAD(TKRPAREN);
  }
  if (ARG == PNil) {
    FNPTR = ({ PString tmp1 = CONCAT(CpStringPtr, &NAMEPREFIX, CpEnd | CpLenPtr, 2, "_n"); FINDNAMEOFCLASS(&tmp1, TNCFUNCTION, 1); })->FNPTR;
    ARGS.SIZE = 0;
    RESULT = EXFUNCTIONCALL(EXFNREF(FNPTR), &ARGS);
  }
  else {
    FNPTR = ({ PString tmp2 = _PF_FUN_OVERLOAD(&NAMEPREFIX, ARG->TYPEPTR); FINDNAMEOFCLASS(&tmp2, TNCFUNCTION, 1); })->FNPTR;
    ARGS.SIZE = 1;
    ARGS.VALUES[0] = ARG;
    RESULT = EXFUNCTIONCALL(EXFNREF(FNPTR), &ARGS);
  }
  return RESULT;
}

TEXPRESSIONOBJ* PFDISPOSE_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* PTR;
  PTR = _PF_UNARY_PARSE();
  ENSUREASSIGNABLEEXPR(PTR);
  ENSUREPOINTEREXPR(PTR);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = PTR;
  return RESULT;
}

TEXPRESSIONOBJ* PFNEW_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* PTR;
  PTR = _PF_UNARY_PARSE();
  ENSUREASSIGNABLEEXPR(PTR);
  ENSUREPOINTEREXPR(PTR);
  EXMARKINITIALIZED(PTR);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = PTR;
  return RESULT;
}

TEXPRESSIONOBJ* PFORD_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  EXDISPOSE(&FNEXPR);
  RESULT = PFORD(_PF_UNARY_PARSE());
  return RESULT;
}

TEXPRESSIONOBJ* PFORD(TEXPRESSIONOBJ* ARG) {
  TEXPRESSIONOBJ* RESULT;
  ENSUREORDINALEXPR(ARG);
  if (EXISIMMEDIATE(ARG)) {
    {
      TEXIMMEDIATE *with1 = &ARG->IMMEDIATE;
      switch (with1->CLS) {
        case XICBOOLEAN:
          if (with1->BOOLEANVAL) RESULT = EXINTEGERCONSTANT(1);
          else RESULT = EXINTEGERCONSTANT(0);
          break;
        case XICINTEGER:
          RESULT = EXINTEGERCONSTANT(with1->INTEGERVAL);
          break;
        case XICCHAR:
          RESULT = EXINTEGERCONSTANT((int)with1->CHARVAL);
          break;
        case XICENUM:
          RESULT = EXINTEGERCONSTANT(with1->ENUMORDINAL);
          break;
        default:
          {
            PString tmp2 = str_make(19, "Expected an ordinal");
            ERRORFOREXPR(&tmp2, ARG);
          }
          break;
      }
    }
    EXDISPOSE(&ARG);
  }
  else {
    RESULT = EXPSEUDOFNCALL(EXPSEUDOFN(PSEUDOFUNS.ORD));
    RESULT->PSEUDOFNCALL.ARG1 = ARG;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  }
  return RESULT;
}

TEXPRESSIONOBJ* PFPRED_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  EXDISPOSE(&FNEXPR);
  RESULT = PFPRED(_PF_UNARY_PARSE());
  return RESULT;
}

TEXPRESSIONOBJ* PFPRED(TEXPRESSIONOBJ* ARG) {
  TEXPRESSIONOBJ* RESULT;
  PBoolean OUTOFBOUNDS;
  ENSUREORDINALEXPR(ARG);
  if (EXISIMMEDIATE(ARG)) {
    OUTOFBOUNDS = 0;
    {
      TEXIMMEDIATE *with1 = &ARG->IMMEDIATE;
      switch (with1->CLS) {
        case XICBOOLEAN:
          if (with1->BOOLEANVAL) RESULT = EXBOOLEANCONSTANT(0);
          else OUTOFBOUNDS = 1;
          break;
        case XICINTEGER:
          RESULT = EXINTEGERCONSTANT(with1->INTEGERVAL - 1);
          break;
        case XICCHAR:
          if ((int)with1->CHARVAL > 0) RESULT = EXCHARCONSTANT(pred(with1->CHARVAL, 0, 255));
          else OUTOFBOUNDS = 1;
          break;
        case XICENUM:
          if (with1->ENUMORDINAL > 0) RESULT = EXENUMCONSTANT(with1->ENUMORDINAL - 1, ARG->TYPEPTR);
          else OUTOFBOUNDS = 1;
          break;
        default:
          {
            PString tmp2 = str_make(19, "Expected an ordinal");
            ERRORFOREXPR(&tmp2, ARG);
          }
          break;
      }
    }
    if (OUTOFBOUNDS) {
      PString tmp1 = str_make(47, "Predecessor for argument would be out of bounds");
      ERRORFOREXPR(&tmp1, ARG);
    }
    EXDISPOSE(&ARG);
  }
  else {
    RESULT = EXPSEUDOFNCALL(EXPSEUDOFN(PSEUDOFUNS.PRED));
    RESULT->PSEUDOFNCALL.ARG1 = ARG;
    RESULT->TYPEPTR = ARG->TYPEPTR;
  }
  return RESULT;
}

TEXPRESSIONOBJ* PFRANDOM_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = PF_OVERLOAD_PARSE(FNEXPR, str_make(6, "RANDOM"));
  return RESULT;
}

TEXPRESSIONOBJ* PFSIZEOF_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER ID;
  TPSNAME FOUND;
  WANTTOKENANDREAD(TKLPAREN);
  ID = PSIDENTIFIER();
  WANTTOKENANDREAD(TKRPAREN);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  FOUND = *FINDNAME(&ID.NAME, 1);
  if (FOUND.CLS == TNCVARIABLE) RESULT->PSEUDOFNCALL.ARG1 = EXVARIABLE(FOUND.VARPTR);
  else if (FOUND.CLS == TNCTYPE) RESULT->PSEUDOFNCALL.TYPEARG = FOUND.TYPEPTR;
  else COMPILEERROR(CONCAT(CpLenPtr, 46, "Expected a variable or a type identifier; got ", CpEnd | CpStringPtr, &ID.NAME));
  return RESULT;
}

TEXPRESSIONOBJ* PFSUCC_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  EXDISPOSE(&FNEXPR);
  RESULT = PFSUCC(_PF_UNARY_PARSE());
  return RESULT;
}

TEXPRESSIONOBJ* PFSUCC(TEXPRESSIONOBJ* ARG) {
  TEXPRESSIONOBJ* RESULT;
  PBoolean OUTOFBOUNDS;
  ENSUREORDINALEXPR(ARG);
  if (EXISIMMEDIATE(ARG)) {
    OUTOFBOUNDS = 0;
    {
      TEXIMMEDIATE *with1 = &ARG->IMMEDIATE;
      switch (with1->CLS) {
        case XICBOOLEAN:
          if (!with1->BOOLEANVAL) RESULT = EXBOOLEANCONSTANT(1);
          else OUTOFBOUNDS = 1;
          break;
        case XICINTEGER:
          RESULT = EXINTEGERCONSTANT(with1->INTEGERVAL + 1);
          break;
        case XICCHAR:
          if ((int)with1->CHARVAL < 255) RESULT = EXCHARCONSTANT(succ(with1->CHARVAL, 0, 255));
          else OUTOFBOUNDS = 1;
          break;
        case XICENUM:
          if (with1->ENUMORDINAL < with1->ENUMPTR->SIZE - 1) RESULT = EXENUMCONSTANT(with1->ENUMORDINAL + 1, ARG->TYPEPTR);
          else OUTOFBOUNDS = 1;
          break;
        default:
          {
            PString tmp2 = str_make(19, "Expected an ordinal");
            ERRORFOREXPR(&tmp2, ARG);
          }
          break;
      }
    }
    if (OUTOFBOUNDS) {
      PString tmp1 = str_make(45, "Successor for argument would be out of bounds");
      ERRORFOREXPR(&tmp1, ARG);
    }
    EXDISPOSE(&ARG);
  }
  else {
    RESULT = EXPSEUDOFNCALL(EXPSEUDOFN(PSEUDOFUNS.SUCC));
    RESULT->PSEUDOFNCALL.ARG1 = ARG;
    RESULT->TYPEPTR = ARG->TYPEPTR;
  }
  return RESULT;
}

PString PF_UNARY_DESCRIBE(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    RESULT = CONCAT(CpStringPtr, &with1->PSEUDOFNPTR->NAME, CpChar, '(', CpString, EXDESCRIBE(with1->ARG1), CpEnd | CpChar, ')');
  }
  return RESULT;
}

PString PF_TERNARY_DESCRIBE(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    RESULT = CONCAT(CpStringPtr, &with1->PSEUDOFNPTR->NAME, CpChar, '(', CpString, EXDESCRIBE(with1->ARG1), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(with1->ARG2), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(with1->ARG3), CpEnd | CpChar, ')');
  }
  return RESULT;
}

PString PF_INDEF_DESCRIBE(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    RESULT = CONCAT(CpStringPtr, &with1->PSEUDOFNPTR->NAME, CpEnd | CpLenPtr, 5, "(...)");
  }
  return RESULT;
}

PString PFSIZEOF_DESCRIBE(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    if (with1->ARG1 != PNil) RESULT = CONCAT(CpLenPtr, 7, "SIZEOF(", CpString, EXDESCRIBE(with1->ARG1), CpEnd | CpChar, ')');
    else RESULT = CONCAT(CpLenPtr, 7, "SIZEOF(", CpString, TYPENAME(with1->TYPEARG), CpEnd | CpChar, ')');
  }
  return RESULT;
}

TEXPRESSIONOBJ* _MODIOREAD_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  PBoolean FIRST;
  TEXPRESSIONOBJ* OUTVAR;
  TEXREADARGS* READARG;
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = EXVARIABLE(({ PString tmp1 = str_make(5, "INPUT"); FINDNAMEOFCLASS(&tmp1, TNCVARIABLE, 1); })->VARPTR);
  READARG = PNil;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTVAR = PSEXPRESSION();
      if (FIRST && ISTEXTTYPE(OUTVAR->TYPEPTR)) {
        ENSUREADDRESSABLEEXPR(OUTVAR);
        EXDISPOSE(&RESULT->PSEUDOFNCALL.ARG1);
        RESULT->PSEUDOFNCALL.ARG1 = OUTVAR;
      }
      else {
        ENSUREASSIGNABLEEXPR(OUTVAR);
        if (READARG == PNil) {
          RESULT->PSEUDOFNCALL.READARGS = malloc(sizeof(TEXREADARGS));
          READARG = RESULT->PSEUDOFNCALL.READARGS;
        }
        else {
          READARG->NEXT = malloc(sizeof(TEXREADARGS));
          READARG = READARG->NEXT;
        }
        READARG->NEXT = PNil;
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

TEXPRESSIONOBJ* _MODIOWRITE_EVALUATEZEROARG(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXFUNCTIONARGS ARGS;
  ARGS.SIZE = 0;
  if (ISFUNCTIONTYPE(EXPR->TYPEPTR) && EXPR->TYPEPTR->FNDEFPTR->RETURNTYPEPTR != PNil && EXPR->TYPEPTR->FNDEFPTR->ARGS.COUNT == 0) RESULT = EXFUNCTIONCALL(EXPR, &ARGS);
  else RESULT = EXPR;
  return RESULT;
}

TEXPRESSIONOBJ* _MODIOWRITE_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  PBoolean FIRST;
  TEXPRESSIONOBJ* OUTEXPR;
  TEXWRITEARGS* WRITEARG;
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = EXVARIABLE(({ PString tmp1 = str_make(6, "OUTPUT"); FINDNAMEOFCLASS(&tmp1, TNCVARIABLE, 1); })->VARPTR);
  WRITEARG = PNil;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      OUTEXPR = _MODIOWRITE_EVALUATEZEROARG(PSEXPRESSION());
      if (FIRST && ISTEXTTYPE(OUTEXPR->TYPEPTR)) {
        ENSUREADDRESSABLEEXPR(OUTEXPR);
        EXDISPOSE(&RESULT->PSEUDOFNCALL.ARG1);
        RESULT->PSEUDOFNCALL.ARG1 = OUTEXPR;
      }
      else {
        if (WRITEARG == PNil) {
          RESULT->PSEUDOFNCALL.WRITEARGS = malloc(sizeof(TEXWRITEARGS));
          WRITEARG = RESULT->PSEUDOFNCALL.WRITEARGS;
        }
        else {
          WRITEARG->NEXT = malloc(sizeof(TEXWRITEARGS));
          WRITEARG = WRITEARG->NEXT;
        }
        WRITEARG->ARG = OUTEXPR;
        WRITEARG->WIDTH = PNil;
        WRITEARG->PREC = PNil;
        WRITEARG->NEXT = PNil;
        if (LEXER.TOKEN.ID == TKCOLON) {
          WANTTOKENANDREAD(TKCOLON);
          WRITEARG->WIDTH = PSEXPRESSION();
          ENSUREINTEGEREXPR(WRITEARG->WIDTH);
          if (ISREALTYPE(WRITEARG->ARG->TYPEPTR) && LEXER.TOKEN.ID == TKCOLON) {
            WANTTOKENANDREAD(TKCOLON);
            WRITEARG->PREC = PSEXPRESSION();
            ENSUREINTEGEREXPR(WRITEARG->PREC);
          }
        }
      }
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
      FIRST = 0;
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  return RESULT;
}

void _UPFIRST(PString *STR) {
  PInteger POS;
  do {
    PInteger first = 1;
    PInteger last = LENGTH(STR);
    if (first <= last) {
      POS = first;
      while (1) {
        if (POS == 1) STR->chr[POS] = UPCASE(STR->chr[POS]);
        else STR->chr[POS] = LOWERCASE(STR->chr[POS]);
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
}

TEXPRESSIONOBJ* _MODIO_FILEFUN_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXFUNCTIONARGS ARGS;
  PString FNNAME;
  TPSFUNCTION* FNPTR;
  FNNAME = FNEXPR->PSEUDOFNPTR->NAME;
  _UPFIRST(&FNNAME);
  EXDISPOSE(&FNEXPR);
  FNPTR = FINDNAMEOFCLASS(&FNNAME, TNCFUNCTION, 1)->FNPTR;
  ARGS = PSFUNCTIONARGS();
  ARGS.SIZE = ARGS.SIZE + 1;
  ARGS.VALUES[subrange(ARGS.SIZE, 1, 16) - 1] = EXBOOLEANCONSTANT(OPTIONS.CHECKIORESULT);
  RESULT = EXFUNCTIONCALL(EXFNREF(FNPTR), &ARGS);
  return RESULT;
}

TEXPRESSIONOBJ* _MODIO_FILERESETFUN_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXFUNCTIONARGS ARGS;
  PString FNNAME;
  TPSFUNCTION* FNPTR;
  FNNAME = FNEXPR->PSEUDOFNPTR->NAME;
  _UPFIRST(&FNNAME);
  EXDISPOSE(&FNEXPR);
  FNPTR = FINDNAMEOFCLASS(&FNNAME, TNCFUNCTION, 1)->FNPTR;
  ARGS = PSFUNCTIONARGS();
  ARGS.SIZE = ARGS.SIZE + 2;
  ARGS.VALUES[subrange(ARGS.SIZE - 1, 1, 16) - 1] = EXINTEGERCONSTANT(0);
  ARGS.VALUES[subrange(ARGS.SIZE, 1, 16) - 1] = EXBOOLEANCONSTANT(OPTIONS.CHECKIORESULT);
  RESULT = EXFUNCTIONCALL(EXFNREF(FNPTR), &ARGS);
  return RESULT;
}

void _ADDIOPROC1(PString NAME, TPSVARIABLE ARG1) {
  ADDPSEUDOFN(&NAME, &_MODIO_FILEFUN_PARSE, &PF_INDEF_DESCRIBE);
  _UPFIRST(&NAME);
  {
    TPSFUNCTION tmp2 = MAKEPROCEDURE2(&NAME, ARG1, ({ PString tmp1 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp1, PRIMITIVETYPES.PTBOOLEAN); }));
    ADDFUNCTION(&tmp2);
  }
}

void _ADDIOPROC2(PString NAME, TPSVARIABLE ARG1, TPSVARIABLE ARG2) {
  ADDPSEUDOFN(&NAME, &_MODIO_FILEFUN_PARSE, &PF_INDEF_DESCRIBE);
  _UPFIRST(&NAME);
  {
    TPSFUNCTION tmp2 = MAKEPROCEDURE3(&NAME, ARG1, ARG2, ({ PString tmp1 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp1, PRIMITIVETYPES.PTBOOLEAN); }));
    ADDFUNCTION(&tmp2);
  }
}

void _ADDIOFUN1(PString NAME, TPSTYPE* RETTYPE, TPSVARIABLE ARG1) {
  ADDPSEUDOFN(&NAME, &_MODIO_FILEFUN_PARSE, &PF_INDEF_DESCRIBE);
  _UPFIRST(&NAME);
  {
    TPSFUNCTION tmp2 = MAKEFUNCTION2(&NAME, RETTYPE, ARG1, ({ PString tmp1 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp1, PRIMITIVETYPES.PTBOOLEAN); }));
    ADDFUNCTION(&tmp2);
  }
}

void _ADDFILEPROC(PString NAME) {
  _ADDIOPROC1(NAME, ({ PString tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTTEXT); }));
}

void _ADDFILERESETPROC(PString NAME) {
  ADDPSEUDOFN(&NAME, &_MODIO_FILERESETFUN_PARSE, &PF_INDEF_DESCRIBE);
  _UPFIRST(&NAME);
  {
    TPSFUNCTION tmp4 = MAKEPROCEDURE3(&NAME, ({ PString tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTTEXT); }), ({ PString tmp2 = str_make(10, "BLOCK_SIZE"); MAKEARG(&tmp2, PRIMITIVETYPES.PTINTEGER); }), ({ PString tmp3 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp3, PRIMITIVETYPES.PTBOOLEAN); }));
    ADDFUNCTION(&tmp4);
  }
}

void _ADDFILEPROC1(PString NAME, TPSVARIABLE ARG1) {
  _ADDIOPROC2(NAME, ({ PString tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTTEXT); }), ARG1);
}

void _ADDDIRPROC(PString NAME) {
  _ADDIOPROC1(NAME, ({ PString tmp1 = str_make(3, "DIR"); MAKECONSTARG(&tmp1, PRIMITIVETYPES.PTSTRING); }));
}

void _ADDFILEFUN(PString NAME, TPSTYPE* RETURNTYPEPTR) {
  _ADDIOFUN1(NAME, RETURNTYPEPTR, ({ PString tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTTEXT); }));
}

void _ADDCONSTFILEFUN(PString NAME, TPSTYPE* RETURNTYPEPTR) {
  _ADDIOFUN1(NAME, RETURNTYPEPTR, ({ PString tmp1 = str_of('F'); MAKECONSTARG(&tmp1, PRIMITIVETYPES.PTTEXT); }));
}

void REGISTERGLOBALS_IO() {
  {
    TPSVARIABLE tmp2 = ({ PString tmp1 = str_make(5, "INPUT"); MAKEVARIABLE(&tmp1, PRIMITIVETYPES.PTTEXT); });
    ADDVARIABLE(&tmp2);
  }
  {
    TPSVARIABLE tmp4 = ({ PString tmp3 = str_make(6, "OUTPUT"); MAKEVARIABLE(&tmp3, PRIMITIVETYPES.PTTEXT); });
    ADDVARIABLE(&tmp4);
  }
  {
    TPSVARIABLE tmp6 = ({ PString tmp5 = str_make(6, "STDERR"); MAKEVARIABLE(&tmp5, PRIMITIVETYPES.PTTEXT); });
    ADDVARIABLE(&tmp6);
  }
  PSEUDOFUNS.READ = ({ PString tmp7 = str_make(4, "READ"); ADDPSEUDOFN(&tmp7, &_MODIOREAD_PARSE, &PF_INDEF_DESCRIBE); });
  PSEUDOFUNS.READLN = ({ PString tmp8 = str_make(6, "READLN"); ADDPSEUDOFN(&tmp8, &_MODIOREAD_PARSE, &PF_INDEF_DESCRIBE); });
  PSEUDOFUNS.WRITE = ({ PString tmp9 = str_make(5, "WRITE"); ADDPSEUDOFN(&tmp9, &_MODIOWRITE_PARSE, &PF_INDEF_DESCRIBE); });
  PSEUDOFUNS.WRITELN = ({ PString tmp10 = str_make(7, "WRITELN"); ADDPSEUDOFN(&tmp10, &_MODIOWRITE_PARSE, &PF_INDEF_DESCRIBE); });
  _ADDFILEPROC1(str_make(6, "ASSIGN"), ({ PString tmp11 = str_make(4, "NAME"); MAKECONSTARG(&tmp11, PRIMITIVETYPES.PTSTRING); }));
  _ADDFILEPROC(str_make(5, "CLOSE"));
  _ADDFILEFUN(str_make(3, "EOF"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDFILEFUN(str_make(4, "EOLN"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDCONSTFILEFUN(str_make(7, "FILEPOS"), PRIMITIVETYPES.PTINTEGER);
  _ADDCONSTFILEFUN(str_make(8, "FILESIZE"), PRIMITIVETYPES.PTINTEGER);
  _ADDFILEPROC(str_make(5, "FLUSH"));
  {
    TPSFUNCTION tmp13 = ({ PString tmp12 = str_make(8, "IORESULT"); MAKEFUNCTION0(&tmp12, PRIMITIVETYPES.PTINTEGER); });
    ADDFUNCTION(&tmp13);
  }
  _ADDFILERESETPROC(str_make(5, "RESET"));
  _ADDFILERESETPROC(str_make(7, "REWRITE"));
  _ADDFILEPROC1(str_make(4, "SEEK"), ({ PString tmp14 = str_make(3, "POS"); MAKEARG(&tmp14, PRIMITIVETYPES.PTINTEGER); }));
  _ADDFILEFUN(str_make(7, "SEEKEOF"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDFILEFUN(str_make(8, "SEEKEOLN"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDDIRPROC(str_make(5, "CHDIR"));
  _ADDFILEPROC(str_make(5, "ERASE"));
  _ADDIOPROC2(str_make(6, "GETDIR"), ({ PString tmp15 = str_make(5, "DRIVE"); MAKEARG(&tmp15, PRIMITIVETYPES.PTINTEGER); }), ({ PString tmp16 = str_make(3, "DIR"); MAKEVARARG(&tmp16, PRIMITIVETYPES.PTSTRING); }));
  _ADDDIRPROC(str_make(5, "MKDIR"));
  _ADDFILEPROC1(str_make(6, "RENAME"), ({ PString tmp17 = str_make(4, "NAME"); MAKECONSTARG(&tmp17, PRIMITIVETYPES.PTSTRING); }));
  _ADDDIRPROC(str_make(5, "RMDIR"));
}

TEXPRESSIONOBJ* _MODMATH_ABS_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = PF_OVERLOAD_PARSE(FNEXPR, str_make(3, "ABS"));
  return RESULT;
}

TEXPRESSIONOBJ* _MODMATH_SQR_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  RESULT = PF_OVERLOAD_PARSE(FNEXPR, str_make(3, "SQR"));
  return RESULT;
}

void REGISTERGLOBALS_MATH() {
  {
    TPSCONSTANT tmp2 = ({ PString tmp1 = str_make(2, "PI"); MAKECONSTANT(&tmp1, EXREALCONSTANT( 3.14159265358979E+000)); });
    ADDCONSTANT(&tmp2);
  }
  {
    PString tmp3 = str_make(3, "ABS");
    ADDPSEUDOFN(&tmp3, &_MODMATH_ABS_PARSE, &PF_UNARY_DESCRIBE);
  }
  {
    PString tmp4 = str_make(3, "SQR");
    ADDPSEUDOFN(&tmp4, &_MODMATH_SQR_PARSE, &PF_UNARY_DESCRIBE);
  }
  {
    TPSFUNCTION tmp7 = ({ PString tmp6 = str_make(5, "ABS_i"); MAKEFUNCTION1(&tmp6, PRIMITIVETYPES.PTINTEGER, ({ PString tmp5 = str_make(3, "NUM"); MAKEARG(&tmp5, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp7);
  }
  {
    TPSFUNCTION tmp10 = ({ PString tmp9 = str_make(5, "ABS_r"); MAKEFUNCTION1(&tmp9, PRIMITIVETYPES.PTREAL, ({ PString tmp8 = str_make(3, "NUM"); MAKEARG(&tmp8, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp10);
  }
  {
    TPSFUNCTION tmp13 = ({ PString tmp12 = str_make(6, "ARCTAN"); MAKEFUNCTION1(&tmp12, PRIMITIVETYPES.PTREAL, ({ PString tmp11 = str_make(3, "TAN"); MAKEARG(&tmp11, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp13);
  }
  {
    TPSFUNCTION tmp16 = ({ PString tmp15 = str_make(3, "COS"); MAKEFUNCTION1(&tmp15, PRIMITIVETYPES.PTREAL, ({ PString tmp14 = str_make(5, "ANGLE"); MAKEARG(&tmp14, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp16);
  }
  {
    TPSFUNCTION tmp19 = ({ PString tmp18 = str_make(3, "EXP"); MAKEFUNCTION1(&tmp18, PRIMITIVETYPES.PTREAL, ({ PString tmp17 = str_make(3, "POW"); MAKEARG(&tmp17, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp19);
  }
  {
    TPSFUNCTION tmp22 = ({ PString tmp21 = str_make(4, "FRAC"); MAKEFUNCTION1(&tmp21, PRIMITIVETYPES.PTREAL, ({ PString tmp20 = str_of('X'); MAKEARG(&tmp20, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp22);
  }
  {
    TPSFUNCTION tmp25 = ({ PString tmp24 = str_make(3, "INT"); MAKEFUNCTION1(&tmp24, PRIMITIVETYPES.PTREAL, ({ PString tmp23 = str_of('X'); MAKEARG(&tmp23, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp25);
  }
  {
    TPSFUNCTION tmp28 = ({ PString tmp27 = str_make(2, "LN"); MAKEFUNCTION1(&tmp27, PRIMITIVETYPES.PTREAL, ({ PString tmp26 = str_of('X'); MAKEARG(&tmp26, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp28);
  }
  {
    TPSFUNCTION tmp31 = ({ PString tmp30 = str_make(3, "ODD"); MAKEFUNCTION1(&tmp30, PRIMITIVETYPES.PTBOOLEAN, ({ PString tmp29 = str_of('X'); MAKEARG(&tmp29, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp31);
  }
  {
    TPSFUNCTION tmp34 = ({ PString tmp33 = str_make(5, "ROUND"); MAKEFUNCTION1(&tmp33, PRIMITIVETYPES.PTINTEGER, ({ PString tmp32 = str_of('X'); MAKEARG(&tmp32, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp34);
  }
  {
    TPSFUNCTION tmp37 = ({ PString tmp36 = str_make(3, "SIN"); MAKEFUNCTION1(&tmp36, PRIMITIVETYPES.PTREAL, ({ PString tmp35 = str_make(5, "ANGLE"); MAKEARG(&tmp35, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp37);
  }
  {
    TPSFUNCTION tmp40 = ({ PString tmp39 = str_make(5, "SQR_i"); MAKEFUNCTION1(&tmp39, PRIMITIVETYPES.PTINTEGER, ({ PString tmp38 = str_make(3, "NUM"); MAKEARG(&tmp38, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp40);
  }
  {
    TPSFUNCTION tmp43 = ({ PString tmp42 = str_make(5, "SQR_r"); MAKEFUNCTION1(&tmp42, PRIMITIVETYPES.PTREAL, ({ PString tmp41 = str_make(3, "NUM"); MAKEARG(&tmp41, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp43);
  }
  {
    TPSFUNCTION tmp46 = ({ PString tmp45 = str_make(4, "SQRT"); MAKEFUNCTION1(&tmp45, PRIMITIVETYPES.PTREAL, ({ PString tmp44 = str_of('X'); MAKEARG(&tmp44, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp46);
  }
  {
    TPSFUNCTION tmp49 = ({ PString tmp48 = str_make(5, "TRUNC"); MAKEFUNCTION1(&tmp48, PRIMITIVETYPES.PTINTEGER, ({ PString tmp47 = str_of('X'); MAKEARG(&tmp47, PRIMITIVETYPES.PTREAL); })); });
    ADDFUNCTION(&tmp49);
  }
}

TEXPRESSIONOBJ* _MODSTRINGS_CONCAT_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* OPERAND;
  EXDISPOSE(&FNEXPR);
  RESULT = PNil;
  WANTTOKENANDREAD(TKLPAREN);
  do {
    OPERAND = PSEXPRESSION();
    ENSURESTRINGYEXPR(OPERAND);
    if (RESULT == PNil) RESULT = OPERAND;
    else RESULT = EXOPADD(RESULT, OPERAND);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  } while (LEXER.TOKEN.ID != TKRPAREN);
  WANTTOKENANDREAD(TKRPAREN);
  return RESULT;
}

TEXPRESSIONOBJ* _MODSTRINGS_STR_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* SRC;
  TEXPRESSIONOBJ* DEST;
  TEXPRESSIONOBJ* WIDTH;
  TEXPRESSIONOBJ* PREC;
  WIDTH = PNil;
  PREC = PNil;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = EXOUTRANGE(PSEXPRESSION());
  if (LEXER.TOKEN.ID == TKCOLON) {
    WANTTOKENANDREAD(TKCOLON);
    WIDTH = PSEXPRESSION();
    ENSUREINTEGEREXPR(WIDTH);
    if (ISREALTYPE(SRC->TYPEPTR) && LEXER.TOKEN.ID == TKCOLON) {
      WANTTOKENANDREAD(TKCOLON);
      PREC = PSEXPRESSION();
      ENSUREINTEGEREXPR(PREC);
    }
  }
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  ENSUREASSIGNABLEEXPR(DEST);
  ENSURESTRINGEXPR(DEST);
  if (!ISBOOLEANTYPE(SRC->TYPEPTR) && !ISINTEGERTYPE(SRC->TYPEPTR) && !ISREALTYPE(SRC->TYPEPTR) && !ISENUMTYPE(SRC->TYPEPTR)) {
    PString tmp1 = str_make(30, "Invalid type for source of STR");
    ERRORFOREXPR(&tmp1, SRC);
  }
  EXMARKINITIALIZED(DEST);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = SRC;
  RESULT->PSEUDOFNCALL.ARG2 = DEST;
  RESULT->PSEUDOFNCALL.ARG3 = WIDTH;
  RESULT->PSEUDOFNCALL.ARG4 = PREC;
  return RESULT;
}

PString _MODSTRINGS_STR_DESCRIBE(TEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    if (with1->ARG3 == PNil) RESULT = CONCAT(CpLenPtr, 4, "STR(", CpString, EXDESCRIBE(with1->ARG1), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(with1->ARG2), CpEnd | CpChar, ')');
    else if (with1->ARG4 == PNil) RESULT = CONCAT(CpLenPtr, 4, "STR(", CpString, EXDESCRIBE(with1->ARG1), CpChar, ':', CpString, EXDESCRIBE(with1->ARG3), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(with1->ARG2), CpEnd | CpChar, ')');
    else RESULT = CONCAT(CpLenPtr, 4, "STR(", CpString, EXDESCRIBE(with1->ARG1), CpChar, ':', CpString, EXDESCRIBE(with1->ARG3), CpChar, ':', CpString, EXDESCRIBE(with1->ARG4), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(with1->ARG2), CpEnd | CpChar, ')');
  }
  return RESULT;
}

TEXPRESSIONOBJ* _MODSTRINGS_VAL_PARSE(TEXPRESSIONOBJ* FNEXPR) {
  TEXPRESSIONOBJ* RESULT;
  TEXPRESSIONOBJ* SRC;
  TEXPRESSIONOBJ* DEST;
  TEXPRESSIONOBJ* CODE;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = EXOUTRANGE(PSEXPRESSION());
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  WANTTOKENANDREAD(TKCOMMA);
  CODE = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  ENSURESTRINGEXPR(SRC);
  ENSUREASSIGNABLEEXPR(DEST);
  if (!ISBOOLEANTYPE(DEST->TYPEPTR) && !ISINTEGERTYPE(DEST->TYPEPTR) && !ISREALTYPE(DEST->TYPEPTR) && !ISENUMTYPE(DEST->TYPEPTR)) {
    PString tmp1 = str_make(30, "Invalid type for target of VAL");
    ERRORFOREXPR(&tmp1, DEST);
  }
  ENSUREASSIGNABLEEXPR(CODE);
  ENSUREINTEGEREXPR(CODE);
  EXMARKINITIALIZED(DEST);
  EXMARKINITIALIZED(CODE);
  RESULT = EXPSEUDOFNCALL(FNEXPR);
  RESULT->PSEUDOFNCALL.ARG1 = SRC;
  RESULT->PSEUDOFNCALL.ARG2 = DEST;
  RESULT->PSEUDOFNCALL.ARG3 = CODE;
  return RESULT;
}

void REGISTERGLOBALS_STRINGS() {
  {
    PString tmp1 = str_make(6, "CONCAT");
    ADDPSEUDOFN(&tmp1, &_MODSTRINGS_CONCAT_PARSE, &PF_INDEF_DESCRIBE);
  }
  PSEUDOFUNS.STR = ({ PString tmp2 = str_make(3, "STR"); ADDPSEUDOFN(&tmp2, &_MODSTRINGS_STR_PARSE, &_MODSTRINGS_STR_DESCRIBE); });
  PSEUDOFUNS.VAL = ({ PString tmp3 = str_make(3, "VAL"); ADDPSEUDOFN(&tmp3, &_MODSTRINGS_VAL_PARSE, &PF_TERNARY_DESCRIBE); });
  {
    TPSFUNCTION tmp6 = ({ PString tmp5 = str_make(3, "CHR"); MAKEFUNCTION1(&tmp5, PRIMITIVETYPES.PTCHAR, ({ PString tmp4 = str_make(3, "POS"); MAKEARG(&tmp4, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp6);
  }
  {
    TPSFUNCTION tmp11 = ({ PString tmp10 = str_make(4, "COPY"); MAKEFUNCTION3(&tmp10, PRIMITIVETYPES.PTSTRING, ({ PString tmp7 = str_make(3, "STR"); MAKECONSTARG(&tmp7, PRIMITIVETYPES.PTSTRING); }), ({ PString tmp8 = str_make(3, "POS"); MAKEARG(&tmp8, PRIMITIVETYPES.PTINTEGER); }), ({ PString tmp9 = str_make(3, "NUM"); MAKEARG(&tmp9, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp11);
  }
  {
    TPSFUNCTION tmp16 = ({ PString tmp15 = str_make(6, "DELETE"); MAKEPROCEDURE3(&tmp15, ({ PString tmp12 = str_make(3, "STR"); MAKEVARARG(&tmp12, PRIMITIVETYPES.PTSTRING); }), ({ PString tmp13 = str_make(3, "POS"); MAKEARG(&tmp13, PRIMITIVETYPES.PTINTEGER); }), ({ PString tmp14 = str_make(3, "NUM"); MAKEARG(&tmp14, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp16);
  }
  {
    TPSFUNCTION tmp21 = ({ PString tmp20 = str_make(6, "INSERT"); MAKEPROCEDURE3(&tmp20, ({ PString tmp17 = str_make(3, "INS"); MAKECONSTARG(&tmp17, PRIMITIVETYPES.PTSTRING); }), ({ PString tmp18 = str_make(6, "TARGET"); MAKEVARARG(&tmp18, PRIMITIVETYPES.PTSTRING); }), ({ PString tmp19 = str_make(3, "POS"); MAKEARG(&tmp19, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp21);
  }
  {
    TPSFUNCTION tmp24 = ({ PString tmp23 = str_make(6, "LENGTH"); MAKEFUNCTION1(&tmp23, PRIMITIVETYPES.PTINTEGER, ({ PString tmp22 = str_make(3, "STR"); MAKECONSTARG(&tmp22, PRIMITIVETYPES.PTSTRING); })); });
    ADDFUNCTION(&tmp24);
  }
  {
    TPSFUNCTION tmp27 = ({ PString tmp26 = str_make(9, "LOWERCASE"); MAKEFUNCTION1(&tmp26, PRIMITIVETYPES.PTCHAR, ({ PString tmp25 = str_make(3, "CHR"); MAKEARG(&tmp25, PRIMITIVETYPES.PTCHAR); })); });
    ADDFUNCTION(&tmp27);
  }
  {
    TPSFUNCTION tmp31 = ({ PString tmp30 = str_make(3, "POS"); MAKEFUNCTION2(&tmp30, PRIMITIVETYPES.PTINTEGER, ({ PString tmp28 = str_make(6, "NEEDLE"); MAKECONSTARG(&tmp28, PRIMITIVETYPES.PTSTRING); }), ({ PString tmp29 = str_make(8, "HAYSTACK"); MAKECONSTARG(&tmp29, PRIMITIVETYPES.PTSTRING); })); });
    ADDFUNCTION(&tmp31);
  }
  {
    TPSFUNCTION tmp34 = ({ PString tmp33 = str_make(6, "UPCASE"); MAKEFUNCTION1(&tmp33, PRIMITIVETYPES.PTCHAR, ({ PString tmp32 = str_make(3, "CHR"); MAKEARG(&tmp32, PRIMITIVETYPES.PTCHAR); })); });
    ADDFUNCTION(&tmp34);
  }
}

void STARTGLOBALSCOPE() {
  TPSDEFENTRY* DEF;
  INITDEFS();
  PRIMITIVETYPES.PTNIL = ({ PString tmp1 = str_make(3, "NIL"); MAKEBASETYPE(&tmp1, TTCNIL); });
  PRIMITIVETYPES.PTBOOLEAN = ({ PString tmp2 = str_make(7, "BOOLEAN"); MAKEBASETYPE(&tmp2, TTCBOOLEAN); });
  PRIMITIVETYPES.PTINTEGER = ({ PString tmp3 = str_make(7, "INTEGER"); MAKEBASETYPE(&tmp3, TTCINTEGER); });
  PRIMITIVETYPES.PTREAL = ({ PString tmp4 = str_make(4, "REAL"); MAKEBASETYPE(&tmp4, TTCREAL); });
  PRIMITIVETYPES.PTCHAR = ({ PString tmp5 = str_make(4, "CHAR"); MAKEBASETYPE(&tmp5, TTCCHAR); });
  PRIMITIVETYPES.PTSTRING = ({ PString tmp6 = str_make(6, "STRING"); MAKEBASETYPE(&tmp6, TTCSTRING); });
  PRIMITIVETYPES.PTTEXT = ({ PString tmp7 = str_make(4, "TEXT"); MAKEBASETYPE(&tmp7, TTCTEXT); });
  PRIMITIVETYPES.PTEMPTYSET = MAKESETTYPE(PNil);
  {
    TPSCONSTANT tmp9 = ({ PString tmp8 = str_make(5, "FALSE"); MAKECONSTANT(&tmp8, EXBOOLEANCONSTANT(0)); });
    ADDCONSTANT(&tmp9);
  }
  {
    TPSCONSTANT tmp11 = ({ PString tmp10 = str_make(4, "TRUE"); MAKECONSTANT(&tmp10, EXBOOLEANCONSTANT(1)); });
    ADDCONSTANT(&tmp11);
  }
  {
    TPSCONSTANT tmp13 = ({ PString tmp12 = str_make(6, "MAXINT"); MAKECONSTANT(&tmp12, EXINTEGERCONSTANT(32767)); });
    ADDCONSTANT(&tmp13);
  }
  PSEUDOFUNS.ORD = ({ PString tmp14 = str_make(3, "ORD"); ADDPSEUDOFN(&tmp14, &PFORD_PARSE, &PF_UNARY_DESCRIBE); });
  PSEUDOFUNS.PRED = ({ PString tmp15 = str_make(4, "PRED"); ADDPSEUDOFN(&tmp15, &PFPRED_PARSE, &PF_UNARY_DESCRIBE); });
  PSEUDOFUNS.SUCC = ({ PString tmp16 = str_make(4, "SUCC"); ADDPSEUDOFN(&tmp16, &PFSUCC_PARSE, &PF_UNARY_DESCRIBE); });
  PSEUDOFUNS.DISPOSE = ({ PString tmp17 = str_make(7, "DISPOSE"); ADDPSEUDOFN(&tmp17, &PFDISPOSE_PARSE, &PF_UNARY_DESCRIBE); });
  PSEUDOFUNS.NEW = ({ PString tmp18 = str_make(3, "NEW"); ADDPSEUDOFN(&tmp18, &PFNEW_PARSE, &PF_UNARY_DESCRIBE); });
  PSEUDOFUNS.SIZEOF = ({ PString tmp19 = str_make(6, "SIZEOF"); ADDPSEUDOFN(&tmp19, &PFSIZEOF_PARSE, &PFSIZEOF_DESCRIBE); });
  REGISTERGLOBALS_IO();
  REGISTERGLOBALS_MATH();
  REGISTERGLOBALS_STRINGS();
  {
    PString tmp20 = str_make(6, "RANDOM");
    ADDPSEUDOFN(&tmp20, &PFRANDOM_PARSE, &PF_INDEF_DESCRIBE);
  }
  {
    TPSFUNCTION tmp23 = ({ PString tmp22 = str_make(4, "HALT"); MAKEPROCEDURE1(&tmp22, ({ PString tmp21 = str_make(4, "CODE"); MAKEARG(&tmp21, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp23);
  }
  {
    TPSFUNCTION tmp25 = ({ PString tmp24 = str_make(10, "PARAMCOUNT"); MAKEFUNCTION0(&tmp24, PRIMITIVETYPES.PTINTEGER); });
    ADDFUNCTION(&tmp25);
  }
  {
    TPSFUNCTION tmp28 = ({ PString tmp27 = str_make(8, "PARAMSTR"); MAKEFUNCTION1(&tmp27, PRIMITIVETYPES.PTSTRING, ({ PString tmp26 = str_of('I'); MAKEARG(&tmp26, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp28);
  }
  {
    TPSFUNCTION tmp30 = ({ PString tmp29 = str_make(8, "RANDOM_n"); MAKEFUNCTION0(&tmp29, PRIMITIVETYPES.PTREAL); });
    ADDFUNCTION(&tmp30);
  }
  {
    TPSFUNCTION tmp33 = ({ PString tmp32 = str_make(8, "RANDOM_i"); MAKEFUNCTION1(&tmp32, PRIMITIVETYPES.PTINTEGER, ({ PString tmp31 = str_make(3, "NUM"); MAKEARG(&tmp31, PRIMITIVETYPES.PTINTEGER); })); });
    ADDFUNCTION(&tmp33);
  }
  {
    TPSFUNCTION tmp35 = ({ PString tmp34 = str_make(9, "RANDOMIZE"); MAKEPROCEDURE0(&tmp34); });
    ADDFUNCTION(&tmp35);
  }
  DEF = DEFS.LATEST;
  while (DEF != PNil) {
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

typedef enum __attribute__((__packed__)) enum9 { TOTNONE, TOTTYPE, TOTVAR, TOTENUMVAL, TOTFUNDEC, TOTFUNDEF } TOUTPUTTYPE;

const char* enumvalues9[] = { "TOTNONE", "TOTTYPE", "TOTVAR", "TOTENUMVAL", "TOTFUNDEC", "TOTFUNDEF" };

struct record37 {
  PFile OUTPUT;
  PBoolean ISMULTISTATEMENT;
  PInteger INDENT;
  PBoolean NEWLINE;
  TOUTPUTTYPE LASTOUT;
} CODEGEN;

void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF);

void _OUTCOMMA() {
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
}

void _OUTNEWLINE() {
  Write(&CODEGEN.OUTPUT, 1, RwpEnd | RwpLn);
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
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "  ");
        if (CT == last) break;
        ++CT;
      }
    }
  } while(0);
  CODEGEN.NEWLINE = 0;
}

void OUTBEGIN() {
  CODEGEN.ISMULTISTATEMENT = 1;
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '{');
  _OUTNEWLINE();
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
}

void OUTEND() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '}');
  _OUTNEWLINE();
}

void OUTENDSAMELINE() {
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '}');
}

void _OUTESCAPEDCHAR(PChar CH) {
  PInteger CODE;
  PInteger N1;
  PInteger N2;
  CODE = (int)CH;
  N1 = CODE / 16;
  N2 = CODE % 16;
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\x");
  if (N1 < 10) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, CHR(N1 + 48));
  else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, CHR(N1 + 87));
  if (N2 < 10) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, CHR(N2 + 48));
  else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, CHR(N2 + 87));
}

void _OUTCHAR(PChar CHR) {
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '\'');
  if (CHR == '\'') Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\'");
  else if (CHR == '\\') Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\\\");
  else if (' ' <= CHR && CHR <= '~') Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, CHR);
  else _OUTESCAPEDCHAR(CHR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '\'');
}

void _OUTCSTRING(const PString *STR) {
  PInteger POS;
  PChar CH;
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '"');
  do {
    PInteger first = 1;
    PInteger last = LENGTH(STR);
    if (first <= last) {
      POS = first;
      while (1) {
        {
          CH = STR->chr[POS];
          if (CH < ' ' || CH > '~') {
            _OUTESCAPEDCHAR(CH);
            if (POS < LENGTH(STR) && LXISHEXDIGIT(STR->chr[POS + 1])) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\"\"");
          }
          else {
            if (CH == '"') Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\\"");
            else if (CH == '\\') Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\\\");
            else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, CH);
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '"');
}

void _OUTSTRING(const PString *STR) {
  if (LENGTH(STR) == 1) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "str_of(");
    _OUTCHAR(STR->chr[1]);
    Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
  else {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 9, "str_make(", RwpInt | RwpEnd, LENGTH(STR));
    _OUTCOMMA();
    _OUTCSTRING(STR);
    Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
}

PInteger _BINOPPREC(TEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  PBoolean ISSETLEFT;
  PBoolean ISSETRIGHT;
  ISSETLEFT = ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR);
  ISSETRIGHT = ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR);
  switch (EXPR->BINARY.OP) {
    case XOADD:
      if (ISSTRINGYTYPE(EXPR->TYPEPTR)) RESULT = 1;
      else RESULT = 4;
      break;
    case XOSUB:
      RESULT = 4;
      break;
    case XOMUL:
      RESULT = 3;
      break;
    case XODIVREAL:
      RESULT = 3;
      break;
    case XODIVINT:
      RESULT = 3;
      break;
    case XOMOD:
      RESULT = 3;
      break;
    case XOAND:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 11;
      else RESULT = 8;
      break;
    case XOOR:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 12;
      else RESULT = 10;
      break;
    case XOXOR:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 7;
      else RESULT = 9;
      break;
    case XOSHL:
      RESULT = 5;
      break;
    case XOSHR:
      RESULT = 5;
      break;
    case XOIN:
      if (EXISIMMEDIATE(EXPR->BINARY.RIGHT)) RESULT = 12;
      else RESULT = 1;
      break;
    case XOEQ:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 7;
      break;
    case XONE:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 7;
      break;
    case XOLT:
      RESULT = 6;
      break;
    case XOGT:
      RESULT = 6;
      break;
    case XOLTEQ:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 6;
      break;
    case XOGTEQ:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 6;
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 35, "Unknown precedence for operator in ", CpEnd | CpString, EXDESCRIBE(EXPR)));
      break;
  }
  return RESULT;
}

PInteger _PRECEDENCE(TEXPRESSIONOBJ* EXPR) {
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
    case XCADDRESS:
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
      INTERNALERROR(CONCAT(CpLenPtr, 23, "Unknown precedence for ", CpEnd | CpString, EXDESCRIBE(EXPR)));
      break;
  }
  return RESULT;
}

void _OUTEXPRESSIONPARENSPREC(TEXPRESSIONOBJ* EXPR, PInteger PREC) {
  PBoolean USEPARENS;
  USEPARENS = _PRECEDENCE(EXPR) > PREC;
  if (USEPARENS) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '(');
  OUTEXPRESSION(EXPR);
  if (USEPARENS) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _OUTEXPRESSIONPARENS(TEXPRESSIONOBJ* EXPR, TEXPRESSIONOBJ* REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF));
}

void _OUTEXPRESSIONPARENSEXTRA(TEXPRESSIONOBJ* EXPR, TEXPRESSIONOBJ* REF) {
  _OUTEXPRESSIONPARENSPREC(EXPR, _PRECEDENCE(REF) - 1);
}

void _OUTSETTYPENAME(TPSTYPE* TYPEPTR) {
  PInteger NUMBYTES;
  NUMBYTES = GETTYPEHIGHBOUND(TYPEPTR->ELEMENTTYPEPTR) / 8 - GETTYPELOWBOUND(TYPEPTR->ELEMENTTYPEPTR) / 8 + 1;
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 4, "PSet", RwpInt | RwpEnd, 8 * NUMBYTES);
}

void _OUTSETIMMEDIATE(TEXPRESSIONOBJ* EXPR) {
  PInteger LOWBOUND;
  PInteger HIGHBOUND;
  PInteger LOWBOUNDBYTE;
  PInteger SETSIZE;
  TEXSETIMMBOUNDSOBJ* BOUNDS;
  TPSTYPE* ELEMTYPEPTR;
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
  while (BOUNDS != PNil) {
    if (BOUNDS->FIRST < LOWBOUND || BOUNDS->LAST > HIGHBOUND) COMPILEERROR(CONCAT(CpLenPtr, 4, "Set ", CpString, EXDESCRIBE(EXPR), CpLenPtr, 19, " contains elements ", CpLenPtr, 27, "that are out of bounds for ", CpEnd | CpString, TYPENAME(EXPR->TYPEPTR)));
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
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 5, "(PSet", RwpInt, SETSIZE * 8, RwpLenPtr | RwpEnd, 4, ") { ");
  do {
    PInteger first = 1;
    PInteger last = SETSIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) _OUTCOMMA();
          Write(&CODEGEN.OUTPUT, 1, RwpInt | RwpEnd, SETELEMS[subrange(POS, 1, 32) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
}

void _OUTEXSET(TEXPRESSIONOBJ* EXPR) {
  TPSTYPE* ELEMENTTYPEPTR;
  TEXSETEXPRBOUNDSOBJ* BOUNDS;
  TEXPRESSIONOBJ* FIRST;
  TEXPRESSIONOBJ* LAST;
  PInteger LOWBOUNDBYTE;
  ELEMENTTYPEPTR = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMENTTYPEPTR) / 8;
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "({ ");
  _OUTSETTYPENAME(EXPR->TYPEPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, " dst = ");
  _OUTSETIMMEDIATE(EXPR->SETBASE);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "; ");
  BOUNDS = EXPR->SETBOUNDS;
  while (BOUNDS != PNil) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "set_set(");
    FIRST = PFORD(EXCOERCE(EXCOPY(BOUNDS->FIRST), ELEMENTTYPEPTR));
    if (BOUNDS->LAST == PNil) LAST = EXCOPY(FIRST);
    else LAST = PFORD(EXCOERCE(EXCOPY(BOUNDS->LAST), ELEMENTTYPEPTR));
    OUTEXPRESSION(FIRST);
    _OUTCOMMA();
    OUTEXPRESSION(LAST);
    _OUTCOMMA();
    Write(&CODEGEN.OUTPUT, 1, RwpInt, LOWBOUNDBYTE, RwpLenPtr | RwpEnd, 13, ", dst.bits); ");
    EXDISPOSE(&FIRST);
    EXDISPOSE(&LAST);
    BOUNDS = BOUNDS->NEXT;
  }
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "dst; })");
}

void _OUTEXIMMEDIATE(TEXPRESSIONOBJ* EXPR) {
  {
    TEXIMMEDIATE *with1 = &EXPR->IMMEDIATE;
    switch (with1->CLS) {
      case XICNIL:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "PNil");
        break;
      case XICBOOLEAN:
        if (with1->BOOLEANVAL) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '1');
        else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '0');
        break;
      case XICINTEGER:
        Write(&CODEGEN.OUTPUT, 1, RwpInt | RwpEnd, with1->INTEGERVAL);
        break;
      case XICREAL:
        Write(&CODEGEN.OUTPUT, 1, RwpReal | RwpEnd, with1->REALVAL);
        break;
      case XICCHAR:
        _OUTCHAR(with1->CHARVAL);
        break;
      case XICSTRING:
        _OUTSTRING(&with1->STRINGVAL);
        break;
      case XICENUM:
        Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &with1->ENUMPTR->VALUES[subrange(with1->ENUMORDINAL, 0, 127)]);
        break;
      case XICSET:
        _OUTSETIMMEDIATE(EXPR);
        break;
      default:
        break;
    }
  }
}

void _OUTBOUNDS(TPSTYPE* TYPEPTR) {
  Write(&CODEGEN.OUTPUT, 1, RwpInt | RwpEnd, GETTYPELOWBOUND(TYPEPTR));
  _OUTCOMMA();
  Write(&CODEGEN.OUTPUT, 1, RwpInt | RwpEnd, GETTYPEHIGHBOUND(TYPEPTR));
}

void _OUTARRAYINDEX(TEXPRESSIONOBJ* INDEX, TPSTYPE* TYPEPTR) {
  PInteger LOWBOUND;
  TEXPRESSIONOBJ* SIZE;
  LOWBOUND = GETTYPELOWBOUND(TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  SIZE = EXOPSUB(PFORD(EXCOPY(INDEX)), EXINTEGERCONSTANT(LOWBOUND));
  OUTEXPRESSION(SIZE);
  EXDISPOSE(&SIZE);
}

void _OUTADDRESS(TEXPRESSIONOBJ* EXPR) {
  if (EXPR->CLS == XCPOINTER) OUTEXPRESSION(EXPR->POINTEREXPR);
  else if (EXPR->CLS == XCVARIABLE && EXPR->VARPTR->ISREFERENCE) Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->VARPTR->NAME);
  else {
    Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '&');
    _OUTEXPRESSIONPARENSPREC(EXPR, 2);
  }
}

void _OUTEXSUBRANGE(TEXPRESSIONOBJ* EXPR) {
  if (!OPTIONS.CHECKBOUNDS) OUTEXPRESSION(EXPR->SUBRANGEPARENT);
  else {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "subrange(");
    OUTEXPRESSION(EXPR->SUBRANGEPARENT);
    _OUTCOMMA();
    _OUTBOUNDS(EXPR->TYPEPTR);
    Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
}

void _OUTEXVARIABLE(TEXPRESSIONOBJ* EXPR) {
  if (EXPR->VARPTR->ISREFERENCE) Write(&CODEGEN.OUTPUT, 1, RwpChar, '*', RwpStringPtr | RwpEnd, &EXPR->VARPTR->NAME);
  else Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->VARPTR->NAME);
}

void _OUTEXFIELDACCESS(TEXPRESSIONOBJ* EXPR) {
  {
    TEXPRESSIONOBJ *with1 = EXPR->RECEXPR;
    {
      if (with1->CLS == XCPOINTER) {
        _OUTEXPRESSIONPARENS(with1->POINTEREXPR, EXPR);
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "->");
      }
      else if (with1->CLS == XCVARIABLE && with1->VARPTR->ISREFERENCE) Write(&CODEGEN.OUTPUT, 1, RwpStringPtr, &with1->VARPTR->NAME, RwpLenPtr | RwpEnd, 2, "->");
      else {
        _OUTEXPRESSIONPARENS(EXPR->RECEXPR, EXPR);
        Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '.');
      }
      Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[subrange(EXPR->RECFIELDNUM, 1, 32) - 1].NAME);
    }
  }
}

void _OUTEXSTRINGCHAR(TEXPRESSIONOBJ* EXPR) {
  {
    TEXPRESSIONOBJ *with1 = EXPR->STRINGEXPR;
    {
      if (with1->CLS == XCPOINTER) {
        _OUTEXPRESSIONPARENS(with1->POINTEREXPR, EXPR);
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "->chr[");
      }
      else if (with1->CLS == XCVARIABLE && with1->VARPTR->ISREFERENCE) Write(&CODEGEN.OUTPUT, 1, RwpStringPtr, &with1->VARPTR->NAME, RwpLenPtr | RwpEnd, 6, "->chr[");
      else {
        _OUTEXPRESSIONPARENS(EXPR->STRINGEXPR, EXPR);
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, ".chr[");
      }
      OUTEXPRESSION(EXPR->STRINGINDEX);
      Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ']');
    }
  }
}

void _OUTEXFUNCTIONCALLARGS(const TPSFNARGS *ARGDEFS, const TEXFUNCTIONARGS *ARGVALUES) {
  PInteger POS;
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '(');
  do {
    PInteger first = 1;
    PInteger last = ARGVALUES->SIZE;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) _OUTCOMMA();
          if (ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE) {
            ENSUREADDRESSABLEEXPR(ARGVALUES->VALUES[subrange(POS, 1, 16) - 1]);
            _OUTADDRESS(ARGVALUES->VALUES[subrange(POS, 1, 16) - 1]);
          }
          else OUTEXPRESSION(ARGVALUES->VALUES[subrange(POS, 1, 16) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _OUTEXFUNCTIONCALL(TEXPRESSIONOBJ* EXPR) {
  _OUTEXPRESSIONPARENS(EXPR->FNEXPR, EXPR);
  if (EXPR->FNEXPR->CLS == XCFNREF) _OUTEXFUNCTIONCALLARGS(&EXPR->FNEXPR->FNPTR->ARGS, &EXPR->CALLARGS);
  else _OUTEXFUNCTIONCALLARGS(&EXPR->FNEXPR->TYPEPTR->FNDEFPTR->ARGS, &EXPR->CALLARGS);
}

void _OUTDISPOSE(TEXPRESSIONOBJ* EXPR);
void _OUTNEW(TEXPRESSIONOBJ* EXPR);
void _OUTORD(TEXPRESSIONOBJ* EXPR);
void _OUTPRED(TEXPRESSIONOBJ* EXPR);
void _OUTREAD(TEXPRESSIONOBJ* EXPR);
void _OUTSIZEOF(TEXPRESSIONOBJ* EXPR);
void _OUTSTR(TEXPRESSIONOBJ* EXPR);
void _OUTSUCC(TEXPRESSIONOBJ* EXPR);
void _OUTVAL(TEXPRESSIONOBJ* EXPR);
void _OUTWRITE(TEXPRESSIONOBJ* EXPR);

void _OUTEXPSEUDOFNCALL(TEXPRESSIONOBJ* EXPR) {
  {
    TEXPSEUDOFNCALL *with1 = &EXPR->PSEUDOFNCALL;
    if (with1->PSEUDOFNPTR == PSEUDOFUNS.DISPOSE) _OUTDISPOSE(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.NEW) _OUTNEW(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.ORD) _OUTORD(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.PRED) _OUTPRED(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.READ) _OUTREAD(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.READLN) _OUTREAD(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.SIZEOF) _OUTSIZEOF(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.STR) _OUTSTR(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.SUCC) _OUTSUCC(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.VAL) _OUTVAL(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.WRITE) _OUTWRITE(EXPR);
    else if (with1->PSEUDOFNPTR == PSEUDOFUNS.WRITELN) _OUTWRITE(EXPR);
    else INTERNALERROR(CONCAT(CpLenPtr, 31, "Unimplemented special function ", CpEnd | CpString, EXDESCRIBE(EXPR)));
  }
}

void _OUTEXUNARYOP(TEXPRESSIONOBJ* EXPR) {
  if (EXPR->UNARY.OP == XONEG) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '-');
  else if (EXPR->UNARY.OP == XONOT && ISBOOLEANTYPE(EXPR->TYPEPTR)) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '!');
  else if (EXPR->UNARY.OP == XONOT && ISINTEGERTYPE(EXPR->TYPEPTR)) Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '~');
  _OUTEXPRESSIONPARENS(EXPR->UNARY.PARENT, EXPR);
}

PBoolean _ISARITHMETICOP(TEXOPERATOR OP) {
  PBoolean RESULT;
  RESULT = XOADD <= OP && OP <= XOMOD;
  return RESULT;
}

PString _GETARITHMETICOP(TEXOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case XOADD:
      RESULT = str_of('+');
      break;
    case XOSUB:
      RESULT = str_of('-');
      break;
    case XOMUL:
      RESULT = str_of('*');
      break;
    case XODIVREAL:
      RESULT = str_of('/');
      break;
    case XODIVINT:
      RESULT = str_of('/');
      break;
    case XOMOD:
      RESULT = str_of('%');
      break;
    default:
      break;
  }
  return RESULT;
}

PBoolean _ISLOGICALORBITWISEOP(TEXOPERATOR OP) {
  PBoolean RESULT;
  RESULT = XOAND <= OP && OP <= XOXOR;
  return RESULT;
}

PBoolean _ISBITWISEOP(TEXOPERATOR OP) {
  PBoolean RESULT;
  RESULT = XOSHL <= OP && OP <= XOSHR;
  return RESULT;
}

PString _GETLOGICALOP(TEXOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case XOAND:
      RESULT = str_make(2, "&&");
      break;
    case XOOR:
      RESULT = str_make(2, "||");
      break;
    case XOXOR:
      RESULT = str_make(2, "!=");
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 31, "Unimplemented logical operator ", CpEnd | CpString, EXDESCRIBEOPERATOR(OP)));
      break;
  }
  return RESULT;
}

PString _GETBITWISEOP(TEXOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case XOAND:
      RESULT = str_of('&');
      break;
    case XOOR:
      RESULT = str_of('|');
      break;
    case XOXOR:
      RESULT = str_of('^');
      break;
    case XOSHL:
      RESULT = str_make(2, "<<");
      break;
    case XOSHR:
      RESULT = str_make(2, ">>");
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 31, "Unimplemented bitwise operator ", CpEnd | CpString, EXDESCRIBEOPERATOR(OP)));
      break;
  }
  return RESULT;
}

PBoolean _ISRELATIONALOP(TEXOPERATOR OP) {
  PBoolean RESULT;
  RESULT = XOEQ <= OP && OP <= XOGTEQ;
  return RESULT;
}

PString _GETRELATIONALOP(TEXOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case XOEQ:
      RESULT = str_make(2, "==");
      break;
    case XONE:
      RESULT = str_make(2, "!=");
      break;
    case XOLT:
      RESULT = str_of('<');
      break;
    case XOGT:
      RESULT = str_of('>');
      break;
    case XOLTEQ:
      RESULT = str_make(2, "<=");
      break;
    case XOGTEQ:
      RESULT = str_make(2, ">=");
      break;
    default:
      break;
  }
  return RESULT;
}

void _OUTEXSETOPERATION(TEXPRESSIONOBJ* LEFT, TEXPRESSIONOBJ* RIGHT, TEXOPERATOR OP) {
  TPSTYPE* ELEMTYPEPTR;
  PInteger LOWBOUND;
  PInteger HIGHBOUND;
  PInteger LOWBOUNDBYTE;
  PInteger SETSIZE;
  ELEMTYPEPTR = RIGHT->TYPEPTR->ELEMENTTYPEPTR;
  if (OP == XOLTEQ) _OUTEXSETOPERATION(RIGHT, LEFT, XOGTEQ);
  else if (OP == XONE) {
    Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '!');
    _OUTEXSETOPERATION(LEFT, RIGHT, XOEQ);
  }
  else if (OP == XOIN) {
    LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMTYPEPTR) / 8;
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "set_in(");
    OUTEXPRESSION(LEFT);
    _OUTCOMMA();
    Write(&CODEGEN.OUTPUT, 1, RwpInt | RwpEnd, LOWBOUNDBYTE);
    _OUTCOMMA();
    _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, ".bits)");
  }
  else {
    LOWBOUND = GETTYPELOWBOUND(ELEMTYPEPTR);
    HIGHBOUND = GETTYPEHIGHBOUND(ELEMTYPEPTR);
    SETSIZE = HIGHBOUND / 8 - LOWBOUND / 8 + 1;
    if (OP == XOEQ) {
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 11, "set_equals(");
      _OUTEXPRESSIONPARENSPREC(LEFT, 1);
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, ".bits, ");
      _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 7, ".bits, ", RwpInt, SETSIZE, RwpChar | RwpEnd, ')');
    }
    else if (OP == XOGTEQ) {
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 15, "set_issuperset(");
      _OUTEXPRESSIONPARENSPREC(LEFT, 1);
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, ".bits, ");
      _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 7, ".bits, ", RwpInt, SETSIZE, RwpChar | RwpEnd, ')');
    }
    else {
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 7, "({ PSet", RwpInt, SETSIZE * 8, RwpLenPtr | RwpEnd, 6, " dst; ");
      switch (OP) {
        case XOADD:
          Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "set_union(");
          break;
        case XOSUB:
          Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 15, "set_difference(");
          break;
        case XOMUL:
          Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 17, "set_intersection(");
          break;
        default:
          INTERNALERROR(CONCAT(CpLenPtr, 44, "Materialized set operation not implemented: ", CpEnd | CpString, EXDESCRIBEOPERATOR(OP)));
          break;
      }
      _OUTEXPRESSIONPARENSPREC(LEFT, 1);
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, ".bits, ");
      _OUTEXPRESSIONPARENSPREC(RIGHT, 1);
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 17, ".bits, dst.bits, ", RwpInt, SETSIZE, RwpLenPtr | RwpEnd, 10, "); dst; })");
    }
  }
}

void _OUTCMPCONCATARG(TEXPRESSIONOBJ* EXPR) {
  if (EXISIMMEDIATE(EXPR) && ISSTRINGTYPE(EXPR->TYPEPTR)) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 10, "CpLenPtr, ", RwpInt | RwpEnd, LENGTH(&EXPR->IMMEDIATE.STRINGVAL));
    _OUTCOMMA();
    _OUTCSTRING(&EXPR->IMMEDIATE.STRINGVAL);
  }
  else if (ISCHARTYPE(EXPR->TYPEPTR)) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "CpChar, ");
    OUTEXPRESSION(EXPR);
  }
  else if (EXPR->ISADDRESSABLE) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 13, "CpStringPtr, ");
    _OUTADDRESS(EXPR);
  }
  else {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "CpString, ");
    OUTEXPRESSION(EXPR);
  }
}

void _OUTCONCATARGS(TEXPRESSIONOBJ* EXPR, PBoolean LAST) {
  if (!ISSTRINGYTYPE(EXPR->TYPEPTR)) INTERNALERROR(CONCAT(CpLenPtr, 28, "Expected a stringy type for ", CpEnd | CpString, EXDESCRIBE(EXPR)));
  else if (EXPR->CLS != XCBINARYOP || EXPR->BINARY.OP != XOADD) {
    if (LAST) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "CpEnd | ");
    _OUTCMPCONCATARG(EXPR);
    if (!LAST) _OUTCOMMA();
  }
  else {
    _OUTCONCATARGS(EXPR->BINARY.LEFT, 0);
    _OUTCONCATARGS(EXPR->BINARY.RIGHT, LAST);
  }
}

void _OUTEXBINARYOP(TEXPRESSIONOBJ* EXPR) {
  {
    TEXBINARYOP *with1 = &EXPR->BINARY;
    {
      if (ISBOOLEANTYPE(with1->LEFT->TYPEPTR) && ISBOOLEANTYPE(with1->RIGHT->TYPEPTR)) {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISLOGICALORBITWISEOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETLOGICALOP(with1->OP), RwpChar | RwpEnd, ' ');
        else if (_ISRELATIONALOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETRELATIONALOP(with1->OP), RwpChar | RwpEnd, ' ');
        else ERRORINVALIDOPERATOR(EXPR, with1->OP);
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
      else if (ISNUMERICTYPE(with1->LEFT->TYPEPTR) && ISNUMERICTYPE(with1->RIGHT->TYPEPTR)) {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISARITHMETICOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETARITHMETICOP(with1->OP), RwpChar | RwpEnd, ' ');
        else if (_ISLOGICALORBITWISEOP(with1->OP) || _ISBITWISEOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETBITWISEOP(with1->OP), RwpChar | RwpEnd, ' ');
        else if (_ISRELATIONALOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETRELATIONALOP(with1->OP), RwpChar | RwpEnd, ' ');
        else ERRORINVALIDOPERATOR(EXPR, with1->OP);
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
      else if (ISSTRINGYTYPE(with1->LEFT->TYPEPTR) && ISSTRINGYTYPE(with1->RIGHT->TYPEPTR)) {
        if (with1->OP == XOADD) {
          Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "CONCAT(");
          _OUTCONCATARGS(EXPR, 1);
          Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
        }
        else if (ISCHARTYPE(with1->LEFT->TYPEPTR) && ISCHARTYPE(with1->RIGHT->TYPEPTR)) {
          _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
          if (_ISRELATIONALOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETRELATIONALOP(with1->OP), RwpChar | RwpEnd, ' ');
          else ERRORINVALIDOPERATOR(EXPR, with1->OP);
          _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
        }
        else {
          Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "cmp_str(");
          switch (with1->OP) {
            case XOEQ:
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "CoEq, ");
              break;
            case XONE:
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "CoNotEq, ");
              break;
            case XOLT:
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "CoBefore, ");
              break;
            case XOGT:
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "CoAfter, ");
              break;
            case XOLTEQ:
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 14, "CoBeforeOrEq, ");
              break;
            case XOGTEQ:
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 13, "CoAfterOrEq, ");
              break;
            default:
              ERRORINVALIDOPERATOR(EXPR, with1->OP);
              break;
          }
          _OUTCMPCONCATARG(with1->LEFT);
          _OUTCOMMA();
          _OUTCMPCONCATARG(with1->RIGHT);
          Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
        }
      }
      else if (ISSETTYPE(with1->RIGHT->TYPEPTR)) _OUTEXSETOPERATION(with1->LEFT, with1->RIGHT, with1->OP);
      else {
        _OUTEXPRESSIONPARENS(with1->LEFT, EXPR);
        if (_ISRELATIONALOP(with1->OP)) Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpString, _GETRELATIONALOP(with1->OP), RwpChar | RwpEnd, ' ');
        else ERRORINVALIDOPERATOR(EXPR, with1->OP);
        _OUTEXPRESSIONPARENSEXTRA(with1->RIGHT, EXPR);
      }
    }
  }
}

void _OUTEXWITHTMPVAR(TEXPRESSIONOBJ* EXPR) {
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "({ ");
  while (EXPR->CLS == XCWITHTMPVAR) {
    OUTVARIABLEDECLARATION(*EXPR->TMPVAR->VARPTR);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
    OUTEXPRESSION(EXPR->TMPVARVALUE);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "; ");
    EXPR = EXPR->TMPVARCHILD;
  }
  OUTEXPRESSION(EXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "; })");
}

void OUTEXPRESSION(TEXPRESSIONOBJ* EXPR) {
  switch (EXPR->CLS) {
    case XCIMMEDIATE:
      _OUTEXIMMEDIATE(EXPR);
      break;
    case XCTOSTRING:
      {
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "str_of(");
        OUTEXPRESSION(EXPR->TOSTRPARENT);
        Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
      }
      break;
    case XCTOREAL:
      {
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "(double)");
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
        Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '[');
        _OUTARRAYINDEX(EXPR->ARRAYINDEX, EXPR->ARRAYEXPR->TYPEPTR);
        Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ']');
      }
      break;
    case XCPOINTER:
      {
        Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '*');
        _OUTEXPRESSIONPARENS(EXPR->POINTEREXPR, EXPR);
      }
      break;
    case XCADDRESS:
      _OUTADDRESS(EXPR->ADDRESSEXPR);
      break;
    case XCSTRINGCHAR:
      _OUTEXSTRINGCHAR(EXPR);
      break;
    case XCFNREF:
      Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->FNPTR->EXTERNALNAME);
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

void OUTENUMVALUES(TPSENUMDEF* ENUMPTR) {
  PInteger POSINENUM;
  _OUTBLANKLINE(TOTENUMVAL);
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 22, "const char* enumvalues", RwpInt, ENUMPTR->ID, RwpLenPtr | RwpEnd, 7, "[] = { ");
  do {
    PInteger first = 0;
    PInteger last = ENUMPTR->SIZE - 1;
    if (first <= last) {
      POSINENUM = first;
      while (1) {
        {
          if (POSINENUM != 0) _OUTCOMMA();
          Write(&CODEGEN.OUTPUT, 1, RwpChar, '"', RwpStringPtr, &ENUMPTR->VALUES[subrange(POSINENUM, 0, 127)], RwpChar | RwpEnd, '"');
        }
        if (POSINENUM == last) break;
        ++POSINENUM;
      }
    }
  } while(0);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " };");
  _OUTNEWLINE();
}

void OUTENUMVALUESFROMCHECKPOINT(TPSDEFENTRY* CHECKPOINT) {
  TPSDEFENTRY* DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != PNil) {
    if (DEF->CLS == TDCTYPE && DEF->TYPEPTR->CLS == TTCENUM) {
      if (!DEF->TYPEPTR->ENUMPTR->VALUESHAVEBEENOUTPUT) OUTENUMVALUES(DEF->TYPEPTR->ENUMPTR);
      DEF->TYPEPTR->ENUMPTR->VALUESHAVEBEENOUTPUT = 1;
    }
    DEF = DEF->NEXT;
  }
}

PString _GETRANGETYPE(TPSTYPE* TYPEPTR) {
  PString RESULT;
  typedef enum __attribute__((__packed__)) enum10 { U8, S8, U16, S16, U32, S32 } TYPES;
  const char* enumvalues10[] = { "U8", "S8", "U16", "S16", "U32", "S32" };
  const PString NAMES[6] = { str_make(6, "PBits8"), str_make(7, "PBits8S"), str_make(7, "PBits16"), str_make(8, "PBits16S"), str_make(7, "PBits32"), str_make(8, "PBits32S") };
  const PInteger LOWLIMITS[6] = { 0, -128, 0, -32768, 0, -0 };
  const PInteger HIGHLIMITS[6] = { 255, 127, 65535, 32767, -1, 2147483647 };
  PSet8 FITTYPES;
  PInteger LOW;
  PInteger HIGH;
  TYPES T;
  FITTYPES = (PSet8) { 0 };
  LOW = GETTYPELOWBOUND(TYPEPTR);
  HIGH = GETTYPEHIGHBOUND(TYPEPTR);
  do {
    PInteger first = U8;
    PInteger last = S32;
    if (first <= last) {
      T = first;
      while (1) {
        if (LOW >= LOWLIMITS[T] && HIGH <= HIGHLIMITS[T]) FITTYPES = ({ PSet8 dst; set_union(FITTYPES.bits, ({ PSet8 dst = (PSet8) { 0 }; set_set(T, T, 0, dst.bits); dst; }).bits, dst.bits, 1); dst; });
        if (T == last) break;
        ++T;
      }
    }
  } while(0);
  RESULT = str_make(8, "PInteger");
  do {
    PInteger first = S32;
    PInteger last = U8;
    if (first >= last) {
      T = first;
      while (1) {
        if (set_in(T, 0, FITTYPES.bits)) RESULT = NAMES[T];
        if (T == last) break;
        --T;
      }
    }
  } while(0);
  return RESULT;
}

void OUTTYPEREFERENCE(TPSTYPE* TYPEPTR) {
  TPSTYPE* THETYPE;
  if (TYPEPTR == PNil) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "void");
  else if (TYPEPTR->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEPTR->POINTEDTYPEPTR);
    Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '*');
  }
  else if (TYPEPTR->CLS == TTCBOOLEAN) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "PBoolean");
  else if (TYPEPTR->CLS == TTCINTEGER) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "PInteger");
  else if (TYPEPTR->CLS == TTCREAL) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "PReal");
  else if (TYPEPTR->CLS == TTCCHAR) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "PChar");
  else if (TYPEPTR->CLS == TTCSTRING) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "PString");
  else if (TYPEPTR->CLS == TTCTEXT) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "PFile");
  else if (TYPEPTR->CLS == TTCENUM) {
    if (TYPEPTR->ENUMPTR->HASBEENDEFINED && cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &TYPEPTR->NAME);
    else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 9, "enum enum", RwpInt | RwpEnd, TYPEPTR->ENUMPTR->ID);
  }
  else if (TYPEPTR->CLS == TTCRANGE) Write(&CODEGEN.OUTPUT, 1, RwpString | RwpEnd, _GETRANGETYPE(TYPEPTR));
  else if (TYPEPTR->CLS == TTCSET) _OUTSETTYPENAME(TYPEPTR);
  else if (TYPEPTR->CLS == TTCRECORD) {
    if (TYPEPTR->RECPTR->HASBEENDEFINED && cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &TYPEPTR->NAME);
    else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 13, "struct record", RwpInt | RwpEnd, TYPEPTR->RECPTR->ID);
  }
  else if (TYPEPTR->CLS == TTCARRAY) {
    THETYPE = TYPEPTR;
    while (ISARRAYTYPE(THETYPE)) THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
    OUTTYPEREFERENCE(THETYPE);
    THETYPE = TYPEPTR;
    while (ISARRAYTYPE(THETYPE)) {
      Write(&CODEGEN.OUTPUT, 1, RwpChar, '[', RwpInt, GETBOUNDEDTYPESIZE(THETYPE->ARRAYDEF.INDEXTYPEPTR), RwpChar | RwpEnd, ']');
      THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
    }
  }
  else INTERNALERROR(CONCAT(CpLenPtr, 30, "Error writing type reference: ", CpEnd | CpString, TYPENAME(TYPEPTR)));
}

void OUTNAMEANDTYPE(const PString *NAME, TPSTYPE* TYPEPTR);

void OUTNAMEANDRECORD(const PString *NAME, TPSRECORDDEF* RECPTR) {
  PInteger POS;
  PInteger NUMVARIANT;
  NUMVARIANT = 0;
  if (RECPTR->HASBEENDEFINED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 13, "struct record", RwpInt | RwpEnd, RECPTR->ID);
  else {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "struct ");
    if (RECPTR->ISPACKED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 28, "__attribute__((__packed__)) ");
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 6, "record", RwpInt, RECPTR->ID, RwpChar | RwpEnd, ' ');
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
                Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "union ");
                OUTBEGIN();
              }
              else {
                OUTENDSAMELINE();
                Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
                _OUTNEWLINE();
              }
              _OUTINDENT();
              Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "struct ");
              OUTBEGIN();
            }
            _OUTINDENT();
            OUTNAMEANDTYPE(&RECPTR->FIELDS[subrange(POS, 1, 32) - 1].NAME, RECPTR->FIELDS[subrange(POS, 1, 32) - 1].TYPEPTR);
            Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
            _OUTNEWLINE();
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    if (NUMVARIANT > 0) {
      OUTENDSAMELINE();
      Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
      _OUTNEWLINE();
      OUTENDSAMELINE();
      Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
      _OUTNEWLINE();
    }
    OUTENDSAMELINE();
    RECPTR->HASBEENDEFINED = 1;
  }
  Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpStringPtr | RwpEnd, NAME);
}

void OUTNAMEANDENUM(const PString *NAME, TPSENUMDEF* ENUMPTR) {
  PInteger POS;
  if (ENUMPTR->HASBEENDEFINED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 9, "enum enum", RwpInt | RwpEnd, ENUMPTR->ID);
  else {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 37, "enum __attribute__((__packed__)) enum", RwpInt, ENUMPTR->ID, RwpLenPtr | RwpEnd, 3, " { ");
    do {
      PInteger first = 0;
      PInteger last = ENUMPTR->SIZE - 1;
      if (first <= last) {
        POS = first;
        while (1) {
          {
            if (POS > 0) _OUTCOMMA();
            Write(&CODEGEN.OUTPUT, 1, RwpStringPtr | RwpEnd, &ENUMPTR->VALUES[subrange(POS, 0, 127)]);
          }
          if (POS == last) break;
          ++POS;
        }
      }
    } while(0);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
    ENUMPTR->HASBEENDEFINED = 1;
  }
  Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpStringPtr | RwpEnd, NAME);
}

void OUTNAMEANDARRAY(const PString *NAME, TPSTYPE* TYPEPTR) {
  TPSTYPE* THETYPE;
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
  OUTNAMEANDTYPE(NAME, THETYPE);
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) {
    Write(&CODEGEN.OUTPUT, 1, RwpChar, '[', RwpInt, GETBOUNDEDTYPESIZE(THETYPE->ARRAYDEF.INDEXTYPEPTR), RwpChar | RwpEnd, ']');
    THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
  }
}

void OUTNAMEANDFUNCTION(const PString *NAME, TPSTYPE* TYPEPTR) {
  PInteger POS;
  {
    PString tmp1 = CONCAT(CpLenPtr, 2, "(*", CpStringPtr, NAME, CpEnd | CpChar, ')');
    OUTNAMEANDTYPE(&tmp1, TYPEPTR->FNDEFPTR->RETURNTYPEPTR);
  }
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '(');
  do {
    PInteger first = 1;
    PInteger last = TYPEPTR->FNDEFPTR->ARGS.COUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) _OUTCOMMA();
          OUTVARIABLEDECLARATION(TYPEPTR->FNDEFPTR->ARGS.DEFS[subrange(POS, 1, 16) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void OUTNAMEANDTYPE(const PString *NAME, TPSTYPE* TYPEPTR) {
  if (TYPEPTR == PNil) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 5, "void ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCPOINTER) {
    OUTTYPEREFERENCE(TYPEPTR->POINTEDTYPEPTR);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 2, "* ", RwpStringPtr | RwpEnd, NAME);
  }
  else if (TYPEPTR->ALIASFOR != PNil && cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) Write(&CODEGEN.OUTPUT, 1, RwpStringPtr, &TYPEPTR->NAME, RwpChar, ' ', RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCBOOLEAN) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 9, "PBoolean ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCINTEGER) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 9, "PInteger ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCREAL) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 6, "PReal ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCCHAR) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 6, "PChar ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCSTRING) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 8, "PString ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCTEXT) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 6, "PFile ", RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCENUM) OUTNAMEANDENUM(NAME, TYPEPTR->ENUMPTR);
  else if (TYPEPTR->CLS == TTCRANGE) Write(&CODEGEN.OUTPUT, 1, RwpString, _GETRANGETYPE(TYPEPTR), RwpChar, ' ', RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == TTCSET) {
    _OUTSETTYPENAME(TYPEPTR);
    Write(&CODEGEN.OUTPUT, 1, RwpChar, ' ', RwpStringPtr | RwpEnd, NAME);
  }
  else if (TYPEPTR->CLS == TTCRECORD) OUTNAMEANDRECORD(NAME, TYPEPTR->RECPTR);
  else if (TYPEPTR->CLS == TTCARRAY) OUTNAMEANDARRAY(NAME, TYPEPTR);
  else if (TYPEPTR->CLS == TTCFUNCTION) OUTNAMEANDFUNCTION(NAME, TYPEPTR);
  else INTERNALERROR(CONCAT(CpLenPtr, 29, "Error writing name and type: ", CpStringPtr, NAME, CpLenPtr, 2, ", ", CpEnd | CpString, TYPENAME(TYPEPTR)));
}

void OUTTYPEDEFINITION(TPSTYPE* TYPEPTR) {
  PString NAME;
  _OUTBLANKLINE(TOTTYPE);
  _OUTINDENT();
  NAME = TYPEPTR->NAME;
  if (TYPEPTR->ALIASFOR == PNil) INTERNALERROR(CONCAT(CpLenPtr, 5, "Type ", CpStringPtr, &NAME, CpEnd | CpLenPtr, 16, " is not an alias"));
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "typedef ");
  OUTNAMEANDTYPE(&NAME, TYPEPTR->ALIASFOR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTTYPEDEFINITIONSFROMCHECKPOINT(TPSDEFENTRY* CHECKPOINT) {
  TPSDEFENTRY* DEF;
  DEF = CHECKPOINT->NEXT;
  while (DEF != PNil) {
    if (DEF->CLS == TDCTYPE) {
      _RESOLVEPOINTERUNKNOWN(DEF->TYPEPTR);
      if (DEF->TYPEPTR->ALIASFOR != PNil) OUTTYPEDEFINITION(DEF->TYPEPTR);
    }
    DEF = DEF->NEXT;
  }
}

void OUTCONSTANTARRAYBEGIN() {
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "{ ");
}

void OUTCONSTANTARRAYSEPARATOR() {
  _OUTCOMMA();
}

void OUTCONSTANTARRAYEND() {
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
}

void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF) {
  PString NAME;
  NAME = VARDEF.NAME;
  if (VARDEF.ISCONSTANT) {
    if (VARDEF.ISREFERENCE && ISPOINTERTYPE(VARDEF.TYPEPTR)) NAME = CONCAT(CpLenPtr, 7, " const ", CpEnd | CpStringPtr, &NAME);
    else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "const ");
  }
  if (VARDEF.ISREFERENCE) NAME = CONCAT(CpChar, '*', CpEnd | CpStringPtr, &NAME);
  OUTNAMEANDTYPE(&NAME, VARDEF.TYPEPTR);
}

void OUTVARIABLEDEFINITION(TPSVARIABLE* VARPTR) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTCONSTANTDEFINITIONBEGIN(TPSVARIABLE* VARPTR) {
  _OUTBLANKLINE(TOTVAR);
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
}

void OUTCONSTANTDEFINITIONEND() {
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF) {
  PInteger POS;
  _OUTINDENT();
  OUTNAMEANDTYPE(&DEF.EXTERNALNAME, DEF.RETURNTYPEPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '(');
  do {
    PInteger first = 1;
    PInteger last = DEF.ARGS.COUNT;
    if (first <= last) {
      POS = first;
      while (1) {
        {
          if (POS != 1) _OUTCOMMA();
          OUTVARIABLEDECLARATION(DEF.ARGS.DEFS[subrange(POS, 1, 16) - 1]);
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void OUTFUNCTIONDECLARATION(TPSFUNCTION* FNPTR) {
  _OUTBLANKLINE(TOTFUNDEC);
  OUTFUNCTIONPROTOTYPE(*FNPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTFUNCTIONDEFINITION(TPSFUNCTION* FNPTR) {
  _OUTBLANKLINE(TOTFUNDEF);
  OUTFUNCTIONPROTOTYPE(*FNPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ' ');
  OUTBEGIN();
}

void OUTFUNCTIONEND(TPSFUNCTION* FNPTR) {
  if (FNPTR->RETURNTYPEPTR != PNil) {
    _OUTINDENT();
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 14, "return RESULT;");
    _OUTNEWLINE();
  }
  OUTEND();
}

void OUTPROGRAMHEADING(PString NAME) {
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 12, "/* Program: ", RwpStringPtr, &NAME, RwpLenPtr | RwpEnd, 3, " */");
  _OUTNEWLINE();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 20, "#include \"pascual.h\"");
  _OUTNEWLINE();
}

PChar SHORTTYPENAME(TPSTYPE* TYPEPTR) {
  PChar RESULT;
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = 'b';
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = 'i';
  else if (ISREALTYPE(TYPEPTR)) RESULT = 'r';
  else if (ISCHARTYPE(TYPEPTR)) RESULT = 'c';
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = 's';
  else COMPILEERROR(CONCAT(CpLenPtr, 5, "Type ", CpString, TYPENAME(TYPEPTR), CpLenPtr, 26, " is not representable for ", CpEnd | CpLenPtr, 24, "READ, WRITE, STR, or VAL"));
  return RESULT;
}

void _OUTREAD(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* SRC;
  TEXREADARGS* READARG;
  PBoolean LINEFEED;
  TPSTYPE* TYPEPTR;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  LINEFEED = EXPR->PSEUDOFNCALL.PSEUDOFNPTR == PSEUDOFUNS.READLN;
  READARG = EXPR->PSEUDOFNCALL.READARGS;
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "Read(");
  _OUTADDRESS(SRC);
  _OUTCOMMA();
  OUTEXPRESSION(EXBOOLEANCONSTANT(OPTIONS.CHECKIORESULT));
  if (READARG == PNil) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpEnd");
    if (LINEFEED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
  }
  while (READARG != PNil) {
    TYPEPTR = GETFUNDAMENTALTYPE(READARG->ARG->TYPEPTR);
    switch (TYPEPTR->CLS) {
      case TTCINTEGER:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpInt");
        break;
      case TTCREAL:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpReal");
        break;
      case TTCCHAR:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpChar");
        break;
      case TTCSTRING:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 11, ", RwpString");
        break;
      default:
        {
          PString tmp1 = str_make(36, "Expression has invalid type for READ");
          ERRORFOREXPR(&tmp1, READARG->ARG);
        }
        break;
    }
    if (READARG->NEXT == PNil) {
      if (LINEFEED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " | RwpEnd");
    }
    _OUTCOMMA();
    _OUTADDRESS(READARG->ARG);
    READARG = READARG->NEXT;
  }
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
  _OUTNEWLINE();
}

void _OUTWRITE(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* DST;
  TEXWRITEARGS* WRITEARG;
  PBoolean LINEFEED;
  TPSTYPE* TYPEPTR;
  DST = EXPR->PSEUDOFNCALL.ARG1;
  LINEFEED = EXPR->PSEUDOFNCALL.PSEUDOFNPTR == PSEUDOFUNS.WRITELN;
  WRITEARG = EXPR->PSEUDOFNCALL.WRITEARGS;
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "Write(");
  _OUTADDRESS(DST);
  _OUTCOMMA();
  OUTEXPRESSION(EXBOOLEANCONSTANT(OPTIONS.CHECKIORESULT));
  if (WRITEARG == PNil) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpEnd");
    if (LINEFEED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
  }
  while (WRITEARG != PNil) {
    TYPEPTR = GETFUNDAMENTALTYPE(WRITEARG->ARG->TYPEPTR);
    switch (TYPEPTR->CLS) {
      case TTCBOOLEAN:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpBool");
        break;
      case TTCINTEGER:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpInt");
        break;
      case TTCREAL:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpReal");
        break;
      case TTCCHAR:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpChar");
        break;
      case TTCENUM:
        Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpEnum");
        break;
      case TTCSTRING:
        {
          if (EXISIMMEDIATE(WRITEARG->ARG)) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 11, ", RwpLenPtr");
          else if (WRITEARG->ARG->ISADDRESSABLE) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 14, ", RwpStringPtr");
          else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 11, ", RwpString");
        }
        break;
      default:
        {
          PString tmp1 = str_make(37, "Expression has invalid type for WRITE");
          ERRORFOREXPR(&tmp1, WRITEARG->ARG);
        }
        break;
    }
    if (WRITEARG->WIDTH != PNil) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 11, " | RwpWidth");
    if (ISREALTYPE(TYPEPTR) && WRITEARG->PREC != PNil) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, " | RwpPrec");
    if (WRITEARG->NEXT == PNil) {
      if (LINEFEED) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " | RwpEnd");
    }
    if (WRITEARG->WIDTH != PNil) {
      _OUTCOMMA();
      OUTEXPRESSION(WRITEARG->WIDTH);
    }
    if (ISREALTYPE(TYPEPTR) && WRITEARG->PREC != PNil) {
      _OUTCOMMA();
      OUTEXPRESSION(WRITEARG->PREC);
    }
    _OUTCOMMA();
    if (ISSTRINGTYPE(WRITEARG->ARG->TYPEPTR) && EXISIMMEDIATE(WRITEARG->ARG)) {
      Write(&CODEGEN.OUTPUT, 1, RwpInt | RwpEnd, LENGTH(&WRITEARG->ARG->IMMEDIATE.STRINGVAL));
      _OUTCOMMA();
      _OUTCSTRING(&WRITEARG->ARG->IMMEDIATE.STRINGVAL);
    }
    else if (ISSTRINGTYPE(WRITEARG->ARG->TYPEPTR) && WRITEARG->ARG->ISADDRESSABLE) _OUTADDRESS(WRITEARG->ARG);
    else OUTEXPRESSION(WRITEARG->ARG);
    if (ISENUMTYPE(TYPEPTR)) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 12, ", enumvalues", RwpInt | RwpEnd, TYPEPTR->ENUMPTR->ID);
    WRITEARG = WRITEARG->NEXT;
  }
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
  _OUTNEWLINE();
}

void _OUTSIZEOF(TEXPRESSIONOBJ* EXPR) {
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "sizeof(");
  if (EXPR->PSEUDOFNCALL.ARG1 != PNil) OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
  else OUTTYPEREFERENCE(EXPR->PSEUDOFNCALL.TYPEARG);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _OUTSTR(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* SRC;
  TEXPRESSIONOBJ* DST;
  TEXPRESSIONOBJ* WIDTH;
  TEXPRESSIONOBJ* PREC;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  DST = EXPR->PSEUDOFNCALL.ARG2;
  WIDTH = EXPR->PSEUDOFNCALL.ARG3;
  PREC = EXPR->PSEUDOFNCALL.ARG4;
  if (ISENUMTYPE(SRC->TYPEPTR)) {
    _OUTINDENT();
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "STR_e(");
    OUTEXPRESSION(SRC);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 12, ", enumvalues", RwpInt | RwpEnd, SRC->TYPEPTR->ENUMPTR->ID);
    _OUTCOMMA();
    if (WIDTH != PNil) OUTEXPRESSION(WIDTH);
    else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '0');
    _OUTCOMMA();
    _OUTADDRESS(DST);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
    _OUTNEWLINE();
  }
  else if (ISREALTYPE(SRC->TYPEPTR)) {
    _OUTINDENT();
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "STR_r(");
    OUTEXPRESSION(SRC);
    _OUTCOMMA();
    if (WIDTH != PNil) OUTEXPRESSION(WIDTH);
    else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '0');
    _OUTCOMMA();
    if (PREC != PNil) OUTEXPRESSION(PREC);
    else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "-1");
    _OUTCOMMA();
    _OUTADDRESS(DST);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 4, "STR_", RwpChar, SHORTTYPENAME(SRC->TYPEPTR), RwpChar | RwpEnd, '(');
    OUTEXPRESSION(SRC);
    _OUTCOMMA();
    if (WIDTH != PNil) OUTEXPRESSION(WIDTH);
    else Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, '0');
    _OUTCOMMA();
    _OUTADDRESS(DST);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
    _OUTNEWLINE();
  }
}

void _OUTVAL(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* SRC;
  TEXPRESSIONOBJ* DST;
  TEXPRESSIONOBJ* CODE;
  TEXPRESSIONOBJ* TMPEXPR;
  SRC = EXPR->PSEUDOFNCALL.ARG1;
  DST = EXPR->PSEUDOFNCALL.ARG2;
  CODE = EXPR->PSEUDOFNCALL.ARG3;
  if (ISENUMTYPE(DST->TYPEPTR)) {
    _OUTINDENT();
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "VAL_e(");
    _OUTADDRESS(SRC);
    _OUTCOMMA();
    _OUTADDRESS(DST);
    _OUTCOMMA();
    TMPEXPR = EXINTEGERCONSTANT(DST->TYPEPTR->ENUMPTR->SIZE);
    OUTEXPRESSION(TMPEXPR);
    EXDISPOSE(&TMPEXPR);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 12, ", enumvalues", RwpInt | RwpEnd, DST->TYPEPTR->ENUMPTR->ID);
    _OUTCOMMA();
    _OUTADDRESS(CODE);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
    _OUTNEWLINE();
  }
  else {
    _OUTINDENT();
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr, 4, "VAL_", RwpChar, SHORTTYPENAME(DST->TYPEPTR), RwpChar | RwpEnd, '(');
    _OUTADDRESS(SRC);
    _OUTCOMMA();
    _OUTADDRESS(DST);
    _OUTCOMMA();
    _OUTADDRESS(CODE);
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
    _OUTNEWLINE();
  }
}

void _OUTNEW(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* PTR;
  PTR = EXPR->PSEUDOFNCALL.ARG1;
  _OUTINDENT();
  OUTEXPRESSION(PTR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 17, " = malloc(sizeof(");
  OUTTYPEREFERENCE(PTR->TYPEPTR->POINTEDTYPEPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "));");
  _OUTNEWLINE();
}

void _OUTDISPOSE(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* PTR;
  PTR = EXPR->PSEUDOFNCALL.ARG1;
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "free(");
  OUTEXPRESSION(PTR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
  _OUTNEWLINE();
}

void _OUTORD(TEXPRESSIONOBJ* EXPR) {
  ENSUREORDINALEXPR(EXPR->PSEUDOFNCALL.ARG1);
  if (ISCHARTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "(int)");
    _OUTEXPRESSIONPARENSPREC(EXPR->PSEUDOFNCALL.ARG1, 2);
  }
  else OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
}

void _OUTPRED(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* TMPEXPR;
  ENSUREORDINALEXPR(EXPR->PSEUDOFNCALL.ARG1);
  if (ISBOUNDEDTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    if (OPTIONS.CHECKBOUNDS) {
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "pred(");
      OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
      _OUTCOMMA();
      _OUTBOUNDS(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR);
      Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
    }
    else {
      TMPEXPR = EXOPSUB(PFORD(EXCOPY(EXPR->PSEUDOFNCALL.ARG1)), EXINTEGERCONSTANT(1));
      OUTEXPRESSION(TMPEXPR);
      EXDISPOSE(&TMPEXPR);
    }
  }
  else {
    TMPEXPR = EXOPSUB(EXCOPY(EXPR->PSEUDOFNCALL.ARG1), EXINTEGERCONSTANT(1));
    OUTEXPRESSION(TMPEXPR);
    EXDISPOSE(&TMPEXPR);
  }
}

void _OUTSUCC(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* TMPEXPR;
  ENSUREORDINALEXPR(EXPR->PSEUDOFNCALL.ARG1);
  if (ISBOUNDEDTYPE(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR)) {
    if (OPTIONS.CHECKBOUNDS) {
      Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "succ(");
      OUTEXPRESSION(EXPR->PSEUDOFNCALL.ARG1);
      _OUTCOMMA();
      _OUTBOUNDS(EXPR->PSEUDOFNCALL.ARG1->TYPEPTR);
      Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ')');
    }
    else {
      TMPEXPR = EXOPADD(PFORD(EXCOPY(EXPR->PSEUDOFNCALL.ARG1)), EXINTEGERCONSTANT(1));
      OUTEXPRESSION(TMPEXPR);
      EXDISPOSE(&TMPEXPR);
    }
  }
  else {
    TMPEXPR = EXOPADD(EXCOPY(EXPR->PSEUDOFNCALL.ARG1), EXINTEGERCONSTANT(1));
    OUTEXPRESSION(TMPEXPR);
    EXDISPOSE(&TMPEXPR);
  }
}

void OUTASSIGN(TEXPRESSIONOBJ* LHS, TEXPRESSIONOBJ* RHS) {
  _OUTINDENT();
  OUTEXPRESSION(LHS);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
  OUTEXPRESSION(RHS);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTDECLAREANDASSIGN(TPSVARIABLE* VARPTR, TEXPRESSIONOBJ* RHS) {
  _OUTINDENT();
  OUTVARIABLEDECLARATION(*VARPTR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
  if (VARPTR->ISREFERENCE) _OUTADDRESS(RHS);
  else OUTEXPRESSION(RHS);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTIF(TEXPRESSIONOBJ* EXPR) {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "if (");
  OUTEXPRESSION(EXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ") ");
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTELSE() {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "else ");
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

void OUTCASEBEGIN(TEXPRESSIONOBJ* CASEINDEX) {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "switch (");
  OUTEXPRESSION(CASEINDEX);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ") ");
  OUTBEGIN();
}

void OUTCASESTATEMENTBEGIN(TEXPRESSIONOBJ* CASELABEL) {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "case ");
  OUTEXPRESSION(CASELABEL);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ':');
  Write(&CODEGEN.OUTPUT, 1, RwpEnd);
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASESTATEMENTEND() {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "break;");
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEELSEBEGIN() {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "default:");
  CODEGEN.INDENT = CODEGEN.INDENT + 1;
  _OUTNEWLINE();
}

void OUTCASEELSEEND() {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "break;");
  CODEGEN.INDENT = CODEGEN.INDENT - 1;
  _OUTNEWLINE();
}

void OUTCASEEND() {
  OUTEND();
}

void OUTREPEATBEGIN() {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "do ");
  OUTBEGIN();
}

void OUTREPEATEND(TEXPRESSIONOBJ* EXPR) {
  TEXPRESSIONOBJ* TMPEXPR;
  OUTENDSAMELINE();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " while (");
  TMPEXPR = EXOPNOT(EXCOPY(EXPR));
  OUTEXPRESSION(TMPEXPR);
  EXDISPOSE(&TMPEXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
  _OUTNEWLINE();
}

void OUTWHILEBEGIN(TEXPRESSIONOBJ* EXPR) {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "while (");
  OUTEXPRESSION(EXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ") ");
  CODEGEN.ISMULTISTATEMENT = 0;
}

void OUTWHILEEND() {
  CODEGEN.ISMULTISTATEMENT = 1;
}

void OUTFORBEGIN(TEXPRESSIONOBJ* ITER, TEXPRESSIONOBJ* FIRSTEXPR, TEXPRESSIONOBJ* LASTEXPR, PBoolean ASCENDING) {
  TPSTYPE* LIMITTYPE;
  TPSVARIABLE FIRST;
  TPSVARIABLE LAST;
  LIMITTYPE = ITER->TYPEPTR;
  if (ISENUMTYPE(LIMITTYPE)) LIMITTYPE = PRIMITIVETYPES.PTINTEGER;
  FIRST = ({ PString tmp1 = str_make(5, "first"); MAKEVARIABLE(&tmp1, LIMITTYPE); });
  LAST = ({ PString tmp2 = str_make(4, "last"); MAKEVARIABLE(&tmp2, LIMITTYPE); });
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "do ");
  OUTBEGIN();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(FIRST);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
  OUTEXPRESSION(FIRSTEXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
  _OUTINDENT();
  OUTVARIABLEDECLARATION(LAST);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
  OUTEXPRESSION(LASTEXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "if (first ");
  if (ASCENDING) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "<=");
  else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ">=");
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 7, " last) ");
  OUTBEGIN();
  _OUTINDENT();
  OUTEXPRESSION(ITER);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " = first;");
  _OUTNEWLINE();
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "while (1) ");
  OUTBEGIN();
}

void OUTFOREND(TEXPRESSIONOBJ* ITER, PBoolean ASCENDING) {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "if (");
  OUTEXPRESSION(ITER);
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 16, " == last) break;");
  _OUTNEWLINE();
  _OUTINDENT();
  if (ASCENDING) Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "++");
  else Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "--");
  OUTEXPRESSION(ITER);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
  OUTEND();
  OUTEND();
  OUTENDSAMELINE();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 10, " while(0);");
  _OUTNEWLINE();
}

void OUTPROCEDURECALL(TEXPRESSIONOBJ* EXPR) {
  _OUTINDENT();
  OUTEXPRESSION(EXPR);
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTPSEUDOPROCCALL(TEXPRESSIONOBJ* EXPR) {
  _OUTEXPSEUDOFNCALL(EXPR);
}

void OUTEMPTYSTATEMENT() {
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpChar | RwpEnd, ';');
  _OUTNEWLINE();
}

void OUTPROGRAMBEGIN() {
  _OUTBLANKLINE(TOTFUNDEF);
  _OUTINDENT();
  Write(&CODEGEN.OUTPUT, 1, RwpLenPtr | RwpEnd, 20, "void pascual_main() ");
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
  Assign(&CODEGEN.OUTPUT, &FILENAME, 1);
  Rewrite(&CODEGEN.OUTPUT, 0, 1);
}

void COMPILEERROR(PString MSG) {
  Write(&STDERR, 1, RwpStringPtr, &MSG, RwpString | RwpLn | RwpEnd, LXWHERESTR());
  HALT(1);
}

void INTERNALERROR(PString MSG) {
  Write(&STDERR, 1, RwpLenPtr, 17, "Internal error : ", RwpStringPtr, &MSG, RwpString | RwpLn | RwpEnd, LXWHERESTR());
  HALT(1);
}

void COMPILEWARNING(PString MSG) {
  if (!OPTIONS.SUPPRESSWARNINGS) Write(&STDERR, 1, RwpLenPtr, 9, "Warning: ", RwpStringPtr, &MSG, RwpString | RwpLn | RwpEnd, LXWHERESTR());
}

void USAGE(PString MSG) {
  if (cmp_str(CoNotEq, CpStringPtr, &MSG, CpLenPtr, 0, "")) {
    Write(&OUTPUT, 1, RwpStringPtr | RwpLn | RwpEnd, &MSG);
    Write(&OUTPUT, 1, RwpEnd | RwpLn);
  }
  Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 6, "Usage:");
  Write(&OUTPUT, 1, RwpString, PARAMSTR(0), RwpLenPtr | RwpLn | RwpEnd, 33, " input.pas [-o output.c] [-Wnone]");
  Write(&OUTPUT, 1, RwpEnd | RwpLn);
  Write(&OUTPUT, 1, RwpLenPtr, 48, "If you specify \"-\" as the input or output file, ", RwpLenPtr | RwpLn | RwpEnd, 26, "stdin/stdout will be used.");
  Write(&OUTPUT, 1, RwpEnd | RwpLn);
  Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 8, "Options:");
  Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 46, "   -Wnone    :- Suppress all warning messages.");
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
    if (MATCHES) RESULT = CONCAT(CpString, COPY(&STR, 1, BASELEN), CpEnd | CpStringPtr, &NEW);
  }
  return RESULT;
}

void PARSECMDLINE() {
  PInteger POS;
  PString INPUTFILE;
  PString OUTPUTFILE;
  PBoolean SUPPRESSWARNINGS;
  enum __attribute__((__packed__)) enum10 { FLAGNONE, FLAGOUTPUT } FLAG;
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
          if (PARAM.chr[1] == '-' && cmp_str(CoNotEq, CpStringPtr, &PARAM, CpChar, '-')) {
            if (cmp_str(CoEq, CpStringPtr, &PARAM, CpLenPtr, 2, "-o")) FLAG = FLAGOUTPUT;
            else if (cmp_str(CoEq, CpStringPtr, &PARAM, CpLenPtr, 6, "-Wnone")) SUPPRESSWARNINGS = 1;
            else if (cmp_str(CoEq, CpStringPtr, &PARAM, CpLenPtr, 2, "-h")) USAGE(str_make(0, ""));
            else USAGE(CONCAT(CpLenPtr, 16, "Unknown option: ", CpEnd | CpStringPtr, &PARAM));
          }
          else if (FLAG == FLAGOUTPUT) {
            if (cmp_str(CoNotEq, CpStringPtr, &OUTPUTFILE, CpLenPtr, 0, "")) USAGE(str_make(39, "Output file must be specified only once"));
            else OUTPUTFILE = PARAM;
            FLAG = FLAGNONE;
          }
          else {
            if (cmp_str(CoNotEq, CpStringPtr, &INPUTFILE, CpLenPtr, 0, "")) USAGE(str_make(38, "Input file must be specified only once"));
            else INPUTFILE = PARAM;
          }
        }
        if (POS == last) break;
        ++POS;
      }
    }
  } while(0);
  if (cmp_str(CoEq, CpStringPtr, &INPUTFILE, CpLenPtr, 0, "")) USAGE(str_make(28, "Input file must be specified"));
  if (cmp_str(CoEq, CpStringPtr, &OUTPUTFILE, CpLenPtr, 0, "")) {
    if (cmp_str(CoEq, CpStringPtr, &INPUTFILE, CpChar, '-')) OUTPUTFILE = str_of('-');
    else OUTPUTFILE = REPLACEEXTENSION(INPUTFILE, str_make(4, ".pas"), str_make(2, ".c"));
  }
  if (cmp_str(CoEq, CpStringPtr, &OUTPUTFILE, CpLenPtr, 0, "")) USAGE(str_make(29, "Output file must be specified"));
  if (cmp_str(CoNotEq, CpStringPtr, &INPUTFILE, CpChar, '-')) LXOPEN(INPUTFILE);
  if (cmp_str(CoNotEq, CpStringPtr, &OUTPUTFILE, CpChar, '-')) CODEGENSETOUTPUT(OUTPUTFILE);
  OPTIONS.SUPPRESSWARNINGS = SUPPRESSWARNINGS;
  OPTIONS.CHECKBOUNDS = 1;
  OPTIONS.CHECKIORESULT = 1;
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
  Close(&LEXER.INPUT.SRC, 1);
  Close(&CODEGEN.OUTPUT, 1);
}
