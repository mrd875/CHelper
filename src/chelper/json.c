#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "json.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"
#include "dictionary.h"
#include "arraylist.h"
#include "stringbuilder.h"

struct JSON
{
    /*The data*/
    Dictionary data;
};

struct JSONArray
{
    /*The data*/
    ArrayList data;
};

/*Creates the json value*/
JSONValue *JSONVCreate(JSONType jt, void *data)
{
    JSONValue *a;

    a = malloc(sizeof(struct JSONValue));
    assert(a != NULL);
    a->type = jt;
    a->data = data;

    return a;
}

/*Frees the jsv*/
void JSONVFree(void *a)
{
    JSONValue *jv;

    jv = (JSONValue *)a;
    assert(jv != NULL);

    switch (jv->type)
    {
    case JSON_Array:
        JSONArrayFree(jv->data);
        break;

    case JSON_Object:
        JSONFree(jv->data);
        break;

    case JSON_Null:
        break;

    default:
        free(jv->data);
        break;
    }

    free(jv);
}

/*Copy the jsv*/
void *JSONVCopy(void *a)
{
    JSONValue *jv, *an;

    jv = (JSONValue *)a;

    assert(jv != NULL);

    an = JSONVCreate(jv->type, NULL);

    switch (jv->type)
    {
    case JSON_Array:
        an->data = JSONArrayCopy(jv->data);
        break;

    case JSON_Object:
        an->data = JSONCopy(jv->data);
        break;

    case JSON_Null:
        an->data = NULL;
        break;

    case JSON_Bool:
    case JSON_Int:
        an->data = CopyInt(jv->data);
        break;

    case JSON_String:
        an->data = CopyString(jv->data);
        break;

    case JSON_Float:
        an->data = CopyFloat(jv->data);
        break;

    default:
        break;
    }

    return an;
}

JSONValue JSONValueBool(bool in)
{
    JSONValue jv;

    jv.type = JSON_Bool;
    jv.data = IntCopy(in);

    return jv;
}

JSONValue JSONValueInt(int in)
{
    JSONValue jv;

    jv.type = JSON_Int;
    jv.data = IntCopy(in);

    return jv;
}

JSONValue JSONValueFloat(float in)
{
    JSONValue jv;

    jv.type = JSON_Float;
    jv.data = FloatCopy(in);

    return jv;
}

JSONValue JSONValueNull()
{
    JSONValue jv;

    jv.type = JSON_Null;
    jv.data = NULL;

    return jv;
}

JSONValue JSONValueString(String s)
{
    JSONValue jv;

    jv.type = JSON_String;
    jv.data = StringCopy(s);

    return jv;
}

JSONValue JSONValueObject(JSON j)
{
    JSONValue jv;

    jv.type = JSON_Object;
    jv.data = j;

    return jv;
}

JSONValue JSONValueArray(JSONArray j)
{
    JSONValue jv;

    jv.type = JSON_Array;
    jv.data = j;

    return jv;
}

JSON JSONCreate()
{
    JSON j;

    j = malloc(sizeof(struct JSON));
    assert(j != NULL);

    j->data = DictionaryCreate(&JSONVFree, &JSONVCopy);

    return j;
}

void JSONFree(JSON j)
{
    assert(j != NULL);

    DictionaryFree(j->data);

    free(j);
}

JSON JSONCopy(JSON j)
{
    JSON a;

    assert(j != NULL);

    a = JSONCreate();
    DictionaryFree(a->data);

    a->data = DictionaryCopy(j->data);

    return a;
}

JSONValue JSONGet(JSON j, String k)
{
    JSONValue jv, *a;

    assert(j);
    assert(k);

    a = (JSONValue *)DictionaryGet(j->data, k);

    if (a)
    {
        jv.type = a->type;
        jv.data = a->data;
    }
    else
    {
        jv.data = NULL;
        jv.type = JSON_Null;
    }

    return jv;
}

bool JSONSet(JSON j, String k, JSONValue v)
{
    assert(j);
    assert(k);

    return DictionarySet(j->data, k, JSONVCreate(v.type, v.data));
}

bool JSONHas(JSON j, String k)
{
    assert(j);
    assert(k);

    return DictionaryHas(j->data, k);
}

bool JSONRemove(JSON j, String k)
{
    assert(j);
    assert(k);

    return DictionaryRemove(j->data, k);
}

JSONArray JSONArrayCreate()
{
    JSONArray ja;

    ja = malloc(sizeof(struct JSONArray));
    assert(ja);

    ja->data = ArrayListCreate(0, &JSONVFree, &JSONVCopy);

    return ja;
}

void JSONArrayFree(JSONArray ja)
{
    assert(ja);

    ArrayListFree(ja->data);

    free(ja);
}

