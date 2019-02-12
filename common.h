#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

#define RETURN_SUCCESS 0
#define is_true(a) (a == RETURN_SECCESS)

#include "string.h"

/*
    Prints the last error message.
*/
void PrintError(String calledFrom);

#endif /* __CHELPCOMMON__ */
