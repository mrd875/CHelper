#ifndef __CHELPCOMMON__
#define __CHELPCOMMON__

/*
    This is the data type for lists.
*/
typedef struct List *List;

/*
    This is the data type for dictionaries.
*/
typedef struct Dictionary *Dictionary;

/*
    This struct holds the data args.
*/
typedef struct DataArgs
{
    /* The function to free the data. */
    void (*free_routine)(void *data);

    /* The function to copy the data. */
    void *(*copy_routine)(void *data);

    /* Size of the data */
    int size;
} DataArgs;

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
int *IntCopy(int i);

/*
    Null data args
*/
DataArgs DataArgsNULL();

/*
    string data args, make sure to update the size to reflect how big ur string is
*/
DataArgs DataArgsString();

/*
    int data args
*/
DataArgs DataArgsInt();

#endif /* __CHELPCOMMON__ */