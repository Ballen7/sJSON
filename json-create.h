#include <stdint.h>
#include <stddef.h>

typedef enum {
    JSON_INIT = 0,
    JSON_START,
    ADD_KEY,
    ADD_TOKEN,
    JSON_COMPLETE,
    JSON_VALID,
    JSON_INVALID 
} builder_state_t;

typedef enum {
    JSON_SUCCESS = 0,
    JSON_ERROR,
    JSON_INVALID_BUFFER
} json_create_retval_t;

typedef enum {
    JSON_8BIT_INT = 0,
    JSON_16BIT_INT,
    JSON_32BIT_INT,
    JSON_64BIT_INT,
    JSON_STRING,
} json_value_type_t;

typedef struct {
    uint8_t *pBuf;
    size_t buf_size;
    size_t index;
    size_t key_pair_count;
    builder_state_t state;
} json_context_t;