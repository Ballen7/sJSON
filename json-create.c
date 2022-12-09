#include "json-create.h"

#include <stdio.h>
#include <string.h>

json_create_retval_t valid_context(json_context_t *ctx) {
  return (ctx->pBuf == NULL || ctx->buf_size == 0 || ctx->state == JSON_INIT ||
          ctx->index > ctx->buf_size - 1)
             ? JSON_SUCCESS
             : JSON_ERROR;
}

json_create_retval_t json_create_init(json_context_t *ctx, uint8_t *buf,
                                      size_t buf_size) {
  if (buf == NULL) return JSON_INVALID_BUFFER;
  if (buf_size == 0) return JSON_INVALID_BUFFER;

  memset(ctx, 0, sizeof(json_context_t));
  ctx->pBuf = buf;
  ctx->buf_size = buf_size;
  ctx->state = JSON_START;

  return JSON_SUCCESS;
}

static inline json_create_retval_t json_add_key(void) { return 1; }

static inline json_create_retval_t json_add_value() { return 1; }

json_create_retval_t json_add_pair(json_context_t *ctx, uint8_t *key,
                                   size_t key_len, void *value,
                                   size_t value_len, json_value_type_t type) {
  if (JSON_SUCCESS != valid_context(ctx)) return JSON_ERROR;


}

int main(void) {
  printf("start\r\n");

  json_create_retval_t ret;
  uint8_t buffer[1024];
  json_context_t json;

  if (JSON_SUCCESS != json_create_init(&json, buffer, sizeof(buffer))) return 1;

  printf("end\r\n");
}