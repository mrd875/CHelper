#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../chelper/common.h"
#include "../chelper/arraylist.h"
#include "../chelper/string.h"

int main(void)
{
    ArrayList l;

    l = ArrayListCreate(0, &free, NULL);

    ArrayListAddEnd(l, IntCopy(-1));
    for (int i = 0; i < 123; i++)
    {
        ArrayListAddEnd(l, IntCopy(i));
    }

    ArrayListRemoveX(l, 0);
    ArrayListRemoveX(l, 0);
    ArrayListRemoveX(l, 0);
    ArrayListRemoveX(l, 0);
    ArrayListRemoveX(l, 50);

    for (int i = 0; i < ArrayListLength(l); i++)
    {
        printf("%d ", *(int *)ArrayListGetX(l, i));
    }

    ArrayListFree(l);

    return 0;
}
