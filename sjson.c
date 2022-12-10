#include "sjson.h"

#include <stdarg.h>

#define SJSON_WRITE_ADDRESS(ctx) ((char *)&ctx->pBuf[ctx->index])
#define SJSON_AVAIABLE_SPACE(ctx) ((size_t)(ctx->buf_size - 1) - ctx->index)

static sjson_retval_t sjson_valid_context(sjson_context_t *ctx) {
  return (ctx->pBuf == NULL || ctx->buf_size == 0 ||
          ctx->index > ctx->buf_size - 1)
             ? JSON_ERROR
             : JSON_SUCCESS;
}

static sjson_retval_t sjson_add_key(sjson_context_t *ctx, uint8_t *key,
                                    size_t key_len) {
  size_t vacany;
  int space_used;
  vacany = SJSON_AVAIABLE_SPACE(ctx);
  space_used = SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx), vacany,
                              "\"%*s\":", (int)key_len, key);
  ctx->index += space_used;
  return (space_used >= vacany) ? JSON_ERROR : JSON_SUCCESS;
}

sjson_retval_t (*sjson_add_value[SJSON_MAX])(sjson_context_t *ctx, void *value);

sjson_retval_t int8_handler(sjson_context_t *ctx, void *value) {
  size_t vacany;
  int space_used;
  uint8_t _val = (uint8_t) * (uint8_t *)value;
  vacany = SJSON_AVAIABLE_SPACE(ctx);
  space_used = SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx), vacany, "%u", _val);
  ctx->index += space_used;
  return (space_used >= vacany) ? JSON_ERROR : JSON_SUCCESS;
}

sjson_retval_t int16_handler(sjson_context_t *ctx, void *value) {
  size_t vacany;
  int space_used;
  uint16_t _val = (uint16_t) * (uint16_t *)value;
  vacany = SJSON_AVAIABLE_SPACE(ctx);
  space_used = SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx), vacany, "%u", _val);
  ctx->index += space_used;
  return (space_used >= vacany) ? JSON_ERROR : JSON_SUCCESS;
}

sjson_retval_t int32_handler(sjson_context_t *ctx, void *value) {
  size_t vacany;
  int space_used;
  uint32_t _val = (uint32_t) * (uint32_t *)value;
  vacany = SJSON_AVAIABLE_SPACE(ctx);
  space_used = SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx), vacany, "%u", _val);
  ctx->index += space_used;
  return (space_used >= vacany) ? JSON_ERROR : JSON_SUCCESS;
}

sjson_retval_t int64_handler(sjson_context_t *ctx, void *value) {
  size_t vacany;
  int space_used;
  uint64_t _val = (uint64_t) * (uint64_t *)value;
  vacany = SJSON_AVAIABLE_SPACE(ctx);
  space_used = SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx), vacany, "%lu", _val);
  ctx->index += space_used;
  return (space_used >= vacany) ? JSON_ERROR : JSON_SUCCESS;
}

static inline uint8_t init_sjson_add_value(void) {
  sjson_add_value[SJSON_8BIT_INT] = int8_handler;
  sjson_add_value[SJSON_16BIT_INT] = int16_handler;
  sjson_add_value[SJSON_32BIT_INT] = int32_handler;
  sjson_add_value[SJSON_64BIT_INT] = int64_handler;
}

sjson_retval_t sjson_add_integer(sjson_context_t *ctx, uint8_t *key,
                                 size_t key_len, void *value,
                                 sjson_integer_size_t type) {
  /* Check for a valid context and value type */
  if (type > SJSON_MAX - 1) return JSON_ERROR;
  if (JSON_SUCCESS != sjson_valid_context(ctx)) return JSON_ERROR;

  if (ctx->state == JSON_ADD_CLOSING_BRACKET && SJSON_AVAIABLE_SPACE(ctx) > 0)
    ctx->pBuf[ctx->index++] = ',';

  ctx->state = JSON_IN_PROGRESS;
  if (JSON_SUCCESS != sjson_add_key(ctx, key, key_len)) return JSON_ERROR;
  if (JSON_SUCCESS != sjson_add_value[type](ctx, value)) return JSON_ERROR;
  ctx->state = JSON_ADD_CLOSING_BRACKET;

  return JSON_SUCCESS;
}

sjson_retval_t sjson_init(sjson_context_t *ctx, uint8_t *buf, size_t buf_size) {
  if (buf == NULL) return JSON_INVALID_BUFFER;
  if (buf_size == 0) return JSON_INVALID_BUFFER;

  init_sjson_add_value();

  memset(buf, 0, buf_size);
  memset(ctx, 0, sizeof(sjson_context_t));
  ctx->pBuf = buf;
  ctx->buf_size = buf_size;
  ctx->state = JSON_START;
  ctx->pBuf[ctx->index++] = '{';

  return JSON_SUCCESS;
}

sjson_retval_t sjson_complete(sjson_context_t *ctx) {
  if (ctx->state == JSON_ADD_CLOSING_BRACKET && SJSON_AVAIABLE_SPACE(ctx) > 0) {
    ctx->pBuf[ctx->index++] = '}';
    printf(">>%s<<\n\n", ctx->pBuf);
    return JSON_SUCCESS;
  }
  return JSON_ERROR;
}
