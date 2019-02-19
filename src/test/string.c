#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../chelper/common.h"
#include "../chelper/arraylist.h"
#include "../chelper/string.h"

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    ArrayList l;
    String s, s2, s3;

    if (!StringBeginsWith("hello there guys", "hello "))
    {
        fail("Begins with failed.");
    }

    if (StringBeginsWith("hello there guys", "hellto "))
    {
        fail("Begins with failed.");
    }

    if (!StringEndsWith("hello there guys", "guys"))
    {
        fail("Ends with failed.");
    }

    if (StringEndsWith("hello there guys", "gbuys"))
    {
        fail("Ends with failed.");
    }

    l = StringSplitIntoArrayList("hello there guys", " ");
    if (ArrayListLength(l) != 3)
    {
        fail("Split len failed");
    }
    if (strcmp("hello", ArrayListGetX(l, 0)))
    {
        fail("Split get failed.");
    }
    if (strcmp("there", ArrayListGetX(l, 1)))
    {
        fail("Split get failed.");
    }
    if (strcmp("guys", ArrayListGetX(l, 2)))
    {
        fail("Split get failed.");
    }
    ArrayListFree(l);

    s = StringReplace("yo,.,.it,.is,,me,,..", ",.", " ");
    if (strcmp("yo  it is,,me, .", s))
    {
        fail("StringReplace failed.");
    }
    free(s);

    s = StringFormat("%d %d ", 1, 5);
    if (strcmp("1 5 ", s))
    {
        fail("StringFormat failed.");
    }
    free(s);

    s = StringCopy("[");
    s = StringAdd(s, "9 ");
    s = StringAdd(s, "4 ");
    s = StringAdd(s, "6 ");
    s = StringAdd(s, "1 ");
    s = StringAdd(s, "8 ");
    s = StringAdd(s, "9 ");
    s = StringAdd(s, "]");
    if (strcmp("[9 4 6 1 8 9 ]", s))
    {
        fail("StringAdd failed.");
    }
    s = StringChop(s, 4);
    if (strcmp("[9 4 6 1 8", s))
    {
        fail("StringChop failed.");
    }
    free(s);

    s = StringCopy("abcdefghijklmnopqrstuvwxyz");
    s2 = StringCopy("0123456789");
    s3 = StringConcat(s, s2);

    if (strcmp("abcdefghijklmnopqrstuvwxyz0123456789", s3))
    {
        fail("StringConcat failed.");
    }
    free(s3);
    free(s2);

    s2 = StringGetSubString(s, 10, 10);
    if (strcmp("klmnopqrst", s2))
    {
        fail("StringGetSubString failed.");
    }
    free(s2);

    if (StringIndexOf(s, "pqrs") != 15)
    {
        fail("StringIndexOf failed");
    }

    if (StringIndexOf(s, "cba") != -1)
    {
        fail("StringIndexOf failed");
    }

    free(s);

    printf("COMPLETED TESTS\n");

    return 0;
}