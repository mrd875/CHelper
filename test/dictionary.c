#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../lib/common.h"
#include "../lib/dictionary.h"
#include "../lib/string.h"

int main(void)
{
    Dictionary d;

    d = DictionaryCreate();

    DictionarySet(d, "a", IntCopy(1), DataArgsInt());

    DictionaryFree(d);

    return 0;
}
