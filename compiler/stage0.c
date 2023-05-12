/* Program: PASCUAL */
#include "pascual.h"

typedef struct record31* TSEXPRESSION;
typedef struct record13* TSDTYPE;
typedef struct record19* TSDEFINITION;
typedef struct record15* TSDSUBROUTINE;
typedef struct record34* TSSTATEMENT;
typedef struct record32* TSSSEQUENCE;
typedef enum __attribute__((__packed__)) enum1 { SCTENUM, SCTRECORD, SCTTMPVAR } TSCOUNTERTYPE;
typedef struct record1 {
  PInteger ENUMCTR;
  PInteger RECORDCTR;
  PInteger TMPVARCTR;
} TSCOUNTERS;
typedef struct record2* TSSCOPE;
typedef struct record2 {
  struct record2* PARENT;
  struct record19* LATESTDEF;
  struct record15* CURRENTFN;
  TSCOUNTERS COUNTERS;
} TSSCOPEOBJ;
typedef struct record3* TSDCONSTANT;
typedef struct record3 {
  PString NAME;
  struct record31* VALUE;
} TSDCONSTANTDEF;
typedef enum __attribute__((__packed__)) enum2 { SDTFCNONE, SDTFCTEXT, SDTFCBINARY } TSDTFILECLASS;
typedef struct record4 {
  TSDTFILECLASS CLS;
  struct record13* TYPEPTR;
} TSDTFILE;
typedef struct record5* TSDTENUM;
typedef struct record5 {
  PInteger SIZE;
  PString VALUES[128];
  PInteger ID;
  PBoolean HASBEENDEFINED;
  PBoolean NEEDVALUEARRAY;
} TSDTENUMDEF;
typedef struct record6 {
  PInteger FIRST;
  PInteger LAST;
  struct record13* BASETYPEPTR;
} TSDTRANGE;
typedef struct record8* TSDTRECORD;
typedef struct record7 {
  PString NAME;
  struct record13* TYPEPTR;
} TSDTRECORDFIELD;
typedef struct record8 {
  PInteger SIZE;
  TSDTRECORDFIELD FIELDS[64];
  PInteger NUMVARIANTS;
  PInteger VARIANTBOUNDS[64];
  PBoolean ISPACKED;
  PInteger ID;
  PBoolean HASBEENDEFINED;
} TSDTRECORDDEF;
typedef struct record11* TSDTSUBROUTINE;
typedef struct record9 {
  PString NAME;
  struct record13* TYPEPTR;
  PBoolean ISREFERENCE;
  PBoolean ISCONSTANT;
} TSDSUBROUTINEARG;
typedef struct record10 {
  PInteger COUNT;
  TSDSUBROUTINEARG DEFS[16];
} TSDSUBROUTINEARGS;
typedef struct record11 {
  TSDSUBROUTINEARGS ARGS;
  struct record13* RETURNTYPEPTR;
} TSDTSUBROUTINEDEF;
typedef struct record12 {
  struct record13* INDEXTYPEPTR;
  struct record13* VALUETYPEPTR;
} TSDTARRAY;
typedef enum __attribute__((__packed__)) enum3 { SDTCBOOLEAN, SDTCINTEGER, SDTCREAL, SDTCCHAR, SDTCSTRING, SDTCFILE, SDTCENUM, SDTCRANGE, SDTCSET, SDTCRECORD, SDTCARRAY, SDTCPOINTER, SDTCNIL, SDTCPOINTERFORWARD, SDTCFUNCTION } TSDTYPECLASS;
typedef struct record13 {
  PString NAME;
  struct record13* ALIASFOR;
  PBoolean WASUSED;
  TSDTYPECLASS CLS;
  union {
    struct {
      TSDTFILE FILEDEF;
    };
    struct {
      TSDTENUMDEF* ENUMPTR;
    };
    struct {
      TSDTRANGE RANGEDEF;
    };
    struct {
      struct record13* ELEMENTTYPEPTR;
    };
    struct {
      TSDTRECORDDEF* RECPTR;
    };
    struct {
      TSDTARRAY ARRAYDEF;
    };
    struct {
      struct record13* POINTEDTYPEPTR;
    };
    struct {
      PString* TARGETNAME;
    };
    struct {
      TSDTSUBROUTINEDEF* FNDEFPTR;
    };
  };
} TSDTYPEDEF;
typedef struct record14* TSDVARIABLE;
typedef struct record14 {
  PString NAME;
  TSDTYPEDEF* TYPEPTR;
  PBoolean ISREFERENCE;
  struct record31* LOCATION;
  PBoolean ISCONSTANT;
  struct record31* CONSTANTVALUE;
  PBoolean ISARGUMENT;
  PBoolean WASINITIALIZED;
  PBoolean WASUSED;
  struct record31* ISALIASFOR;
} TSDVARIABLEDEF;
typedef struct record15 {
  PString NAME;
  PString EXTERNALNAME;
  TSDSUBROUTINEARGS ARGS;
  TSDTYPEDEF* RETURNTYPEPTR;
  TSSCOPEOBJ SCOPE;
  struct record32* BODY;
  PBoolean ISDECLARATION;
  PBoolean HADDECLARATION;
  PBoolean WASUSED;
} TSDSUBROUTINEDEF;
typedef struct record16* TSDPSFN;
typedef struct record31* (*TSDPSFNPARSER)(struct record31* FNEXPR);
typedef struct record16 {
  PString NAME;
  TSDPSFNPARSER PARSEFN;
} TSDPSFNDEF;
typedef struct record17* TSDTMPVAR;
typedef struct record17 {
  TSDVARIABLEDEF VARDEF;
  PBoolean INUSE;
} TSDTMPVARDEF;
typedef struct record18* TSDWITHVAR;
typedef struct record18 {
  TSDTMPVARDEF* TMPVARPTR;
  PBoolean ISACTIVE;
} TSDWITHVARDEF;
typedef enum __attribute__((__packed__)) enum4 { SDCTYPE, SDCCONSTANT, SDCVARIABLE, SDCSUBROUTINE, SDCPSFN, SDCTMPVAR, SDCWITHVAR } TSDEFCLASS;
typedef struct record19 {
  struct record19* OLDER;
  struct record19* NEWER;
  TSDEFCLASS CLS;
  union {
    struct {
      TSDTYPEDEF TYPEDEF;
    };
    struct {
      TSDCONSTANTDEF CONSTDEF;
    };
    struct {
      TSDVARIABLEDEF VARDEF;
    };
    struct {
      TSDSUBROUTINEDEF SRDEF;
    };
    struct {
      TSDPSFNDEF PSFNDEF;
    };
    struct {
      TSDTMPVARDEF TMPVARDEF;
    };
    struct {
      TSDWITHVARDEF WITHVARDEF;
    };
  };
} TSDEFENTRY;
typedef struct record20* TSEARRAYELEM;
typedef struct record20 {
  struct record20* NEXT;
  struct record31* VALUE;
} TSEARRAYELEMOBJ;
typedef struct record21* TSERECORDFIELD;
typedef struct record21 {
  struct record21* NEXT;
  PInteger ORDINAL;
  struct record31* VALUE;
} TSERECORDFIELDOBJ;
typedef struct record22* TSESETIMMBOUNDS;
typedef struct record22 {
  struct record22* NEXT;
  PInteger FIRST;
  PInteger LAST;
} TSESETIMMBOUNDSOBJ;
typedef struct record23* TSESETEXPRBOUNDS;
typedef struct record23 {
  struct record23* NEXT;
  struct record31* FIRST;
  struct record31* LAST;
} TSESETEXPRBOUNDSOBJ;
typedef enum __attribute__((__packed__)) enum5 { SEICNIL, SEICBOOLEAN, SEICINTEGER, SEICREAL, SEICCHAR, SEICSTRING, SEICENUM, SEICSET } TSEIMMEDIATECLASS;
typedef struct record24 {
  TSEIMMEDIATECLASS CLS;
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
      TSDTENUMDEF* ENUMPTR;
    };
    struct {
      TSESETIMMBOUNDSOBJ* SETBOUNDS;
      TSDTYPEDEF* SETOFTYPEPTR;
    };
  };
} TSEIMMEDIATE;
typedef struct record25 {
  PInteger SIZE;
  struct record31* VALUES[16];
} TSEFUNCTIONARGS;
typedef struct record26* TSEREADARGLIST;
typedef struct record26 {
  struct record26* NEXT;
  struct record31* DEST;
} TSEREADARGVALUE;
typedef struct record28* TSEWRITEARGLIST;
typedef struct record27 {
  struct record31* ARG;
  struct record31* WIDTH;
  struct record31* PREC;
} TSEWRITEARG;
typedef struct record28 {
  struct record28* NEXT;
  TSEWRITEARG VALUE;
} TSEWRITEARGVALUE;
typedef enum __attribute__((__packed__)) enum6 { SEOADD, SEOSUB, SEOMUL, SEODIVREAL, SEODIVINT, SEOMOD, SEONEG, SEOAND, SEOOR, SEOXOR, SEOSHL, SEOSHR, SEONOT, SEOIN, SEOEQ, SEONE, SEOLT, SEOGT, SEOLTEQ, SEOGTEQ, SEOORD, SEOPRED, SEOSUCC } TSEOPERATOR;
typedef struct record29 {
  struct record31* PARENT;
  TSEOPERATOR OP;
} TSEUNARYOP;
typedef struct record30 {
  struct record31* LEFT;
  struct record31* RIGHT;
  TSEOPERATOR OP;
} TSEBINARYOP;
typedef enum __attribute__((__packed__)) enum7 { SECIMMEDIATE, SECARRAYVALUE, SECRECORDVALUE, SECSETVALUE, SECTOSTRING, SECTOREAL, SECTOUNTYPEDPTR, SECTOGENERICFILE, SECWITHTMPVAR, SECSUBRANGE, SECVARIABLE, SECFIELD, SECARRAY, SECPOINTER, SECADDRESS, SECSTRINGCHAR, SECFNREF, SECFNCALL, SECPSFNREF, SECSIZEOF, SECCONVERTTOSTR, SECCONVERTTOVAL, SECREAD, SECWRITE, SECUNARYOP, SECBINARYOP } TSEXPRESSIONCLASS;
typedef struct record31 {
  TSDTYPEDEF* TYPEPTR;
  PBoolean ISASSIGNABLE;
  PBoolean ISADDRESSABLE;
  PBoolean ISFUNCTIONRESULT;
  PBoolean ISSTATEMENT;
  PBoolean CHECKBOUNDS;
  TSEXPRESSIONCLASS CLS;
  union {
    struct {
      TSEIMMEDIATE IMMEDIATE;
    };
    struct {
      TSEARRAYELEMOBJ* ARRAYELEM;
    };
    struct {
      TSERECORDFIELDOBJ* RECORDFIELD;
    };
    struct {
      struct record31* SETBASE;
      TSESETEXPRBOUNDSOBJ* SETBOUNDS;
    };
    struct {
      struct record31* TOSTRPARENT;
    };
    struct {
      struct record31* TOREALPARENT;
    };
    struct {
      struct record31* TOUNTYPEDPTRPARENT;
    };
    struct {
      struct record31* TOGENERICFILEPARENT;
    };
    struct {
      TSDTMPVARDEF* TMPVARPTR;
      struct record31* TMPVARVALUE;
      struct record31* TMPVARCHILD;
    };
    struct {
      struct record31* SUBRANGEPARENT;
    };
    struct {
      TSDVARIABLEDEF* VARPTR;
    };
    struct {
      struct record31* RECEXPR;
      PInteger RECFIELDNUM;
    };
    struct {
      struct record31* ARRAYEXPR;
      struct record31* ARRAYINDEX;
    };
    struct {
      struct record31* POINTEREXPR;
    };
    struct {
      struct record31* ADDRESSEXPR;
    };
    struct {
      struct record31* STRINGEXPR;
      struct record31* STRINGINDEX;
    };
    struct {
      TSDSUBROUTINEDEF* FNPTR;
    };
    struct {
      struct record31* FNEXPR;
      TSEFUNCTIONARGS CALLARGS;
    };
    struct {
      TSDPSFNDEF* PSFNPTR;
    };
    struct {
      TSDTYPEDEF* SIZEOFTYPEPTR;
    };
    struct {
      TSEWRITEARG TOSTRSRC;
      struct record31* TOSTRDEST;
    };
    struct {
      struct record31* TOVALSRC;
      struct record31* TOVALDEST;
      struct record31* TOVALCODE;
    };
    struct {
      struct record31* READFILE;
      TSEREADARGVALUE* READARGS;
      PBoolean READLN;
      PBoolean CHECKIORESULTAFTERREAD;
    };
    struct {
      struct record31* WRITEFILE;
      TSEWRITEARGVALUE* WRITEARGS;
      PBoolean WRITELN;
      PBoolean CHECKIORESULTAFTERWRITE;
    };
    struct {
      TSEUNARYOP UNARY;
    };
    struct {
      TSEBINARYOP BINARY;
    };
  };
} TSEXPRESSIONOBJ;
typedef struct record32 {
  struct record32* NEXT;
  struct record34* STATEMENT;
} TSSSEQUENCEENTRY;
typedef struct record33* TSSCASE;
typedef struct record33 {
  struct record33* NEXT;
  TSEXPRESSIONOBJ* CASELABEL;
  struct record34* STATEMENT;
} TSSCASEENTRY;
typedef enum __attribute__((__packed__)) enum8 { SSCEMPTY, SSCSEQUENCE, SSCASSIGN, SSCPROCCALL, SSCIF, SSCREPEAT, SSCWHILE, SSCFOR, SSCWITH, SSCCASE } TSSTATEMENTCLASS;
typedef struct record34 {
  TSSTATEMENTCLASS CLS;
  union {
    struct {
      TSSSEQUENCEENTRY* SEQUENCE;
    };
    struct {
      TSEXPRESSIONOBJ* LHS;
      TSEXPRESSIONOBJ* RHS;
    };
    struct {
      TSEXPRESSIONOBJ* PROCCALL;
    };
    struct {
      TSEXPRESSIONOBJ* IFCOND;
      struct record34* IFTHEN;
      struct record34* IFELSE;
    };
    struct {
      TSEXPRESSIONOBJ* UNTILCOND;
      TSSSEQUENCEENTRY* REPEATSEQUENCE;
    };
    struct {
      TSEXPRESSIONOBJ* WHILECOND;
      struct record34* WHILESTATEMENT;
    };
    struct {
      TSEXPRESSIONOBJ* ITERATOR;
      TSEXPRESSIONOBJ* FIRST;
      TSEXPRESSIONOBJ* LAST;
      PBoolean ASCENDING;
      struct record34* FORSTATEMENT;
    };
    struct {
      TSDVARIABLEDEF* WITHVAR;
      TSEXPRESSIONOBJ* WITHVALUE;
      struct record34* WITHSTATEMENT;
    };
    struct {
      TSEXPRESSIONOBJ* CASESELECTOR;
      TSSCASEENTRY* CASEENTRY;
    };
  };
} TSSTATEMENTOBJ;
typedef struct record35* TSPROGRAM;
typedef struct record35 {
  PString NAME;
  TSSCOPEOBJ SCOPE;
  TSSSEQUENCEENTRY* BODY;
} TSPROGRAMOBJ;
typedef struct record36* TCODEGEN;
typedef struct record36 {
  void (*SETOUTPUTFILE)(struct record36* THIS, const PString* NAME);
  void (*GENERATE)(struct record36* THIS, TSPROGRAMOBJ* AST);
} TCODEGENBASE;
typedef struct record38* TSTACK;
typedef struct record38 {
  struct record38* OLDER;
  struct record38* NEWER;
} TSTACKPTRS;
typedef PBoolean (*TSTACKPREDICATE)(void* ITEM, void* CONTEXT);
typedef struct record39* TLIST;
typedef struct record39 {
  struct record39* NEXT;
} TLISTPTRS;
typedef TLISTPTRS** TLISTADDPOINT;
typedef enum __attribute__((__packed__)) enum9 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKINTEGER, TKREAL, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAT, TKABSOLUTE, TKAND, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKSHL, TKSHR, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH, TKXOR } TLXTOKENID;
typedef struct record40 {
  PInteger ROW;
  PInteger COL;
} TLXPOS;
typedef struct record41 {
  TLXTOKENID ID;
  PString VALUE;
  TLXPOS POS;
} TLXTOKEN;
typedef struct record42 {
  PFile SRC;
  PString NAME;
  TLXPOS POS;
} TLXINPUTFILE;
typedef struct record43* TLXINCLUDESTACK;
typedef struct record43 {
  TLXINPUTFILE INPUT;
  struct record43* PREV;
} TLXINCLUDESTACKELEM;
typedef struct record47 {
  PString NAME;
} TPSIDENTIFIER;
typedef enum __attribute__((__packed__)) enum10 { TOTNONE, TOTTYPE, TOTVAR, TOTENUMVAL, TOTFUNDEC, TOTFUNDEF } TCGC_OUTPUTTYPE;
typedef struct record48* TCGC;
typedef struct record48 {
  TCODEGENBASE BASE;
  PFile OUTPUT;
  PBoolean ISMULTISTATEMENT;
  PInteger INDENT;
  PBoolean NEWLINE;
  TCGC_OUTPUTTYPE LASTOUT;
} TCGC_OBJ;

struct record37 {
  PBoolean SUPPRESSWARNINGS;
  PBoolean CHECKBOUNDS;
  PBoolean CHECKIORESULT;
} OPTIONS;
TCODEGENBASE* CG;
struct record44 {
  PString LINE;
  TLXTOKEN TOKEN;
  TLXINPUTFILE INPUT;
  TLXINCLUDESTACKELEM* INCLUDESTACK;
} LEXER;
TSSCOPEOBJ* GLOBALDEFINITIONS;
TSSCOPEOBJ* CURRENTSCOPE;
struct record45 {
  TSDTYPEDEF* PTNIL;
  TSDTYPEDEF* PTBOOLEAN;
  TSDTYPEDEF* PTINTEGER;
  TSDTYPEDEF* PTREAL;
  TSDTYPEDEF* PTCHAR;
  TSDTYPEDEF* PTSTRING;
  TSDTYPEDEF* PTTEXT;
  TSDTYPEDEF* PTFILE;
  TSDTYPEDEF* PTEMPTYSET;
  TSDTYPEDEF* PTUNTYPEDPTR;
} PRIMITIVETYPES;
const PInteger _EXPRECEDENCES[26] = { 0, 0, 0, 0, -1, -1, -1, -1, -1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, -1, -1 };
const struct record46 {
  PInteger PRECEDENCE;
  PString NAME;
} _EXOPERATORS[23] = { { .PRECEDENCE = 4, .NAME = str_of('+') }, { .PRECEDENCE = 4, .NAME = str_of('-') }, { .PRECEDENCE = 4, .NAME = str_of('*') }, { .PRECEDENCE = 3, .NAME = str_of('/') }, { .PRECEDENCE = 3, .NAME = str_make(3, "DIV") }, { .PRECEDENCE = 3, .NAME = str_make(3, "MOD") }, { .PRECEDENCE = 4, .NAME = str_of('-') }, { .PRECEDENCE = 3, .NAME = str_make(3, "AND") }, { .PRECEDENCE = 4, .NAME = str_make(2, "OR") }, { .PRECEDENCE = 4, .NAME = str_make(3, "XOR") }, { .PRECEDENCE = 3, .NAME = str_make(3, "SHL") }, { .PRECEDENCE = 3, .NAME = str_make(3, "SHR") }, { .PRECEDENCE = 1, .NAME = str_make(3, "NOT") }, { .PRECEDENCE = 5, .NAME = str_make(2, "IN") }, { .PRECEDENCE = 5, .NAME = str_of('=') }, { .PRECEDENCE = 5, .NAME = str_make(2, "<>") }, { .PRECEDENCE = 5, .NAME = str_of('<') }, { .PRECEDENCE = 5, .NAME = str_of('>') }, { .PRECEDENCE = 5, .NAME = str_make(2, "<=") }, { .PRECEDENCE = 5, .NAME = str_make(2, ">=") }, { .PRECEDENCE = 1, .NAME = str_make(3, "ORD") }, { .PRECEDENCE = 1, .NAME = str_make(4, "PREC") }, { .PRECEDENCE = 1, .NAME = str_make(4, "SUCC") } };

const char* enumvalues3[] = { "SDTCBOOLEAN", "SDTCINTEGER", "SDTCREAL", "SDTCCHAR", "SDTCSTRING", "SDTCFILE", "SDTCENUM", "SDTCRANGE", "SDTCSET", "SDTCRECORD", "SDTCARRAY", "SDTCPOINTER", "SDTCNIL", "SDTCPOINTERFORWARD", "SDTCFUNCTION" };
const char* enumvalues9[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKINTEGER", "TKREAL", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAT", "TKABSOLUTE", "TKAND", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKSHL", "TKSHR", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH", "TKXOR" };

void COMPILEERROR(PString MSG);
void INTERNALERROR(PString MSG);
void COMPILEWARNING(PString MSG);
void EXDISPOSE(TSEXPRESSIONOBJ** EXPR);
TSEXPRESSIONOBJ* EXCOPY(TSEXPRESSIONOBJ* EXPR);
PString EXDESCRIBEOPERATOR(TSEOPERATOR OP);
PString EXDESCRIBE(TSEXPRESSIONOBJ* EXPR);
void EXMARKINITIALIZED(TSEXPRESSIONOBJ* LHS);
PInteger EXGETORDINAL(TSEXPRESSIONOBJ* EXPR);
TSEXPRESSIONOBJ* EXGETANTIORDINAL(PInteger ORDINAL, TSDTYPEDEF* TYPEPTR);
TSEXPRESSIONOBJ* EXOPNEG(TSEXPRESSIONOBJ* EXPR);
TSEXPRESSIONOBJ* EXOPADD(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPSUB(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPMUL(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPDIVREAL(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPDIVINT(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPMOD(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPNOT(TSEXPRESSIONOBJ* EXPR);
TSEXPRESSIONOBJ* EXOPAND(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPOR(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPXOR(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPSHL(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPSHR(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPIN(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPEQ(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPNE(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPLT(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPGT(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPLTEQ(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPGTEQ(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT);
TSEXPRESSIONOBJ* EXOPORD(TSEXPRESSIONOBJ* EXPR);
TSEXPRESSIONOBJ* EXOPPRED(TSEXPRESSIONOBJ* EXPR);
TSEXPRESSIONOBJ* EXOPSUCC(TSEXPRESSIONOBJ* EXPR);
PString TYPENAME(TSDTYPEDEF* TYPEPTR);
PBoolean ISUNTYPED(TSDTYPEDEF* TYPEPTR);
PBoolean ISENUMTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISORDINALTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISRECORDTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISARRAYTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISPOINTERTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISSTRINGYTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISSTRINGTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISINTEGERTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISNUMERICTYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISFILETYPE(TSDTYPEDEF* TYPEPTR);
PBoolean ISGENERICFILETYPE(TSDTYPEDEF* TYPEPTR);
TSEXPRESSIONOBJ* EXCOERCE(TSEXPRESSIONOBJ* EXPR, TSDTYPEDEF* TYPEPTR);
TSEXPRESSIONOBJ* EXSUBRANGE(TSEXPRESSIONOBJ* PARENT, TSDTYPEDEF* TYPEPTR);
void READTOKEN();
TSDTYPEDEF* PSTYPEDENOTER();
TSEXPRESSIONOBJ* PSEXPRESSION();
TSEXPRESSIONOBJ* PSVARIABLE();
TSSTATEMENTOBJ* PSSTATEMENT();
void PSDEFINITIONS();
TSEXPRESSIONOBJ* PSCONSTANTVALUE(TSDTYPEDEF* TYPEPTR);
void _CGC_OUTTYPEREFERENCE(TCGC_OBJ* THIS, TSDTYPEDEF* TYPEPTR);
void _CGC_OUTNAMEANDTYPE(TCGC_OBJ* THIS, const PString* NAME, TSDTYPEDEF* TYPEPTR);
void _CGC_OUTEXPRESSION(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR);
void _CGC_OUTBODY(TCGC_OBJ* THIS, TSSSEQUENCEENTRY* BODY);
void _CGC_OUTSTATEMENT(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT);
void _CGC_OUTLOCALDEFINITIONS(TCGC_OBJ* THIS, TSDEFENTRY* FIRSTDEF);

PString INTTOSTR(PInteger VALUE) {
  PString RESULT;
  STR_i(VALUE, 0, &RESULT);
  return RESULT;
}

void STACK_PUSH(void* HEAD, void* ITEM) {
  TSTACKPTRS** THEHEAD = (TSTACKPTRS**)HEAD;
  TSTACKPTRS** NEWITEM = (TSTACKPTRS**)ITEM;
  (*NEWITEM)->OLDER = *THEHEAD;
  (*NEWITEM)->NEWER = PNil;
  if (*THEHEAD != PNil) (*THEHEAD)->NEWER = *NEWITEM;
  *THEHEAD = *NEWITEM;
}

void STACK_GETOLDEST(void* HEAD, void* OLDESTITEM) {
  TSTACKPTRS** THEHEAD = (TSTACKPTRS**)HEAD;
  TSTACKPTRS** THEITEM = (TSTACKPTRS**)OLDESTITEM;
  *THEITEM = *THEHEAD;
  if (*THEHEAD != PNil) while ((*THEITEM)->OLDER != PNil) *THEITEM = (*THEITEM)->OLDER;
}

PBoolean STACK_FIND(void* HEAD, void* FOUNDITEM, TSTACKPREDICATE PREDICATE, void* CONTEXT) {
  PBoolean RESULT;
  TSTACKPTRS** THEHEAD = (TSTACKPTRS**)HEAD;
  TSTACKPTRS* ITEM;
  TSTACKPTRS** THEFOUNDITEM = (TSTACKPTRS**)FOUNDITEM;
  PBoolean FOUND;
  FOUND = 0;
  ITEM = *THEHEAD;
  while (!FOUND && ITEM != PNil) {
    FOUND = PREDICATE(&ITEM, CONTEXT);
    if (FOUND) *THEFOUNDITEM = ITEM;
    else ITEM = ITEM->OLDER;
  }
  RESULT = FOUND;
  return RESULT;
}

TLISTPTRS** LIST_GETADDPOINT(void* LIST) {
  TLISTPTRS** RESULT;
  TLISTPTRS** THELIST = (TLISTPTRS**)LIST;
  TLISTPTRS* TAIL;
  if (*THELIST == PNil) RESULT = THELIST;
  else {
    TAIL = *THELIST;
    while (TAIL->NEXT != PNil) TAIL = TAIL->NEXT;
    RESULT = &TAIL->NEXT;
  }
  return RESULT;
}

void LIST_ADD(void* ADDPOINT, void* ITEM) {
  TLISTPTRS*** THEADDPOINT = (TLISTPTRS***)ADDPOINT;
  TLISTPTRS** THEITEM = (TLISTPTRS**)ITEM;
  (*THEITEM)->NEXT = PNil;
  **THEADDPOINT = *THEITEM;
  *THEADDPOINT = &(*THEITEM)->NEXT;
}

PBoolean LIST_SHIFT(void* LIST, void* DELETEDITEM) {
  PBoolean RESULT;
  TLISTPTRS** THELIST = (TLISTPTRS**)LIST;
  TLISTPTRS** THEDELETEDITEM = (TLISTPTRS**)DELETEDITEM;
  RESULT = *THELIST != PNil;
  if (RESULT) {
    *THEDELETEDITEM = *THELIST;
    *THELIST = (*THELIST)->NEXT;
  }
  return RESULT;
}

PString ERRORDESCRIBEEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  RESULT = CONCAT(CpString, EXDESCRIBE(EXPR), CpLenPtr, 2, " (", CpString, TYPENAME(EXPR->TYPEPTR), CpEnd | CpChar, ')');
  return RESULT;
}

void ERRORINVALIDOPERATOR(TSEXPRESSIONOBJ* EXPR, TSEOPERATOR OP) {
  COMPILEERROR(CONCAT(CpLenPtr, 10, "Operator '", CpString, EXDESCRIBEOPERATOR(OP), CpLenPtr, 29, "' is not valid in expression ", CpEnd | CpString, ERRORDESCRIBEEXPR(EXPR)));
}

void ERRORINVALIDOPERATOR2(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  COMPILEERROR(CONCAT(CpLenPtr, 18, "Invalid operator '", CpString, EXDESCRIBEOPERATOR(OP), CpLenPtr, 6, "' for ", CpString, ERRORDESCRIBEEXPR(LEFT), CpLenPtr, 5, " and ", CpEnd | CpString, ERRORDESCRIBEEXPR(RIGHT)));
}

void ERRORFORTYPE(const PString* MSG, TSDTYPEDEF* GOT) {
  COMPILEERROR(CONCAT(CpStringPtr, MSG, CpLenPtr, 6, "; got ", CpEnd | CpString, TYPENAME(GOT)));
}

void ERRORFOREXPR(const PString* MSG, TSEXPRESSIONOBJ* GOT) {
  COMPILEERROR(CONCAT(CpStringPtr, MSG, CpLenPtr, 6, "; got ", CpEnd | CpString, ERRORDESCRIBEEXPR(GOT)));
}

void ENSURERECORDTYPE(TSDTYPEDEF* TYP) {
  PString tmp1;
  if (!ISRECORDTYPE(TYP)) {
    tmp1 = str_make(17, "Expected a record");
    ERRORFORTYPE(&tmp1, TYP);
  }
}

void ENSUREENUMTYPE(TSDTYPEDEF* TYP) {
  PString tmp1;
  if (!ISENUMTYPE(TYP)) {
    tmp1 = str_make(23, "Expected an enumeration");
    ERRORFORTYPE(&tmp1, TYP);
  }
}

void ENSUREORDINALTYPE(TSDTYPEDEF* TYP) {
  PString tmp1;
  if (!ISORDINALTYPE(TYP)) {
    tmp1 = str_make(19, "Expected an ordinal");
    ERRORFORTYPE(&tmp1, TYP);
  }
}

void ENSURETYPEDEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (ISUNTYPED(EXPR->TYPEPTR)) {
    tmp1 = str_make(27, "Expected a typed expression");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURERECORDEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISRECORDTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(17, "Expected a record");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREARRAYEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISARRAYTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(17, "Expected an array");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREPOINTEREXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISPOINTERTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(18, "Expected a pointer");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURESTRINGYEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISSTRINGYTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(17, "Expected a string");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURESTRINGEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISSTRINGTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(17, "Expected a string");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSURENUMERICEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISNUMERICTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(29, "Expected a numeric expression");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREINTEGEREXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISINTEGERTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(19, "Expected an integer");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREORDINALEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!ISORDINALTYPE(EXPR->TYPEPTR)) {
    tmp1 = str_make(19, "Expected an ordinal");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREASSIGNABLEEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!EXPR->ISASSIGNABLE) {
    tmp1 = str_make(19, "Expected a variable");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

void ENSUREADDRESSABLEEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString tmp1;
  if (!EXPR->ISADDRESSABLE) {
    tmp1 = str_make(29, "Expected an addressable value");
    ERRORFOREXPR(&tmp1, EXPR);
  }
}

PString LXTOKENNAME(TLXTOKENID ID) {
  PString RESULT;
  PString NAME;
  STR_e(ID, enumvalues9, 0, &NAME);
  RESULT = NAME;
  return RESULT;
}

PString LXPOSSTR(TLXPOS POS) {
  PString RESULT;
  RESULT = CONCAT(CpLenPtr, 4, "row ", CpString, INTTOSTR(POS.ROW), CpLenPtr, 5, " col ", CpString, INTTOSTR(POS.COL), CpLenPtr, 4, " in ", CpEnd | CpStringPtr, &LEXER.INPUT.NAME);
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
  typedef PBits8 KWTOKENS;
  const PString KEYWORDS[40] = { str_make(8, "ABSOLUTE"), str_make(3, "AND"), str_make(5, "ARRAY"), str_make(5, "BEGIN"), str_make(4, "CASE"), str_make(5, "CONST"), str_make(3, "DIV"), str_make(2, "DO"), str_make(6, "DOWNTO"), str_make(4, "ELSE"), str_make(3, "END"), str_make(4, "FILE"), str_make(3, "FOR"), str_make(7, "FORWARD"), str_make(8, "FUNCTION"), str_make(4, "GOTO"), str_make(2, "IF"), str_make(2, "IN"), str_make(5, "LABEL"), str_make(3, "MOD"), str_make(3, "NIL"), str_make(3, "NOT"), str_make(2, "OF"), str_make(2, "OR"), str_make(6, "PACKED"), str_make(9, "PROCEDURE"), str_make(7, "PROGRAM"), str_make(6, "RECORD"), str_make(6, "REPEAT"), str_make(3, "SET"), str_make(3, "SHL"), str_make(3, "SHR"), str_make(4, "THEN"), str_make(2, "TO"), str_make(4, "TYPE"), str_make(5, "UNTIL"), str_make(3, "VAR"), str_make(5, "WHILE"), str_make(4, "WITH"), str_make(3, "XOR") };
  PChar CHR;
  PInteger POS;
  KWTOKENS TOKEN;
  PBoolean INTOKEN;
  POS = 0;
  INTOKEN = 1;
  while (POS < LENGTH(&LEXER.LINE) && INTOKEN) {
    CHR = LEXER.LINE.chr[POS + 1];
    INTOKEN = '0' <= CHR && CHR <= '9' || 'A' <= CHR && CHR <= 'Z' || CHR == '_' || 'a' <= CHR && CHR <= 'z';
    if (INTOKEN) POS = POS + 1;
  }
  LXGETSYMBOL(TKIDENTIFIER, POS);
  for (PInteger first = 1, last = LENGTH(&LEXER.TOKEN.VALUE); first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) LEXER.TOKEN.VALUE.chr[POS] = UPCASE(LEXER.TOKEN.VALUE.chr[POS]);
    break;
  }
  for (PInteger first = TKABSOLUTE, last = TKXOR; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (TOKEN = first; !done; done = TOKEN == last ? 1 : (++TOKEN, 0)) if (LEXER.TOKEN.ID == TKIDENTIFIER && cmp_str(CoEq, CpStringPtr, &LEXER.TOKEN.VALUE, CpStringPtr, &KEYWORDS[TOKEN - 29])) LEXER.TOKEN.ID = TOKEN;
    break;
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
  enum __attribute__((__packed__)) enum10 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, CARET, DONE } STATE;
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
      else if ('0' <= CHR && CHR <= '9') {
        STATE = NUMCHARDEC;
        LAST = POS;
      }
      else STATE = DONE;
    }
    else if (STATE == NUMCHARDEC) {
      if ('0' <= CHR && CHR <= '9') LAST = POS;
      else if (CHR == '\'') STATE = QUOTEDSTR;
      else if (CHR == '#') STATE = HASH;
      else if (CHR == '^') STATE = CARET;
      else STATE = DONE;
    }
    else if (STATE == NUMCHARHEX) {
      if ('0' <= CHR && CHR <= '9' || 'A' <= CHR && CHR <= 'F' || 'a' <= CHR && CHR <= 'f') LAST = POS;
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

void LXGETSTRINGFROMCARET() {
  LEXER.LINE = CONCAT(CpChar, '^', CpEnd | CpStringPtr, &LEXER.LINE);
  LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL - 1;
  LXGETSTRING();
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
    else if ('A' <= CHR && CHR <= 'Z' || CHR == '_' || 'a' <= CHR && CHR <= 'z') LXGETIDENTIFIER();
    else if ('0' <= CHR && CHR <= '9') LXGETNUMBER();
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

PString _LXRESOLVEFILENAME(const PString* CURRENT, const PString* NEW) {
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
  PFile INPUTFILE = (PFile){.handle = PNil};
  LEXER.INPUT.NAME = FILENAME;
  Assign(&INPUTFILE, &LEXER.INPUT.NAME, 1);
  Reset(&INPUTFILE, 0, 1);
  LEXER.INPUT.SRC = INPUTFILE;
}

void LXINCLUDE(const PString* FILENAME) {
  TLXINCLUDESTACKELEM* NEWSTACK;
  New((void**)&NEWSTACK, sizeof(TLXINCLUDESTACKELEM));
  NEWSTACK->INPUT = LEXER.INPUT;
  NEWSTACK->PREV = LEXER.INCLUDESTACK;
  LEXER.INCLUDESTACK = NEWSTACK;
  LEXER.INPUT.POS.ROW = 0;
  LEXER.INPUT.POS.COL = 0;
  LXOPEN(_LXRESOLVEFILENAME(&LEXER.INPUT.NAME, FILENAME));
}

PInteger DEFCOUNTER(TSCOUNTERTYPE COUNTERTYPE) {
  PInteger RESULT;
  PInteger* CTR;
  switch (COUNTERTYPE) {
    case SCTENUM:
      CTR = &CURRENTSCOPE->COUNTERS.ENUMCTR;
      break;
    case SCTRECORD:
      CTR = &CURRENTSCOPE->COUNTERS.RECORDCTR;
      break;
    case SCTTMPVAR:
      CTR = &CURRENTSCOPE->COUNTERS.TMPVARCTR;
      break;
    default:
      break;
  }
  *CTR = *CTR + 1;
  RESULT = *CTR;
  return RESULT;
}

void PUSHGLOBALDEFS(TSSCOPEOBJ* DEFS) {
  DEFS->PARENT = PNil;
  DEFS->LATESTDEF = PNil;
  DEFS->COUNTERS.ENUMCTR = 0;
  DEFS->COUNTERS.RECORDCTR = 0;
  DEFS->COUNTERS.TMPVARCTR = 0;
  DEFS->CURRENTFN = PNil;
  CURRENTSCOPE = DEFS;
}

TSDTENUMDEF* NEWENUM(const TSDTENUMDEF* ENUM) {
  TSDTENUMDEF* RESULT;
  New((void**)&RESULT, sizeof(TSDTENUMDEF));
  *RESULT = *ENUM;
  RESULT->ID = DEFCOUNTER(SCTENUM);
  RESULT->HASBEENDEFINED = 0;
  RESULT->NEEDVALUEARRAY = 0;
  return RESULT;
}

TSDTRECORDDEF* NEWRECORD(const TSDTRECORDDEF* REC) {
  TSDTRECORDDEF* RESULT;
  New((void**)&RESULT, sizeof(TSDTRECORDDEF));
  *RESULT = *REC;
  RESULT->ID = DEFCOUNTER(SCTRECORD);
  RESULT->HASBEENDEFINED = 0;
  return RESULT;
}

TSDTSUBROUTINEDEF* NEWFNDEF() {
  TSDTSUBROUTINEDEF* RESULT;
  New((void**)&RESULT, sizeof(TSDTSUBROUTINEDEF));
  return RESULT;
}

PBoolean _HASUNUSEDPREFIX(const PString* NAME) {
  PBoolean RESULT;
  RESULT = LENGTH(NAME) > 7 && NAME->chr[1] == 'U' && NAME->chr[2] == 'N' && NAME->chr[3] == 'U' && NAME->chr[4] == 'S' && NAME->chr[5] == 'E' && NAME->chr[6] == 'D' && NAME->chr[7] == '_';
  return RESULT;
}

void _CHECKUNUSEDSYMBOLS(TSDEFENTRY* DEF) {
  PString WHERE;
  if (CURRENTSCOPE->CURRENTFN == PNil) WHERE = str_make(0, "");
  else if (CURRENTSCOPE->CURRENTFN->RETURNTYPEPTR == PNil) WHERE = CONCAT(CpLenPtr, 14, " in procedure ", CpEnd | CpStringPtr, &CURRENTSCOPE->CURRENTFN->NAME);
  else WHERE = CONCAT(CpLenPtr, 13, " in function ", CpEnd | CpStringPtr, &CURRENTSCOPE->CURRENTFN->NAME);
  switch (DEF->CLS) {
    case SDCVARIABLE:
      if (!DEF->VARDEF.WASUSED && !_HASUNUSEDPREFIX(&DEF->VARDEF.NAME)) {
        if (DEF->VARDEF.ISCONSTANT) COMPILEWARNING(CONCAT(CpLenPtr, 9, "Constant ", CpStringPtr, &DEF->VARDEF.NAME, CpLenPtr, 13, " was not used", CpEnd | CpStringPtr, &WHERE));
        else COMPILEWARNING(CONCAT(CpLenPtr, 9, "Variable ", CpStringPtr, &DEF->VARDEF.NAME, CpLenPtr, 13, " was not used", CpEnd | CpStringPtr, &WHERE));
      }
      else if (!DEF->VARDEF.WASINITIALIZED) COMPILEWARNING(CONCAT(CpLenPtr, 9, "Variable ", CpStringPtr, &DEF->VARDEF.NAME, CpLenPtr, 20, " was not initialized", CpEnd | CpStringPtr, &WHERE));
      break;
    case SDCSUBROUTINE:
      if (!DEF->SRDEF.WASUSED) {
        if (DEF->SRDEF.RETURNTYPEPTR == PNil) COMPILEWARNING(CONCAT(CpLenPtr, 10, "Procedure ", CpStringPtr, &DEF->SRDEF.NAME, CpEnd | CpLenPtr, 13, " was not used"));
        else COMPILEWARNING(CONCAT(CpLenPtr, 9, "Function ", CpStringPtr, &DEF->SRDEF.NAME, CpEnd | CpLenPtr, 13, " was not used"));
      }
      break;
    case SDCTYPE:
      if (cmp_str(CoNotEq, CpStringPtr, &DEF->TYPEDEF.NAME, CpLenPtr, 0, "") && !DEF->TYPEDEF.WASUSED) COMPILEWARNING(CONCAT(CpLenPtr, 5, "Type ", CpString, TYPENAME(&DEF->TYPEDEF), CpEnd | CpLenPtr, 13, " was not used"));
      break;
    default:
      break;
  }
}

TSDEFENTRY* _ADDDEF(TSDEFCLASS CLS) {
  TSDEFENTRY* RESULT;
  New((void**)&RESULT, sizeof(TSDEFENTRY));
  RESULT->CLS = CLS;
  STACK_PUSH(&CURRENTSCOPE->LATESTDEF, &RESULT);
  return RESULT;
}

void STARTLOCALSCOPE(TSSCOPEOBJ* DEFS, TSDSUBROUTINEDEF* NEWFUNCTION) {
  DEFS->PARENT = CURRENTSCOPE;
  DEFS->LATESTDEF = PNil;
  DEFS->COUNTERS = CURRENTSCOPE->COUNTERS;
  DEFS->CURRENTFN = NEWFUNCTION;
  CURRENTSCOPE = DEFS;
}

void CLOSELOCALSCOPE() {
  TSDEFENTRY* DEF;
  DEF = CURRENTSCOPE->LATESTDEF;
  while (DEF != PNil) {
    _CHECKUNUSEDSYMBOLS(DEF);
    DEF = DEF->OLDER;
  }
  CURRENTSCOPE = CURRENTSCOPE->PARENT;
  if (CURRENTSCOPE == PNil) INTERNALERROR(str_make(23, "Closed the global scope"));
}

PBoolean _FINDDEF(TSDEFENTRY** FOUNDDEF, TSTACKPREDICATE PREDICATE, void* CONTEXT, PBoolean FROMLOCALSCOPE) {
  PBoolean RESULT;
  TSSCOPEOBJ* DEFSET;
  PBoolean FOUND;
  DEFSET = CURRENTSCOPE;
  do {
    FOUND = STACK_FIND(&DEFSET->LATESTDEF, FOUNDDEF, PREDICATE, CONTEXT);
    if (!FOUND && !FROMLOCALSCOPE) DEFSET = DEFSET->PARENT;
  } while (!FOUND && !FROMLOCALSCOPE && DEFSET != PNil);
  RESULT = FOUND;
  return RESULT;
}

PBoolean _DEFHASNAME(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  PString* NAME = (PString*)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && cmp_str(CoEq, CpStringPtr, &(*DEF)->TYPEDEF.NAME, CpStringPtr, NAME) || (*DEF)->CLS == SDCCONSTANT && cmp_str(CoEq, CpStringPtr, &(*DEF)->CONSTDEF.NAME, CpStringPtr, NAME) || (*DEF)->CLS == SDCVARIABLE && cmp_str(CoEq, CpStringPtr, &(*DEF)->VARDEF.NAME, CpStringPtr, NAME) || (*DEF)->CLS == SDCSUBROUTINE && cmp_str(CoEq, CpStringPtr, &(*DEF)->SRDEF.NAME, CpStringPtr, NAME) || (*DEF)->CLS == SDCPSFN && cmp_str(CoEq, CpStringPtr, &(*DEF)->PSFNDEF.NAME, CpStringPtr, NAME);
  return RESULT;
}

TSDEFENTRY* _FINDNAME(PString NAME, PBoolean REQUIRED, PBoolean FROMLOCALSCOPE) {
  TSDEFENTRY* RESULT;
  TSDEFENTRY* DEF;
  if (_FINDDEF(&DEF, &_DEFHASNAME, &NAME, FROMLOCALSCOPE)) RESULT = DEF;
  else if (REQUIRED) COMPILEERROR(CONCAT(CpLenPtr, 20, "Unknown identifier: ", CpEnd | CpStringPtr, &NAME));
  else RESULT = PNil;
  return RESULT;
}

void _CHECKNAMECLASS(TSDEFENTRY* DEF, TSDEFCLASS CLS, const PString* NAME) {
  if (DEF != PNil && DEF->CLS != CLS) switch (CLS) {
    case SDCTYPE:
      COMPILEERROR(CONCAT(CpLenPtr, 12, "Not a type: ", CpEnd | CpStringPtr, NAME));
      break;
    case SDCVARIABLE:
      COMPILEERROR(CONCAT(CpLenPtr, 16, "Not a variable: ", CpEnd | CpStringPtr, NAME));
      break;
    case SDCSUBROUTINE:
      COMPILEERROR(CONCAT(CpLenPtr, 29, "Not a procedure or function: ", CpEnd | CpStringPtr, NAME));
      break;
    case SDCPSFN:
      COMPILEERROR(CONCAT(CpLenPtr, 29, "Not a procedure or function: ", CpEnd | CpStringPtr, NAME));
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 24, "Name class mismatch for ", CpEnd | CpStringPtr, NAME));
      break;
  }
}

TSDEFENTRY* FINDNAMEINLOCALSCOPE(const PString* NAME, PBoolean REQUIRED) {
  TSDEFENTRY* RESULT;
  RESULT = _FINDNAME(*NAME, REQUIRED, 1);
  return RESULT;
}

TSDEFENTRY* FINDNAMEOFCLASSINLOCALSCOPE(const PString* NAME, TSDEFCLASS CLS, PBoolean REQUIRED) {
  TSDEFENTRY* RESULT;
  RESULT = FINDNAMEINLOCALSCOPE(NAME, REQUIRED);
  _CHECKNAMECLASS(RESULT, CLS, NAME);
  return RESULT;
}

TSDEFENTRY* FINDNAME(const PString* NAME, PBoolean REQUIRED) {
  TSDEFENTRY* RESULT;
  RESULT = _FINDNAME(*NAME, REQUIRED, 0);
  return RESULT;
}

TSDEFENTRY* FINDNAMEOFCLASS(const PString* NAME, TSDEFCLASS CLS, PBoolean REQUIRED) {
  TSDEFENTRY* RESULT;
  RESULT = FINDNAME(NAME, REQUIRED);
  _CHECKNAMECLASS(RESULT, CLS, NAME);
  return RESULT;
}

TSDPSFNDEF* ADDPSFN(const PString* NAME, TSDPSFNPARSER PARSE) {
  TSDPSFNDEF* RESULT;
  TSDEFENTRY* DEF;
  DEF = _ADDDEF(SDCPSFN);
  DEF->PSFNDEF.NAME = *NAME;
  DEF->PSFNDEF.PARSEFN = PARSE;
  RESULT = &DEF->PSFNDEF;
  return RESULT;
}

TSDTYPEDEF COPYTYPE(TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF RESULT;
  RESULT = *TYPEPTR;
  if (RESULT.CLS == SDTCPOINTERFORWARD) {
    New((void**)&RESULT.TARGETNAME, sizeof(PString));
    *RESULT.TARGETNAME = *TYPEPTR->TARGETNAME;
  }
  return RESULT;
}

TSDTYPEDEF* GETFUNDAMENTALTYPE(TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  if (TYPEPTR != PNil && TYPEPTR->CLS == SDTCRANGE) RESULT = TYPEPTR->RANGEDEF.BASETYPEPTR;
  else RESULT = TYPEPTR;
  return RESULT;
}

PBoolean _TYPEHASCLASS(TSDTYPEDEF* TYPEPTR, TSDTYPECLASS CLS) {
  PBoolean RESULT;
  RESULT = TYPEPTR != PNil && GETFUNDAMENTALTYPE(TYPEPTR)->CLS == CLS;
  return RESULT;
}

PBoolean ISREALTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCREAL);
  return RESULT;
}

PBoolean ISCHARTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCCHAR);
  return RESULT;
}

PBoolean ISBOOLEANTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCBOOLEAN);
  return RESULT;
}

PBoolean ISTEXTTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISFILETYPE(TYPEPTR) && TYPEPTR->FILEDEF.CLS == SDTFCTEXT;
  return RESULT;
}

PBoolean ISRANGETYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR != PNil && TYPEPTR->CLS == SDTCRANGE;
  return RESULT;
}

PBoolean ISSETTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCSET);
  return RESULT;
}

PBoolean ISNILTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCNIL);
  return RESULT;
}

PBoolean ISPOINTERYTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISPOINTERTYPE(TYPEPTR) || ISNILTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISPOINTERFORWARDTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCPOINTERFORWARD);
  return RESULT;
}

PBoolean ISUNTYPEDPTRTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISPOINTERTYPE(TYPEPTR) && TYPEPTR->POINTEDTYPEPTR == PNil;
  return RESULT;
}

PBoolean ISFUNCTIONTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCFUNCTION);
  return RESULT;
}

PBoolean ISFUNCTIONYTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISFUNCTIONTYPE(TYPEPTR) || ISNILTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISBOUNDEDTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

PInteger GETTYPELOWBOUND(TSDTYPEDEF* TYPEPTR) {
  PInteger RESULT;
  PString tmp1;
  switch (TYPEPTR->CLS) {
    case SDTCBOOLEAN:
      RESULT = 0;
      break;
    case SDTCCHAR:
      RESULT = 0;
      break;
    case SDTCENUM:
      RESULT = 0;
      break;
    case SDTCRANGE:
      RESULT = TYPEPTR->RANGEDEF.FIRST;
      break;
    default:
      tmp1 = str_make(21, "Expected bounded type");
      ERRORFORTYPE(&tmp1, TYPEPTR);
      break;
  }
  return RESULT;
}

PInteger GETTYPEHIGHBOUND(TSDTYPEDEF* TYPEPTR) {
  PInteger RESULT;
  PString tmp1;
  switch (TYPEPTR->CLS) {
    case SDTCBOOLEAN:
      RESULT = 1;
      break;
    case SDTCCHAR:
      RESULT = 255;
      break;
    case SDTCENUM:
      RESULT = TYPEPTR->ENUMPTR->SIZE - 1;
      break;
    case SDTCRANGE:
      RESULT = TYPEPTR->RANGEDEF.LAST;
      break;
    default:
      tmp1 = str_make(21, "Expected bounded type");
      ERRORFORTYPE(&tmp1, TYPEPTR);
      break;
  }
  return RESULT;
}

PInteger GETBOUNDEDTYPESIZE(TSDTYPEDEF* TYPEPTR) {
  PInteger RESULT;
  RESULT = GETTYPEHIGHBOUND(TYPEPTR) - GETTYPELOWBOUND(TYPEPTR) + 1;
  return RESULT;
}

PBoolean ISSAMETYPE(TSDTYPEDEF* A, TSDTYPEDEF* B) {
  PBoolean RESULT;
  if (A == PNil || B == PNil) RESULT = A == B;
  else {
    while (A->ALIASFOR != PNil) A = A->ALIASFOR;
    while (B->ALIASFOR != PNil) B = B->ALIASFOR;
    RESULT = A == B || ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE(A->POINTEDTYPEPTR, B->POINTEDTYPEPTR) || ISRANGETYPE(A) && ISRANGETYPE(B) && ISSAMETYPE(GETFUNDAMENTALTYPE(A), GETFUNDAMENTALTYPE(B)) && GETTYPELOWBOUND(A) == GETTYPELOWBOUND(B) && GETTYPEHIGHBOUND(A) == GETTYPEHIGHBOUND(B) || ISSETTYPE(A) && ISSETTYPE(B) && ISSAMETYPE(A->ELEMENTTYPEPTR, B->ELEMENTTYPEPTR);
  }
  return RESULT;
}

PBoolean ISFUNDAMENTALLYSAMETYPE(TSDTYPEDEF* A, TSDTYPEDEF* B) {
  PBoolean RESULT;
  RESULT = ISSAMETYPE(GETFUNDAMENTALTYPE(A), GETFUNDAMENTALTYPE(B));
  return RESULT;
}

PBoolean AREPOINTERSCOMPATIBLE(TSDTYPEDEF* A, TSDTYPEDEF* B) {
  PBoolean RESULT;
  RESULT = ISPOINTERYTYPE(A) && ISPOINTERYTYPE(B) && (ISNILTYPE(A) || ISNILTYPE(B) || ISSAMETYPE(A, B));
  return RESULT;
}

PBoolean AREFUNCTIONSCOMPATIBLE(TSDTYPEDEF* A, TSDTYPEDEF* B) {
  PBoolean RESULT;
  RESULT = ISFUNCTIONYTYPE(A) && ISFUNCTIONYTYPE(B) && (ISNILTYPE(A) || ISNILTYPE(B) || ISSAMETYPE(A, B));
  return RESULT;
}

PString UNPARSECHAR(PChar CHR) {
  PString RESULT;
  if (CHR == '\'') RESULT = str_make(4, "''''");
  else if (CHR < ' ') RESULT = CONCAT(CpChar, '#', CpEnd | CpString, INTTOSTR((int)CHR));
  else RESULT = CONCAT(CpChar, '\'', CpChar, CHR, CpEnd | CpChar, '\'');
  return RESULT;
}

PString UNPARSESTRING(const PString* ST) {
  PString RESULT;
  PInteger POS;
  PBoolean QUOTED;
  QUOTED = 0;
  RESULT = str_make(0, "");
  for (PInteger first = 1, last = LENGTH(ST); first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      if (ST->chr[POS] < ' ') {
        if (QUOTED) {
          QUOTED = 0;
          RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '\'');
        }
        RESULT = CONCAT(CpStringPtr, &RESULT, CpChar, '#', CpEnd | CpString, INTTOSTR((int)ST->chr[POS]));
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
    break;
  }
  if (QUOTED) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '\'');
  if (cmp_str(CoEq, CpStringPtr, &RESULT, CpLenPtr, 0, "")) RESULT = str_make(2, "''");
  return RESULT;
}

PString _ANTIORDINAL(PInteger ORDINAL, TSDTYPEDEF* TYPEPTR) {
  PString RESULT;
  RESULT = EXDESCRIBE(EXGETANTIORDINAL(ORDINAL, TYPEPTR));
  return RESULT;
}

PString DEEPTYPENAME(TSDTYPEDEF* TYPEPTR, PBoolean USEORIGINAL) {
  PString RESULT;
  PInteger POS;
  TSDTSUBROUTINEDEF* tmp1;
  if (USEORIGINAL && TYPEPTR != PNil) while (TYPEPTR->ALIASFOR != PNil) TYPEPTR = TYPEPTR->ALIASFOR;
  if (TYPEPTR == PNil) RESULT = str_make(7, "untyped");
  else if (cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) RESULT = TYPEPTR->NAME;
  else if (TYPEPTR->CLS == SDTCFILE) {
    if (TYPEPTR->FILEDEF.CLS == SDTFCNONE) RESULT = str_make(4, "FILE");
    else if (TYPEPTR->FILEDEF.CLS == SDTFCTEXT) RESULT = str_make(4, "TEXT");
    else RESULT = CONCAT(CpLenPtr, 8, "FILE OF ", CpEnd | CpString, DEEPTYPENAME(TYPEPTR->FILEDEF.TYPEPTR, 0));
  }
  else if (TYPEPTR->CLS == SDTCENUM) {
    RESULT = str_of('(');
    for (PInteger first = 0, last = TYPEPTR->ENUMPTR->SIZE - 1; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
        if (POS != 0) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ',');
        RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpStringPtr, &TYPEPTR->ENUMPTR->VALUES[subrange(POS, 0, 127)]);
      }
      break;
    }
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  }
  else if (TYPEPTR->CLS == SDTCRANGE) RESULT = CONCAT(CpString, _ANTIORDINAL(TYPEPTR->RANGEDEF.FIRST, TYPEPTR->RANGEDEF.BASETYPEPTR), CpLenPtr, 2, "..", CpEnd | CpString, _ANTIORDINAL(TYPEPTR->RANGEDEF.LAST, TYPEPTR->RANGEDEF.BASETYPEPTR));
  else if (TYPEPTR->CLS == SDTCSET) {
    if (TYPEPTR->ELEMENTTYPEPTR == PNil) RESULT = str_make(9, "SET OF []");
    else RESULT = CONCAT(CpLenPtr, 7, "SET OF ", CpEnd | CpString, DEEPTYPENAME(TYPEPTR->ELEMENTTYPEPTR, 0));
  }
  else if (TYPEPTR->CLS == SDTCRECORD) {
    RESULT = str_make(7, "RECORD ");
    for (PInteger first = 1, last = TYPEPTR->RECPTR->SIZE; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
        if (POS != 1) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, "; ");
        RESULT = CONCAT(CpStringPtr, &RESULT, CpString, DEEPTYPENAME(TYPEPTR->RECPTR->FIELDS[subrange(POS, 1, 64) - 1].TYPEPTR, 0), CpChar, ':', CpEnd | CpStringPtr, &TYPEPTR->RECPTR->FIELDS[subrange(POS, 1, 64) - 1].NAME);
      }
      break;
    }
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 4, " END");
  }
  else if (TYPEPTR->CLS == SDTCARRAY) RESULT = CONCAT(CpLenPtr, 7, "ARRAY [", CpString, DEEPTYPENAME(TYPEPTR->ARRAYDEF.INDEXTYPEPTR, 0), CpLenPtr, 5, "] OF ", CpEnd | CpString, DEEPTYPENAME(TYPEPTR->ARRAYDEF.VALUETYPEPTR, 0));
  else if (TYPEPTR->CLS == SDTCPOINTER) {
    if (TYPEPTR->POINTEDTYPEPTR == PNil) RESULT = str_make(7, "POINTER");
    else RESULT = CONCAT(CpChar, '^', CpEnd | CpString, DEEPTYPENAME(TYPEPTR->POINTEDTYPEPTR, USEORIGINAL));
  }
  else if (TYPEPTR->CLS == SDTCPOINTERFORWARD) RESULT = CONCAT(CpChar, '^', CpEnd | CpStringPtr, TYPEPTR->TARGETNAME);
  else if (TYPEPTR->CLS == SDTCFUNCTION) {
    tmp1 = TYPEPTR->FNDEFPTR;
    if (tmp1->RETURNTYPEPTR == PNil) RESULT = str_make(9, "PROCEDURE");
    else RESULT = str_make(8, "FUNCTION");
    if (tmp1->ARGS.COUNT > 0) {
      RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '(');
      for (PInteger first = 1, last = tmp1->ARGS.COUNT; first <= last; /*breaks*/) {
        PBoolean done = 0;
        for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
          if (POS != 1) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, "; ");
          if (tmp1->ARGS.DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 6, "CONST ");
          else if (tmp1->ARGS.DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 4, "VAR ");
          RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpStringPtr, &tmp1->ARGS.DEFS[subrange(POS, 1, 16) - 1].NAME);
          RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 3, " : ", CpEnd | CpString, DEEPTYPENAME(tmp1->ARGS.DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, 0));
        }
        break;
      }
      RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
    }
    if (tmp1->RETURNTYPEPTR != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpLenPtr, 3, " : ", CpEnd | CpString, DEEPTYPENAME(tmp1->RETURNTYPEPTR, 0));
  }
  else {
    STR_e(TYPEPTR->CLS, enumvalues3, 0, &RESULT);
    COMPILEERROR(CONCAT(CpLenPtr, 37, "Could not get name for type of class ", CpEnd | CpStringPtr, &RESULT));
  }
  return RESULT;
}

TSDCONSTANTDEF* ADDCONSTANT(const TSDCONSTANTDEF* CONSTANT) {
  TSDCONSTANTDEF* RESULT;
  TSDEFENTRY* DEF;
  if (FINDNAMEINLOCALSCOPE(&CONSTANT->NAME, 0) != PNil) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, &CONSTANT->NAME, CpEnd | CpLenPtr, 16, " already defined"));
  DEF = _ADDDEF(SDCCONSTANT);
  RESULT = &DEF->CONSTDEF;
  *RESULT = *CONSTANT;
  return RESULT;
}

TSDVARIABLEDEF* ADDVARIABLE(const TSDVARIABLEDEF* VARDEF) {
  TSDVARIABLEDEF* RESULT;
  TSDEFENTRY* DEF;
  if (FINDNAMEINLOCALSCOPE(&VARDEF->NAME, 0) != PNil) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, &VARDEF->NAME, CpEnd | CpLenPtr, 16, " already defined"));
  DEF = _ADDDEF(SDCVARIABLE);
  RESULT = &DEF->VARDEF;
  *RESULT = *VARDEF;
  return RESULT;
}

TSDSUBROUTINEDEF EMPTYFUNCTION() {
  TSDSUBROUTINEDEF RESULT;
  RESULT.NAME = str_make(0, "");
  RESULT.EXTERNALNAME = str_make(0, "");
  RESULT.ARGS.COUNT = 0;
  RESULT.RETURNTYPEPTR = PNil;
  RESULT.ISDECLARATION = 0;
  RESULT.HADDECLARATION = 0;
  RESULT.WASUSED = 0;
  return RESULT;
}

PBoolean ISSAMEFUNCTIONDEFINITION(TSDSUBROUTINEDEF* DECLPTR, TSDSUBROUTINEDEF FUN) {
  PBoolean RESULT;
  TSDSUBROUTINEDEF DECL;
  PBoolean SAME;
  PInteger POS;
  DECL = *DECLPTR;
  SAME = ISSAMETYPE(DECL.RETURNTYPEPTR, FUN.RETURNTYPEPTR) && DECL.ARGS.COUNT == FUN.ARGS.COUNT;
  for (PInteger first = 1, last = DECL.ARGS.COUNT; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) SAME = SAME && ISSAMETYPE(DECL.ARGS.DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, FUN.ARGS.DEFS[subrange(POS, 1, 16) - 1].TYPEPTR) && DECL.ARGS.DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE == FUN.ARGS.DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE;
    break;
  }
  RESULT = SAME;
  return RESULT;
}

PBoolean HASFORWARDDECLARATION(const PString* NAME) {
  PBoolean RESULT;
  TSDEFENTRY* DEF;
  DEF = FINDNAMEOFCLASSINLOCALSCOPE(NAME, SDCSUBROUTINE, 0);
  RESULT = DEF != PNil && DEF->SRDEF.ISDECLARATION;
  return RESULT;
}

TSDSUBROUTINEDEF* ADDFUNCTION(const TSDSUBROUTINEDEF* FUN) {
  TSDSUBROUTINEDEF* RESULT;
  TSDEFENTRY* DEF;
  PBoolean ISPROCEDURE;
  ISPROCEDURE = FUN->RETURNTYPEPTR == PNil;
  DEF = FINDNAMEINLOCALSCOPE(&FUN->NAME, 0);
  if (DEF == PNil) {
    DEF = _ADDDEF(SDCSUBROUTINE);
    RESULT = &DEF->SRDEF;
    *RESULT = *FUN;
  }
  else {
    if (DEF->CLS != SDCSUBROUTINE || FUN->ISDECLARATION) COMPILEERROR(CONCAT(CpLenPtr, 11, "Identifier ", CpStringPtr, &FUN->NAME, CpEnd | CpLenPtr, 16, " already defined"));
    RESULT = &DEF->SRDEF;
    if (RESULT->ISDECLARATION) {
      if (FUN->ARGS.COUNT == 0 && FUN->RETURNTYPEPTR == PNil || ISSAMEFUNCTIONDEFINITION(RESULT, *FUN)) {
        RESULT->ISDECLARATION = 0;
        RESULT->HADDECLARATION = 1;
      }
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
  return RESULT;
}

PInteger FINDFIELD(TSDTYPEDEF* TYPEPTR, const PString* NAME, PBoolean REQUIRED) {
  PInteger RESULT;
  PInteger POS;
  PInteger RET;
  TSDTRECORDDEF* tmp1;
  ENSURERECORDTYPE(TYPEPTR);
  tmp1 = TYPEPTR->RECPTR;
  RET = 0;
  POS = tmp1->SIZE;
  while (POS >= 1 && RET == 0) {
    if (cmp_str(CoEq, CpStringPtr, NAME, CpStringPtr, &tmp1->FIELDS[subrange(POS, 1, 64) - 1].NAME)) RET = POS;
    POS = POS - 1;
  }
  if (REQUIRED && RET == 0) COMPILEERROR(CONCAT(CpLenPtr, 17, "Field not found: ", CpEnd | CpStringPtr, NAME));
  RESULT = RET;
  return RESULT;
}

TSDTYPEDEF* FINDFIELDTYPE(TSDTYPEDEF* TYPEPTR, const PString* NAME, PBoolean REQUIRED) {
  TSDTYPEDEF* RESULT;
  PInteger POS;
  POS = FINDFIELD(TYPEPTR, NAME, REQUIRED);
  if (POS == 0) RESULT = PNil;
  else RESULT = TYPEPTR->RECPTR->FIELDS[subrange(POS, 1, 64) - 1].TYPEPTR;
  return RESULT;
}

PBoolean _DEFISTMPVAR(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDVARIABLEDEF* VARDEF = (TSDVARIABLEDEF*)CTX;
  RESULT = (*DEF)->CLS == SDCTMPVAR && !(*DEF)->TMPVARDEF.INUSE && (*DEF)->TMPVARDEF.VARDEF.ISREFERENCE == VARDEF->ISREFERENCE && (*DEF)->TMPVARDEF.VARDEF.TYPEPTR == VARDEF->TYPEPTR;
  return RESULT;
}

TSDTMPVARDEF* GETTEMPORARYVARIABLE(TSDTYPEDEF* TYPEPTR, PBoolean ISREFERENCE) {
  TSDTMPVARDEF* RESULT;
  TSDEFENTRY* DEF;
  TSDVARIABLEDEF VARDEF;
  VARDEF.TYPEPTR = TYPEPTR;
  VARDEF.ISREFERENCE = ISREFERENCE;
  if (_FINDDEF(&DEF, &_DEFISTMPVAR, &VARDEF, 1)) RESULT = &DEF->TMPVARDEF;
  else {
    VARDEF.NAME = CONCAT(CpLenPtr, 3, "tmp", CpEnd | CpString, INTTOSTR(DEFCOUNTER(SCTTMPVAR)));
    VARDEF.ISCONSTANT = 0;
    VARDEF.CONSTANTVALUE = PNil;
    VARDEF.LOCATION = PNil;
    VARDEF.ISARGUMENT = 0;
    VARDEF.WASINITIALIZED = 1;
    VARDEF.WASUSED = 1;
    VARDEF.ISALIASFOR = PNil;
    DEF = _ADDDEF(SDCTMPVAR);
    RESULT = &DEF->TMPVARDEF;
    RESULT->VARDEF = VARDEF;
  }
  RESULT->INUSE = 1;
  return RESULT;
}

TSDTMPVARDEF* GETALIASVARIABLE(TSDTYPEDEF* TYPEPTR, TSEXPRESSIONOBJ* EXPR) {
  TSDTMPVARDEF* RESULT;
  RESULT = GETTEMPORARYVARIABLE(TYPEPTR, 0);
  RESULT->VARDEF.ISALIASFOR = EXPR;
  return RESULT;
}

PBoolean _DEFISWITHVAR(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  PString* NAME = (PString*)CTX;
  RESULT = (*DEF)->CLS == SDCWITHVAR && (*DEF)->WITHVARDEF.ISACTIVE && FINDFIELDTYPE((*DEF)->WITHVARDEF.TMPVARPTR->VARDEF.TYPEPTR, NAME, 0) != PNil;
  return RESULT;
}

TSDWITHVARDEF* FINDWITHVAR(PString NAME) {
  TSDWITHVARDEF* RESULT;
  TSDEFENTRY* DEF;
  if (_FINDDEF(&DEF, &_DEFISWITHVAR, &NAME, 1)) RESULT = &DEF->WITHVARDEF;
  else RESULT = PNil;
  return RESULT;
}

TSDWITHVARDEF* ADDWITHVAR(TSEXPRESSIONOBJ* BASE) {
  TSDWITHVARDEF* RESULT;
  TSDEFENTRY* DEF;
  TSDTMPVARDEF* TMPVARPTR;
  ENSURERECORDEXPR(BASE);
  TMPVARPTR = GETTEMPORARYVARIABLE(BASE->TYPEPTR, BASE->ISADDRESSABLE);
  TMPVARPTR->VARDEF.ISALIASFOR = BASE;
  DEF = _ADDDEF(SDCWITHVAR);
  RESULT = &DEF->WITHVARDEF;
  RESULT->TMPVARPTR = TMPVARPTR;
  RESULT->ISACTIVE = 1;
  return RESULT;
}

TSDCONSTANTDEF MAKECONSTANT(const PString* NAME, TSEXPRESSIONOBJ* VALUE) {
  TSDCONSTANTDEF RESULT;
  TSDCONSTANTDEF CONSTANT;
  CONSTANT.NAME = *NAME;
  CONSTANT.VALUE = VALUE;
  RESULT = CONSTANT;
  return RESULT;
}

TSDVARIABLEDEF MAKETYPEDCONSTANT(const PString* NAME, TSDTYPEDEF* TYPEPTR, TSEXPRESSIONOBJ* VALUE) {
  TSDVARIABLEDEF RESULT;
  RESULT.NAME = *NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = 0;
  RESULT.LOCATION = PNil;
  RESULT.ISCONSTANT = 1;
  RESULT.CONSTANTVALUE = VALUE;
  RESULT.ISARGUMENT = 0;
  RESULT.WASINITIALIZED = 1;
  RESULT.WASUSED = 0;
  RESULT.ISALIASFOR = PNil;
  return RESULT;
}

TSDVARIABLEDEF MAKEVARIABLE(const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  TSDVARIABLEDEF RESULT;
  RESULT.NAME = *NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = 0;
  RESULT.LOCATION = PNil;
  RESULT.ISCONSTANT = 0;
  RESULT.CONSTANTVALUE = PNil;
  RESULT.ISARGUMENT = 0;
  RESULT.WASINITIALIZED = 0;
  RESULT.WASUSED = 0;
  RESULT.ISALIASFOR = PNil;
  return RESULT;
}

TSDVARIABLEDEF MAKEABSOLUTE(const PString* NAME, TSDTYPEDEF* TYPEPTR, TSEXPRESSIONOBJ* LOCATION) {
  TSDVARIABLEDEF RESULT;
  RESULT.NAME = *NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = 1;
  RESULT.LOCATION = LOCATION;
  RESULT.ISCONSTANT = 0;
  RESULT.CONSTANTVALUE = PNil;
  RESULT.ISARGUMENT = 0;
  RESULT.WASINITIALIZED = 1;
  RESULT.WASUSED = 0;
  RESULT.ISALIASFOR = PNil;
  return RESULT;
}

TSDVARIABLEDEF MAKEFROMARG(const TSDSUBROUTINEARG* ARG) {
  TSDVARIABLEDEF RESULT;
  RESULT.NAME = ARG->NAME;
  RESULT.TYPEPTR = ARG->TYPEPTR;
  RESULT.ISREFERENCE = ARG->ISREFERENCE;
  RESULT.LOCATION = PNil;
  RESULT.ISCONSTANT = ARG->ISCONSTANT;
  RESULT.CONSTANTVALUE = PNil;
  RESULT.ISARGUMENT = 1;
  RESULT.WASINITIALIZED = 1;
  RESULT.WASUSED = 0;
  RESULT.ISALIASFOR = PNil;
  return RESULT;
}

TSDSUBROUTINEARG _MAKEARG(const PString* NAME, TSDTYPEDEF* TYPEPTR, PBoolean ISREF, PBoolean ISCONST) {
  TSDSUBROUTINEARG RESULT;
  RESULT.NAME = *NAME;
  RESULT.TYPEPTR = TYPEPTR;
  RESULT.ISREFERENCE = ISREF || ISCONST;
  RESULT.ISCONSTANT = ISCONST;
  return RESULT;
}

TSDSUBROUTINEARG MAKEARG(const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  TSDSUBROUTINEARG RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 0, 0);
  return RESULT;
}

TSDSUBROUTINEARG MAKEVARARG(const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  TSDSUBROUTINEARG RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 1, 0);
  return RESULT;
}

TSDSUBROUTINEARG MAKECONSTARG(const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  TSDSUBROUTINEARG RESULT;
  RESULT = _MAKEARG(NAME, TYPEPTR, 0, 1);
  return RESULT;
}

TSDSUBROUTINEDEF MAKEPROCEDURE0(const PString* NAME) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 0;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEPROCEDURE1(const PString* NAME, TSDSUBROUTINEARG ARG) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 1;
  RESULT.ARGS.DEFS[0] = ARG;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEPROCEDURE2(const PString* NAME, TSDSUBROUTINEARG ARG1, TSDSUBROUTINEARG ARG2) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 2;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEPROCEDURE3(const PString* NAME, TSDSUBROUTINEARG ARG1, TSDSUBROUTINEARG ARG2, TSDSUBROUTINEARG ARG3) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.ARGS.COUNT = 3;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  RESULT.ARGS.DEFS[2] = ARG3;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEFUNCTION0(const PString* NAME, TSDTYPEDEF* RETTYPEPTR) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEFUNCTION1(const PString* NAME, TSDTYPEDEF* RETTYPEPTR, TSDSUBROUTINEARG ARG) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGS.COUNT = 1;
  RESULT.ARGS.DEFS[0] = ARG;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEFUNCTION2(const PString* NAME, TSDTYPEDEF* RETTYPEPTR, TSDSUBROUTINEARG ARG1, TSDSUBROUTINEARG ARG2) {
  TSDSUBROUTINEDEF RESULT;
  RESULT = EMPTYFUNCTION();
  RESULT.NAME = *NAME;
  RESULT.EXTERNALNAME = *NAME;
  RESULT.RETURNTYPEPTR = RETTYPEPTR;
  RESULT.ARGS.COUNT = 2;
  RESULT.ARGS.DEFS[0] = ARG1;
  RESULT.ARGS.DEFS[1] = ARG2;
  return RESULT;
}

TSDSUBROUTINEDEF MAKEFUNCTION3(const PString* NAME, TSDTYPEDEF* RETTYPEPTR, TSDSUBROUTINEARG ARG1, TSDSUBROUTINEARG ARG2, TSDSUBROUTINEARG ARG3) {
  TSDSUBROUTINEDEF RESULT;
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

TSDTYPEDEF* _UNALIASTYPE(TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  RESULT = TYPEPTR;
  while (RESULT->ALIASFOR != PNil) RESULT = RESULT->ALIASFOR;
  return RESULT;
}

TSDTYPEDEF* _NEWTYPE(TSDTYPECLASS CLS) {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  DEF = _ADDDEF(SDCTYPE);
  RESULT = &DEF->TYPEDEF;
  RESULT->NAME = str_make(0, "");
  RESULT->CLS = CLS;
  RESULT->ALIASFOR = PNil;
  RESULT->WASUSED = 0;
  return RESULT;
}

TSDTYPEDEF* MAKEBASETYPE(const PString* NAME, TSDTYPECLASS CLS) {
  TSDTYPEDEF* RESULT;
  RESULT = _NEWTYPE(CLS);
  RESULT->NAME = *NAME;
  return RESULT;
}

PBoolean _DEFISFILETYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDTFILE* WANTED = (TSDTFILE*)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCFILE && (*DEF)->TYPEDEF.FILEDEF.CLS == WANTED->CLS && (WANTED->CLS != SDTFCBINARY || ISSAMETYPE((*DEF)->TYPEDEF.FILEDEF.TYPEPTR, WANTED->TYPEPTR));
  return RESULT;
}

TSDTYPEDEF* _MAKEFILETYPE(TSDTFILECLASS CLS, TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  TSDTFILE FILEDEF;
  TSDEFENTRY* DEF;
  FILEDEF.CLS = CLS;
  FILEDEF.TYPEPTR = TYPEPTR;
  if (_FINDDEF(&DEF, &_DEFISFILETYPE, &FILEDEF, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCFILE);
    RESULT->FILEDEF = FILEDEF;
  }
  return RESULT;
}

TSDTYPEDEF* MAKEGENERICFILETYPE() {
  TSDTYPEDEF* RESULT;
  RESULT = _MAKEFILETYPE(SDTFCNONE, PNil);
  return RESULT;
}

TSDTYPEDEF* MAKETEXTTYPE() {
  TSDTYPEDEF* RESULT;
  RESULT = _MAKEFILETYPE(SDTFCTEXT, PNil);
  RESULT->NAME = str_make(4, "TEXT");
  return RESULT;
}

TSDTYPEDEF* MAKEFILETYPE(TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  RESULT = _MAKEFILETYPE(SDTFCBINARY, TYPEPTR);
  return RESULT;
}

TSDTYPEDEF* MAKEENUMTYPE(const TSDTENUMDEF* ENUM) {
  TSDTYPEDEF* RESULT;
  RESULT = _NEWTYPE(SDTCENUM);
  RESULT->ENUMPTR = NEWENUM(ENUM);
  RESULT->WASUSED = 1;
  return RESULT;
}

TSDTYPEDEF* MAKERECORDTYPE(const TSDTRECORDDEF* REC) {
  TSDTYPEDEF* RESULT;
  RESULT = _NEWTYPE(SDTCRECORD);
  RESULT->RECPTR = NEWRECORD(REC);
  return RESULT;
}

PBoolean _DEFISARRAYTYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDTARRAY* WANTED = (TSDTARRAY*)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCARRAY && ISSAMETYPE((*DEF)->TYPEDEF.ARRAYDEF.INDEXTYPEPTR, WANTED->INDEXTYPEPTR) && ISSAMETYPE((*DEF)->TYPEDEF.ARRAYDEF.VALUETYPEPTR, WANTED->VALUETYPEPTR);
  return RESULT;
}

TSDTYPEDEF* MAKEARRAYTYPE(TSDTYPEDEF* INDEXTYPE, TSDTYPEDEF* VALUETYPE) {
  TSDTYPEDEF* RESULT;
  TSDTARRAY ARRAYDEF;
  TSDEFENTRY* DEF;
  PString tmp1;
  if (!ISBOUNDEDTYPE(INDEXTYPE)) {
    tmp1 = str_make(51, "Array indices must belong to a bounded ordinal type");
    ERRORFORTYPE(&tmp1, INDEXTYPE);
  }
  ARRAYDEF.INDEXTYPEPTR = INDEXTYPE;
  ARRAYDEF.VALUETYPEPTR = VALUETYPE;
  if (_FINDDEF(&DEF, &_DEFISARRAYTYPE, &ARRAYDEF, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCARRAY);
    RESULT->ARRAYDEF = ARRAYDEF;
  }
  return RESULT;
}

PBoolean _DEFISRANGETYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDTRANGE* WANTED = (TSDTRANGE*)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCRANGE && ISSAMETYPE((*DEF)->TYPEDEF.RANGEDEF.BASETYPEPTR, WANTED->BASETYPEPTR) && (*DEF)->TYPEDEF.RANGEDEF.FIRST == WANTED->FIRST && (*DEF)->TYPEDEF.RANGEDEF.LAST == WANTED->LAST;
  return RESULT;
}

TSDTYPEDEF* MAKERANGETYPE(TSDTYPEDEF* TYPEPTR, PInteger FIRST, PInteger LAST) {
  TSDTYPEDEF* RESULT;
  TSDTRANGE RANGEDEF;
  TSDEFENTRY* DEF;
  if (FIRST > LAST) COMPILEERROR(str_make(51, "The bounds of a subrange must be in ascending order"));
  RANGEDEF.BASETYPEPTR = GETFUNDAMENTALTYPE(TYPEPTR);
  RANGEDEF.FIRST = FIRST;
  RANGEDEF.LAST = LAST;
  if (_FINDDEF(&DEF, &_DEFISRANGETYPE, &RANGEDEF, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCRANGE);
    RESULT->RANGEDEF = RANGEDEF;
  }
  return RESULT;
}

PBoolean _DEFISPOINTERTYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDTYPEDEF** TYPEPTR = (TSDTYPEDEF**)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCPOINTER && ISSAMETYPE((*DEF)->TYPEDEF.POINTEDTYPEPTR, *TYPEPTR);
  return RESULT;
}

TSDTYPEDEF* MAKEPOINTERTYPE(TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  if (_FINDDEF(&DEF, &_DEFISPOINTERTYPE, &TYPEPTR, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCPOINTER);
    RESULT->POINTEDTYPEPTR = TYPEPTR;
    if (TYPEPTR == PNil) RESULT->NAME = str_make(7, "POINTER");
  }
  return RESULT;
}

PBoolean _DEFISPOINTERFORWARDTYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  PString* TARGETNAME = (PString*)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCPOINTERFORWARD && cmp_str(CoEq, CpStringPtr, (*DEF)->TYPEDEF.TARGETNAME, CpStringPtr, TARGETNAME);
  return RESULT;
}

TSDTYPEDEF* MAKEPOINTERFORWARDTYPE(PString TARGETNAME) {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  if (_FINDDEF(&DEF, &_DEFISPOINTERFORWARDTYPE, &TARGETNAME, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCPOINTERFORWARD);
    New((void**)&RESULT->TARGETNAME, sizeof(PString));
    *RESULT->TARGETNAME = TARGETNAME;
  }
  return RESULT;
}

PBoolean _DEFISSETTYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDTYPEDEF** TYPEPTR = (TSDTYPEDEF**)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCSET && ISSAMETYPE((*DEF)->TYPEDEF.ELEMENTTYPEPTR, *TYPEPTR);
  return RESULT;
}

TSDTYPEDEF* MAKESETTYPE(TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  if (_FINDDEF(&DEF, &_DEFISSETTYPE, &TYPEPTR, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCSET);
    RESULT->ELEMENTTYPEPTR = TYPEPTR;
  }
  return RESULT;
}

PBoolean ARESAMEARGS(const TSDSUBROUTINEARGS* A, const TSDSUBROUTINEARGS* B) {
  PBoolean RESULT;
  PInteger POS;
  RESULT = A->COUNT == B->COUNT;
  if (RESULT) for (PInteger first = 1, last = A->COUNT; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) RESULT = RESULT && A->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE == B->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE && A->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT == B->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT && ISSAMETYPE(A->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, B->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR);
    break;
  }
  return RESULT;
}

PBoolean _DEFISFUNCTIONTYPE(void* ITEM, void* CTX) {
  PBoolean RESULT;
  TSDEFENTRY** DEF = (TSDEFENTRY**)ITEM;
  TSDTSUBROUTINEDEF* FNDEF = (TSDTSUBROUTINEDEF*)CTX;
  RESULT = (*DEF)->CLS == SDCTYPE && (*DEF)->TYPEDEF.CLS == SDTCFUNCTION && ISSAMETYPE((*DEF)->TYPEDEF.FNDEFPTR->RETURNTYPEPTR, FNDEF->RETURNTYPEPTR) && ARESAMEARGS(&(*DEF)->TYPEDEF.FNDEFPTR->ARGS, &FNDEF->ARGS);
  return RESULT;
}

TSDTYPEDEF* MAKEFUNCTIONTYPE(const TSDSUBROUTINEARGS* ARGS, TSDTYPEDEF* RETURNTYPEPTR) {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  TSDTSUBROUTINEDEF FNDEF;
  FNDEF.RETURNTYPEPTR = RETURNTYPEPTR;
  FNDEF.ARGS = *ARGS;
  if (_FINDDEF(&DEF, &_DEFISFUNCTIONTYPE, &FNDEF, 0)) RESULT = _UNALIASTYPE(&DEF->TYPEDEF);
  else {
    RESULT = _NEWTYPE(SDTCFUNCTION);
    RESULT->FNDEFPTR = NEWFNDEF();
    RESULT->FNDEFPTR->RETURNTYPEPTR = FNDEF.RETURNTYPEPTR;
    RESULT->FNDEFPTR->ARGS = FNDEF.ARGS;
  }
  return RESULT;
}

TSDTYPEDEF* MAKEALIASTYPE(const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* RESULT;
  TYPEPTR = _UNALIASTYPE(TYPEPTR);
  RESULT = _NEWTYPE(TYPEPTR->CLS);
  *RESULT = COPYTYPE(TYPEPTR);
  RESULT->NAME = *NAME;
  RESULT->ALIASFOR = TYPEPTR;
  return RESULT;
}

TSEXPRESSIONOBJ* _NEWEXPR(TSEXPRESSIONCLASS CLS) {
  TSEXPRESSIONOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSEXPRESSIONOBJ));
  RESULT->CLS = CLS;
  RESULT->TYPEPTR = PNil;
  RESULT->ISASSIGNABLE = 0;
  RESULT->ISADDRESSABLE = 0;
  RESULT->ISFUNCTIONRESULT = 0;
  RESULT->ISSTATEMENT = 0;
  RESULT->CHECKBOUNDS = OPTIONS.CHECKBOUNDS;
  return RESULT;
}

void _DISPOSEIMMEDIATE(TSEIMMEDIATE* IMM) {
  TSESETIMMBOUNDSOBJ* BOUNDS;
  if (IMM->CLS == SEICSET) {
    while (IMM->SETBOUNDS != PNil) {
      BOUNDS = IMM->SETBOUNDS->NEXT;
      Dispose((void**)&IMM->SETBOUNDS);
      IMM->SETBOUNDS = BOUNDS;
    }
  }
}

void _DISPOSEARRAYELEMS(TSEARRAYELEMOBJ* ELEM) {
  TSEARRAYELEMOBJ* NEXTELEM;
  while (ELEM != PNil) {
    NEXTELEM = ELEM->NEXT;
    EXDISPOSE(&ELEM->VALUE);
    Dispose((void**)&ELEM);
    ELEM = NEXTELEM;
  }
}

void _DISPOSERECORDFIELDS(TSERECORDFIELDOBJ* FIELD) {
  TSERECORDFIELDOBJ* NEXTFIELD;
  while (FIELD != PNil) {
    NEXTFIELD = FIELD->NEXT;
    EXDISPOSE(&FIELD->VALUE);
    Dispose((void**)&FIELD);
    FIELD = NEXTFIELD;
  }
}

void _DISPOSEBOUNDS(TSESETEXPRBOUNDSOBJ* BOUNDS) {
  TSESETEXPRBOUNDSOBJ* NEXT;
  while (BOUNDS != PNil) {
    NEXT = BOUNDS->NEXT;
    EXDISPOSE(&BOUNDS->FIRST);
    if (BOUNDS->LAST != PNil) EXDISPOSE(&BOUNDS->LAST);
    Dispose((void**)&BOUNDS);
    BOUNDS = NEXT;
  }
}

void _DISPOSEWRITEARG(TSEWRITEARG* WRITEARG) {
  EXDISPOSE(&WRITEARG->ARG);
  if (WRITEARG->WIDTH != PNil) EXDISPOSE(&WRITEARG->WIDTH);
  if (WRITEARG->PREC != PNil) EXDISPOSE(&WRITEARG->PREC);
}

void _DISPOSEREADEXPR(TSEXPRESSIONOBJ** EXPR) {
  TSEREADARGVALUE* READARG;
  EXDISPOSE(&(*EXPR)->READFILE);
  while (LIST_SHIFT(&(*EXPR)->READARGS, &READARG)) {
    EXDISPOSE(&READARG->DEST);
    Dispose((void**)&READARG);
  }
}

void _DISPOSEWRITEEXPR(TSEXPRESSIONOBJ** EXPR) {
  TSEWRITEARGVALUE* WRITEARG;
  EXDISPOSE(&(*EXPR)->WRITEFILE);
  while (LIST_SHIFT(&(*EXPR)->WRITEARGS, &WRITEARG)) {
    _DISPOSEWRITEARG(&WRITEARG->VALUE);
    Dispose((void**)&WRITEARG);
  }
}

TSEIMMEDIATE _COPYIMMEDIATE(const TSEIMMEDIATE* IMM) {
  TSEIMMEDIATE RESULT;
  TSESETIMMBOUNDSOBJ* SRC;
  TSESETIMMBOUNDSOBJ* DST;
  TLISTPTRS** ADDPOINT;
  RESULT = *IMM;
  if (IMM->CLS == SEICSET) {
    SRC = IMM->SETBOUNDS;
    RESULT.SETBOUNDS = PNil;
    ADDPOINT = LIST_GETADDPOINT(&RESULT.SETBOUNDS);
    while (SRC != PNil) {
      New((void**)&DST, sizeof(TSESETIMMBOUNDSOBJ));
      *DST = *SRC;
      LIST_ADD(&ADDPOINT, &DST);
      SRC = SRC->NEXT;
    }
  }
  return RESULT;
}

TSEARRAYELEMOBJ* _COPYARRAYELEMS(TSEARRAYELEMOBJ* ELEM) {
  TSEARRAYELEMOBJ* RESULT;
  TSEARRAYELEMOBJ* SRC;
  TSEARRAYELEMOBJ* DST;
  TLISTPTRS** ADDPOINT;
  SRC = ELEM;
  RESULT = PNil;
  ADDPOINT = LIST_GETADDPOINT(&RESULT);
  while (SRC != PNil) {
    New((void**)&DST, sizeof(TSEARRAYELEMOBJ));
    DST->VALUE = EXCOPY(SRC->VALUE);
    LIST_ADD(&ADDPOINT, &DST);
    SRC = SRC->NEXT;
  }
  return RESULT;
}

TSERECORDFIELDOBJ* _COPYRECORDFIELDS(TSERECORDFIELDOBJ* FIELD) {
  TSERECORDFIELDOBJ* RESULT;
  TSERECORDFIELDOBJ* SRC;
  TSERECORDFIELDOBJ* DST;
  TLISTPTRS** ADDPOINT;
  SRC = FIELD;
  RESULT = PNil;
  ADDPOINT = LIST_GETADDPOINT(&RESULT);
  while (SRC != PNil) {
    New((void**)&DST, sizeof(TSERECORDFIELDOBJ));
    DST->ORDINAL = SRC->ORDINAL;
    DST->VALUE = EXCOPY(SRC->VALUE);
    LIST_ADD(&ADDPOINT, &DST);
    SRC = SRC->NEXT;
  }
  return RESULT;
}

TSESETEXPRBOUNDSOBJ* _COPYBOUNDS(TSESETEXPRBOUNDSOBJ* BOUNDS) {
  TSESETEXPRBOUNDSOBJ* RESULT;
  TSESETEXPRBOUNDSOBJ* SRC;
  TSESETEXPRBOUNDSOBJ* DST;
  TLISTPTRS** ADDPOINT;
  SRC = BOUNDS;
  RESULT = PNil;
  ADDPOINT = LIST_GETADDPOINT(&RESULT);
  while (SRC != PNil) {
    New((void**)&DST, sizeof(TSESETEXPRBOUNDSOBJ));
    DST->FIRST = EXCOPY(SRC->FIRST);
    if (SRC->LAST != PNil) DST->LAST = EXCOPY(SRC->LAST);
    else DST->LAST = PNil;
    LIST_ADD(&ADDPOINT, &DST);
    SRC = SRC->NEXT;
  }
  return RESULT;
}

TSEWRITEARG _COPYWRITEARG(const TSEWRITEARG* WRITEARG) {
  TSEWRITEARG RESULT;
  RESULT.ARG = EXCOPY(WRITEARG->ARG);
  if (WRITEARG->WIDTH == PNil) RESULT.WIDTH = PNil;
  else RESULT.WIDTH = EXCOPY(WRITEARG->WIDTH);
  if (WRITEARG->PREC == PNil) RESULT.PREC = PNil;
  else RESULT.PREC = EXCOPY(WRITEARG->PREC);
  return RESULT;
}

void _COPYREADEXPR(TSEXPRESSIONOBJ** EXPR, TSEXPRESSIONOBJ** COPY) {
  TSEREADARGVALUE* SRC;
  TSEREADARGVALUE* DST;
  TLISTPTRS** ADDPOINT;
  (*COPY)->READFILE = EXCOPY((*EXPR)->READFILE);
  (*COPY)->READLN = (*EXPR)->READLN;
  (*COPY)->READARGS = PNil;
  ADDPOINT = LIST_GETADDPOINT(&(*COPY)->READARGS);
  SRC = (*EXPR)->READARGS;
  while (SRC != PNil) {
    New((void**)&DST, sizeof(TSEREADARGVALUE));
    DST->DEST = EXCOPY(SRC->DEST);
    LIST_ADD(&ADDPOINT, &DST);
    SRC = SRC->NEXT;
  }
}

void _COPYWRITEEXPR(TSEXPRESSIONOBJ** EXPR, TSEXPRESSIONOBJ** COPY) {
  TSEWRITEARGVALUE* SRC;
  TSEWRITEARGVALUE* DST;
  TLISTPTRS** ADDPOINT;
  (*COPY)->WRITEFILE = EXCOPY((*EXPR)->WRITEFILE);
  (*COPY)->WRITELN = (*EXPR)->WRITELN;
  (*COPY)->WRITEARGS = PNil;
  ADDPOINT = LIST_GETADDPOINT(&(*COPY)->WRITEARGS);
  SRC = (*EXPR)->WRITEARGS;
  while (SRC != PNil) {
    New((void**)&DST, sizeof(TSEWRITEARGVALUE));
    DST->VALUE = _COPYWRITEARG(&SRC->VALUE);
    LIST_ADD(&ADDPOINT, &DST);
    SRC = SRC->NEXT;
  }
}

PString _DESCRIBEIMMSETINTERNAL(TSESETIMMBOUNDSOBJ* BOUNDS, TSDTYPEDEF* SETOFTYPEPTR) {
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

PString _DESCRIBEIMMSET(TSESETIMMBOUNDSOBJ* BOUNDS, TSDTYPEDEF* SETOFTYPEPTR) {
  PString RESULT;
  RESULT = CONCAT(CpChar, '[', CpString, _DESCRIBEIMMSETINTERNAL(BOUNDS, SETOFTYPEPTR), CpEnd | CpChar, ']');
  return RESULT;
}

PString _DESCRIBEIMMEDIATE(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  TSEIMMEDIATE* tmp1;
  tmp1 = &EXPR->IMMEDIATE;
  switch (tmp1->CLS) {
    case SEICNIL:
      RESULT = str_make(3, "nil");
      break;
    case SEICBOOLEAN:
      STR_b(tmp1->BOOLEANVAL, 0, &RESULT);
      break;
    case SEICINTEGER:
      STR_i(tmp1->INTEGERVAL, 0, &RESULT);
      break;
    case SEICREAL:
      STR_r(tmp1->REALVAL, 0, -1, &RESULT);
      break;
    case SEICCHAR:
      RESULT = UNPARSECHAR(tmp1->CHARVAL);
      break;
    case SEICSTRING:
      RESULT = UNPARSESTRING(&tmp1->STRINGVAL);
      break;
    case SEICENUM:
      RESULT = tmp1->ENUMPTR->VALUES[subrange(tmp1->ENUMORDINAL, 0, 127)];
      break;
    case SEICSET:
      RESULT = _DESCRIBEIMMSET(tmp1->SETBOUNDS, tmp1->SETOFTYPEPTR);
      break;
    default:
      INTERNALERROR(str_make(31, "Cannot describe immediate value"));
      break;
  }
  return RESULT;
}

PInteger _EXPRPRECEDENCE(TSEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  RESULT = _EXPRECEDENCES[EXPR->CLS];
  if (RESULT < 0) {
    switch (EXPR->CLS) {
      case SECTOSTRING:
        RESULT = _EXPRPRECEDENCE(EXPR->TOSTRPARENT);
        break;
      case SECTOREAL:
        RESULT = _EXPRPRECEDENCE(EXPR->TOREALPARENT);
        break;
      case SECTOUNTYPEDPTR:
        RESULT = _EXPRPRECEDENCE(EXPR->TOUNTYPEDPTRPARENT);
        break;
      case SECTOGENERICFILE:
        RESULT = _EXPRPRECEDENCE(EXPR->TOGENERICFILEPARENT);
        break;
      case SECWITHTMPVAR:
        RESULT = _EXPRPRECEDENCE(EXPR->TMPVARCHILD);
        break;
      case SECSUBRANGE:
        RESULT = _EXPRPRECEDENCE(EXPR->SUBRANGEPARENT);
        break;
      case SECUNARYOP:
        RESULT = _EXOPERATORS[EXPR->UNARY.OP].PRECEDENCE;
        break;
      case SECBINARYOP:
        RESULT = _EXOPERATORS[EXPR->BINARY.OP].PRECEDENCE;
        break;
      default:
        break;
    }
  }
  if (RESULT < 0) COMPILEERROR(str_make(33, "Unknown precedence for expression"));
  return RESULT;
}

PString _DESCRIBEUNARYOPEXPR(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  PBoolean USEPARENS;
  if (SEOORD <= EXPR->UNARY.OP && EXPR->UNARY.OP <= SEOSUCC) {
    RESULT = CONCAT(CpString, EXDESCRIBEOPERATOR(EXPR->UNARY.OP), CpChar, '(', CpString, EXDESCRIBE(EXPR->UNARY.PARENT), CpEnd | CpChar, ')');
  }
  else {
    RESULT = EXDESCRIBEOPERATOR(EXPR->UNARY.OP);
    if (EXPR->UNARY.OP != SEONEG) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ' ');
    USEPARENS = _EXPRPRECEDENCE(EXPR) < _EXPRPRECEDENCE(EXPR->UNARY.PARENT);
    if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, '(');
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXPR->UNARY.PARENT));
    if (USEPARENS) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  }
  return RESULT;
}

PString _DESCRIBEBINARYOPEXPR(TSEXPRESSIONOBJ* EXPR) {
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

PString _DESCRIBEARRAYVALUE(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  TSEARRAYELEMOBJ* ELEM;
  RESULT = str_of('(');
  ELEM = EXPR->ARRAYELEM;
  while (ELEM != PNil) {
    RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(ELEM->VALUE));
    ELEM = ELEM->NEXT;
    if (ELEM != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
  }
  RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  return RESULT;
}

PString _DESCRIBERECORDVALUE(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  TSERECORDFIELDOBJ* FIELD;
  RESULT = str_of('(');
  FIELD = EXPR->RECORDFIELD;
  while (FIELD != PNil) {
    RESULT = CONCAT(CpStringPtr, &RESULT, CpStringPtr, &EXPR->TYPEPTR->RECPTR->FIELDS[subrange(FIELD->ORDINAL, 1, 64) - 1].NAME, CpChar, ':', CpEnd | CpString, EXDESCRIBE(FIELD->VALUE));
    FIELD = FIELD->NEXT;
    if (FIELD != PNil) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, "; ");
  }
  RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
  return RESULT;
}

PString _DESCRIBESETVALUE(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  TSESETEXPRBOUNDSOBJ* BOUNDS;
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

void EXCLEARTMPVAR(TSEXPRESSIONOBJ* EXPR) {
  if (EXPR != PNil && EXPR->CLS == SECWITHTMPVAR) EXPR->TMPVARPTR->INUSE = 0;
}

TSEXPRESSIONOBJ* _EXIMMEDIATE(TSEIMMEDIATECLASS CLS) {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* EXPR;
  EXPR = _NEWEXPR(SECIMMEDIATE);
  EXPR->IMMEDIATE.CLS = CLS;
  RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* EXNIL() {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICNIL);
  RESULT->TYPEPTR = PRIMITIVETYPES.PTNIL;
  return RESULT;
}

TSEXPRESSIONOBJ* EXBOOLEANCONSTANT(PBoolean VALUE) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICBOOLEAN);
  RESULT->IMMEDIATE.BOOLEANVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
  return RESULT;
}

TSEXPRESSIONOBJ* EXINTEGERCONSTANT(PInteger VALUE) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICINTEGER);
  RESULT->IMMEDIATE.INTEGERVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  return RESULT;
}

TSEXPRESSIONOBJ* EXREALCONSTANT(PReal VALUE) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICREAL);
  RESULT->IMMEDIATE.REALVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
  return RESULT;
}

TSEXPRESSIONOBJ* EXCHARCONSTANT(PChar VALUE) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICCHAR);
  RESULT->IMMEDIATE.CHARVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTCHAR;
  return RESULT;
}

TSEXPRESSIONOBJ* EXSTRINGCONSTANT(PString VALUE) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICSTRING);
  RESULT->IMMEDIATE.STRINGVAL = VALUE;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
  return RESULT;
}

TSEXPRESSIONOBJ* EXENUMCONSTANT(PInteger ORDINAL, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  ENSUREENUMTYPE(TYPEPTR);
  if (ORDINAL < 0 || ORDINAL > TYPEPTR->ENUMPTR->SIZE - 1) COMPILEERROR(CONCAT(CpLenPtr, 18, "Invalid value for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
  RESULT = _EXIMMEDIATE(SEICENUM);
  RESULT->IMMEDIATE.ENUMORDINAL = ORDINAL;
  RESULT->IMMEDIATE.ENUMPTR = TYPEPTR->ENUMPTR;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TSEXPRESSIONOBJ* EXSETCONSTANT(TSESETIMMBOUNDSOBJ* BOUNDS, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDTYPEDEF* ELEMENTTYPE;
  ELEMENTTYPE = TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMENTTYPE != PNil) ENSUREORDINALTYPE(ELEMENTTYPE);
  RESULT = _EXIMMEDIATE(SEICSET);
  RESULT->IMMEDIATE.SETBOUNDS = BOUNDS;
  RESULT->IMMEDIATE.SETOFTYPEPTR = ELEMENTTYPE;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TSESETIMMBOUNDSOBJ* EXSETADDBOUNDS(TSESETIMMBOUNDSOBJ* BOUNDS, PInteger FIRST, PInteger LAST) {
  TSESETIMMBOUNDSOBJ* RESULT;
  TSESETIMMBOUNDSOBJ* PREV;
  TSESETIMMBOUNDSOBJ* THIS;
  TSESETIMMBOUNDSOBJ* NEWBOUNDS;
  PBoolean DONE;
  if (FIRST > LAST) COMPILEERROR(str_make(41, "Set bounds must appear in ascending order"));
  RESULT = BOUNDS;
  DONE = 0;
  PREV = PNil;
  THIS = BOUNDS;
  do {
    if (THIS == PNil || LAST + 1 < THIS->FIRST) {
      New((void**)&NEWBOUNDS, sizeof(TSESETIMMBOUNDSOBJ));
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
      Dispose((void**)&THIS);
      THIS = NEWBOUNDS;
      if (PREV == PNil) RESULT = NEWBOUNDS;
      else PREV->NEXT = NEWBOUNDS;
      DONE = 0;
    }
    else if (FIRST > THIS->FIRST && FIRST <= THIS->LAST + 1 && LAST > THIS->LAST) {
      FIRST = THIS->FIRST;
      NEWBOUNDS = THIS->NEXT;
      Dispose((void**)&THIS);
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

PBoolean EXISIMMEDIATE(TSEXPRESSIONOBJ* EXPR) {
  PBoolean RESULT;
  RESULT = EXPR->CLS == SECIMMEDIATE;
  return RESULT;
}

PBoolean EXISIMMEDIATEOFCLASS(TSEXPRESSIONOBJ* EXPR, TSEIMMEDIATECLASS CLS) {
  PBoolean RESULT;
  RESULT = EXPR->CLS == SECIMMEDIATE && EXPR->IMMEDIATE.CLS == CLS;
  return RESULT;
}

TSEXPRESSIONOBJ* EXARRAYVALUE(TSDTYPEDEF* TYPEPTR, TSEARRAYELEMOBJ* ELEMS) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(SECARRAYVALUE);
  RESULT->ARRAYELEM = ELEMS;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TSEXPRESSIONOBJ* EXRECORDVALUE(TSDTYPEDEF* TYPEPTR, TSERECORDFIELDOBJ* FIELDS) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(SECRECORDVALUE);
  RESULT->RECORDFIELD = FIELDS;
  RESULT->TYPEPTR = TYPEPTR;
  return RESULT;
}

TSEXPRESSIONOBJ* EXSET() {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXIMMEDIATE(SEICSET);
  RESULT->IMMEDIATE.SETBOUNDS = PNil;
  RESULT->IMMEDIATE.SETOFTYPEPTR = PNil;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTEMPTYSET;
  return RESULT;
}

TSEXPRESSIONOBJ* EXSETADDRANGE(TSEXPRESSIONOBJ* SETEXPR, TSEXPRESSIONOBJ* FIRST, TSEXPRESSIONOBJ* LAST) {
  TSEXPRESSIONOBJ* RESULT;
  TSDTYPEDEF* ELEMENTTYPEPTR;
  TSEXPRESSIONOBJ* IMMSET;
  TSEXPRESSIONOBJ* EXPRSET;
  TSESETEXPRBOUNDSOBJ* NEWBOUNDS;
  PString tmp1;
  TSEIMMEDIATE* tmp2;
  EXCLEARTMPVAR(FIRST);
  EXCLEARTMPVAR(LAST);
  ELEMENTTYPEPTR = SETEXPR->TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMENTTYPEPTR == PNil) {
    ELEMENTTYPEPTR = GETFUNDAMENTALTYPE(FIRST->TYPEPTR);
    SETEXPR->IMMEDIATE.SETOFTYPEPTR = ELEMENTTYPEPTR;
    SETEXPR->TYPEPTR = MAKESETTYPE(FIRST->TYPEPTR);
  }
  if (!ISFUNDAMENTALLYSAMETYPE(FIRST->TYPEPTR, ELEMENTTYPEPTR)) {
    tmp1 = CONCAT(CpLenPtr, 26, "Cannot add element to set ", CpEnd | CpString, ERRORDESCRIBEEXPR(SETEXPR));
    ERRORFOREXPR(&tmp1, FIRST);
  }
  if (LAST != PNil && !ISFUNDAMENTALLYSAMETYPE(LAST->TYPEPTR, ELEMENTTYPEPTR) && !ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) {
    tmp1 = CONCAT(CpLenPtr, 26, "Cannot add element to set ", CpEnd | CpString, ERRORDESCRIBEEXPR(SETEXPR));
    ERRORFOREXPR(&tmp1, LAST);
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
    tmp2 = &IMMSET->IMMEDIATE;
    tmp2->SETBOUNDS = EXSETADDBOUNDS(tmp2->SETBOUNDS, EXGETORDINAL(FIRST), EXGETORDINAL(LAST));
    EXDISPOSE(&FIRST);
    EXDISPOSE(&LAST);
  }
  else {
    if (EXPRSET == PNil) {
      EXPRSET = _NEWEXPR(SECSETVALUE);
      EXPRSET->SETBASE = IMMSET;
      EXPRSET->SETBOUNDS = PNil;
      EXPRSET->TYPEPTR = IMMSET->TYPEPTR;
    }
    New((void**)&NEWBOUNDS, sizeof(TSESETEXPRBOUNDSOBJ));
    NEWBOUNDS->FIRST = FIRST;
    NEWBOUNDS->LAST = LAST;
    NEWBOUNDS->NEXT = EXPRSET->SETBOUNDS;
    EXPRSET->SETBOUNDS = NEWBOUNDS;
  }
  if (EXPRSET != PNil) RESULT = EXPRSET;
  else RESULT = IMMSET;
  return RESULT;
}

TSEXPRESSIONOBJ* EXTOSTRING(TSEXPRESSIONOBJ* PARENT) {
  TSEXPRESSIONOBJ* RESULT;
  PString STR;
  ENSURESTRINGYEXPR(PARENT);
  if (ISCHARTYPE(PARENT->TYPEPTR)) {
    if (EXISIMMEDIATE(PARENT)) {
      STR = str_of(PARENT->IMMEDIATE.CHARVAL);
      PARENT->IMMEDIATE.CLS = SEICSTRING;
      PARENT->IMMEDIATE.STRINGVAL = STR;
      RESULT = PARENT;
    }
    else {
      EXCLEARTMPVAR(PARENT);
      RESULT = _NEWEXPR(SECTOSTRING);
      RESULT->TOSTRPARENT = PARENT;
      RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
      RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
    }
  }
  else RESULT = PARENT;
  return RESULT;
}

TSEXPRESSIONOBJ* EXTOREAL(TSEXPRESSIONOBJ* PARENT) {
  TSEXPRESSIONOBJ* RESULT;
  PReal VALUE;
  if (EXISIMMEDIATE(PARENT)) {
    VALUE = (double)PARENT->IMMEDIATE.INTEGERVAL;
    PARENT->IMMEDIATE.CLS = SEICREAL;
    PARENT->IMMEDIATE.REALVAL = VALUE;
    PARENT->TYPEPTR = PRIMITIVETYPES.PTREAL;
    RESULT = PARENT;
  }
  else {
    EXCLEARTMPVAR(PARENT);
    RESULT = _NEWEXPR(SECTOREAL);
    RESULT->TOREALPARENT = PARENT;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTREAL;
    RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXTOUNTYPEDPTR(TSEXPRESSIONOBJ* PARENT) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  RESULT = _NEWEXPR(SECTOUNTYPEDPTR);
  RESULT->TOUNTYPEDPTRPARENT = PARENT;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTUNTYPEDPTR;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  return RESULT;
}

TSEXPRESSIONOBJ* EXTOGENERICFILE(TSEXPRESSIONOBJ* PARENT) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  RESULT = _NEWEXPR(SECTOGENERICFILE);
  RESULT->TOGENERICFILEPARENT = PARENT;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTFILE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  return RESULT;
}

TSEXPRESSIONOBJ* EXWITHTMPVAR(TSDTMPVARDEF* TMPVAR, TSEXPRESSIONOBJ* VALUE, TSEXPRESSIONOBJ* CHILD) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(VALUE);
  EXCLEARTMPVAR(CHILD);
  RESULT = _NEWEXPR(SECWITHTMPVAR);
  RESULT->TMPVARPTR = TMPVAR;
  RESULT->TMPVARVALUE = VALUE;
  RESULT->TMPVARCHILD = CHILD;
  RESULT->TYPEPTR = CHILD->TYPEPTR;
  RESULT->ISASSIGNABLE = CHILD->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = CHILD->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = CHILD->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* EXOUTRANGE(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* TMPEXPR;
  EXCLEARTMPVAR(EXPR);
  while (EXPR->CLS == SECSUBRANGE) {
    TMPEXPR = EXCOPY(EXPR->SUBRANGEPARENT);
    EXDISPOSE(&EXPR);
    EXPR = TMPEXPR;
  }
  EXPR->TYPEPTR = GETFUNDAMENTALTYPE(EXPR->TYPEPTR);
  RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* EXVARIABLE(TSDVARIABLEDEF* VARPTR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(SECVARIABLE);
  RESULT->VARPTR = VARPTR;
  RESULT->TYPEPTR = VARPTR->TYPEPTR;
  RESULT->ISASSIGNABLE = !VARPTR->ISCONSTANT;
  RESULT->ISADDRESSABLE = 1;
  return RESULT;
}

TSEXPRESSIONOBJ* EXFIELDACCESS(TSEXPRESSIONOBJ* PARENT, PInteger FIELDNUM) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  ENSURERECORDEXPR(PARENT);
  if (FIELDNUM < 1 || FIELDNUM > PARENT->TYPEPTR->RECPTR->SIZE) COMPILEERROR(CONCAT(CpLenPtr, 23, "Invalid field for type ", CpEnd | CpString, TYPENAME(PARENT->TYPEPTR)));
  RESULT = _NEWEXPR(SECFIELD);
  RESULT->RECEXPR = PARENT;
  RESULT->RECFIELDNUM = FIELDNUM;
  RESULT->TYPEPTR = PARENT->TYPEPTR->RECPTR->FIELDS[subrange(FIELDNUM, 1, 64) - 1].TYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* EXARRAYACCESS(TSEXPRESSIONOBJ* PARENT, TSEXPRESSIONOBJ* SUBSCRIPT) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  ENSUREARRAYEXPR(PARENT);
  RESULT = _NEWEXPR(SECARRAY);
  RESULT->ARRAYEXPR = PARENT;
  RESULT->ARRAYINDEX = EXCOERCE(SUBSCRIPT, PARENT->TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  RESULT->TYPEPTR = PARENT->TYPEPTR->ARRAYDEF.VALUETYPEPTR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* EXPOINTERACCESS(TSEXPRESSIONOBJ* PARENT) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  ENSUREPOINTEREXPR(PARENT);
  RESULT = _NEWEXPR(SECPOINTER);
  RESULT->POINTEREXPR = PARENT;
  RESULT->TYPEPTR = PARENT->TYPEPTR->POINTEDTYPEPTR;
  RESULT->ISASSIGNABLE = 1;
  RESULT->ISADDRESSABLE = 1;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* EXADDRESSOF(TSEXPRESSIONOBJ* PARENT) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  RESULT = _NEWEXPR(SECADDRESS);
  RESULT->ADDRESSEXPR = PARENT;
  if (PARENT->CLS == SECFNREF) RESULT->TYPEPTR = MAKEFUNCTIONTYPE(&PARENT->FNPTR->ARGS, PARENT->FNPTR->RETURNTYPEPTR);
  else {
    ENSUREADDRESSABLEEXPR(PARENT);
    ENSUREASSIGNABLEEXPR(PARENT);
    RESULT->TYPEPTR = MAKEPOINTERTYPE(PARENT->TYPEPTR);
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXSTRINGCHAR(TSEXPRESSIONOBJ* PARENT, TSEXPRESSIONOBJ* SUBSCRIPT) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(PARENT);
  EXCLEARTMPVAR(SUBSCRIPT);
  ENSURESTRINGYEXPR(PARENT);
  ENSUREINTEGEREXPR(SUBSCRIPT);
  RESULT = _NEWEXPR(SECSTRINGCHAR);
  RESULT->ARRAYEXPR = EXTOSTRING(PARENT);
  RESULT->ARRAYINDEX = SUBSCRIPT;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTCHAR;
  RESULT->ISASSIGNABLE = PARENT->ISASSIGNABLE;
  RESULT->ISADDRESSABLE = PARENT->ISADDRESSABLE;
  RESULT->ISFUNCTIONRESULT = PARENT->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* EXFNREF(TSDSUBROUTINEDEF* FNPTR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(SECFNREF);
  RESULT->FNPTR = FNPTR;
  RESULT->TYPEPTR = PNil;
  return RESULT;
}

TSEXPRESSIONOBJ* _EXFUNCTIONCALL(TSEXPRESSIONOBJ* FNEXPR, const TSDSUBROUTINEARGS* ARGDEFS, TSDTYPEDEF* RETURNTYPEPTR, const TSEFUNCTIONARGS* ARGS) {
  TSEXPRESSIONOBJ* RESULT;
  PInteger POS;
  TSEXPRESSIONOBJ* FNCALL;
  EXCLEARTMPVAR(FNEXPR);
  if (ARGS->SIZE != ARGDEFS->COUNT) COMPILEERROR(CONCAT(CpLenPtr, 37, "Wrong number of arguments in call to ", CpEnd | CpString, EXDESCRIBE(FNEXPR)));
  FNCALL = _NEWEXPR(SECFNCALL);
  FNCALL->FNEXPR = FNEXPR;
  FNCALL->CALLARGS.SIZE = ARGS->SIZE;
  FNCALL->TYPEPTR = RETURNTYPEPTR;
  FNCALL->ISFUNCTIONRESULT = 1;
  FNCALL->ISSTATEMENT = 1;
  RESULT = FNCALL;
  for (PInteger first = 1, last = ARGS->SIZE; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      EXCLEARTMPVAR(ARGS->VALUES[subrange(POS, 1, 16) - 1]);
      FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1] = EXCOERCE(ARGS->VALUES[subrange(POS, 1, 16) - 1], ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR);
      if (ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE) {
        if (!FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]->ISADDRESSABLE) {
          if (ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT) {
            RESULT = EXWITHTMPVAR(GETALIASVARIABLE(ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].TYPEPTR, FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]), FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1], RESULT);
            FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1] = EXVARIABLE(&RESULT->TMPVARPTR->VARDEF);
          }
          else COMPILEERROR(CONCAT(CpLenPtr, 47, "Pass-by-reference argument must be assignable: ", CpEnd | CpString, EXDESCRIBE(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1])));
        }
        else if (!ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISCONSTANT) {
          ENSUREASSIGNABLEEXPR(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
          EXMARKINITIALIZED(FNCALL->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
        }
      }
    }
    break;
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXFUNCTIONCALL(TSEXPRESSIONOBJ* FNEXPR, const TSEFUNCTIONARGS* ARGS) {
  TSEXPRESSIONOBJ* RESULT;
  PString tmp1;
  if (FNEXPR->CLS == SECFNREF) RESULT = _EXFUNCTIONCALL(FNEXPR, &FNEXPR->FNPTR->ARGS, FNEXPR->FNPTR->RETURNTYPEPTR, ARGS);
  else if (ISFUNCTIONTYPE(FNEXPR->TYPEPTR)) RESULT = _EXFUNCTIONCALL(FNEXPR, &FNEXPR->TYPEPTR->FNDEFPTR->ARGS, FNEXPR->TYPEPTR->FNDEFPTR->RETURNTYPEPTR, ARGS);
  else {
    tmp1 = str_make(24, "Cannot call non-function");
    ERRORFOREXPR(&tmp1, FNEXPR);
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXPSFN(TSDPSFNDEF* SPECIALFN) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(SECPSFNREF);
  RESULT->TYPEPTR = PNil;
  RESULT->PSFNPTR = SPECIALFN;
  return RESULT;
}

TSEXPRESSIONOBJ* EXSIZEOF(TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _NEWEXPR(SECSIZEOF);
  RESULT->SIZEOFTYPEPTR = TYPEPTR;
  RESULT->TYPEPTR = PRIMITIVETYPES.PTINTEGER;
  return RESULT;
}

TSEXPRESSIONOBJ* EXCONVERTTOSTR(TSEXPRESSIONOBJ* SRC, TSEXPRESSIONOBJ* WIDTH, TSEXPRESSIONOBJ* PREC, TSEXPRESSIONOBJ* DEST) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(SRC);
  EXCLEARTMPVAR(WIDTH);
  EXCLEARTMPVAR(PREC);
  EXCLEARTMPVAR(DEST);
  RESULT = _NEWEXPR(SECCONVERTTOSTR);
  RESULT->TYPEPTR = PNil;
  RESULT->TOSTRSRC.ARG = SRC;
  RESULT->TOSTRSRC.WIDTH = WIDTH;
  RESULT->TOSTRSRC.PREC = PREC;
  RESULT->TOSTRDEST = DEST;
  RESULT->ISSTATEMENT = 1;
  return RESULT;
}

TSEXPRESSIONOBJ* EXCONVERTTOVAL(TSEXPRESSIONOBJ* SRC, TSEXPRESSIONOBJ* DEST, TSEXPRESSIONOBJ* CODE) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(SRC);
  EXCLEARTMPVAR(DEST);
  EXCLEARTMPVAR(CODE);
  RESULT = _NEWEXPR(SECCONVERTTOVAL);
  RESULT->TYPEPTR = PNil;
  RESULT->TOVALSRC = SRC;
  RESULT->TOVALDEST = DEST;
  RESULT->TOVALCODE = CODE;
  RESULT->ISSTATEMENT = 1;
  return RESULT;
}

TSEXPRESSIONOBJ* EXREAD(TSEXPRESSIONOBJ* READFILE, TSEREADARGVALUE* ARGS, PBoolean NEWLINE, PBoolean CHECKIORESULT) {
  TSEXPRESSIONOBJ* RESULT;
  TSEREADARGVALUE* ARG;
  EXCLEARTMPVAR(READFILE);
  ARG = ARGS;
  while (ARG != PNil) {
    EXCLEARTMPVAR(ARG->DEST);
    ARG = ARG->NEXT;
  }
  RESULT = _NEWEXPR(SECREAD);
  RESULT->TYPEPTR = PNil;
  RESULT->READFILE = READFILE;
  RESULT->READARGS = ARGS;
  RESULT->READLN = NEWLINE;
  RESULT->CHECKIORESULTAFTERREAD = CHECKIORESULT;
  RESULT->ISSTATEMENT = 1;
  return RESULT;
}

TSEXPRESSIONOBJ* EXWRITE(TSEXPRESSIONOBJ* WRITEFILE, TSEWRITEARGVALUE* ARGS, PBoolean NEWLINE, PBoolean CHECKIORESULT) {
  TSEXPRESSIONOBJ* RESULT;
  TSEWRITEARGVALUE* ARG;
  EXCLEARTMPVAR(WRITEFILE);
  ARG = ARGS;
  while (ARG != PNil) {
    EXCLEARTMPVAR(ARG->VALUE.ARG);
    EXCLEARTMPVAR(ARG->VALUE.WIDTH);
    EXCLEARTMPVAR(ARG->VALUE.PREC);
    ARG = ARG->NEXT;
  }
  RESULT = _NEWEXPR(SECWRITE);
  RESULT->TYPEPTR = PNil;
  RESULT->WRITEFILE = WRITEFILE;
  RESULT->WRITEARGS = ARGS;
  RESULT->WRITELN = NEWLINE;
  RESULT->CHECKIORESULTAFTERWRITE = CHECKIORESULT;
  RESULT->ISSTATEMENT = 1;
  return RESULT;
}

TSEXPRESSIONOBJ* EXRERANGE(TSEXPRESSIONOBJ* EXPR, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  if (TYPEPTR->RANGEDEF.FIRST <= EXPR->TYPEPTR->RANGEDEF.FIRST && EXPR->TYPEPTR->RANGEDEF.LAST <= TYPEPTR->RANGEDEF.LAST) {
    EXPR->TYPEPTR = TYPEPTR;
    RESULT = EXPR;
  }
  else RESULT = EXSUBRANGE(EXOUTRANGE(EXPR), TYPEPTR);
  return RESULT;
}

void EXSETCOERCETOCOMMON(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSDTYPEDEF* LEFTTYPE;
  TSDTYPEDEF* RIGHTTYPE;
  EXCLEARTMPVAR(LEFT);
  EXCLEARTMPVAR(RIGHT);
  LEFTTYPE = LEFT->TYPEPTR;
  RIGHTTYPE = RIGHT->TYPEPTR;
  if (LEFTTYPE->ELEMENTTYPEPTR == PNil || EXISIMMEDIATE(LEFT)) LEFTTYPE->ELEMENTTYPEPTR = RIGHTTYPE->ELEMENTTYPEPTR;
  else if (RIGHTTYPE->ELEMENTTYPEPTR == PNil || EXISIMMEDIATE(RIGHT)) RIGHTTYPE->ELEMENTTYPEPTR = LEFTTYPE->ELEMENTTYPEPTR;
  else if (!ISSAMETYPE(LEFTTYPE, RIGHTTYPE)) COMPILEERROR(CONCAT(CpLenPtr, 30, "Type mismatch: cannot combine ", CpString, TYPENAME(LEFT->TYPEPTR), CpLenPtr, 6, " with ", CpEnd | CpString, TYPENAME(RIGHT->TYPEPTR)));
}

TSEXPRESSIONOBJ* _EXCOERCESET(TSEXPRESSIONOBJ* EXPR, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  enum __attribute__((__packed__)) enum10 { PASS, REJECT, REPLACE } OUTCOME;
  TSDTYPEDEF* EXPRELEMTYPE;
  TSDTYPEDEF* DESTELEMTYPE;
  PString tmp1;
  EXPRELEMTYPE = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  DESTELEMTYPE = TYPEPTR->ELEMENTTYPEPTR;
  if (EXPRELEMTYPE == PNil) OUTCOME = REPLACE;
  else if (!ISFUNDAMENTALLYSAMETYPE(EXPRELEMTYPE, DESTELEMTYPE)) OUTCOME = REJECT;
  else if (EXISIMMEDIATE(EXPR)) OUTCOME = REPLACE;
  else if (EXPR->CLS == SECSETVALUE) OUTCOME = REPLACE;
  else if (GETTYPELOWBOUND(EXPRELEMTYPE) == GETTYPELOWBOUND(DESTELEMTYPE) && GETTYPEHIGHBOUND(EXPRELEMTYPE) == GETTYPEHIGHBOUND(DESTELEMTYPE)) OUTCOME = PASS;
  else OUTCOME = REJECT;
  switch (OUTCOME) {
    case REJECT:
      tmp1 = CONCAT(CpLenPtr, 20, "Cannot treat set as ", CpEnd | CpString, TYPENAME(TYPEPTR));
      ERRORFOREXPR(&tmp1, EXPR);
      break;
    case REPLACE:
      EXPR->TYPEPTR = TYPEPTR;
      if (EXPR->CLS == SECSETVALUE) EXPR->SETBASE->TYPEPTR = TYPEPTR;
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

TSSTATEMENTOBJ* STEMPTY() {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCEMPTY;
  return RESULT;
}

TSSTATEMENTOBJ* STSEQUENCE() {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCSEQUENCE;
  RESULT->SEQUENCE = PNil;
  return RESULT;
}

TSSSEQUENCEENTRY* STSEQUENCEENTRY(TSSTATEMENTOBJ* STMT) {
  TSSSEQUENCEENTRY* RESULT;
  New((void**)&RESULT, sizeof(TSSSEQUENCEENTRY));
  RESULT->NEXT = PNil;
  RESULT->STATEMENT = STMT;
  return RESULT;
}

TSSTATEMENTOBJ* STASSIGN(TSEXPRESSIONOBJ* LHS, TSEXPRESSIONOBJ* RHS) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCASSIGN;
  RESULT->LHS = LHS;
  RESULT->RHS = RHS;
  return RESULT;
}

TSSTATEMENTOBJ* STPROCCALL(TSEXPRESSIONOBJ* PROCCALL) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCPROCCALL;
  RESULT->PROCCALL = PROCCALL;
  return RESULT;
}

TSSTATEMENTOBJ* STIF(TSEXPRESSIONOBJ* COND, TSSTATEMENTOBJ* IFTHEN, TSSTATEMENTOBJ* IFELSE) {
  TSSTATEMENTOBJ* RESULT;
  if (IFTHEN == PNil) IFTHEN = STEMPTY();
  if (IFELSE == PNil) IFELSE = STEMPTY();
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCIF;
  RESULT->IFCOND = COND;
  RESULT->IFTHEN = IFTHEN;
  RESULT->IFELSE = IFELSE;
  return RESULT;
}

TSSTATEMENTOBJ* STREPEAT(TSEXPRESSIONOBJ* COND, TSSSEQUENCEENTRY* SEQUENCE) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCREPEAT;
  RESULT->UNTILCOND = COND;
  RESULT->REPEATSEQUENCE = SEQUENCE;
  return RESULT;
}

TSSTATEMENTOBJ* STWHILE(TSEXPRESSIONOBJ* COND, TSSTATEMENTOBJ* STMT) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCWHILE;
  RESULT->WHILECOND = COND;
  RESULT->WHILESTATEMENT = STMT;
  return RESULT;
}

TSSTATEMENTOBJ* STFOR(TSEXPRESSIONOBJ* ITERATOR, TSEXPRESSIONOBJ* FIRST, TSEXPRESSIONOBJ* LAST, PBoolean ASCENDING, TSSTATEMENTOBJ* STMT) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCFOR;
  RESULT->ITERATOR = ITERATOR;
  RESULT->FIRST = FIRST;
  RESULT->LAST = LAST;
  RESULT->ASCENDING = ASCENDING;
  RESULT->FORSTATEMENT = STMT;
  return RESULT;
}

TSSTATEMENTOBJ* STWITH(TSDVARIABLEDEF* WITHVAR, TSEXPRESSIONOBJ* VALUE, TSSTATEMENTOBJ* STMT) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCWITH;
  RESULT->WITHVAR = WITHVAR;
  RESULT->WITHVALUE = VALUE;
  RESULT->WITHSTATEMENT = STMT;
  return RESULT;
}

TSSTATEMENTOBJ* STCASE(TSEXPRESSIONOBJ* SELECTOR, TSSCASEENTRY* CASES) {
  TSSTATEMENTOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSSTATEMENTOBJ));
  RESULT->CLS = SSCCASE;
  RESULT->CASESELECTOR = SELECTOR;
  RESULT->CASEENTRY = CASES;
  return RESULT;
}

TSSCASEENTRY* STCASEENTRY(TSEXPRESSIONOBJ* CASELABEL, TSSTATEMENTOBJ* STMT) {
  TSSCASEENTRY* RESULT;
  New((void**)&RESULT, sizeof(TSSCASEENTRY));
  RESULT->CASELABEL = CASELABEL;
  RESULT->STATEMENT = STMT;
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOP_MAKEUNARY(TSEXPRESSIONOBJ* EXPR, TSEOPERATOR OP, TSDTYPEDEF* RESULTTYPE) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(EXPR);
  RESULT = _NEWEXPR(SECUNARYOP);
  RESULT->UNARY.PARENT = EXPR;
  RESULT->UNARY.OP = OP;
  RESULT->TYPEPTR = RESULTTYPE;
  RESULT->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOP_MAKEBINARY(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP, TSDTYPEDEF* RESULTTYPE) {
  TSEXPRESSIONOBJ* RESULT;
  EXCLEARTMPVAR(LEFT);
  EXCLEARTMPVAR(RIGHT);
  RESULT = _NEWEXPR(SECBINARYOP);
  RESULT->BINARY.LEFT = LEFT;
  RESULT->BINARY.RIGHT = RIGHT;
  RESULT->BINARY.OP = OP;
  RESULT->TYPEPTR = RESULTTYPE;
  RESULT->ISFUNCTIONRESULT = LEFT->ISFUNCTIONRESULT || RIGHT->ISFUNCTIONRESULT;
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPARITHMETIC_INTEGERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean HASLTNULL;
  PBoolean HASRTNULL;
  PBoolean HASLTNEG;
  PBoolean HASRTNEG;
  PInteger LTNULL;
  PInteger RTNULL;
  PInteger LTNEG;
  PInteger RTNEG;
  PInteger LT;
  PInteger RT;
  PInteger RET;
  HASLTNULL = 0;
  HASRTNULL = 0;
  HASLTNEG = 0;
  HASRTNEG = 0;
  LTNULL = 0;
  RTNULL = 0;
  LTNEG = 0;
  RTNEG = 0;
  switch (OP) {
    case SEOADD:
      HASLTNULL = 1;
      HASRTNULL = 1;
      break;
    case SEOSUB:
      HASLTNEG = 1;
      HASRTNULL = 1;
      break;
    case SEOMUL:
      HASLTNULL = 1;
      LTNULL = 1;
      HASRTNULL = 1;
      RTNULL = 1;
      HASLTNEG = 1;
      LTNEG = -1;
      HASRTNEG = 1;
      RTNEG = -1;
      break;
    case SEODIVINT:
      HASRTNULL = 1;
      RTNULL = 1;
      HASRTNEG = 1;
      RTNEG = -1;
      break;
    default:
      break;
  }
  if (HASLTNULL && EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.INTEGERVAL == LTNULL) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (HASRTNULL && EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == RTNULL) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (HASLTNEG && EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.INTEGERVAL == LTNEG) {
    EXDISPOSE(&LEFT);
    RESULT = EXOPNEG(RIGHT);
  }
  else if (HASRTNEG && EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == RTNEG) {
    EXDISPOSE(&RIGHT);
    RESULT = EXOPNEG(LEFT);
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.INTEGERVAL;
    RT = RIGHT->IMMEDIATE.INTEGERVAL;
    switch (OP) {
      case SEOADD:
        RET = LT + RT;
        break;
      case SEOSUB:
        RET = LT - RT;
        break;
      case SEOMUL:
        RET = LT * RT;
        break;
      case SEODIVINT:
        RET = LT / RT;
        break;
      case SEOMOD:
        RET = LT % RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = RET;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPARITHMETIC_NUMBERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean HASLTNULL;
  PBoolean HASRTNULL;
  PBoolean HASLTNEG;
  PBoolean HASRTNEG;
  PReal LTNULL;
  PReal RTNULL;
  PReal LTNEG;
  PReal RTNEG;
  PReal LT;
  PReal RT;
  PReal RET;
  HASLTNULL = 0;
  HASRTNULL = 0;
  HASLTNEG = 0;
  HASRTNEG = 0;
  LTNULL =  0.00000000000000E+000;
  RTNULL =  0.00000000000000E+000;
  LTNEG =  0.00000000000000E+000;
  RTNEG =  0.00000000000000E+000;
  switch (OP) {
    case SEOADD:
      HASLTNULL = 1;
      HASRTNULL = 1;
      break;
    case SEOSUB:
      HASLTNEG = 1;
      HASRTNULL = 1;
      break;
    case SEOMUL:
      HASLTNULL = 1;
      LTNULL =  1.00000000000000E+000;
      HASRTNULL = 1;
      RTNULL =  1.00000000000000E+000;
      HASRTNEG = 1;
      RTNEG = -1.00000000000000E+000;
      break;
    case SEODIVREAL:
      HASRTNULL = 1;
      RTNULL =  1.00000000000000E+000;
      HASRTNEG = 1;
      RTNEG = -1.00000000000000E+000;
      break;
    default:
      break;
  }
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (HASLTNULL && EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.REALVAL == LTNULL) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (HASRTNULL && EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL == RTNULL) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (HASLTNEG && EXISIMMEDIATE(LEFT) && LEFT->IMMEDIATE.REALVAL == LTNEG) {
    EXDISPOSE(&LEFT);
    RESULT = EXOPNEG(RIGHT);
  }
  else if (HASRTNEG && EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.REALVAL == RTNEG) {
    EXDISPOSE(&RIGHT);
    RESULT = EXOPNEG(LEFT);
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.REALVAL;
    RT = RIGHT->IMMEDIATE.REALVAL;
    switch (OP) {
      case SEOADD:
        RET = LT + RT;
        break;
      case SEOSUB:
        RET = LT - RT;
        break;
      case SEOMUL:
        RET = LT * RT;
        break;
      case SEODIVREAL:
        RET = LT / RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.REALVAL = RET;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTREAL);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPADD_STRINGS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  if (EXISIMMEDIATEOFCLASS(LEFT, SEICSTRING) && cmp_str(CoEq, CpStringPtr, &LEFT->IMMEDIATE.STRINGVAL, CpLenPtr, 0, "")) {
    EXDISPOSE(&LEFT);
    RESULT = RIGHT;
  }
  else if (EXISIMMEDIATEOFCLASS(RIGHT, SEICSTRING) && cmp_str(CoEq, CpStringPtr, &RIGHT->IMMEDIATE.STRINGVAL, CpLenPtr, 0, "")) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == SEICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == SEICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    RESULT = LEFT;
    RESULT->IMMEDIATE.STRINGVAL = CONCAT(CpStringPtr, &LT, CpEnd | CpStringPtr, &RT);
    RESULT->IMMEDIATE.CLS = SEICSTRING;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTSTRING;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOADD, PRIMITIVETYPES.PTSTRING);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPUNION_SETS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  TSESETIMMBOUNDSOBJ* NEWBDS;
  TSESETIMMBOUNDSOBJ* OLDBDS;
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
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOADD, LEFT->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPDIFFERENCE_SETS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  TSESETIMMBOUNDSOBJ* LTBDS;
  TSESETIMMBOUNDSOBJ* RTBDS;
  TSESETIMMBOUNDSOBJ* NEWBDS;
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
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOSUB, LEFT->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPINTERSECTION_SETS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  TSESETIMMBOUNDSOBJ* LTBDS;
  TSESETIMMBOUNDSOBJ* RTBDS;
  TSESETIMMBOUNDSOBJ* NEWBDS;
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
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOMUL, LEFT->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPBITWISE_INTEGERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.INTEGERVAL == 0) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    switch (OP) {
      case SEOSHL:
        RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL << RIGHT->IMMEDIATE.INTEGERVAL;
        break;
      case SEOSHR:
        RESULT->IMMEDIATE.INTEGERVAL = LEFT->IMMEDIATE.INTEGERVAL >> RIGHT->IMMEDIATE.INTEGERVAL;
        break;
      default:
        break;
    }
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPNOT_BOOLEAN(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = EXPR;
  if (EXISIMMEDIATE(EXPR)) RESULT->IMMEDIATE.BOOLEANVAL = !EXPR->IMMEDIATE.BOOLEANVAL;
  else if (EXPR->CLS == SECUNARYOP && EXPR->UNARY.OP == SEONOT) {
    RESULT = EXCOPY(EXPR->UNARY.PARENT);
    EXDISPOSE(&EXPR);
  }
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOEQ) RESULT->BINARY.OP = SEONE;
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEONE) RESULT->BINARY.OP = SEOEQ;
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOLT) RESULT->BINARY.OP = SEOGTEQ;
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOGT) RESULT->BINARY.OP = SEOLTEQ;
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOLTEQ && !ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR) && !ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR)) RESULT->BINARY.OP = SEOGT;
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOGTEQ && !ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR) && !ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR)) RESULT->BINARY.OP = SEOLT;
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOAND && ISBOOLEANTYPE(EXPR->TYPEPTR)) {
    RESULT = EXOPOR(EXOPNOT(EXCOPY(EXPR->BINARY.LEFT)), EXOPNOT(EXCOPY(EXPR->BINARY.RIGHT)));
    EXDISPOSE(&EXPR);
  }
  else if (EXPR->CLS == SECBINARYOP && EXPR->BINARY.OP == SEOOR && ISBOOLEANTYPE(EXPR->TYPEPTR)) {
    RESULT = EXOPAND(EXOPNOT(EXCOPY(EXPR->BINARY.LEFT)), EXOPNOT(EXCOPY(EXPR->BINARY.RIGHT)));
    EXDISPOSE(&EXPR);
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, SEONOT, EXPR->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPLOGICAL_BOOLEANS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean HASSHORTCUT;
  PBoolean SHORTCUT;
  HASSHORTCUT = 0;
  switch (OP) {
    case SEOAND:
      HASSHORTCUT = 1;
      SHORTCUT = 0;
      break;
    case SEOOR:
      HASSHORTCUT = 1;
      SHORTCUT = 1;
      break;
    default:
      break;
  }
  if (HASSHORTCUT && EXISIMMEDIATE(LEFT)) {
    if (LEFT->IMMEDIATE.BOOLEANVAL == SHORTCUT) {
      EXDISPOSE(&RIGHT);
      RESULT = LEFT;
    }
    else {
      EXDISPOSE(&LEFT);
      RESULT = RIGHT;
    }
  }
  else if (HASSHORTCUT && EXISIMMEDIATE(RIGHT) && RIGHT->IMMEDIATE.BOOLEANVAL != SHORTCUT) {
    EXDISPOSE(&RIGHT);
    RESULT = LEFT;
  }
  else if (OP == SEOXOR && EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = LEFT->IMMEDIATE.BOOLEANVAL != RIGHT->IMMEDIATE.BOOLEANVAL;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPNOT_INTEGER(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  if (EXISIMMEDIATE(EXPR)) {
    EXPR->IMMEDIATE.INTEGERVAL = ~EXPR->IMMEDIATE.INTEGERVAL;
    RESULT = EXPR;
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, SEONOT, EXPR->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPLOGICAL_INTEGERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PInteger LT;
  PInteger RT;
  PInteger RET;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.INTEGERVAL;
    RT = RIGHT->IMMEDIATE.INTEGERVAL;
    switch (OP) {
      case SEOAND:
        RET = LT & RT;
        break;
      case SEOOR:
        RET = LT | RT;
        break;
      case SEOXOR:
        RET = LT ^ RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.INTEGERVAL = RET;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPRELATIONAL_BOOLEANS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean LT;
  PBoolean RT;
  PBoolean RET;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.BOOLEANVAL;
    RT = RIGHT->IMMEDIATE.BOOLEANVAL;
    switch (OP) {
      case SEOEQ:
        RET = LT == RT;
        break;
      case SEONE:
        RET = LT != RT;
        break;
      case SEOLT:
        RET = LT < RT;
        break;
      case SEOGT:
        RET = LT > RT;
        break;
      case SEOLTEQ:
        RET = LT <= RT;
        break;
      case SEOGTEQ:
        RET = LT >= RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = RET;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPRELATIONAL_INTEGERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PInteger LT;
  PInteger RT;
  PBoolean RET;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.INTEGERVAL;
    RT = RIGHT->IMMEDIATE.INTEGERVAL;
    switch (OP) {
      case SEOEQ:
        RET = LT == RT;
        break;
      case SEONE:
        RET = LT != RT;
        break;
      case SEOLT:
        RET = LT < RT;
        break;
      case SEOGT:
        RET = LT > RT;
        break;
      case SEOLTEQ:
        RET = LT <= RT;
        break;
      case SEOGTEQ:
        RET = LT >= RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = RET;
    RESULT->IMMEDIATE.CLS = SEICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPRELATIONAL_NUMBERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PReal LT;
  PReal RT;
  PBoolean RET;
  LEFT = EXCOERCE(LEFT, PRIMITIVETYPES.PTREAL);
  RIGHT = EXCOERCE(RIGHT, PRIMITIVETYPES.PTREAL);
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.REALVAL;
    RT = RIGHT->IMMEDIATE.REALVAL;
    switch (OP) {
      case SEOEQ:
        RET = LT == RT;
        break;
      case SEONE:
        RET = LT != RT;
        break;
      case SEOLT:
        RET = LT < RT;
        break;
      case SEOGT:
        RET = LT > RT;
        break;
      case SEOLTEQ:
        RET = LT <= RT;
        break;
      case SEOGTEQ:
        RET = LT >= RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = RET;
    RESULT->IMMEDIATE.CLS = SEICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPRELATIONAL_STRINGS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PString LT;
  PString RT;
  PBoolean RET;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    if (LEFT->IMMEDIATE.CLS == SEICCHAR) LT = str_of(LEFT->IMMEDIATE.CHARVAL);
    else LT = LEFT->IMMEDIATE.STRINGVAL;
    if (RIGHT->IMMEDIATE.CLS == SEICCHAR) RT = str_of(RIGHT->IMMEDIATE.CHARVAL);
    else RT = RIGHT->IMMEDIATE.STRINGVAL;
    switch (OP) {
      case SEOEQ:
        RET = cmp_str(CoEq, CpStringPtr, &LT, CpStringPtr, &RT);
        break;
      case SEONE:
        RET = cmp_str(CoNotEq, CpStringPtr, &LT, CpStringPtr, &RT);
        break;
      case SEOLT:
        RET = cmp_str(CoBefore, CpStringPtr, &LT, CpStringPtr, &RT);
        break;
      case SEOGT:
        RET = cmp_str(CoAfter, CpStringPtr, &LT, CpStringPtr, &RT);
        break;
      case SEOLTEQ:
        RET = cmp_str(CoBeforeOrEq, CpStringPtr, &LT, CpStringPtr, &RT);
        break;
      case SEOGTEQ:
        RET = cmp_str(CoAfterOrEq, CpStringPtr, &LT, CpStringPtr, &RT);
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = RET;
    RESULT->IMMEDIATE.CLS = SEICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPRELATIONAL_ENUMS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSEXPRESSIONOBJ* RESULT;
  PInteger LT;
  PInteger RT;
  PBoolean RET;
  if (EXISIMMEDIATE(LEFT) && EXISIMMEDIATE(RIGHT)) {
    LT = LEFT->IMMEDIATE.ENUMORDINAL;
    RT = RIGHT->IMMEDIATE.ENUMORDINAL;
    switch (OP) {
      case SEOEQ:
        RET = LT == RT;
        break;
      case SEONE:
        RET = LT != RT;
        break;
      case SEOLT:
        RET = LT < RT;
        break;
      case SEOGT:
        RET = LT > RT;
        break;
      case SEOLTEQ:
        RET = LT <= RT;
        break;
      case SEOGTEQ:
        RET = LT >= RT;
        break;
      default:
        break;
    }
    RESULT = LEFT;
    RESULT->IMMEDIATE.BOOLEANVAL = RET;
    RESULT->IMMEDIATE.CLS = SEICBOOLEAN;
    RESULT->TYPEPTR = PRIMITIVETYPES.PTBOOLEAN;
    EXDISPOSE(&RIGHT);
  }
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, OP, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPEQ_SETS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  TSESETIMMBOUNDSOBJ* LTBDS;
  TSESETIMMBOUNDSOBJ* RTBDS;
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
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPGTEQ_SETS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  TSESETIMMBOUNDSOBJ* LTBDS;
  TSESETIMMBOUNDSOBJ* RTBDS;
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
  else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOGTEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPIN_IMPL(TSEXPRESSIONOBJ* NEEDLE, TSEXPRESSIONOBJ* HAYSTACK) {
  TSEXPRESSIONOBJ* RESULT;
  TSDTYPEDEF* ELEMTYPE;
  TSEXPRESSIONOBJ* IMMSET;
  TSEXPRESSIONOBJ* EXPRSET;
  TSESETIMMBOUNDSOBJ* IMMBOUNDS;
  TSESETEXPRBOUNDSOBJ* EXPRBOUNDS;
  TSEXPRESSIONOBJ* COND;
  TSDTMPVARDEF* TMPVAR;
  TSEXPRESSIONOBJ* WANTED;
  ELEMTYPE = HAYSTACK->TYPEPTR->ELEMENTTYPEPTR;
  if (ELEMTYPE == PNil) ELEMTYPE = NEEDLE->TYPEPTR;
  else NEEDLE = EXCOERCE(NEEDLE, ELEMTYPE);
  if (NEEDLE->ISFUNCTIONRESULT) {
    TMPVAR = GETALIASVARIABLE(ELEMTYPE, NEEDLE);
    WANTED = EXVARIABLE(&TMPVAR->VARDEF);
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
    RESULT = EXWITHTMPVAR(TMPVAR, NEEDLE, RESULT);
  }
  else EXDISPOSE(&NEEDLE);
  EXDISPOSE(&HAYSTACK);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPEQ_POINTERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPNE_POINTERS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPEQ_FUNCTIONS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOEQ, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

TSEXPRESSIONOBJ* _EXOPNE_FUNCTIONS(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEONE, PRIMITIVETYPES.PTBOOLEAN);
  return RESULT;
}

void WANTTOKEN(TLXTOKENID ID) {
  if (LEXER.TOKEN.ID != ID) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
}

void WANTTOKEN2(TLXTOKENID ID1, TLXTOKENID ID2) {
  if (LEXER.TOKEN.ID != ID2 && LEXER.TOKEN.ID != ID1) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID1), CpLenPtr, 4, " or ", CpString, LXTOKENNAME(ID2), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
}

void WANTTOKEN3(TLXTOKENID ID1, TLXTOKENID ID2, TLXTOKENID ID3) {
  if (LEXER.TOKEN.ID != ID3 && LEXER.TOKEN.ID != ID2 && LEXER.TOKEN.ID != ID1) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID1), CpLenPtr, 2, ", ", CpString, LXTOKENNAME(ID2), CpLenPtr, 5, ", or ", CpString, LXTOKENNAME(ID3), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
}

void WANTTOKEN4(TLXTOKENID ID1, TLXTOKENID ID2, TLXTOKENID ID3, TLXTOKENID ID4) {
  if (LEXER.TOKEN.ID != ID4 && LEXER.TOKEN.ID != ID3 && LEXER.TOKEN.ID != ID2 && LEXER.TOKEN.ID != ID1) COMPILEERROR(CONCAT(CpLenPtr, 13, "Wanted token ", CpString, LXTOKENNAME(ID1), CpLenPtr, 2, ", ", CpString, LXTOKENNAME(ID2), CpLenPtr, 2, ", ", CpString, LXTOKENNAME(ID3), CpLenPtr, 5, ", or ", CpString, LXTOKENNAME(ID4), CpLenPtr, 8, ", found ", CpEnd | CpString, LXTOKENSTR()));
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

TSDTYPEDEF* PSTYPEIDENTIFIER() {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  WANTTOKEN(TKIDENTIFIER);
  DEF = FINDNAMEOFCLASS(&LEXER.TOKEN.VALUE, SDCTYPE, 1);
  RESULT = &DEF->TYPEDEF;
  if (RESULT != PNil) RESULT->WASUSED = 1;
  READTOKEN();
  return RESULT;
}

TPSIDENTIFIER PSIDENTIFIER() {
  TPSIDENTIFIER RESULT;
  RESULT.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
  return RESULT;
}

TSEXPRESSIONOBJ* PSIMMEDIATE() {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* EXPR;
  PString tmp1;
  EXPR = PSEXPRESSION();
  if (!EXISIMMEDIATE(EXPR)) {
    tmp1 = str_make(32, "Expected an immediate expression");
    ERRORFOREXPR(&tmp1, EXPR);
  }
  RESULT = EXPR;
  return RESULT;
}

TSDTYPEDEF* PSENUMERATEDTYPE() {
  TSDTYPEDEF* RESULT;
  TSDTENUMDEF ENUM;
  PInteger POS;
  TSDCONSTANTDEF tmp1;
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
  for (PInteger first = 0, last = RESULT->ENUMPTR->SIZE - 1; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      tmp1 = MAKECONSTANT(&RESULT->ENUMPTR->VALUES[subrange(POS, 0, 127)], EXENUMCONSTANT(POS, RESULT));
      ADDCONSTANT(&tmp1);
    }
    break;
  }
  return RESULT;
}

void PSRECORDFIELD(TSDTRECORDDEF* REC, TLXTOKENID DELIMITER) {
  PString NAME;
  PInteger LASTFIELD;
  PInteger FIELD;
  TSDTYPEDEF* TYPEPTR;
  LASTFIELD = REC->SIZE;
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    for (PInteger first = 1, last = REC->SIZE; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (FIELD = first; !done; done = FIELD == last ? 1 : (++FIELD, 0)) {
        if (cmp_str(CoEq, CpStringPtr, &REC->FIELDS[subrange(FIELD, 1, 64) - 1].NAME, CpStringPtr, &NAME)) COMPILEERROR(CONCAT(CpLenPtr, 14, "A field named ", CpStringPtr, &NAME, CpEnd | CpLenPtr, 25, " has already been defined"));
      }
      break;
    }
    REC->SIZE = REC->SIZE + 1;
    if (REC->SIZE > 64) COMPILEERROR(str_make(25, "Too many fields in record"));
    REC->FIELDS[subrange(REC->SIZE, 1, 64) - 1].NAME = NAME;
    WANTTOKEN2(TKCOMMA, TKCOLON);
    SKIPTOKEN(TKCOMMA);
  } while (LEXER.TOKEN.ID != TKCOLON);
  WANTTOKENANDREAD(TKCOLON);
  TYPEPTR = PSTYPEDENOTER();
  for (PInteger first = LASTFIELD + 1, last = REC->SIZE; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (FIELD = first; !done; done = FIELD == last ? 1 : (++FIELD, 0)) REC->FIELDS[subrange(FIELD, 1, 64) - 1].TYPEPTR = TYPEPTR;
    break;
  }
  WANTTOKEN2(TKSEMICOLON, DELIMITER);
  SKIPTOKEN(TKSEMICOLON);
}

void PSRECORDVARIANTS(TSDTRECORDDEF* REC) {
  TSDEFENTRY* DEF;
  TPSIDENTIFIER TAG;
  TSDTYPEDEF* TAGTYPE;
  TSEXPRESSIONOBJ* CASELABEL;
  WANTTOKENANDREAD(TKCASE);
  TAG = PSIDENTIFIER();
  WANTTOKEN2(TKCOLON, TKOF);
  if (LEXER.TOKEN.ID == TKCOLON) {
    READTOKEN();
    TAGTYPE = PSTYPEIDENTIFIER();
    REC->SIZE = REC->SIZE + 1;
    REC->FIELDS[subrange(REC->SIZE, 1, 64) - 1].NAME = TAG.NAME;
    REC->FIELDS[subrange(REC->SIZE, 1, 64) - 1].TYPEPTR = TAGTYPE;
  }
  else {
    DEF = FINDNAMEOFCLASS(&TAG.NAME, SDCTYPE, 1);
    TAGTYPE = &DEF->TYPEDEF;
  }
  ENSUREORDINALTYPE(TAGTYPE);
  WANTTOKENANDREAD(TKOF);
  do {
    REC->NUMVARIANTS = REC->NUMVARIANTS + 1;
    REC->VARIANTBOUNDS[subrange(REC->NUMVARIANTS, 1, 64) - 1] = REC->SIZE + 1;
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

TSDTYPEDEF* PSRECORDTYPE(PBoolean ISPACKED) {
  TSDTYPEDEF* RESULT;
  TSDTRECORDDEF REC;
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

void PSARGUMENTS(TSDSUBROUTINEARGS* ARGS) {
  PBoolean ISCONST;
  PBoolean ISVAR;
  PInteger LASTARG;
  PInteger ARG;
  TSDTYPEDEF* TYPEPTR;
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
      WANTTOKEN4(TKCOMMA, TKCOLON, TKSEMICOLON, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
    } while ((TKCOLON > LEXER.TOKEN.ID || LEXER.TOKEN.ID > TKSEMICOLON) && LEXER.TOKEN.ID != TKRPAREN);
    if (LEXER.TOKEN.ID == TKCOLON) {
      SKIPTOKEN(TKCOLON);
      TYPEPTR = PSTYPEIDENTIFIER();
    }
    else if (ISVAR) TYPEPTR = PNil;
    else COMPILEERROR(str_make(44, "Untyped parameters must be pass-by-reference"));
    for (PInteger first = LASTARG + 1, last = ARGS->COUNT; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (ARG = first; !done; done = ARG == last ? 1 : (++ARG, 0)) ARGS->DEFS[subrange(ARG, 1, 16) - 1].TYPEPTR = TYPEPTR;
      break;
    }
    WANTTOKEN2(TKSEMICOLON, TKRPAREN);
    SKIPTOKEN(TKSEMICOLON);
  } while (LEXER.TOKEN.ID != TKRPAREN);
  SKIPTOKEN(TKRPAREN);
}

TSDTYPEDEF* PSRESULTTYPE() {
  TSDTYPEDEF* RESULT;
  RESULT = PSTYPEIDENTIFIER();
  return RESULT;
}

TSDTYPEDEF* PSPROCEDURETYPE() {
  TSDTYPEDEF* RESULT;
  TSDSUBROUTINEARGS ARGS;
  WANTTOKENANDREAD(TKPROCEDURE);
  if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&ARGS);
  RESULT = MAKEFUNCTIONTYPE(&ARGS, PNil);
  return RESULT;
}

TSDTYPEDEF* PSFUNCTIONTYPE() {
  TSDTYPEDEF* RESULT;
  TSDSUBROUTINEARGS ARGS;
  TSDTYPEDEF* RETURNTYPE;
  WANTTOKENANDREAD(TKFUNCTION);
  WANTTOKEN2(TKLPAREN, TKCOLON);
  if (LEXER.TOKEN.ID == TKLPAREN) PSARGUMENTS(&ARGS);
  WANTTOKENANDREAD(TKCOLON);
  RETURNTYPE = PSRESULTTYPE();
  RESULT = MAKEFUNCTIONTYPE(&ARGS, RETURNTYPE);
  return RESULT;
}

TSDTYPEDEF* _PSARRAYTYPEINTERNAL() {
  TSDTYPEDEF* RESULT;
  TSDTYPEDEF* INDEXTYPE;
  TSDTYPEDEF* VALUETYPE;
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

TSDTYPEDEF* PSARRAYTYPE() {
  TSDTYPEDEF* RESULT;
  WANTTOKENANDREAD(TKARRAY);
  WANTTOKENANDREAD(TKLBRACKET);
  RESULT = _PSARRAYTYPEINTERNAL();
  return RESULT;
}

TSDTYPEDEF* PSPOINTERTYPE() {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  WANTTOKENANDREAD(TKCARET);
  WANTTOKEN(TKIDENTIFIER);
  DEF = FINDNAMEOFCLASS(&LEXER.TOKEN.VALUE, SDCTYPE, 0);
  if (DEF == PNil) RESULT = MAKEPOINTERFORWARDTYPE(LEXER.TOKEN.VALUE);
  else {
    RESULT = MAKEPOINTERTYPE(&DEF->TYPEDEF);
    DEF->TYPEDEF.WASUSED = 1;
  }
  READTOKEN();
  return RESULT;
}

TSDTYPEDEF* PSRANGETYPE() {
  TSDTYPEDEF* RESULT;
  TSEXPRESSIONOBJ* FIRST;
  TSEXPRESSIONOBJ* LAST;
  PString tmp1;
  FIRST = PSIMMEDIATE();
  WANTTOKENANDREAD(TKRANGE);
  LAST = PSIMMEDIATE();
  ENSUREORDINALEXPR(FIRST);
  if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) {
    tmp1 = CONCAT(CpLenPtr, 9, "Expected ", CpEnd | CpString, TYPENAME(FIRST->TYPEPTR));
    ERRORFOREXPR(&tmp1, LAST);
  }
  RESULT = MAKERANGETYPE(FIRST->TYPEPTR, EXGETORDINAL(FIRST), EXGETORDINAL(LAST));
  EXDISPOSE(&FIRST);
  EXDISPOSE(&LAST);
  return RESULT;
}

TSDTYPEDEF* PSSETTYPE() {
  TSDTYPEDEF* RESULT;
  TSDTYPEDEF* ELEMENTTYPEPTR;
  PString tmp1;
  WANTTOKENANDREAD(TKSET);
  WANTTOKENANDREAD(TKOF);
  ELEMENTTYPEPTR = PSTYPEDENOTER();
  if (!ISBOUNDEDTYPE(ELEMENTTYPEPTR)) {
    tmp1 = str_make(47, "Set element types must be bounded ordinal types");
    ERRORFORTYPE(&tmp1, ELEMENTTYPEPTR);
  }
  if (GETBOUNDEDTYPESIZE(ELEMENTTYPEPTR) > 256) {
    tmp1 = str_make(54, "Set element types may not contain more than 256 values");
    ERRORFORTYPE(&tmp1, ELEMENTTYPEPTR);
  }
  RESULT = MAKESETTYPE(ELEMENTTYPEPTR);
  return RESULT;
}

TSDTYPEDEF* PSFILETYPE() {
  TSDTYPEDEF* RESULT;
  WANTTOKENANDREAD(TKFILE);
  WANTTOKENANDREAD(TKOF);
  RESULT = MAKEFILETYPE(PSTYPEIDENTIFIER());
  return RESULT;
}

void _RESOLVEPOINTERFORWARD(TSDTYPEDEF* TYPEPTR) {
  TSDEFENTRY* DEF;
  TSDTYPEDEF* TARGETPTR;
  if (ISPOINTERFORWARDTYPE(TYPEPTR)) {
    DEF = FINDNAMEOFCLASS(TYPEPTR->TARGETNAME, SDCTYPE, 1);
    TARGETPTR = &DEF->TYPEDEF;
    Dispose((void**)&TYPEPTR->TARGETNAME);
    TYPEPTR->CLS = SDTCPOINTER;
    TYPEPTR->POINTEDTYPEPTR = TARGETPTR;
    TARGETPTR->WASUSED = 1;
  }
}

void _RESOLVEPOINTERFORWARDS(TSDEFENTRY* CHECKPOINT) {
  TSDEFENTRY* DEF;
  if (CHECKPOINT == PNil) STACK_GETOLDEST(&CURRENTSCOPE->LATESTDEF, &DEF);
  else DEF = CHECKPOINT->NEWER;
  while (DEF != PNil) {
    if (DEF->CLS == SDCTYPE) _RESOLVEPOINTERFORWARD(&DEF->TYPEDEF);
    DEF = DEF->NEWER;
  }
}

void PSTYPEDEFINITIONS() {
  PString NAME;
  TSDTYPEDEF* TYPEPTR;
  TSDEFENTRY* CHECKPOINT;
  CHECKPOINT = CURRENTSCOPE->LATESTDEF;
  WANTTOKENANDREAD(TKTYPE);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKENANDREAD(TKEQUALS);
    TYPEPTR = PSTYPEDENOTER();
    WANTTOKENANDREAD(TKSEMICOLON);
    MAKEALIASTYPE(&NAME, TYPEPTR);
  } while (LEXER.TOKEN.ID == TKIDENTIFIER);
  _RESOLVEPOINTERFORWARDS(CHECKPOINT);
}

void PSCONSTANT(const PString* NAME) {
  TSDCONSTANTDEF CONSTANT;
  WANTTOKENANDREAD(TKEQUALS);
  CONSTANT.NAME = *NAME;
  CONSTANT.VALUE = PSIMMEDIATE();
  ADDCONSTANT(&CONSTANT);
}

TSEXPRESSIONOBJ* PSCONSTANTARRAY(TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  PInteger CONSTSIZE;
  PInteger WANTEDSIZE;
  TSEARRAYELEMOBJ* ARRAYELEMS;
  TSEARRAYELEMOBJ* NEWELEM;
  TLISTPTRS** ADDPOINT;
  ARRAYELEMS = PNil;
  ADDPOINT = LIST_GETADDPOINT(&ARRAYELEMS);
  WANTTOKENANDREAD(TKLPAREN);
  CONSTSIZE = 0;
  while (LEXER.TOKEN.ID != TKRPAREN) {
    CONSTSIZE = CONSTSIZE + 1;
    New((void**)&NEWELEM, sizeof(TSEARRAYELEMOBJ));
    NEWELEM->VALUE = PSCONSTANTVALUE(TYPEPTR->ARRAYDEF.VALUETYPEPTR);
    LIST_ADD(&ADDPOINT, &NEWELEM);
    WANTTOKEN2(TKCOMMA, TKRPAREN);
    SKIPTOKEN(TKCOMMA);
  }
  WANTTOKENANDREAD(TKRPAREN);
  WANTEDSIZE = GETBOUNDEDTYPESIZE(TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  if (CONSTSIZE != WANTEDSIZE) COMPILEERROR(CONCAT(CpLenPtr, 24, "Array constant has size ", CpString, INTTOSTR(CONSTSIZE), CpLenPtr, 12, " instead of ", CpString, INTTOSTR(WANTEDSIZE), CpLenPtr, 5, " for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
  RESULT = EXARRAYVALUE(TYPEPTR, ARRAYELEMS);
  return RESULT;
}

TSEXPRESSIONOBJ* PSCONSTANTRECORD(TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER FIELDID;
  TSDTYPEDEF* FIELDTYPE;
  TSERECORDFIELDOBJ* RECORDFIELDS;
  TSERECORDFIELDOBJ* NEWFIELD;
  TLISTPTRS** ADDPOINT;
  RECORDFIELDS = PNil;
  ADDPOINT = LIST_GETADDPOINT(&RECORDFIELDS);
  WANTTOKENANDREAD(TKLPAREN);
  while (LEXER.TOKEN.ID != TKRPAREN) {
    New((void**)&NEWFIELD, sizeof(TSERECORDFIELDOBJ));
    FIELDID = PSIDENTIFIER();
    WANTTOKENANDREAD(TKCOLON);
    NEWFIELD->ORDINAL = FINDFIELD(TYPEPTR, &FIELDID.NAME, 1);
    FIELDTYPE = TYPEPTR->RECPTR->FIELDS[subrange(NEWFIELD->ORDINAL, 1, 64) - 1].TYPEPTR;
    NEWFIELD->VALUE = PSCONSTANTVALUE(FIELDTYPE);
    LIST_ADD(&ADDPOINT, &NEWFIELD);
    WANTTOKEN2(TKSEMICOLON, TKRPAREN);
    SKIPTOKEN(TKSEMICOLON);
  }
  WANTTOKENANDREAD(TKRPAREN);
  RESULT = EXRECORDVALUE(TYPEPTR, RECORDFIELDS);
  return RESULT;
}

void PSTYPEDCONSTANT(const PString* NAME) {
  TSDTYPEDEF* TYPEPTR;
  TSDVARIABLEDEF tmp1;
  WANTTOKENANDREAD(TKCOLON);
  TYPEPTR = PSTYPEDENOTER();
  WANTTOKENANDREAD(TKEQUALS);
  tmp1 = MAKETYPEDCONSTANT(NAME, TYPEPTR, PSCONSTANTVALUE(TYPEPTR));
  ADDVARIABLE(&tmp1);
}

void PSCONSTDEFINITIONS() {
  PString NAME;
  WANTTOKENANDREAD(TKCONST);
  do {
    NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
    WANTTOKEN2(TKEQUALS, TKCOLON);
    if (LEXER.TOKEN.ID == TKEQUALS) PSCONSTANT(&NAME);
    else PSTYPEDCONSTANT(&NAME);
    WANTTOKENANDREAD(TKSEMICOLON);
  } while (LEXER.TOKEN.ID == TKIDENTIFIER);
}

void PSVARDEFINITIONS() {
  PInteger NUMNAMES;
  PString NAMES[8];
  TSDTYPEDEF* TYPEPTR;
  TSEXPRESSIONOBJ* LOCATION;
  TSDVARIABLEDEF tmp1;
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
    if (LEXER.TOKEN.ID == TKABSOLUTE) {
      WANTTOKENANDREAD(TKABSOLUTE);
      LOCATION = PSVARIABLE();
      ENSUREADDRESSABLEEXPR(LOCATION);
    }
    else LOCATION = PNil;
    WANTTOKENANDREAD(TKSEMICOLON);
    for (PInteger first = 1, last = NUMNAMES; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (NUMNAMES = first; !done; done = NUMNAMES == last ? 1 : (++NUMNAMES, 0)) {
        if (LOCATION == PNil) {
          tmp1 = MAKEVARIABLE(&NAMES[subrange(NUMNAMES, 1, 8) - 1], TYPEPTR);
          ADDVARIABLE(&tmp1);
        }
        else {
          tmp1 = MAKEABSOLUTE(&NAMES[subrange(NUMNAMES, 1, 8) - 1], TYPEPTR, LOCATION);
          ADDVARIABLE(&tmp1);
        }
      }
      break;
    }
  } while (LEXER.TOKEN.ID == TKIDENTIFIER);
}

TSSSEQUENCEENTRY* PSBODY() {
  TSSSEQUENCEENTRY* RESULT;
  TLISTPTRS** ADDPOINT;
  TSSSEQUENCEENTRY* NEWSTATEMENT;
  RESULT = PNil;
  ADDPOINT = LIST_GETADDPOINT(&RESULT);
  WANTTOKENANDREAD(TKBEGIN);
  while (LEXER.TOKEN.ID != TKEND) {
    New((void**)&NEWSTATEMENT, sizeof(TSSSEQUENCEENTRY));
    NEWSTATEMENT->STATEMENT = PSSTATEMENT();
    LIST_ADD(&ADDPOINT, &NEWSTATEMENT);
    WANTTOKEN2(TKSEMICOLON, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  }
  WANTTOKENANDREAD(TKEND);
  return RESULT;
}

void PSFUNCTIONBODY(TSDSUBROUTINEDEF* SRPTR) {
  PInteger POS;
  TSDVARIABLEDEF* RESULTPTR;
  TSDVARIABLEDEF tmp1;
  PString tmp2;
  STARTLOCALSCOPE(&SRPTR->SCOPE, SRPTR);
  for (PInteger first = 1, last = SRPTR->ARGS.COUNT; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      tmp1 = MAKEFROMARG(&SRPTR->ARGS.DEFS[subrange(POS, 1, 16) - 1]);
      ADDVARIABLE(&tmp1);
    }
    break;
  }
  if (SRPTR->RETURNTYPEPTR != PNil) {
    RESULTPTR = ({ tmp1 = ({ tmp2 = str_make(6, "RESULT"); MAKEVARIABLE(&tmp2, SRPTR->RETURNTYPEPTR); }); ADDVARIABLE(&tmp1); });
    RESULTPTR->WASUSED = 1;
  }
  PSDEFINITIONS();
  SRPTR->BODY = PSBODY();
  WANTTOKENANDREAD(TKSEMICOLON);
  CLOSELOCALSCOPE();
}

void PSPROCEDUREDEFINITION() {
  TSDSUBROUTINEDEF DEF;
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
    ADDFUNCTION(&DEF);
  }
  else PSFUNCTIONBODY(ADDFUNCTION(&DEF));
}

void PSFUNCTIONDEFINITION() {
  TSDSUBROUTINEDEF DEF;
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
    ADDFUNCTION(&DEF);
  }
  else PSFUNCTIONBODY(ADDFUNCTION(&DEF));
}

PString PSPROGRAMHEADING() {
  PString RESULT;
  WANTTOKENANDREAD(TKPROGRAM);
  RESULT = GETTOKENVALUEANDREAD(TKIDENTIFIER);
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
  return RESULT;
}

TSPROGRAMOBJ* PSPROGRAM() {
  TSPROGRAMOBJ* RESULT;
  New((void**)&RESULT, sizeof(TSPROGRAMOBJ));
  RESULT->NAME = PSPROGRAMHEADING();
  STARTLOCALSCOPE(&RESULT->SCOPE, PNil);
  PSDEFINITIONS();
  RESULT->BODY = PSBODY();
  CLOSELOCALSCOPE();
  return RESULT;
}

TSEXPRESSIONOBJ* PSPOINTERDEREF(TSEXPRESSIONOBJ* PTR) {
  TSEXPRESSIONOBJ* RESULT;
  if (PTR->CLS == SECVARIABLE) PTR->VARPTR->WASUSED = 1;
  WANTTOKENANDREAD(TKCARET);
  RESULT = EXPOINTERACCESS(PTR);
  return RESULT;
}

TSEFUNCTIONARGS PSFUNCTIONARGS() {
  TSEFUNCTIONARGS RESULT;
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

TSEXPRESSIONOBJ* PSFUNCTIONCALL(TSEXPRESSIONOBJ* FN) {
  TSEXPRESSIONOBJ* RESULT;
  TSEFUNCTIONARGS tmp1;
  if (FN->CLS == SECFNREF && FN->FNPTR != CURRENTSCOPE->CURRENTFN) FN->FNPTR->WASUSED = 1;
  else if (FN->CLS == SECVARIABLE) FN->VARPTR->WASUSED = 1;
  if (FN->CLS == SECFNREF) RESULT = ({ tmp1 = PSFUNCTIONARGS(); EXFUNCTIONCALL(FN, &tmp1); });
  else if (ISFUNCTIONTYPE(FN->TYPEPTR)) RESULT = ({ tmp1 = PSFUNCTIONARGS(); EXFUNCTIONCALL(FN, &tmp1); });
  else if (FN->CLS == SECPSFNREF) RESULT = FN->PSFNPTR->PARSEFN(FN);
  return RESULT;
}

TSEXPRESSIONOBJ* PSARRAYACCESS(TSEXPRESSIONOBJ* ARR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* IDX;
  PString tmp1;
  if (ARR->CLS == SECVARIABLE) ARR->VARPTR->WASUSED = 1;
  WANTTOKENANDREAD(TKLBRACKET);
  do {
    IDX = PSEXPRESSION();
    if (ISSTRINGYTYPE(ARR->TYPEPTR)) ARR = EXSTRINGCHAR(ARR, IDX);
    else ARR = EXARRAYACCESS(ARR, IDX);
    WANTTOKEN2(TKCOMMA, TKRBRACKET);
    if (LEXER.TOKEN.ID == TKCOMMA && !ISARRAYTYPE(ARR->TYPEPTR) && !ISSTRINGYTYPE(ARR->TYPEPTR)) {
      tmp1 = str_make(29, "Expected an array or a string");
      ERRORFOREXPR(&tmp1, ARR);
    }
    SKIPTOKEN(TKCOMMA);
  } while (LEXER.TOKEN.ID != TKRBRACKET);
  WANTTOKENANDREAD(TKRBRACKET);
  RESULT = ARR;
  return RESULT;
}

TSEXPRESSIONOBJ* PSFIELDACCESS(TSEXPRESSIONOBJ* REC) {
  TSEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER FLD;
  if (REC->CLS == SECVARIABLE) REC->VARPTR->WASUSED = 1;
  WANTTOKENANDREAD(TKDOT);
  FLD = PSIDENTIFIER();
  RESULT = EXFIELDACCESS(REC, FINDFIELD(REC->TYPEPTR, &FLD.NAME, 1));
  return RESULT;
}

TSEXPRESSIONOBJ* _PSVARIABLEINTERNAL(PBoolean FORSTATEMENT, PBoolean CALLFNS) {
  TSEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER ID;
  TSDWITHVARDEF* WITHVARPTR;
  TSDEFENTRY* FOUND;
  TSEXPRESSIONOBJ* EXPR;
  PBoolean DONE;
  ID = PSIDENTIFIER();
  WITHVARPTR = FINDWITHVAR(ID.NAME);
  FOUND = FINDNAME(&ID.NAME, 0);
  if (WITHVARPTR != PNil) {
    EXPR = EXVARIABLE(&WITHVARPTR->TMPVARPTR->VARDEF);
    EXPR = EXFIELDACCESS(EXPR, FINDFIELD(EXPR->TYPEPTR, &ID.NAME, 1));
  }
  else if (FOUND == PNil) COMPILEERROR(CONCAT(CpLenPtr, 20, "Unknown identifier: ", CpEnd | CpStringPtr, &ID.NAME));
  else if (FOUND->CLS == SDCVARIABLE) EXPR = EXVARIABLE(&FOUND->VARDEF);
  else if (FOUND->CLS == SDCCONSTANT) EXPR = EXCOPY(FOUND->CONSTDEF.VALUE);
  else if (FOUND->CLS == SDCSUBROUTINE) EXPR = EXFNREF(&FOUND->SRDEF);
  else if (FOUND->CLS == SDCPSFN) EXPR = EXPSFN(&FOUND->PSFNDEF);
  else COMPILEERROR(CONCAT(CpLenPtr, 20, "Invalid identifier: ", CpEnd | CpStringPtr, &ID.NAME));
  DONE = FORSTATEMENT && EXPR->CLS == SECFNREF && EXPR->FNPTR == CURRENTSCOPE->CURRENTFN && LEXER.TOKEN.ID == TKASSIGN;
  while (!DONE) {
    if (CALLFNS && (EXPR->CLS == SECFNREF || EXPR->CLS == SECPSFNREF)) EXPR = PSFUNCTIONCALL(EXPR);
    else if (CALLFNS && ISFUNCTIONTYPE(EXPR->TYPEPTR) && LEXER.TOKEN.ID == TKLPAREN) EXPR = PSFUNCTIONCALL(EXPR);
    else if (LEXER.TOKEN.ID == TKDOT) EXPR = PSFIELDACCESS(EXPR);
    else if (LEXER.TOKEN.ID == TKLBRACKET) EXPR = PSARRAYACCESS(EXPR);
    else if (LEXER.TOKEN.ID == TKCARET) EXPR = PSPOINTERDEREF(EXPR);
    else DONE = 1;
  }
  if (EXPR->CLS == SECVARIABLE && !FORSTATEMENT) EXPR->VARPTR->WASUSED = 1;
  else if (EXPR->CLS == SECFNREF && EXPR->FNPTR != CURRENTSCOPE->CURRENTFN) EXPR->FNPTR->WASUSED = 1;
  RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* PSVARIABLEORFUNCTION() {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _PSVARIABLEINTERNAL(0, 0);
  return RESULT;
}

TSEXPRESSIONOBJ* PSVARIABLEFORSTATEMENT() {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _PSVARIABLEINTERNAL(1, 1);
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

PString PARSESTRING(const PString* PSTR) {
  PString RESULT;
  enum __attribute__((__packed__)) enum10 { NONE, QUOTEDSTR, HASH, NUMCHARDEC, NUMCHARHEX, NUMCHARREADY, CARET, ERROR, DONE } STATE;
  PInteger POS;
  PChar CH;
  PInteger CHNUM;
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
      if ('0' <= CH && CH <= '9') STATE = NUMCHARDEC;
      else if (CH == '$') {
        STATE = NUMCHARHEX;
        POS = POS + 1;
      }
    }
    else if (STATE == NUMCHARDEC) {
      if ('0' <= CH && CH <= '9') {
        POS = POS + 1;
        CHNUM = CHNUM * 10 + (int)CH - 48;
      }
      else STATE = NUMCHARREADY;
    }
    else if (STATE == NUMCHARHEX) {
      if ('0' <= CH && CH <= '9' || 'A' <= CH && CH <= 'F' || 'a' <= CH && CH <= 'f') {
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

PInteger PARSEINT(const PString* PSTR) {
  PInteger RESULT;
  PInteger CODE;
  VAL_i(PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(CONCAT(CpLenPtr, 25, "Could not parse integer: ", CpEnd | CpStringPtr, PSTR));
  return RESULT;
}

PReal PARSEREAL(const PString* PSTR) {
  PReal RESULT;
  PInteger CODE;
  VAL_r(PSTR, &RESULT, &CODE);
  if (CODE != 0) COMPILEERROR(CONCAT(CpLenPtr, 29, "Could not parse real number: ", CpEnd | CpStringPtr, PSTR));
  return RESULT;
}

TSEXPRESSIONOBJ* PSSETCONSTRUCTOR() {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* FIRST;
  TSEXPRESSIONOBJ* LAST;
  PString tmp1;
  RESULT = EXSET();
  WANTTOKENANDREAD(TKLBRACKET);
  while (LEXER.TOKEN.ID != TKRBRACKET) {
    FIRST = PSEXPRESSION();
    ENSUREORDINALEXPR(FIRST);
    if (LEXER.TOKEN.ID == TKRANGE) {
      WANTTOKENANDREAD(TKRANGE);
      LAST = PSEXPRESSION();
      if (!ISSAMETYPE(FIRST->TYPEPTR, LAST->TYPEPTR)) {
        tmp1 = str_make(53, "Set element range bounds must belong to the same type");
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

TSEXPRESSIONOBJ* PSFACTOR() {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* EXPR;
  PString STR;
  PString tmp1;
  if (LEXER.TOKEN.ID == TKCARET) LXGETSTRINGFROMCARET();
  if (LEXER.TOKEN.ID == TKNIL) {
    EXPR = EXNIL();
    READTOKEN();
  }
  else if (LEXER.TOKEN.ID == TKSTRING) {
    STR = ({ tmp1 = GETTOKENVALUEANDREAD(TKSTRING); PARSESTRING(&tmp1); });
    if (LENGTH(&STR) == 1) EXPR = EXCHARCONSTANT(STR.chr[1]);
    else EXPR = EXSTRINGCONSTANT(STR);
  }
  else if (LEXER.TOKEN.ID == TKINTEGER) EXPR = EXINTEGERCONSTANT(({ tmp1 = GETTOKENVALUEANDREAD(TKINTEGER); PARSEINT(&tmp1); }));
  else if (LEXER.TOKEN.ID == TKREAL) EXPR = EXREALCONSTANT(({ tmp1 = GETTOKENVALUEANDREAD(TKREAL); PARSEREAL(&tmp1); }));
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) EXPR = PSVARIABLE();
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
    EXPR = EXADDRESSOF(PSVARIABLEORFUNCTION());
  }
  else COMPILEERROR(CONCAT(CpLenPtr, 29, "Invalid token in expression: ", CpEnd | CpString, LXTOKENSTR()));
  RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* PSBINARYOP(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TLXTOKENID OP) {
  TSEXPRESSIONOBJ* RESULT;
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

TSEXPRESSIONOBJ* PSTERM() {
  TSEXPRESSIONOBJ* RESULT;
  TLXTOKENID OP;
  TSEXPRESSIONOBJ* EXPR;
  EXPR = PSFACTOR();
  while (ISOPMULTIPLYING(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = PSBINARYOP(EXPR, PSFACTOR(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* PSSIMPLEEXPRESSION() {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean NEGATIVE;
  TLXTOKENID OP;
  TSEXPRESSIONOBJ* EXPR;
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

TSSTATEMENTOBJ* PSASSIGN(TSEXPRESSIONOBJ* LHS, TSEXPRESSIONOBJ* RHS) {
  TSSTATEMENTOBJ* RESULT;
  TSDEFENTRY* DEF;
  TSDVARIABLEDEF* RESULTVARPTR;
  PString tmp1;
  EXCLEARTMPVAR(RHS);
  if (LHS->CLS == SECFNREF) {
    if (LHS->FNPTR != CURRENTSCOPE->CURRENTFN) {
      tmp1 = str_make(35, "Cannot assign a value to a function");
      ERRORFOREXPR(&tmp1, LHS);
    }
    DEF = ({ tmp1 = str_make(6, "RESULT"); FINDNAMEOFCLASS(&tmp1, SDCVARIABLE, 1); });
    RESULTVARPTR = &DEF->VARDEF;
    EXDISPOSE(&LHS);
    LHS = EXVARIABLE(RESULTVARPTR);
  }
  RHS = EXCOERCE(RHS, LHS->TYPEPTR);
  if (!LHS->ISASSIGNABLE) {
    if (LHS->ISFUNCTIONRESULT) {
      tmp1 = str_make(41, "Cannot assign to the result of a function");
      ERRORFOREXPR(&tmp1, LHS);
    }
    else {
      tmp1 = str_make(27, "Cannot assign to a constant");
      ERRORFOREXPR(&tmp1, LHS);
    }
  }
  EXMARKINITIALIZED(LHS);
  RESULT = STASSIGN(LHS, RHS);
  return RESULT;
}

TSSTATEMENTOBJ* PSSTATEMENTSEQUENCE() {
  TSSTATEMENTOBJ* RESULT;
  RESULT = STSEQUENCE();
  RESULT->SEQUENCE = PSBODY();
  return RESULT;
}

TSSTATEMENTOBJ* PSPROCCALLSTATEMENT(TSEXPRESSIONOBJ* LHS) {
  TSSTATEMENTOBJ* RESULT;
  TSSTATEMENTOBJ* STMT;
  EXCLEARTMPVAR(LHS);
  if (LHS->CLS == SECWITHTMPVAR) {
    STMT = PSPROCCALLSTATEMENT(LHS->TMPVARCHILD);
    RESULT = STWITH(&LHS->TMPVARPTR->VARDEF, LHS->TMPVARVALUE, STMT);
  }
  else if (LHS->ISSTATEMENT) RESULT = STPROCCALL(LHS);
  else if (LEXER.TOKEN.ID == TKEQUALS) COMPILEERROR(str_make(56, "Invalid statement (maybe you wrote '=' instead of ':='?)"));
  else COMPILEERROR(str_make(17, "Invalid statement"));
  return RESULT;
}

TSSTATEMENTOBJ* PSIDENTIFIERSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* LHS;
  LHS = PSVARIABLEFORSTATEMENT();
  if (LEXER.TOKEN.ID == TKASSIGN) {
    WANTTOKENANDREAD(TKASSIGN);
    RESULT = PSASSIGN(LHS, PSEXPRESSION());
  }
  else {
    if (ISFUNCTIONTYPE(LHS->TYPEPTR)) LHS = PSFUNCTIONCALL(LHS);
    RESULT = PSPROCCALLSTATEMENT(LHS);
  }
  return RESULT;
}

TSSTATEMENTOBJ* PSIFSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* COND;
  TSSTATEMENTOBJ* IFTHEN;
  TSSTATEMENTOBJ* IFELSE;
  IFTHEN = PNil;
  IFELSE = PNil;
  WANTTOKENANDREAD(TKIF);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  WANTTOKENANDREAD(TKTHEN);
  if (LEXER.TOKEN.ID != TKELSE) IFTHEN = PSSTATEMENT();
  if (LEXER.TOKEN.ID == TKELSE) {
    WANTTOKENANDREAD(TKELSE);
    IFELSE = PSSTATEMENT();
  }
  RESULT = STIF(COND, IFTHEN, IFELSE);
  return RESULT;
}

TSSTATEMENTOBJ* PSCASESTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* CASESELECTOR;
  TSDTYPEDEF* CASETYPEPTR;
  TSEXPRESSIONOBJ* CASELABEL;
  TSSTATEMENTOBJ* STMT;
  TSSCASEENTRY* CASELIST;
  TSSCASEENTRY* CASEENTRY;
  TLISTPTRS** ADDPOINT;
  TSSSEQUENCEENTRY* SEQENTRY;
  TLISTPTRS** SEQADDPOINT;
  CASELIST = PNil;
  ADDPOINT = LIST_GETADDPOINT(&CASELIST);
  WANTTOKENANDREAD(TKCASE);
  CASESELECTOR = PSEXPRESSION();
  CASETYPEPTR = CASESELECTOR->TYPEPTR;
  ENSUREORDINALEXPR(CASESELECTOR);
  WANTTOKENANDREAD(TKOF);
  do {
    CASELABEL = EXCOERCE(PSIMMEDIATE(), CASETYPEPTR);
    WANTTOKENANDREAD(TKCOLON);
    STMT = PSSTATEMENT();
    CASEENTRY = STCASEENTRY(CASELABEL, STMT);
    LIST_ADD(&ADDPOINT, &CASEENTRY);
    WANTTOKEN3(TKSEMICOLON, TKELSE, TKEND);
    SKIPTOKEN(TKSEMICOLON);
  } while (TKELSE > LEXER.TOKEN.ID || LEXER.TOKEN.ID > TKEND);
  if (LEXER.TOKEN.ID == TKELSE) {
    READTOKEN();
    STMT = STSEQUENCE();
    SEQADDPOINT = LIST_GETADDPOINT(&STMT->SEQUENCE);
    while (LEXER.TOKEN.ID != TKEND) {
      SEQENTRY = STSEQUENCEENTRY(PSSTATEMENT());
      LIST_ADD(&SEQADDPOINT, &SEQENTRY);
      WANTTOKEN2(TKSEMICOLON, TKEND);
      SKIPTOKEN(TKSEMICOLON);
    }
    CASEENTRY = STCASEENTRY(PNil, STMT);
    LIST_ADD(&ADDPOINT, &CASEENTRY);
  }
  WANTTOKENANDREAD(TKEND);
  RESULT = STCASE(CASESELECTOR, CASELIST);
  return RESULT;
}

TSSTATEMENTOBJ* PSREPEATSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* COND;
  TSSSEQUENCEENTRY* SEQUENCE;
  TSSSEQUENCEENTRY* SEQENTRY;
  TLISTPTRS** ADDPOINT;
  SEQUENCE = PNil;
  ADDPOINT = LIST_GETADDPOINT(&SEQUENCE);
  WANTTOKENANDREAD(TKREPEAT);
  while (LEXER.TOKEN.ID != TKUNTIL) {
    SEQENTRY = STSEQUENCEENTRY(PSSTATEMENT());
    LIST_ADD(&ADDPOINT, &SEQENTRY);
    WANTTOKEN2(TKSEMICOLON, TKUNTIL);
    SKIPTOKEN(TKSEMICOLON);
  }
  WANTTOKENANDREAD(TKUNTIL);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  RESULT = STREPEAT(COND, SEQUENCE);
  return RESULT;
}

TSSTATEMENTOBJ* PSWHILESTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* COND;
  TSSTATEMENTOBJ* STMT;
  WANTTOKENANDREAD(TKWHILE);
  COND = EXCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN);
  WANTTOKENANDREAD(TKDO);
  STMT = PSSTATEMENT();
  RESULT = STWHILE(COND, STMT);
  return RESULT;
}

TSSTATEMENTOBJ* PSFORSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* ITER;
  TSEXPRESSIONOBJ* FIRST;
  TSEXPRESSIONOBJ* LAST;
  PBoolean ASCENDING;
  TSSTATEMENTOBJ* STMT;
  PString tmp1;
  WANTTOKENANDREAD(TKFOR);
  ITER = PSEXPRESSION();
  if (ITER->ISFUNCTIONRESULT) {
    tmp1 = str_make(45, "Iterator must not be the result of a function");
    ERRORFOREXPR(&tmp1, ITER);
  }
  ENSUREASSIGNABLEEXPR(ITER);
  ENSUREORDINALEXPR(ITER);
  if (ITER->CLS == SECVARIABLE) {
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
  STMT = PSSTATEMENT();
  RESULT = STFOR(ITER, FIRST, LAST, ASCENDING, STMT);
  return RESULT;
}

TSSTATEMENTOBJ* _PSWITHSTATEMENTINNER() {
  TSSTATEMENTOBJ* RESULT;
  TSEXPRESSIONOBJ* BASE;
  TSDWITHVARDEF* WITHVARPTR;
  TSSTATEMENTOBJ* STMT;
  READTOKEN();
  BASE = PSEXPRESSION();
  WITHVARPTR = ADDWITHVAR(BASE);
  WANTTOKEN2(TKCOMMA, TKDO);
  if (LEXER.TOKEN.ID == TKCOMMA) STMT = _PSWITHSTATEMENTINNER();
  else {
    WANTTOKENANDREAD(TKDO);
    STMT = PSSTATEMENT();
  }
  RESULT = STWITH(&WITHVARPTR->TMPVARPTR->VARDEF, BASE, STMT);
  WITHVARPTR->ISACTIVE = 0;
  WITHVARPTR->TMPVARPTR->INUSE = 0;
  return RESULT;
}

TSSTATEMENTOBJ* PSWITHSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  WANTTOKEN(TKWITH);
  RESULT = _PSWITHSTATEMENTINNER();
  return RESULT;
}

TSSTATEMENTOBJ* PSEMPTYSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  RESULT = STEMPTY();
  return RESULT;
}

void EXECUTEDIRECTIVE(const PString* DIR) {
  PString tmp1;
  if (LENGTH(DIR) > 3 && DIR->chr[2] == 'I' && DIR->chr[3] == ' ') {
    tmp1 = COPY(DIR, 4, 255);
    LXINCLUDE(&tmp1);
  }
  else if (LENGTH(DIR) == 3 && DIR->chr[2] == 'R' && (DIR->chr[3] == '+' || DIR->chr[3] == '-')) OPTIONS.CHECKBOUNDS = DIR->chr[3] == '+';
  else if (LENGTH(DIR) == 3 && DIR->chr[2] == 'I' && (DIR->chr[3] == '+' || DIR->chr[3] == '-')) OPTIONS.CHECKIORESULT = DIR->chr[3] == '+';
}

TSPROGRAMOBJ* PARSEPROGRAM() {
  TSPROGRAMOBJ* RESULT;
  READTOKEN();
  RESULT = PSPROGRAM();
  WANTTOKENANDREAD(TKDOT);
  WANTTOKEN(TKEOF);
  return RESULT;
}

TSEXPRESSIONOBJ* _PF_UNARY_PARSE() {
  TSEXPRESSIONOBJ* RESULT;
  WANTTOKENANDREAD(TKLPAREN);
  RESULT = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  return RESULT;
}

PString _PF_FUN_OVERLOAD(const PString* NAMEPREFIX, TSDTYPEDEF* TYPEPTR) {
  PString RESULT;
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_b");
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_i");
  else if (ISREALTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_r");
  else if (ISCHARTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_c");
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = CONCAT(CpStringPtr, NAMEPREFIX, CpEnd | CpLenPtr, 2, "_s");
  else COMPILEERROR(CONCAT(CpLenPtr, 5, "Type ", CpString, TYPENAME(TYPEPTR), CpLenPtr, 26, " is not representable for ", CpLenPtr, 12, "overload of ", CpEnd | CpStringPtr, NAMEPREFIX));
  return RESULT;
}

TSEXPRESSIONOBJ* PF_OVERLOAD_PARSE(TSEXPRESSIONOBJ* FNEXPR, PString NAMEPREFIX) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  TSEXPRESSIONOBJ* ARG;
  TSEFUNCTIONARGS ARGS;
  TSDSUBROUTINEDEF* SRPTR;
  PString tmp1;
  EXDISPOSE(&FNEXPR);
  ARG = PNil;
  if (LEXER.TOKEN.ID == TKLPAREN) {
    WANTTOKENANDREAD(TKLPAREN);
    if (LEXER.TOKEN.ID != TKRPAREN) ARG = PSEXPRESSION();
    WANTTOKENANDREAD(TKRPAREN);
  }
  if (ARG == PNil) {
    DEF = ({ tmp1 = CONCAT(CpStringPtr, &NAMEPREFIX, CpEnd | CpLenPtr, 2, "_n"); FINDNAMEOFCLASS(&tmp1, SDCSUBROUTINE, 1); });
    SRPTR = &DEF->SRDEF;
    ARGS.SIZE = 0;
    RESULT = EXFUNCTIONCALL(EXFNREF(SRPTR), &ARGS);
  }
  else {
    DEF = ({ tmp1 = _PF_FUN_OVERLOAD(&NAMEPREFIX, ARG->TYPEPTR); FINDNAMEOFCLASS(&tmp1, SDCSUBROUTINE, 1); });
    SRPTR = &DEF->SRDEF;
    ARGS.SIZE = 1;
    ARGS.VALUES[0] = ARG;
    RESULT = EXFUNCTIONCALL(EXFNREF(SRPTR), &ARGS);
  }
  return RESULT;
}

TSEWRITEARG PF_WRITEARG_PARSE() {
  TSEWRITEARG RESULT;
  RESULT.WIDTH = PNil;
  RESULT.PREC = PNil;
  RESULT.ARG = PSEXPRESSION();
  ENSURETYPEDEXPR(RESULT.ARG);
  if (!ISTEXTTYPE(RESULT.ARG->TYPEPTR) && LEXER.TOKEN.ID == TKCOLON) {
    WANTTOKENANDREAD(TKCOLON);
    RESULT.WIDTH = PSEXPRESSION();
    ENSUREINTEGEREXPR(RESULT.WIDTH);
    if (ISREALTYPE(RESULT.ARG->TYPEPTR) && LEXER.TOKEN.ID == TKCOLON) {
      WANTTOKENANDREAD(TKCOLON);
      RESULT.PREC = PSEXPRESSION();
      ENSUREINTEGEREXPR(RESULT.PREC);
    }
  }
  return RESULT;
}

TSEXPRESSIONOBJ* PFDISPOSE_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  TSEXPRESSIONOBJ* PTR;
  TSDSUBROUTINEDEF* SRPTR;
  TSEFUNCTIONARGS ARGS;
  PString tmp1;
  PTR = _PF_UNARY_PARSE();
  ENSUREASSIGNABLEEXPR(PTR);
  ENSUREPOINTEREXPR(PTR);
  EXMARKINITIALIZED(PTR);
  EXDISPOSE(&FNEXPR);
  ARGS.SIZE = 1;
  ARGS.VALUES[0] = PTR;
  DEF = ({ tmp1 = str_make(7, "Dispose"); FINDNAMEOFCLASS(&tmp1, SDCSUBROUTINE, 1); });
  SRPTR = &DEF->SRDEF;
  RESULT = EXFUNCTIONCALL(EXFNREF(SRPTR), &ARGS);
  return RESULT;
}

TSEXPRESSIONOBJ* PFNEW_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  TSEXPRESSIONOBJ* PTR;
  TSDSUBROUTINEDEF* SRPTR;
  TSEFUNCTIONARGS ARGS;
  PString tmp1;
  PTR = _PF_UNARY_PARSE();
  ENSUREASSIGNABLEEXPR(PTR);
  ENSUREPOINTEREXPR(PTR);
  EXMARKINITIALIZED(PTR);
  EXDISPOSE(&FNEXPR);
  ARGS.SIZE = 2;
  ARGS.VALUES[0] = PTR;
  ARGS.VALUES[1] = EXSIZEOF(PTR->TYPEPTR->POINTEDTYPEPTR);
  DEF = ({ tmp1 = str_make(3, "New"); FINDNAMEOFCLASS(&tmp1, SDCSUBROUTINE, 1); });
  SRPTR = &DEF->SRDEF;
  RESULT = EXFUNCTIONCALL(EXFNREF(SRPTR), &ARGS);
  return RESULT;
}

TSEXPRESSIONOBJ* PFORD_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  EXDISPOSE(&FNEXPR);
  RESULT = EXOPORD(_PF_UNARY_PARSE());
  return RESULT;
}

TSEXPRESSIONOBJ* PFPRED_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  EXDISPOSE(&FNEXPR);
  RESULT = EXOPPRED(_PF_UNARY_PARSE());
  return RESULT;
}

TSEXPRESSIONOBJ* PFRANDOM_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = PF_OVERLOAD_PARSE(FNEXPR, str_make(6, "RANDOM"));
  return RESULT;
}

TSEXPRESSIONOBJ* PFSIZEOF_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TPSIDENTIFIER ID;
  TSDEFENTRY* FOUND;
  EXDISPOSE(&FNEXPR);
  WANTTOKENANDREAD(TKLPAREN);
  ID = PSIDENTIFIER();
  WANTTOKENANDREAD(TKRPAREN);
  FOUND = FINDNAME(&ID.NAME, 1);
  if (FOUND->CLS == SDCVARIABLE) RESULT = EXSIZEOF(FOUND->VARDEF.TYPEPTR);
  else if (FOUND->CLS == SDCTYPE) RESULT = EXSIZEOF(&FOUND->TYPEDEF);
  else COMPILEERROR(CONCAT(CpLenPtr, 46, "Expected a variable or a type identifier; got ", CpEnd | CpStringPtr, &ID.NAME));
  return RESULT;
}

TSEXPRESSIONOBJ* PFSUCC_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  EXDISPOSE(&FNEXPR);
  RESULT = EXOPSUCC(_PF_UNARY_PARSE());
  return RESULT;
}

PBoolean _MODIO_TYPEISVALIDFORFILEREAD(TSEXPRESSIONOBJ* INFILE, TSEXPRESSIONOBJ* EXPR) {
  PBoolean RESULT;
  enum enum3 tmp1;
  if (EXPR->TYPEPTR == PNil) RESULT = 0;
  else if (ISTEXTTYPE(INFILE->TYPEPTR)) RESULT = ({ tmp1 = GETFUNDAMENTALTYPE(EXPR->TYPEPTR)->CLS; SDTCINTEGER <= tmp1 && tmp1 <= SDTCSTRING; });
  else if (ISFILETYPE(INFILE->TYPEPTR)) RESULT = ISSAMETYPE(INFILE->TYPEPTR->FILEDEF.TYPEPTR, EXPR->TYPEPTR);
  else RESULT = 0;
  return RESULT;
}

PBoolean _MODIO_TYPEISVALIDFORFILEWRITE(TSEXPRESSIONOBJ* OUTFILE, TSEXPRESSIONOBJ* EXPR) {
  PBoolean RESULT;
  enum enum3 tmp1;
  if (EXPR->TYPEPTR == PNil) RESULT = 0;
  else if (ISTEXTTYPE(OUTFILE->TYPEPTR)) RESULT = ({ tmp1 = GETFUNDAMENTALTYPE(EXPR->TYPEPTR)->CLS; SDTCBOOLEAN <= tmp1 && tmp1 <= SDTCSTRING || tmp1 == SDTCENUM; });
  else if (ISFILETYPE(OUTFILE->TYPEPTR)) RESULT = ISSAMETYPE(OUTFILE->TYPEPTR->FILEDEF.TYPEPTR, EXPR->TYPEPTR);
  else RESULT = 0;
  return RESULT;
}

PBoolean _MODIO_NEEDSTOMAKEADDRESSABLE(TSEXPRESSIONOBJ* OUTFILE, TSEXPRESSIONOBJ* EXPR) {
  PBoolean RESULT;
  RESULT = !ISTEXTTYPE(OUTFILE->TYPEPTR) && !EXPR->ISADDRESSABLE;
  return RESULT;
}

TSEXPRESSIONOBJ* _MODIOREAD_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  PBoolean FIRST;
  TSEXPRESSIONOBJ* READVAR;
  TSEXPRESSIONOBJ* INFILE;
  PBoolean NEWLINE;
  TSEREADARGVALUE* ARGLIST;
  TSEREADARGVALUE* READARG;
  TLISTPTRS** ARGADDPOINT;
  PString tmp1;
  NEWLINE = cmp_str(CoEq, CpStringPtr, &FNEXPR->PSFNPTR->NAME, CpLenPtr, 6, "READLN");
  EXDISPOSE(&FNEXPR);
  DEF = ({ tmp1 = str_make(5, "INPUT"); FINDNAMEOFCLASS(&tmp1, SDCVARIABLE, 1); });
  INFILE = EXVARIABLE(&DEF->VARDEF);
  ARGLIST = PNil;
  ARGADDPOINT = LIST_GETADDPOINT(&ARGLIST);
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      READVAR = PSEXPRESSION();
      if (FIRST && ISFILETYPE(READVAR->TYPEPTR)) {
        ENSUREADDRESSABLEEXPR(READVAR);
        EXDISPOSE(&INFILE);
        INFILE = READVAR;
        if (NEWLINE && !ISTEXTTYPE(INFILE->TYPEPTR)) {
          tmp1 = str_make(28, "Invalid file type for READLN");
          ERRORFOREXPR(&tmp1, INFILE);
        }
      }
      else {
        ENSUREASSIGNABLEEXPR(READVAR);
        if (!_MODIO_TYPEISVALIDFORFILEREAD(INFILE, READVAR)) {
          tmp1 = CONCAT(CpLenPtr, 38, "Variable has invalid type for READ on ", CpEnd | CpString, TYPENAME(INFILE->TYPEPTR));
          ERRORFOREXPR(&tmp1, READVAR);
        }
        New((void**)&READARG, sizeof(TSEREADARGVALUE));
        READARG->DEST = READVAR;
        EXMARKINITIALIZED(READVAR);
        LIST_ADD(&ARGADDPOINT, &READARG);
      }
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
      FIRST = 0;
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  RESULT = EXREAD(INFILE, ARGLIST, NEWLINE, OPTIONS.CHECKIORESULT);
  return RESULT;
}

TSEXPRESSIONOBJ* _MODIOWRITE_EVALUATEZEROARG(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEFUNCTIONARGS ARGS;
  ARGS.SIZE = 0;
  if (ISFUNCTIONTYPE(EXPR->TYPEPTR) && EXPR->TYPEPTR->FNDEFPTR->RETURNTYPEPTR != PNil && EXPR->TYPEPTR->FNDEFPTR->ARGS.COUNT == 0) RESULT = EXFUNCTIONCALL(EXPR, &ARGS);
  else RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* _MODIOWRITE_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  PBoolean FIRST;
  TSEWRITEARG WRITEVALUE;
  TSEXPRESSIONOBJ* OUTFILE;
  PBoolean NEWLINE;
  TSEWRITEARGVALUE* ARGLIST;
  TSEWRITEARGVALUE* WRITEARG;
  TLISTPTRS** ARGADDPOINT;
  PString tmp1;
  NEWLINE = cmp_str(CoEq, CpStringPtr, &FNEXPR->PSFNPTR->NAME, CpLenPtr, 7, "WRITELN");
  EXDISPOSE(&FNEXPR);
  DEF = ({ tmp1 = str_make(6, "OUTPUT"); FINDNAMEOFCLASS(&tmp1, SDCVARIABLE, 1); });
  OUTFILE = EXVARIABLE(&DEF->VARDEF);
  RESULT = PNil;
  ARGLIST = PNil;
  ARGADDPOINT = LIST_GETADDPOINT(&ARGLIST);
  if (LEXER.TOKEN.ID == TKLPAREN) {
    FIRST = 1;
    WANTTOKENANDREAD(TKLPAREN);
    while (LEXER.TOKEN.ID != TKRPAREN) {
      WRITEVALUE = PF_WRITEARG_PARSE();
      WRITEVALUE.ARG = _MODIOWRITE_EVALUATEZEROARG(WRITEVALUE.ARG);
      if (FIRST && ISFILETYPE(WRITEVALUE.ARG->TYPEPTR)) {
        ENSUREADDRESSABLEEXPR(WRITEVALUE.ARG);
        EXDISPOSE(&OUTFILE);
        OUTFILE = WRITEVALUE.ARG;
        if (NEWLINE && !ISTEXTTYPE(OUTFILE->TYPEPTR)) {
          tmp1 = str_make(29, "Invalid file type for WRITELN");
          ERRORFOREXPR(&tmp1, OUTFILE);
        }
      }
      else {
        if (!_MODIO_TYPEISVALIDFORFILEWRITE(OUTFILE, WRITEVALUE.ARG)) {
          tmp1 = CONCAT(CpLenPtr, 41, "Expression has invalid type for WRITE on ", CpEnd | CpString, TYPENAME(OUTFILE->TYPEPTR));
          ERRORFOREXPR(&tmp1, WRITEVALUE.ARG);
        }
        if (_MODIO_NEEDSTOMAKEADDRESSABLE(OUTFILE, WRITEVALUE.ARG)) {
          if (RESULT == PNil) {
            RESULT = EXWRITE(OUTFILE, ARGLIST, NEWLINE, OPTIONS.CHECKIORESULT);
            ARGADDPOINT = LIST_GETADDPOINT(&RESULT->WRITEARGS);
          }
          RESULT = EXWITHTMPVAR(GETALIASVARIABLE(WRITEVALUE.ARG->TYPEPTR, WRITEVALUE.ARG), WRITEVALUE.ARG, RESULT);
          WRITEVALUE.ARG = EXVARIABLE(&RESULT->TMPVARPTR->VARDEF);
        }
        if (ISENUMTYPE(WRITEVALUE.ARG->TYPEPTR)) GETFUNDAMENTALTYPE(WRITEVALUE.ARG->TYPEPTR)->ENUMPTR->NEEDVALUEARRAY = 1;
        New((void**)&WRITEARG, sizeof(TSEWRITEARGVALUE));
        WRITEARG->VALUE = WRITEVALUE;
        LIST_ADD(&ARGADDPOINT, &WRITEARG);
      }
      WANTTOKEN2(TKCOMMA, TKRPAREN);
      SKIPTOKEN(TKCOMMA);
      FIRST = 0;
    }
    WANTTOKENANDREAD(TKRPAREN);
  }
  if (RESULT == PNil) RESULT = EXWRITE(OUTFILE, ARGLIST, NEWLINE, OPTIONS.CHECKIORESULT);
  return RESULT;
}

void _UPFIRST(PString* STR) {
  PInteger POS;
  for (PInteger first = 1, last = LENGTH(STR); first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) if (POS == 1) STR->chr[POS] = UPCASE(STR->chr[POS]);
    else STR->chr[POS] = LOWERCASE(STR->chr[POS]);
    break;
  }
}

TSEXPRESSIONOBJ* _MODIO_FILEFUN_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  TSEFUNCTIONARGS ARGS;
  PString FNNAME;
  TSDSUBROUTINEDEF* SRPTR;
  FNNAME = FNEXPR->PSFNPTR->NAME;
  _UPFIRST(&FNNAME);
  EXDISPOSE(&FNEXPR);
  DEF = FINDNAMEOFCLASS(&FNNAME, SDCSUBROUTINE, 1);
  SRPTR = &DEF->SRDEF;
  ARGS = PSFUNCTIONARGS();
  ARGS.SIZE = ARGS.SIZE + 1;
  ARGS.VALUES[subrange(ARGS.SIZE, 1, 16) - 1] = EXBOOLEANCONSTANT(OPTIONS.CHECKIORESULT);
  RESULT = EXFUNCTIONCALL(EXFNREF(SRPTR), &ARGS);
  return RESULT;
}

TSEXPRESSIONOBJ* _MODIO_FILERESETFUN_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSDEFENTRY* DEF;
  TSEFUNCTIONARGS ARGS;
  PString FNNAME;
  TSDSUBROUTINEDEF* SRPTR;
  TSDTYPEDEF* FILETYPEPTR;
  FNNAME = FNEXPR->PSFNPTR->NAME;
  _UPFIRST(&FNNAME);
  EXDISPOSE(&FNEXPR);
  DEF = FINDNAMEOFCLASS(&FNNAME, SDCSUBROUTINE, 1);
  SRPTR = &DEF->SRDEF;
  ARGS = PSFUNCTIONARGS();
  ARGS.SIZE = ARGS.SIZE + 2;
  if (ARGS.VALUES[0]->CLS == SECTOGENERICFILE) FILETYPEPTR = ARGS.VALUES[0]->TOGENERICFILEPARENT->TYPEPTR;
  else FILETYPEPTR = ARGS.VALUES[0]->TYPEPTR;
  if (ISTEXTTYPE(FILETYPEPTR) || ISGENERICFILETYPE(FILETYPEPTR)) ARGS.VALUES[subrange(ARGS.SIZE - 1, 1, 16) - 1] = EXINTEGERCONSTANT(0);
  else ARGS.VALUES[subrange(ARGS.SIZE - 1, 1, 16) - 1] = EXSIZEOF(FILETYPEPTR->FILEDEF.TYPEPTR);
  ARGS.VALUES[subrange(ARGS.SIZE, 1, 16) - 1] = EXBOOLEANCONSTANT(OPTIONS.CHECKIORESULT);
  RESULT = EXFUNCTIONCALL(EXFNREF(SRPTR), &ARGS);
  return RESULT;
}

void _ADDIOPROC1(PString NAME, TSDSUBROUTINEARG ARG1) {
  PString tmp1;
  TSDSUBROUTINEDEF tmp2;
  ADDPSFN(&NAME, &_MODIO_FILEFUN_PARSE);
  _UPFIRST(&NAME);
  tmp2 = MAKEPROCEDURE2(&NAME, ARG1, ({ tmp1 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp1, PRIMITIVETYPES.PTBOOLEAN); }));
  ADDFUNCTION(&tmp2);
}

void _ADDIOPROC2(PString NAME, TSDSUBROUTINEARG ARG1, TSDSUBROUTINEARG ARG2) {
  PString tmp1;
  TSDSUBROUTINEDEF tmp2;
  ADDPSFN(&NAME, &_MODIO_FILEFUN_PARSE);
  _UPFIRST(&NAME);
  tmp2 = MAKEPROCEDURE3(&NAME, ARG1, ARG2, ({ tmp1 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp1, PRIMITIVETYPES.PTBOOLEAN); }));
  ADDFUNCTION(&tmp2);
}

void _ADDIOFUN1(PString NAME, TSDTYPEDEF* RETTYPE, TSDSUBROUTINEARG ARG1) {
  PString tmp1;
  TSDSUBROUTINEDEF tmp2;
  ADDPSFN(&NAME, &_MODIO_FILEFUN_PARSE);
  _UPFIRST(&NAME);
  tmp2 = MAKEFUNCTION2(&NAME, RETTYPE, ARG1, ({ tmp1 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp1, PRIMITIVETYPES.PTBOOLEAN); }));
  ADDFUNCTION(&tmp2);
}

void _ADDFILEPROC(PString NAME) {
  PString tmp1;
  _ADDIOPROC1(NAME, ({ tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTFILE); }));
}

void _ADDFILERESETPROC(PString NAME) {
  PString tmp1;
  PString tmp2;
  PString tmp3;
  TSDSUBROUTINEDEF tmp4;
  ADDPSFN(&NAME, &_MODIO_FILERESETFUN_PARSE);
  _UPFIRST(&NAME);
  tmp4 = MAKEPROCEDURE3(&NAME, ({ tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTFILE); }), ({ tmp2 = str_make(10, "BLOCK_SIZE"); MAKEARG(&tmp2, PRIMITIVETYPES.PTINTEGER); }), ({ tmp3 = str_make(12, "DIE_ON_ERROR"); MAKEARG(&tmp3, PRIMITIVETYPES.PTBOOLEAN); }));
  ADDFUNCTION(&tmp4);
}

void _ADDFILEPROC1(PString NAME, TSDSUBROUTINEARG ARG1) {
  PString tmp1;
  _ADDIOPROC2(NAME, ({ tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTFILE); }), ARG1);
}

void _ADDDIRPROC(PString NAME) {
  PString tmp1;
  _ADDIOPROC1(NAME, ({ tmp1 = str_make(3, "DIR"); MAKECONSTARG(&tmp1, PRIMITIVETYPES.PTSTRING); }));
}

void _ADDFILEFUN(PString NAME, TSDTYPEDEF* RETURNTYPEPTR) {
  PString tmp1;
  _ADDIOFUN1(NAME, RETURNTYPEPTR, ({ tmp1 = str_of('F'); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTFILE); }));
}

void _ADDCONSTFILEFUN(PString NAME, TSDTYPEDEF* RETURNTYPEPTR) {
  PString tmp1;
  _ADDIOFUN1(NAME, RETURNTYPEPTR, ({ tmp1 = str_of('F'); MAKECONSTARG(&tmp1, PRIMITIVETYPES.PTFILE); }));
}

void REGISTERGLOBALS_IO() {
  PString tmp1;
  TSDVARIABLEDEF tmp2;
  TSDSUBROUTINEDEF tmp3;
  PString tmp4;
  tmp2 = ({ tmp1 = str_make(5, "INPUT"); MAKEVARIABLE(&tmp1, PRIMITIVETYPES.PTTEXT); });
  ADDVARIABLE(&tmp2);
  tmp2 = ({ tmp1 = str_make(6, "OUTPUT"); MAKEVARIABLE(&tmp1, PRIMITIVETYPES.PTTEXT); });
  ADDVARIABLE(&tmp2);
  tmp2 = ({ tmp1 = str_make(6, "STDERR"); MAKEVARIABLE(&tmp1, PRIMITIVETYPES.PTTEXT); });
  ADDVARIABLE(&tmp2);
  tmp1 = str_make(4, "READ");
  ADDPSFN(&tmp1, &_MODIOREAD_PARSE);
  tmp1 = str_make(6, "READLN");
  ADDPSFN(&tmp1, &_MODIOREAD_PARSE);
  tmp1 = str_make(5, "WRITE");
  ADDPSFN(&tmp1, &_MODIOWRITE_PARSE);
  tmp1 = str_make(7, "WRITELN");
  ADDPSFN(&tmp1, &_MODIOWRITE_PARSE);
  _ADDFILEPROC1(str_make(6, "ASSIGN"), ({ tmp1 = str_make(4, "NAME"); MAKECONSTARG(&tmp1, PRIMITIVETYPES.PTSTRING); }));
  _ADDFILEPROC(str_make(5, "CLOSE"));
  _ADDFILEFUN(str_make(3, "EOF"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDFILEFUN(str_make(4, "EOLN"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDCONSTFILEFUN(str_make(7, "FILEPOS"), PRIMITIVETYPES.PTINTEGER);
  _ADDCONSTFILEFUN(str_make(8, "FILESIZE"), PRIMITIVETYPES.PTINTEGER);
  _ADDFILEPROC(str_make(5, "FLUSH"));
  tmp3 = ({ tmp1 = str_make(8, "IORESULT"); MAKEFUNCTION0(&tmp1, PRIMITIVETYPES.PTINTEGER); });
  ADDFUNCTION(&tmp3);
  _ADDFILERESETPROC(str_make(5, "RESET"));
  _ADDFILERESETPROC(str_make(7, "REWRITE"));
  _ADDFILEPROC1(str_make(4, "SEEK"), ({ tmp1 = str_make(3, "POS"); MAKEARG(&tmp1, PRIMITIVETYPES.PTINTEGER); }));
  _ADDFILEFUN(str_make(7, "SEEKEOF"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDFILEFUN(str_make(8, "SEEKEOLN"), PRIMITIVETYPES.PTBOOLEAN);
  _ADDDIRPROC(str_make(5, "CHDIR"));
  _ADDFILEPROC(str_make(5, "ERASE"));
  _ADDIOPROC2(str_make(6, "GETDIR"), ({ tmp1 = str_make(5, "DRIVE"); MAKEARG(&tmp1, PRIMITIVETYPES.PTINTEGER); }), ({ tmp4 = str_make(3, "DIR"); MAKEVARARG(&tmp4, PRIMITIVETYPES.PTSTRING); }));
  _ADDDIRPROC(str_make(5, "MKDIR"));
  _ADDFILEPROC1(str_make(6, "RENAME"), ({ tmp4 = str_make(4, "NAME"); MAKECONSTARG(&tmp4, PRIMITIVETYPES.PTSTRING); }));
  _ADDDIRPROC(str_make(5, "RMDIR"));
}

TSEXPRESSIONOBJ* _MODMATH_ABS_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = PF_OVERLOAD_PARSE(FNEXPR, str_make(3, "ABS"));
  return RESULT;
}

TSEXPRESSIONOBJ* _MODMATH_SQR_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = PF_OVERLOAD_PARSE(FNEXPR, str_make(3, "SQR"));
  return RESULT;
}

void REGISTERGLOBALS_MATH() {
  PString tmp1;
  TSDCONSTANTDEF tmp2;
  PString tmp3;
  TSDSUBROUTINEDEF tmp4;
  tmp2 = ({ tmp1 = str_make(2, "PI"); MAKECONSTANT(&tmp1, EXREALCONSTANT( 3.14159265358979E+000)); });
  ADDCONSTANT(&tmp2);
  tmp1 = str_make(3, "ABS");
  ADDPSFN(&tmp1, &_MODMATH_ABS_PARSE);
  tmp1 = str_make(3, "SQR");
  ADDPSFN(&tmp1, &_MODMATH_SQR_PARSE);
  tmp4 = ({ tmp3 = str_make(5, "ABS_i"); MAKEFUNCTION1(&tmp3, PRIMITIVETYPES.PTINTEGER, ({ tmp1 = str_make(3, "NUM"); MAKEARG(&tmp1, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(5, "ABS_r"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_make(3, "NUM"); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(6, "ARCTAN"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_make(3, "TAN"); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(3, "COS"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_make(5, "ANGLE"); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(3, "EXP"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_make(3, "POW"); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(4, "FRAC"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(3, "INT"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(2, "LN"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(3, "ODD"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTBOOLEAN, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(5, "ROUND"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTINTEGER, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(3, "SIN"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_make(5, "ANGLE"); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(5, "SQR_i"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTINTEGER, ({ tmp3 = str_make(3, "NUM"); MAKEARG(&tmp3, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(5, "SQR_r"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_make(3, "NUM"); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(4, "SQRT"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTREAL, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp1 = str_make(5, "TRUNC"); MAKEFUNCTION1(&tmp1, PRIMITIVETYPES.PTINTEGER, ({ tmp3 = str_of('X'); MAKEARG(&tmp3, PRIMITIVETYPES.PTREAL); })); });
  ADDFUNCTION(&tmp4);
}

TSEXPRESSIONOBJ* _MODSTRINGS_CONCAT_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* OPERAND;
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

TSEXPRESSIONOBJ* _MODSTRINGS_STR_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEWRITEARG SRC;
  TSEXPRESSIONOBJ* DEST;
  PString tmp1;
  WANTTOKENANDREAD(TKLPAREN);
  SRC = PF_WRITEARG_PARSE();
  WANTTOKENANDREAD(TKCOMMA);
  DEST = PSEXPRESSION();
  WANTTOKENANDREAD(TKRPAREN);
  ENSUREASSIGNABLEEXPR(DEST);
  ENSURESTRINGEXPR(DEST);
  if (!ISBOOLEANTYPE(SRC.ARG->TYPEPTR) && !ISINTEGERTYPE(SRC.ARG->TYPEPTR) && !ISREALTYPE(SRC.ARG->TYPEPTR) && !ISENUMTYPE(SRC.ARG->TYPEPTR)) {
    tmp1 = str_make(30, "Invalid type for source of STR");
    ERRORFOREXPR(&tmp1, SRC.ARG);
  }
  EXMARKINITIALIZED(DEST);
  EXDISPOSE(&FNEXPR);
  if (ISENUMTYPE(SRC.ARG->TYPEPTR)) GETFUNDAMENTALTYPE(SRC.ARG->TYPEPTR)->ENUMPTR->NEEDVALUEARRAY = 1;
  RESULT = EXCONVERTTOSTR(SRC.ARG, SRC.WIDTH, SRC.PREC, DEST);
  return RESULT;
}

TSEXPRESSIONOBJ* _MODSTRINGS_VAL_PARSE(TSEXPRESSIONOBJ* FNEXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* SRC;
  TSEXPRESSIONOBJ* DEST;
  TSEXPRESSIONOBJ* CODE;
  PString tmp1;
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
    tmp1 = str_make(30, "Invalid type for target of VAL");
    ERRORFOREXPR(&tmp1, DEST);
  }
  ENSUREASSIGNABLEEXPR(CODE);
  ENSUREINTEGEREXPR(CODE);
  EXMARKINITIALIZED(DEST);
  EXMARKINITIALIZED(CODE);
  EXDISPOSE(&FNEXPR);
  if (ISENUMTYPE(DEST->TYPEPTR)) GETFUNDAMENTALTYPE(DEST->TYPEPTR)->ENUMPTR->NEEDVALUEARRAY = 1;
  RESULT = EXCONVERTTOVAL(SRC, DEST, CODE);
  return RESULT;
}

void REGISTERGLOBALS_STRINGS() {
  PString tmp1;
  PString tmp2;
  TSDSUBROUTINEDEF tmp3;
  PString tmp4;
  PString tmp5;
  tmp1 = str_make(6, "CONCAT");
  ADDPSFN(&tmp1, &_MODSTRINGS_CONCAT_PARSE);
  tmp1 = str_make(3, "STR");
  ADDPSFN(&tmp1, &_MODSTRINGS_STR_PARSE);
  tmp1 = str_make(3, "VAL");
  ADDPSFN(&tmp1, &_MODSTRINGS_VAL_PARSE);
  tmp3 = ({ tmp2 = str_make(3, "CHR"); MAKEFUNCTION1(&tmp2, PRIMITIVETYPES.PTCHAR, ({ tmp1 = str_make(3, "POS"); MAKEARG(&tmp1, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp5 = str_make(4, "COPY"); MAKEFUNCTION3(&tmp5, PRIMITIVETYPES.PTSTRING, ({ tmp2 = str_make(3, "STR"); MAKECONSTARG(&tmp2, PRIMITIVETYPES.PTSTRING); }), ({ tmp1 = str_make(3, "POS"); MAKEARG(&tmp1, PRIMITIVETYPES.PTINTEGER); }), ({ tmp4 = str_make(3, "NUM"); MAKEARG(&tmp4, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp1 = str_make(6, "DELETE"); MAKEPROCEDURE3(&tmp1, ({ tmp5 = str_make(3, "STR"); MAKEVARARG(&tmp5, PRIMITIVETYPES.PTSTRING); }), ({ tmp4 = str_make(3, "POS"); MAKEARG(&tmp4, PRIMITIVETYPES.PTINTEGER); }), ({ tmp2 = str_make(3, "NUM"); MAKEARG(&tmp2, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp1 = str_make(6, "INSERT"); MAKEPROCEDURE3(&tmp1, ({ tmp5 = str_make(3, "INS"); MAKECONSTARG(&tmp5, PRIMITIVETYPES.PTSTRING); }), ({ tmp4 = str_make(6, "TARGET"); MAKEVARARG(&tmp4, PRIMITIVETYPES.PTSTRING); }), ({ tmp2 = str_make(3, "POS"); MAKEARG(&tmp2, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp4 = str_make(6, "LENGTH"); MAKEFUNCTION1(&tmp4, PRIMITIVETYPES.PTINTEGER, ({ tmp5 = str_make(3, "STR"); MAKECONSTARG(&tmp5, PRIMITIVETYPES.PTSTRING); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp4 = str_make(9, "LOWERCASE"); MAKEFUNCTION1(&tmp4, PRIMITIVETYPES.PTCHAR, ({ tmp5 = str_make(3, "CHR"); MAKEARG(&tmp5, PRIMITIVETYPES.PTCHAR); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp2 = str_make(3, "POS"); MAKEFUNCTION2(&tmp2, PRIMITIVETYPES.PTINTEGER, ({ tmp5 = str_make(6, "NEEDLE"); MAKECONSTARG(&tmp5, PRIMITIVETYPES.PTSTRING); }), ({ tmp4 = str_make(8, "HAYSTACK"); MAKECONSTARG(&tmp4, PRIMITIVETYPES.PTSTRING); })); });
  ADDFUNCTION(&tmp3);
  tmp3 = ({ tmp4 = str_make(6, "UPCASE"); MAKEFUNCTION1(&tmp4, PRIMITIVETYPES.PTCHAR, ({ tmp5 = str_make(3, "CHR"); MAKEARG(&tmp5, PRIMITIVETYPES.PTCHAR); })); });
  ADDFUNCTION(&tmp3);
}

void CREATEGLOBALDEFINITIONS() {
  TSDEFENTRY* DEF;
  PString tmp1;
  TSDCONSTANTDEF tmp2;
  PString tmp3;
  TSDSUBROUTINEDEF tmp4;
  PString tmp5;
  New((void**)&GLOBALDEFINITIONS, sizeof(TSSCOPEOBJ));
  PUSHGLOBALDEFS(GLOBALDEFINITIONS);
  PRIMITIVETYPES.PTNIL = ({ tmp1 = str_make(3, "NIL"); MAKEBASETYPE(&tmp1, SDTCNIL); });
  PRIMITIVETYPES.PTBOOLEAN = ({ tmp1 = str_make(7, "BOOLEAN"); MAKEBASETYPE(&tmp1, SDTCBOOLEAN); });
  PRIMITIVETYPES.PTINTEGER = ({ tmp1 = str_make(7, "INTEGER"); MAKEBASETYPE(&tmp1, SDTCINTEGER); });
  PRIMITIVETYPES.PTREAL = ({ tmp1 = str_make(4, "REAL"); MAKEBASETYPE(&tmp1, SDTCREAL); });
  PRIMITIVETYPES.PTCHAR = ({ tmp1 = str_make(4, "CHAR"); MAKEBASETYPE(&tmp1, SDTCCHAR); });
  PRIMITIVETYPES.PTSTRING = ({ tmp1 = str_make(6, "STRING"); MAKEBASETYPE(&tmp1, SDTCSTRING); });
  PRIMITIVETYPES.PTTEXT = MAKETEXTTYPE();
  PRIMITIVETYPES.PTFILE = MAKEGENERICFILETYPE();
  PRIMITIVETYPES.PTEMPTYSET = MAKESETTYPE(PNil);
  PRIMITIVETYPES.PTUNTYPEDPTR = MAKEPOINTERTYPE(PNil);
  tmp2 = ({ tmp1 = str_make(5, "FALSE"); MAKECONSTANT(&tmp1, EXBOOLEANCONSTANT(0)); });
  ADDCONSTANT(&tmp2);
  tmp2 = ({ tmp1 = str_make(4, "TRUE"); MAKECONSTANT(&tmp1, EXBOOLEANCONSTANT(1)); });
  ADDCONSTANT(&tmp2);
  tmp2 = ({ tmp1 = str_make(6, "MAXINT"); MAKECONSTANT(&tmp1, EXINTEGERCONSTANT(32767)); });
  ADDCONSTANT(&tmp2);
  tmp1 = str_make(3, "ORD");
  ADDPSFN(&tmp1, &PFORD_PARSE);
  tmp1 = str_make(4, "PRED");
  ADDPSFN(&tmp1, &PFPRED_PARSE);
  tmp1 = str_make(4, "SUCC");
  ADDPSFN(&tmp1, &PFSUCC_PARSE);
  tmp1 = str_make(7, "DISPOSE");
  ADDPSFN(&tmp1, &PFDISPOSE_PARSE);
  tmp1 = str_make(3, "NEW");
  ADDPSFN(&tmp1, &PFNEW_PARSE);
  tmp1 = str_make(6, "SIZEOF");
  ADDPSFN(&tmp1, &PFSIZEOF_PARSE);
  tmp4 = ({ tmp3 = str_make(7, "Dispose"); MAKEPROCEDURE1(&tmp3, ({ tmp1 = str_make(3, "PTR"); MAKEVARARG(&tmp1, PRIMITIVETYPES.PTUNTYPEDPTR); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp5 = str_make(3, "New"); MAKEPROCEDURE2(&tmp5, ({ tmp3 = str_make(3, "PTR"); MAKEVARARG(&tmp3, PRIMITIVETYPES.PTUNTYPEDPTR); }), ({ tmp1 = str_make(4, "SIZE"); MAKEARG(&tmp1, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  REGISTERGLOBALS_IO();
  REGISTERGLOBALS_MATH();
  REGISTERGLOBALS_STRINGS();
  tmp5 = str_make(6, "RANDOM");
  ADDPSFN(&tmp5, &PFRANDOM_PARSE);
  tmp4 = ({ tmp3 = str_make(4, "HALT"); MAKEPROCEDURE1(&tmp3, ({ tmp5 = str_make(4, "CODE"); MAKEARG(&tmp5, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp5 = str_make(10, "PARAMCOUNT"); MAKEFUNCTION0(&tmp5, PRIMITIVETYPES.PTINTEGER); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp3 = str_make(8, "PARAMSTR"); MAKEFUNCTION1(&tmp3, PRIMITIVETYPES.PTSTRING, ({ tmp5 = str_of('I'); MAKEARG(&tmp5, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp5 = str_make(8, "RANDOM_n"); MAKEFUNCTION0(&tmp5, PRIMITIVETYPES.PTREAL); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp3 = str_make(8, "RANDOM_i"); MAKEFUNCTION1(&tmp3, PRIMITIVETYPES.PTINTEGER, ({ tmp5 = str_make(3, "NUM"); MAKEARG(&tmp5, PRIMITIVETYPES.PTINTEGER); })); });
  ADDFUNCTION(&tmp4);
  tmp4 = ({ tmp5 = str_make(9, "RANDOMIZE"); MAKEPROCEDURE0(&tmp5); });
  ADDFUNCTION(&tmp4);
  DEF = CURRENTSCOPE->LATESTDEF;
  while (DEF != PNil) {
    switch (DEF->CLS) {
      case SDCVARIABLE:
        DEF->VARDEF.WASINITIALIZED = 1;
        DEF->VARDEF.WASUSED = 1;
        break;
      case SDCSUBROUTINE:
        DEF->SRDEF.WASUSED = 1;
        break;
      case SDCTYPE:
        DEF->TYPEDEF.WASUSED = 1;
        break;
      default:
        break;
    }
    DEF = DEF->OLDER;
  }
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
    for (PInteger first = 1, last = LENGTH(&OLD); first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) MATCHES = MATCHES && UPCASE(STR.chr[POS + BASELEN]) == UPCASE(OLD.chr[POS]);
      break;
    }
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
  INPUTFILE = str_make(0, "");
  OUTPUTFILE = str_make(0, "");
  SUPPRESSWARNINGS = 0;
  FLAG = FLAGNONE;
  for (PInteger first = 1, last = PARAMCOUNT(); first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
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
    break;
  }
  if (cmp_str(CoEq, CpStringPtr, &INPUTFILE, CpLenPtr, 0, "")) USAGE(str_make(28, "Input file must be specified"));
  if (cmp_str(CoEq, CpStringPtr, &OUTPUTFILE, CpLenPtr, 0, "")) {
    if (cmp_str(CoEq, CpStringPtr, &INPUTFILE, CpChar, '-')) OUTPUTFILE = str_of('-');
    else OUTPUTFILE = REPLACEEXTENSION(INPUTFILE, str_make(4, ".pas"), str_make(2, ".c"));
  }
  if (cmp_str(CoEq, CpStringPtr, &OUTPUTFILE, CpLenPtr, 0, "")) USAGE(str_make(29, "Output file must be specified"));
  if (cmp_str(CoNotEq, CpStringPtr, &INPUTFILE, CpChar, '-')) LXOPEN(INPUTFILE);
  if (cmp_str(CoNotEq, CpStringPtr, &OUTPUTFILE, CpChar, '-')) CG->SETOUTPUTFILE(CG, &OUTPUTFILE);
  OPTIONS.SUPPRESSWARNINGS = SUPPRESSWARNINGS;
  OPTIONS.CHECKBOUNDS = 1;
  OPTIONS.CHECKIORESULT = 1;
}

void _CGC_OUTNEWLINE(TCGC_OBJ* THIS) {
  Write(&THIS->OUTPUT, 1, RwpEnd | RwpLn);
  THIS->NEWLINE = 1;
}

void _CGC_OUTBLANKLINE(TCGC_OBJ* THIS, TCGC_OUTPUTTYPE NEWOUT) {
  if (THIS->INDENT == 0 && (THIS->LASTOUT != NEWOUT || NEWOUT == TOTFUNDEF)) _CGC_OUTNEWLINE(THIS);
  THIS->LASTOUT = NEWOUT;
}

void _CGC_OUTINDENT(TCGC_OBJ* THIS) {
  PInteger CT;
  if (THIS->NEWLINE) for (PInteger first = 1, last = THIS->INDENT; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (CT = first; !done; done = CT == last ? 1 : (++CT, 0)) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "  ");
    break;
  }
  THIS->NEWLINE = 0;
}

void _CGC_OUTBEGIN(TCGC_OBJ* THIS) {
  THIS->ISMULTISTATEMENT = 1;
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '{');
  _CGC_OUTNEWLINE(THIS);
  THIS->INDENT = THIS->INDENT + 1;
}

void _CGC_OUTEND(TCGC_OBJ* THIS) {
  THIS->INDENT = THIS->INDENT - 1;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '}');
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTENDSAMELINE(TCGC_OBJ* THIS) {
  THIS->INDENT = THIS->INDENT - 1;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '}');
}

PString _CGC_GETRANGETYPE(TSDTYPEDEF* TYPEPTR) {
  typedef enum __attribute__((__packed__)) enum11 { U8, S8, U16, S16, U32, S32 } TYPES;
  PString RESULT;
  const struct record49 {
    PString NAME;
    PInteger LOW;
    PInteger HIGH;
  } TYPEINFO[6] = { { .NAME = str_make(6, "PBits8"), .LOW = 0, .HIGH = 255 }, { .NAME = str_make(7, "PBits8S"), .LOW = -128, .HIGH = 127 }, { .NAME = str_make(7, "PBits16"), .LOW = 0, .HIGH = 65535 }, { .NAME = str_make(8, "PBits16S"), .LOW = -32768, .HIGH = 32767 }, { .NAME = str_make(7, "PBits32"), .LOW = 0, .HIGH = 2147483647 }, { .NAME = str_make(8, "PBits32S"), .LOW = -2147483648, .HIGH = 2147483647 } };
  PSet8 FITTYPES;
  PInteger LOW;
  PInteger HIGH;
  TYPES T;
  FITTYPES = (PSet8) { 0 };
  LOW = GETTYPELOWBOUND(TYPEPTR);
  HIGH = GETTYPEHIGHBOUND(TYPEPTR);
  for (PInteger first = U8, last = S32; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (T = first; !done; done = T == last ? 1 : (++T, 0)) if (LOW >= TYPEINFO[T].LOW && HIGH <= TYPEINFO[T].HIGH) FITTYPES = ({ PSet8 dst; set_union(FITTYPES.bits, ({ PSet8 dst = (PSet8) { 0 }; set_set(T, T, 0, dst.bits); dst; }).bits, dst.bits, 1); dst; });
    break;
  }
  RESULT = str_make(8, "PInteger");
  for (PInteger first = S32, last = U8; first >= last; /*breaks*/) {
    PBoolean done = 0;
    for (T = first; !done; done = T == last ? 1 : (--T, 0)) if (set_in(T, 0, FITTYPES.bits)) RESULT = TYPEINFO[T].NAME;
    break;
  }
  return RESULT;
}

PInteger _CGC_GETBINOPPRECEDENCE(TSEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  PBoolean ISSETLEFT;
  PBoolean ISSETRIGHT;
  ISSETLEFT = ISSETTYPE(EXPR->BINARY.LEFT->TYPEPTR);
  ISSETRIGHT = ISSETTYPE(EXPR->BINARY.RIGHT->TYPEPTR);
  switch (EXPR->BINARY.OP) {
    case SEOADD:
      if (ISSTRINGYTYPE(EXPR->TYPEPTR)) RESULT = 1;
      else RESULT = 4;
      break;
    case SEOSUB:
      RESULT = 4;
      break;
    case SEOMUL:
      RESULT = 3;
      break;
    case SEODIVREAL:
      RESULT = 3;
      break;
    case SEODIVINT:
      RESULT = 3;
      break;
    case SEOMOD:
      RESULT = 3;
      break;
    case SEOAND:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 11;
      else RESULT = 8;
      break;
    case SEOOR:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 12;
      else RESULT = 10;
      break;
    case SEOXOR:
      if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = 7;
      else RESULT = 9;
      break;
    case SEOSHL:
      RESULT = 5;
      break;
    case SEOSHR:
      RESULT = 5;
      break;
    case SEOIN:
      if (EXISIMMEDIATE(EXPR->BINARY.RIGHT)) RESULT = 12;
      else RESULT = 1;
      break;
    case SEOEQ:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 7;
      break;
    case SEONE:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 7;
      break;
    case SEOLT:
      RESULT = 6;
      break;
    case SEOGT:
      RESULT = 6;
      break;
    case SEOLTEQ:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 6;
      break;
    case SEOGTEQ:
      if (ISSETLEFT && ISSETRIGHT) RESULT = 1;
      else RESULT = 6;
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 35, "Unknown precedence for operator in ", CpEnd | CpString, EXDESCRIBE(EXPR)));
      break;
  }
  return RESULT;
}

PInteger _CGC_GETPRECEDENCE(TSEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  switch (EXPR->CLS) {
    case SECIMMEDIATE:
      RESULT = 0;
      break;
    case SECARRAYVALUE:
      RESULT = 1;
      break;
    case SECRECORDVALUE:
      RESULT = 1;
      break;
    case SECSETVALUE:
      RESULT = 0;
      break;
    case SECTOSTRING:
      RESULT = 0;
      break;
    case SECTOREAL:
      RESULT = 2;
      break;
    case SECTOUNTYPEDPTR:
      RESULT = _CGC_GETPRECEDENCE(EXPR->TOUNTYPEDPTRPARENT);
      break;
    case SECTOGENERICFILE:
      RESULT = _CGC_GETPRECEDENCE(EXPR->TOGENERICFILEPARENT);
      break;
    case SECWITHTMPVAR:
      RESULT = 0;
      break;
    case SECSUBRANGE:
      RESULT = 0;
      break;
    case SECVARIABLE:
      if (EXPR->VARPTR->ISREFERENCE) RESULT = 2;
      else RESULT = 0;
      break;
    case SECFIELD:
      RESULT = 1;
      break;
    case SECARRAY:
      RESULT = 1;
      break;
    case SECPOINTER:
      RESULT = 2;
      break;
    case SECADDRESS:
      RESULT = 2;
      break;
    case SECSTRINGCHAR:
      RESULT = 1;
      break;
    case SECFNREF:
      RESULT = 0;
      break;
    case SECFNCALL:
      RESULT = 1;
      break;
    case SECPSFNREF:
      RESULT = 0;
      break;
    case SECSIZEOF:
      RESULT = 1;
      break;
    case SECCONVERTTOSTR:
      RESULT = 1;
      break;
    case SECCONVERTTOVAL:
      RESULT = 1;
      break;
    case SECREAD:
      RESULT = 1;
      break;
    case SECWRITE:
      RESULT = 1;
      break;
    case SECUNARYOP:
      RESULT = 2;
      break;
    case SECBINARYOP:
      RESULT = _CGC_GETBINOPPRECEDENCE(EXPR);
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 23, "Unknown precedence for ", CpEnd | CpString, EXDESCRIBE(EXPR)));
      break;
  }
  return RESULT;
}

void _CGC_OUTSETTYPENAME(TCGC_OBJ* THIS, TSDTYPEDEF* TYPEPTR) {
  PInteger NUMBYTES;
  NUMBYTES = GETTYPEHIGHBOUND(TYPEPTR->ELEMENTTYPEPTR) / 8 - GETTYPELOWBOUND(TYPEPTR->ELEMENTTYPEPTR) / 8 + 1;
  Write(&THIS->OUTPUT, 1, RwpLenPtr, 4, "PSet", RwpInt | RwpEnd, 8 * NUMBYTES);
}

void _CGC_OUTVARARGDECLARATION(TCGC_OBJ* THIS, PString NAME, PBoolean ISREFERENCE, PBoolean ISCONSTANT, TSDTYPEDEF* TYPEPTR) {
  if (ISCONSTANT) {
    if (ISREFERENCE && ISPOINTERTYPE(TYPEPTR)) NAME = CONCAT(CpLenPtr, 7, " const ", CpEnd | CpStringPtr, &NAME);
    else Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "const ");
  }
  if (ISREFERENCE) NAME = CONCAT(CpLenPtr, 2, "* ", CpEnd | CpStringPtr, &NAME);
  _CGC_OUTNAMEANDTYPE(THIS, &NAME, TYPEPTR);
}

void _CGC_OUTVARIABLEDECLARATION(TCGC_OBJ* THIS, TSDVARIABLEDEF* VARDEF) {
  _CGC_OUTVARARGDECLARATION(THIS, VARDEF->NAME, VARDEF->ISREFERENCE, VARDEF->ISCONSTANT, VARDEF->TYPEPTR);
}

void _CGC_OUTARGUMENTDECLARATION(TCGC_OBJ* THIS, const TSDSUBROUTINEARG* ARGDEF) {
  _CGC_OUTVARARGDECLARATION(THIS, ARGDEF->NAME, ARGDEF->ISREFERENCE, ARGDEF->ISCONSTANT, ARGDEF->TYPEPTR);
}

void _CGC_OUTBOUNDS(TCGC_OBJ* THIS, TSDTYPEDEF* TYPEPTR) {
  Write(&THIS->OUTPUT, 1, RwpInt, GETTYPELOWBOUND(TYPEPTR), RwpLenPtr, 2, ", ", RwpInt | RwpEnd, GETTYPEHIGHBOUND(TYPEPTR));
}

void _CGC_OUTEXPRESSIONPARENSPREC(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR, PInteger PREC) {
  PBoolean USEPARENS;
  USEPARENS = _CGC_GETPRECEDENCE(EXPR) > PREC;
  if (USEPARENS) Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '(');
  _CGC_OUTEXPRESSION(THIS, EXPR);
  if (USEPARENS) Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _CGC_OUTEXPRESSIONPARENS(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR, TSEXPRESSIONOBJ* REF) {
  _CGC_OUTEXPRESSIONPARENSPREC(THIS, EXPR, _CGC_GETPRECEDENCE(REF));
}

void _CGC_OUTEXPRESSIONPARENSEXTRA(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR, TSEXPRESSIONOBJ* REF) {
  _CGC_OUTEXPRESSIONPARENSPREC(THIS, EXPR, _CGC_GETPRECEDENCE(REF) - 1);
}

void _CGC_OUTEXESCAPEDCHAR(TCGC_OBJ* THIS, PChar CH) {
  PInteger CODE;
  PInteger N1;
  PInteger N2;
  CODE = (int)CH;
  N1 = CODE / 16;
  N2 = CODE % 16;
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\x");
  if (N1 < 10) Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, CHR(N1 + 48));
  else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, CHR(N1 + 87));
  if (N2 < 10) Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, CHR(N2 + 48));
  else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, CHR(N2 + 87));
}

void _CGC_OUTEXCHAR(TCGC_OBJ* THIS, PChar CHR) {
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '\'');
  if (CHR == '\'') Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\'");
  else if (CHR == '\\') Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\\\");
  else if (' ' <= CHR && CHR <= '~') Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, CHR);
  else _CGC_OUTEXESCAPEDCHAR(THIS, CHR);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '\'');
}

void _CGC_OUTEXCSTRING(TCGC_OBJ* THIS, const PString* STR) {
  PInteger POS;
  PChar CH;
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '"');
  for (PInteger first = 1, last = LENGTH(STR); first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      CH = STR->chr[POS];
      if (CH < ' ' || CH > '~') {
        _CGC_OUTEXESCAPEDCHAR(THIS, CH);
        if (POS < LENGTH(STR) && ('0' <= STR->chr[POS + 1] && STR->chr[POS + 1] <= '9' || 'A' <= STR->chr[POS + 1] && STR->chr[POS + 1] <= 'F' || 'a' <= STR->chr[POS + 1] && STR->chr[POS + 1] <= 'f')) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\"\"");
      }
      else {
        if (CH == '"') Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\\"");
        else if (CH == '\\') Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "\\\\");
        else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, CH);
      }
    }
    break;
  }
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '"');
}

void _CGC_OUTEXSTRING(TCGC_OBJ* THIS, const PString* STR) {
  if (LENGTH(STR) == 1) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "str_of(");
    _CGC_OUTEXCHAR(THIS, STR->chr[1]);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
  else {
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 9, "str_make(", RwpInt, LENGTH(STR), RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXCSTRING(THIS, STR);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
}

void _CGC_OUTEXSETIMMEDIATE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  PInteger LOWBOUND;
  PInteger HIGHBOUND;
  PInteger LOWBOUNDBYTE;
  PInteger SETSIZE;
  TSESETIMMBOUNDSOBJ* BOUNDS;
  TSDTYPEDEF* ELEMTYPEPTR;
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
  for (PInteger first = 1, last = SETSIZE; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) SETELEMS[subrange(POS, 1, 32) - 1] = 0;
    break;
  }
  while (BOUNDS != PNil) {
    if (BOUNDS->FIRST < LOWBOUND || BOUNDS->LAST > HIGHBOUND) COMPILEERROR(CONCAT(CpLenPtr, 4, "Set ", CpString, EXDESCRIBE(EXPR), CpLenPtr, 19, " contains elements ", CpLenPtr, 27, "that are out of bounds for ", CpEnd | CpString, TYPENAME(EXPR->TYPEPTR)));
    for (PInteger first = BOUNDS->FIRST, last = BOUNDS->LAST; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
        BYTENUM = 1 + POS / 8 - LOWBOUNDBYTE;
        BITNUM = POS % 8;
        SETELEMS[subrange(BYTENUM, 1, 32) - 1] = SETELEMS[subrange(BYTENUM, 1, 32) - 1] | 1 << BITNUM;
      }
      break;
    }
    BOUNDS = BOUNDS->NEXT;
  }
  Write(&THIS->OUTPUT, 1, RwpLenPtr, 5, "(PSet", RwpInt, SETSIZE * 8, RwpLenPtr | RwpEnd, 4, ") { ");
  for (PInteger first = 1, last = SETSIZE; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      if (POS != 1) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      Write(&THIS->OUTPUT, 1, RwpInt | RwpEnd, SETELEMS[subrange(POS, 1, 32) - 1]);
    }
    break;
  }
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
}

void _CGC_OUTEXIMMEDIATE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEIMMEDIATE* tmp1;
  tmp1 = &EXPR->IMMEDIATE;
  switch (tmp1->CLS) {
    case SEICNIL:
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "PNil");
      break;
    case SEICBOOLEAN:
      if (tmp1->BOOLEANVAL) Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '1');
      else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '0');
      break;
    case SEICINTEGER:
      Write(&THIS->OUTPUT, 1, RwpInt | RwpEnd, tmp1->INTEGERVAL);
      break;
    case SEICREAL:
      Write(&THIS->OUTPUT, 1, RwpReal | RwpEnd, tmp1->REALVAL);
      break;
    case SEICCHAR:
      _CGC_OUTEXCHAR(THIS, tmp1->CHARVAL);
      break;
    case SEICSTRING:
      _CGC_OUTEXSTRING(THIS, &tmp1->STRINGVAL);
      break;
    case SEICENUM:
      Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &tmp1->ENUMPTR->VALUES[subrange(tmp1->ENUMORDINAL, 0, 127)]);
      break;
    case SEICSET:
      _CGC_OUTEXSETIMMEDIATE(THIS, EXPR);
      break;
    default:
      break;
  }
}

void _CGC_OUTEXARRAYVALUE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEARRAYELEMOBJ* ELEM;
  ELEM = EXPR->ARRAYELEM;
  if (ELEM == PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "{}");
  else {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "{ ");
    while (ELEM != PNil) {
      _CGC_OUTEXPRESSION(THIS, ELEM->VALUE);
      ELEM = ELEM->NEXT;
      if (ELEM != PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    }
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
  }
}

void _CGC_OUTEXRECORDVALUE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSERECORDFIELDOBJ* FIELD;
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "{ ");
  FIELD = EXPR->RECORDFIELD;
  if (FIELD == PNil) Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '}');
  else {
    while (FIELD != PNil) {
      Write(&THIS->OUTPUT, 1, RwpChar, '.', RwpStringPtr, &EXPR->TYPEPTR->RECPTR->FIELDS[subrange(FIELD->ORDINAL, 1, 64) - 1].NAME, RwpLenPtr | RwpEnd, 3, " = ");
      _CGC_OUTEXPRESSION(THIS, FIELD->VALUE);
      FIELD = FIELD->NEXT;
      if (FIELD != PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    }
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
  }
}

void _CGC_OUTEXSETVALUE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSDTYPEDEF* ELEMENTTYPEPTR;
  TSESETEXPRBOUNDSOBJ* BOUNDS;
  TSEXPRESSIONOBJ* FIRST;
  TSEXPRESSIONOBJ* LAST;
  PInteger LOWBOUNDBYTE;
  ELEMENTTYPEPTR = EXPR->TYPEPTR->ELEMENTTYPEPTR;
  LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMENTTYPEPTR) / 8;
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "({ ");
  _CGC_OUTSETTYPENAME(THIS, EXPR->TYPEPTR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, " dst = ");
  _CGC_OUTEXSETIMMEDIATE(THIS, EXPR->SETBASE);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "; ");
  BOUNDS = EXPR->SETBOUNDS;
  while (BOUNDS != PNil) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "set_set(");
    FIRST = EXOPORD(EXCOERCE(EXCOPY(BOUNDS->FIRST), ELEMENTTYPEPTR));
    if (BOUNDS->LAST == PNil) LAST = EXCOPY(FIRST);
    else LAST = EXOPORD(EXCOERCE(EXCOPY(BOUNDS->LAST), ELEMENTTYPEPTR));
    _CGC_OUTEXPRESSION(THIS, FIRST);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXPRESSION(THIS, LAST);
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 2, ", ", RwpInt, LOWBOUNDBYTE, RwpLenPtr | RwpEnd, 13, ", dst.bits); ");
    EXDISPOSE(&FIRST);
    EXDISPOSE(&LAST);
    BOUNDS = BOUNDS->NEXT;
  }
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "dst; })");
}

void _CGC_OUTEXWITHTMPVAR(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "({ ");
  while (EXPR->CLS == SECWITHTMPVAR) {
    Write(&THIS->OUTPUT, 1, RwpStringPtr, &EXPR->TMPVARPTR->VARDEF.NAME, RwpLenPtr | RwpEnd, 3, " = ");
    _CGC_OUTEXPRESSION(THIS, EXPR->TMPVARVALUE);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "; ");
    EXPR = EXPR->TMPVARCHILD;
  }
  _CGC_OUTEXPRESSION(THIS, EXPR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "; })");
}

void _CGC_OUTEXSUBRANGE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  if (!EXPR->CHECKBOUNDS) _CGC_OUTEXPRESSION(THIS, EXPR->SUBRANGEPARENT);
  else {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "subrange(");
    _CGC_OUTEXPRESSION(THIS, EXPR->SUBRANGEPARENT);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTBOUNDS(THIS, EXPR->TYPEPTR);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
}

void _CGC_OUTEXVARIABLE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  if (EXPR->VARPTR->ISREFERENCE) Write(&THIS->OUTPUT, 1, RwpChar, '*', RwpStringPtr | RwpEnd, &EXPR->VARPTR->NAME);
  else Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->VARPTR->NAME);
}

void _CGC_OUTEXFIELDACCESS(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* tmp1;
  tmp1 = EXPR->RECEXPR;
  if (tmp1->CLS == SECPOINTER) {
    _CGC_OUTEXPRESSIONPARENS(THIS, tmp1->POINTEREXPR, EXPR);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "->");
  }
  else if (tmp1->CLS == SECVARIABLE && tmp1->VARPTR->ISREFERENCE) Write(&THIS->OUTPUT, 1, RwpStringPtr, &tmp1->VARPTR->NAME, RwpLenPtr | RwpEnd, 2, "->");
  else {
    _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->RECEXPR, EXPR);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '.');
  }
  Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[subrange(EXPR->RECFIELDNUM, 1, 64) - 1].NAME);
}

void _CGC_OUTEXARRAYINDEX(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* INDEX, TSDTYPEDEF* TYPEPTR) {
  PInteger LOWBOUND;
  TSEXPRESSIONOBJ* SIZE;
  LOWBOUND = GETTYPELOWBOUND(TYPEPTR->ARRAYDEF.INDEXTYPEPTR);
  SIZE = EXOPSUB(EXOPORD(EXCOPY(INDEX)), EXINTEGERCONSTANT(LOWBOUND));
  _CGC_OUTEXPRESSION(THIS, SIZE);
  EXDISPOSE(&SIZE);
}

void _CGC_OUTEXADDRESS(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  if (EXPR->CLS == SECTOGENERICFILE) _CGC_OUTEXADDRESS(THIS, EXPR->TOGENERICFILEPARENT);
  else if (EXPR->CLS == SECTOREAL) _CGC_OUTEXADDRESS(THIS, EXPR->TOREALPARENT);
  else if (EXPR->CLS == SECTOSTRING) _CGC_OUTEXADDRESS(THIS, EXPR->TOSTRPARENT);
  else if (EXPR->CLS == SECPOINTER) _CGC_OUTEXPRESSION(THIS, EXPR->POINTEREXPR);
  else if (EXPR->CLS == SECTOUNTYPEDPTR) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "(void**)&");
    _CGC_OUTEXPRESSIONPARENSPREC(THIS, EXPR, 1);
  }
  else if (EXPR->CLS == SECVARIABLE && EXPR->VARPTR->ISREFERENCE) Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->VARPTR->NAME);
  else {
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '&');
    _CGC_OUTEXPRESSIONPARENSPREC(THIS, EXPR, 1);
  }
}

void _CGC_OUTEXSTRINGCHAR(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* tmp1;
  tmp1 = EXPR->STRINGEXPR;
  if (tmp1->CLS == SECPOINTER) {
    _CGC_OUTEXPRESSIONPARENS(THIS, tmp1->POINTEREXPR, EXPR);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "->chr[");
  }
  else if (tmp1->CLS == SECVARIABLE && tmp1->VARPTR->ISREFERENCE) Write(&THIS->OUTPUT, 1, RwpStringPtr, &tmp1->VARPTR->NAME, RwpLenPtr | RwpEnd, 6, "->chr[");
  else {
    _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->STRINGEXPR, EXPR);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, ".chr[");
  }
  _CGC_OUTEXPRESSION(THIS, EXPR->STRINGINDEX);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ']');
}

void _CGC_OUTEXFUNCTIONCALLARGS(TCGC_OBJ* THIS, const TSDSUBROUTINEARGS* ARGDEFS, const TSEFUNCTIONARGS* ARGVALUES) {
  PInteger POS;
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '(');
  for (PInteger first = 1, last = ARGVALUES->SIZE; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      if (POS != 1) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      if (ARGDEFS->DEFS[subrange(POS, 1, 16) - 1].ISREFERENCE) {
        ENSUREADDRESSABLEEXPR(ARGVALUES->VALUES[subrange(POS, 1, 16) - 1]);
        _CGC_OUTEXADDRESS(THIS, ARGVALUES->VALUES[subrange(POS, 1, 16) - 1]);
      }
      else _CGC_OUTEXPRESSION(THIS, ARGVALUES->VALUES[subrange(POS, 1, 16) - 1]);
    }
    break;
  }
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _CGC_OUTEXFUNCTIONCALL(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->FNEXPR, EXPR);
  if (EXPR->FNEXPR->CLS == SECFNREF) _CGC_OUTEXFUNCTIONCALLARGS(THIS, &EXPR->FNEXPR->FNPTR->ARGS, &EXPR->CALLARGS);
  else _CGC_OUTEXFUNCTIONCALLARGS(THIS, &EXPR->FNEXPR->TYPEPTR->FNDEFPTR->ARGS, &EXPR->CALLARGS);
}

void _CGC_OUTEXSIZEOF(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "sizeof(");
  _CGC_OUTTYPEREFERENCE(THIS, EXPR->SIZEOFTYPEPTR);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

PChar _CGC_SHORTTYPENAME(TSDTYPEDEF* TYPEPTR) {
  PChar RESULT;
  if (ISBOOLEANTYPE(TYPEPTR)) RESULT = 'b';
  else if (ISINTEGERTYPE(TYPEPTR)) RESULT = 'i';
  else if (ISREALTYPE(TYPEPTR)) RESULT = 'r';
  else if (ISCHARTYPE(TYPEPTR)) RESULT = 'c';
  else if (ISSTRINGTYPE(TYPEPTR)) RESULT = 's';
  else COMPILEERROR(CONCAT(CpLenPtr, 5, "Type ", CpString, TYPENAME(TYPEPTR), CpLenPtr, 26, " is not representable for ", CpEnd | CpLenPtr, 24, "READ, WRITE, STR, or VAL"));
  return RESULT;
}

void _CGC_OUTEXCONVERTTOSTR(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* SRC;
  TSEXPRESSIONOBJ* DST;
  TSEXPRESSIONOBJ* WIDTH;
  TSEXPRESSIONOBJ* PREC;
  SRC = EXPR->TOSTRSRC.ARG;
  DST = EXPR->TOSTRDEST;
  WIDTH = EXPR->TOSTRSRC.WIDTH;
  PREC = EXPR->TOSTRSRC.PREC;
  if (ISENUMTYPE(SRC->TYPEPTR)) {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "STR_e(");
    _CGC_OUTEXPRESSION(THIS, SRC);
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 12, ", enumvalues", RwpInt, SRC->TYPEPTR->ENUMPTR->ID, RwpLenPtr | RwpEnd, 2, ", ");
    if (WIDTH != PNil) _CGC_OUTEXPRESSION(THIS, WIDTH);
    else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '0');
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, DST);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
  else if (ISREALTYPE(SRC->TYPEPTR)) {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "STR_r(");
    _CGC_OUTEXPRESSION(THIS, SRC);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    if (WIDTH != PNil) _CGC_OUTEXPRESSION(THIS, WIDTH);
    else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '0');
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    if (PREC != PNil) _CGC_OUTEXPRESSION(THIS, PREC);
    else Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "-1");
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, DST);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
  else {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 4, "STR_", RwpChar, _CGC_SHORTTYPENAME(SRC->TYPEPTR), RwpChar | RwpEnd, '(');
    _CGC_OUTEXPRESSION(THIS, SRC);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    if (WIDTH != PNil) _CGC_OUTEXPRESSION(THIS, WIDTH);
    else Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '0');
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, DST);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
}

void _CGC_OUTEXCONVERTTOVAL(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* SRC;
  TSEXPRESSIONOBJ* DST;
  TSEXPRESSIONOBJ* CODE;
  TSEXPRESSIONOBJ* TMPEXPR;
  SRC = EXPR->TOVALSRC;
  DST = EXPR->TOVALDEST;
  CODE = EXPR->TOVALCODE;
  if (ISENUMTYPE(DST->TYPEPTR)) {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "VAL_e(");
    _CGC_OUTEXADDRESS(THIS, SRC);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, DST);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    TMPEXPR = EXINTEGERCONSTANT(DST->TYPEPTR->ENUMPTR->SIZE);
    _CGC_OUTEXPRESSION(THIS, TMPEXPR);
    EXDISPOSE(&TMPEXPR);
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 12, ", enumvalues", RwpInt, DST->TYPEPTR->ENUMPTR->ID, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, CODE);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
  else {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 4, "VAL_", RwpChar, _CGC_SHORTTYPENAME(DST->TYPEPTR), RwpChar | RwpEnd, '(');
    _CGC_OUTEXADDRESS(THIS, SRC);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, DST);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXADDRESS(THIS, CODE);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
  }
}

void _CGC_OUTEXREAD(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* SRC;
  TSEREADARGVALUE* READARG;
  PBoolean LINEFEED;
  TSDTYPEDEF* TYPEPTR;
  PString tmp1;
  SRC = EXPR->READFILE;
  LINEFEED = EXPR->READLN;
  READARG = EXPR->READARGS;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "Read(");
  _CGC_OUTEXADDRESS(THIS, SRC);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
  _CGC_OUTEXPRESSION(THIS, EXBOOLEANCONSTANT(EXPR->CHECKIORESULTAFTERREAD));
  if (READARG == PNil) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpEnd");
    if (LINEFEED) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
  }
  while (READARG != PNil) {
    if (ISTEXTTYPE(SRC->TYPEPTR)) {
      TYPEPTR = GETFUNDAMENTALTYPE(READARG->DEST->TYPEPTR);
      switch (TYPEPTR->CLS) {
        case SDTCINTEGER:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpInt");
          break;
        case SDTCREAL:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpReal");
          break;
        case SDTCCHAR:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpChar");
          break;
        case SDTCSTRING:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 11, ", RwpString");
          break;
        default:
          tmp1 = str_make(36, "Expression has invalid type for READ");
          ERRORFOREXPR(&tmp1, READARG->DEST);
          break;
      }
      if (READARG->NEXT == PNil) {
        if (LINEFEED) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
        Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " | RwpEnd");
      }
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTEXADDRESS(THIS, READARG->DEST);
    }
    else {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 12, ", RwpDataPtr");
      if (READARG->NEXT == PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " | RwpEnd");
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTEXADDRESS(THIS, READARG->DEST);
    }
    READARG = READARG->NEXT;
  }
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _CGC_OUTEXWRITE(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* DST;
  TSEWRITEARGVALUE* WRITEARG;
  PBoolean LINEFEED;
  TSDTYPEDEF* TYPEPTR;
  PString tmp1;
  DST = EXPR->WRITEFILE;
  LINEFEED = EXPR->WRITELN;
  WRITEARG = EXPR->WRITEARGS;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "Write(");
  _CGC_OUTEXADDRESS(THIS, DST);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
  _CGC_OUTEXPRESSION(THIS, EXBOOLEANCONSTANT(EXPR->CHECKIORESULTAFTERWRITE));
  if (WRITEARG == PNil) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpEnd");
    if (LINEFEED) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
  }
  while (WRITEARG != PNil) {
    if (ISTEXTTYPE(DST->TYPEPTR)) {
      TYPEPTR = GETFUNDAMENTALTYPE(WRITEARG->VALUE.ARG->TYPEPTR);
      switch (TYPEPTR->CLS) {
        case SDTCBOOLEAN:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpBool");
          break;
        case SDTCINTEGER:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, ", RwpInt");
          break;
        case SDTCREAL:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpReal");
          break;
        case SDTCCHAR:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpChar");
          break;
        case SDTCENUM:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, ", RwpEnum");
          break;
        case SDTCSTRING:
          if (EXISIMMEDIATE(WRITEARG->VALUE.ARG)) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 11, ", RwpLenPtr");
          else if (WRITEARG->VALUE.ARG->ISADDRESSABLE) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 14, ", RwpStringPtr");
          else Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 11, ", RwpString");
          break;
        default:
          tmp1 = str_make(37, "Expression has invalid type for WRITE");
          ERRORFOREXPR(&tmp1, WRITEARG->VALUE.ARG);
          break;
      }
      if (WRITEARG->VALUE.WIDTH != PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 11, " | RwpWidth");
      if (ISREALTYPE(TYPEPTR) && WRITEARG->VALUE.PREC != PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 10, " | RwpPrec");
      if (WRITEARG->NEXT == PNil) {
        if (LINEFEED) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " | RwpLn");
        Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " | RwpEnd");
      }
      if (WRITEARG->VALUE.WIDTH != PNil) {
        Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
        _CGC_OUTEXPRESSION(THIS, WRITEARG->VALUE.WIDTH);
      }
      if (WRITEARG->VALUE.PREC != PNil) {
        Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
        _CGC_OUTEXPRESSION(THIS, WRITEARG->VALUE.PREC);
      }
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      if (ISSTRINGTYPE(WRITEARG->VALUE.ARG->TYPEPTR) && EXISIMMEDIATE(WRITEARG->VALUE.ARG)) {
        Write(&THIS->OUTPUT, 1, RwpInt | RwpEnd, LENGTH(&WRITEARG->VALUE.ARG->IMMEDIATE.STRINGVAL));
        Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
        _CGC_OUTEXCSTRING(THIS, &WRITEARG->VALUE.ARG->IMMEDIATE.STRINGVAL);
      }
      else if (ISSTRINGTYPE(WRITEARG->VALUE.ARG->TYPEPTR) && WRITEARG->VALUE.ARG->ISADDRESSABLE) _CGC_OUTEXADDRESS(THIS, WRITEARG->VALUE.ARG);
      else _CGC_OUTEXPRESSION(THIS, WRITEARG->VALUE.ARG);
      if (ISENUMTYPE(TYPEPTR)) Write(&THIS->OUTPUT, 1, RwpLenPtr, 12, ", enumvalues", RwpInt | RwpEnd, TYPEPTR->ENUMPTR->ID);
    }
    else {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 12, ", RwpDataPtr");
      if (WRITEARG->NEXT == PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, " | RwpEnd");
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTEXADDRESS(THIS, WRITEARG->VALUE.ARG);
    }
    WRITEARG = WRITEARG->NEXT;
  }
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _CGC_OUTORD(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  ENSUREORDINALEXPR(EXPR->UNARY.PARENT);
  if (ISCHARTYPE(EXPR->UNARY.PARENT->TYPEPTR)) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "(int)");
    _CGC_OUTEXPRESSIONPARENSPREC(THIS, EXPR->UNARY.PARENT, 2);
  }
  else _CGC_OUTEXPRESSION(THIS, EXPR->UNARY.PARENT);
}

void _CGC_OUTPRED(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* TMPEXPR;
  ENSUREORDINALEXPR(EXPR->UNARY.PARENT);
  if (ISBOUNDEDTYPE(EXPR->UNARY.PARENT->TYPEPTR)) {
    if (EXPR->CHECKBOUNDS) {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "pred(");
      _CGC_OUTEXPRESSION(THIS, EXPR->UNARY.PARENT);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTBOUNDS(THIS, EXPR->UNARY.PARENT->TYPEPTR);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
    }
    else {
      TMPEXPR = EXOPSUB(EXOPORD(EXCOPY(EXPR->UNARY.PARENT)), EXINTEGERCONSTANT(1));
      _CGC_OUTEXPRESSION(THIS, TMPEXPR);
      EXDISPOSE(&TMPEXPR);
    }
  }
  else {
    TMPEXPR = EXOPSUB(EXCOPY(EXPR->UNARY.PARENT), EXINTEGERCONSTANT(1));
    _CGC_OUTEXPRESSION(THIS, TMPEXPR);
    EXDISPOSE(&TMPEXPR);
  }
}

void _CGC_OUTSUCC(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* TMPEXPR;
  ENSUREORDINALEXPR(EXPR->UNARY.PARENT);
  if (ISBOUNDEDTYPE(EXPR->UNARY.PARENT->TYPEPTR)) {
    if (EXPR->CHECKBOUNDS) {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "succ(");
      _CGC_OUTEXPRESSION(THIS, EXPR->UNARY.PARENT);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTBOUNDS(THIS, EXPR->UNARY.PARENT->TYPEPTR);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
    }
    else {
      TMPEXPR = EXOPADD(EXOPORD(EXCOPY(EXPR->UNARY.PARENT)), EXINTEGERCONSTANT(1));
      _CGC_OUTEXPRESSION(THIS, TMPEXPR);
      EXDISPOSE(&TMPEXPR);
    }
  }
  else {
    TMPEXPR = EXOPADD(EXCOPY(EXPR->UNARY.PARENT), EXINTEGERCONSTANT(1));
    _CGC_OUTEXPRESSION(THIS, TMPEXPR);
    EXDISPOSE(&TMPEXPR);
  }
}

void _CGC_OUTEXUNARYOP(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  if (EXPR->UNARY.OP == SEOORD) _CGC_OUTORD(THIS, EXPR);
  else if (EXPR->UNARY.OP == SEOPRED) _CGC_OUTPRED(THIS, EXPR);
  else if (EXPR->UNARY.OP == SEOSUCC) _CGC_OUTSUCC(THIS, EXPR);
  else if (EXPR->UNARY.OP == SEONEG) {
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '-');
    _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->UNARY.PARENT, EXPR);
  }
  else if (EXPR->UNARY.OP == SEONOT && ISBOOLEANTYPE(EXPR->TYPEPTR)) {
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '!');
    _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->UNARY.PARENT, EXPR);
  }
  else if (EXPR->UNARY.OP == SEONOT && ISINTEGERTYPE(EXPR->TYPEPTR)) {
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '~');
    _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->UNARY.PARENT, EXPR);
  }
}

void _CGC_OUTEXCMPCONCATARG(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  if (EXISIMMEDIATE(EXPR) && ISSTRINGTYPE(EXPR->TYPEPTR)) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 10, "CpLenPtr, ", RwpInt, LENGTH(&EXPR->IMMEDIATE.STRINGVAL), RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXCSTRING(THIS, &EXPR->IMMEDIATE.STRINGVAL);
  }
  else if (ISCHARTYPE(EXPR->TYPEPTR)) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "CpChar, ");
    _CGC_OUTEXPRESSION(THIS, EXPR);
  }
  else if (EXPR->ISADDRESSABLE) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 13, "CpStringPtr, ");
    _CGC_OUTEXADDRESS(THIS, EXPR);
  }
  else {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "CpString, ");
    _CGC_OUTEXPRESSION(THIS, EXPR);
  }
}

void _CGC_OUTEXCONCATARGS(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR, PBoolean LAST) {
  if (!ISSTRINGYTYPE(EXPR->TYPEPTR)) INTERNALERROR(CONCAT(CpLenPtr, 28, "Expected a stringy type for ", CpEnd | CpString, EXDESCRIBE(EXPR)));
  else if (EXPR->CLS != SECBINARYOP || EXPR->BINARY.OP != SEOADD) {
    if (LAST) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "CpEnd | ");
    _CGC_OUTEXCMPCONCATARG(THIS, EXPR);
    if (!LAST) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
  }
  else {
    _CGC_OUTEXCONCATARGS(THIS, EXPR->BINARY.LEFT, 0);
    _CGC_OUTEXCONCATARGS(THIS, EXPR->BINARY.RIGHT, LAST);
  }
}

void _CGC_OUTEXSETOPERATION(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT, TSEOPERATOR OP) {
  TSDTYPEDEF* ELEMTYPEPTR;
  PInteger LOWBOUND;
  PInteger HIGHBOUND;
  PInteger LOWBOUNDBYTE;
  PInteger SETSIZE;
  ELEMTYPEPTR = RIGHT->TYPEPTR->ELEMENTTYPEPTR;
  if (OP == SEOLTEQ) _CGC_OUTEXSETOPERATION(THIS, RIGHT, LEFT, SEOGTEQ);
  else if (OP == SEONE) {
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '!');
    _CGC_OUTEXSETOPERATION(THIS, LEFT, RIGHT, SEOEQ);
  }
  else if (OP == SEOIN) {
    LOWBOUNDBYTE = GETTYPELOWBOUND(ELEMTYPEPTR) / 8;
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "set_in(");
    _CGC_OUTEXPRESSION(THIS, LEFT);
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 2, ", ", RwpInt, LOWBOUNDBYTE, RwpLenPtr | RwpEnd, 2, ", ");
    _CGC_OUTEXPRESSIONPARENSPREC(THIS, RIGHT, 1);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, ".bits)");
  }
  else {
    LOWBOUND = GETTYPELOWBOUND(ELEMTYPEPTR);
    HIGHBOUND = GETTYPEHIGHBOUND(ELEMTYPEPTR);
    SETSIZE = HIGHBOUND / 8 - LOWBOUND / 8 + 1;
    if (OP == SEOEQ) {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 11, "set_equals(");
      _CGC_OUTEXPRESSIONPARENSPREC(THIS, LEFT, 1);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, ".bits, ");
      _CGC_OUTEXPRESSIONPARENSPREC(THIS, RIGHT, 1);
      Write(&THIS->OUTPUT, 1, RwpLenPtr, 7, ".bits, ", RwpInt, SETSIZE, RwpChar | RwpEnd, ')');
    }
    else if (OP == SEOGTEQ) {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 15, "set_issuperset(");
      _CGC_OUTEXPRESSIONPARENSPREC(THIS, LEFT, 1);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, ".bits, ");
      _CGC_OUTEXPRESSIONPARENSPREC(THIS, RIGHT, 1);
      Write(&THIS->OUTPUT, 1, RwpLenPtr, 7, ".bits, ", RwpInt, SETSIZE, RwpChar | RwpEnd, ')');
    }
    else {
      Write(&THIS->OUTPUT, 1, RwpLenPtr, 7, "({ PSet", RwpInt, SETSIZE * 8, RwpLenPtr | RwpEnd, 6, " dst; ");
      switch (OP) {
        case SEOADD:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "set_union(");
          break;
        case SEOSUB:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 15, "set_difference(");
          break;
        case SEOMUL:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 17, "set_intersection(");
          break;
        default:
          INTERNALERROR(CONCAT(CpLenPtr, 44, "Materialized set operation not implemented: ", CpEnd | CpString, EXDESCRIBEOPERATOR(OP)));
          break;
      }
      _CGC_OUTEXPRESSIONPARENSPREC(THIS, LEFT, 1);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, ".bits, ");
      _CGC_OUTEXPRESSIONPARENSPREC(THIS, RIGHT, 1);
      Write(&THIS->OUTPUT, 1, RwpLenPtr, 17, ".bits, dst.bits, ", RwpInt, SETSIZE, RwpLenPtr | RwpEnd, 10, "); dst; })");
    }
  }
}

PBoolean _CGC_ISARITHMETICOP(TSEOPERATOR OP) {
  PBoolean RESULT;
  RESULT = SEOADD <= OP && OP <= SEOMOD;
  return RESULT;
}

PString _CGC_GETARITHMETICOP(TSEOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case SEOADD:
      RESULT = str_of('+');
      break;
    case SEOSUB:
      RESULT = str_of('-');
      break;
    case SEOMUL:
      RESULT = str_of('*');
      break;
    case SEODIVREAL:
      RESULT = str_of('/');
      break;
    case SEODIVINT:
      RESULT = str_of('/');
      break;
    case SEOMOD:
      RESULT = str_of('%');
      break;
    default:
      break;
  }
  return RESULT;
}

PBoolean _CGC_ISLOGICALORBITWISEOP(TSEOPERATOR OP) {
  PBoolean RESULT;
  RESULT = SEOAND <= OP && OP <= SEOXOR;
  return RESULT;
}

PBoolean _CGC_ISBITWISEOP(TSEOPERATOR OP) {
  PBoolean RESULT;
  RESULT = SEOSHL <= OP && OP <= SEOSHR;
  return RESULT;
}

PString _CGC_GETLOGICALOP(TSEOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case SEOAND:
      RESULT = str_make(2, "&&");
      break;
    case SEOOR:
      RESULT = str_make(2, "||");
      break;
    case SEOXOR:
      RESULT = str_make(2, "!=");
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 31, "Unimplemented logical operator ", CpEnd | CpString, EXDESCRIBEOPERATOR(OP)));
      break;
  }
  return RESULT;
}

PString _CGC_GETBITWISEOP(TSEOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case SEOAND:
      RESULT = str_of('&');
      break;
    case SEOOR:
      RESULT = str_of('|');
      break;
    case SEOXOR:
      RESULT = str_of('^');
      break;
    case SEOSHL:
      RESULT = str_make(2, "<<");
      break;
    case SEOSHR:
      RESULT = str_make(2, ">>");
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 31, "Unimplemented bitwise operator ", CpEnd | CpString, EXDESCRIBEOPERATOR(OP)));
      break;
  }
  return RESULT;
}

PBoolean _CGC_ISRELATIONALOP(TSEOPERATOR OP) {
  PBoolean RESULT;
  RESULT = SEOEQ <= OP && OP <= SEOGTEQ;
  return RESULT;
}

PString _CGC_GETRELATIONALOP(TSEOPERATOR OP) {
  PString RESULT;
  switch (OP) {
    case SEOEQ:
      RESULT = str_make(2, "==");
      break;
    case SEONE:
      RESULT = str_make(2, "!=");
      break;
    case SEOLT:
      RESULT = str_of('<');
      break;
    case SEOGT:
      RESULT = str_of('>');
      break;
    case SEOLTEQ:
      RESULT = str_make(2, "<=");
      break;
    case SEOGTEQ:
      RESULT = str_make(2, ">=");
      break;
    default:
      break;
  }
  return RESULT;
}

void _CGC_OUTEXBINARYOP(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  TSEBINARYOP* tmp1;
  tmp1 = &EXPR->BINARY;
  if (ISBOOLEANTYPE(tmp1->LEFT->TYPEPTR) && ISBOOLEANTYPE(tmp1->RIGHT->TYPEPTR)) {
    _CGC_OUTEXPRESSIONPARENS(THIS, tmp1->LEFT, EXPR);
    if (_CGC_ISLOGICALORBITWISEOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETLOGICALOP(tmp1->OP), RwpChar | RwpEnd, ' ');
    else if (_CGC_ISRELATIONALOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETRELATIONALOP(tmp1->OP), RwpChar | RwpEnd, ' ');
    else ERRORINVALIDOPERATOR(EXPR, tmp1->OP);
    _CGC_OUTEXPRESSIONPARENSEXTRA(THIS, tmp1->RIGHT, EXPR);
  }
  else if (ISNUMERICTYPE(tmp1->LEFT->TYPEPTR) && ISNUMERICTYPE(tmp1->RIGHT->TYPEPTR)) {
    _CGC_OUTEXPRESSIONPARENS(THIS, tmp1->LEFT, EXPR);
    if (_CGC_ISARITHMETICOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETARITHMETICOP(tmp1->OP), RwpChar | RwpEnd, ' ');
    else if (_CGC_ISLOGICALORBITWISEOP(tmp1->OP) || _CGC_ISBITWISEOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETBITWISEOP(tmp1->OP), RwpChar | RwpEnd, ' ');
    else if (_CGC_ISRELATIONALOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETRELATIONALOP(tmp1->OP), RwpChar | RwpEnd, ' ');
    else ERRORINVALIDOPERATOR(EXPR, tmp1->OP);
    _CGC_OUTEXPRESSIONPARENSEXTRA(THIS, tmp1->RIGHT, EXPR);
  }
  else if (ISSTRINGYTYPE(tmp1->LEFT->TYPEPTR) && ISSTRINGYTYPE(tmp1->RIGHT->TYPEPTR)) {
    if (tmp1->OP == SEOADD) {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "CONCAT(");
      _CGC_OUTEXCONCATARGS(THIS, EXPR, 1);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
    }
    else if (ISCHARTYPE(tmp1->LEFT->TYPEPTR) && ISCHARTYPE(tmp1->RIGHT->TYPEPTR)) {
      _CGC_OUTEXPRESSIONPARENS(THIS, tmp1->LEFT, EXPR);
      if (_CGC_ISRELATIONALOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETRELATIONALOP(tmp1->OP), RwpChar | RwpEnd, ' ');
      else ERRORINVALIDOPERATOR(EXPR, tmp1->OP);
      _CGC_OUTEXPRESSIONPARENSEXTRA(THIS, tmp1->RIGHT, EXPR);
    }
    else {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "cmp_str(");
      switch (tmp1->OP) {
        case SEOEQ:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "CoEq, ");
          break;
        case SEONE:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "CoNotEq, ");
          break;
        case SEOLT:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 10, "CoBefore, ");
          break;
        case SEOGT:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 9, "CoAfter, ");
          break;
        case SEOLTEQ:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 14, "CoBeforeOrEq, ");
          break;
        case SEOGTEQ:
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 13, "CoAfterOrEq, ");
          break;
        default:
          ERRORINVALIDOPERATOR(EXPR, tmp1->OP);
          break;
      }
      _CGC_OUTEXCMPCONCATARG(THIS, tmp1->LEFT);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTEXCMPCONCATARG(THIS, tmp1->RIGHT);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
    }
  }
  else if (ISSETTYPE(tmp1->RIGHT->TYPEPTR)) _CGC_OUTEXSETOPERATION(THIS, tmp1->LEFT, tmp1->RIGHT, tmp1->OP);
  else {
    _CGC_OUTEXPRESSIONPARENS(THIS, tmp1->LEFT, EXPR);
    if (_CGC_ISRELATIONALOP(tmp1->OP)) Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpString, _CGC_GETRELATIONALOP(tmp1->OP), RwpChar | RwpEnd, ' ');
    else ERRORINVALIDOPERATOR(EXPR, tmp1->OP);
    _CGC_OUTEXPRESSIONPARENSEXTRA(THIS, tmp1->RIGHT, EXPR);
  }
}

void _CGC_OUTNAMEANDENUM(TCGC_OBJ* THIS, const PString* NAME, TSDTENUMDEF* ENUMPTR) {
  PInteger POS;
  if (ENUMPTR->HASBEENDEFINED) Write(&THIS->OUTPUT, 1, RwpLenPtr, 9, "enum enum", RwpInt | RwpEnd, ENUMPTR->ID);
  else {
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 37, "enum __attribute__((__packed__)) enum", RwpInt, ENUMPTR->ID, RwpLenPtr | RwpEnd, 3, " { ");
    for (PInteger first = 0, last = ENUMPTR->SIZE - 1; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
        if (POS > 0) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
        Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &ENUMPTR->VALUES[subrange(POS, 0, 127)]);
      }
      break;
    }
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, " }");
    ENUMPTR->HASBEENDEFINED = 1;
  }
  Write(&THIS->OUTPUT, 1, RwpChar, ' ', RwpStringPtr | RwpEnd, NAME);
}

void _CGC_OUTNAMEANDRECORD(TCGC_OBJ* THIS, const PString* NAME, TSDTRECORDDEF* RECPTR) {
  PInteger POS;
  PInteger NUMVARIANT;
  NUMVARIANT = 0;
  if (RECPTR->HASBEENDEFINED) Write(&THIS->OUTPUT, 1, RwpLenPtr, 13, "struct record", RwpInt | RwpEnd, RECPTR->ID);
  else {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "struct ");
    if (RECPTR->ISPACKED) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 28, "__attribute__((__packed__)) ");
    Write(&THIS->OUTPUT, 1, RwpLenPtr, 6, "record", RwpInt, RECPTR->ID, RwpChar | RwpEnd, ' ');
    _CGC_OUTBEGIN(THIS);
    for (PInteger first = 1, last = RECPTR->SIZE; first <= last; /*breaks*/) {
      PBoolean done = 0;
      for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
        if (RECPTR->NUMVARIANTS > NUMVARIANT && RECPTR->VARIANTBOUNDS[subrange(NUMVARIANT + 1, 1, 64) - 1] == POS) {
          NUMVARIANT = NUMVARIANT + 1;
          if (NUMVARIANT == 1) {
            _CGC_OUTINDENT(THIS);
            Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "union ");
            _CGC_OUTBEGIN(THIS);
          }
          else {
            _CGC_OUTENDSAMELINE(THIS);
            Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
            _CGC_OUTNEWLINE(THIS);
          }
          _CGC_OUTINDENT(THIS);
          Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "struct ");
          _CGC_OUTBEGIN(THIS);
        }
        _CGC_OUTINDENT(THIS);
        _CGC_OUTNAMEANDTYPE(THIS, &RECPTR->FIELDS[subrange(POS, 1, 64) - 1].NAME, RECPTR->FIELDS[subrange(POS, 1, 64) - 1].TYPEPTR);
        Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
        _CGC_OUTNEWLINE(THIS);
      }
      break;
    }
    if (NUMVARIANT > 0) {
      _CGC_OUTENDSAMELINE(THIS);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
      _CGC_OUTNEWLINE(THIS);
      _CGC_OUTENDSAMELINE(THIS);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
      _CGC_OUTNEWLINE(THIS);
    }
    _CGC_OUTENDSAMELINE(THIS);
    RECPTR->HASBEENDEFINED = 1;
  }
  if (NAME->chr[1] != '*') Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ' ');
  Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, NAME);
}

void _CGC_OUTNAMEANDARRAY(TCGC_OBJ* THIS, const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* THETYPE;
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
  _CGC_OUTNAMEANDTYPE(THIS, NAME, THETYPE);
  THETYPE = TYPEPTR;
  while (ISARRAYTYPE(THETYPE)) {
    Write(&THIS->OUTPUT, 1, RwpChar, '[', RwpInt, GETBOUNDEDTYPESIZE(THETYPE->ARRAYDEF.INDEXTYPEPTR), RwpChar | RwpEnd, ']');
    THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
  }
}

void _CGC_OUTNAMEANDFUNCTION(TCGC_OBJ* THIS, const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  PInteger POS;
  PString tmp1;
  tmp1 = CONCAT(CpLenPtr, 2, "(*", CpStringPtr, NAME, CpEnd | CpChar, ')');
  _CGC_OUTNAMEANDTYPE(THIS, &tmp1, TYPEPTR->FNDEFPTR->RETURNTYPEPTR);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '(');
  for (PInteger first = 1, last = TYPEPTR->FNDEFPTR->ARGS.COUNT; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      if (POS != 1) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTARGUMENTDECLARATION(THIS, &TYPEPTR->FNDEFPTR->ARGS.DEFS[subrange(POS, 1, 16) - 1]);
    }
    break;
  }
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _CGC_OUTTYPEDEFINITION(TCGC_OBJ* THIS, TSDTYPEDEF* TYPEPTR) {
  PString NAME;
  _CGC_OUTBLANKLINE(THIS, TOTTYPE);
  _CGC_OUTINDENT(THIS);
  NAME = TYPEPTR->NAME;
  if (TYPEPTR->ALIASFOR == PNil) INTERNALERROR(CONCAT(CpLenPtr, 5, "Type ", CpStringPtr, &NAME, CpEnd | CpLenPtr, 16, " is not an alias"));
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "typedef ");
  _CGC_OUTNAMEANDTYPE(THIS, &NAME, TYPEPTR->ALIASFOR);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTENUMVALUES(TCGC_OBJ* THIS, TSDTENUMDEF* ENUMPTR) {
  PInteger POSINENUM;
  _CGC_OUTBLANKLINE(THIS, TOTENUMVAL);
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr, 22, "const char* enumvalues", RwpInt, ENUMPTR->ID, RwpLenPtr | RwpEnd, 7, "[] = { ");
  for (PInteger first = 0, last = ENUMPTR->SIZE - 1; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POSINENUM = first; !done; done = POSINENUM == last ? 1 : (++POSINENUM, 0)) {
      if (POSINENUM != 0) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      Write(&THIS->OUTPUT, 1, RwpChar, '"', RwpStringPtr, &ENUMPTR->VALUES[subrange(POSINENUM, 0, 127)], RwpChar | RwpEnd, '"');
    }
    break;
  }
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " };");
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTVARIABLEDEFINITION(TCGC_OBJ* THIS, TSDVARIABLEDEF* VARPTR) {
  _CGC_OUTBLANKLINE(THIS, TOTVAR);
  _CGC_OUTINDENT(THIS);
  _CGC_OUTVARIABLEDECLARATION(THIS, VARPTR);
  if (VARPTR->ISCONSTANT) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
    _CGC_OUTEXPRESSION(THIS, VARPTR->CONSTANTVALUE);
  }
  else if (VARPTR->LOCATION != PNil) {
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
    if (!ISSAMETYPE(VARPTR->TYPEPTR, VARPTR->LOCATION->TYPEPTR)) {
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '(');
      _CGC_OUTTYPEREFERENCE(THIS, VARPTR->TYPEPTR);
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "*)");
    }
    _CGC_OUTEXADDRESS(THIS, VARPTR->LOCATION);
  }
  else if (ISFILETYPE(VARPTR->TYPEPTR)) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 26, " = (PFile){.handle = PNil}");
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTSUBROUTINEPROTOTYPE(TCGC_OBJ* THIS, TSDSUBROUTINEDEF* DEF) {
  PInteger POS;
  _CGC_OUTINDENT(THIS);
  _CGC_OUTNAMEANDTYPE(THIS, &DEF->EXTERNALNAME, DEF->RETURNTYPEPTR);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '(');
  for (PInteger first = 1, last = DEF->ARGS.COUNT; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
      if (POS != 1) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ", ");
      _CGC_OUTARGUMENTDECLARATION(THIS, &DEF->ARGS.DEFS[subrange(POS, 1, 16) - 1]);
    }
    break;
  }
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
}

void _CGC_OUTSUBROUTINEDECLARATION(TCGC_OBJ* THIS, TSDSUBROUTINEDEF* DEF) {
  _CGC_OUTBLANKLINE(THIS, TOTFUNDEC);
  _CGC_OUTSUBROUTINEPROTOTYPE(THIS, DEF);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
}

TSDEFENTRY* _CGC_GETSTARTOFDEFINITIONS(TSSCOPEOBJ* SCOPE) {
  TSDEFENTRY* RESULT;
  STACK_GETOLDEST(&SCOPE->LATESTDEF, &RESULT);
  return RESULT;
}

void _CGC_OUTSUBROUTINEDEFINITION(TCGC_OBJ* THIS, TSDSUBROUTINEDEF* DEF) {
  TSDEFENTRY* FIRSTDEF;
  FIRSTDEF = _CGC_GETSTARTOFDEFINITIONS(&DEF->SCOPE);
  _CGC_OUTBLANKLINE(THIS, TOTFUNDEF);
  _CGC_OUTSUBROUTINEPROTOTYPE(THIS, DEF);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ' ');
  _CGC_OUTBEGIN(THIS);
  _CGC_OUTLOCALDEFINITIONS(THIS, FIRSTDEF);
  _CGC_OUTBODY(THIS, DEF->BODY);
  if (DEF->RETURNTYPEPTR != PNil) {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 14, "return RESULT;");
    _CGC_OUTNEWLINE(THIS);
  }
  _CGC_OUTEND(THIS);
}

void _CGC_OUTDEFINEDTYPES(TCGC_OBJ* THIS, TSDEFENTRY* DEF) {
  while (DEF != PNil) {
    if (DEF->CLS == SDCTYPE && DEF->TYPEDEF.ALIASFOR != PNil) _CGC_OUTTYPEDEFINITION(THIS, &DEF->TYPEDEF);
    DEF = DEF->NEWER;
  }
}

void _CGC_OUTDEFINEDVARIABLES(TCGC_OBJ* THIS, TSDEFENTRY* DEF) {
  while (DEF != PNil) {
    if (DEF->CLS == SDCVARIABLE && !DEF->VARDEF.ISARGUMENT && DEF->VARDEF.ISALIASFOR == PNil) _CGC_OUTVARIABLEDEFINITION(THIS, &DEF->VARDEF);
    DEF = DEF->NEWER;
  }
}

void _CGC_OUTDEFINEDTEMPORARYVARIABLES(TCGC_OBJ* THIS, TSDEFENTRY* DEF) {
  while (DEF != PNil) {
    if (DEF->CLS == SDCTMPVAR) _CGC_OUTVARIABLEDEFINITION(THIS, &DEF->TMPVARDEF.VARDEF);
    DEF = DEF->NEWER;
  }
}

void _CGC_OUTDEFINEDENUMVALUEARRAYS(TCGC_OBJ* THIS, TSDEFENTRY* DEF) {
  while (DEF != PNil) {
    if (DEF->CLS == SDCTYPE && DEF->TYPEDEF.CLS == SDTCENUM && DEF->TYPEDEF.ENUMPTR->NEEDVALUEARRAY) {
      _CGC_OUTENUMVALUES(THIS, DEF->TYPEDEF.ENUMPTR);
      DEF->TYPEDEF.ENUMPTR->NEEDVALUEARRAY = 0;
    }
    DEF = DEF->NEWER;
  }
}

void _CGC_OUTDEFINEDFUNCTIONPROTOTYPES(TCGC_OBJ* THIS, TSDEFENTRY* DEF) {
  while (DEF != PNil) {
    if (DEF->CLS == SDCSUBROUTINE && DEF->SRDEF.HADDECLARATION) _CGC_OUTSUBROUTINEDECLARATION(THIS, &DEF->SRDEF);
    DEF = DEF->NEWER;
  }
}

void _CGC_OUTDEFINEDFUNCTIONS(TCGC_OBJ* THIS, TSDEFENTRY* DEF, PBoolean ISFORWARDDECLARED) {
  while (DEF != PNil) {
    if (DEF->CLS == SDCSUBROUTINE && DEF->SRDEF.HADDECLARATION == ISFORWARDDECLARED) _CGC_OUTSUBROUTINEDEFINITION(THIS, &DEF->SRDEF);
    DEF = DEF->NEWER;
  }
}

void _CGC_OUTGLOBALDEFINITIONS(TCGC_OBJ* THIS, TSDEFENTRY* FIRSTDEF) {
  _CGC_OUTDEFINEDTYPES(THIS, FIRSTDEF);
  _CGC_OUTDEFINEDVARIABLES(THIS, FIRSTDEF);
  _CGC_OUTDEFINEDENUMVALUEARRAYS(THIS, FIRSTDEF);
  _CGC_OUTDEFINEDFUNCTIONPROTOTYPES(THIS, FIRSTDEF);
  _CGC_OUTDEFINEDFUNCTIONS(THIS, FIRSTDEF, 0);
  _CGC_OUTDEFINEDFUNCTIONS(THIS, FIRSTDEF, 1);
}

void _CGC_OUTSTEMPTY(TCGC_OBJ* THIS) {
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTSTSEQUENCE(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  PBoolean WASMULTISTATEMENT;
  WASMULTISTATEMENT = THIS->ISMULTISTATEMENT;
  if (!WASMULTISTATEMENT) {
    _CGC_OUTINDENT(THIS);
    _CGC_OUTBEGIN(THIS);
  }
  _CGC_OUTBODY(THIS, STMT->SEQUENCE);
  if (!WASMULTISTATEMENT) _CGC_OUTEND(THIS);
  THIS->ISMULTISTATEMENT = WASMULTISTATEMENT;
}

void _CGC_OUTSTASSIGN(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  _CGC_OUTINDENT(THIS);
  _CGC_OUTEXPRESSION(THIS, STMT->LHS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
  _CGC_OUTEXPRESSION(THIS, STMT->RHS);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTSTPROCCALL(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  _CGC_OUTINDENT(THIS);
  _CGC_OUTEXPRESSION(THIS, STMT->PROCCALL);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTSTIF(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  PBoolean WASMULTISTATEMENT;
  WASMULTISTATEMENT = THIS->ISMULTISTATEMENT;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "if (");
  _CGC_OUTEXPRESSION(THIS, STMT->IFCOND);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ") ");
  THIS->ISMULTISTATEMENT = 0;
  _CGC_OUTSTATEMENT(THIS, STMT->IFTHEN);
  if (STMT->IFELSE->CLS != SSCEMPTY) {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "else ");
    THIS->ISMULTISTATEMENT = 0;
    _CGC_OUTSTATEMENT(THIS, STMT->IFELSE);
  }
  THIS->ISMULTISTATEMENT = WASMULTISTATEMENT;
}

void _CGC_OUTSTREPEAT(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  TSEXPRESSIONOBJ* TMPEXPR;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, "do ");
  _CGC_OUTBEGIN(THIS);
  _CGC_OUTBODY(THIS, STMT->REPEATSEQUENCE);
  _CGC_OUTENDSAMELINE(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, " while (");
  TMPEXPR = EXOPNOT(EXCOPY(STMT->UNTILCOND));
  _CGC_OUTEXPRESSION(THIS, TMPEXPR);
  EXDISPOSE(&TMPEXPR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ");");
  _CGC_OUTNEWLINE(THIS);
}

void _CGC_OUTSTWHILE(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  PBoolean WASMULTISTATEMENT;
  WASMULTISTATEMENT = THIS->ISMULTISTATEMENT;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "while (");
  _CGC_OUTEXPRESSION(THIS, STMT->WHILECOND);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ") ");
  THIS->ISMULTISTATEMENT = 0;
  _CGC_OUTSTATEMENT(THIS, STMT->WHILESTATEMENT);
  THIS->ISMULTISTATEMENT = WASMULTISTATEMENT;
}

void _CGC_OUTSTFOR(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  PBoolean WASMULTISTATEMENT;
  TSDTYPEDEF* LIMITTYPE;
  TSDVARIABLEDEF FIRST;
  TSDVARIABLEDEF LAST;
  PString tmp1;
  WASMULTISTATEMENT = THIS->ISMULTISTATEMENT;
  LIMITTYPE = STMT->ITERATOR->TYPEPTR;
  if (ISENUMTYPE(LIMITTYPE)) LIMITTYPE = PRIMITIVETYPES.PTINTEGER;
  FIRST = ({ tmp1 = str_make(5, "first"); MAKEVARIABLE(&tmp1, LIMITTYPE); });
  LAST = ({ tmp1 = str_make(4, "last"); MAKEVARIABLE(&tmp1, LIMITTYPE); });
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "for (");
  _CGC_OUTVARIABLEDECLARATION(THIS, &FIRST);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 3, " = ");
  _CGC_OUTEXPRESSION(THIS, STMT->FIRST);
  Write(&THIS->OUTPUT, 1, RwpLenPtr, 2, ", ", RwpStringPtr, &LAST.NAME, RwpLenPtr | RwpEnd, 3, " = ");
  _CGC_OUTEXPRESSION(THIS, STMT->LAST);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "; first ");
  if (STMT->ASCENDING) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "<=");
  else Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ">=");
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 19, " last; /*breaks*/) ");
  _CGC_OUTBEGIN(THIS);
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 18, "PBoolean done = 0;");
  _CGC_OUTNEWLINE(THIS);
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "for (");
  _CGC_OUTEXPRESSION(THIS, STMT->ITERATOR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 24, " = first; !done; done = ");
  _CGC_OUTEXPRESSION(THIS, STMT->ITERATOR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 16, " == last ? 1 : (");
  if (STMT->ASCENDING) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "++");
  else Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, "--");
  _CGC_OUTEXPRESSION(THIS, STMT->ITERATOR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, ", 0)) ");
  THIS->ISMULTISTATEMENT = 0;
  _CGC_OUTSTATEMENT(THIS, STMT->FORSTATEMENT);
  THIS->ISMULTISTATEMENT = WASMULTISTATEMENT;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "break;");
  _CGC_OUTNEWLINE(THIS);
  _CGC_OUTEND(THIS);
}

void _CGC_OUTSTWITH(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  PBoolean WASMULTISTATEMENT;
  WASMULTISTATEMENT = THIS->ISMULTISTATEMENT;
  if (!WASMULTISTATEMENT) {
    _CGC_OUTINDENT(THIS);
    _CGC_OUTBEGIN(THIS);
  }
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpStringPtr, &STMT->WITHVAR->NAME, RwpLenPtr | RwpEnd, 3, " = ");
  if (STMT->WITHVAR->ISREFERENCE) _CGC_OUTEXADDRESS(THIS, STMT->WITHVALUE);
  else _CGC_OUTEXPRESSION(THIS, STMT->WITHVALUE);
  Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ';');
  _CGC_OUTNEWLINE(THIS);
  _CGC_OUTSTATEMENT(THIS, STMT->WITHSTATEMENT);
  if (!WASMULTISTATEMENT) _CGC_OUTEND(THIS);
  THIS->ISMULTISTATEMENT = WASMULTISTATEMENT;
}

void _CGC_OUTSTCASE(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  TSSCASEENTRY* CASEENTRY;
  PBoolean WASMULTISTATEMENT;
  PBoolean HASDEFAULT;
  HASDEFAULT = 0;
  WASMULTISTATEMENT = THIS->ISMULTISTATEMENT;
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "switch (");
  _CGC_OUTEXPRESSION(THIS, STMT->CASESELECTOR);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 2, ") ");
  _CGC_OUTBEGIN(THIS);
  CASEENTRY = STMT->CASEENTRY;
  while (CASEENTRY != PNil) {
    _CGC_OUTINDENT(THIS);
    if (CASEENTRY->CASELABEL != PNil) {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "case ");
      _CGC_OUTEXPRESSION(THIS, CASEENTRY->CASELABEL);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ':');
    }
    else {
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "default:");
      HASDEFAULT = 1;
    }
    THIS->INDENT = THIS->INDENT + 1;
    _CGC_OUTNEWLINE(THIS);
    THIS->ISMULTISTATEMENT = 1;
    _CGC_OUTSTATEMENT(THIS, CASEENTRY->STATEMENT);
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "break;");
    THIS->INDENT = THIS->INDENT - 1;
    _CGC_OUTNEWLINE(THIS);
    THIS->ISMULTISTATEMENT = WASMULTISTATEMENT;
    CASEENTRY = CASEENTRY->NEXT;
  }
  if (!HASDEFAULT) {
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "default:");
    THIS->INDENT = THIS->INDENT + 1;
    _CGC_OUTNEWLINE(THIS);
    _CGC_OUTINDENT(THIS);
    Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 6, "break;");
    THIS->INDENT = THIS->INDENT - 1;
    _CGC_OUTNEWLINE(THIS);
  }
  _CGC_OUTEND(THIS);
}

void _CGC_OUTPROGRAM(TCGC_OBJ* THIS, TSPROGRAMOBJ* PROG) {
  TSDEFENTRY* FIRSTDEF;
  FIRSTDEF = _CGC_GETSTARTOFDEFINITIONS(&PROG->SCOPE);
  Write(&THIS->OUTPUT, 1, RwpLenPtr, 12, "/* Program: ", RwpStringPtr, &PROG->NAME, RwpLenPtr | RwpEnd, 3, " */");
  _CGC_OUTNEWLINE(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 20, "#include \"pascual.h\"");
  _CGC_OUTNEWLINE(THIS);
  _CGC_OUTGLOBALDEFINITIONS(THIS, FIRSTDEF);
  _CGC_OUTBLANKLINE(THIS, TOTFUNDEF);
  _CGC_OUTINDENT(THIS);
  Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 20, "void pascual_main() ");
  _CGC_OUTBEGIN(THIS);
  _CGC_OUTDEFINEDTEMPORARYVARIABLES(THIS, FIRSTDEF);
  _CGC_OUTBODY(THIS, PROG->BODY);
  _CGC_OUTEND(THIS);
}

void _CGC_SETOUTPUTFILE_EXT(TCODEGENBASE* THIS, const PString* NAME) {
  TCGC_OBJ** CG = (TCGC_OBJ**)&THIS;
  PFile F = (PFile){.handle = PNil};
  Assign(&F, NAME, 1);
  Rewrite(&F, 0, 1);
  (*CG)->OUTPUT = F;
}

void _CGC_GENERATE_EXT(TCODEGENBASE* THIS, TSPROGRAMOBJ* AST) {
  TCGC_OBJ** CG = (TCGC_OBJ**)&THIS;
  _CGC_OUTPROGRAM(*CG, AST);
  Close(&(*CG)->OUTPUT, 1);
}

void CG_C_INIT(TCODEGENBASE** CODEGEN) {
  TCGC_OBJ** THIS = (TCGC_OBJ**)CODEGEN;
  New((void**)&(*THIS), sizeof(TCGC_OBJ));
  (*THIS)->BASE.SETOUTPUTFILE = &_CGC_SETOUTPUTFILE_EXT;
  (*THIS)->BASE.GENERATE = &_CGC_GENERATE_EXT;
  (*THIS)->OUTPUT = OUTPUT;
  (*THIS)->ISMULTISTATEMENT = 0;
  (*THIS)->INDENT = 0;
  (*THIS)->NEWLINE = 1;
  (*THIS)->LASTOUT = TOTNONE;
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

void EXDISPOSE(TSEXPRESSIONOBJ** EXPR) {
  PInteger POS;
  switch ((*EXPR)->CLS) {
    case SECIMMEDIATE:
      _DISPOSEIMMEDIATE(&(*EXPR)->IMMEDIATE);
      break;
    case SECARRAYVALUE:
      _DISPOSEARRAYELEMS((*EXPR)->ARRAYELEM);
      break;
    case SECRECORDVALUE:
      _DISPOSERECORDFIELDS((*EXPR)->RECORDFIELD);
      break;
    case SECSETVALUE:
      EXDISPOSE(&(*EXPR)->SETBASE);
      _DISPOSEBOUNDS((*EXPR)->SETBOUNDS);
      break;
    case SECTOSTRING:
      EXDISPOSE(&(*EXPR)->TOSTRPARENT);
      break;
    case SECTOREAL:
      EXDISPOSE(&(*EXPR)->TOREALPARENT);
      break;
    case SECTOUNTYPEDPTR:
      EXDISPOSE(&(*EXPR)->TOUNTYPEDPTRPARENT);
      break;
    case SECTOGENERICFILE:
      EXDISPOSE(&(*EXPR)->TOGENERICFILEPARENT);
      break;
    case SECWITHTMPVAR:
      EXDISPOSE(&(*EXPR)->TMPVARVALUE);
      EXDISPOSE(&(*EXPR)->TMPVARCHILD);
      break;
    case SECSUBRANGE:
      EXDISPOSE(&(*EXPR)->SUBRANGEPARENT);
      break;
    case SECFIELD:
      EXDISPOSE(&(*EXPR)->RECEXPR);
      break;
    case SECARRAY:
      EXDISPOSE(&(*EXPR)->ARRAYEXPR);
      EXDISPOSE(&(*EXPR)->ARRAYINDEX);
      break;
    case SECPOINTER:
      EXDISPOSE(&(*EXPR)->POINTEREXPR);
      break;
    case SECADDRESS:
      EXDISPOSE(&(*EXPR)->ADDRESSEXPR);
      break;
    case SECSTRINGCHAR:
      EXDISPOSE(&(*EXPR)->STRINGEXPR);
      EXDISPOSE(&(*EXPR)->STRINGINDEX);
      break;
    case SECFNCALL:
      EXDISPOSE(&(*EXPR)->FNEXPR);
      for (PInteger first = 1, last = (*EXPR)->CALLARGS.SIZE; first <= last; /*breaks*/) {
        PBoolean done = 0;
        for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) EXDISPOSE(&(*EXPR)->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
        break;
      }
      break;
    case SECCONVERTTOSTR:
      _DISPOSEWRITEARG(&(*EXPR)->TOSTRSRC);
      EXDISPOSE(&(*EXPR)->TOSTRDEST);
      break;
    case SECCONVERTTOVAL:
      EXDISPOSE(&(*EXPR)->TOVALSRC);
      EXDISPOSE(&(*EXPR)->TOVALDEST);
      EXDISPOSE(&(*EXPR)->TOVALCODE);
      break;
    case SECREAD:
      _DISPOSEREADEXPR(EXPR);
      break;
    case SECWRITE:
      _DISPOSEWRITEEXPR(EXPR);
      break;
    case SECUNARYOP:
      EXDISPOSE(&(*EXPR)->UNARY.PARENT);
      break;
    case SECBINARYOP:
      EXDISPOSE(&(*EXPR)->BINARY.LEFT);
      EXDISPOSE(&(*EXPR)->BINARY.RIGHT);
      break;
    default:
      break;
  }
  Dispose((void**)&(*EXPR));
}

TSEXPRESSIONOBJ* EXCOPY(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEXPRESSIONOBJ* COPY;
  PInteger POS;
  COPY = _NEWEXPR(EXPR->CLS);
  COPY->TYPEPTR = EXPR->TYPEPTR;
  COPY->ISASSIGNABLE = EXPR->ISASSIGNABLE;
  COPY->ISADDRESSABLE = EXPR->ISADDRESSABLE;
  COPY->ISFUNCTIONRESULT = EXPR->ISFUNCTIONRESULT;
  COPY->CHECKBOUNDS = EXPR->CHECKBOUNDS;
  switch (EXPR->CLS) {
    case SECIMMEDIATE:
      COPY->IMMEDIATE = _COPYIMMEDIATE(&EXPR->IMMEDIATE);
      break;
    case SECARRAYVALUE:
      COPY->ARRAYELEM = _COPYARRAYELEMS(EXPR->ARRAYELEM);
      break;
    case SECRECORDVALUE:
      COPY->RECORDFIELD = _COPYRECORDFIELDS(EXPR->RECORDFIELD);
      break;
    case SECSETVALUE:
      COPY->SETBASE = EXCOPY(EXPR->SETBASE);
      COPY->SETBOUNDS = _COPYBOUNDS(EXPR->SETBOUNDS);
      break;
    case SECTOSTRING:
      COPY->TOSTRPARENT = EXCOPY(EXPR->TOSTRPARENT);
      break;
    case SECTOREAL:
      COPY->TOREALPARENT = EXCOPY(EXPR->TOREALPARENT);
      break;
    case SECTOUNTYPEDPTR:
      COPY->TOUNTYPEDPTRPARENT = EXCOPY(EXPR->TOUNTYPEDPTRPARENT);
      break;
    case SECTOGENERICFILE:
      COPY->TOGENERICFILEPARENT = EXCOPY(EXPR->TOGENERICFILEPARENT);
      break;
    case SECWITHTMPVAR:
      COPY->TMPVARPTR = EXPR->TMPVARPTR;
      COPY->TMPVARVALUE = EXCOPY(EXPR->TMPVARVALUE);
      COPY->TMPVARCHILD = EXCOPY(EXPR->TMPVARCHILD);
      break;
    case SECSUBRANGE:
      COPY->SUBRANGEPARENT = EXCOPY(EXPR->SUBRANGEPARENT);
      break;
    case SECVARIABLE:
      COPY->VARPTR = EXPR->VARPTR;
      break;
    case SECFIELD:
      COPY->RECEXPR = EXCOPY(EXPR->RECEXPR);
      COPY->RECFIELDNUM = EXPR->RECFIELDNUM;
      break;
    case SECARRAY:
      COPY->ARRAYEXPR = EXCOPY(EXPR->ARRAYEXPR);
      COPY->ARRAYINDEX = EXCOPY(EXPR->ARRAYINDEX);
      break;
    case SECPOINTER:
      COPY->POINTEREXPR = EXCOPY(EXPR->POINTEREXPR);
      break;
    case SECADDRESS:
      COPY->ADDRESSEXPR = EXCOPY(EXPR->ADDRESSEXPR);
      break;
    case SECSTRINGCHAR:
      COPY->STRINGEXPR = EXCOPY(EXPR->STRINGEXPR);
      COPY->STRINGINDEX = EXCOPY(EXPR->STRINGINDEX);
      break;
    case SECFNREF:
      COPY->FNPTR = EXPR->FNPTR;
      break;
    case SECFNCALL:
      COPY->FNEXPR = EXCOPY(EXPR->FNEXPR);
      COPY->CALLARGS.SIZE = EXPR->CALLARGS.SIZE;
      for (PInteger first = 1, last = EXPR->CALLARGS.SIZE; first <= last; /*breaks*/) {
        PBoolean done = 0;
        for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) COPY->CALLARGS.VALUES[subrange(POS, 1, 16) - 1] = EXCOPY(EXPR->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]);
        break;
      }
      break;
    case SECPSFNREF:
      COPY->PSFNPTR = EXPR->PSFNPTR;
      break;
    case SECSIZEOF:
      COPY->SIZEOFTYPEPTR = EXPR->SIZEOFTYPEPTR;
      break;
    case SECCONVERTTOSTR:
      COPY->TOSTRSRC = _COPYWRITEARG(&EXPR->TOSTRSRC);
      COPY->TOSTRDEST = EXCOPY(EXPR->TOSTRDEST);
      break;
    case SECCONVERTTOVAL:
      COPY->TOVALSRC = EXCOPY(EXPR->TOVALSRC);
      COPY->TOVALDEST = EXCOPY(EXPR->TOVALDEST);
      COPY->TOVALCODE = EXCOPY(EXPR->TOVALCODE);
      break;
    case SECREAD:
      _COPYREADEXPR(&EXPR, &COPY);
      break;
    case SECWRITE:
      _COPYWRITEEXPR(&EXPR, &COPY);
      break;
    case SECUNARYOP:
      COPY->UNARY.PARENT = EXCOPY(EXPR->UNARY.PARENT);
      COPY->UNARY.OP = EXPR->UNARY.OP;
      break;
    case SECBINARYOP:
      COPY->BINARY.LEFT = EXCOPY(EXPR->BINARY.LEFT);
      COPY->BINARY.RIGHT = EXCOPY(EXPR->BINARY.RIGHT);
      COPY->BINARY.OP = EXPR->BINARY.OP;
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 24, "Cannot copy expression: ", CpEnd | CpString, EXDESCRIBE(EXPR)));
      break;
  }
  RESULT = COPY;
  return RESULT;
}

PString EXDESCRIBEOPERATOR(TSEOPERATOR OP) {
  PString RESULT;
  RESULT = _EXOPERATORS[OP].NAME;
  return RESULT;
}

PString EXDESCRIBE(TSEXPRESSIONOBJ* EXPR) {
  PString RESULT;
  PInteger POS;
  TSEWRITEARG* tmp1;
  switch (EXPR->CLS) {
    case SECIMMEDIATE:
      RESULT = _DESCRIBEIMMEDIATE(EXPR);
      break;
    case SECARRAYVALUE:
      RESULT = _DESCRIBEARRAYVALUE(EXPR);
      break;
    case SECRECORDVALUE:
      RESULT = _DESCRIBERECORDVALUE(EXPR);
      break;
    case SECSETVALUE:
      RESULT = _DESCRIBESETVALUE(EXPR);
      break;
    case SECTOSTRING:
      RESULT = EXDESCRIBE(EXPR->TOSTRPARENT);
      break;
    case SECTOREAL:
      RESULT = EXDESCRIBE(EXPR->TOREALPARENT);
      break;
    case SECTOUNTYPEDPTR:
      RESULT = EXDESCRIBE(EXPR->TOUNTYPEDPTRPARENT);
      break;
    case SECTOGENERICFILE:
      RESULT = EXDESCRIBE(EXPR->TOGENERICFILEPARENT);
      break;
    case SECWITHTMPVAR:
      RESULT = EXDESCRIBE(EXPR->TMPVARCHILD);
      break;
    case SECSUBRANGE:
      RESULT = EXDESCRIBE(EXPR->TOSTRPARENT);
      break;
    case SECVARIABLE:
      if (EXPR->VARPTR->ISALIASFOR == PNil) RESULT = EXPR->VARPTR->NAME;
      else RESULT = EXDESCRIBE(EXPR->VARPTR->ISALIASFOR);
      break;
    case SECFIELD:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->RECEXPR), CpChar, '.', CpEnd | CpStringPtr, &EXPR->RECEXPR->TYPEPTR->RECPTR->FIELDS[subrange(EXPR->RECFIELDNUM, 1, 64) - 1].NAME);
      break;
    case SECARRAY:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->ARRAYEXPR), CpChar, '[', CpString, EXDESCRIBE(EXPR->ARRAYINDEX), CpEnd | CpChar, ']');
      break;
    case SECPOINTER:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->POINTEREXPR), CpEnd | CpChar, '^');
      break;
    case SECADDRESS:
      RESULT = CONCAT(CpChar, '@', CpEnd | CpString, EXDESCRIBE(EXPR->ADDRESSEXPR));
      break;
    case SECSTRINGCHAR:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->STRINGEXPR), CpChar, '[', CpString, EXDESCRIBE(EXPR->STRINGINDEX), CpEnd | CpChar, ']');
      break;
    case SECFNREF:
      RESULT = EXPR->FNPTR->NAME;
      break;
    case SECFNCALL:
      RESULT = CONCAT(CpString, EXDESCRIBE(EXPR->FNEXPR), CpEnd | CpChar, '(');
      for (PInteger first = 1, last = EXPR->CALLARGS.SIZE; first <= last; /*breaks*/) {
        PBoolean done = 0;
        for (POS = first; !done; done = POS == last ? 1 : (++POS, 0)) {
          if (POS != 1) RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpLenPtr, 2, ", ");
          RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpString, EXDESCRIBE(EXPR->CALLARGS.VALUES[subrange(POS, 1, 16) - 1]));
        }
        break;
      }
      RESULT = CONCAT(CpStringPtr, &RESULT, CpEnd | CpChar, ')');
      break;
    case SECPSFNREF:
      RESULT = EXPR->PSFNPTR->NAME;
      break;
    case SECSIZEOF:
      RESULT = CONCAT(CpLenPtr, 7, "SIZEOF(", CpString, TYPENAME(EXPR->SIZEOFTYPEPTR), CpEnd | CpChar, ')');
      break;
    case SECCONVERTTOSTR:
      tmp1 = &EXPR->TOSTRSRC;
      if (tmp1->WIDTH == PNil) RESULT = CONCAT(CpLenPtr, 4, "STR(", CpString, EXDESCRIBE(tmp1->ARG), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(EXPR->TOSTRDEST), CpEnd | CpChar, ')');
      else if (tmp1->PREC == PNil) RESULT = CONCAT(CpLenPtr, 4, "STR(", CpString, EXDESCRIBE(tmp1->ARG), CpChar, ':', CpString, EXDESCRIBE(tmp1->WIDTH), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(EXPR->TOSTRDEST), CpEnd | CpChar, ')');
      else RESULT = CONCAT(CpLenPtr, 4, "STR(", CpString, EXDESCRIBE(tmp1->ARG), CpChar, ':', CpString, EXDESCRIBE(tmp1->WIDTH), CpChar, ':', CpString, EXDESCRIBE(tmp1->PREC), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(EXPR->TOSTRDEST), CpEnd | CpChar, ')');
      break;
    case SECCONVERTTOVAL:
      RESULT = CONCAT(CpLenPtr, 4, "VAL(", CpString, EXDESCRIBE(EXPR->TOVALSRC), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(EXPR->TOVALDEST), CpLenPtr, 2, ", ", CpString, EXDESCRIBE(EXPR->TOVALCODE), CpEnd | CpChar, ')');
      break;
    case SECREAD:
      if (EXPR->READLN) RESULT = str_make(11, "READLN(...)");
      else RESULT = str_make(9, "READ(...)");
      break;
    case SECWRITE:
      if (EXPR->WRITELN) RESULT = str_make(12, "WRITELN(...)");
      else RESULT = str_make(10, "WRITE(...)");
      break;
    case SECUNARYOP:
      RESULT = _DESCRIBEUNARYOPEXPR(EXPR);
      break;
    case SECBINARYOP:
      RESULT = _DESCRIBEBINARYOPEXPR(EXPR);
      break;
    default:
      INTERNALERROR(str_make(26, "Cannot describe expression"));
      break;
  }
  return RESULT;
}

void EXMARKINITIALIZED(TSEXPRESSIONOBJ* LHS) {
  PBoolean ISTERMINAL;
  ISTERMINAL = 0;
  while (!ISTERMINAL) switch (LHS->CLS) {
    case SECFIELD:
      LHS = LHS->RECEXPR;
      break;
    case SECARRAY:
      LHS = LHS->ARRAYEXPR;
      break;
    case SECSTRINGCHAR:
      LHS = LHS->STRINGEXPR;
      break;
    case SECTOSTRING:
      LHS = LHS->TOSTRPARENT;
      break;
    case SECTOREAL:
      LHS = LHS->TOREALPARENT;
      break;
    case SECTOGENERICFILE:
      LHS = LHS->TOGENERICFILEPARENT;
      break;
    default:
      ISTERMINAL = 1;
      break;
  }
  if (LHS->CLS == SECVARIABLE) {
    LHS->VARPTR->WASINITIALIZED = 1;
    if (LHS->VARPTR->ISREFERENCE) LHS->VARPTR->WASUSED = 1;
  }
}

PInteger EXGETORDINAL(TSEXPRESSIONOBJ* EXPR) {
  PInteger RESULT;
  PString tmp1;
  TSEIMMEDIATE* tmp2;
  EXCLEARTMPVAR(EXPR);
  if (!EXISIMMEDIATE(EXPR)) {
    tmp1 = str_make(27, "Expected an immediate value");
    ERRORFOREXPR(&tmp1, EXPR);
  }
  tmp2 = &EXPR->IMMEDIATE;
  switch (tmp2->CLS) {
    case SEICBOOLEAN:
      RESULT = tmp2->BOOLEANVAL;
      break;
    case SEICINTEGER:
      RESULT = tmp2->INTEGERVAL;
      break;
    case SEICCHAR:
      RESULT = (int)tmp2->CHARVAL;
      break;
    case SEICENUM:
      RESULT = tmp2->ENUMORDINAL;
      break;
    default:
      tmp1 = str_make(19, "Expected an ordinal");
      ERRORFOREXPR(&tmp1, EXPR);
      break;
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXGETANTIORDINAL(PInteger ORDINAL, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  TYPEPTR = GETFUNDAMENTALTYPE(TYPEPTR);
  switch (TYPEPTR->CLS) {
    case SDTCBOOLEAN:
      if (ORDINAL == 0) RESULT = EXBOOLEANCONSTANT(0);
      else RESULT = EXBOOLEANCONSTANT(1);
      break;
    case SDTCINTEGER:
      RESULT = EXINTEGERCONSTANT(ORDINAL);
      break;
    case SDTCCHAR:
      RESULT = EXCHARCONSTANT(CHR(ORDINAL));
      break;
    case SDTCENUM:
      RESULT = EXENUMCONSTANT(ORDINAL, TYPEPTR);
      break;
    default:
      INTERNALERROR(CONCAT(CpLenPtr, 32, "Cannot compute anti-ordinal for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
      break;
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPNEG(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEIMMEDIATE* tmp1;
  ENSURENUMERICEXPR(EXPR);
  if (EXISIMMEDIATE(EXPR)) {
    tmp1 = &EXPR->IMMEDIATE;
    if (tmp1->CLS == SEICINTEGER) tmp1->INTEGERVAL = -tmp1->INTEGERVAL;
    else tmp1->REALVAL = -tmp1->REALVAL;
    RESULT = EXPR;
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, SEONEG, EXPR->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPADD(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_INTEGERS(LEFT, RIGHT, SEOADD);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_NUMBERS(LEFT, RIGHT, SEOADD);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPADD_STRINGS(LEFT, RIGHT);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPUNION_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOADD);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPSUB(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_INTEGERS(LEFT, RIGHT, SEOSUB);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_NUMBERS(LEFT, RIGHT, SEOSUB);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPDIFFERENCE_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOSUB);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPMUL(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_INTEGERS(LEFT, RIGHT, SEOMUL);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_NUMBERS(LEFT, RIGHT, SEOMUL);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPINTERSECTION_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOMUL);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPDIVREAL(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR) && (ISREALTYPE(LEFT->TYPEPTR) || ISREALTYPE(RIGHT->TYPEPTR))) RESULT = _EXOPARITHMETIC_NUMBERS(LEFT, RIGHT, SEODIVREAL);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEODIVREAL);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPDIVINT(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_INTEGERS(LEFT, RIGHT, SEODIVINT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEODIVINT);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPMOD(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPARITHMETIC_INTEGERS(LEFT, RIGHT, SEOMOD);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOMOD);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPNOT(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(EXPR->TYPEPTR)) RESULT = _EXOPNOT_BOOLEAN(EXPR);
  else if (ISINTEGERTYPE(EXPR->TYPEPTR)) RESULT = _EXOPNOT_INTEGER(EXPR);
  else ERRORINVALIDOPERATOR(EXPR, SEONOT);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPAND(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLOGICAL_BOOLEANS(LEFT, RIGHT, SEOAND);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLOGICAL_INTEGERS(LEFT, RIGHT, SEOAND);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOAND);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPOR(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLOGICAL_BOOLEANS(LEFT, RIGHT, SEOOR);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLOGICAL_INTEGERS(LEFT, RIGHT, SEOOR);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOOR);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPXOR(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLOGICAL_BOOLEANS(LEFT, RIGHT, SEOXOR);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPLOGICAL_INTEGERS(LEFT, RIGHT, SEOXOR);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOXOR);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPSHL(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPBITWISE_INTEGERS(LEFT, RIGHT, SEOSHL);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOSHL);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPSHR(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPBITWISE_INTEGERS(LEFT, RIGHT, SEOSHR);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOSHR);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPIN(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISSETTYPE(RIGHT->TYPEPTR)) {
    if (EXISIMMEDIATE(RIGHT) || RIGHT->CLS == SECSETVALUE) RESULT = _EXOPIN_IMPL(LEFT, RIGHT);
    else RESULT = _EXOP_MAKEBINARY(LEFT, RIGHT, SEOIN, PRIMITIVETYPES.PTBOOLEAN);
  }
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOXOR);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPEQ(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_BOOLEANS(LEFT, RIGHT, SEOEQ);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_INTEGERS(LEFT, RIGHT, SEOEQ);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_NUMBERS(LEFT, RIGHT, SEOEQ);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_STRINGS(LEFT, RIGHT, SEOEQ);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_ENUMS(LEFT, RIGHT, SEOEQ);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPEQ_SETS(LEFT, RIGHT);
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPEQ_POINTERS(LEFT, RIGHT);
  else if (AREFUNCTIONSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPEQ_FUNCTIONS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOEQ);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPNE(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_BOOLEANS(LEFT, RIGHT, SEONE);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_INTEGERS(LEFT, RIGHT, SEONE);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_NUMBERS(LEFT, RIGHT, SEONE);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_STRINGS(LEFT, RIGHT, SEONE);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_ENUMS(LEFT, RIGHT, SEONE);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = EXOPNOT(_EXOPEQ_SETS(LEFT, RIGHT));
  else if (AREPOINTERSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPNE_POINTERS(LEFT, RIGHT);
  else if (AREFUNCTIONSCOMPATIBLE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPNE_FUNCTIONS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEONE);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPLT(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_BOOLEANS(LEFT, RIGHT, SEOLT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_INTEGERS(LEFT, RIGHT, SEOLT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_NUMBERS(LEFT, RIGHT, SEOLT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_STRINGS(LEFT, RIGHT, SEOLT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_ENUMS(LEFT, RIGHT, SEOLT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOLT);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPGT(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_BOOLEANS(LEFT, RIGHT, SEOGT);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_INTEGERS(LEFT, RIGHT, SEOGT);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_NUMBERS(LEFT, RIGHT, SEOGT);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_STRINGS(LEFT, RIGHT, SEOGT);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_ENUMS(LEFT, RIGHT, SEOGT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOGT);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPLTEQ(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_BOOLEANS(LEFT, RIGHT, SEOLTEQ);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_INTEGERS(LEFT, RIGHT, SEOLTEQ);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_NUMBERS(LEFT, RIGHT, SEOLTEQ);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_STRINGS(LEFT, RIGHT, SEOLTEQ);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_ENUMS(LEFT, RIGHT, SEOLTEQ);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_SETS(RIGHT, LEFT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOLTEQ);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPGTEQ(TSEXPRESSIONOBJ* LEFT, TSEXPRESSIONOBJ* RIGHT) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISBOOLEANTYPE(LEFT->TYPEPTR) && ISBOOLEANTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_BOOLEANS(LEFT, RIGHT, SEOGTEQ);
  else if (ISINTEGERTYPE(LEFT->TYPEPTR) && ISINTEGERTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_INTEGERS(LEFT, RIGHT, SEOGTEQ);
  else if (ISNUMERICTYPE(LEFT->TYPEPTR) && ISNUMERICTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_NUMBERS(LEFT, RIGHT, SEOGTEQ);
  else if (ISSTRINGYTYPE(LEFT->TYPEPTR) && ISSTRINGYTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_STRINGS(LEFT, RIGHT, SEOGTEQ);
  else if (ISENUMTYPE(LEFT->TYPEPTR) && ISSAMETYPE(LEFT->TYPEPTR, RIGHT->TYPEPTR)) RESULT = _EXOPRELATIONAL_ENUMS(LEFT, RIGHT, SEOGTEQ);
  else if (ISSETTYPE(LEFT->TYPEPTR) && ISSETTYPE(RIGHT->TYPEPTR)) RESULT = _EXOPGTEQ_SETS(LEFT, RIGHT);
  else ERRORINVALIDOPERATOR2(LEFT, RIGHT, SEOGTEQ);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPORD(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  TSEIMMEDIATE* tmp1;
  ENSUREORDINALEXPR(EXPR);
  if (EXISIMMEDIATE(EXPR)) {
    tmp1 = &EXPR->IMMEDIATE;
    switch (tmp1->CLS) {
      case SEICBOOLEAN:
        if (tmp1->BOOLEANVAL) RESULT = EXINTEGERCONSTANT(1);
        else RESULT = EXINTEGERCONSTANT(0);
        break;
      case SEICINTEGER:
        RESULT = EXINTEGERCONSTANT(tmp1->INTEGERVAL);
        break;
      case SEICCHAR:
        RESULT = EXINTEGERCONSTANT((int)tmp1->CHARVAL);
        break;
      case SEICENUM:
        RESULT = EXINTEGERCONSTANT(tmp1->ENUMORDINAL);
        break;
      default:
        break;
    }
    EXDISPOSE(&EXPR);
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, SEOORD, PRIMITIVETYPES.PTINTEGER);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPPRED(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean OUTOFBOUNDS;
  TSEIMMEDIATE* tmp1;
  PString tmp2;
  ENSUREORDINALEXPR(EXPR);
  if (EXISIMMEDIATE(EXPR)) {
    OUTOFBOUNDS = 0;
    tmp1 = &EXPR->IMMEDIATE;
    switch (tmp1->CLS) {
      case SEICBOOLEAN:
        if (tmp1->BOOLEANVAL) tmp1->BOOLEANVAL = 0;
        else OUTOFBOUNDS = 1;
        break;
      case SEICINTEGER:
        tmp1->INTEGERVAL = tmp1->INTEGERVAL - 1;
        break;
      case SEICCHAR:
        if ((int)tmp1->CHARVAL > 0) tmp1->CHARVAL = pred(tmp1->CHARVAL, 0, 255);
        else OUTOFBOUNDS = 1;
        break;
      case SEICENUM:
        if (tmp1->ENUMORDINAL > 0) tmp1->ENUMORDINAL = tmp1->ENUMORDINAL - 1;
        else OUTOFBOUNDS = 1;
        break;
      default:
        break;
    }
    if (OUTOFBOUNDS) {
      tmp2 = str_make(47, "Predecessor for argument would be out of bounds");
      ERRORFOREXPR(&tmp2, EXPR);
    }
    RESULT = EXPR;
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, SEOPRED, EXPR->TYPEPTR);
  return RESULT;
}

TSEXPRESSIONOBJ* EXOPSUCC(TSEXPRESSIONOBJ* EXPR) {
  TSEXPRESSIONOBJ* RESULT;
  PBoolean OUTOFBOUNDS;
  TSEIMMEDIATE* tmp1;
  PString tmp2;
  ENSUREORDINALEXPR(EXPR);
  if (EXISIMMEDIATE(EXPR)) {
    OUTOFBOUNDS = 0;
    tmp1 = &EXPR->IMMEDIATE;
    switch (tmp1->CLS) {
      case SEICBOOLEAN:
        if (!tmp1->BOOLEANVAL) tmp1->BOOLEANVAL = 1;
        else OUTOFBOUNDS = 1;
        break;
      case SEICINTEGER:
        tmp1->INTEGERVAL = tmp1->INTEGERVAL + 1;
        break;
      case SEICCHAR:
        if ((int)tmp1->CHARVAL < 255) tmp1->CHARVAL = succ(tmp1->CHARVAL, 0, 255);
        else OUTOFBOUNDS = 1;
        break;
      case SEICENUM:
        if (tmp1->ENUMORDINAL < tmp1->ENUMPTR->SIZE - 1) tmp1->ENUMORDINAL = tmp1->ENUMORDINAL + 1;
        else OUTOFBOUNDS = 1;
        break;
      default:
        break;
    }
    if (OUTOFBOUNDS) {
      tmp2 = str_make(45, "Successor for argument would be out of bounds");
      ERRORFOREXPR(&tmp2, EXPR);
    }
    RESULT = EXPR;
  }
  else RESULT = _EXOP_MAKEUNARY(EXPR, SEOSUCC, EXPR->TYPEPTR);
  return RESULT;
}

PString TYPENAME(TSDTYPEDEF* TYPEPTR) {
  PString RESULT;
  RESULT = DEEPTYPENAME(TYPEPTR, 0);
  return RESULT;
}

PBoolean ISUNTYPED(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = TYPEPTR == PNil;
  return RESULT;
}

PBoolean ISENUMTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCENUM);
  return RESULT;
}

PBoolean ISORDINALTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISBOOLEANTYPE(TYPEPTR) || ISINTEGERTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR) || ISENUMTYPE(TYPEPTR) || ISRANGETYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISRECORDTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCRECORD);
  return RESULT;
}

PBoolean ISARRAYTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCARRAY);
  return RESULT;
}

PBoolean ISPOINTERTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCPOINTER);
  return RESULT;
}

PBoolean ISSTRINGYTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISSTRINGTYPE(TYPEPTR) || ISCHARTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISSTRINGTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCSTRING);
  return RESULT;
}

PBoolean ISINTEGERTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCINTEGER);
  return RESULT;
}

PBoolean ISNUMERICTYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISINTEGERTYPE(TYPEPTR) || ISREALTYPE(TYPEPTR);
  return RESULT;
}

PBoolean ISFILETYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = _TYPEHASCLASS(TYPEPTR, SDTCFILE);
  return RESULT;
}

PBoolean ISGENERICFILETYPE(TSDTYPEDEF* TYPEPTR) {
  PBoolean RESULT;
  RESULT = ISFILETYPE(TYPEPTR) && TYPEPTR->FILEDEF.CLS == SDTFCNONE;
  return RESULT;
}

TSEXPRESSIONOBJ* EXCOERCE(TSEXPRESSIONOBJ* EXPR, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  PString tmp1;
  EXCLEARTMPVAR(EXPR);
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
  else if (ISPOINTERYTYPE(EXPR->TYPEPTR) && ISUNTYPEDPTRTYPE(TYPEPTR)) RESULT = EXTOUNTYPEDPTR(EXPR);
  else if (ISSETTYPE(EXPR->TYPEPTR) && ISSETTYPE(TYPEPTR)) RESULT = _EXCOERCESET(EXPR, TYPEPTR);
  else if (ISFILETYPE(EXPR->TYPEPTR) && ISGENERICFILETYPE(TYPEPTR)) RESULT = EXTOGENERICFILE(EXPR);
  else if (ISUNTYPED(TYPEPTR)) RESULT = EXPR;
  else {
    tmp1 = CONCAT(CpLenPtr, 22, "Cannot treat value as ", CpEnd | CpString, TYPENAME(TYPEPTR));
    ERRORFOREXPR(&tmp1, EXPR);
  }
  return RESULT;
}

TSEXPRESSIONOBJ* EXSUBRANGE(TSEXPRESSIONOBJ* PARENT, TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  PInteger ORDINAL;
  if (EXISIMMEDIATE(PARENT)) {
    ORDINAL = EXGETORDINAL(PARENT);
    if (ORDINAL < TYPEPTR->RANGEDEF.FIRST || ORDINAL > TYPEPTR->RANGEDEF.LAST) COMPILEERROR(CONCAT(CpLenPtr, 6, "Value ", CpString, EXDESCRIBE(PARENT), CpLenPtr, 19, " out of bounds for ", CpEnd | CpString, TYPENAME(TYPEPTR)));
    PARENT->TYPEPTR = TYPEPTR;
    RESULT = PARENT;
  }
  else {
    EXCLEARTMPVAR(PARENT);
    RESULT = _NEWEXPR(SECSUBRANGE);
    RESULT->SUBRANGEPARENT = PARENT;
    RESULT->TYPEPTR = TYPEPTR;
  }
  return RESULT;
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
      Dispose((void**)&PREVSTACK);
      STOP = 0;
    }
  } while (!STOP);
}

TSDTYPEDEF* PSTYPEDENOTER() {
  TSDTYPEDEF* RESULT;
  TSDEFENTRY* DEF;
  PBoolean ISPACKED;
  RESULT = PNil;
  ISPACKED = LEXER.TOKEN.ID == TKPACKED;
  SKIPTOKEN(TKPACKED);
  if (LEXER.TOKEN.ID == TKLPAREN) RESULT = PSENUMERATEDTYPE();
  else if (LEXER.TOKEN.ID == TKFILE) RESULT = PSFILETYPE();
  else if (LEXER.TOKEN.ID == TKSET) RESULT = PSSETTYPE();
  else if (LEXER.TOKEN.ID == TKRECORD) RESULT = PSRECORDTYPE(ISPACKED);
  else if (LEXER.TOKEN.ID == TKARRAY) RESULT = PSARRAYTYPE();
  else if (LEXER.TOKEN.ID == TKCARET) RESULT = PSPOINTERTYPE();
  else if (LEXER.TOKEN.ID == TKPROCEDURE) RESULT = PSPROCEDURETYPE();
  else if (LEXER.TOKEN.ID == TKFUNCTION) RESULT = PSFUNCTIONTYPE();
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) {
    DEF = FINDNAME(&LEXER.TOKEN.VALUE, 0);
    if (DEF == PNil) ;
    else if (DEF->CLS == SDCTYPE) RESULT = PSTYPEIDENTIFIER();
    else if (DEF->CLS == SDCCONSTANT) RESULT = PSRANGETYPE();
  }
  else if (LEXER.TOKEN.ID == TKINTEGER || TKSTRING <= LEXER.TOKEN.ID && LEXER.TOKEN.ID <= TKMINUS) RESULT = PSRANGETYPE();
  if (RESULT == PNil) COMPILEERROR(CONCAT(CpLenPtr, 29, "Expected type denoter, found ", CpEnd | CpString, LXTOKENSTR()));
  return RESULT;
}

TSEXPRESSIONOBJ* PSEXPRESSION() {
  TSEXPRESSIONOBJ* RESULT;
  TLXTOKENID OP;
  TSEXPRESSIONOBJ* EXPR;
  EXPR = PSSIMPLEEXPRESSION();
  while (ISOPRELATIONAL(LEXER.TOKEN)) {
    OP = LEXER.TOKEN.ID;
    READTOKEN();
    EXPR = PSBINARYOP(EXPR, PSSIMPLEEXPRESSION(), OP);
  }
  RESULT = EXPR;
  return RESULT;
}

TSEXPRESSIONOBJ* PSVARIABLE() {
  TSEXPRESSIONOBJ* RESULT;
  RESULT = _PSVARIABLEINTERNAL(0, 1);
  return RESULT;
}

TSSTATEMENTOBJ* PSSTATEMENT() {
  TSSTATEMENTOBJ* RESULT;
  if (LEXER.TOKEN.ID == TKSEMICOLON) RESULT = PSEMPTYSTATEMENT();
  else if (LEXER.TOKEN.ID == TKBEGIN) RESULT = PSSTATEMENTSEQUENCE();
  else if (LEXER.TOKEN.ID == TKIDENTIFIER) RESULT = PSIDENTIFIERSTATEMENT();
  else if (LEXER.TOKEN.ID == TKIF) RESULT = PSIFSTATEMENT();
  else if (LEXER.TOKEN.ID == TKREPEAT) RESULT = PSREPEATSTATEMENT();
  else if (LEXER.TOKEN.ID == TKWHILE) RESULT = PSWHILESTATEMENT();
  else if (LEXER.TOKEN.ID == TKFOR) RESULT = PSFORSTATEMENT();
  else if (LEXER.TOKEN.ID == TKWITH) RESULT = PSWITHSTATEMENT();
  else if (LEXER.TOKEN.ID == TKCASE) RESULT = PSCASESTATEMENT();
  else COMPILEERROR(CONCAT(CpLenPtr, 17, "Unexpected token ", CpEnd | CpString, LXTOKENSTR()));
  return RESULT;
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

TSEXPRESSIONOBJ* PSCONSTANTVALUE(TSDTYPEDEF* TYPEPTR) {
  TSEXPRESSIONOBJ* RESULT;
  if (ISARRAYTYPE(TYPEPTR)) RESULT = PSCONSTANTARRAY(TYPEPTR);
  else if (ISRECORDTYPE(TYPEPTR)) RESULT = PSCONSTANTRECORD(TYPEPTR);
  else RESULT = EXCOERCE(PSIMMEDIATE(), TYPEPTR);
  return RESULT;
}

void _CGC_OUTTYPEREFERENCE(TCGC_OBJ* THIS, TSDTYPEDEF* TYPEPTR) {
  TSDTYPEDEF* THETYPE;
  if (TYPEPTR == PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 4, "void");
  else if (TYPEPTR->CLS == SDTCPOINTER) {
    _CGC_OUTTYPEREFERENCE(THIS, TYPEPTR->POINTEDTYPEPTR);
    Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '*');
  }
  else if (TYPEPTR->CLS == SDTCBOOLEAN) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "PBoolean");
  else if (TYPEPTR->CLS == SDTCINTEGER) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "PInteger");
  else if (TYPEPTR->CLS == SDTCREAL) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "PReal");
  else if (TYPEPTR->CLS == SDTCCHAR) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "PChar");
  else if (TYPEPTR->CLS == SDTCSTRING) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "PString");
  else if (TYPEPTR->CLS == SDTCFILE) Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 5, "PFile");
  else if (TYPEPTR->CLS == SDTCENUM) {
    if (TYPEPTR->ENUMPTR->HASBEENDEFINED && cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &TYPEPTR->NAME);
    else Write(&THIS->OUTPUT, 1, RwpLenPtr, 9, "enum enum", RwpInt | RwpEnd, TYPEPTR->ENUMPTR->ID);
  }
  else if (TYPEPTR->CLS == SDTCRANGE) Write(&THIS->OUTPUT, 1, RwpString | RwpEnd, _CGC_GETRANGETYPE(TYPEPTR));
  else if (TYPEPTR->CLS == SDTCSET) _CGC_OUTSETTYPENAME(THIS, TYPEPTR);
  else if (TYPEPTR->CLS == SDTCRECORD) {
    if (TYPEPTR->RECPTR->HASBEENDEFINED && cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &TYPEPTR->NAME);
    else Write(&THIS->OUTPUT, 1, RwpLenPtr, 13, "struct record", RwpInt | RwpEnd, TYPEPTR->RECPTR->ID);
  }
  else if (TYPEPTR->CLS == SDTCARRAY) {
    THETYPE = TYPEPTR;
    while (ISARRAYTYPE(THETYPE)) THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
    _CGC_OUTTYPEREFERENCE(THIS, THETYPE);
    THETYPE = TYPEPTR;
    while (ISARRAYTYPE(THETYPE)) {
      Write(&THIS->OUTPUT, 1, RwpChar, '[', RwpInt, GETBOUNDEDTYPESIZE(THETYPE->ARRAYDEF.INDEXTYPEPTR), RwpChar | RwpEnd, ']');
      THETYPE = THETYPE->ARRAYDEF.VALUETYPEPTR;
    }
  }
  else INTERNALERROR(CONCAT(CpLenPtr, 30, "Error writing type reference: ", CpEnd | CpString, TYPENAME(TYPEPTR)));
}

void _CGC_OUTNAMEANDTYPE(TCGC_OBJ* THIS, const PString* NAME, TSDTYPEDEF* TYPEPTR) {
  PString SP;
  if (NAME->chr[1] != '*') SP = str_of(' ');
  else SP = str_make(0, "");
  if (TYPEPTR == PNil) Write(&THIS->OUTPUT, 1, RwpLenPtr, 4, "void", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCPOINTER) {
    _CGC_OUTTYPEREFERENCE(THIS, TYPEPTR->POINTEDTYPEPTR);
    Write(&THIS->OUTPUT, 1, RwpChar, '*', RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  }
  else if (TYPEPTR->ALIASFOR != PNil && cmp_str(CoNotEq, CpStringPtr, &TYPEPTR->NAME, CpLenPtr, 0, "")) Write(&THIS->OUTPUT, 1, RwpStringPtr, &TYPEPTR->NAME, RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCBOOLEAN) Write(&THIS->OUTPUT, 1, RwpLenPtr, 8, "PBoolean", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCINTEGER) Write(&THIS->OUTPUT, 1, RwpLenPtr, 8, "PInteger", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCREAL) Write(&THIS->OUTPUT, 1, RwpLenPtr, 5, "PReal", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCCHAR) Write(&THIS->OUTPUT, 1, RwpLenPtr, 5, "PChar", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCSTRING) Write(&THIS->OUTPUT, 1, RwpLenPtr, 7, "PString", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCFILE) Write(&THIS->OUTPUT, 1, RwpLenPtr, 5, "PFile", RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCENUM) _CGC_OUTNAMEANDENUM(THIS, NAME, TYPEPTR->ENUMPTR);
  else if (TYPEPTR->CLS == SDTCRANGE) Write(&THIS->OUTPUT, 1, RwpString, _CGC_GETRANGETYPE(TYPEPTR), RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  else if (TYPEPTR->CLS == SDTCSET) {
    _CGC_OUTSETTYPENAME(THIS, TYPEPTR);
    Write(&THIS->OUTPUT, 1, RwpStringPtr, &SP, RwpStringPtr | RwpEnd, NAME);
  }
  else if (TYPEPTR->CLS == SDTCRECORD) _CGC_OUTNAMEANDRECORD(THIS, NAME, TYPEPTR->RECPTR);
  else if (TYPEPTR->CLS == SDTCARRAY) _CGC_OUTNAMEANDARRAY(THIS, NAME, TYPEPTR);
  else if (TYPEPTR->CLS == SDTCFUNCTION) _CGC_OUTNAMEANDFUNCTION(THIS, NAME, TYPEPTR);
  else INTERNALERROR(CONCAT(CpLenPtr, 29, "Error writing name and type: ", CpStringPtr, NAME, CpLenPtr, 2, ", ", CpEnd | CpString, TYPENAME(TYPEPTR)));
}

void _CGC_OUTEXPRESSION(TCGC_OBJ* THIS, TSEXPRESSIONOBJ* EXPR) {
  switch (EXPR->CLS) {
    case SECIMMEDIATE:
      _CGC_OUTEXIMMEDIATE(THIS, EXPR);
      break;
    case SECARRAYVALUE:
      _CGC_OUTEXARRAYVALUE(THIS, EXPR);
      break;
    case SECRECORDVALUE:
      _CGC_OUTEXRECORDVALUE(THIS, EXPR);
      break;
    case SECSETVALUE:
      _CGC_OUTEXSETVALUE(THIS, EXPR);
      break;
    case SECTOSTRING:
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 7, "str_of(");
      _CGC_OUTEXPRESSION(THIS, EXPR->TOSTRPARENT);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ')');
      break;
    case SECTOREAL:
      Write(&THIS->OUTPUT, 1, RwpLenPtr | RwpEnd, 8, "(double)");
      _CGC_OUTEXPRESSION(THIS, EXPR->TOREALPARENT);
      break;
    case SECTOUNTYPEDPTR:
      _CGC_OUTEXPRESSION(THIS, EXPR->TOUNTYPEDPTRPARENT);
      break;
    case SECTOGENERICFILE:
      _CGC_OUTEXPRESSION(THIS, EXPR->TOGENERICFILEPARENT);
      break;
    case SECWITHTMPVAR:
      _CGC_OUTEXWITHTMPVAR(THIS, EXPR);
      break;
    case SECSUBRANGE:
      _CGC_OUTEXSUBRANGE(THIS, EXPR);
      break;
    case SECVARIABLE:
      _CGC_OUTEXVARIABLE(THIS, EXPR);
      break;
    case SECFIELD:
      _CGC_OUTEXFIELDACCESS(THIS, EXPR);
      break;
    case SECARRAY:
      _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->ARRAYEXPR, EXPR);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '[');
      _CGC_OUTEXARRAYINDEX(THIS, EXPR->ARRAYINDEX, EXPR->ARRAYEXPR->TYPEPTR);
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, ']');
      break;
    case SECPOINTER:
      Write(&THIS->OUTPUT, 1, RwpChar | RwpEnd, '*');
      _CGC_OUTEXPRESSIONPARENS(THIS, EXPR->POINTEREXPR, EXPR);
      break;
    case SECADDRESS:
      _CGC_OUTEXADDRESS(THIS, EXPR->ADDRESSEXPR);
      break;
    case SECSTRINGCHAR:
      _CGC_OUTEXSTRINGCHAR(THIS, EXPR);
      break;
    case SECFNREF:
      Write(&THIS->OUTPUT, 1, RwpStringPtr | RwpEnd, &EXPR->FNPTR->EXTERNALNAME);
      break;
    case SECFNCALL:
      _CGC_OUTEXFUNCTIONCALL(THIS, EXPR);
      break;
    case SECSIZEOF:
      _CGC_OUTEXSIZEOF(THIS, EXPR);
      break;
    case SECCONVERTTOSTR:
      _CGC_OUTEXCONVERTTOSTR(THIS, EXPR);
      break;
    case SECCONVERTTOVAL:
      _CGC_OUTEXCONVERTTOVAL(THIS, EXPR);
      break;
    case SECREAD:
      _CGC_OUTEXREAD(THIS, EXPR);
      break;
    case SECWRITE:
      _CGC_OUTEXWRITE(THIS, EXPR);
      break;
    case SECUNARYOP:
      _CGC_OUTEXUNARYOP(THIS, EXPR);
      break;
    case SECBINARYOP:
      _CGC_OUTEXBINARYOP(THIS, EXPR);
      break;
    default:
      break;
  }
}

void _CGC_OUTBODY(TCGC_OBJ* THIS, TSSSEQUENCEENTRY* BODY) {
  TSSSEQUENCEENTRY* ELEM;
  ELEM = BODY;
  while (ELEM != PNil) {
    _CGC_OUTSTATEMENT(THIS, ELEM->STATEMENT);
    ELEM = ELEM->NEXT;
  }
}

void _CGC_OUTSTATEMENT(TCGC_OBJ* THIS, TSSTATEMENTOBJ* STMT) {
  switch (STMT->CLS) {
    case SSCEMPTY:
      _CGC_OUTSTEMPTY(THIS);
      break;
    case SSCSEQUENCE:
      _CGC_OUTSTSEQUENCE(THIS, STMT);
      break;
    case SSCASSIGN:
      _CGC_OUTSTASSIGN(THIS, STMT);
      break;
    case SSCPROCCALL:
      _CGC_OUTSTPROCCALL(THIS, STMT);
      break;
    case SSCIF:
      _CGC_OUTSTIF(THIS, STMT);
      break;
    case SSCREPEAT:
      _CGC_OUTSTREPEAT(THIS, STMT);
      break;
    case SSCWHILE:
      _CGC_OUTSTWHILE(THIS, STMT);
      break;
    case SSCFOR:
      _CGC_OUTSTFOR(THIS, STMT);
      break;
    case SSCWITH:
      _CGC_OUTSTWITH(THIS, STMT);
      break;
    case SSCCASE:
      _CGC_OUTSTCASE(THIS, STMT);
      break;
    default:
      break;
  }
}

void _CGC_OUTLOCALDEFINITIONS(TCGC_OBJ* THIS, TSDEFENTRY* FIRSTDEF) {
  _CGC_OUTGLOBALDEFINITIONS(THIS, FIRSTDEF);
  _CGC_OUTDEFINEDTEMPORARYVARIABLES(THIS, FIRSTDEF);
}

void pascual_main() {
  LXRESET();
  CG_C_INIT(&CG);
  PARSECMDLINE();
  CREATEGLOBALDEFINITIONS();
  CG->GENERATE(CG, PARSEPROGRAM());
  Close(&LEXER.INPUT.SRC, 1);
}
