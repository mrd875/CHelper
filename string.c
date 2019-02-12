#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "string.h"
#include "common.h"
#include "internal/common.h"
#include "list.h"

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

String StringReplace(String src, String replace, String with)
{
    String result;

    assert(src != NULL);
    assert(replace != NULL);
    assert(with != NULL);

    result = StringCopy(src);

    return result;
}

List StringSplitIntoList(String src, String delims)
{
    String token, rest, copy;
    List result;

    assert(src != NULL);
    assert(delims != NULL);

    copy = StringCopy(src);
    result = ListCreate(-1);

    rest = copy;
    while ((token = strtok_r(rest, delims, &rest)) != NULL)
    {
        ListAddLast(result, StringCopy(token), &free, &CopyString);
    }

    free(copy);
    return result;
}
