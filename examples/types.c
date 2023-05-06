/* Program: TYPES */
#include "pascual.h"

void BASICTYPES() {
  PBoolean B;
  PInteger I;
  PReal R;
  PChar C;
  PString S;
  B = 1;
  B = 0;
  I = 12345678;
  I = -12345678;
  I = 51966;
  R =  6.02000000000000E+023;
  R =  1.00000000000000E-010;
  R = -4.20000000000000E+001;
  C = 'a';
  C = 'Z';
  S = str_make(12, "Hello, world");
  S = str_make(0, "");
  S = str_make(63, "En un lugar de La Mancha, de cuyo nombre no quiero acordarme...");
}

void ENUMERATEDTYPES() {
  typedef enum __attribute__((__packed__)) enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef enum __attribute__((__packed__)) enum2 { NORTH, EAST, SOUTH, WEST } DIRECTIONS;
  SUITS SUIT;
  DIRECTIONS DIRECTION;
  enum __attribute__((__packed__)) enum3 { PARK, REVERSE, NEUTRAL, LOW, DIRECT } GEAR;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  const char* enumvalues2[] = { "NORTH", "EAST", "SOUTH", "WEST" };
  const char* enumvalues3[] = { "PARK", "REVERSE", "NEUTRAL", "LOW", "DIRECT" };
  SUIT = DIAMONDS;
  DIRECTION = WEST;
  GEAR = NEUTRAL;
  Write(&OUTPUT, 1, RwpEnum, SUIT, enumvalues1, RwpChar, ' ', RwpEnum, DIRECTION, enumvalues2, RwpChar, ' ', RwpEnum | RwpLn | RwpEnd, GEAR, enumvalues3);
  Write(&OUTPUT, 1, RwpLenPtr, 18, "Suit > Hearts  -> ", RwpBool | RwpLn | RwpEnd, SUIT > HEARTS);
  Write(&OUTPUT, 1, RwpLenPtr, 21, "Direction = West  -> ", RwpBool | RwpLn | RwpEnd, DIRECTION == WEST);
}

void SUBRANGES() {
  typedef PBits8 FIRSTHUNDRED;
  typedef enum __attribute__((__packed__)) enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef PBits8 REDS;
  typedef PBits8 BLACKS;
  FIRSTHUNDRED NUM;
  REDS REDSUIT;
  BLACKS BLACKSUIT;
  PBits8 LETTER;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  NUM = 27;
  REDSUIT = HEARTS;
  BLACKSUIT = SPADES;
  LETTER = 'X';
}

void ARRAYS() {
  typedef enum __attribute__((__packed__)) enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef enum __attribute__((__packed__)) enum2 { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN } PIECE;
  PString NAMES[12];
  PInteger TEMPERATURECOUNTS[201];
  PInteger CHARACTERCOUNTS[256];
  PInteger LETTERCOUNTS[26];
  PInteger SUITCOUNTS[4];
  PInteger BOOLEANCOUNTS[2];
  PIECE CHESSBOARD[8][8];
  PIECE OTHERCHESSBOARD[8][8];
  PReal MASHUP[13][26][2];
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  const char* enumvalues2[] = { "KING", "QUEEN", "BISHOP", "KNIGHT", "ROOK", "PAWN" };
  NAMES[0] = str_make(4, "Mark");
  SUITCOUNTS[1] = 3;
  CHESSBOARD[2][1] = PAWN;
  CHESSBOARD[2][1] = PAWN;
  MASHUP[1][9][1] =  3.14160000000000E+000;
  MASHUP[1][9][1] =  3.14160000000000E+000;
}

void RECORDS() {
  typedef enum __attribute__((__packed__)) enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef struct record1 {
    PInteger A;
    PReal B;
    PReal C;
    PString D;
  } R;
  R ONE;
  R TWO;
  struct record2 {
    PInteger A;
    PReal B;
    SUITS C;
    union {
      struct {
        PString D;
      };
      struct {
        PString E;
        PInteger F;
      };
      struct {
        PBoolean G;
      };
    };
  } THREE;
  struct record3 {
    PInteger A;
    PReal B;
    union {
      struct {
        PString D;
      };
      struct {
        PString E;
        PInteger F;
      };
      struct {
        PBoolean G;
      };
    };
  } FOUR;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  ONE.A = 123;
  ONE.B =  1.23400000000000E+001;
  TWO = ONE;
  THREE.D = str_make(16, "These are hearts");
}

void pascual_main() {
  BASICTYPES();
  ENUMERATEDTYPES();
  SUBRANGES();
  ARRAYS();
  RECORDS();
}
