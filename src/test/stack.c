#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <chelper/common.h>
#include <chelper/stack.h>
#include <chelper/string.h>

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    Stack s;

    s = StackCreate(&free, &CopyInt);

    printf("adding 1 .. 6\n");
    StackAdd(s, IntCopy(1));
    StackAdd(s, IntCopy(2));
    StackAdd(s, IntCopy(3));
    StackAdd(s, IntCopy(4));
    StackAdd(s, IntCopy(5));
    StackAdd(s, IntCopy(6));

    while (StackLength(s))
    {
        printf("%d ", *(int *)StackGet(s));

        StackRemove(s);
    }
    printf("\n");

    StackFree(s);

    printf("COMPLETED TESTS\n");

    return 0;
}
