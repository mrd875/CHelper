#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../chelper/common.h"
#include "../chelper/linkedlist.h"
#include "../chelper/string.h"

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

void EnsureSize(LinkedList l, size_t expectSize)
{
    size_t i;

    if (expectSize > 0)
    {
        if (LinkedListIsEmpty(l))
            fail("List shouldn't be empty");
    }
    else
    {
        if (!LinkedListIsEmpty(l))
            fail("List should be empty");
    }

    if (LinkedListLength(l) != expectSize)
    {
        fail("List was not expected size");
    }

    i = 0;
    LinkedListCursorBeforeHead(l);
    while (LinkedListCursorHasNext(l))
    {
        i++;
        LinkedListCursorNext(l);
    }

    if (i != expectSize)
    {
        fail("List cursor didn't iterator over expectedSize things.");
    }

    i = 0;
    LinkedListCursorAfterTail(l);
    while (LinkedListCursorHasPrevious(l))
    {
        i++;
        LinkedListCursorPrevious(l);
    }

    if (i != expectSize)
    {
        fail("List cursor didn't iterator over expectedSize things.");
    }
}

void EnsureContents(LinkedList l, String expect)
{
    String is, temp;
    size_t i, len;
    int *a;

    len = LinkedListLength(l);
    is = StringCopy("");
    for (i = 0; i < len; i++)
    {
        a = LinkedListGetX(l, i);

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

    LinkedListCursorBeforeHead(l);
    while (LinkedListCursorHasNext(l))
    {
        LinkedListCursorNext(l);

        a = LinkedListCursorGet(l);

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
    LinkedList l, l2, l3;
    int i;
    size_t size;
    String build, temp;

    l = LinkedListCreate(0, &free, &CopyInt);
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
        //printf("Adding %d...\n", i);

        LinkedListAdd(l, IntCopy(i));
        EnsureSize(l, size);
        EnsureContents(l, build);
    }

    for (i = -200; i < 197; i++)
    {
        size--;
        temp = StringFormat("%d ", *(int *)LinkedListGet(l));
        build = StringChop(build, strlen(temp));
        free(temp);
        //printf("Removing %d...\n", *(int *)LinkedListGet(l));

        LinkedListRemove(l);
        EnsureSize(l, size);
        EnsureContents(l, build);
    }

    LinkedListFree(l);
    free(build);

    l = LinkedListCreate(-1, &free, &CopyInt);

    EnsureSize(l, 0);
    EnsureContents(l, "");

    LinkedListAdd(l, IntCopy(7));
    EnsureSize(l, 1);
    EnsureContents(l, "7 ");

    LinkedListAddX(l, IntCopy(5), 0);
    EnsureSize(l, 2);
    EnsureContents(l, "5 7 ");

    LinkedListAddX(l, IntCopy(2), 1);
    EnsureSize(l, 3);
    EnsureContents(l, "5 2 7 ");

    LinkedListAddX(l, IntCopy(3), 1);
    EnsureSize(l, 4);
    EnsureContents(l, "5 3 2 7 ");

    i = 5;
    if (LinkedListIndexOf(l, &EqualInt, &i) != 0)
    {
        fail("Index of 5 should be 0!");
    }

    i = 3;
    if (LinkedListIndexOf(l, &EqualInt, &i) != 1)
    {
        fail("Index of 3 should be 1!");
    }

    i = 2;
    if (LinkedListIndexOf(l, &EqualInt, &i) != 2)
    {
        fail("Index of 2 should be 2!");
    }

    i = 7;
    if (LinkedListIndexOf(l, &EqualInt, &i) != 3)
    {
        fail("Index of 7 should be 3!");
    }

    if (*(int *)LinkedListGet(l) != i)
    {
        fail("Last element should be 7!");
    }

    i = 9;
    if (LinkedListIndexOf(l, &EqualInt, &i) != -1)
    {
        fail("Index of 9 should be -1!");
    }

    LinkedListRemove(l);
    EnsureSize(l, 3);
    EnsureContents(l, "5 3 2 ");

    LinkedListRemoveX(l, 0);
    EnsureSize(l, 2);
    EnsureContents(l, "3 2 ");

    LinkedListAddX(l, IntCopy(1), 0);
    EnsureSize(l, 3);
    EnsureContents(l, "1 3 2 ");

    LinkedListRemoveX(l, 1);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");

    l2 = LinkedListCopy(l);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");
    EnsureSize(l2, 2);
    EnsureContents(l2, "1 2 ");

    LinkedListSetX(l2, IntCopy(3), 0);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");
    EnsureSize(l2, 2);
    EnsureContents(l2, "3 2 ");

    LinkedListRemove(l2);
    EnsureSize(l, 2);
    EnsureContents(l, "1 2 ");
    EnsureSize(l2, 1);
    EnsureContents(l2, "3 ");

    l3 = LinkedListConcat(l, l2);
    EnsureSize(l3, 3);
    EnsureContents(l3, "1 2 3 ");

    LinkedListFree(l3);
    LinkedListFree(l2);
    LinkedListFree(l);

    l = LinkedListCreate(-1, &free, &CopyInt);
    LinkedListAdd(l, IntCopy(1));
    LinkedListAdd(l, IntCopy(2));
    LinkedListAdd(l, IntCopy(3));
    LinkedListAdd(l, IntCopy(4));
    EnsureSize(l, 4);
    EnsureContents(l, "1 2 3 4 ");

    l2 = LinkedListGetSubLinkedList(l, 1, 2);
    EnsureSize(l2, 2);
    EnsureContents(l2, "2 3 ");
    LinkedListFree(l2);

    LinkedListClear(l);
    EnsureSize(l, 0);
    EnsureContents(l, "");

    for (i = 0; i < 100; i++)
    {
        LinkedListAdd(l, IntCopy(i));
    }
    EnsureSize(l, 100);

    i = 94;
    l2 = LinkedListFilter(l, &LessInt, &i);
    EnsureSize(l2, 6);
    EnsureContents(l2, "94 95 96 97 98 99 ");
    LinkedListFree(l2);

    LinkedListCursorBeforeHead(l);
    if (!LinkedListCursorSearchNext(l, &EqualInt, &i))
    {
        fail("Couldn't find i!");
    }

    if (LinkedListCursorIndexOf(l) != 94)
    {
        fail("Wrong index!");
    }

    if (LinkedListCursorSearchNext(l, &EqualInt, &i))
    {
        fail("shouldn't find i!");
    }

    LinkedListFree(l);

    printf("COMPLETED TEST!\n");
    return 0;
}
