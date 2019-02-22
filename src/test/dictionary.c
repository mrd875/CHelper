#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../chelper/common.h"
#include "../chelper/arraylist.h"
#include "../chelper/dictionary.h"
#include "../chelper/string.h"

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    Dictionary d;
    int i, *ip, size;
    String s;

    d = DictionaryCreate(&free, &CopyInt);

    size = 10000;
    s = StringCopy("");
    for (i = 0; i < size; i++)
    {
        s = StringAdd(s, "a");

        DictionarySet(d, s, IntCopy(i));
    }
    free(s);

    if (DictionaryLength(d) != (size_t)size)
        fail("Length is wrong.");

    s = StringCopy("");
    for (i = 0; i < size; i++)
    {
        s = StringAdd(s, "a");

        ip = DictionaryGet(d, s);

        if (ip == NULL)
            fail("Get failed.");

        if (*ip != i)
            fail("Get didn't return right value!");
    }
    free(s);

    DictionaryFree(d);

    printf("TESTS COMPLETED!\n");

    return 0;
}
