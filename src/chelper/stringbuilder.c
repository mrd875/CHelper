#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "stringbuilder.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

struct StringBuilder
{
    /*THe number of chars*/
    size_t length;
    /*The data*/
    ArrayList data;
};

StringBuilder StringBuilderCreate()
{
    StringBuilder sb;

    sb = malloc(sizeof(struct StringBuilder));
    assert(sb != NULL);

    sb->data = ArrayListCreate(0, &free, &CopyString);
    sb->length = 0;

    return sb;
}

StringBuilder StringBuilderCopy(StringBuilder sb)
{
    StringBuilder answer;

    assert(sb != NULL);

    answer = StringBuilderCreate();

    ArrayListFree(answer->data);
    answer->data = ArrayListCopy(sb->data);

    answer->length = sb->length;

    return answer;
}

void StringBuilderFree(StringBuilder sb)
{
    assert(sb != NULL);

    StringBuilderClear(sb);

    ArrayListFree(sb->data);

    free(sb);
}

void StringBuilderClear(StringBuilder sb)
{
    assert(sb != NULL);

    ArrayListClear(sb->data);

    sb->length = 0;
}

size_t StringBuilderLength(StringBuilder sb)
{
    assert(sb != NULL);

    return sb->length;
}

bool StringBuilderAdd(StringBuilder sb, String s)
{
    size_t slen;
    String result;

    assert(sb != NULL);
    assert(s != NULL);

    slen = strlen(s);

    sb->length += slen;

    result = calloc(sizeof(char), slen + 1);
    assert(result != NULL);

    memcpy(result, s, slen + 1);

    ArrayListAdd(sb->data, result);

    return true;
}

/*Adds a formatted string.*/
bool StringBuilderAddFormatted(StringBuilder sb, String fmt, ...);

String StringBuilderToString(StringBuilder sb)
{
    String result, temp;
    ArrayList l;

    assert(sb != NULL);

    result = calloc(sizeof(char), sb->length + 1);
    assert(result != NULL);

    l = sb->data;

    ArrayListCursorBeforeHead(l);
    while (ArrayListCursorHasNext(l))
    {
        ArrayListCursorNext(l);

        temp = (String)ArrayListCursorGet(l);

        strcat(result, temp);
    }

    return result;
}

void StringBuilderPrint(StringBuilder sb)
{
    String s;

    assert(sb != NULL);

    s = StringBuilderToString(sb);

    printf("%s\n", s);

    free(s);
}
