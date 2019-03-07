#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../chelper/common.h"
#include "../chelper/stack.h"

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{

    printf("COMPLETED TESTS\n");

    return 0;
}
