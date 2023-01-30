/* Program: COMPILER */
#include "pascual.h"
typedef enum enum1 { TKUNKNOWN, TKEOF, TKCOMMENT, TKIDENTIFIER, TKNUMBER, TKSTRING, TKPLUS, TKMINUS, TKASTERISK, TKSLASH, TKEQUALS, TKLESSTHAN, TKMORETHAN, TKLBRACKET, TKRBRACKET, TKDOT, TKCOMMA, TKCOLON, TKSEMICOLON, TKCARET, TKLPAREN, TKRPAREN, TKNOTEQUALS, TKLESSOREQUALS, TKMOREOREQUALS, TKASSIGN, TKRANGE, TKAND, TKFALSE, TKTRUE, TKARRAY, TKBEGIN, TKCASE, TKCONST, TKDIV, TKDO, TKDOWNTO, TKELSE, TKEND, TKFILE, TKFOR, TKFORWARD, TKFUNCTION, TKGOTO, TKIF, TKIN, TKLABEL, TKMOD, TKNIL, TKNOT, TKOF, TKOR, TKPACKED, TKPROCEDURE, TKPROGRAM, TKRECORD, TKREPEAT, TKSET, TKTHEN, TKTO, TKTYPE, TKUNTIL, TKVAR, TKWHILE, TKWITH} TLXTOKENID;
typedef struct record1 { int ROW; int COL; } TLXPOS;
typedef struct record2 { TLXTOKENID ID; PString VALUE; TLXPOS POS; } TLXTOKEN;
typedef struct record3 { PFile SRC; PString NAME; TLXPOS POS; } TLXINPUTFILE;
const char* EnumValues1[] = { "TKUNKNOWN", "TKEOF", "TKCOMMENT", "TKIDENTIFIER", "TKNUMBER", "TKSTRING", "TKPLUS", "TKMINUS", "TKASTERISK", "TKSLASH", "TKEQUALS", "TKLESSTHAN", "TKMORETHAN", "TKLBRACKET", "TKRBRACKET", "TKDOT", "TKCOMMA", "TKCOLON", "TKSEMICOLON", "TKCARET", "TKLPAREN", "TKRPAREN", "TKNOTEQUALS", "TKLESSOREQUALS", "TKMOREOREQUALS", "TKASSIGN", "TKRANGE", "TKAND", "TKFALSE", "TKTRUE", "TKARRAY", "TKBEGIN", "TKCASE", "TKCONST", "TKDIV", "TKDO", "TKDOWNTO", "TKELSE", "TKEND", "TKFILE", "TKFOR", "TKFORWARD", "TKFUNCTION", "TKGOTO", "TKIF", "TKIN", "TKLABEL", "TKMOD", "TKNIL", "TKNOT", "TKOF", "TKOR", "TKPACKED", "TKPROCEDURE", "TKPROGRAM", "TKRECORD", "TKREPEAT", "TKSET", "TKTHEN", "TKTO", "TKTYPE", "TKUNTIL", "TKVAR", "TKWHILE", "TKWITH" };
struct record5 { PString LINE; TLXTOKEN TOKEN; TLXINPUTFILE INPUT; struct record4 { PBoolean EXISTS; TLXINPUTFILE INPUT; } PREV; } LEXER;
PString LXTOKENNAME(TLXTOKENID ID) {
PString return_LXTOKENNAME;
PString NAME;
NAME = to_str_e(ID, EnumValues1);
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
{
write_s(&STDERR, MSG);
write_s(&STDERR, LXWHERESTR());
writeln(&STDERR);
}
HALT(1);
}
PBoolean LXISALPHA(char CHR) {
PBoolean return_LXISALPHA;
return_LXISALPHA = ((cmp_cc(CHR, 'a') >= 0) && (cmp_cc(CHR, 'z') <= 0)) || ((cmp_cc(CHR, 'A') >= 0) && (cmp_cc(CHR, 'Z') <= 0));
return return_LXISALPHA;
}
PBoolean LXISDIGIT(char CHR) {
PBoolean return_LXISDIGIT;
return_LXISDIGIT = (cmp_cc(CHR, '0') >= 0) && (cmp_cc(CHR, '9') <= 0);
return return_LXISDIGIT;
}
PBoolean LXISALPHANUM(char CHR) {
PBoolean return_LXISALPHANUM;
return_LXISALPHANUM = LXISALPHA(CHR) || LXISDIGIT(CHR);
return return_LXISALPHANUM;
}
PBoolean LXISIDENTIFIERFIRST(char CHR) {
PBoolean return_LXISIDENTIFIERFIRST;
return_LXISIDENTIFIERFIRST = LXISALPHA(CHR) || (cmp_cc(CHR, '_') == 0);
return return_LXISIDENTIFIERFIRST;
}
PBoolean LXISIDENTIFIERCHAR(char CHR) {
PBoolean return_LXISIDENTIFIERCHAR;
return_LXISIDENTIFIERCHAR = LXISALPHANUM(CHR) || (cmp_cc(CHR, '_') == 0);
return return_LXISIDENTIFIERCHAR;
}
PBoolean LXISTOKENWAITING() {
PBoolean return_LXISTOKENWAITING;
do {
while ((LENGTH(LEXER.LINE) == 0) && !EOF(&LEXER.INPUT.SRC)) {
LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
LEXER.INPUT.POS.COL = 1;
{
read_s(&LEXER.INPUT.SRC, &LEXER.LINE);
readln(&LEXER.INPUT.SRC);
}
}
while ((LENGTH(LEXER.LINE) > 0) && (cmp_cc(LEXER.LINE.chr[1], ' ') == 0)) {
LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + 1;
DELETE(&LEXER.LINE, 1, 1);
}
} while (!(EOF(&LEXER.INPUT.SRC) || (LENGTH(LEXER.LINE) > 0)));
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
while ((POS < LENGTH(LEXER.LINE)) && INTOKEN) {
CHR = LEXER.LINE.chr[POS + 1];
INTOKEN = LXISIDENTIFIERCHAR(CHR);
if (INTOKEN) POS = POS + 1;
}
LXGETSYMBOL(TKIDENTIFIER, POS);
{
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
}
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
while ((POS < LENGTH(LEXER.LINE)) && INTOKEN) {
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
if (cmp_cc(CHR, '\'') == 0) {
if ((LENGTH(LEXER.LINE) > POS + 1) && (cmp_cc(LEXER.LINE.chr[POS + 1], '\'') == 0)) POS = POS + 1;
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
if (cmp_cc(LEXER.LINE.chr[1], '{') == 0) DELIMITERLENGTH = 1;
 else DELIMITERLENGTH = 2;
LXGETSYMBOL(TKCOMMENT, DELIMITERLENGTH);
do {
while (cmp_ss(LEXER.LINE, str_make(0, "")) == 0) {
COMMENT = cat_sc(COMMENT, ' ');
{
read_s(&LEXER.INPUT.SRC, &LEXER.LINE);
readln(&LEXER.INPUT.SRC);
}
LEXER.INPUT.POS.ROW = LEXER.INPUT.POS.ROW + 1;
LEXER.INPUT.POS.COL = 1;
}
if (DELIMITERLENGTH == 1) DONE = cmp_cc(LEXER.LINE.chr[1], '}') == 0;
 else DONE = (cmp_cc(LEXER.LINE.chr[1], '*') == 0) && (cmp_cc(LEXER.LINE.chr[2], ')') == 0);
if (!DONE) {
COMMENT = cat_sc(COMMENT, LEXER.LINE.chr[1]);
DELETE(&LEXER.LINE, 1, 1);
LEXER.INPUT.POS.COL = LEXER.INPUT.POS.COL + 1;
}
} while (!(DONE));
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
case '\'': LXGETSTRING();
break;
case '+': LXGETSYMBOL(TKPLUS, 1);
break;
case '-': LXGETSYMBOL(TKMINUS, 1);
break;
case '*': LXGETSYMBOL(TKASTERISK, 1);
break;
case '/': LXGETSYMBOL(TKSLASH, 1);
break;
case '=': LXGETSYMBOL(TKEQUALS, 1);
break;
case '<': LXGETSYMBOL(TKLESSTHAN, 1);
break;
case '>': LXGETSYMBOL(TKMORETHAN, 1);
break;
case '[': LXGETSYMBOL(TKLBRACKET, 1);
break;
case ']': LXGETSYMBOL(TKRBRACKET, 1);
break;
case '.': LXGETSYMBOL(TKDOT, 1);
break;
case ',': LXGETSYMBOL(TKCOMMA, 1);
break;
case ':': LXGETSYMBOL(TKCOLON, 1);
break;
case ';': LXGETSYMBOL(TKSEMICOLON, 1);
break;
case '^': LXGETSYMBOL(TKCARET, 1);
break;
case '(': LXGETSYMBOL(TKLPAREN, 1);
break;
case ')': LXGETSYMBOL(TKRPAREN, 1);
break;
case '{': LXGETCOMMENT();
break;
default: COMPILEERROR(cat_ss(str_make(17, "Could not parse ["), LEXER.LINE));
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
typedef struct record9 *TPSTYPEINDEX;
typedef struct record10 *TPSENUMINDEX;
typedef struct record12 *TPSRECORDINDEX;
typedef struct record13 *TPSARRAYINDEX;
typedef struct record14 *TPSCONSTANTINDEX;
typedef struct record15 *TPSVARIABLEINDEX;
typedef struct record16 *TPSFUNCTIONINDEX;
typedef struct record17 *TPSWITHVARINDEX;
typedef struct record18 *TPSNAMEINDEX;
typedef enum enum2 { TECVALUE, TECFUNCTION, TECSTATEMENT} TPSEXPRESSIONCLASS;
typedef struct record6 { PString VALUE; PBoolean ISCONSTANT; TPSEXPRESSIONCLASS CLS; union { struct { struct record9 *TYPEINDEX; }; struct { struct record16 *FUNCTIONINDEX; }; }; } TPSEXPRESSION;
typedef struct record7 { int SIZE; TPSEXPRESSION ARGS[1 + 4 - 1]; } TPSEXPRESSIONFNARGS;
typedef struct record8 { PString NAME; } TPSIDENTIFIER;
typedef enum enum3 { TTCBOOLEAN, TTCINTEGER, TTCCHAR, TTCSTRING, TTCTEXT, TTCENUM, TTCRECORD, TTCARRAY, TTCPOINTER, TTCNIL, TTCPOINTERUNKNOWN} TPSTYPECLASS;
typedef struct record9 { PString NAME; struct record9 *ALIASFOR; TPSTYPECLASS CLS; union { struct { struct record10 *ENUMINDEX; }; struct { struct record12 *RECORDINDEX; }; struct { struct record13 *ARRAYINDEX; }; struct { struct record9 *POINTEDTYPEINDEX; }; struct { PString *TARGETNAME; }; }; } TPSTYPE;
typedef struct record10 { int SIZE; PString VALUES[1 + 128 - 1]; int ID; PBoolean HASBEENDEFINED; } TPSENUMDEF;
typedef struct record11 { PString NAME; struct record9 *TYPEINDEX; } TPSRECORDFIELD;
typedef struct record12 { int SIZE; TPSRECORDFIELD FIELDS[1 + 16 - 1]; int NUMVARIANTS; int VARIANTBOUNDS[1 + 16 - 1]; int ID; PBoolean HASBEENDEFINED; } TPSRECORDDEF;
typedef struct record13 { PString LOWBOUND; PString HIGHBOUND; struct record9 *TYPEINDEX; } TPSARRAYDEF;
typedef struct record14 { PString NAME; TLXTOKEN REPLACEMENT; } TPSCONSTANT;
typedef struct record15 { PString NAME; struct record9 *TYPEINDEX; PBoolean ISREFERENCE; PBoolean ISCONSTANT; } TPSVARIABLE;
typedef struct record16 { PString NAME; int ARGCOUNT; TPSVARIABLE ARGS[1 + 4 - 1]; struct record9 *RETURNTYPEINDEX; PBoolean ISDECLARATION; } TPSFUNCTION;
typedef struct record17 { struct record15 *VARIABLEINDEX; } TPSWITHVAR;
typedef enum enum4 { TNCTYPE, TNCVARIABLE, TNCENUMVALUE, TNCFUNCTION, TNCSPECIALFUNCTION} TPSNAMECLASS;
typedef enum enum5 { TSFREAD, TSFREADLN, TSFWRITE, TSFWRITELN, TSFSTR, TSFNEW, TSFDISPOSE} TPSSPECIALFUNCTION;
typedef struct record18 { PString NAME; TPSNAMECLASS CLS; union { struct { struct record9 *TYPEINDEX; }; struct { struct record15 *VARIABLEINDEX; }; struct { struct record9 *ENUMTYPEINDEX; int ORDINAL; }; struct { struct record16 *FUNCTIONINDEX; }; struct { TPSSPECIALFUNCTION SPECIALFUNCTION; }; }; } TPSNAME;
typedef struct record19 *TPSDEFPTR;
typedef enum enum6 { TDCNAME, TDCTYPE, TDCENUM, TDCRECORD, TDCARRAY, TDCCONSTANT, TDCVARIABLE, TDCFUNCTION, TDCWITHVAR, TDCSCOPEBOUNDARY} TPSDEFCLASS;
typedef struct record19 { struct record19 *PREV; struct record19 *NEXT; TPSDEFCLASS CLS; union { struct { struct record18 *NAMEINDEX; }; struct { struct record9 *TYPEINDEX; }; struct { struct record10 *ENUMINDEX; }; struct { struct record12 *RECORDINDEX; }; struct { struct record13 *ARRAYINDEX; }; struct { struct record14 *CONSTANTINDEX; }; struct { struct record15 *VARIABLEINDEX; }; struct { struct record16 *FUNCTIONINDEX; }; struct { struct record17 *WITHVARINDEX; }; struct { PBoolean TEMPORARYSCOPE; }; }; } TPSDEFENTRY;
typedef struct record20 { struct record19 *LATEST; int COUNTER; } TPSDEFS;
const char* EnumValues2[] = { "TECVALUE", "TECFUNCTION", "TECSTATEMENT" };
const char* EnumValues3[] = { "TTCBOOLEAN", "TTCINTEGER", "TTCCHAR", "TTCSTRING", "TTCTEXT", "TTCENUM", "TTCRECORD", "TTCARRAY", "TTCPOINTER", "TTCNIL", "TTCPOINTERUNKNOWN" };
const char* EnumValues4[] = { "TNCTYPE", "TNCVARIABLE", "TNCENUMVALUE", "TNCFUNCTION", "TNCSPECIALFUNCTION" };
const char* EnumValues5[] = { "TSFREAD", "TSFREADLN", "TSFWRITE", "TSFWRITELN", "TSFSTR", "TSFNEW", "TSFDISPOSE" };
const char* EnumValues6[] = { "TDCNAME", "TDCTYPE", "TDCENUM", "TDCRECORD", "TDCARRAY", "TDCCONSTANT", "TDCVARIABLE", "TDCFUNCTION", "TDCWITHVAR", "TDCSCOPEBOUNDARY" };
TPSDEFS DEFS;
struct record21 { struct record9 *PTNIL; struct record9 *PTBOOLEAN; struct record9 *PTINTEGER; struct record9 *PTCHAR; struct record9 *PTSTRING; struct record9 *PTTEXT; } PRIMITIVETYPES;
int DEFCOUNTER() {
int return_DEFCOUNTER;
DEFS.COUNTER = DEFS.COUNTER + 1;
return_DEFCOUNTER = DEFS.COUNTER;
return return_DEFCOUNTER;
}
void INITDEFS() {
DEFS.LATEST = ((void*)0);
DEFS.COUNTER = 0;
}
struct record19 *_NEWDEF(TPSDEFCLASS CLS) {
struct record19 *return__NEWDEF;
struct record19 *DEF;
DEF = malloc(sizeof(struct record19));
(*(DEF)).PREV = ((void*)0);
(*(DEF)).NEXT = ((void*)0);
(*(DEF)).CLS = CLS;
switch (CLS) {
case TDCNAME: (*(DEF)).NAMEINDEX = malloc(sizeof(struct record18));
break;
case TDCTYPE: (*(DEF)).TYPEINDEX = malloc(sizeof(struct record9));
break;
case TDCENUM: (*(DEF)).ENUMINDEX = malloc(sizeof(struct record10));
break;
case TDCRECORD: (*(DEF)).RECORDINDEX = malloc(sizeof(struct record12));
break;
case TDCARRAY: (*(DEF)).ARRAYINDEX = malloc(sizeof(struct record13));
break;
case TDCCONSTANT: (*(DEF)).CONSTANTINDEX = malloc(sizeof(struct record14));
break;
case TDCVARIABLE: (*(DEF)).VARIABLEINDEX = malloc(sizeof(struct record15));
break;
case TDCFUNCTION: (*(DEF)).FUNCTIONINDEX = malloc(sizeof(struct record16));
break;
case TDCWITHVAR: (*(DEF)).WITHVARINDEX = malloc(sizeof(struct record17));
break;
case TDCSCOPEBOUNDARY: (*(DEF)).TEMPORARYSCOPE = 0;
break;
default: break;
}
return__NEWDEF = DEF;
return return__NEWDEF;
}
void _DISPOSEDEF(struct record19 *DEF) {
switch ((*(DEF)).CLS) {
case TDCNAME: free((*(DEF)).NAMEINDEX);
break;
case TDCTYPE: free((*(DEF)).TYPEINDEX);
break;
case TDCENUM: free((*(DEF)).ENUMINDEX);
break;
case TDCRECORD: free((*(DEF)).RECORDINDEX);
break;
case TDCARRAY: free((*(DEF)).ARRAYINDEX);
break;
case TDCCONSTANT: free((*(DEF)).CONSTANTINDEX);
break;
case TDCVARIABLE: free((*(DEF)).VARIABLEINDEX);
break;
case TDCFUNCTION: free((*(DEF)).FUNCTIONINDEX);
break;
case TDCWITHVAR: free((*(DEF)).WITHVARINDEX);
break;
default: break;
}
free(DEF);
}
struct record19 *_ADDDEF(TPSDEFCLASS CLS) {
struct record19 *return__ADDDEF;
struct record19 *DEF;
DEF = _NEWDEF(CLS);
(*(DEF)).PREV = DEFS.LATEST;
if (DEFS.LATEST != ((void*)0)) (*(DEFS.LATEST)).NEXT = DEF;
DEFS.LATEST = DEF;
return__ADDDEF = DEF;
return return__ADDDEF;
}
PBoolean _DELETEDEF(TPSDEFENTRY *DELETEDDEF) {
PBoolean return__DELETEDEF;
if (DEFS.LATEST == ((void*)0)) return__DELETEDEF = 0;
 else {
*DELETEDDEF = *(DEFS.LATEST);
_DISPOSEDEF(DEFS.LATEST);
DEFS.LATEST = (*DELETEDDEF).PREV;
return__DELETEDEF = 1;
}
return return__DELETEDEF;
}
void _STARTSCOPE(PBoolean TEMPORARY) {
struct record19 *DEF;
DEF = _ADDDEF(TDCSCOPEBOUNDARY);
(*(DEF)).TEMPORARYSCOPE = TEMPORARY;
}
void _CLOSESCOPE(PBoolean TEMPORARY) {
TPSDEFENTRY DELETEDDEF;
PBoolean DELETED;
do {
DELETED = _DELETEDEF(&DELETEDDEF);
} while (!(!DELETED || ((DELETEDDEF.CLS == TDCSCOPEBOUNDARY) && (TEMPORARY || !DELETEDDEF.TEMPORARYSCOPE))));
}
void STARTLOCALSCOPE() {
_STARTSCOPE(0);
}
void CLOSELOCALSCOPE() {
_CLOSESCOPE(0);
}
void STARTTEMPORARYSCOPE() {
_STARTSCOPE(1);
}
void CLOSETEMPORARYSCOPE() {
_CLOSESCOPE(1);
}
struct record18 *_FINDNAME(PString NAME, PBoolean REQUIRED, PBoolean FROMLOCALSCOPE) {
struct record18 *return__FINDNAME;
struct record19 *DEF;
struct record18 *RET;
RET = ((void*)0);
DEF = DEFS.LATEST;
while ((RET == ((void*)0)) && (DEF != ((void*)0)) && (!FROMLOCALSCOPE || ((*(DEF)).CLS != TDCSCOPEBOUNDARY))) {
if (((*(DEF)).CLS == TDCNAME) && (cmp_ss(NAME, (*((*(DEF)).NAMEINDEX)).NAME) == 0)) RET = (*(DEF)).NAMEINDEX;
DEF = (*(DEF)).PREV;
}
if (REQUIRED && (RET == ((void*)0))) COMPILEERROR(cat_ss(str_make(20, "Unknown identifier: "), NAME));
return__FINDNAME = RET;
return return__FINDNAME;
}
struct record18 *_CHECKNAMECLASS(struct record18 *NAMEINDEX, TPSNAMECLASS CLS) {
struct record18 *return__CHECKNAMECLASS;
if ((NAMEINDEX != ((void*)0)) && ((*(NAMEINDEX)).CLS != CLS)) switch ((*(NAMEINDEX)).CLS) {
case TNCTYPE: COMPILEERROR(cat_ss(str_make(12, "Not a type: "), (*(NAMEINDEX)).NAME));
break;
case TNCVARIABLE: COMPILEERROR(cat_ss(str_make(16, "Not a variable: "), (*(NAMEINDEX)).NAME));
break;
case TNCENUMVALUE: COMPILEERROR(cat_ss(str_make(26, "Not an enumeration value: "), (*(NAMEINDEX)).NAME));
break;
case TNCFUNCTION: COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), (*(NAMEINDEX)).NAME));
break;
case TNCSPECIALFUNCTION: COMPILEERROR(cat_ss(str_make(29, "Not a procedure or function: "), (*(NAMEINDEX)).NAME));
break;
default: COMPILEERROR(str_make(35, "Internal error: name class mismatch"));
break;
}
return__CHECKNAMECLASS = NAMEINDEX;
return return__CHECKNAMECLASS;
}
struct record18 *FINDNAMEINLOCALSCOPE(PString NAME, PBoolean REQUIRED) {
struct record18 *return_FINDNAMEINLOCALSCOPE;
return_FINDNAMEINLOCALSCOPE = _FINDNAME(NAME, REQUIRED, 1);
return return_FINDNAMEINLOCALSCOPE;
}
struct record18 *FINDNAMEOFCLASSINLOCALSCOPE(PString NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
struct record18 *return_FINDNAMEOFCLASSINLOCALSCOPE;
return_FINDNAMEOFCLASSINLOCALSCOPE = _CHECKNAMECLASS(FINDNAMEINLOCALSCOPE(NAME, REQUIRED), CLS);
return return_FINDNAMEOFCLASSINLOCALSCOPE;
}
struct record18 *FINDNAME(PString NAME, PBoolean REQUIRED) {
struct record18 *return_FINDNAME;
return_FINDNAME = _FINDNAME(NAME, REQUIRED, 0);
return return_FINDNAME;
}
struct record18 *FINDNAMEOFCLASS(PString NAME, TPSNAMECLASS CLS, PBoolean REQUIRED) {
struct record18 *return_FINDNAMEOFCLASS;
return_FINDNAMEOFCLASS = _CHECKNAMECLASS(FINDNAME(NAME, REQUIRED), CLS);
return return_FINDNAMEOFCLASS;
}
struct record18 *_ADDNAME(PString NAME, TPSNAMECLASS CLS) {
struct record18 *return__ADDNAME;
struct record18 *POS;
if (FINDNAMEINLOCALSCOPE(NAME, 0) != ((void*)0)) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), NAME), str_make(16, " already defined")));
POS = (*(_ADDDEF(TDCNAME))).NAMEINDEX;
(*(POS)).NAME = NAME;
(*(POS)).CLS = CLS;
return__ADDNAME = POS;
return return__ADDNAME;
}
struct record18 *ADDTYPENAME(PString NAME, struct record9 *IDX) {
struct record18 *return_ADDTYPENAME;
struct record18 *DEF;
DEF = _ADDNAME(NAME, TNCTYPE);
(*(DEF)).TYPEINDEX = IDX;
return_ADDTYPENAME = DEF;
return return_ADDTYPENAME;
}
struct record18 *ADDVARIABLENAME(PString NAME, struct record15 *IDX) {
struct record18 *return_ADDVARIABLENAME;
struct record18 *DEF;
DEF = _ADDNAME(NAME, TNCVARIABLE);
(*(DEF)).VARIABLEINDEX = IDX;
return_ADDVARIABLENAME = DEF;
return return_ADDVARIABLENAME;
}
struct record18 *ADDFUNCTIONNAME(PString NAME, struct record16 *IDX) {
struct record18 *return_ADDFUNCTIONNAME;
struct record18 *DEF;
DEF = _ADDNAME(NAME, TNCFUNCTION);
(*(DEF)).FUNCTIONINDEX = IDX;
return_ADDFUNCTIONNAME = DEF;
return return_ADDFUNCTIONNAME;
}
struct record18 *ADDENUMVALUENAME(int ORDINAL, struct record9 *TYPEIDX) {
struct record18 *return_ADDENUMVALUENAME;
struct record18 *DEF;
DEF = _ADDNAME((*((*(TYPEIDX)).ENUMINDEX)).VALUES[(ORDINAL) - 1], TNCENUMVALUE);
(*(DEF)).ENUMTYPEINDEX = TYPEIDX;
(*(DEF)).ORDINAL = ORDINAL;
return_ADDENUMVALUENAME = DEF;
return return_ADDENUMVALUENAME;
}
struct record18 *ADDSPECIALFUNCTIONNAME(PString NAME, TPSSPECIALFUNCTION FN) {
struct record18 *return_ADDSPECIALFUNCTIONNAME;
struct record18 *DEF;
DEF = _ADDNAME(NAME, TNCSPECIALFUNCTION);
(*(DEF)).SPECIALFUNCTION = FN;
return_ADDSPECIALFUNCTIONNAME = DEF;
return return_ADDSPECIALFUNCTIONNAME;
}
PString DEEPTYPENAME(struct record9 *TYPEINDEX, PBoolean USEORIGINAL) {
PString return_DEEPTYPENAME;
TPSTYPE TYP;
PString RET;
int POS;
do {
TYP = *(TYPEINDEX);
TYPEINDEX = TYP.ALIASFOR;
} while (!(!USEORIGINAL || (TYPEINDEX == ((void*)0))));
if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) return_DEEPTYPENAME = TYP.NAME;
 else if (TYP.CLS == TTCENUM) {
RET = str_of('(');
{
int first = 1;
int last = (*(TYP.ENUMINDEX)).SIZE;
if (first <= last) {
POS = first;
while (1) {
{
if (POS != 1) RET = cat_sc(RET, ',');
RET = cat_ss(RET, (*(TYP.ENUMINDEX)).VALUES[(POS) - 1]);
}
if (POS == last) break;
++POS;
}
}
}
return_DEEPTYPENAME = cat_sc(RET, ')');
}
 else if (TYP.CLS == TTCRECORD) {
RET = str_make(7, "record ");
{
int first = 1;
int last = (*(TYP.RECORDINDEX)).SIZE;
if (first <= last) {
POS = first;
while (1) {
{
if (POS != 1) RET = cat_sc(RET, ',');
RET = cat_ss(RET, DEEPTYPENAME((*(TYP.RECORDINDEX)).FIELDS[(POS) - 1].TYPEINDEX, 1));
RET = cat_ss(cat_sc(RET, ':'), (*(TYP.RECORDINDEX)).FIELDS[(POS) - 1].NAME);
}
if (POS == last) break;
++POS;
}
}
}
return_DEEPTYPENAME = cat_ss(RET, str_make(4, " end"));
}
 else if (TYP.CLS == TTCARRAY) {
RET = cat_ss(str_make(7, "array ["), (*(TYP.ARRAYINDEX)).LOWBOUND);
RET = cat_ss(cat_ss(RET, str_make(2, "..")), (*(TYP.ARRAYINDEX)).HIGHBOUND);
RET = cat_ss(cat_ss(RET, str_make(5, "] of ")), DEEPTYPENAME((*(TYP.ARRAYINDEX)).TYPEINDEX, 1));
return_DEEPTYPENAME = RET;
}
 else if (TYP.CLS == TTCPOINTER) return_DEEPTYPENAME = cat_cs('^', DEEPTYPENAME(TYP.POINTEDTYPEINDEX, 1));
 else {
RET = to_str_e(TYP.CLS, EnumValues3);
COMPILEERROR(cat_ss(str_make(37, "Could not get name for type of class "), RET));
}
return return_DEEPTYPENAME;
}
PString TYPENAME(struct record9 *TYPEINDEX) {
PString return_TYPENAME;
if (TYPEINDEX == ((void*)0)) return_TYPENAME = str_make(6, "(none)");
else return_TYPENAME = DEEPTYPENAME(TYPEINDEX, 0);
return return_TYPENAME;
}
TPSTYPE EMPTYTYPE() {
TPSTYPE return_EMPTYTYPE;
TPSTYPE RET;
RET.NAME = str_make(0, "");
RET.CLS = TTCBOOLEAN;
RET.ALIASFOR = ((void*)0);
return_EMPTYTYPE = RET;
return return_EMPTYTYPE;
}
TPSTYPE COPYTYPE(struct record9 *TYPEINDEX) {
TPSTYPE return_COPYTYPE;
TPSTYPE NEWTYP;
NEWTYP = *(TYPEINDEX);
if (NEWTYP.CLS == TTCPOINTERUNKNOWN) {
NEWTYP.TARGETNAME = malloc(sizeof(PString));
*(NEWTYP.TARGETNAME) = *((*(TYPEINDEX)).TARGETNAME);
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
PBoolean ISINTEGERTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISINTEGERTYPE;
return_ISINTEGERTYPE = (*(TYPEINDEX)).CLS == TTCINTEGER;
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
PBoolean ISSTRINGTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISSTRINGTYPE;
return_ISSTRINGTYPE = (*(TYPEINDEX)).CLS == TTCSTRING;
return return_ISSTRINGTYPE;
}
PBoolean ISCHARTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISCHARTYPE;
return_ISCHARTYPE = (*(TYPEINDEX)).CLS == TTCCHAR;
return return_ISCHARTYPE;
}
PBoolean ISSTRINGYTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISSTRINGYTYPE;
return_ISSTRINGYTYPE = ISSTRINGTYPE(TYPEINDEX) || ISCHARTYPE(TYPEINDEX);
return return_ISSTRINGYTYPE;
}
TPSTYPE BOOLEANTYPE() {
TPSTYPE return_BOOLEANTYPE;
return_BOOLEANTYPE = TYPEOFCLASS(TTCBOOLEAN);
return return_BOOLEANTYPE;
}
PBoolean ISBOOLEANTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISBOOLEANTYPE;
return_ISBOOLEANTYPE = (*(TYPEINDEX)).CLS == TTCBOOLEAN;
return return_ISBOOLEANTYPE;
}
TPSTYPE TEXTTYPE() {
TPSTYPE return_TEXTTYPE;
return_TEXTTYPE = TYPEOFCLASS(TTCTEXT);
return return_TEXTTYPE;
}
PBoolean ISTEXTTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISTEXTTYPE;
return_ISTEXTTYPE = (*(TYPEINDEX)).CLS == TTCTEXT;
return return_ISTEXTTYPE;
}
PBoolean ISENUMTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISENUMTYPE;
return_ISENUMTYPE = (*(TYPEINDEX)).CLS == TTCENUM;
return return_ISENUMTYPE;
}
PBoolean ISRECORDTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISRECORDTYPE;
return_ISRECORDTYPE = (*(TYPEINDEX)).CLS == TTCRECORD;
return return_ISRECORDTYPE;
}
PBoolean ISARRAYTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISARRAYTYPE;
return_ISARRAYTYPE = (*(TYPEINDEX)).CLS == TTCARRAY;
return return_ISARRAYTYPE;
}
TPSTYPE POINTERTYPE(struct record9 *TYPEINDEX) {
TPSTYPE return_POINTERTYPE;
TPSTYPE TYP;
TYP = TYPEOFCLASS(TTCPOINTER);
TYP.POINTEDTYPEINDEX = TYPEINDEX;
return_POINTERTYPE = TYP;
return return_POINTERTYPE;
}
PBoolean ISPOINTERTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISPOINTERTYPE;
return_ISPOINTERTYPE = (*(TYPEINDEX)).CLS == TTCPOINTER;
return return_ISPOINTERTYPE;
}
TPSTYPE NILTYPE() {
TPSTYPE return_NILTYPE;
return_NILTYPE = TYPEOFCLASS(TTCNIL);
return return_NILTYPE;
}
PBoolean ISNILTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISNILTYPE;
return_ISNILTYPE = (*(TYPEINDEX)).CLS == TTCNIL;
return return_ISNILTYPE;
}
PBoolean ISPOINTERYTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISPOINTERYTYPE;
return_ISPOINTERYTYPE = ISPOINTERTYPE(TYPEINDEX) || ISNILTYPE(TYPEINDEX);
return return_ISPOINTERYTYPE;
}
TPSTYPE POINTERUNKNOWNTYPE(PString TARGETNAME) {
TPSTYPE return_POINTERUNKNOWNTYPE;
TPSTYPE TYP;
TYP = TYPEOFCLASS(TTCPOINTERUNKNOWN);
TYP.TARGETNAME = malloc(sizeof(PString));
*(TYP.TARGETNAME) = TARGETNAME;
return_POINTERUNKNOWNTYPE = TYP;
return return_POINTERUNKNOWNTYPE;
}
PBoolean ISPOINTERUNKNOWNTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISPOINTERUNKNOWNTYPE;
return_ISPOINTERUNKNOWNTYPE = (*(TYPEINDEX)).CLS == TTCPOINTERUNKNOWN;
return return_ISPOINTERUNKNOWNTYPE;
}
PBoolean ISORDINALTYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISORDINALTYPE;
return_ISORDINALTYPE = ISBOOLEANTYPE(TYPEINDEX) || ISINTEGERTYPE(TYPEINDEX) || ISCHARTYPE(TYPEINDEX) || ISENUMTYPE(TYPEINDEX);
return return_ISORDINALTYPE;
}
PBoolean ISSIMPLETYPE(struct record9 *TYPEINDEX) {
PBoolean return_ISSIMPLETYPE;
return_ISSIMPLETYPE = ISORDINALTYPE(TYPEINDEX) || ISSTRINGTYPE(TYPEINDEX);
return return_ISSIMPLETYPE;
}
PBoolean ISSAMETYPE(struct record9 *A, struct record9 *B) {
PBoolean return_ISSAMETYPE;
if ((A == ((void*)0)) || (B == ((void*)0))) return_ISSAMETYPE = A == B;
 else {
while ((*(A)).ALIASFOR != ((void*)0)) A = (*(A)).ALIASFOR;
while ((*(B)).ALIASFOR != ((void*)0)) B = (*(B)).ALIASFOR;
return_ISSAMETYPE = (A == B) || (ISPOINTERTYPE(A) && ISPOINTERTYPE(B) && ISSAMETYPE((*(A)).POINTEDTYPEINDEX, (*(B)).POINTEDTYPEINDEX));
}
return return_ISSAMETYPE;
}
PBoolean AREPOINTERSCOMPATIBLE(struct record9 *A, struct record9 *B) {
PBoolean return_AREPOINTERSCOMPATIBLE;
return_AREPOINTERSCOMPATIBLE = ISPOINTERYTYPE(A) && ISPOINTERYTYPE(B) && (ISNILTYPE(A) || ISNILTYPE(B) || ISSAMETYPE(A, B));
return return_AREPOINTERSCOMPATIBLE;
}
struct record9 *ADDTYPE(TPSTYPE TYP) {
struct record9 *return_ADDTYPE;
struct record9 *TYPEINDEX;
int ENUMPOS;
TYPEINDEX = (*(_ADDDEF(TDCTYPE))).TYPEINDEX;
*(TYPEINDEX) = TYP;
return_ADDTYPE = TYPEINDEX;
if (cmp_ss(TYP.NAME, str_make(0, "")) != 0) {
if (FINDNAMEINLOCALSCOPE(TYP.NAME, 0) != ((void*)0)) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), TYP.NAME), str_make(16, " already defined")));
ADDTYPENAME(TYP.NAME, TYPEINDEX);
}
if ((TYP.CLS == TTCENUM) && (TYP.ALIASFOR == ((void*)0))) {
int first = 1;
int last = (*(TYP.ENUMINDEX)).SIZE;
if (first <= last) {
ENUMPOS = first;
while (1) {
ADDENUMVALUENAME(ENUMPOS, TYPEINDEX);
if (ENUMPOS == last) break;
++ENUMPOS;
}
}
}
return return_ADDTYPE;
}
struct record10 *ADDENUM(TPSENUMDEF ENUM) {
struct record10 *return_ADDENUM;
struct record10 *ENUMINDEX;
ENUMINDEX = (*(_ADDDEF(TDCENUM))).ENUMINDEX;
*(ENUMINDEX) = ENUM;
(*(ENUMINDEX)).ID = DEFCOUNTER();
return_ADDENUM = ENUMINDEX;
return return_ADDENUM;
}
struct record12 *ADDRECORD(TPSRECORDDEF REC) {
struct record12 *return_ADDRECORD;
struct record12 *RECORDINDEX;
RECORDINDEX = (*(_ADDDEF(TDCRECORD))).RECORDINDEX;
*(RECORDINDEX) = REC;
(*(RECORDINDEX)).ID = DEFCOUNTER();
return_ADDRECORD = RECORDINDEX;
return return_ADDRECORD;
}
struct record13 *ADDARRAY(TPSARRAYDEF ARR) {
struct record13 *return_ADDARRAY;
struct record13 *ARRAYINDEX;
ARRAYINDEX = (*(_ADDDEF(TDCARRAY))).ARRAYINDEX;
*(ARRAYINDEX) = ARR;
return_ADDARRAY = ARRAYINDEX;
return return_ADDARRAY;
}
struct record14 *FINDCONSTANT(PString NAME) {
struct record14 *return_FINDCONSTANT;
struct record19 *DEF;
struct record14 *RET;
RET = ((void*)0);
DEF = DEFS.LATEST;
while ((RET == ((void*)0)) && (DEF != ((void*)0))) {
if (((*(DEF)).CLS == TDCCONSTANT) && (cmp_ss(NAME, (*((*(DEF)).CONSTANTINDEX)).NAME) == 0)) RET = (*(DEF)).CONSTANTINDEX;
DEF = (*(DEF)).PREV;
}
return_FINDCONSTANT = RET;
return return_FINDCONSTANT;
}
struct record14 *ADDCONSTANT(TPSCONSTANT CONSTANT) {
struct record14 *return_ADDCONSTANT;
struct record14 *CONSTANTINDEX;
if (FINDCONSTANT(CONSTANT.NAME) != ((void*)0)) COMPILEERROR(cat_ss(cat_ss(str_make(9, "Constant "), CONSTANT.NAME), str_make(16, " already defined")));
CONSTANTINDEX = (*(_ADDDEF(TDCCONSTANT))).CONSTANTINDEX;
*(CONSTANTINDEX) = CONSTANT;
return_ADDCONSTANT = CONSTANTINDEX;
return return_ADDCONSTANT;
}
struct record15 *ADDVARIABLE(TPSVARIABLE VARDEF) {
struct record15 *return_ADDVARIABLE;
struct record15 *VARIABLEINDEX;
if (FINDNAMEINLOCALSCOPE(VARDEF.NAME, 0) != ((void*)0)) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), VARDEF.NAME), str_make(16, " already defined")));
VARIABLEINDEX = (*(_ADDDEF(TDCVARIABLE))).VARIABLEINDEX;
ADDVARIABLENAME(VARDEF.NAME, VARIABLEINDEX);
*(VARIABLEINDEX) = VARDEF;
return_ADDVARIABLE = VARIABLEINDEX;
return return_ADDVARIABLE;
}
TPSFUNCTION EMPTYFUNCTION() {
TPSFUNCTION return_EMPTYFUNCTION;
TPSFUNCTION RET;
RET.NAME = str_make(0, "");
RET.ARGCOUNT = 0;
RET.RETURNTYPEINDEX = ((void*)0);
RET.ISDECLARATION = 0;
return_EMPTYFUNCTION = RET;
return return_EMPTYFUNCTION;
}
PBoolean ISEMPTYFUNCTION(TPSFUNCTION FN) {
PBoolean return_ISEMPTYFUNCTION;
return_ISEMPTYFUNCTION = cmp_ss(FN.NAME, str_make(0, "")) == 0;
return return_ISEMPTYFUNCTION;
}
PBoolean ISSAMEFUNCTIONDEFINITION(struct record16 *DECLINDEX, TPSFUNCTION FUN) {
PBoolean return_ISSAMEFUNCTIONDEFINITION;
TPSFUNCTION DECL;
PBoolean SAME;
int POS;
DECL = *(DECLINDEX);
SAME = ISSAMETYPE(DECL.RETURNTYPEINDEX, FUN.RETURNTYPEINDEX) && (DECL.ARGCOUNT == FUN.ARGCOUNT);
{
int first = 1;
int last = DECL.ARGCOUNT;
if (first <= last) {
POS = first;
while (1) {
SAME = SAME && ISSAMETYPE(DECL.ARGS[(POS) - 1].TYPEINDEX, FUN.ARGS[(POS) - 1].TYPEINDEX) && (DECL.ARGS[(POS) - 1].ISREFERENCE == FUN.ARGS[(POS) - 1].ISREFERENCE);
if (POS == last) break;
++POS;
}
}
}
return_ISSAMEFUNCTIONDEFINITION = SAME;
return return_ISSAMEFUNCTIONDEFINITION;
}
PBoolean HASFORWARDDECLARATION(PString NAME) {
PBoolean return_HASFORWARDDECLARATION;
struct record18 *NAMEINDEX;
NAMEINDEX = FINDNAMEOFCLASSINLOCALSCOPE(NAME, TNCFUNCTION, 0);
return_HASFORWARDDECLARATION = (NAMEINDEX != ((void*)0)) && ((*((*(NAMEINDEX)).FUNCTIONINDEX)).ISDECLARATION);
return return_HASFORWARDDECLARATION;
}
struct record16 *ADDFUNCTION(TPSFUNCTION FUN) {
struct record16 *return_ADDFUNCTION;
struct record18 *NAMEINDEX;
struct record16 *FNINDEX;
PBoolean ISPROCEDURE;
ISPROCEDURE = FUN.RETURNTYPEINDEX == ((void*)0);
NAMEINDEX = FINDNAMEINLOCALSCOPE(FUN.NAME, 0);
if (NAMEINDEX == ((void*)0)) {
FNINDEX = (*(_ADDDEF(TDCFUNCTION))).FUNCTIONINDEX;
*(FNINDEX) = FUN;
ADDFUNCTIONNAME(FUN.NAME, FNINDEX);
}
 else {
if (((*(NAMEINDEX)).CLS != TNCFUNCTION) || FUN.ISDECLARATION) COMPILEERROR(cat_ss(cat_ss(str_make(11, "Identifier "), FUN.NAME), str_make(16, " already defined")));
FNINDEX = (*(NAMEINDEX)).FUNCTIONINDEX;
if ((*(FNINDEX)).ISDECLARATION) {
if (((FUN.ARGCOUNT == 0) && (FUN.RETURNTYPEINDEX == ((void*)0))) || ISSAMEFUNCTIONDEFINITION(FNINDEX, FUN)) (*(FNINDEX)).ISDECLARATION = 0;
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
struct record9 *FINDFIELDTYPE(struct record9 *TYPEINDEX, PString NAME, PBoolean REQUIRED) {
struct record9 *return_FINDFIELDTYPE;
int POS;
struct record9 *RET;
if ((*(TYPEINDEX)).CLS != TTCRECORD) COMPILEERROR(cat_ss(str_make(14, "Not a record: "), (*(TYPEINDEX)).NAME));
{
TPSRECORDDEF *_with_1 = &(*((*(TYPEINDEX)).RECORDINDEX));
{
RET = ((void*)0);
POS = (*_with_1).SIZE;
while ((POS >= 1) && (RET == ((void*)0))) {
if (cmp_ss(NAME, (*_with_1).FIELDS[(POS) - 1].NAME) == 0) RET = (*_with_1).FIELDS[(POS) - 1].TYPEINDEX;
POS = POS - 1;
}
}
}
if (REQUIRED && (RET == ((void*)0))) COMPILEERROR(cat_ss(str_make(17, "Field not found: "), NAME));
return_FINDFIELDTYPE = RET;
return return_FINDFIELDTYPE;
}
struct record17 *FINDWITHVAR(PString NAME) {
struct record17 *return_FINDWITHVAR;
struct record17 *RET;
struct record19 *DEF;
struct record9 *TYPEINDEX;
RET = ((void*)0);
DEF = DEFS.LATEST;
while ((RET == ((void*)0)) && (DEF != ((void*)0)) && ((*(DEF)).CLS != TDCSCOPEBOUNDARY)) {
if ((*(DEF)).CLS == TDCWITHVAR) {
TYPEINDEX = (*((*((*(DEF)).WITHVARINDEX)).VARIABLEINDEX)).TYPEINDEX;
if (FINDFIELDTYPE(TYPEINDEX, NAME, 0) != ((void*)0)) RET = (*(DEF)).WITHVARINDEX;
}
DEF = (*(DEF)).PREV;
}
return_FINDWITHVAR = RET;
return return_FINDWITHVAR;
}
struct record15 *ADDWITHVAR(TPSEXPRESSION BASE) {
struct record15 *return_ADDWITHVAR;
PString TMPVARNUM;
TPSVARIABLE TMPVAR;
struct record15 *TMPVARINDEX;
struct record17 *WITHVARINDEX;
if ((BASE.CLS != TECVALUE) || !ISRECORDTYPE(BASE.TYPEINDEX)) COMPILEERROR(str_make(31, "\'With\' variable is not a record"));
TMPVARNUM = to_str_i(DEFCOUNTER());
TMPVAR.NAME = cat_ss(str_make(4, "with"), TMPVARNUM);
TMPVAR.TYPEINDEX = BASE.TYPEINDEX;
TMPVAR.ISCONSTANT = BASE.ISCONSTANT;
TMPVAR.ISREFERENCE = 1;
TMPVARINDEX = ADDVARIABLE(TMPVAR);
WITHVARINDEX = (*(_ADDDEF(TDCWITHVAR))).WITHVARINDEX;
(*(WITHVARINDEX)).VARIABLEINDEX = TMPVARINDEX;
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
TPSVARIABLE MAKETYPEDCONSTANT(PString NAME, struct record9 *TYPEINDEX) {
TPSVARIABLE return_MAKETYPEDCONSTANT;
TPSVARIABLE VARDEF;
VARDEF.NAME = NAME;
VARDEF.TYPEINDEX = TYPEINDEX;
VARDEF.ISREFERENCE = 0;
VARDEF.ISCONSTANT = 1;
return_MAKETYPEDCONSTANT = VARDEF;
return return_MAKETYPEDCONSTANT;
}
TPSVARIABLE MAKEVARIABLE(PString NAME, struct record9 *TYPEINDEX, PBoolean ISREF) {
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
void OUTENUMVALUES(struct record10 *ENUMINDEX);
void OUTENUMVALUESFROMCHECKPOINT(struct record19 *CHECKPOINT);
PString OUTVARIABLENAME(PString NAME, PBoolean ISREFERENCE);
PString OUTRETURNVARIABLENAME(PString NAME);
void OUTTYPEREFERENCE(struct record9 *TYPEINDEX);
void OUTNAMEANDTYPE(PString NAME, struct record9 *TYPEINDEX);
void OUTTYPEDEFINITION(struct record9 *TYPEINDEX);
void OUTTYPEDEFINITIONSFROMCHECKPOINT(struct record19 *CHECKPOINT);
void OUTCONSTANTVALUE(TPSEXPRESSION EXPR);
void OUTCONSTANTARRAYBEGIN();
void OUTCONSTANTARRAYSEPARATOR();
void OUTCONSTANTARRAYEND();
void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF);
void OUTVARIABLEDEFINITION(struct record15 *VARINDEX);
void OUTCONSTANTDEFINITIONBEGIN(struct record15 *VARINDEX);
void OUTCONSTANTDEFINITIONEND();
void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF);
void OUTFUNCTIONDECLARATION(struct record16 *FNINDEX);
void OUTFUNCTIONDEFINITION(struct record16 *FNINDEX);
void OUTFUNCTIONEND(struct record16 *FNINDEX);
void OUTPROGRAMHEADING(PString NAME);
PString GENFUNCTIONCALLSTART(PString FN);
PString GENFUNCTIONCALLEND(PString FN);
PString GENFUNCTIONCALLARGUMENT(PString FN, TPSEXPRESSION EXPR, PBoolean ISREFERENCE, int ARGNUM);
void OUTREAD(TPSEXPRESSION SRC, TPSEXPRESSION OUTVAR);
void OUTREADLN(TPSEXPRESSION SRC);
void OUTWRITE(TPSEXPRESSION DST, TPSEXPRESSION EXPR);
void OUTWRITELN(TPSEXPRESSION DST);
void OUTSTR(PString DST, TPSEXPRESSION EXPR);
void OUTNEW(TPSEXPRESSION DST);
void OUTDISPOSE(TPSEXPRESSION DST);
void OUTEXPRESSION(TPSEXPRESSION EXPR);
void OUTASSIGN(TPSEXPRESSION LHS, TPSEXPRESSION RHS);
void OUTASSIGNRETURNVALUE(TPSEXPRESSION LHS, TPSEXPRESSION RHS);
void OUTASSIGNTOREFERENCE(struct record15 *VARINDEX, TPSEXPRESSION RHS);
void OUTIF(TPSEXPRESSION EXPR);
void OUTELSE();
void OUTCASEBEGIN(TPSEXPRESSION CASEINDEX);
void OUTCASESTATEMENTBEGIN(TPSEXPRESSION CASELABEL);
void OUTCASESTATEMENTEND();
void OUTCASEELSEBEGIN();
void OUTCASEELSEEND();
void OUTCASEEND();
void OUTREPEATBEGIN();
void OUTREPEATEND(TPSEXPRESSION EXPR);
void OUTWHILEBEGIN(TPSEXPRESSION EXPR);
void OUTWHILEEND();
void OUTFORBEGIN(TPSEXPRESSION ITER, TPSEXPRESSION FIRSTEXPR, TPSEXPRESSION LASTEXPR, PBoolean ASCENDING);
void OUTFOREND(TPSEXPRESSION ITER, PBoolean ASCENDING);
void OUTPROCEDURECALL(TPSEXPRESSION EXPR);
void OUTEMPTYSTATEMENT();
void OUTPROGRAMBEGIN();
void OUTPROGRAMEND();
TPSEXPRESSION EXPREVALUATE(TPSEXPRESSION EXPR) {
TPSEXPRESSION return_EXPREVALUATE;
if (EXPR.CLS == TECFUNCTION) {
if ((*(EXPR.FUNCTIONINDEX)).ARGCOUNT != 0) COMPILEERROR(str_make(27, "Function requires arguments"));
EXPR.VALUE = cat_ss(EXPR.VALUE, str_make(2, "()"));
EXPR.TYPEINDEX = (*(EXPR.FUNCTIONINDEX)).RETURNTYPEINDEX;
if (EXPR.TYPEINDEX == ((void*)0)) EXPR.CLS = TECSTATEMENT;
 else EXPR.CLS = TECVALUE;
}
return_EXPREVALUATE = EXPR;
return return_EXPREVALUATE;
}
TPSEXPRESSION EXPRCOERCE(TPSEXPRESSION EXPR, struct record9 *TYPEINDEX) {
TPSEXPRESSION return_EXPRCOERCE;
EXPR = EXPREVALUATE(EXPR);
if (EXPR.CLS != TECVALUE) COMPILEERROR(cat_ss(str_make(26, "Cannot assign function to "), TYPENAME(TYPEINDEX)));
if (ISCHARTYPE(EXPR.TYPEINDEX) && ISSTRINGTYPE(TYPEINDEX)) {
EXPR.TYPEINDEX = PRIMITIVETYPES.PTSTRING;
EXPR.VALUE = cat_sc(cat_ss(str_make(7, "str_of("), EXPR.VALUE), ')');
}
 else if (ISNILTYPE(EXPR.TYPEINDEX) && ISPOINTERTYPE(TYPEINDEX)) EXPR.TYPEINDEX = TYPEINDEX;
 else if (!ISSAMETYPE(EXPR.TYPEINDEX, TYPEINDEX)) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(13, "Cannot treat "), TYPENAME(EXPR.TYPEINDEX)), str_make(4, " as ")), TYPENAME(TYPEINDEX)));
