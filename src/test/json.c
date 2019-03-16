#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <chelper/common.h>
#include <chelper/json.h>
#include <chelper/string.h>

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    JSONArray ja, ja2;
    String json;

    ja = JSONArrayCreate();
    ja2 = JSONArrayCreate();

    JSONArraySet(ja, 2, JSONValueBool(false));
    JSONArraySet(ja, 4, JSONValueBool(false));

    JSONArraySet(ja2, 1, JSONValueBool(false));

    JSONArraySet(ja, 0, JSONValueArray(ja2));

    json = JSONArrayToString(ja);
    printf("%s\n", json);
    free(json);

    JSONArrayFree(ja);

    printf("COMPLETED TESTS\n");

    return 0;
}
