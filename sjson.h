#ifndef _H_SJON
#define _H_SJON

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SJSON_LOGGING 1

#if (SJSON_LOGGING == 1)
#define sjson_log(format, ...) printf(format, __VA_ARGS__)
#else
#define sjson_log(format, ...)
#endif

#define SJSON_SNPRINTF(s, n, format, ...) snprintf(s, n, format, __VA_ARGS__)

typedef enum {
  JSON_INIT = 0,
  JSON_START,
  JSON_IN_PROGRESS,
  JSON_ADD_CLOSING_BRACKET,
  JSON_COMPLETE,
} sjson_state_t;

typedef enum {
  JSON_SUCCESS = 0,
  JSON_ERROR,
  JSON_INVALID_BUFFER
} sjson_retval_t;

typedef enum {
  SJSON_8BIT_INT = 0,
  SJSON_16BIT_INT = 1,
  SJSON_32BIT_INT = 2,
  SJSON_64BIT_INT = 3,

  SJSON_INT_MAX_INDEX = 4
} sjson_integer_size_t;

typedef enum {
  SJSON_TRUE = 0,
  SJSON_FALSE,

  SJSON_BOOLEAN_MAX
} sjson_boolean_t;

typedef struct {
  uint8_t *pBuf;
  size_t buf_size;
  size_t index;
  size_t key_pair_count;
  sjson_state_t state;
} sjson_context_t;

sjson_retval_t sjson_init(sjson_context_t *ctx, uint8_t *buf, size_t buf_size);

sjson_retval_t sjson_add_string(sjson_context_t *ctx, uint8_t *key,
                                size_t key_len, void *value, size_t value_len);

sjson_retval_t sjson_add_integer(sjson_context_t *ctx, uint8_t *key,
                                 size_t key_len, void *value,
                                 sjson_integer_size_t type);

sjson_retval_t sjson_add_boolean(sjson_context_t *ctx, uint8_t *key,
                                 size_t key_len, sjson_boolean_t bool_val);

sjson_retval_t sjson_complete(sjson_context_t *ctx);

#endif /* _H_SJON */