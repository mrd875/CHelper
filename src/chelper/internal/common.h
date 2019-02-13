#ifndef __CHELPCOMMONINTERN__
#define __CHELPCOMMONINTERN__

#include "../string.h"
#include <stdio.h>

#ifdef __CHELPDEBUG__
#define debugp(...)               \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n")
#else
#define debugp(...) (void)0
#endif

/*
    Sets the error message.
*/
void SetErrorMessage(String msg);

/*
    Gets the current error message.
*/
String GetErrorMessage();

#endif /* __CHELPCOMMONINTERN__ */
