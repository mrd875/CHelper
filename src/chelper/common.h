#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

#include <stdbool.h>

/*The function pointer to free data.*/
typedef void (*free_fn_t)(void *data);
/*The function pointer to copy data.*/
typedef void *(*copy_fn_t)(void *data);
/*The function pointer to compare data.*/
typedef bool (*compare_fn_t)(void *data, void *compare_arg);
/*The foreach function pointer*/
typedef void (*foreach_fn_t)(void *data, size_t i);

#include "string.h"

/*
    Prints the last error message.
*/
void PrintError(String calledFrom);

/*
    Creates an int. Free afterwards.
*/
int *IntCopy(int i);

/*
    Creates a float. Free afterwards.
*/
float *FloatCopy(float i);

/*Copy and int for lists and collections*/
void *CopyInt(void *a);

/*Copy a float for lists and collections*/
void *CopyFloat(void *a);

/*Compares the arg for ints.*/
bool EqualInt(void *a, void *b);

/*Compares the arg for ints.*/
bool LessInt(void *a, void *b);

/*Compares the arg for ints.*/
bool GreatInt(void *a, void *b);

#endif /* __CHELPCOMMON__ */
