/*
    Mason Demerais
    mrd875
    11202338
*/

#ifndef __CHELPSTRING__
#define __CHELPSTRING__

#include <stdbool.h>

typedef char *String;

#include "arraylist.h"

#define MAX_STRING_BUFFER 1023

/*
    Copies the string and returns it. Make sure to free it afterwards.
*/
String StringCopy(String src);

/*
    Adds the string s to src. May change the address of src, so use the return value.
*/
String StringAdd(String src, const String s);

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
ArrayList StringSplitIntoArrayList(String src, String delims);

/*
    Returns a string with the format. Free afterwards.
*/
String StringFormat(String fmt, ...);

/*
    Returns a string being the int. Free afterwards.
*/
String IntToString(int i);

/*Copies a string for collections*/
void *CopyString(void *a);

#endif /* __CHELPSTRING__ */
