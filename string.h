/*
    Mason Demerais
    mrd875
    11202338
*/

#ifndef __CHELPSTRING__
#define __CHELPSTRING__

#include <stdbool.h>
#include "common.h"

#define MAX_STRING_BUFFER 1023

/*
    Copies the string and returns it. Make sure to free it afterwards.
*/
String StringCopy(String src);

/*
    Concats the two strings together and returns it. Free afterwards.
*/
String StringConcat(String src, String src2);

/*
    Returns a sub string of s. Make sure to free it after.
*/
String StringGetSubString(String src, int i, int size);

/*
    Returns a string with all occurrences replaced. Make sure to free it after.
*/
String StringReplace(String src, String replace, String with);

/*
    Returns the start index of the found substring. -1 if not found.
*/
int StringFind(String src, String what);

/*
    Returns if the string contains the substring.
*/
bool StringContains(String src, String what);

/*
    Counts the the substring.
*/
int StringCount(String src, String what);

/*
    Splits src into a list of strings delimitted by delims. Make sure to free the list afterwards.
*/
List StringSplitIntoList(String src, String delims);

/*
    Returns a string with the format. Free afterwards.
*/
String StringFormat(String fmt, ...);

/*
    Returns a string being the int. Free afterwards.
*/
String IntToString(int i);

#endif /* __CHELPSTRING__ */
