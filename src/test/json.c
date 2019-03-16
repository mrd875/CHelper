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
    JSON j, j2;
    JSONArray ja;
    String str;

    j = JSONCreate();
    ja = JSONArrayCreate();
    j2 = JSONCreate();

    JSONSet(j, "hello", JSONValueBool(true));
    JSONSet(j, "xd", JSONValueBool(false));
    JSONSet(j, "there", JSONValueArray(ja));
    JSONSet(j, "another", JSONValueObject(j2));

    JSONArraySet(ja, 0, JSONValueFloat(-1.5));
    JSONArraySet(ja, 2, JSONValueFloat(1.5));
    JSONArraySet(ja, 3, JSONValueInt(5));
    JSONArraySet(ja, 4, JSONValueInt(-5));

    JSONSet(j2, "ok", JSONValueString("hi"));
    JSONSet(j2, "1", JSONValueString("rvrgs"));
    JSONSet(j2, "nothing", JSONValueNull());

    str = JSONStringify(JSONValueObject(j));
    printf("%s\n", str);
    free(str);

    JSONRemove(j, "another");
    JSONRemove(j, "there");

    str = JSONStringify(JSONValueObject(j));
    printf("%s\n", str);
    free(str);

    JSONFree(j);

    printf("COMPLETED TESTS\n");

    return 0;
}
