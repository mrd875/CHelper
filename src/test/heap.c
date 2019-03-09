#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <chelper/common.h>
#include <chelper/heap.h>
#include <chelper/string.h>

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    Heap h;
    ArrayList l;
    int i;

    h = HeapCreate(false, &free, &CopyInt);

    printf("Adding 3, 5, -1, 2, 3, 4, -2, -3, -4\n");
    i = 3;
    HeapAdd(h, i, IntCopy(i));
    i = 5;
    HeapAdd(h, i, IntCopy(i));
    i = -1;
    HeapAdd(h, i, IntCopy(i));
    i = 2;
    HeapAdd(h, i, IntCopy(i));
    i = 3;
    HeapAdd(h, i, IntCopy(i));
    i = 4;
    HeapAdd(h, i, IntCopy(i));
    i = -2;
    HeapAdd(h, i, IntCopy(i));
    i = -3;
    HeapAdd(h, i, IntCopy(i));
    i = -4;
    HeapAdd(h, i, IntCopy(i));

    l = HeapToArrayList(h);
    ArrayListPrintInt(l);
    ArrayListFree(l);

    printf("Removing 4 elements...\n");
    HeapRemove(h);
    HeapRemove(h);
    HeapRemove(h);
    HeapRemove(h);

    l = HeapToArrayList(h);
    ArrayListPrintInt(l);
    ArrayListFree(l);

    printf("Clearing and printing...\n");
    HeapClear(h);

    l = HeapToArrayList(h);
    ArrayListPrintInt(l);
    ArrayListFree(l);

    HeapFree(h);

    printf("New reverse heap: adding -4, -6, -8, 1, 2, 3, 87, -498, 0\n");
    h = HeapCreate(true, &free, &CopyInt);

    i = -4;
    HeapAdd(h, i, IntCopy(i));
    i = -6;
    HeapAdd(h, i, IntCopy(i));
    i = -8;
    HeapAdd(h, i, IntCopy(i));
    i = 1;
    HeapAdd(h, i, IntCopy(i));
    i = 2;
    HeapAdd(h, i, IntCopy(i));
    i = 3;
    HeapAdd(h, i, IntCopy(i));
    i = 87;
    HeapAdd(h, i, IntCopy(i));
    i = -498;
    HeapAdd(h, i, IntCopy(i));
    i = 0;
    HeapAdd(h, i, IntCopy(i));

    l = HeapToArrayList(h);
    ArrayListPrintInt(l);
    ArrayListFree(l);

    printf("Removing top 3 elements...\n");
    HeapRemove(h);
    HeapRemove(h);
    HeapRemove(h);

    l = HeapToArrayList(h);
    ArrayListPrintInt(l);
    ArrayListFree(l);

    HeapFree(h);

    printf("COMPLETED TESTS\n");

    return 0;
}
