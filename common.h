#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

#include "string.h"

/*
    Prints the last error message.
*/
void PrintError(String calledFrom);

/*
    Creates an int. Free afterwards.
*/
int *QuickCreateInt(int i);

/*
    Copies an int.
*/
void *CopyInt(void *a);

#endif /* __CHELPCOMMON__ */
