#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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
        StringBuilderAddFormatted(sb, "%g", *(float *)jv.data);
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

/*white space*/
bool JSONWhiteSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/*Reads a string*/
String JSONValueReadString(String str, size_t *readChars)
{
    String a;
    size_t og;

    if (str[*readChars] != '"')
        return NULL;
    *readChars += 1;

    og = *readChars;

    while (str[*readChars] != '\0' && str[*readChars] != '"')
        *readChars += 1;

    if (str[*readChars] != '"')
        return NULL;
    *readChars += 1;

    a = calloc(sizeof(char), *readChars - og);
    assert(a);

    strncpy(a, &str[og], *readChars - og - 1);

    return a;
}

/*Reads a false value, returns null if failed*/
int *JSONValueReadFalse(String str, size_t *readChars)
{
    if (str[*readChars] != 'f')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 'a')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 'l')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 's')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 'e')
        return NULL;
    *readChars += 1;

    return IntCopy(false);
}

/*Reads a true value, returns null if failed*/
int *JSONValueReadTrue(String str, size_t *readChars)
{
    if (str[*readChars] != 't')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 'r')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 'u')
        return NULL;
    *readChars += 1;

    if (str[*readChars] != 'e')
        return NULL;
    *readChars += 1;

    return IntCopy(true);
}

/*Reads a null value, returns false if failed*/
bool JSONValueReadNull(String str, size_t *readChars)
{
    if (str[*readChars] != 'n')
        return false;
    *readChars += 1;

    if (str[*readChars] != 'u')
        return false;
    *readChars += 1;

    if (str[*readChars] != 'l')
        return false;
    *readChars += 1;

    if (str[*readChars] != 'l')
        return false;
    *readChars += 1;

    return true;
}

/*Reads white space, returns false if at end of string*/
bool JSONValueReadWhiteSpace(String str, size_t *readChars)
{
    while (JSONWhiteSpace(str[*readChars]))
        *readChars += 1;

    return str[*readChars] != '\0';
}

/*Returns the read value. null if failed*/
JSONValue *JSONValueReadValue(String str, size_t *readChars);

/*Parses the string into a JSON object, returns null if cannot be done.*/
JSON JSONFromString(String str, size_t *readChars)
{
    JSONValue *jv;
    JSON j;
    String key;

    if (str[*readChars] != '{')
        return NULL;
    *readChars += 1;

    j = JSONCreate();

    JSONValueReadWhiteSpace(str, readChars);

    if (str[*readChars] == '}')
    {
        *readChars += 1;
        return j;
    }

    for (;;)
    {
        JSONValueReadWhiteSpace(str, readChars);

        key = JSONValueReadString(str, readChars);
        if (!key)
        {
            JSONFree(j);
            free(key);
            return NULL;
        }

        JSONValueReadWhiteSpace(str, readChars);

        if (str[*readChars] != ':')
        {
            JSONFree(j);
            free(key);
            return NULL;
        }
        *readChars += 1;

        JSONValueReadWhiteSpace(str, readChars);

        jv = JSONValueReadValue(str, readChars);

        if (!jv)
        {
            JSONFree(j);
            free(key);
            return NULL;
        }

        DictionarySet(j->data, key, jv);

        free(key);

        JSONValueReadWhiteSpace(str, readChars);

        if (str[*readChars] == ',')
        {
            *readChars += 1;
            continue;
        }

        break;
    }

    if (str[*readChars] == '}')
    {
        *readChars += 1;
        return j;
    }

    JSONFree(j);
    return NULL;
}

/*Parses the string into a json array. null if error*/
JSONArray JSONArrayFromString(String str, size_t *readChars)
{
    JSONArray ja;
    JSONValue *jv;

    if (str[*readChars] != '[')
        return NULL;
    *readChars += 1;

    ja = JSONArrayCreate();

    JSONValueReadWhiteSpace(str, readChars);

    if (str[*readChars] == ']')
    {
        *readChars += 1;
        return ja;
    }

    for (;;)
    {
        JSONValueReadWhiteSpace(str, readChars);

        jv = JSONValueReadValue(str, readChars);

        if (!jv)
        {
            JSONArrayFree(ja);
            return NULL;
        }

        ArrayListAdd(ja->data, jv);

        JSONValueReadWhiteSpace(str, readChars);

        if (str[*readChars] == ',')
        {
            *readChars += 1;
            continue;
        }

        break;
    }

    if (str[*readChars] == ']')
    {
        *readChars += 1;
        return ja;
    }

    JSONArrayFree(ja);
    return NULL;
}

JSONValue *JSONValueReadValue(String str, size_t *readChars)
{
    JSONValue *jv;
    char c;
    void *data;

    jv = NULL;
    c = str[*readChars];

    switch (c)
    {
    case '[':
        if ((data = JSONArrayFromString(str, readChars)))
            jv = JSONVCreate(JSON_Array, data);
        break;

    case '{':
        if ((data = JSONFromString(str, readChars)))
            jv = JSONVCreate(JSON_Object, data);
        break;

    case 't':
        if ((data = JSONValueReadTrue(str, readChars)))
            jv = JSONVCreate(JSON_Bool, data);
        break;

    case 'f':
        if ((data = JSONValueReadFalse(str, readChars)))
            jv = JSONVCreate(JSON_Bool, data);
        break;

    case 'n':
        if (JSONValueReadNull(str, readChars))
            jv = JSONVCreate(JSON_Null, NULL);
        break;

    case '"':
        if ((data = JSONValueReadString(str, readChars)))
            jv = JSONVCreate(JSON_String, data);
        break;

    default:
        break;
    }

    return jv;
}

JSONValue JSONParse(String str)
{
    JSONValue jv;
    void *obj;
    size_t readChars;

    jv.data = NULL;
    jv.type = JSON_Null;

    readChars = 0;
    JSONValueReadWhiteSpace(str, &readChars);

    if (str[readChars] == '{')
    {
        obj = JSONFromString(str, &readChars);

        if (obj)
        {
            if (JSONValueReadWhiteSpace(str, &readChars))
            {
                JSONFree(obj);
            }
            else
            {
                jv.data = obj;
                jv.type = JSON_Object;
            }
        }
    }
    else if (str[readChars] == '[')
    {
        obj = JSONArrayFromString(str, &readChars);

        if (obj)
        {
            if (JSONValueReadWhiteSpace(str, &readChars))
            {
                JSONArrayFree(obj);
            }
            else
            {
                jv.data = obj;
                jv.type = JSON_Array;
            }
        }
    }

    return jv;
}
