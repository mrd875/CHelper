#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../chelper/common.h"
#include "../chelper/avltree.h"
#include "../chelper/string.h"

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    AVLTree t;
    int i, low, high;
    size_t size;

    t = AVLTreeCreate(&free, &CopyInt);

    low = -100;
    high = 100;
    size = 0;

    if (AVLTreeLength(t) != size)
        fail("Size was wrong!");

    for (i = low; i <= high; i++)
    {
        AVLTreeAdd(t, i, IntCopy(i));
        size++;

        if (!AVLTreeHas(t, i))
            fail("Doesn't has!");

        if (*(int *)AVLTreeGet(t, i) != i)
            fail("Wrong get!");

        if (AVLTreeLength(t) != size)
            fail("Size was wrong!");
    }

    if (AVLTreeLength(t) != size)
        fail("Size was wrong!");

    for (i = low; i <= high; i++)
    {
        if (!AVLTreeHas(t, i))
            fail("Doesn't has!");

        if (*(int *)AVLTreeGet(t, i) != i)
            fail("Wrong get!");
    }

    for (i = low; i <= high; i++)
    {
        AVLTreeRemove(t, i);
        size--;

        if (AVLTreeHas(t, i))
            fail("Has!");

        if (AVLTreeLength(t) != size)
            fail("Size was wrong!");
    }

    if (AVLTreeLength(t) != size)
        fail("Size was wrong!");

    for (i = low; i <= high; i++)
    {
        AVLTreeAdd(t, i, IntCopy(i));
        size++;

        if (!AVLTreeHas(t, i))
            fail("Doesn't has!");

        if (*(int *)AVLTreeGet(t, i) != i)
            fail("Wrong get!");

        if (AVLTreeLength(t) != size)
            fail("Size was wrong!");
    }

    AVLTreeFree(t);

    t = AVLTreeCreate(&free, &CopyInt);

    AVLTreeAdd(t, 12, IntCopy(0));
    AVLTreeAdd(t, 14, IntCopy(0));
    AVLTreeAdd(t, 22, IntCopy(0));
    AVLTreeAdd(t, 35, IntCopy(0));
    AVLTreeAdd(t, 36, IntCopy(0));
    AVLTreeAdd(t, 43, IntCopy(0));
    AVLTreeAdd(t, 55, IntCopy(0));
    AVLTreeAdd(t, 63, IntCopy(0));
    AVLTreeAdd(t, 73, IntCopy(0));
    AVLTreeAdd(t, 99, IntCopy(0));
    AVLTreeAdd(t, 40, IntCopy(0));

    printf("\n\nPrint left rots");
    AVLTreePrintKeys(t);
    AVLTreeClear(t);

    AVLTreeAdd(t, 99, IntCopy(0));
    AVLTreeAdd(t, 88, IntCopy(0));
    AVLTreeAdd(t, 77, IntCopy(0));
    AVLTreeAdd(t, 66, IntCopy(0));
    AVLTreeAdd(t, 55, IntCopy(0));
    AVLTreeAdd(t, 44, IntCopy(0));
    AVLTreeAdd(t, 33, IntCopy(0));
    AVLTreeAdd(t, 22, IntCopy(0));
    AVLTreeAdd(t, 11, IntCopy(0));
    AVLTreeAdd(t, 10, IntCopy(0));
    AVLTreeAdd(t, 9, IntCopy(0));

    printf("\n\nPrint right rots");
    AVLTreePrintKeys(t);
    AVLTreeClear(t);

    AVLTreeFree(t);

    printf("COMPLETED TESTS\n");

    return 0;
}