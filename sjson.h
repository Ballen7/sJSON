#ifndef _H_SJON
#define _H_SJON

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SJSON_SNPRINTF(s, n, format, ...) snprintf(s, n, format, __VA_ARGS__)

typedef enum {
  JSON_INIT = 0,
  JSON_START,
  ADD_KEY,
  ADD_TOKEN,
  JSON_COMPLETE,
  JSON_VALID,
  JSON_INVALID
} sjson_state_t;

typedef enum {
  JSON_SUCCESS = 0,
  JSON_ERROR,
  JSON_INVALID_BUFFER
} sjson_retval_t;

typedef enum {
  SJSON_8BIT_INT = 0,
  SJSON_16BIT_INT,
  SJSON_32BIT_INT,
  SJSON_64BIT_INT,

  SJSON_STRING,

  SJSON_MAX
} sjson_type_t;

typedef struct {
  uint8_t *pBuf;
  size_t buf_size;
  size_t index;
  size_t key_pair_count;
  sjson_state_t state;
} sjson_context_t;

sjson_retval_t sjson_add_pair(sjson_context_t *ctx, uint8_t *key,
                              size_t key_len, void *value, size_t value_len,
                              sjson_type_t type);

sjson_retval_t sjson_init(sjson_context_t *ctx, uint8_t *buf, size_t buf_size);

#endif /* _H_SJON */