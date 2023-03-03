#include "error.h"

#include <stdio.h>
#include <stdlib.h>

static const char* RtErrorMsgs[] = {"Unknown error", "Out of bounds", "Math error"};

void rt_error(RtError err) {
  if ((err < 0) || (err >= sizeof(RtErrorMsgs))) err = 0;
  fprintf(stderr, "Run-time error: %s\n", RtErrorMsgs[err]);
  abort();
}

static const char* IoErrorMsgs[] = {"Unknown error", "File not found",
                                    "Access denied", "File not open",
                                    "Read error",    "Write error"};

void io_error(IoError err, const char* msg) {
  if ((err < 0) || (err >= sizeof(IoErrorMsgs))) err = 0;
  fprintf(stderr, "I/O error: %s: %s\n", msg, IoErrorMsgs[err]);
  abort();
}