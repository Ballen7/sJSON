# sJSON
A simple to use JSON creator with no dynamic allocation. Only supports the following types:
* string
* unsigned integers
* boolean
## Example 
```c

 uint8_t buffer[128];
 sjson_context_t ctx;

 char *value_key = "value_key";
 uint16_t value = 0x1122;

 char *string_key = "string_key";
 char *my_string = "This is my string.";

 char *bool_key = "bool_key";
 sjson_boolean_t bool_val = SJSON_TRUE;


 sjson_init(&ctx, buffer, sizeof(buffer));
 sjson_add_integer(&ctx, value_key, strlen(value_key), &value, SJSON_16BIT_INT);
 sjson_add_string(&ctx, string_key, strlen(string_key), my_string, strlen(my_string));
 sjson_add_boolean(&ctx, bool_key, strlen(bool_key), bool_val);
 sjson_complete(&ctx)
```
Which outputs
```json
 {
	"value_key": 4386,
	"string_key": "This is my string.",
	"bool_key": true
 }
```