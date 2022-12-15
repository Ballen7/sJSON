# sJSON
A simple to use JSON creator with no dynamic allocation. Only supports the following types:
* string
* unsigned integers
* boolean
* JSON Objects
## Example 
```c
 uint8_t buffer[256];
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
 sjson_complete(&ctx);
```
Which outputs
```json
 {
  "value_key": 4386,
  "string_key": "This is my string.",
  "bool_key": true
 }
```
To create a JSON object one could do,
```c
 uint8_t buffer2[256];
 sjson_context_t ctx2;

 char *json_obj_key = "obj_key";
 char *json_obj = ctx.pBuf; /* from example above */

 sjson_init(&ctx2, buffer2, sizeof(buffer2));
 sjson_add_object(&ctx2, json_obj_key, strlen(json_obj_key), json_obj, strlen(json_obj));
 sjson_complete(&ctx2);

```
which outputs
```json
{
  "obj_key": {
    "value_key": 4386,
    "string_key": "This is my string.",
    "bool_key": true
  }
}
```