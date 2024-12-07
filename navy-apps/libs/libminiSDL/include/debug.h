#ifndef __DEBUG__H__
#define __DEBUG__H__

#include <stdio.h>
#include <stdlib.h>

#define Log(format, ...) \
  printf("\33[1;35m[%s,%d,%s] " format "\33[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#undef panic
#define panic(format, ...) \
  do { \
    Log("\33[1;31msystem panic: " format, ## __VA_ARGS__); \
    abort();\
  } while (0)

#endif
