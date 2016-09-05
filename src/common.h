
#ifndef UVT_COMMON_H
#define UVT_COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define UVT_IMIN(a,b) (((a)<(b))?(a):(b))
#define UVT_IMAX(a,b) (((a)>(b))?(a):(b))

typedef enum uvt_return_codes_e {
  UVT_OK = 0,
  UVT_ERROR = -1,
  UVT_NOT_ADDED = -2
} uvt_return_code;

typedef enum {
  UVT_FALSE = 0,
  UVT_TRUE = 1
} uvt_bool;

#endif