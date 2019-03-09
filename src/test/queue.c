#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <chelper/common.h>
#include <chelper/queue.h>

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    Queue s;

    s = QueueCreate(&free, &CopyInt);

    printf("adding 1 .. 6\n");
    QueueAdd(s, IntCopy(1));
    QueueAdd(s, IntCopy(2));
    QueueAdd(s, IntCopy(3));
    QueueAdd(s, IntCopy(4));
    QueueAdd(s, IntCopy(5));
    QueueAdd(s, IntCopy(6));

    while (QueueLength(s))
    {
        printf("%d ", *(int *)QueueGet(s));

        QueueRemove(s);
    }
    printf("\n");

    QueueFree(s);

    printf("COMPLETED TESTS\n");

    return 0;
}
