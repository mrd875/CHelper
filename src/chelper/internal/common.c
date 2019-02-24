#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "../common.h"
#include "../string.h"

char CurrentErrorMsg[MAX_STRING_BUFFER + 1];

void SetErrorMessage(String msg)
{
    int i, slen;
    assert(msg != NULL);

    slen = strlen(msg);

    for (i = 0; i < slen && i < MAX_STRING_BUFFER; i++)
    {
        CurrentErrorMsg[i] = msg[i];
    }

    CurrentErrorMsg[i] = '\0';
    CurrentErrorMsg[MAX_STRING_BUFFER] = '\0';
}

String GetErrorMessage()
{
    return CurrentErrorMsg;
}
