#include "common.h"
#include "../common.h"
#include "../string.h"

String CurrentErrorMsg;

void SetErrorMessage(String msg)
{
    CurrentErrorMsg = StringCopy(msg);
}

String GetErrorMessage()
{
    return CurrentErrorMsg;
}
