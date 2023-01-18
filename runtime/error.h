#ifndef __PASCUAL_ERROR_H
#define __PASCUAL_ERROR_H

typedef enum { reOutOfBounds = 0 } RtError;

void rt_error(RtError err);

#endif  // __PASCUAL_ERROR_H
