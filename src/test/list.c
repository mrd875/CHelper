#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../chelper/common.h"
#include "../chelper/list.h"
#include "../chelper/string.h"

int CountCursorFromHead(List l)
{
    int i = 0;

    ListCursorBeforeHead(l);
    while (ListCursorHasNext(l))
    {
        ListCursorNext(l);
        i++;
    }

    return i;
}

int CountCursorFromTail(List l)
{
    int i = 0;

    ListCursorAfterTail(l);
    while (ListCursorHasPrevious(l))
    {
        ListCursorPrevious(l);
        i++;
    }

    return i;
}

void AddFirstInt(List l, int i)
{
    int before_size, a, new_size, should_size;
    int *before_head, *new_thing, *cur;
    bool wasFull;

    before_size = ListSize(l);
    before_head = ListGetFirst(l);
    wasFull = ListFull(l);
    new_thing = IntCopy(i);

    int *expect_addrs[before_size + 1];
    int expect_contents[before_size + 1];

    ListCursorBeforeHead(l);
    a = 1;
    while (ListCursorHasNext(l))
    {
        ListCursorNext(l);
        cur = ListCursorGet(l);

        expect_addrs[a] = cur;

        if (cur != NULL)
            expect_contents[a] = *cur;
        else
            expect_contents[a] = 0;

        a++;
    }
    expect_addrs[0] = new_thing;
    expect_contents[0] = i;

    should_size = before_size;
    if (!wasFull)
        should_size++;

    printf("\nAdding %d to first: ", i);
    ListAddFirst(l, new_thing, DataArgsInt());

    new_size = ListSize(l);

    a = CountCursorFromHead(l);
    if (a != new_size)
    {
        printf("BAD!\nCursor count from head (%d) does not equal size of list (%d)!\n", a, new_size);
        exit(1);
    }

    a = CountCursorFromTail(l);
    if (a != new_size)
    {
        printf("BAD!\nCursor count from tail (%d) does not equal size of list (%d)!\n", a, new_size);
        exit(1);
    }

    if (new_size != should_size)
    {
        printf("BAD!\nNew size of the list (%d) does not equal the expected size (%d)!\n", new_size, should_size);
        exit(1);
    }

    if (wasFull)
    {
        if (before_head != ListGetFirst(l))
        {
            printf("BAD!\nBefore head != new head (full)\n");
            exit(1);
        }
    }
    else
    {
        if (before_head == ListGetFirst(l))
        {
            printf("BAD!\nBefore head == new head\n");
            exit(1);
        }
    }

    if (wasFull)
        a = 1;
    else
        a = 0;

    ListCursorBeforeHead(l);
    for (; a < should_size; a++)
    {
        ListCursorNext(l);

        cur = ListCursorGet(l);

        if (cur != expect_addrs[a])
        {
            printf("BAD!\nAddresses are not as expected\n");
            exit(1);
        }

        if (cur == NULL)
            continue;

        if (*cur != expect_contents[a])
        {
            printf("BAD!\nContents are not as expected\n");
            exit(1);
        }
    }

    printf("GOOD!\n");
}

int main(void)
{
    List l, l2;

    l = ListCreate(-1);

    AddFirstInt(l, 1);
    AddFirstInt(l, 2);
    AddFirstInt(l, 3);
    AddFirstInt(l, 4);

    ListFree(l);

    printf("\nCOMPLETED TESTS!\n");

    l = StringSplitIntoList("hi everyone it is me the guy of the awd aw aw daw daw dawfiowe  90 fe0 9u 09 u09 u 09u 09u 09u 09 j", " ");
    ListDebugPrintString(l);

    l2 = ListCopy(l);
    ListChangeX(l2, StringCopy("giawdawdwad089324723948"), DataArgsString(), 5);
    ListDebugPrintString(l2);
    ListDebugPrintString(l);

    return 0;
}
