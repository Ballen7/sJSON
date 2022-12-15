/*
* MIT License

* Copyright (c) 2022 Ballen7

* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "sjson.h"

#define SJSON_WRITE_ADDRESS(ctx) ((char *)&ctx->pBuf[ctx->index])
#define SJSON_AVAIABLE_SPACE(ctx) ((size_t)(ctx->buf_size - ctx->index))

static const uint8_t *const SJSON_TRUE_STRING = (const uint8_t *)"true";
static const uint8_t *const SJSON_FALSE_STRING = (const uint8_t *)"false";

static sjson_retval_t sjson_initate_keypair(sjson_context_t *ctx) {
  if (ctx->pBuf == NULL) return SJSON_ERROR;
  if (ctx->buf_size == 0) return SJSON_ERROR;
  if (SJSON_AVAIABLE_SPACE(ctx) == 0) return SJSON_ERROR;
  if (ctx->state != SJSON_NEXT_KEY_PAIR && ctx->state != SJSON_START)
    return SJSON_ERROR;

  if (ctx->state == SJSON_NEXT_KEY_PAIR) ctx->pBuf[ctx->index++] = ',';

  return SJSON_SUCCESS;
}

static sjson_retval_t sjson_add_key(sjson_context_t *ctx, uint8_t *key,
                                    size_t key_len) {
  if (SJSON_AVAIABLE_SPACE(ctx) < 1 + key_len + 1 + 1) return SJSON_ERROR;

  ctx->pBuf[ctx->index++] = '"';
  memcpy(&ctx->pBuf[ctx->index], (void *)key, key_len);
  ctx->index += key_len;
  ctx->pBuf[ctx->index++] = '"';
  ctx->pBuf[ctx->index++] = ':';

  return SJSON_SUCCESS;
}

static sjson_retval_t int8_handler(sjson_context_t *ctx, void *value) {
  uint8_t _val = (uint8_t) * (uint8_t *)value;
  ctx->index += (int32_t)SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx),
                                        SJSON_AVAIABLE_SPACE(ctx), "%u", _val);
  return (ctx->index > ctx->buf_size) ? SJSON_ERROR : SJSON_SUCCESS;
}

static sjson_retval_t int16_handler(sjson_context_t *ctx, void *value) {
  uint16_t _val = (uint16_t) * (uint16_t *)value;
  ctx->index += (int32_t)SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx),
                                        SJSON_AVAIABLE_SPACE(ctx), "%u", _val);
  return (ctx->index > ctx->buf_size) ? SJSON_ERROR : SJSON_SUCCESS;
}

static sjson_retval_t int32_handler(sjson_context_t *ctx, void *value) {
  uint32_t _val = (uint32_t) * (uint32_t *)value;
  ctx->index += (int32_t)SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx),
                                        SJSON_AVAIABLE_SPACE(ctx), "%u", _val);
  return (ctx->index > ctx->buf_size) ? SJSON_ERROR : SJSON_SUCCESS;
}

static sjson_retval_t int64_handler(sjson_context_t *ctx, void *value) {
  uint64_t _val = (uint64_t) * (uint64_t *)value;
  ctx->index += (int32_t)SJSON_SNPRINTF(SJSON_WRITE_ADDRESS(ctx),
                                        SJSON_AVAIABLE_SPACE(ctx), "%lu", _val);
  return (ctx->index > ctx->buf_size) ? SJSON_ERROR : SJSON_SUCCESS;
}

static sjson_retval_t (*sjson_add_value[SJSON_INT_MAX_INDEX])(
    sjson_context_t *ctx, void *value) = {int8_handler, int16_handler,
                                          int32_handler, int64_handler};

sjson_retval_t sjson_add_integer(sjson_context_t *ctx, uint8_t *key,
                                 size_t key_len, void *value,
                                 sjson_integer_size_t type) {
  if (type > SJSON_INT_MAX_INDEX - 1) return SJSON_ERROR;
  if (SJSON_SUCCESS != sjson_initate_keypair(ctx)) return SJSON_ERROR;

  ctx->state = SJSON_IN_PROGRESS;
  if (SJSON_SUCCESS != sjson_add_key(ctx, key, key_len)) return SJSON_ERROR;
  if (SJSON_SUCCESS != sjson_add_value[type](ctx, value)) return SJSON_ERROR;
  ctx->state = SJSON_NEXT_KEY_PAIR;

  return SJSON_SUCCESS;
}

sjson_retval_t sjson_add_string(sjson_context_t *ctx, uint8_t *key,
                                size_t key_len, void *value, size_t value_len) {
  if (SJSON_SUCCESS != sjson_initate_keypair(ctx)) return SJSON_ERROR;

  ctx->state = SJSON_IN_PROGRESS;

  if (SJSON_SUCCESS != sjson_add_key(ctx, key, key_len)) return SJSON_ERROR;
  if (SJSON_AVAIABLE_SPACE(ctx) < 1 + value_len + 1) return SJSON_ERROR;

  ctx->pBuf[ctx->index++] = '"';
  memcpy(&ctx->pBuf[ctx->index], value, value_len);
  ctx->index += value_len;
  ctx->pBuf[ctx->index++] = '"';

  ctx->state = SJSON_NEXT_KEY_PAIR;

  return SJSON_SUCCESS;
}

sjson_retval_t sjson_add_boolean(sjson_context_t *ctx, uint8_t *key,
                                 size_t key_len, sjson_boolean_t bool_val) {
  const uint8_t *pStr;

  if (bool_val > SJSON_BOOLEAN_MAX - 1) return SJSON_ERROR;
  if (SJSON_SUCCESS != sjson_initate_keypair(ctx)) return SJSON_ERROR;

  ctx->state = SJSON_IN_PROGRESS;
  if (SJSON_SUCCESS != sjson_add_key(ctx, key, key_len)) return SJSON_ERROR;
  if (bool_val == SJSON_TRUE)
    pStr = SJSON_TRUE_STRING;
  else
    pStr = SJSON_FALSE_STRING;
  if (SJSON_AVAIABLE_SPACE(ctx) < strlen((const char *)pStr))
    return SJSON_ERROR;

  memcpy(&ctx->pBuf[ctx->index], pStr, strlen((const char *)pStr));
  ctx->index += strlen((const char *)pStr);
  ctx->state = SJSON_NEXT_KEY_PAIR;

  return SJSON_SUCCESS;
}

sjson_retval_t sjson_add_object(sjson_context_t *ctx, uint8_t *key,
                                size_t key_len, void *value, size_t value_len) {
  if (SJSON_SUCCESS != sjson_initate_keypair(ctx)) return SJSON_ERROR;

  ctx->state = SJSON_IN_PROGRESS;

  if (SJSON_SUCCESS != sjson_add_key(ctx, key, key_len)) return SJSON_ERROR;
  if (SJSON_AVAIABLE_SPACE(ctx) < value_len) return SJSON_ERROR;

  memcpy(&ctx->pBuf[ctx->index], value, value_len);
  ctx->index += value_len;

  ctx->state = SJSON_NEXT_KEY_PAIR;

  return SJSON_SUCCESS;
}

sjson_retval_t sjson_init(sjson_context_t *ctx, uint8_t *buf, size_t buf_size) {
  if (buf == NULL) return SJSON_INVALID_BUFFER;
  if (buf_size == 0) return SJSON_INVALID_BUFFER;

  memset(ctx, 0, sizeof(sjson_context_t));
  ctx->pBuf = buf;
  ctx->buf_size = buf_size;

  ctx->state = SJSON_START;
  ctx->pBuf[ctx->index++] = '{';

  return SJSON_SUCCESS;
}

sjson_retval_t sjson_close(sjson_context_t *ctx) {
  if (ctx->state != SJSON_NEXT_KEY_PAIR) return SJSON_ERROR;
  if (SJSON_AVAIABLE_SPACE(ctx) < 1 + 1) return SJSON_ERROR;

  ctx->pBuf[ctx->index++] = '}';
  ctx->pBuf[ctx->index++] = 0;
  ctx->state = SJSON_COMPLETE;

  sjson_log(">>%s<<\n\n", ctx->pBuf);

  return SJSON_SUCCESS;
}
