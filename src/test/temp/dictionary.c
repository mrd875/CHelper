#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../chelper/common.h"
#include "../chelper/dictionary.h"
#include "../chelper/string.h"

int main(void)
{
    Dictionary d;

    d = DictionaryCreate();

    printf("%d", DictionaryHas(d, "a"));

    DictionarySet(d, "a", IntCopy(1), DataArgsInt());
    printf("%d", DictionaryHas(d, "a"));
    DictionarySet(d, "a", IntCopy(1), DataArgsInt());
    printf("%d", DictionaryHas(d, "a"));
    DictionaryFree(d);

    return 0;
}