JSONArray JSONArrayCopy(JSONArray ja)
{
    JSONArray a;

    assert(ja);

    a = JSONArrayCreate();
    ArrayListFree(a->data);

    a->data = ArrayListCopy(ja->data);

    return a;
}

size_t JSONArrayLength(JSONArray ja)
{
    assert(ja);

    return ArrayListLength(ja->data);
}

JSONValue JSONArrayGet(JSONArray ja, size_t i)
{
    JSONValue jv, *a;
    size_t len;
    assert(ja);

    len = ArrayListLength(ja->data);

    if (i >= len)
    {
        jv.type = JSON_Null;
        jv.data = NULL;
    }
    else
    {
        a = (JSONValue *)ArrayListGetX(ja->data, i);

        jv.data = a->data;
        jv.type = a->type;
    }

    return jv;
}

bool JSONArraySet(JSONArray ja, size_t x, JSONValue jv)
{
    size_t len, i;

    assert(ja);

    len = ArrayListLength(ja->data);

    for (i = len; i <= x; i++)
    {
        ArrayListAdd(ja->data, JSONVCreate(JSON_Null, NULL));
    }

    return ArrayListSetX(ja->data, JSONVCreate(jv.type, jv.data), x);
}

bool JSONArrayRemove(JSONArray ja, size_t i)
{
    assert(ja);

    return ArrayListRemoveX(ja->data, i);
}

/*Adds the value to sb*/
void JSONValueAddToStringBuilder(StringBuilder sb, JSONValue jv);

/*Strings the jsonarray*/
String JSONArrayToString(JSONArray ja)
{
    String a;
    StringBuilder sb;
    size_t len, i;
    JSONValue jv;

    sb = StringBuilderCreate();
    StringBuilderAdd(sb, "[");

    len = ArrayListLength(ja->data);

    for (i = 0; i < len; i++)
    {
        jv = JSONArrayGet(ja, i);

        JSONValueAddToStringBuilder(sb, jv);

        if (i < len - 1)
            StringBuilderAdd(sb, ",");
    }

    StringBuilderAdd(sb, "]");

    a = StringBuilderToString(sb);
    StringBuilderFree(sb);

    return a;
}

/*Stringifies the json object*/
String JSONToString(JSON j)
{
    String a, temp;
    StringBuilder sb;
    size_t len, i;
    JSONValue jv;
    ArrayList keys;

    sb = StringBuilderCreate();
    StringBuilderAdd(sb, "{");

    keys = DictionaryKeys(j->data);
    len = ArrayListLength(keys);

    for (i = 0; i < len; i++)
    {
        temp = (String)ArrayListGetX(keys, i);

        jv = JSONGet(j, temp);

        StringBuilderAddFormatted(sb, "\"%s\":", temp);

        JSONValueAddToStringBuilder(sb, jv);

        if (i < len - 1)
            StringBuilderAdd(sb, ",");
    }

    StringBuilderAdd(sb, "}");

    a = StringBuilderToString(sb);
    StringBuilderFree(sb);
    ArrayListFree(keys);

    return a;
}

String JSONStringify(JSONValue jv)
{
    switch (jv.type)
    {
    case JSON_Array:
        return JSONArrayToString(jv.data);

    case JSON_Object:
        return JSONToString(jv.data);

    default:
        return NULL;
    }
}

void JSONValueAddToStringBuilder(StringBuilder sb, JSONValue jv)
{
    String temp;

    switch (jv.type)
    {
    case JSON_Null:
        StringBuilderAdd(sb, "null");
        break;

    case JSON_Bool:
        if (*(int *)jv.data)
            StringBuilderAdd(sb, "true");
        else
            StringBuilderAdd(sb, "false");
        break;

    case JSON_String:
        StringBuilderAddFormatted(sb, "\"%s\"", jv.data);
        break;

    case JSON_Int:
        StringBuilderAddFormatted(sb, "%d", *(int *)jv.data);
        break;

    case JSON_Float:
        StringBuilderAddFormatted(sb, "%f", *(float *)jv.data);
        break;

    case JSON_Array:
        temp = JSONArrayToString(jv.data);
        StringBuilderAdd(sb, temp);
        free(temp);
        break;

    case JSON_Object:
        temp = JSONToString(jv.data);
        StringBuilderAdd(sb, temp);
        free(temp);
        break;

    default:
        break;
    }
}

/*Parses the string into a JSON object, returns null if cannot be done.*/
JSON JSONFromString(String str);

/*Creates a json array from a string, null if failed*/
JSONArray JSONArrayFromString(String str);

/*Parses the string into a JSON object, returns null if cannot be done.*/
JSONValue JSONParse(String str);
