#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <chelper/common.h>
#include <chelper/arraylist.h>
#include <chelper/string.h>

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

void EnsureSize(ArrayList l, size_t expectSize)
{
    size_t i;

    if (expectSize > 0)
    {
        if (ArrayListIsEmpty(l))
            fail("List shouldn't be empty");
    }
    else
    {
        if (!ArrayListIsEmpty(l))
            fail("List should be empty");
    }

    if (ArrayListLength(l) != expectSize)
    {
        fail("List was not expected size");
    }

    i = 0;
    ArrayListCursorBeforeHead(l);
    while (ArrayListCursorHasNext(l))
    {
        i++;
        ArrayListCursorNext(l);
    }

    if (i != expectSize)
    {
        fail("List cursor didn't iterator over expectedSize things.");
    }

    i = 0;
    ArrayListCursorAfterTail(l);
    while (ArrayListCursorHasPrevious(l))
    {
        i++;
        ArrayListCursorPrevious(l);
    }

    if (i != expectSize)
    {
        fail("List cursor didn't iterator over expectedSize things.");
    }
}

void EnsureContents(ArrayList l, String expect)
{
    String is, temp;
    size_t i, len;
    int *a;

    len = ArrayListLength(l);
    is = StringCopy("");
    for (i = 0; i < len; i++)
    {
        a = ArrayListGetX(l, i);

        if (a == NULL)
            is = StringAdd(is, "NULL ");
        else
        {
            temp = StringFormat("%d ", *a);
            is = StringAdd(is, temp);
            free(temp);
        }
    }

    if (strcmp(expect, is))
    {
        printf("%s\n\n%s\n\n", expect, is);
        fail("Contents do not match!");
    }

    free(is);

    is = StringCopy("");

    ArrayListCursorBeforeHead(l);
    while (ArrayListCursorHasNext(l))
    {
        ArrayListCursorNext(l);

        a = ArrayListCursorGet(l);

        if (a == NULL)
            is = StringAdd(is, "NULL ");
        else
        {
            temp = StringFormat("%d ", *a);
            is = StringAdd(is, temp);
            free(temp);
        }
    }

    if (strcmp(expect, is))
    {
        printf("%s\n\n%s\n\n", expect, is);
        fail("Contents do not match!");
    }

    free(is);
}

int main(void)
{
    ArrayList l, l2, l3;
    int i;
    size_t size;
    String build, temp;

    l = ArrayListCreate(0, &free, &CopyInt);
    size = 0;
    build = StringCopy("");

    EnsureSize(l, 0);
    EnsureContents(l, build);

    for (i = -200; i < 201; i++)
    {
        size++;
        temp = StringFormat("%d ", i);
        build = StringAdd(build, temp);
        free(temp);
        /*printf("Adding %d...\n", i);*/

        ArrayListAdd(l, IntCopy(i));
        EnsureSize(l, size);
        EnsureContents(l, build);
    }

    for (i = -200; i < 197; i++)
    {
        size--;
        temp = StringFormat("%d ", *(int *)ArrayListGet(l));
        build = StringChop(build, strlen(temp));
        free(temp);
        /*printf("Removing %d...\n", *(int *)ArrayListGet(l));*/

        ArrayListRemove(l);
        EnsureSize(l, size);
        EnsureContents(l, build);
    }

    ArrayListFree(l);
    free(build);

    l = ArrayListCreate(-1, &free, &CopyInt);

    EnsureSize(l, 0);
    EnsureContents(l, "");

    ArrayListAdd(l, IntCopy(7));
    EnsureSize(l, 1);
    EnsureContents(l, "7 ");

    ArrayListAddX(l, IntCopy(5), 0);
    EnsureSize(l, 2);
    EnsureContents(l, "5 7 ");

    ArrayListAddX(l, IntCopy(2), 1);
    EnsureSize(l, 3);
    EnsureContents(l, "5 2 7 ");

    ArrayListAddX(l, IntCopy(3), 1);
    EnsureSize(l, 4);
    EnsureContents(l, "5 3 2 7 ");

    i = 5;
    if (ArrayListIndexOf(l, &EqualInt, &i) != 0)
    {
        fail("Index of 5 should be 0!");
    }

    i = 3;
    if (ArrayListIndexOf(l, &EqualInt, &i) != 1)
    {
        fail("Index of 3 should be 1!");
    }

    i = 2;
    if (ArrayListIndexOf(l, &EqualInt, &i) != 2)
    {
        fail("Index of 2 should be 2!");
    }

    i = 7;
    if (ArrayListIndexOf(l, &EqualInt, &i) != 3)
    {
        fail("Index of 7 should be 3!");
    }

    if (*(int *)ArrayListGet(l) != i)
    {
        fail("Last element should be 7!");
    }

    i = 9;
    if (ArrayListIndexOf(l, &EqualInt, &i) != -1)
    {
        fail("Index of 9 should be -1!");
    }

    ArrayListRemove(l);
    EnsureSize(l, 3);
    EnsureContents(l, "5 3 2 ");

    ArrayListRemoveX(l, 0);
    EnsureSize(l, 2);
    EnsureContents(l, "3 2 ");

    ArrayListAddX(l, IntCopy(1), 0);
    EnsureSize(l, 3);
    EnsureContents(l, "1 3 2 ");

    ArrayListRemoveX(l, 1);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");

    l2 = ArrayListCopy(l);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");
    EnsureSize(l2, 2);
    EnsureContents(l2, "1 2 ");

    ArrayListSetX(l2, IntCopy(3), 0);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");
    EnsureSize(l2, 2);
    EnsureContents(l2, "3 2 ");

    ArrayListRemove(l2);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");
    EnsureSize(l2, 1);
    EnsureContents(l2, "3 ");

    l3 = ArrayListConcat(l, l2);
    EnsureSize(l3, 3);
    EnsureContents(l3, "1 2 3 ");

    ArrayListFree(l3);
    ArrayListFree(l2);
    ArrayListFree(l);

    l = ArrayListCreate(-1, &free, &CopyInt);
    ArrayListAdd(l, IntCopy(1));
    ArrayListAdd(l, IntCopy(2));
    ArrayListAdd(l, IntCopy(3));
    ArrayListAdd(l, IntCopy(4));
    EnsureSize(l, 4);
    EnsureContents(l, "1 2 3 4 ");

    l2 = ArrayListGetSubArrayList(l, 1, 2);
    EnsureSize(l2, 2);
    EnsureContents(l2, "2 3 ");
    ArrayListFree(l2);

    ArrayListClear(l);
    EnsureSize(l, 0);
    EnsureContents(l, "");

    for (i = 0; i < 100; i++)
    {
        ArrayListAdd(l, IntCopy(i));
    }
    EnsureSize(l, 100);

    i = 94;
    l2 = ArrayListFilter(l, &LessInt, &i);
    EnsureSize(l2, 6);
    EnsureContents(l2, "94 95 96 97 98 99 ");
    ArrayListFree(l2);

    ArrayListCursorBeforeHead(l);
    if (!ArrayListCursorSearchNext(l, &EqualInt, &i))
    {
        fail("Couldn't find i!");
    }

    if (ArrayListCursorIndexOf(l) != 94)
    {
        fail("Wrong index!");
    }

    if (ArrayListCursorSearchNext(l, &EqualInt, &i))
    {
        fail("shouldn't find i!");
    }

    ArrayListFree(l);

    printf("COMPLETED TEST!\n");
    return 0;
}
