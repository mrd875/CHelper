#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../chelper/common.h"
#include "../chelper/string.h"
#include "../chelper/stringbuilder.h"

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    StringBuilder sb;
    int i;

    sb = StringBuilderCreate();

    StringBuilderAdd(sb, "hi ");
    StringBuilderAdd(sb, "there.");

    StringBuilderPrint(sb);

    StringBuilderClear(sb);

    StringBuilderAdd(sb, "hi ");
    StringBuilderAdd(sb, "there ");
    StringBuilderAdd(sb, "again.");

    StringBuilderPrint(sb);

    StringBuilderClear(sb);

    for (i = 0; i < 10000; i++)
    {
        StringBuilderAddFormatted(sb, "%d ", i);
    }

    StringBuilderPrint(sb);

    StringBuilderFree(sb);

    printf("COMPLETED TESTS\n");

    return 0;
}
