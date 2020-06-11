#include "include/token.h"
#include <stdlib.h>
#include <string.h>


token_T* init_token(int type, char* value)
{
    token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;
    token->value = value;
    token->float_value = 0;

    if (type == TOKEN_FLOAT_VALUE || type == TOKEN_INTEGER_VALUE)
    {
        char* ptr;
        token->float_value = strtof(value, &ptr);
    }

    return token;
}
