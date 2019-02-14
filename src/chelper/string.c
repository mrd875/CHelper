#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#include "string.h"
#include "common.h"
#include "internal/common.h"
#include "arraylist.h"

String StringCopy(String src)
{
    String result;

    assert(src != NULL);

    debugp("Copying string");
    result = calloc(sizeof(char), strlen(src) + 1);
    assert(result != NULL);

    strcpy(result, src);

    return result;
}

void *CopyString(void *a)
{
    if (a == NULL)
        return NULL;

    return StringCopy(a);
}

String StringAdd(String src, const String s)
{
    int slen, slen2;

    assert(src != NULL);
    assert(s != NULL);

    slen = strlen(src);
    slen2 = strlen(s);

    src = realloc(src, (slen + slen2 + 1) * (sizeof(char)));
    assert(src != NULL);

    memset(&src[slen], '\0', slen2 + 1);
    strcpy(&src[slen], s);

    return src;
}

String StringConcat(String src, String src2)
{
    String result;

    assert(src != NULL);
    assert(src2 != NULL);

    debugp("Concatting strings");
    result = calloc(sizeof(char), strlen(src) + strlen(src2) + 1);
    assert(result != NULL);

    strcpy(result, src);
    strcat(result, src2);

    return result;
}

String StringGetSubString(String src, int i, int size)
{
    String result;
    int slen;

    assert(src != NULL);
    assert(i >= 0);
    assert(size >= 0);

    slen = strlen(src);
    assert(i < slen);

    debugp("Getting substr");
    result = calloc(sizeof(char), size + 1);
    assert(result != NULL);

    strncpy(result, &src[i], size);

    return result;
}

int StringFind(String src, String what)
{
    String found;

    assert(src != NULL);
    assert(what != NULL);

    found = strstr(src, what);

    if (found == NULL)
        return -1;

    return (int)(abs(src - found));
}

bool StringContains(String src, String what)
{
    assert(src != NULL);
    assert(what != NULL);

    if (StringFind(src, what) == -1)
        return false;

    return true;
}

int StringCount(String src, String what)
{
    int i, whatlen;
    String sub;

    assert(src != NULL);
    assert(what != NULL);

    whatlen = strlen(what);
    i = 0;

    sub = src;
    while ((sub = strstr(sub, what)) != NULL)
    {
        i++;
        sub += whatlen;
    }

    return i;
}

String StringReplace(String src, String replace, String with)
{
    String result, s;
    int repLen, withLen, i;

    assert(src != NULL);
    assert(replace != NULL);
    assert(with != NULL);

    repLen = strlen(replace);
    withLen = strlen(with);

    i = StringCount(src, replace);

    result = calloc(sizeof(char), strlen(src) + i * (withLen - repLen) + 1);
    assert(result != NULL);

    s = src;
    i = 0;
    while (*s)
    {
        if (strstr(s, replace) == s)
        {
            strcpy(&result[i], with);
            i += withLen;
            s += repLen;
        }
        else
            result[i++] = *s++;
    }

    return result;
}

ArrayList StringSplitIntoArrayList(String src, String delims)
{
    String token, rest, copy;
    ArrayList result;

    assert(src != NULL);
    assert(delims != NULL);

    copy = StringCopy(src);
    result = ArrayListCreate(0, &free, &CopyString);

    rest = copy;
    while ((token = strtok_r(rest, delims, &rest)) != NULL)
    {
        ArrayListAdd(result, StringCopy(token));
    }

    free(copy);
    return result;
}

String StringFormat(String fmt, ...)
{
    String result;
    char buff[MAX_STRING_BUFFER + 1];
    va_list args;
    int rc;

    assert(fmt != NULL);

    buff[MAX_STRING_BUFFER] = '\0';

    rc = 0;

    va_start(args, fmt);
    rc = vsnprintf(buff, MAX_STRING_BUFFER, fmt, args);
    va_end(args);

    assert(rc >= 0);

    result = StringCopy(buff);
    return result;
}

String IntToString(int i)
{
    return StringFormat("%d", i);
}