return_EXPRCOERCE = EXPR;
return return_EXPRCOERCE;
}
PBoolean EXPRISVARIABLE(TPSEXPRESSION EXPR) {
PBoolean return_EXPRISVARIABLE;
return_EXPRISVARIABLE = (EXPR.CLS == TECVALUE) && (EXPR.TYPEINDEX != ((void*)0)) && !EXPR.ISCONSTANT;
return return_EXPRISVARIABLE;
}
TPSEXPRESSION EXPRNILCONSTANT() {
TPSEXPRESSION return_EXPRNILCONSTANT;
TPSEXPRESSION EXPR;
EXPR.TYPEINDEX = PRIMITIVETYPES.PTNIL;
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
EXPR.VALUE = str_make(10, "((void*)0)");
return_EXPRNILCONSTANT = EXPR;
return return_EXPRNILCONSTANT;
}
TPSEXPRESSION EXPRBOOLEANCONSTANT(PBoolean VALUE) {
TPSEXPRESSION return_EXPRBOOLEANCONSTANT;
TPSEXPRESSION EXPR;
EXPR.TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
if (VALUE) EXPR.VALUE = str_of('1');
 else EXPR.VALUE = str_of('0');
return_EXPRBOOLEANCONSTANT = EXPR;
return return_EXPRBOOLEANCONSTANT;
}
TPSEXPRESSION EXPRSTRINGCONSTANT(PString VALUE) {
TPSEXPRESSION return_EXPRSTRINGCONSTANT;
TPSEXPRESSION EXPR;
PString SIZE;
int POS;
PBoolean INSTR;
PBoolean LASTQUOTE;
int LEN;
INSTR = 0;
LASTQUOTE = 0;
EXPR.VALUE = str_make(0, "");
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
LEN = 0;
{
int first = 1;
int last = LENGTH(VALUE);
if (first <= last) {
POS = first;
while (1) {
{
if (cmp_cc(VALUE.chr[POS], '\'') == 0) {
INSTR = !INSTR;
if (INSTR && LASTQUOTE) {
EXPR.VALUE = cat_ss(EXPR.VALUE, str_make(2, "\\\'"));
LEN = LEN + 1;
}
 else LASTQUOTE = !INSTR;
}
 else {
LASTQUOTE = 0;
if (cmp_cc(VALUE.chr[POS], '\"') == 0) EXPR.VALUE = cat_ss(EXPR.VALUE, str_make(2, "\\\""));
 else if (cmp_cc(VALUE.chr[POS], '\\') == 0) EXPR.VALUE = cat_ss(EXPR.VALUE, str_make(2, "\\\\"));
 else EXPR.VALUE = cat_sc(EXPR.VALUE, VALUE.chr[POS]);
LEN = LEN + 1;
}
}
if (POS == last) break;
++POS;
}
}
}
if (LEN == 1) {
EXPR.VALUE = cat_sc(cat_cs('\'', EXPR.VALUE), '\'');
EXPR.TYPEINDEX = PRIMITIVETYPES.PTCHAR;
}
 else {
SIZE = to_str_i(LEN);
EXPR.VALUE = cat_ss(cat_ss(cat_ss(cat_ss(str_make(9, "str_make("), SIZE), str_make(3, ", \"")), EXPR.VALUE), str_make(2, "\")"));
EXPR.TYPEINDEX = PRIMITIVETYPES.PTSTRING;
}
return_EXPRSTRINGCONSTANT = EXPR;
return return_EXPRSTRINGCONSTANT;
}
TPSEXPRESSION EXPRNUMBERCONSTANT(PString VALUE) {
TPSEXPRESSION return_EXPRNUMBERCONSTANT;
TPSEXPRESSION EXPR;
EXPR.TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
EXPR.VALUE = VALUE;
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
return_EXPRNUMBERCONSTANT = EXPR;
return return_EXPRNUMBERCONSTANT;
}
TPSEXPRESSION EXPRVARIABLEACCESS(struct record15 *VARINDEX) {
TPSEXPRESSION return_EXPRVARIABLEACCESS;
TPSEXPRESSION EXPR;
if ((*(VARINDEX)).ISREFERENCE) EXPR.VALUE = cat_cs('*', (*(VARINDEX)).NAME);
 else EXPR.VALUE = (*(VARINDEX)).NAME;
EXPR.CLS = TECVALUE;
EXPR.TYPEINDEX = (*(VARINDEX)).TYPEINDEX;
EXPR.ISCONSTANT = 0;
return_EXPRVARIABLEACCESS = EXPR;
return return_EXPRVARIABLEACCESS;
}
TPSEXPRESSION EXPRFUNCTIONREFERENCE(struct record16 *FNINDEX) {
TPSEXPRESSION return_EXPRFUNCTIONREFERENCE;
TPSEXPRESSION EXPR;
EXPR.VALUE = (*(FNINDEX)).NAME;
EXPR.CLS = TECFUNCTION;
EXPR.FUNCTIONINDEX = FNINDEX;
EXPR.ISCONSTANT = 0;
return_EXPRFUNCTIONREFERENCE = EXPR;
return return_EXPRFUNCTIONREFERENCE;
}
TPSEXPRESSION EXPRENUMVALUE(int ORDINAL, struct record9 *TYPEINDEX) {
TPSEXPRESSION return_EXPRENUMVALUE;
TPSEXPRESSION EXPR;
{
TPSENUMDEF *_with_1 = &(*((*(TYPEINDEX)).ENUMINDEX));
{
EXPR.VALUE = (*_with_1).VALUES[(ORDINAL) - 1];
EXPR.CLS = TECVALUE;
EXPR.TYPEINDEX = TYPEINDEX;
EXPR.ISCONSTANT = 1;
}
}
return_EXPRENUMVALUE = EXPR;
return return_EXPRENUMVALUE;
}
TPSEXPRESSION EXPRFIELDACCESS(TPSEXPRESSION BASE, PString NAME) {
TPSEXPRESSION return_EXPRFIELDACCESS;
TPSEXPRESSION EXPR;
if ((BASE.CLS != TECVALUE) || !ISRECORDTYPE(BASE.TYPEINDEX)) COMPILEERROR(str_make(12, "Not a record"));
EXPR.VALUE = BASE.VALUE;
if (cmp_cc(EXPR.VALUE.chr[1], '*') == 0) EXPR.VALUE = cat_sc(cat_cs('(', EXPR.VALUE), ')');
EXPR.VALUE = cat_ss(cat_sc(EXPR.VALUE, '.'), NAME);
EXPR.CLS = TECVALUE;
EXPR.TYPEINDEX = FINDFIELDTYPE(BASE.TYPEINDEX, NAME, 1);
EXPR.ISCONSTANT = BASE.ISCONSTANT;
return_EXPRFIELDACCESS = EXPR;
return return_EXPRFIELDACCESS;
}
TPSEXPRESSION EXPRARRAYACCESS(TPSEXPRESSION BASE, TPSEXPRESSION IDX) {
TPSEXPRESSION return_EXPRARRAYACCESS;
TPSEXPRESSION EXPR;
if ((BASE.CLS == TECVALUE) && ISSTRINGTYPE(BASE.TYPEINDEX)) {
if ((IDX.CLS != TECVALUE) || !ISINTEGERTYPE(IDX.TYPEINDEX)) COMPILEERROR(str_make(34, "String subscript is not an integer"));
EXPR.VALUE = cat_sc(cat_ss(cat_ss(cat_sc(cat_cs('(', BASE.VALUE), ')'), str_make(5, ".chr[")), IDX.VALUE), ']');
EXPR.CLS = TECVALUE;
EXPR.TYPEINDEX = PRIMITIVETYPES.PTCHAR;
EXPR.ISCONSTANT = BASE.ISCONSTANT;
}
 else if ((BASE.CLS == TECVALUE) && ISARRAYTYPE(BASE.TYPEINDEX)) {
if ((IDX.CLS != TECVALUE) || !ISINTEGERTYPE(IDX.TYPEINDEX)) COMPILEERROR(str_make(33, "Array subscript is not an integer"));
{
TPSARRAYDEF *_with_1 = &(*((*(BASE.TYPEINDEX)).ARRAYINDEX));
{
EXPR.VALUE = cat_sc(cat_ss(cat_ss(cat_ss(cat_ss(BASE.VALUE, str_make(2, "[(")), IDX.VALUE), str_make(4, ") - ")), (*_with_1).LOWBOUND), ']');
EXPR.CLS = TECVALUE;
EXPR.TYPEINDEX = (*_with_1).TYPEINDEX;
EXPR.ISCONSTANT = BASE.ISCONSTANT;
}
}
}
 else COMPILEERROR(str_make(21, "Not a string or array"));
return_EXPRARRAYACCESS = EXPR;
return return_EXPRARRAYACCESS;
}
TPSEXPRESSION EXPRPOINTERDEREF(TPSEXPRESSION PTR) {
TPSEXPRESSION return_EXPRPOINTERDEREF;
TPSEXPRESSION EXPR;
if ((PTR.CLS != TECVALUE) || !ISPOINTERTYPE(PTR.TYPEINDEX)) COMPILEERROR(str_make(13, "Not a pointer"));
EXPR.VALUE = cat_sc(cat_ss(str_make(2, "*("), PTR.VALUE), ')');
EXPR.CLS = TECVALUE;
EXPR.TYPEINDEX = (*(PTR.TYPEINDEX)).POINTEDTYPEINDEX;
EXPR.ISCONSTANT = 0;
return_EXPRPOINTERDEREF = EXPR;
return return_EXPRPOINTERDEREF;
}
TPSEXPRESSION _EXPRBINARYOPINTEGER(TPSEXPRESSION LEFT, TLXTOKENID OP, TPSEXPRESSION RIGHT) {
TPSEXPRESSION return__EXPRBINARYOPINTEGER;
PString OPER;
PString CMP;
TPSEXPRESSION EXPR;
OPER = str_make(0, "");
CMP = str_make(0, "");
switch (OP) {
case TKPLUS: OPER = str_of('+');
break;
case TKMINUS: OPER = str_of('-');
break;
case TKASTERISK: OPER = str_of('*');
break;
case TKDIV: OPER = str_of('/');
break;
case TKMOD: OPER = str_of('%');
break;
case TKAND: OPER = str_of('&');
break;
case TKOR: OPER = str_of('|');
break;
case TKEQUALS: CMP = str_make(2, "==");
break;
case TKNOTEQUALS: CMP = str_make(2, "!=");
break;
case TKLESSTHAN: CMP = str_of('<');
break;
case TKMORETHAN: CMP = str_of('>');
break;
case TKLESSOREQUALS: CMP = str_make(2, "<=");
break;
case TKMOREOREQUALS: CMP = str_make(2, ">=");
break;
default: COMPILEERROR(cat_ss(str_make(40, "Expected integer binary operator, found "), LXTOKENNAME(OP)));
break;
}
if (cmp_ss(CMP, str_make(0, "")) == 0) EXPR.TYPEINDEX = PRIMITIVETYPES.PTINTEGER;
 else EXPR.TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
EXPR.VALUE = cat_ss(cat_sc(cat_ss(cat_ss(cat_sc(LEFT.VALUE, ' '), OPER), CMP), ' '), RIGHT.VALUE);
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
return__EXPRBINARYOPINTEGER = EXPR;
return return__EXPRBINARYOPINTEGER;
}
TPSEXPRESSION _EXPRBINARYOPBOOLEAN(TPSEXPRESSION LEFT, TLXTOKENID OP, TPSEXPRESSION RIGHT) {
TPSEXPRESSION return__EXPRBINARYOPBOOLEAN;
PString OPER;
TPSEXPRESSION EXPR;
switch (OP) {
case TKAND: OPER = str_make(2, "&&");
break;
case TKOR: OPER = str_make(2, "||");
break;
case TKEQUALS: OPER = str_make(2, "==");
break;
case TKNOTEQUALS: OPER = str_make(2, "!=");
break;
case TKLESSTHAN: OPER = str_of('<');
break;
case TKMORETHAN: OPER = str_of('>');
break;
case TKLESSOREQUALS: OPER = str_make(2, "<=");
break;
case TKMOREOREQUALS: OPER = str_make(2, ">=");
break;
default: COMPILEERROR(cat_ss(str_make(40, "Expected boolean binary operator, found "), LXTOKENNAME(OP)));
break;
}
EXPR.TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
EXPR.VALUE = cat_ss(cat_sc(cat_ss(cat_sc(LEFT.VALUE, ' '), OPER), ' '), RIGHT.VALUE);
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
return__EXPRBINARYOPBOOLEAN = EXPR;
return return__EXPRBINARYOPBOOLEAN;
}
TPSEXPRESSION _EXPRBINARYOPSTRINGY(TPSEXPRESSION LEFT, TLXTOKENID OP, TPSEXPRESSION RIGHT) {
TPSEXPRESSION return__EXPRBINARYOPSTRINGY;
PString FNAME;
PString CMP;
TPSEXPRESSION EXPR;
char LTYPE;
char RTYPE;
FNAME = str_make(3, "cmp");
CMP = str_make(0, "");
switch (OP) {
case TKPLUS: FNAME = str_make(3, "cat");
break;
case TKEQUALS: CMP = str_make(2, "==");
break;
case TKNOTEQUALS: CMP = str_make(2, "!=");
break;
case TKLESSTHAN: CMP = str_of('<');
break;
case TKMORETHAN: CMP = str_of('>');
break;
case TKLESSOREQUALS: CMP = str_make(2, "<=");
break;
case TKMOREOREQUALS: CMP = str_make(2, ">=");
break;
default: COMPILEERROR(cat_ss(str_make(39, "Expected string binary operator, found "), LXTOKENNAME(OP)));
break;
}
if (ISCHARTYPE(LEFT.TYPEINDEX)) LTYPE = 'c';
 else LTYPE = 's';
if (ISCHARTYPE(RIGHT.TYPEINDEX)) RTYPE = 'c';
 else RTYPE = 's';
FNAME = cat_sc(cat_sc(cat_sc(FNAME, '_'), LTYPE), RTYPE);
EXPR.TYPEINDEX = PRIMITIVETYPES.PTSTRING;
EXPR.VALUE = cat_sc(cat_ss(cat_ss(cat_ss(cat_sc(FNAME, '('), LEFT.VALUE), str_make(2, ", ")), RIGHT.VALUE), ')');
if (cmp_ss(CMP, str_make(0, "")) != 0) {
EXPR.TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
EXPR.VALUE = cat_ss(cat_ss(cat_sc(EXPR.VALUE, ' '), CMP), str_make(2, " 0"));
}
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
return__EXPRBINARYOPSTRINGY = EXPR;
return return__EXPRBINARYOPSTRINGY;
}
TPSEXPRESSION _EXPRBINARYOPENUM(TPSEXPRESSION LEFT, TLXTOKENID OP, TPSEXPRESSION RIGHT) {
TPSEXPRESSION return__EXPRBINARYOPENUM;
PString CMP;
TPSEXPRESSION EXPR;
CMP = str_make(0, "");
switch (OP) {
case TKEQUALS: CMP = str_make(2, "==");
break;
case TKNOTEQUALS: CMP = str_make(2, "!=");
break;
case TKLESSTHAN: CMP = str_of('<');
break;
case TKMORETHAN: CMP = str_of('>');
break;
case TKLESSOREQUALS: CMP = str_make(2, "<=");
break;
case TKMOREOREQUALS: CMP = str_make(2, ">=");
break;
default: COMPILEERROR(cat_ss(str_make(40, "Expected ordinal binary operator, found "), LXTOKENNAME(OP)));
break;
}
EXPR.TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
EXPR.VALUE = cat_ss(cat_sc(cat_ss(cat_sc(LEFT.VALUE, ' '), CMP), ' '), RIGHT.VALUE);
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
return__EXPRBINARYOPENUM = EXPR;
return return__EXPRBINARYOPENUM;
}
TPSEXPRESSION _EXPRBINARYOPPOINTER(TPSEXPRESSION LEFT, TLXTOKENID OP, TPSEXPRESSION RIGHT) {
TPSEXPRESSION return__EXPRBINARYOPPOINTER;
PString CMP;
TPSEXPRESSION EXPR;
CMP = str_make(0, "");
switch (OP) {
case TKEQUALS: CMP = str_make(2, "==");
break;
case TKNOTEQUALS: CMP = str_make(2, "!=");
break;
default: COMPILEERROR(cat_ss(str_make(40, "Expected pointer binary operator, found "), LXTOKENNAME(OP)));
break;
}
EXPR.TYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
EXPR.VALUE = cat_ss(cat_sc(cat_ss(cat_sc(LEFT.VALUE, ' '), CMP), ' '), RIGHT.VALUE);
EXPR.CLS = TECVALUE;
EXPR.ISCONSTANT = 1;
return__EXPRBINARYOPPOINTER = EXPR;
return return__EXPRBINARYOPPOINTER;
}
TPSEXPRESSION EXPRBINARYOP(TPSEXPRESSION LEFT, TLXTOKENID OP, TPSEXPRESSION RIGHT) {
TPSEXPRESSION return_EXPRBINARYOP;
LEFT = EXPREVALUATE(LEFT);
RIGHT = EXPREVALUATE(RIGHT);
if (ISBOOLEANTYPE(LEFT.TYPEINDEX) && ISBOOLEANTYPE(RIGHT.TYPEINDEX)) return_EXPRBINARYOP = _EXPRBINARYOPBOOLEAN(LEFT, OP, RIGHT);
 else if (ISINTEGERTYPE(LEFT.TYPEINDEX) && ISINTEGERTYPE(RIGHT.TYPEINDEX)) return_EXPRBINARYOP = _EXPRBINARYOPINTEGER(LEFT, OP, RIGHT);
 else if (ISSTRINGYTYPE(LEFT.TYPEINDEX) && ISSTRINGYTYPE(RIGHT.TYPEINDEX)) return_EXPRBINARYOP = _EXPRBINARYOPSTRINGY(LEFT, OP, RIGHT);
 else if (ISENUMTYPE(LEFT.TYPEINDEX) && ISSAMETYPE(LEFT.TYPEINDEX, RIGHT.TYPEINDEX)) return_EXPRBINARYOP = _EXPRBINARYOPENUM(LEFT, OP, RIGHT);
 else if (AREPOINTERSCOMPATIBLE(LEFT.TYPEINDEX, RIGHT.TYPEINDEX)) return_EXPRBINARYOP = _EXPRBINARYOPPOINTER(LEFT, OP, RIGHT);
 else COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(27, "Type mismatch for operator "), LXTOKENNAME(OP)), str_make(2, ": ")), TYPENAME(LEFT.TYPEINDEX)), str_make(5, " and ")), TYPENAME(RIGHT.TYPEINDEX)));
