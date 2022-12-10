#include "sjson.h"

int main(void) {
  sjson_retval_t ret;
  uint8_t buffer[64];
  sjson_context_t ctx;

  if (JSON_SUCCESS != sjson_init(&ctx, buffer, sizeof(buffer))) return 1;

  uint8_t *key = "my_key";
  uint64_t value = 69696969;
  sjson_add_pair(&ctx, key, strlen(key), &value, 1, SJSON_64BIT_INT);

  return 1;
}