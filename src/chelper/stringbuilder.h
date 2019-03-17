#ifndef __CHELPERSTRINGBUILDER__
#define __CHELPERSTRINGBUILDER__

#include <stdbool.h>
#include <stdlib.h>

/* This is the string builder. */
typedef struct StringBuilder *StringBuilder;

#include "common.h"
#include "string.h"

/*Creates a new StringBuilder*/
StringBuilder StringBuilderCreate();

/* Copies the StringBuilder. */
StringBuilder StringBuilderCopy(StringBuilder sb);

/*Frees the StringBuilder*/
void StringBuilderFree(StringBuilder sb);

/*CLears the StringBuilder*/
void StringBuilderClear(StringBuilder sb);

/*Gets the amount of chars in the StringBuilder.*/
size_t StringBuilderLength(StringBuilder sb);

/*Adds the string (copies) to the builder*/
bool StringBuilderAdd(StringBuilder sb, String s);

/*Adds a char to the builder*/
bool StringBuilderAddChar(StringBuilder sb, char c);

/*Adds a formatted string.*/
bool StringBuilderAddFormatted(StringBuilder sb, String fmt, ...);

/*Creates the string from the StringBuilder*/
String StringBuilderToString(StringBuilder sb);

/*Prints the StringBuilder*/
void StringBuilderPrint(StringBuilder sb);

#endif /* __CHELPERSTRINGBUILDER__ */
