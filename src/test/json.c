#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <chelper/common.h>
#include <chelper/json.h>
#include <chelper/string.h>
#include <chelper/arraylist.h>

void fail(String src)
{
    printf("%s\n", src);

    assert(false);
}

int main(void)
{
    JSON j, j2;
    JSONArray ja;
    JSONValue jv;
    ArrayList keys;
    String str;

    j = JSONCreate();
    ja = JSONArrayCreate();
    j2 = JSONCreate();

    JSONSet(j, "hello", JSONValueBool(true));
    JSONSet(j, "x\"d", JSONValueBool(false));
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

    jv = JSONParse("  {  \"\\\"escape string\": \"\nnew line\",  \"test_array\": [], \"test_array2\": [  1 , 4,  6, 7 ],   \"test_neg_int\"  : \t  -42  \n  ,   \"test_neg_float\":-39.24,\"bookstore\": {\"book\": [{\"category\": \"cooking\",\"title\": {\"lang\": \"en\",\"text\": \"Everyday Italian\"},\"author\": \"Giada De Laurentiis\",\"year\": 2005,\"price\": 30.00},{\"category\": \"children\",\"title\": {\"lang\": \"en\",\"text\": \"Harry Potter\"},\"author\": \"J K. Rowling\",\"year\": 2005,\"price\": 29.99}]}  \r  , \"test-obj\":  {  }    }    ");

    if (jv.type != JSON_Null)
    {
        j = jv.data;

        str = JSONStringify(JSONValueObject(j));
        printf("\n\n%s\n", str);
        free(str);

        keys = JSONKeys(j);
        ArrayListPrintString(keys);
        ArrayListFree(keys);

        JSONFree(j);
    }
    else
    {
        fail("BAD JSON PARSE");
    }

    jv = JSONParse("  {  \"\\\"escape string\": \"\nnew line\",  \"test_array\": [], \"test_array2\": [  1 , 4,  6, 7 ],   \"test_neg_int\"  : \t  -42  \n  ,   \"test_neg_float\":-39.24,\"bookstore\": {\"book\": [{\"category\": \"cooking\",\"title\": {\"lang\": \"en\",\"text\": \"Everyday Italian\"},\"author\": \"Giada De Laurentiis\",\"year\": 2005,\"price\": 30.00},{\"category\": \"children\",\"title\": {\"lang\": \"en\",\"text\": \"Harry Potter\"},\"author\": \"J K. Rowling\",\"year\": 2005,\"price\": 29.99}]}  \r      }    []");

    if (jv.type != JSON_Null)
        fail("SHOULD FAIL!");

    jv = JSONParse("  {  \"\\\"escape string\": \"\nnew line\",  \"test_array\": [], \"test_array2\": [  1 , 4,  6, 7 ],   \"test_neg_int\"  : \t  -42  \n  ,   \"test_neg_float\":-39.24,\"bookstore\": {\"book\": [{\"category\": \"cooking\"\"title\": {\"lang\": \"en\",\"text\": \"Everyday Italian\"},\"author\": \"Giada De Laurentiis\",\"year\": 2005,\"price\": 30.00},{\"category\": \"children\",\"title\": {\"lang\": \"en\",\"text\": \"Harry Potter\"},\"author\": \"J K. Rowling\",\"year\": 2005,\"price\": 29.99}]}  \r      }    ");

    if (jv.type != JSON_Null)
        fail("SHOULD FAIL!");

    printf("COMPLETED TESTS\n");

    return 0;
}
