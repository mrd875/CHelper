#include <stdlib.h>
#include <stdio.h>

#include "../common.h"
#include "../list.h"
#include "../string.h"

int main(void)
{
    int i;
    List l, l2;

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

    l2 = ListGetSubList(l, 1, 3);
    ListDebugPrintInt(l2);

    ListChangeX(l2, QuickCreateInt(1000), &free, &CopyInt, 2);
    ListDebugPrintInt(l);
    ListDebugPrintInt(l2);

    ListFree(l);
    ListFree(l2);
}
