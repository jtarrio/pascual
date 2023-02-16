/* Program: TYPES */
#include "pascual.h"

void BASICTYPES() {
  int B;
  int I;
  double R;
  unsigned char C;
  PString S;
  B = 1;
  B = 0;
  I = 12345678;
  I = -12345678;
  R = 6.02e+23;
  R = 1e-10;
  R = -42;
  C = 'a';
  C = 'Z';
  S = str_make(12, "Hello, world");
  S = str_make(0, "");
  S = str_make(63, "En un lugar de La Mancha, de cuyo nombre no quiero acordarme...");
}

void ENUMERATEDTYPES() {
  typedef enum enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef enum enum2 { NORTH, EAST, SOUTH, WEST } DIRECTIONS;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  const char* enumvalues2[] = { "NORTH", "EAST", "SOUTH", "WEST" };
  SUITS SUIT;
  DIRECTIONS DIRECTION;
  enum enum3 { PARK, REVERSE, NEUTRAL, LOW, DIRECT } GEAR;
  const char* enumvalues3[] = { "PARK", "REVERSE", "NEUTRAL", "LOW", "DIRECT" };
  SUIT = DIAMONDS;
  DIRECTION = WEST;
  GEAR = NEUTRAL;
  WRITE_e(&OUTPUT, SUIT, enumvalues1);
  WRITE_c(&OUTPUT, ' ');
  WRITE_e(&OUTPUT, DIRECTION, enumvalues2);
  WRITE_c(&OUTPUT, ' ');
  WRITE_e(&OUTPUT, GEAR, enumvalues3);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(18, "Suit > Hearts  -> "));
  WRITE_b(&OUTPUT, SUIT > HEARTS);
  WRITELN(&OUTPUT);
  WRITE_s(&OUTPUT, str_make(21, "Direction = West  -> "));
  WRITE_b(&OUTPUT, DIRECTION == WEST);
  WRITELN(&OUTPUT);
}

void SUBRANGES() {
  typedef int FIRSTHUNDRED;
  typedef enum enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef enum enum1 REDS;
  typedef enum enum1 BLACKS;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  FIRSTHUNDRED NUM;
  REDS REDSUIT;
  BLACKS BLACKSUIT;
  unsigned char LETTER;
  NUM = 27;
  REDSUIT = HEARTS;
  BLACKSUIT = SPADES;
  LETTER = 'X';
}

void ARRAYS() {
  typedef enum enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  typedef enum enum2 { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN } PIECE;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  const char* enumvalues2[] = { "KING", "QUEEN", "BISHOP", "KNIGHT", "ROOK", "PAWN" };
  PString NAMES[12];
  int TEMPERATURECOUNTS[201];
  int CHARACTERCOUNTS[256];
  int LETTERCOUNTS[26];
  int SUITCOUNTS[4];
  int BOOLEANCOUNTS[2];
  PIECE CHESSBOARD[8][8];
  PIECE OTHERCHESSBOARD[8][8];
  double MASHUP[13][26][2];
  NAMES[0] = str_make(4, "Mark");
  SUITCOUNTS[DIAMONDS] = 3;
  CHESSBOARD[2][1] = PAWN;
  CHESSBOARD[2][1] = PAWN;
  MASHUP[1][9][1] = 3.1416;
  MASHUP[1][9][1] = 3.1416;
}

void RECORDS() {
  typedef enum enum1 { HEARTS, DIAMONDS, CLUBS, SPADES } SUITS;
  const char* enumvalues1[] = { "HEARTS", "DIAMONDS", "CLUBS", "SPADES" };
  typedef struct record1 {
    int A;
    double B;
    double C;
    PString D;
  } R;
  R ONE;
  R TWO;
  struct record2 {
    int A;
    double B;
    SUITS C;
    union {
      struct {
        PString D;
      };
      struct {
        PString E;
        int F;
      };
      struct {
        int G;
      };
    };
  } THREE;
  struct record3 {
    int A;
    double B;
    union {
      struct {
        PString D;
      };
      struct {
        PString E;
        int F;
      };
      struct {
        int G;
      };
    };
  } FOUR;
  ONE.A = 123;
  ONE.B = 12.34;
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
