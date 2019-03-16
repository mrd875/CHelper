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

JSONValue *JSONVCreate(JSONType jt, void *data)
{
    JSONValue *a;

    a = malloc(sizeof(struct JSONValue));
    assert(a != NULL);
    a->type = jt;
    a->data = data;

    return a;
}

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

/*Parses the string into a JSON object, returns null if cannot be done.*/
JSON JSONFromString(String str);

/*Stringifies the json object*/
String JSONToString(JSON j);

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

/*Sets the key in the json*/
bool JSONSet(JSON j, String k, JSONValue v);

/*Does the json object have the key*/
bool JSONHas(JSON j, String k);

/*Removes the key from the json object*/
bool JSONRemove(JSON j, String k);

/*Creates an empty json array*/
JSONArray JSONArrayCreate();

/*Creates a json array from a string, null if failed*/
JSONArray JSONArrayFromString(String str);

/*Frees the json array*/
void JSONArrayFree(JSONArray ja);

/*Copies the json array*/
JSONArray JSONArrayCopy(JSONArray ja);

/*Strings the jsonarray*/
String JSONArrayToString(JSONArray ja);

/*How many elements in the array*/
size_t JSONArrayLength(JSONArray ja);

/*Gets the value at the index*/
JSONValue JSONArrayGet(JSONArray ja, size_t i);

/*Sets the value at the index, fills inbetween with null if needed.*/
bool JSONArraySet(JSONArray ja, size_t i, JSONValue jv);

/*Removes the value at i */
bool JSONArrayRemove(JSONArray ja, size_t i);