return return_EXPRBINARYOP;
}
TPSEXPRESSION EXPRUNARYOP(TLXTOKENID OP, TPSEXPRESSION EXPR) {
TPSEXPRESSION return_EXPRUNARYOP;
EXPR = EXPREVALUATE(EXPR);
if (OP == TKNOT) {
if (!ISBOOLEANTYPE(EXPR.TYPEINDEX)) COMPILEERROR(cat_ss(str_make(33, "Expected boolean expression, got "), TYPENAME(EXPR.TYPEINDEX)));
EXPR.VALUE = cat_cs('!', EXPR.VALUE);
}
 else if (OP == TKMINUS) {
if (!ISINTEGERTYPE(EXPR.TYPEINDEX)) COMPILEERROR(cat_ss(str_make(33, "Expected numeric expression, got "), TYPENAME(EXPR.TYPEINDEX)));
EXPR.VALUE = cat_cs('-', EXPR.VALUE);
}
 else COMPILEERROR(cat_ss(str_make(31, "Expected unary operator, found "), LXTOKENNAME(OP)));
EXPR.ISCONSTANT = 1;
return_EXPRUNARYOP = EXPR;
return return_EXPRUNARYOP;
}
TPSEXPRESSION EXPRPARENTHESES(TPSEXPRESSION EXPR) {
TPSEXPRESSION return_EXPRPARENTHESES;
EXPR.VALUE = cat_sc(cat_cs('(', EXPR.VALUE), ')');
return_EXPRPARENTHESES = EXPR;
return return_EXPRPARENTHESES;
}
void READTOKEN();
void WANTTOKEN(TLXTOKENID ID) {
if (LEXER.TOKEN.ID != ID) COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID)), str_make(8, ", found ")), LXTOKENSTR()));
}
void WANTTOKEN2(TLXTOKENID ID1, TLXTOKENID ID2) {
if ((LEXER.TOKEN.ID != ID1) && (LEXER.TOKEN.ID != ID2)) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID1)), str_make(4, " or ")), LXTOKENNAME(ID2)), str_make(8, ", found ")), LXTOKENSTR()));
}
void WANTTOKEN3(TLXTOKENID ID1, TLXTOKENID ID2, TLXTOKENID ID3) {
if ((LEXER.TOKEN.ID != ID1) && (LEXER.TOKEN.ID != ID2) && (LEXER.TOKEN.ID != ID3)) COMPILEERROR(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(cat_ss(str_make(13, "Wanted token "), LXTOKENNAME(ID1)), str_make(2, ", ")), LXTOKENNAME(ID2)), str_make(5, ", or ")), LXTOKENNAME(ID3)), str_make(7, " found ")), LXTOKENSTR()));
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
struct record9 *PSTYPEDENOTER();
struct record9 *PSTYPEIDENTIFIER() {
struct record9 *return_PSTYPEIDENTIFIER;
WANTTOKEN(TKIDENTIFIER);
return_PSTYPEIDENTIFIER = (*(FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 1))).TYPEINDEX;
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
TPSEXPRESSION PSEXPRESSION();
struct record9 *PSENUMERATEDTYPE() {
struct record9 *return_PSENUMERATEDTYPE;
TPSTYPE TYP;
TPSENUMDEF ENUM;
WANTTOKENANDREAD(TKLPAREN);
ENUM.SIZE = 0;
ENUM.HASBEENDEFINED = 0;
do {
ENUM.SIZE = ENUM.SIZE + 1;
if (ENUM.SIZE > 128) COMPILEERROR(str_make(23, "Too many values in enum"));
ENUM.VALUES[(ENUM.SIZE) - 1] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
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
struct record9 *TYPEINDEX;
LASTFIELD = (*REC).SIZE;
do {
NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
{
int first = 1;
int last = (*REC).SIZE;
if (first <= last) {
FIELD = first;
while (1) {
{
if (cmp_ss((*REC).FIELDS[(FIELD) - 1].NAME, NAME) == 0) COMPILEERROR(cat_ss(cat_ss(str_make(14, "A field named "), NAME), str_make(25, " has already been defined")));
}
if (FIELD == last) break;
++FIELD;
}
}
}
(*REC).SIZE = (*REC).SIZE + 1;
if ((*REC).SIZE > 16) COMPILEERROR(str_make(25, "Too many fields in record"));
(*REC).FIELDS[((*REC).SIZE) - 1].NAME = NAME;
WANTTOKEN2(TKCOMMA, TKCOLON);
SKIPTOKEN(TKCOMMA);
} while (!(LEXER.TOKEN.ID == TKCOLON));
WANTTOKENANDREAD(TKCOLON);
TYPEINDEX = PSTYPEDENOTER();
{
int first = LASTFIELD + 1;
int last = (*REC).SIZE;
if (first <= last) {
FIELD = first;
while (1) {
(*REC).FIELDS[(FIELD) - 1].TYPEINDEX = TYPEINDEX;
if (FIELD == last) break;
++FIELD;
}
}
}
WANTTOKEN2(TKSEMICOLON, DELIMITER);
SKIPTOKEN(TKSEMICOLON);
}
void PSRECORDVARIANTS(TPSRECORDDEF *REC) {
TPSIDENTIFIER TAG;
struct record9 *TAGTYPE;
TPSEXPRESSION CASELABEL;
WANTTOKENANDREAD(TKCASE);
TAG = PSIDENTIFIER();
WANTTOKEN2(TKCOLON, TKOF);
if (LEXER.TOKEN.ID == TKCOLON) {
READTOKEN();
TAGTYPE = PSTYPEIDENTIFIER();
(*REC).SIZE = (*REC).SIZE + 1;
(*REC).FIELDS[((*REC).SIZE) - 1].NAME = TAG.NAME;
(*REC).FIELDS[((*REC).SIZE) - 1].TYPEINDEX = TAGTYPE;
}
 else TAGTYPE = (*(FINDNAMEOFCLASS(TAG.NAME, TNCTYPE, 1))).TYPEINDEX;
if (!ISORDINALTYPE(TAGTYPE)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
WANTTOKENANDREAD(TKOF);
do {
(*REC).NUMVARIANTS = (*REC).NUMVARIANTS + 1;
(*REC).VARIANTBOUNDS[((*REC).NUMVARIANTS) - 1] = (*REC).SIZE + 1;
do {
CASELABEL = EXPRCOERCE(PSEXPRESSION(), TAGTYPE);
if (!CASELABEL.ISCONSTANT) COMPILEERROR(str_make(47, "The label of the case statement is not constant"));
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
struct record9 *PSRECORDTYPE() {
struct record9 *return_PSRECORDTYPE;
TPSTYPE TYP;
TPSRECORDDEF REC;
WANTTOKENANDREAD(TKRECORD);
REC.SIZE = 0;
REC.NUMVARIANTS = 0;
REC.HASBEENDEFINED = 0;
while ((LEXER.TOKEN.ID != TKCASE) && (LEXER.TOKEN.ID != TKEND)) PSRECORDFIELD(&REC, TKEND);
if (LEXER.TOKEN.ID == TKCASE) PSRECORDVARIANTS(&REC);
WANTTOKENANDREAD(TKEND);
TYP = TYPEOFCLASS(TTCRECORD);
TYP.RECORDINDEX = ADDRECORD(REC);
return_PSRECORDTYPE = ADDTYPE(TYP);
return return_PSRECORDTYPE;
}
struct record9 *PSARRAYTYPE() {
struct record9 *return_PSARRAYTYPE;
TPSTYPE TYP;
TPSARRAYDEF ARR;
WANTTOKENANDREAD(TKARRAY);
WANTTOKENANDREAD(TKLBRACKET);
ARR.LOWBOUND = GETTOKENVALUEANDREAD(TKNUMBER);
WANTTOKENANDREAD(TKRANGE);
ARR.HIGHBOUND = GETTOKENVALUEANDREAD(TKNUMBER);
WANTTOKENANDREAD(TKRBRACKET);
WANTTOKENANDREAD(TKOF);
ARR.TYPEINDEX = PSTYPEDENOTER();
TYP = TYPEOFCLASS(TTCARRAY);
TYP.ARRAYINDEX = ADDARRAY(ARR);
return_PSARRAYTYPE = ADDTYPE(TYP);
return return_PSARRAYTYPE;
}
struct record9 *PSPOINTERTYPE() {
struct record9 *return_PSPOINTERTYPE;
TPSTYPE TYP;
struct record18 *NAMEINDEX;
WANTTOKENANDREAD(TKCARET);
WANTTOKEN(TKIDENTIFIER);
NAMEINDEX = FINDNAMEOFCLASS(LEXER.TOKEN.VALUE, TNCTYPE, 0);
if (NAMEINDEX == ((void*)0)) TYP = POINTERUNKNOWNTYPE(LEXER.TOKEN.VALUE);
 else TYP = POINTERTYPE((*(NAMEINDEX)).TYPEINDEX);
READTOKEN();
return_PSPOINTERTYPE = ADDTYPE(TYP);
return return_PSPOINTERTYPE;
}
struct record9 *PSTYPEDENOTER() {
struct record9 *return_PSTYPEDENOTER;
struct record9 *TYPEINDEX;
TYPEINDEX = ((void*)0);
if (LEXER.TOKEN.ID == TKIDENTIFIER) TYPEINDEX = PSTYPEIDENTIFIER();
 else if (LEXER.TOKEN.ID == TKLPAREN) TYPEINDEX = PSENUMERATEDTYPE();
 else if (LEXER.TOKEN.ID == TKRECORD) TYPEINDEX = PSRECORDTYPE();
 else if (LEXER.TOKEN.ID == TKARRAY) TYPEINDEX = PSARRAYTYPE();
 else if (LEXER.TOKEN.ID == TKCARET) TYPEINDEX = PSPOINTERTYPE();
 else COMPILEERROR(cat_ss(str_make(30, "Wanted type definition, found "), LXTOKENSTR()));
return_PSTYPEDENOTER = TYPEINDEX;
return return_PSTYPEDENOTER;
}
void _RESOLVEPOINTERUNKNOWN(struct record9 *TYPEINDEX) {
struct record9 *TARGETINDEX;
if ((*(TYPEINDEX)).CLS == TTCPOINTERUNKNOWN) {
TARGETINDEX = (*(FINDNAMEOFCLASS(*((*(TYPEINDEX)).TARGETNAME), TNCTYPE, 1))).TYPEINDEX;
free((*(TYPEINDEX)).TARGETNAME);
(*(TYPEINDEX)).CLS = TTCPOINTER;
(*(TYPEINDEX)).POINTEDTYPEINDEX = TARGETINDEX;
}
}
void PSTYPEDEFINITIONS() {
PString NAME;
struct record9 *TYPEINDEX;
TPSTYPE NEWTYPE;
struct record19 *CHECKPOINT;
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
if ((LEXER.TOKEN.ID == TKFALSE) || (LEXER.TOKEN.ID == TKTRUE) || (LEXER.TOKEN.ID == TKNUMBER) || (LEXER.TOKEN.ID == TKSTRING)) CONSTANT.REPLACEMENT = LEXER.TOKEN;
 else COMPILEERROR(cat_ss(str_make(31, "Expected constant value, found "), LXTOKENSTR()));
ADDCONSTANT(CONSTANT);
READTOKEN();
}
void PSCONSTANTVALUE(struct record9 *TYPEINDEX) {
TPSEXPRESSION EXPR;
if (ISBOOLEANTYPE(TYPEINDEX)) {
WANTTOKEN2(TKFALSE, TKTRUE);
EXPR = EXPRBOOLEANCONSTANT(LEXER.TOKEN.ID == TKTRUE);
READTOKEN();
OUTCONSTANTVALUE(EXPR);
}
 else if (ISINTEGERTYPE(TYPEINDEX)) {
EXPR = EXPRNUMBERCONSTANT(GETTOKENVALUEANDREAD(TKNUMBER));
OUTCONSTANTVALUE(EXPR);
}
 else if (ISCHARTYPE(TYPEINDEX)) {
EXPR = EXPRSTRINGCONSTANT(GETTOKENVALUEANDREAD(TKSTRING));
if (!ISCHARTYPE(EXPR.TYPEINDEX)) COMPILEERROR(cat_ss(str_make(28, "Expected char constant, got "), TYPENAME(EXPR.TYPEINDEX)));
OUTCONSTANTVALUE(EXPR);
}
 else if (ISSTRINGTYPE(TYPEINDEX)) {
EXPR = EXPRSTRINGCONSTANT(GETTOKENVALUEANDREAD(TKSTRING));
OUTCONSTANTVALUE(EXPR);
}
 else if (ISARRAYTYPE(TYPEINDEX)) {
WANTTOKENANDREAD(TKLPAREN);
TYPEINDEX = (*((*(TYPEINDEX)).ARRAYINDEX)).TYPEINDEX;
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
 else COMPILEERROR(cat_ss(str_make(27, "Invalid type for constant: "), TYPENAME(TYPEINDEX)));
}
void PSTYPEDCONSTANT(PString NAME) {
struct record9 *TYPEINDEX;
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
PString NAMES[1 + 8 - 1];
struct record9 *TYPEINDEX;
struct record19 *CHECKPOINT;
CHECKPOINT = DEFS.LATEST;
WANTTOKENANDREAD(TKVAR);
do {
NUMNAMES = 0;
do {
NUMNAMES = NUMNAMES + 1;
if (NUMNAMES > 8) COMPILEERROR(str_make(37, "Too many names in variable definition"));
NAMES[(NUMNAMES) - 1] = GETTOKENVALUEANDREAD(TKIDENTIFIER);
WANTTOKEN2(TKCOMMA, TKCOLON);
SKIPTOKEN(TKCOMMA);
} while (!(LEXER.TOKEN.ID == TKCOLON));
WANTTOKENANDREAD(TKCOLON);
TYPEINDEX = PSTYPEDENOTER();
WANTTOKENANDREAD(TKSEMICOLON);
{
int first = 1;
int last = NUMNAMES;
if (first <= last) {
NUMNAMES = first;
while (1) {
OUTVARIABLEDEFINITION(ADDVARIABLE(MAKEVARIABLE(NAMES[(NUMNAMES) - 1], TYPEINDEX, 0)));
if (NUMNAMES == last) break;
++NUMNAMES;
}
}
}
} while (!(LEXER.TOKEN.ID != TKIDENTIFIER));
OUTENUMVALUESFROMCHECKPOINT(CHECKPOINT);
}
void PSSTATEMENT();
void PSDEFINITIONS();
void PSFUNCTIONBODY(struct record16 *FNINDEX) {
int POS;
struct record19 *CHECKPOINT;
STARTLOCALSCOPE();
CHECKPOINT = DEFS.LATEST;
{
int first = 1;
int last = (*(FNINDEX)).ARGCOUNT;
if (first <= last) {
POS = first;
while (1) {
ADDVARIABLE((*(FNINDEX)).ARGS[(POS) - 1]);
if (POS == last) break;
++POS;
}
}
}
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
struct record9 *TYPEINDEX;
WANTTOKENANDREAD(TKLPAREN);
do {
ISREFERENCE = LEXER.TOKEN.ID == TKVAR;
SKIPTOKEN(TKVAR);
LASTARG = (*DEF).ARGCOUNT;
do {
(*DEF).ARGCOUNT = (*DEF).ARGCOUNT + 1;
if ((*DEF).ARGCOUNT > 4) COMPILEERROR(cat_ss(str_make(41, "Too many arguments declared for function "), (*DEF).NAME));
(*DEF).ARGS[((*DEF).ARGCOUNT) - 1].NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
(*DEF).ARGS[((*DEF).ARGCOUNT) - 1].ISREFERENCE = ISREFERENCE;
WANTTOKEN2(TKCOLON, TKCOMMA);
SKIPTOKEN(TKCOMMA);
} while (!(LEXER.TOKEN.ID == TKCOLON));
SKIPTOKEN(TKCOLON);
TYPEINDEX = PSTYPEIDENTIFIER();
{
int first = LASTARG + 1;
int last = (*DEF).ARGCOUNT;
if (first <= last) {
ARG = first;
while (1) {
(*DEF).ARGS[(ARG) - 1].TYPEINDEX = TYPEINDEX;
if (ARG == last) break;
++ARG;
}
}
}
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
struct record9 *PSRESULTTYPE() {
struct record9 *return_PSRESULTTYPE;
return_PSRESULTTYPE = PSTYPEIDENTIFIER();
return return_PSRESULTTYPE;
}
void PSFUNCTIONDEFINITION() {
TPSFUNCTION DEF;
DEF = EMPTYFUNCTION();
WANTTOKENANDREAD(TKFUNCTION);
DEF.NAME = GETTOKENVALUEANDREAD(TKIDENTIFIER);
if ((LEXER.TOKEN.ID == TKSEMICOLON) && HASFORWARDDECLARATION(DEF.NAME)) DEF.RETURNTYPEINDEX = ((void*)0);
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
} while (!(DONE));
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
TPSEXPRESSION PSFUNCTIONCALL(TPSEXPRESSION FN) {
TPSEXPRESSION return_PSFUNCTIONCALL;
TPSEXPRESSION EXPR;
int ARGNUM;
if (FN.CLS != TECFUNCTION) COMPILEERROR(str_make(14, "Not a function"));
FN.VALUE = GENFUNCTIONCALLSTART(FN.VALUE);
WANTTOKENANDREAD(TKLPAREN);
{
int first = 1;
int last = (*(FN.FUNCTIONINDEX)).ARGCOUNT;
if (first <= last) {
ARGNUM = first;
while (1) {
{
if (ARGNUM != 1) WANTTOKENANDREAD(TKCOMMA);
EXPR = EXPRCOERCE(PSEXPRESSION(), (*(FN.FUNCTIONINDEX)).ARGS[(ARGNUM) - 1].TYPEINDEX);
FN.VALUE = GENFUNCTIONCALLARGUMENT(FN.VALUE, EXPR, (*(FN.FUNCTIONINDEX)).ARGS[(ARGNUM) - 1].ISREFERENCE, ARGNUM);
}
if (ARGNUM == last) break;
++ARGNUM;
}
}
}
WANTTOKENANDREAD(TKRPAREN);
FN.VALUE = GENFUNCTIONCALLEND(FN.VALUE);
FN.CLS = TECVALUE;
FN.TYPEINDEX = (*(FN.FUNCTIONINDEX)).RETURNTYPEINDEX;
FN.ISCONSTANT = 1;
return_PSFUNCTIONCALL = FN;
return return_PSFUNCTIONCALL;
}
TPSEXPRESSION PSPOINTERDEREF(TPSEXPRESSION PTR) {
TPSEXPRESSION return_PSPOINTERDEREF;
WANTTOKENANDREAD(TKCARET);
return_PSPOINTERDEREF = EXPRPOINTERDEREF(EXPREVALUATE(PTR));
return return_PSPOINTERDEREF;
}
void PSREAD(TPSSPECIALFUNCTION FN) {
TPSEXPRESSION SRC;
PBoolean LINEFEED;
TPSEXPRESSION OUTVAR;
LINEFEED = FN == TSFREADLN;
SRC = EXPRVARIABLEACCESS((*(FINDNAMEOFCLASS(str_make(5, "INPUT"), TNCVARIABLE, 1))).VARIABLEINDEX);
if (LEXER.TOKEN.ID != TKLPAREN) {
if (LINEFEED) OUTREADLN(SRC);
}
 else {
OUTBEGIN();
WANTTOKENANDREAD(TKLPAREN);
if (LEXER.TOKEN.ID != TKRPAREN) {
OUTVAR = PSEXPRESSION();
if (EXPRISVARIABLE(OUTVAR) && ISTEXTTYPE(OUTVAR.TYPEINDEX)) SRC = OUTVAR;
 else {
if (!EXPRISVARIABLE(OUTVAR) || !ISSTRINGYTYPE(OUTVAR.TYPEINDEX)) COMPILEERROR(str_make(36, "Invalid expression for read argument"));
OUTREAD(SRC, OUTVAR);
}
WANTTOKEN2(TKCOMMA, TKRPAREN);
SKIPTOKEN(TKCOMMA);
while (LEXER.TOKEN.ID != TKRPAREN) {
OUTVAR = PSEXPRESSION();
if (!EXPRISVARIABLE(OUTVAR) || !ISSTRINGYTYPE(OUTVAR.TYPEINDEX)) COMPILEERROR(str_make(36, "Invalid expression for read argument"));
OUTREAD(SRC, OUTVAR);
WANTTOKEN2(TKCOMMA, TKRPAREN);
SKIPTOKEN(TKCOMMA);
}
}
WANTTOKENANDREAD(TKRPAREN);
if (LINEFEED) OUTREADLN(SRC);
OUTEND();
}
}
void PSWRITE(TPSSPECIALFUNCTION FN) {
TPSEXPRESSION DST;
PBoolean LINEFEED;
TPSEXPRESSION EXPR;
LINEFEED = FN == TSFWRITELN;
DST = EXPRVARIABLEACCESS((*(FINDNAMEOFCLASS(str_make(6, "OUTPUT"), TNCVARIABLE, 1))).VARIABLEINDEX);
if (LEXER.TOKEN.ID != TKLPAREN) {
if (LINEFEED) OUTWRITELN(DST);
}
 else {
OUTBEGIN();
WANTTOKENANDREAD(TKLPAREN);
if (LEXER.TOKEN.ID != TKRPAREN) {
EXPR = PSEXPRESSION();
if (EXPRISVARIABLE(EXPR) && ISTEXTTYPE(EXPR.TYPEINDEX)) DST = EXPR;
 else OUTWRITE(DST, EXPR);
WANTTOKEN2(TKCOMMA, TKRPAREN);
SKIPTOKEN(TKCOMMA);
while (LEXER.TOKEN.ID != TKRPAREN) {
OUTWRITE(DST, PSEXPRESSION());
WANTTOKEN2(TKCOMMA, TKRPAREN);
SKIPTOKEN(TKCOMMA);
}
}
WANTTOKENANDREAD(TKRPAREN);
if (LINEFEED) OUTWRITELN(DST);
OUTEND();
}
}
void PSSTR() {
TPSEXPRESSION EXPR;
TPSEXPRESSION DEST;
WANTTOKENANDREAD(TKLPAREN);
EXPR = PSEXPRESSION();
WANTTOKENANDREAD(TKCOMMA);
DEST = PSEXPRESSION();
if (!EXPRISVARIABLE(DEST) || !ISSTRINGTYPE(DEST.TYPEINDEX)) COMPILEERROR(str_make(45, "Destination argument is not a string variable"));
WANTTOKENANDREAD(TKRPAREN);
OUTSTR(DEST.VALUE, EXPR);
}
void PSNEW() {
TPSEXPRESSION DEST;
WANTTOKENANDREAD(TKLPAREN);
DEST = PSEXPRESSION();
WANTTOKENANDREAD(TKRPAREN);
if (!EXPRISVARIABLE(DEST) || !ISPOINTERTYPE(DEST.TYPEINDEX)) COMPILEERROR(str_make(25, "Argument is not a pointer"));
OUTNEW(DEST);
}
void PSDISPOSE() {
TPSEXPRESSION DEST;
WANTTOKENANDREAD(TKLPAREN);
DEST = PSEXPRESSION();
WANTTOKENANDREAD(TKRPAREN);
if (!EXPRISVARIABLE(DEST) || !ISPOINTERTYPE(DEST.TYPEINDEX)) COMPILEERROR(str_make(25, "Argument is not a pointer"));
OUTDISPOSE(DEST);
}
TPSEXPRESSION PSARRAYACCESS(TPSEXPRESSION ARR) {
TPSEXPRESSION return_PSARRAYACCESS;
TPSEXPRESSION IDX;
WANTTOKENANDREAD(TKLBRACKET);
IDX = PSEXPRESSION();
WANTTOKENANDREAD(TKRBRACKET);
return_PSARRAYACCESS = EXPRARRAYACCESS(ARR, IDX);
return return_PSARRAYACCESS;
}
TPSEXPRESSION PSFIELDACCESS(TPSEXPRESSION REC) {
TPSEXPRESSION return_PSFIELDACCESS;
TPSIDENTIFIER FLD;
WANTTOKENANDREAD(TKDOT);
FLD = PSIDENTIFIER();
return_PSFIELDACCESS = EXPRFIELDACCESS(REC, FLD.NAME);
return return_PSFIELDACCESS;
}
TPSEXPRESSION PSVARIABLEORFUNCTIONCALL() {
TPSEXPRESSION return_PSVARIABLEORFUNCTIONCALL;
TPSIDENTIFIER ID;
struct record17 *WITHVARINDEX;
TPSNAME FOUND;
TPSEXPRESSION EXPR;
PBoolean DONE;
DONE = 0;
ID = PSIDENTIFIER();
WITHVARINDEX = FINDWITHVAR(ID.NAME);
if (WITHVARINDEX != ((void*)0)) {
EXPR = EXPRVARIABLEACCESS((*(WITHVARINDEX)).VARIABLEINDEX);
EXPR = EXPRFIELDACCESS(EXPR, ID.NAME);
}
 else {
FOUND = *(FINDNAME(ID.NAME, 1));
if (FOUND.CLS == TNCVARIABLE) EXPR = EXPRVARIABLEACCESS(FOUND.VARIABLEINDEX);
 else if (FOUND.CLS == TNCFUNCTION) EXPR = EXPRFUNCTIONREFERENCE(FOUND.FUNCTIONINDEX);
 else if (FOUND.CLS == TNCENUMVALUE) EXPR = EXPRENUMVALUE(FOUND.ORDINAL, FOUND.ENUMTYPEINDEX);
 else if (FOUND.CLS == TNCSPECIALFUNCTION) {
EXPR.CLS = TECSTATEMENT;
EXPR.TYPEINDEX = ((void*)0);
if ((FOUND.SPECIALFUNCTION == TSFREAD) || (FOUND.SPECIALFUNCTION == TSFREADLN)) PSREAD(FOUND.SPECIALFUNCTION);
 else if ((FOUND.SPECIALFUNCTION == TSFWRITE) || (FOUND.SPECIALFUNCTION == TSFWRITELN)) PSWRITE(FOUND.SPECIALFUNCTION);
 else if (FOUND.SPECIALFUNCTION == TSFSTR) PSSTR();
 else if (FOUND.SPECIALFUNCTION == TSFNEW) PSNEW();
 else if (FOUND.SPECIALFUNCTION == TSFDISPOSE) PSDISPOSE();
}
 else COMPILEERROR(cat_ss(str_make(20, "Invalid identifier: "), ID.NAME));
}
do {
if (LEXER.TOKEN.ID == TKDOT) EXPR = PSFIELDACCESS(EXPR);
 else if (LEXER.TOKEN.ID == TKLBRACKET) EXPR = PSARRAYACCESS(EXPR);
 else if (LEXER.TOKEN.ID == TKLPAREN) EXPR = PSFUNCTIONCALL(EXPR);
 else if (LEXER.TOKEN.ID == TKCARET) EXPR = PSPOINTERDEREF(EXPR);
 else DONE = 1;
} while (!(DONE));
return_PSVARIABLEORFUNCTIONCALL = EXPR;
return return_PSVARIABLEORFUNCTIONCALL;
}
PBoolean ISOPADDING(TLXTOKEN TOK) {
PBoolean return_ISOPADDING;
return_ISOPADDING = (TOK.ID == TKPLUS) || (TOK.ID == TKMINUS) || (TOK.ID == TKOR);
return return_ISOPADDING;
}
PBoolean ISOPMULTIPLYING(TLXTOKEN TOK) {
PBoolean return_ISOPMULTIPLYING;
return_ISOPMULTIPLYING = (TOK.ID == TKASTERISK) || (TOK.ID == TKSLASH) || (TOK.ID == TKDIV) || (TOK.ID == TKMOD) || (TOK.ID == TKAND);
return return_ISOPMULTIPLYING;
}
PBoolean ISOPRELATIONAL(TLXTOKEN TOK) {
PBoolean return_ISOPRELATIONAL;
return_ISOPRELATIONAL = (TOK.ID == TKEQUALS) || (TOK.ID == TKNOTEQUALS) || (TOK.ID == TKLESSTHAN) || (TOK.ID == TKMORETHAN) || (TOK.ID == TKLESSOREQUALS) || (TOK.ID == TKMOREOREQUALS) || (TOK.ID == TKIN);
return return_ISOPRELATIONAL;
}
TPSEXPRESSION PSFACTOR() {
TPSEXPRESSION return_PSFACTOR;
TPSEXPRESSION EXPR;
if (LEXER.TOKEN.ID == TKNIL) {
EXPR = EXPRNILCONSTANT();
READTOKEN();
}
 else if ((LEXER.TOKEN.ID == TKFALSE) || (LEXER.TOKEN.ID == TKTRUE)) {
EXPR = EXPRBOOLEANCONSTANT(LEXER.TOKEN.ID == TKTRUE);
READTOKEN();
}
 else if (LEXER.TOKEN.ID == TKSTRING) EXPR = EXPRSTRINGCONSTANT(GETTOKENVALUEANDREAD(TKSTRING));
 else if (LEXER.TOKEN.ID == TKNUMBER) EXPR = EXPRNUMBERCONSTANT(GETTOKENVALUEANDREAD(TKNUMBER));
 else if (LEXER.TOKEN.ID == TKIDENTIFIER) EXPR = PSVARIABLEORFUNCTIONCALL();
 else if (LEXER.TOKEN.ID == TKLPAREN) {
WANTTOKENANDREAD(TKLPAREN);
EXPR = EXPRPARENTHESES(PSEXPRESSION());
WANTTOKENANDREAD(TKRPAREN);
}
 else if (LEXER.TOKEN.ID == TKNOT) {
WANTTOKENANDREAD(TKNOT);
EXPR = EXPRUNARYOP(TKNOT, PSFACTOR());
}
 else COMPILEERROR(cat_ss(str_make(29, "Invalid token in expression: "), LXTOKENSTR()));
return_PSFACTOR = EXPR;
return return_PSFACTOR;
}
TPSEXPRESSION PSTERM() {
TPSEXPRESSION return_PSTERM;
TLXTOKENID OP;
TPSEXPRESSION EXPR;
EXPR = PSFACTOR();
while (ISOPMULTIPLYING(LEXER.TOKEN)) {
OP = LEXER.TOKEN.ID;
READTOKEN();
EXPR = EXPRBINARYOP(EXPR, OP, PSFACTOR());
}
return_PSTERM = EXPR;
return return_PSTERM;
}
TPSEXPRESSION PSSIMPLEEXPRESSION() {
TPSEXPRESSION return_PSSIMPLEEXPRESSION;
PBoolean NEGATIVE;
TLXTOKENID OP;
TPSEXPRESSION EXPR;
NEGATIVE = LEXER.TOKEN.ID == TKMINUS;
if (NEGATIVE) READTOKEN();
EXPR = PSTERM();
if (NEGATIVE) EXPR = EXPRUNARYOP(TKMINUS, EXPR);
while (ISOPADDING(LEXER.TOKEN)) {
OP = LEXER.TOKEN.ID;
READTOKEN();
EXPR = EXPRBINARYOP(EXPR, OP, PSTERM());
}
return_PSSIMPLEEXPRESSION = EXPR;
return return_PSSIMPLEEXPRESSION;
}
TPSEXPRESSION PSEXPRESSION() {
TPSEXPRESSION return_PSEXPRESSION;
TLXTOKENID OP;
TPSEXPRESSION EXPR;
EXPR = PSSIMPLEEXPRESSION();
while (ISOPRELATIONAL(LEXER.TOKEN)) {
OP = LEXER.TOKEN.ID;
READTOKEN();
EXPR = EXPRBINARYOP(EXPR, OP, PSSIMPLEEXPRESSION());
}
return_PSEXPRESSION = EXPR;
return return_PSEXPRESSION;
}
void PSASSIGN(TPSEXPRESSION LHS, TPSEXPRESSION RHS) {
if (LHS.ISCONSTANT) COMPILEERROR(str_make(33, "Cannot assign to a constant value"));
if (LHS.CLS == TECFUNCTION) OUTASSIGNRETURNVALUE(LHS, EXPRCOERCE(RHS, (*(LHS.FUNCTIONINDEX)).RETURNTYPEINDEX));
 else if (LHS.CLS == TECVALUE) OUTASSIGN(LHS, EXPRCOERCE(RHS, LHS.TYPEINDEX));
 else COMPILEERROR(str_make(36, "Cannot assign to result of statement"));
}
void PSSTATEMENTSEQUENCE() {
OUTBEGIN();
SKIPTOKEN(TKBEGIN);
while (LEXER.TOKEN.ID != TKEND) {
PSSTATEMENT();
WANTTOKEN2(TKSEMICOLON, TKEND);
SKIPTOKEN(TKSEMICOLON);
}
OUTEND();
SKIPTOKEN(TKEND);
}
void PSIDENTIFIERSTATEMENT() {
TPSEXPRESSION LHS;
LHS = PSEXPRESSION();
if (LEXER.TOKEN.ID == TKASSIGN) {
WANTTOKENANDREAD(TKASSIGN);
PSASSIGN(LHS, PSEXPRESSION());
}
 else if (LHS.CLS != TECSTATEMENT) {
if (LHS.CLS == TECFUNCTION) LHS = EXPREVALUATE(LHS);
OUTPROCEDURECALL(LHS);
}
}
void PSIFSTATEMENT() {
WANTTOKENANDREAD(TKIF);
OUTIF(EXPRCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN));
WANTTOKENANDREAD(TKTHEN);
if (LEXER.TOKEN.ID == TKELSE) OUTEMPTYSTATEMENT();
 else PSSTATEMENT();
if (LEXER.TOKEN.ID == TKELSE) {
WANTTOKENANDREAD(TKELSE);
OUTELSE();
PSSTATEMENT();
}
}
void PSCASESTATEMENT() {
TPSEXPRESSION CASEINDEX;
TPSEXPRESSION CASELABEL;
WANTTOKENANDREAD(TKCASE);
CASEINDEX = PSEXPRESSION();
if (!ISORDINALTYPE(CASEINDEX.TYPEINDEX)) COMPILEERROR(str_make(46, "The index of the case statement is not ordinal"));
OUTCASEBEGIN(CASEINDEX);
WANTTOKENANDREAD(TKOF);
do {
CASELABEL = EXPRCOERCE(PSEXPRESSION(), CASEINDEX.TYPEINDEX);
if (!CASELABEL.ISCONSTANT) COMPILEERROR(str_make(47, "The label of the case statement is not constant"));
WANTTOKENANDREAD(TKCOLON);
OUTCASESTATEMENTBEGIN(CASELABEL);
PSSTATEMENT();
OUTCASESTATEMENTEND();
WANTTOKEN3(TKSEMICOLON, TKELSE, TKEND);
SKIPTOKEN(TKSEMICOLON);
} while (!((LEXER.TOKEN.ID == TKELSE) || (LEXER.TOKEN.ID == TKEND)));
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
WANTTOKENANDREAD(TKREPEAT);
OUTREPEATBEGIN();
while (LEXER.TOKEN.ID != TKUNTIL) {
PSSTATEMENT();
WANTTOKEN2(TKSEMICOLON, TKUNTIL);
SKIPTOKEN(TKSEMICOLON);
}
WANTTOKENANDREAD(TKUNTIL);
OUTREPEATEND(EXPRCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN));
}
void PSWHILESTATEMENT() {
WANTTOKENANDREAD(TKWHILE);
OUTWHILEBEGIN(EXPRCOERCE(PSEXPRESSION(), PRIMITIVETYPES.PTBOOLEAN));
WANTTOKENANDREAD(TKDO);
PSSTATEMENT();
OUTWHILEEND();
}
void PSFORSTATEMENT() {
TPSEXPRESSION ITER;
TPSEXPRESSION FIRST;
TPSEXPRESSION LAST;
PBoolean ASCENDING;
WANTTOKENANDREAD(TKFOR);
ITER = PSEXPRESSION();
if (!EXPRISVARIABLE(ITER)) COMPILEERROR(str_make(17, "Expected variable"));
if (!ISORDINALTYPE(ITER.TYPEINDEX)) COMPILEERROR(cat_ss(str_make(33, "Type of iterator is not ordinal: "), TYPENAME(ITER.TYPEINDEX)));
WANTTOKENANDREAD(TKASSIGN);
FIRST = EXPREVALUATE(PSEXPRESSION());
WANTTOKEN2(TKTO, TKDOWNTO);
ASCENDING = LEXER.TOKEN.ID == TKTO;
READTOKEN();
LAST = EXPREVALUATE(PSEXPRESSION());
WANTTOKENANDREAD(TKDO);
OUTFORBEGIN(ITER, FIRST, LAST, ASCENDING);
PSSTATEMENT();
OUTFOREND(ITER, ASCENDING);
}
void PSWITHSTATEMENT() {
TPSEXPRESSION BASE;
struct record15 *VARINDEX;
WANTTOKEN(TKWITH);
STARTTEMPORARYSCOPE();
OUTBEGIN();
do {
READTOKEN();
BASE = PSEXPRESSION();
VARINDEX = ADDWITHVAR(BASE);
OUTASSIGNTOREFERENCE(VARINDEX, BASE);
WANTTOKEN2(TKCOMMA, TKDO);
} while (!(LEXER.TOKEN.ID == TKDO));
WANTTOKENANDREAD(TKDO);
PSSTATEMENT();
OUTEND();
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
if ((LENGTH(DIR) > 3) && (cmp_cc(DIR.chr[2], 'I') == 0) && (cmp_cc(DIR.chr[3], ' ') == 0)) LXINCLUDE(COPY(DIR, 4, 255));
}
void READTOKEN() {
struct record14 *CONSTINDEX;
TLXPOS TOKENPOS;
PBoolean STOP;
do {
LXREADTOKEN();
STOP = LEXER.TOKEN.ID != TKCOMMENT;
if (LEXER.TOKEN.ID == TKIDENTIFIER) {
CONSTINDEX = FINDCONSTANT(LEXER.TOKEN.VALUE);
if (CONSTINDEX != ((void*)0)) {
TOKENPOS = LEXER.TOKEN.POS;
LEXER.TOKEN = (*(CONSTINDEX)).REPLACEMENT;
LEXER.TOKEN.POS = TOKENPOS;
}
}
if (LEXER.TOKEN.ID == TKCOMMENT) if ((LENGTH(LEXER.TOKEN.VALUE) >= 2) && (cmp_cc(LEXER.TOKEN.VALUE.chr[1], '$') == 0)) EXECUTEDIRECTIVE(LEXER.TOKEN.VALUE);
if ((LEXER.TOKEN.ID == TKEOF) && LEXER.PREV.EXISTS) {
LEXER.INPUT = LEXER.PREV.INPUT;
LEXER.PREV.EXISTS = 0;
STOP = 0;
}
} while (!(STOP));
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
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
FUN.ARGS[(2) - 1] = MAKEVARIABLE(str_make(4, "NAME"), PRIMITIVETYPES.PTSTRING, 0);
FUN.RETURNTYPEINDEX = ((void*)0);
ADDFUNCTION(FUN);
FUN.NAME = str_make(3, "CHR");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 0);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTCHAR;
ADDFUNCTION(FUN);
FUN.NAME = str_make(5, "CLOSE");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
FUN.RETURNTYPEINDEX = ((void*)0);
ADDFUNCTION(FUN);
FUN.NAME = str_make(4, "COPY");
FUN.ARGCOUNT = 3;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING, 0);
FUN.ARGS[(2) - 1] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 0);
FUN.ARGS[(3) - 1] = MAKEVARIABLE(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER, 0);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTSTRING;
ADDFUNCTION(FUN);
FUN.NAME = str_make(6, "DELETE");
FUN.ARGCOUNT = 3;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING, 1);
FUN.ARGS[(2) - 1] = MAKEVARIABLE(str_make(3, "POS"), PRIMITIVETYPES.PTINTEGER, 0);
FUN.ARGS[(3) - 1] = MAKEVARIABLE(str_make(3, "NUM"), PRIMITIVETYPES.PTINTEGER, 0);
FUN.RETURNTYPEINDEX = ((void*)0);
ADDFUNCTION(FUN);
FUN.NAME = str_make(3, "EOF");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTBOOLEAN;
ADDFUNCTION(FUN);
FUN.NAME = str_make(4, "HALT");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(4, "CODE"), PRIMITIVETYPES.PTINTEGER, 0);
FUN.RETURNTYPEINDEX = ((void*)0);
ADDFUNCTION(FUN);
FUN.NAME = str_make(6, "LENGTH");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(3, "STR"), PRIMITIVETYPES.PTSTRING, 0);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTINTEGER;
ADDFUNCTION(FUN);
FUN.NAME = str_make(3, "ORD");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(3, "CHR"), PRIMITIVETYPES.PTCHAR, 0);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTINTEGER;
ADDFUNCTION(FUN);
FUN.NAME = str_make(10, "PARAMCOUNT");
FUN.ARGCOUNT = 0;
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTINTEGER;
ADDFUNCTION(FUN);
FUN.NAME = str_make(8, "PARAMSTR");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_of('I'), PRIMITIVETYPES.PTINTEGER, 0);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTSTRING;
ADDFUNCTION(FUN);
FUN.NAME = str_make(5, "RESET");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
FUN.RETURNTYPEINDEX = ((void*)0);
ADDFUNCTION(FUN);
FUN.NAME = str_make(7, "REWRITE");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_of('F'), PRIMITIVETYPES.PTTEXT, 1);
FUN.RETURNTYPEINDEX = ((void*)0);
ADDFUNCTION(FUN);
FUN.NAME = str_make(6, "UPCASE");
FUN.ARGCOUNT = 1;
FUN.ARGS[(1) - 1] = MAKEVARIABLE(str_make(3, "CHR"), PRIMITIVETYPES.PTCHAR, 0);
FUN.RETURNTYPEINDEX = PRIMITIVETYPES.PTCHAR;
ADDFUNCTION(FUN);
}
struct record22 { PFile OUTPUT; } CODEGEN;
void OUTBEGIN() {
{
write_c(&CODEGEN.OUTPUT, '{');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTEND() {
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTENUMVALUES(struct record10 *ENUMINDEX) {
int POSINENUM;
{
write_s(&CODEGEN.OUTPUT, str_make(22, "const char* enumvalues"));
write_i(&CODEGEN.OUTPUT, (*(ENUMINDEX)).ID);
write_s(&CODEGEN.OUTPUT, str_make(7, "[] = { "));
}
{
int first = 1;
int last = (*(ENUMINDEX)).SIZE;
if (first <= last) {
POSINENUM = first;
while (1) {
{
if (POSINENUM != 1) {
write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
}
{
write_c(&CODEGEN.OUTPUT, '\"');
write_s(&CODEGEN.OUTPUT, (*(ENUMINDEX)).VALUES[(POSINENUM) - 1]);
write_c(&CODEGEN.OUTPUT, '\"');
}
}
if (POSINENUM == last) break;
++POSINENUM;
}
}
}
{
write_s(&CODEGEN.OUTPUT, str_make(3, " };"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTENUMVALUESFROMCHECKPOINT(struct record19 *CHECKPOINT) {
struct record19 *DEF;
DEF = (*(CHECKPOINT)).NEXT;
while (DEF != ((void*)0)) {
if ((*(DEF)).CLS == TDCENUM) OUTENUMVALUES((*(DEF)).ENUMINDEX);
DEF = (*(DEF)).NEXT;
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
void OUTTYPEREFERENCE(struct record9 *TYPEINDEX) {
if (TYPEINDEX == ((void*)0)) {
write_s(&CODEGEN.OUTPUT, str_make(4, "void"));
}
 else if ((*(TYPEINDEX)).CLS == TTCPOINTER) {
OUTTYPEREFERENCE((*(TYPEINDEX)).POINTEDTYPEINDEX);
{
write_c(&CODEGEN.OUTPUT, '*');
}
}
 else if ((*(TYPEINDEX)).CLS == TTCBOOLEAN) {
write_s(&CODEGEN.OUTPUT, str_make(8, "PBoolean"));
}
 else if ((*(TYPEINDEX)).CLS == TTCINTEGER) {
write_s(&CODEGEN.OUTPUT, str_make(3, "int"));
}
 else if ((*(TYPEINDEX)).CLS == TTCCHAR) {
write_s(&CODEGEN.OUTPUT, str_make(4, "char"));
}
 else if ((*(TYPEINDEX)).CLS == TTCSTRING) {
write_s(&CODEGEN.OUTPUT, str_make(7, "PString"));
}
 else if ((*(TYPEINDEX)).CLS == TTCTEXT) {
write_s(&CODEGEN.OUTPUT, str_make(5, "PFile"));
}
 else if ((*(TYPEINDEX)).CLS == TTCENUM) {
write_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
write_i(&CODEGEN.OUTPUT, (*((*(TYPEINDEX)).ENUMINDEX)).ID);
}
 else if ((*(TYPEINDEX)).CLS == TTCRECORD) {
write_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
write_i(&CODEGEN.OUTPUT, (*((*(TYPEINDEX)).RECORDINDEX)).ID);
}
 else if ((*(TYPEINDEX)).CLS == TTCARRAY) {
OUTTYPEREFERENCE((*((*(TYPEINDEX)).ARRAYINDEX)).TYPEINDEX);
{
write_c(&CODEGEN.OUTPUT, '*');
}
}
 else COMPILEERROR(cat_ss(str_make(30, "Error writing type reference: "), TYPENAME(TYPEINDEX)));
}
void OUTNAMEANDRECORD(PString NAME, struct record12 *RECORDINDEX) {
int POS;
int NUMVARIANT;
NUMVARIANT = 0;
{
write_s(&CODEGEN.OUTPUT, str_make(13, "struct record"));
write_i(&CODEGEN.OUTPUT, (*(RECORDINDEX)).ID);
}
if (!(*(RECORDINDEX)).HASBEENDEFINED) {
{
write_s(&CODEGEN.OUTPUT, str_make(3, " { "));
}
{
int first = 1;
int last = (*(RECORDINDEX)).SIZE;
if (first <= last) {
POS = first;
while (1) {
{
if (((*(RECORDINDEX)).NUMVARIANTS > NUMVARIANT) && ((*(RECORDINDEX)).VARIANTBOUNDS[(NUMVARIANT + 1) - 1] == POS)) {
NUMVARIANT = NUMVARIANT + 1;
if (NUMVARIANT == 1) {
write_s(&CODEGEN.OUTPUT, str_make(8, "union { "));
}
 else {
write_s(&CODEGEN.OUTPUT, str_make(3, "}; "));
}
{
write_s(&CODEGEN.OUTPUT, str_make(9, "struct { "));
}
}
OUTNAMEANDTYPE((*(RECORDINDEX)).FIELDS[(POS) - 1].NAME, (*(RECORDINDEX)).FIELDS[(POS) - 1].TYPEINDEX);
{
write_s(&CODEGEN.OUTPUT, str_make(2, "; "));
}
}
if (POS == last) break;
++POS;
}
}
}
if (NUMVARIANT > 0) {
write_s(&CODEGEN.OUTPUT, str_make(6, "}; }; "));
}
{
write_c(&CODEGEN.OUTPUT, '}');
}
(*(RECORDINDEX)).HASBEENDEFINED = 1;
}
{
write_c(&CODEGEN.OUTPUT, ' ');
write_s(&CODEGEN.OUTPUT, NAME);
}
}
void OUTNAMEANDENUM(PString NAME, struct record10 *ENUMINDEX) {
int POS;
{
write_s(&CODEGEN.OUTPUT, str_make(9, "enum enum"));
write_i(&CODEGEN.OUTPUT, (*(ENUMINDEX)).ID);
}
if (!(*(ENUMINDEX)).HASBEENDEFINED) {
{
write_s(&CODEGEN.OUTPUT, str_make(3, " { "));
}
{
int first = 1;
int last = (*(ENUMINDEX)).SIZE;
if (first <= last) {
POS = first;
while (1) {
{
if (POS > 1) {
write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
}
{
write_s(&CODEGEN.OUTPUT, (*(ENUMINDEX)).VALUES[(POS) - 1]);
}
}
if (POS == last) break;
++POS;
}
}
}
{
write_c(&CODEGEN.OUTPUT, '}');
}
(*(ENUMINDEX)).HASBEENDEFINED = 1;
}
{
write_c(&CODEGEN.OUTPUT, ' ');
write_s(&CODEGEN.OUTPUT, NAME);
}
}
void OUTNAMEANDTYPE(PString NAME, struct record9 *TYPEINDEX) {
TPSARRAYDEF ARR;
if (TYPEINDEX == ((void*)0)) {
write_s(&CODEGEN.OUTPUT, str_make(5, "void "));
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCPOINTER) {
OUTTYPEREFERENCE((*(TYPEINDEX)).POINTEDTYPEINDEX);
{
write_s(&CODEGEN.OUTPUT, str_make(2, " *"));
write_s(&CODEGEN.OUTPUT, NAME);
}
}
 else if (((*(TYPEINDEX)).ALIASFOR != ((void*)0)) && (cmp_ss((*(TYPEINDEX)).NAME, str_make(0, "")) != 0)) {
write_s(&CODEGEN.OUTPUT, (*(TYPEINDEX)).NAME);
write_c(&CODEGEN.OUTPUT, ' ');
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCBOOLEAN) {
write_s(&CODEGEN.OUTPUT, str_make(9, "PBoolean "));
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCINTEGER) {
write_s(&CODEGEN.OUTPUT, str_make(4, "int "));
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCCHAR) {
write_s(&CODEGEN.OUTPUT, str_make(5, "char "));
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCSTRING) {
write_s(&CODEGEN.OUTPUT, str_make(8, "PString "));
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCTEXT) {
write_s(&CODEGEN.OUTPUT, str_make(6, "PFile "));
write_s(&CODEGEN.OUTPUT, NAME);
}
 else if ((*(TYPEINDEX)).CLS == TTCENUM) OUTNAMEANDENUM(NAME, (*(TYPEINDEX)).ENUMINDEX);
 else if ((*(TYPEINDEX)).CLS == TTCRECORD) OUTNAMEANDRECORD(NAME, (*(TYPEINDEX)).RECORDINDEX);
 else if ((*(TYPEINDEX)).CLS == TTCARRAY) {
ARR = *((*(TYPEINDEX)).ARRAYINDEX);
OUTNAMEANDTYPE(NAME, ARR.TYPEINDEX);
{
write_s(&CODEGEN.OUTPUT, str_make(5, "[1 + "));
write_s(&CODEGEN.OUTPUT, ARR.HIGHBOUND);
write_s(&CODEGEN.OUTPUT, str_make(3, " - "));
write_s(&CODEGEN.OUTPUT, ARR.LOWBOUND);
write_c(&CODEGEN.OUTPUT, ']');
}
}
 else COMPILEERROR(cat_ss(cat_ss(cat_ss(str_make(29, "Error writing name and type: "), NAME), str_make(2, ", ")), TYPENAME(TYPEINDEX)));
}
void OUTTYPEDEFINITION(struct record9 *TYPEINDEX) {
PString NAME;
NAME = (*(TYPEINDEX)).NAME;
if ((*(TYPEINDEX)).ALIASFOR == ((void*)0)) COMPILEERROR(cat_ss(cat_ss(str_make(5, "Type "), NAME), str_make(16, " is not an alias")));
{
write_s(&CODEGEN.OUTPUT, str_make(8, "typedef "));
}
OUTNAMEANDTYPE(NAME, (*(TYPEINDEX)).ALIASFOR);
{
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTTYPEDEFINITIONSFROMCHECKPOINT(struct record19 *CHECKPOINT) {
struct record19 *DEF;
DEF = (*(CHECKPOINT)).NEXT;
while (DEF != ((void*)0)) {
if ((*(DEF)).CLS == TDCTYPE) {
_RESOLVEPOINTERUNKNOWN((*(DEF)).TYPEINDEX);
if ((*((*(DEF)).TYPEINDEX)).ALIASFOR != ((void*)0)) OUTTYPEDEFINITION((*(DEF)).TYPEINDEX);
}
DEF = (*(DEF)).NEXT;
}
}
void OUTCONSTANTVALUE(TPSEXPRESSION EXPR) {
{
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
}
}
void OUTCONSTANTARRAYBEGIN() {
{
write_s(&CODEGEN.OUTPUT, str_make(2, "{ "));
}
}
void OUTCONSTANTARRAYSEPARATOR() {
{
write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
}
}
void OUTCONSTANTARRAYEND() {
{
write_s(&CODEGEN.OUTPUT, str_make(2, " }"));
}
}
void OUTVARIABLEDECLARATION(TPSVARIABLE VARDEF) {
OUTNAMEANDTYPE(OUTVARIABLENAME(VARDEF.NAME, VARDEF.ISREFERENCE), VARDEF.TYPEINDEX);
}
void OUTVARIABLEDEFINITION(struct record15 *VARINDEX) {
if ((*(VARINDEX)).ISCONSTANT) {
write_s(&CODEGEN.OUTPUT, str_make(6, "const "));
}
OUTVARIABLEDECLARATION(*(VARINDEX));
{
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTCONSTANTDEFINITIONBEGIN(struct record15 *VARINDEX) {
{
write_s(&CODEGEN.OUTPUT, str_make(6, "const "));
}
OUTVARIABLEDECLARATION(*(VARINDEX));
{
write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
}
}
void OUTCONSTANTDEFINITIONEND() {
{
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTFUNCTIONPROTOTYPE(TPSFUNCTION DEF) {
int POS;
OUTNAMEANDTYPE(DEF.NAME, DEF.RETURNTYPEINDEX);
{
write_c(&CODEGEN.OUTPUT, '(');
}
{
int first = 1;
int last = DEF.ARGCOUNT;
if (first <= last) {
POS = first;
while (1) {
{
OUTVARIABLEDECLARATION(DEF.ARGS[(POS) - 1]);
if (POS != DEF.ARGCOUNT) {
write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
}
}
if (POS == last) break;
++POS;
}
}
}
{
write_c(&CODEGEN.OUTPUT, ')');
}
}
void OUTFUNCTIONDECLARATION(struct record16 *FNINDEX) {
OUTFUNCTIONPROTOTYPE(*(FNINDEX));
{
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTFUNCTIONDEFINITION(struct record16 *FNINDEX) {
OUTFUNCTIONPROTOTYPE(*(FNINDEX));
{
write_s(&CODEGEN.OUTPUT, str_make(2, " {"));
writeln(&CODEGEN.OUTPUT);
}
if ((*(FNINDEX)).RETURNTYPEINDEX != ((void*)0)) {
OUTNAMEANDTYPE(OUTRETURNVARIABLENAME((*(FNINDEX)).NAME), (*(FNINDEX)).RETURNTYPEINDEX);
{
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
}
void OUTFUNCTIONEND(struct record16 *FNINDEX) {
if ((*(FNINDEX)).RETURNTYPEINDEX != ((void*)0)) {
write_s(&CODEGEN.OUTPUT, str_make(7, "return "));
write_s(&CODEGEN.OUTPUT, OUTRETURNVARIABLENAME((*(FNINDEX)).NAME));
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTPROGRAMHEADING(PString NAME) {
{
write_s(&CODEGEN.OUTPUT, str_make(12, "/* Program: "));
write_s(&CODEGEN.OUTPUT, NAME);
write_s(&CODEGEN.OUTPUT, str_make(3, " */"));
writeln(&CODEGEN.OUTPUT);
}
{
write_s(&CODEGEN.OUTPUT, str_make(20, "#include \"pascual.h\""));
writeln(&CODEGEN.OUTPUT);
}
}
PString GENFUNCTIONCALLSTART(PString FN) {
PString return_GENFUNCTIONCALLSTART;
return_GENFUNCTIONCALLSTART = cat_sc(FN, '(');
return return_GENFUNCTIONCALLSTART;
}
PString GENFUNCTIONCALLEND(PString FN) {
PString return_GENFUNCTIONCALLEND;
return_GENFUNCTIONCALLEND = cat_sc(FN, ')');
return return_GENFUNCTIONCALLEND;
}
PString GENFUNCTIONCALLARGUMENT(PString FN, TPSEXPRESSION EXPR, PBoolean ISREFERENCE, int ARGNUM) {
PString return_GENFUNCTIONCALLARGUMENT;
if (ARGNUM != 1) FN = cat_ss(FN, str_make(2, ", "));
if (ISREFERENCE) FN = cat_ss(cat_sc(FN, '&'), EXPR.VALUE);
 else FN = cat_ss(FN, EXPR.VALUE);
return_GENFUNCTIONCALLARGUMENT = FN;
return return_GENFUNCTIONCALLARGUMENT;
}
char SHORTTYPENAME(struct record9 *TYPEINDEX) {
char return_SHORTTYPENAME;
if (ISBOOLEANTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'b';
 else if (ISINTEGERTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'i';
 else if (ISCHARTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'c';
 else if (ISSTRINGTYPE(TYPEINDEX)) return_SHORTTYPENAME = 's';
 else if (ISENUMTYPE(TYPEINDEX)) return_SHORTTYPENAME = 'e';
 else COMPILEERROR(cat_ss(str_make(30, "No short type name exists for "), TYPENAME(TYPEINDEX)));
return return_SHORTTYPENAME;
}
void OUTREAD(TPSEXPRESSION SRC, TPSEXPRESSION OUTVAR) {
{
write_s(&CODEGEN.OUTPUT, str_make(5, "read_"));
write_c(&CODEGEN.OUTPUT, SHORTTYPENAME(OUTVAR.TYPEINDEX));
write_s(&CODEGEN.OUTPUT, str_make(2, "(&"));
write_s(&CODEGEN.OUTPUT, SRC.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(3, ", &"));
write_s(&CODEGEN.OUTPUT, OUTVAR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTREADLN(TPSEXPRESSION SRC) {
{
write_s(&CODEGEN.OUTPUT, str_make(8, "readln(&"));
write_s(&CODEGEN.OUTPUT, SRC.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTWRITE(TPSEXPRESSION DST, TPSEXPRESSION EXPR) {
EXPR = EXPREVALUATE(EXPR);
if ((*(EXPR.TYPEINDEX)).CLS == TTCENUM) {
write_s(&CODEGEN.OUTPUT, str_make(9, "write_e(&"));
write_s(&CODEGEN.OUTPUT, DST.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
write_i(&CODEGEN.OUTPUT, (*((*(EXPR.TYPEINDEX)).ENUMINDEX)).ID);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
 else {
write_s(&CODEGEN.OUTPUT, str_make(6, "write_"));
write_c(&CODEGEN.OUTPUT, SHORTTYPENAME(EXPR.TYPEINDEX));
write_s(&CODEGEN.OUTPUT, str_make(2, "(&"));
write_s(&CODEGEN.OUTPUT, DST.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ", "));
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTWRITELN(TPSEXPRESSION DST) {
{
write_s(&CODEGEN.OUTPUT, str_make(9, "writeln(&"));
write_s(&CODEGEN.OUTPUT, DST.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTSTR(PString DST, TPSEXPRESSION EXPR) {
EXPR = EXPREVALUATE(EXPR);
if ((*(EXPR.TYPEINDEX)).CLS == TTCENUM) {
write_s(&CODEGEN.OUTPUT, DST);
write_s(&CODEGEN.OUTPUT, str_make(12, " = to_str_e("));
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(12, ", enumvalues"));
write_i(&CODEGEN.OUTPUT, (*((*(EXPR.TYPEINDEX)).ENUMINDEX)).ID);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
 else {
write_s(&CODEGEN.OUTPUT, DST);
write_s(&CODEGEN.OUTPUT, str_make(10, " = to_str_"));
write_c(&CODEGEN.OUTPUT, SHORTTYPENAME(EXPR.TYPEINDEX));
write_c(&CODEGEN.OUTPUT, '(');
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTNEW(TPSEXPRESSION DST) {
{
write_s(&CODEGEN.OUTPUT, DST.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(17, " = malloc(sizeof("));
}
OUTTYPEREFERENCE((*(DST.TYPEINDEX)).POINTEDTYPEINDEX);
{
write_s(&CODEGEN.OUTPUT, str_make(3, "));"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTDISPOSE(TPSEXPRESSION DST) {
{
write_s(&CODEGEN.OUTPUT, str_make(5, "free("));
write_s(&CODEGEN.OUTPUT, DST.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTEXPRESSION(TPSEXPRESSION EXPR) {
{
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
}
}
void OUTASSIGN(TPSEXPRESSION LHS, TPSEXPRESSION RHS) {
{
write_s(&CODEGEN.OUTPUT, LHS.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
write_s(&CODEGEN.OUTPUT, RHS.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTASSIGNRETURNVALUE(TPSEXPRESSION LHS, TPSEXPRESSION RHS) {
{
write_s(&CODEGEN.OUTPUT, str_make(7, "return_"));
write_s(&CODEGEN.OUTPUT, (*(LHS.FUNCTIONINDEX)).NAME);
write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
write_s(&CODEGEN.OUTPUT, RHS.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTASSIGNTOREFERENCE(struct record15 *VARINDEX, TPSEXPRESSION RHS) {
OUTVARIABLEDECLARATION(*(VARINDEX));
{
write_s(&CODEGEN.OUTPUT, str_make(5, " = &("));
write_s(&CODEGEN.OUTPUT, RHS.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ");"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTIF(TPSEXPRESSION EXPR) {
{
write_s(&CODEGEN.OUTPUT, str_make(4, "if ("));
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
}
}
void OUTELSE() {
{
write_s(&CODEGEN.OUTPUT, str_make(6, " else "));
}
}
void OUTCASEBEGIN(TPSEXPRESSION CASEINDEX) {
{
write_s(&CODEGEN.OUTPUT, str_make(8, "switch ("));
write_s(&CODEGEN.OUTPUT, CASEINDEX.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(3, ") {"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTCASESTATEMENTBEGIN(TPSEXPRESSION CASELABEL) {
{
write_s(&CODEGEN.OUTPUT, str_make(5, "case "));
write_s(&CODEGEN.OUTPUT, CASELABEL.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ": "));
}
}
void OUTCASESTATEMENTEND() {
{
write_s(&CODEGEN.OUTPUT, str_make(6, "break;"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTCASEELSEBEGIN() {
{
write_s(&CODEGEN.OUTPUT, str_make(9, "default: "));
}
}
void OUTCASEELSEEND() {
{
write_s(&CODEGEN.OUTPUT, str_make(6, "break;"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTCASEEND() {
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTREPEATBEGIN() {
{
write_s(&CODEGEN.OUTPUT, str_make(4, "do {"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTREPEATEND(TPSEXPRESSION EXPR) {
{
write_s(&CODEGEN.OUTPUT, str_make(11, "} while (!("));
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(3, "));"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTWHILEBEGIN(TPSEXPRESSION EXPR) {
{
write_s(&CODEGEN.OUTPUT, str_make(7, "while ("));
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(2, ") "));
}
}
void OUTWHILEEND() {
}
void OUTFORBEGIN(TPSEXPRESSION ITER, TPSEXPRESSION FIRSTEXPR, TPSEXPRESSION LASTEXPR, PBoolean ASCENDING) {
struct record9 *LIMITTYPE;
TPSVARIABLE FIRST;
TPSVARIABLE LAST;
LIMITTYPE = ITER.TYPEINDEX;
if (ISENUMTYPE(LIMITTYPE)) LIMITTYPE = PRIMITIVETYPES.PTINTEGER;
FIRST = MAKEVARIABLE(str_make(5, "first"), LIMITTYPE, 0);
LAST = MAKEVARIABLE(str_make(4, "last"), LIMITTYPE, 0);
{
write_c(&CODEGEN.OUTPUT, '{');
writeln(&CODEGEN.OUTPUT);
}
OUTVARIABLEDECLARATION(FIRST);
{
write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
write_s(&CODEGEN.OUTPUT, FIRSTEXPR.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
OUTVARIABLEDECLARATION(LAST);
{
write_s(&CODEGEN.OUTPUT, str_make(3, " = "));
write_s(&CODEGEN.OUTPUT, LASTEXPR.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
{
write_s(&CODEGEN.OUTPUT, str_make(10, "if (first "));
}
if (ASCENDING) {
write_s(&CODEGEN.OUTPUT, str_make(2, "<="));
}
 else {
write_s(&CODEGEN.OUTPUT, str_make(2, ">="));
}
{
write_s(&CODEGEN.OUTPUT, str_make(8, " last) {"));
writeln(&CODEGEN.OUTPUT);
}
{
write_s(&CODEGEN.OUTPUT, ITER.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(9, " = first;"));
writeln(&CODEGEN.OUTPUT);
}
{
write_s(&CODEGEN.OUTPUT, str_make(11, "while (1) {"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTFOREND(TPSEXPRESSION ITER, PBoolean ASCENDING) {
{
write_s(&CODEGEN.OUTPUT, str_make(4, "if ("));
write_s(&CODEGEN.OUTPUT, ITER.VALUE);
write_s(&CODEGEN.OUTPUT, str_make(16, " == last) break;"));
writeln(&CODEGEN.OUTPUT);
}
if (ASCENDING) {
write_s(&CODEGEN.OUTPUT, str_make(2, "++"));
write_s(&CODEGEN.OUTPUT, ITER.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
 else {
write_s(&CODEGEN.OUTPUT, str_make(2, "--"));
write_s(&CODEGEN.OUTPUT, ITER.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
}
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTPROCEDURECALL(TPSEXPRESSION EXPR) {
{
write_s(&CODEGEN.OUTPUT, EXPR.VALUE);
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTEMPTYSTATEMENT() {
{
write_c(&CODEGEN.OUTPUT, ';');
writeln(&CODEGEN.OUTPUT);
}
}
void OUTPROGRAMBEGIN() {
{
write_s(&CODEGEN.OUTPUT, str_make(21, "void pascual_main() {"));
writeln(&CODEGEN.OUTPUT);
}
}
void OUTPROGRAMEND() {
{
write_c(&CODEGEN.OUTPUT, '}');
writeln(&CODEGEN.OUTPUT);
}
}
void CODEGENRESET() {
CODEGEN.OUTPUT = OUTPUT;
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
{
write_s(&OUTPUT, str_make(6, "Usage:"));
writeln(&OUTPUT);
}
{
write_s(&OUTPUT, PARAMSTR(0));
write_s(&OUTPUT, str_make(24, " input.pas [-o output.c]"));
writeln(&OUTPUT);
}
writeln(&OUTPUT);
{
write_s(&OUTPUT, str_make(48, "If you specify \"-\" as the input or output file, "));
write_s(&OUTPUT, str_make(26, "stdin/stdout will be used."));
writeln(&OUTPUT);
}
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
{
int first = 1;
int last = LENGTH(OLD);
if (first <= last) {
POS = first;
while (1) {
MATCHES = MATCHES && (cmp_cc(UPCASE(STR.chr[POS + BASELEN]), UPCASE(OLD.chr[POS])) == 0);
if (POS == last) break;
++POS;
}
}
}
if (MATCHES) return_REPLACEEXTENSION = cat_ss(COPY(STR, 1, BASELEN), NEW);
}
return return_REPLACEEXTENSION;
}
void PARSECMDLINE() {
int POS;
PString INPUTFILE;
PString OUTPUTFILE;
enum enum7 { FLAGNONE, FLAGOUTPUT} FLAG;
PString PARAM;
const char* EnumValues7[] = { "FLAGNONE", "FLAGOUTPUT" };
INPUTFILE = str_make(0, "");
OUTPUTFILE = str_make(0, "");
FLAG = FLAGNONE;
{
int first = 1;
int last = PARAMCOUNT();
if (first <= last) {
POS = first;
while (1) {
{
PARAM = PARAMSTR(POS);
if ((cmp_cc(PARAM.chr[1], '-') == 0) && (cmp_sc(PARAM, '-') != 0)) {
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
}
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
