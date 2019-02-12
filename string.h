/*
    Mason Demerais
    mrd875
    11202338
*/

#ifndef __CHELPSTRING__
#define __CHELPSTRING__

#include "list.h"

#define MAX_STRING_BUFFER 1023

typedef char *String;

/*
    Copies the string and returns it. Make sure to free it afterwards.
*/
String StringCopy(String src);

/*
    Returns a sub string of s. Make sure to free it after.
*/
String StringGetSubString(String src, int i, int size);

/*
    Returns a string with all occurrences replaced. Make sure to free it after.
*/
String StringReplace(String src, String replace, String with);

/*
    Splits src into a list of strings delimitted by delims. Make sure to free the list afterwards.
*/
List StringSplitIntoList(String src, String delims);

#endif /* __CHELPSTRING__ */
