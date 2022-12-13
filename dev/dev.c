#include "sjson.h"

#include <assert.h>

int main(void) {
  sjson_retval_t ret;
  uint8_t buffer[256];
  sjson_context_t ctx;

  const char *key8 = "my_key";
  uint8_t value8 = 88;

  const char *key16 = "my_16bit_key";
  uint16_t value16 = 1616;

  const char *key32 = "my_32bit_key";
  uint32_t value32 = 32323232;

  const char *key64 = "my_64bit_key";
  uint64_t value64 = 646464646464;

  const char *keyString = "my_string_key";
  const char *my_string = "this is my string";

  const char *keyBool = "my_bool_key";
  sjson_boolean_t bool_val = SJSON_TRUE;

  sjson_init(&ctx, buffer, sizeof(buffer));

  sjson_add_integer(&ctx, (uint8_t *)key8, strlen(key8), (void *)&value8,
                    SJSON_8BIT_INT);

  sjson_add_integer(&ctx, (uint8_t *)key16, strlen(key16), (void *)&value16,
                    SJSON_16BIT_INT);

  sjson_add_integer(&ctx, (uint8_t *)key32, strlen(key32), (void *)&value32,
                    SJSON_32BIT_INT);

  sjson_add_integer(&ctx, (uint8_t *)key64, strlen(key64), (void *)&value64,
                    SJSON_64BIT_INT);

  sjson_add_string(&ctx, (uint8_t *)keyString, strlen(keyString),
                   (void *)my_string, strlen(my_string));

  sjson_add_boolean(&ctx, (uint8_t *)keyBool, strlen(keyBool), bool_val);

  if(SJSON_SUCCESS != sjson_complete(&ctx))
    assert(0);

  return 1;
}