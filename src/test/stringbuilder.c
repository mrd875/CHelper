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

    printf("COMPLETED TESTS\n");

    return 0;
}
