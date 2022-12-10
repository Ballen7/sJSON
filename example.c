#include "sjson.h"

int main(void) {
  sjson_retval_t ret;
  uint8_t buffer[1024];
  sjson_context_t ctx;

  uint8_t *key = "my_key";
  uint8_t value = 88;

  uint8_t *key16 = "my_16bit_key";
  uint16_t value16 = 1616;

  uint8_t *key32 = "my_32bit_key";
  uint32_t value32 = 32323232;

  uint8_t *key64 = "my_64bit_key";
  uint64_t value64 = 646464646464;

  uint8_t *keyString = "my_string_key";
  uint8_t *my_string = "this is my string";

  uint8_t *keyBool = "my_bool_key";
  sjson_boolean_t bool_val = SJSON_TRUE;

  sjson_init(&ctx, buffer, sizeof(buffer));
  sjson_add_integer(&ctx, key, strlen(key), &value, SJSON_8BIT_INT);
  sjson_add_integer(&ctx, key16, strlen(key16), &value16, SJSON_16BIT_INT);
  sjson_add_integer(&ctx, key32, strlen(key32), &value32, SJSON_32BIT_INT);
  sjson_add_integer(&ctx, key64, strlen(key64), &value64, SJSON_64BIT_INT);
  sjson_add_string(&ctx, keyString, strlen(keyString), my_string,
                   strlen(my_string));
  sjson_add_boolean(&ctx, keyBool, strlen(keyBool), bool_val);
  sjson_complete(&ctx);

  return 1;
}