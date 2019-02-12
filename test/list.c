#include <stdlib.h>
#include <stdio.h>

#include "../common.h"
#include "../list.h"
#include "../string.h"

int main(void)
{
    int i;
    List l;

    l = ListCreate(-1);

    for (i = 0; i < 10; i++)
    {
        ListAddFirst(l, QuickCreateInt(i), &free, &CopyInt);
    }

    ListCursorBeforeHead(l);

    ListAddX(l, QuickCreateInt(5), &free, &CopyInt, 5);

    ListDeleteFirst(l);

    ListDeleteLast(l);

    ListDeleteX(l, 0);

    ListDeleteX(l, ListSize(l) - 1);

    ListDeleteX(l, 1);

    ListAddFirst(l, NULL, NULL, NULL);

    ListAddLast(l, StringCopy("hello"), &free, &CopyString);

    while (ListCursorHasNext(l))
    {
        ListCursorNext(l);
        printf("1");
        ListCursorGet(l);
    }

    ListDebugPrintInt(l);
    ListDebugPrintString(l);

    ListFree(l);
}
