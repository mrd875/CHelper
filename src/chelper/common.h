#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

/*The function pointer to free data.*/
typedef void (*free_fn_t)(void *data);
/*The function pointer to copy data.*/
typedef void *(*copy_fn_t)(void *data);

#include "string.h"

/*
    Prints the last error message.
*/
void PrintError(String calledFrom);

/*
    Creates an int. Free afterwards.
*/
int *IntCopy(int i);

/*Copy and int for lists and collections*/
void *CopyInt(void *a);

#endif /* __CHELPCOMMON__ */
