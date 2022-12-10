#include "sjson.h"

#include <stdarg.h>

static sjson_retval_t sjson_valid_context(sjson_context_t *ctx) {
  return (ctx->pBuf == NULL || ctx->buf_size == 0 || ctx->state == JSON_INIT ||
          ctx->index > ctx->buf_size - 1)
             ? JSON_ERROR
             : JSON_SUCCESS;
}

static sjson_retval_t sjson_add_key(sjson_context_t *ctx, uint8_t *key,
                                    size_t key_len) {
  size_t space_available;
  int space_used;

  space_available = (ctx->buf_size - 1) - ctx->index;
  space_used = SJSON_SNPRINTF((char *)&ctx->pBuf[ctx->index], space_available,
                              "\"%*s\":", (int)key_len, key);
  ctx->index += space_used;

  return (space_used >= space_available) ? JSON_ERROR : JSON_SUCCESS;
}

sjson_retval_t (*json_add_value[SJSON_MAX])(sjson_context_t *ctx, void *value,
                                            size_t value_len,
                                            sjson_type_t type);

sjson_retval_t int8_handler(sjson_context_t *ctx, void *value, size_t value_len,
                            sjson_type_t type) {}

sjson_retval_t int16_handler(sjson_context_t *ctx, void *value,
                             size_t value_len, sjson_type_t type) {}

sjson_retval_t int32_handler(sjson_context_t *ctx, void *value,
                             size_t value_len, sjson_type_t type) {}

sjson_retval_t int64_handler(sjson_context_t *ctx, void *value,
                             size_t value_len, sjson_type_t type) {}

static inline uint8_t init_json_add_value(void) {
  json_add_value[SJSON_8BIT_INT] = int8_handler;
  json_add_value[SJSON_16BIT_INT] = int16_handler;
  json_add_value[SJSON_32BIT_INT] = int32_handler;
  json_add_value[SJSON_64BIT_INT] = int64_handler;
}

sjson_retval_t sjson_add_pair(sjson_context_t *ctx, uint8_t *key,
                              size_t key_len, void *value, size_t value_len,
                              sjson_type_t type) {
  /* Check for a valid context and value type */
  if (type > SJSON_MAX - 1) return JSON_ERROR;
  if (JSON_SUCCESS != sjson_valid_context(ctx)) return JSON_ERROR;
  /* Return if there is an error in adding the key */
  if (JSON_SUCCESS != sjson_add_key(ctx, key, key_len)) return JSON_ERROR;
}

sjson_retval_t sjson_init(sjson_context_t *ctx, uint8_t *buf, size_t buf_size) {
  if (buf == NULL) return JSON_INVALID_BUFFER;
  if (buf_size == 0) return JSON_INVALID_BUFFER;

  init_json_add_value();

  memset(buf, 0, buf_size);
  memset(ctx, 0, sizeof(sjson_context_t));
  ctx->pBuf = buf;
  ctx->buf_size = buf_size;
  ctx->state = JSON_START;
  ctx->pBuf[ctx->index++] = '{';

  return JSON_SUCCESS;
}
