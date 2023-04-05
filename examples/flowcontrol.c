/* Program: FLOWCONTROL */
#include "pascual.h"

void SHOWIF() {
  if (1) Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  else Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  if (1) Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  if (1) ;
  else Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  if (1) ;
  if (1) {
    Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
    Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 8, "true dat");
  }
  else {
    Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
    Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 9, "no really");
  }
  if (1) Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  else if (1) Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "true2");
  else Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  if (1) {
    if (1) Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
    else Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
  }
  if (1) if (1) Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 4, "true");
  else Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "false");
}

void SHOWREPEATUNTIL() {
  PInteger I;
  I = 0;
  do {
    I = I + 1;
  } while (I != 5);
  I = 0;
  do {
    I = I + 1;
    Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
  } while (I != 5);
  I = 10;
  do {
    I = I + 1;
  } while (I != 5);
  do {
  } while (1);
}

void SHOWWHILE() {
  PInteger I;
  I = 0;
  while (I != 5) I = I + 1;
  I = 0;
  while (I != 5) {
    I = I + 1;
    Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
  }
  while (0) ;
}

void SHOWFOR() {
  PInteger I;
  for (PInteger first = 1, last = 10; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (I = first; !done; done = I == last ? 1 : (++I, 0)) Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
    break;
  }
  for (PInteger first = 10, last = 1; first >= last; /*breaks*/) {
    PBoolean done = 0;
    for (I = first; !done; done = I == last ? 1 : (--I, 0)) Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
    break;
  }
  for (PInteger first = 1, last = 10; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (I = first; !done; done = I == last ? 1 : (++I, 0)) {
      Write(&OUTPUT, 1, RwpLenPtr | RwpEnd, 14, "The number is ");
      Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
    }
    break;
  }
  I = 10;
  for (PInteger first = 1, last = I; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (I = first; !done; done = I == last ? 1 : (++I, 0)) Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
    break;
  }
  I = 0;
  for (PInteger first = 20, last = 10; first <= last; /*breaks*/) {
    PBoolean done = 0;
    for (I = first; !done; done = I == last ? 1 : (++I, 0)) Write(&OUTPUT, 1, RwpInt | RwpLn | RwpEnd, I);
    break;
  }
  Write(&OUTPUT, 1, RwpLenPtr, 29, "The value of i is 0, not 20: ", RwpInt | RwpLn | RwpEnd, I);
}

void SHOWCASE() {
  PChar I;
  I = 'b';
  switch (I) {
    case 'a':
      Write(&OUTPUT, 1, RwpChar | RwpLn | RwpEnd, 'a');
      break;
    case 'b':
      Write(&OUTPUT, 1, RwpChar | RwpLn | RwpEnd, 'b');
      break;
    case 'c':
      Write(&OUTPUT, 1, RwpChar | RwpLn | RwpEnd, 'c');
      break;
    default:
      break;
  }
  switch (I) {
    case 'd':
      Write(&OUTPUT, 1, RwpChar | RwpLn | RwpEnd, 'd');
      break;
    case 'e':
      Write(&OUTPUT, 1, RwpChar | RwpLn | RwpEnd, 'e');
      break;
    default:
      Write(&OUTPUT, 1, RwpLenPtr | RwpLn | RwpEnd, 5, "other");
      break;
  }
}

void pascual_main() {
  SHOWIF();
  SHOWREPEATUNTIL();
  SHOWWHILE();
  SHOWFOR();
  SHOWCASE();
}
