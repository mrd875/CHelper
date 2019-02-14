#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../chelper/common.h"
#include "../chelper/linkedlist.h"
#include "../chelper/string.h"

int main(void)
{
    LinkedList l;
    String s;

    l = StringSplitIntoLinkedList("hi ther lol x f d da 1", " ");

    LinkedListDebugPrintString(l);

    s = StringCopy("");
    for (int i = 0; i < 134523; i++)
        s = StringAdd(s, "hello");

    printf("%s\n", s);

    free(s);
    LinkedListFree(l);

    return 0;
}
