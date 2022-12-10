#include "sjson.h"

int main(void) {
  sjson_retval_t ret;
  uint8_t buffer[1024];
  sjson_context_t ctx;

  uint8_t *key = "my_key";
  uint8_t value = 88;

  uint8_t *key16 = "my_16_key";
  uint16_t value16 = 1616;
  
  uint8_t *key32 = "my_32_key";
  uint32_t value32 = 32323232;

  uint8_t *key64 = "my_64_key";
  uint64_t value64 = 646464646464;

  sjson_init(&ctx, buffer, sizeof(buffer));
  sjson_add_integer(&ctx, key, strlen(key), &value, SJSON_8BIT_INT);
  sjson_add_integer(&ctx, key16, strlen(key16), &value16, SJSON_16BIT_INT);
  sjson_add_integer(&ctx, key32, strlen(key32), &value32, SJSON_32BIT_INT);
  sjson_add_integer(&ctx, key64, strlen(key64), &value64, SJSON_64BIT_INT);

  sjson_complete(&ctx);

  return 1;
}