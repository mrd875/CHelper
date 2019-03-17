#ifndef __CHELPERJSON__
#define __CHELPERJSON__

#include <stdbool.h>
#include <stdlib.h>

/* This is the JSON. */
typedef struct JSON *JSON;

/*THis is an array of json values*/
typedef struct JSONArray *JSONArray;

enum JSONType
{
    JSON_Bool,
    JSON_String,
    JSON_Int,
    JSON_Float,
    JSON_Null,
    JSON_Object,
    JSON_Array
};

/*Every type of a json value*/
typedef enum JSONType JSONType;

/*The value at a key in the json object*/
typedef struct JSONValue JSONValue;

struct JSONValue
{
    JSONType type;
    void *data;
};

#include "common.h"
#include "arraylist.h"
#include "string.h"

/*Returns a jsonvalue object*/
JSONValue JSONValueBool(bool in);

/*Returns a jsonvalue object*/
JSONValue JSONValueInt(int in);

/*Returns a jsonvalue object*/
JSONValue JSONValueFloat(float in);

/*Returns a jsonvalue object*/
JSONValue JSONValueNull();

/*Returns a jsonvalue object*/
JSONValue JSONValueString(String in);

/*Returns a jsonvalue object*/
JSONValue JSONValueObject(JSON in);

/*Returns a jsonvalue object*/
JSONValue JSONValueArray(JSONArray in);

/*Parses the string into a JSON object, returns null if cannot be done.*/
JSONValue JSONParse(String str);

/*Stringifies the json object*/
String JSONStringify(JSONValue jv);

/*Creates a new empty json object*/
JSON JSONCreate();

/*Frees the json object*/
void JSONFree(JSON j);

/*Copy json object*/
JSON JSONCopy(JSON j);

/*Gets the json value at the key in the json object*/
JSONValue JSONGet(JSON j, String k);

/*Sets the key in the json*/
bool JSONSet(JSON j, String k, JSONValue v);

/*Does the json object have the key*/
bool JSONHas(JSON j, String k);

/*Removes the key from the json object*/
bool JSONRemove(JSON j, String k);

/*Gets all the keys*/
ArrayList JSONKeys(JSON j);

/*Creates an empty json array*/
JSONArray JSONArrayCreate();

/*Frees the json array*/
void JSONArrayFree(JSONArray ja);

/*Copies the json array*/
JSONArray JSONArrayCopy(JSONArray ja);

/*How many elements in the array*/
size_t JSONArrayLength(JSONArray ja);

/*Gets the value at the index*/
JSONValue JSONArrayGet(JSONArray ja, size_t i);

/*Sets the value at the index, fills inbetween with null if needed.*/
bool JSONArraySet(JSONArray ja, size_t i, JSONValue jv);

/*Removes the value at i */
bool JSONArrayRemove(JSONArray ja, size_t i);

#endif /* __CHELPERJSON__ */
