#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

/*
    This is the data type for lists.
*/
typedef struct List *List;

/*
    This struct holds the data args.
*/
typedef struct ListDataArgs
{
    /* The function to free the data. */
    void (*free_routine)(void *data);

    /* The function to copy the data. */
    void *(*copy_routine)(void *data);

    /* Size of the data */
    int size;
} ListDataArgs;

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

/*
    Null data args
*/
ListDataArgs ListDataArgsNULL();

/*
    string data args, make sure to update the size to reflect how big ur string is
*/
ListDataArgs ListDataArgsString();

/*
    int data args
*/
ListDataArgs ListDataArgsInt();

#endif /* __CHELPCOMMON__ */
