#include <stdlib.h>

#include "../common.h"
#include "../list.h"

int main(void)
{
    int i;
    List l;

    l = ListCreate(-1);

    for (i = 0; i < 10; i++)
    {
        ListAddFirst(l, QuickCreateInt(i), &free, &CopyInt);
    }

    ListAddX(l, QuickCreateInt(5), &free, &CopyInt, 5);

    ListDeleteFirst(l);

    ListDeleteLast(l);

    ListDeleteX(l, 0);

    ListDeleteX(l, ListSize(l) - 1);

    ListDeleteX(l, 1);

    ListDebugPrintInt(l);

    ListFree(l);
}
