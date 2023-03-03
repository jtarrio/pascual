#ifndef __PASCUAL_ERROR_H
#define __PASCUAL_ERROR_H

typedef enum { reUnknown, reOutOfBounds, reMath } RtError;
typedef enum {
  ieUnknown,
  ieFileNotFound,
  ieAccessDenied,
  ieFileNotOpen,
  ieReadError,
  ieWriteError
} IoError;

void rt_error(RtError err);
void io_error(IoError err, const char* msg);

#endif  // __PASCUAL_ERROR_H
