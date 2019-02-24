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
    int i, ran;

    t = AVLTreeCreate(&free, &CopyInt);

    for (i = -1000000; i < 1000000; i++)
    {
        ran = rand();

        AVLTreeSet(t, ran, IntCopy(i));

        if (!AVLTreeHas(t, ran))
            fail("Doesn't has!");

        if (*(int *)AVLTreeGet(t, ran) != i)
            fail("Wrong get!");
    }

    AVLTreeFree(t);

    printf("COMPLETED TESTS\n");

    return 0;
}