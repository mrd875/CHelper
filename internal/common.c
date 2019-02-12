#include <assert.h>
#include <stddef.h>

#include "common.h"
#include "../common.h"
#include "../string.h"

String CurrentErrorMsg = NULL;

void SetErrorMessage(String msg)
{
    assert(msg != NULL);

    CurrentErrorMsg = StringCopy(msg);
}

String GetErrorMessage()
{
    return CurrentErrorMsg;
}
