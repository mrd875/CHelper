#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

/*
    This is the data type for lists.
*/
typedef struct List *List;

/*
    String
*/
typedef char *String;

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

/*
    Copies an string.
*/
void *CopyString(void *a);

#endif /* __CHELPCOMMON__ */
