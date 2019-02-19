#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../chelper/common.h"
#include "../chelper/arraylist.h"
#include "../chelper/string.h"

void fail(String src)
{
    printf("%s\n", src);

    exit(1);
}

void EnsureSize(ArrayList l, size_t expectSize)
{
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
}

int main(void)
{
    ArrayList l;
    int i;
    size_t size;
    String build, temp;

    l = ArrayListCreate(0, &free, &CopyInt);
    size = 0;
    build = StringCopy("");

    EnsureSize(l, 0);
    EnsureContents(l, build);

    for (i = -2000; i < 2001; i++)
    {
        size++;
        temp = StringFormat("%d ", i);
        build = StringAdd(build, temp);
        free(temp);
        printf("Adding %d...\n", i);

        ArrayListAdd(l, IntCopy(i));
        EnsureSize(l, size);
        EnsureContents(l, build);
    }

    for (i = -2000; i < 1997; i++)
    {
        size--;
        temp = StringFormat("%d ", *(int *)ArrayListGet(l));
        build = StringChop(build, strlen(temp));
        free(temp);
        printf("Removing %d...\n", *(int *)ArrayListGet(l));

        ArrayListRemove(l);
        EnsureSize(l, size);
        EnsureContents(l, build);
    }

    while (1)
    {
        for (i = 0; i < 30; i++)
            ArrayListAdd(l, IntCopy(i));
        for (i = 0; i < 30; i++)
            ArrayListRemove(l);
    }

    ArrayListFree(l);

    printf("COMPLETED TEST!\n");
    return 0;
}
