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

    low = -1000;
    high = 1000;
    size = 0;

    if (AVLTreeLength(t) != size)
        fail("Size was wrong!");

    for (i = low; i <= high; i++)
    {
        AVLTreeSet(t, i, IntCopy(i));
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

    AVLTreeFree(t);

    printf("COMPLETED TESTS\n");

    return 0;
}