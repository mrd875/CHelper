#ifndef __CHELPERJSON__
#define __CHELPERJSON__

#include <stdbool.h>
#include <stdlib.h>

/* This is the JSON. */
typedef struct JSON *JSON;

/*THis is an array of json values*/
typedef struct JSONArray *JSONArray;

/*Every type of a json value*/
typedef enum JSONType JSONType;

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

/*The value at a key in the json object*/
typedef struct JSONValue JSONValue;

struct JSONValue
{
    JSONType type;
    void *data;
};

#include "common.h"
#include "string.h"

/*Creates a new empty json object*/
JSON JSONCreate();

/*Frees the json object*/
void JSONFree(JSON j);

/*Copy json object*/
JSON JSONCopy(JSON j);

/*Parses the string into a JSON object, returns null if cannot be done.*/
JSON JSONFromString(String str);

/*Stringifies the json object*/
String JSONToString(JSON j);

/*Gets the json value at the key in the json object*/
JSONValue JSONGet(JSON j, String k);

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

#endif /* __CHELPERJSON__ */
