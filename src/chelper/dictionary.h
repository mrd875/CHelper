#ifndef __CHELPERDICT__
#define __CHELPERDICT__

#include <stdbool.h>
#include "common.h"

/*
    Creates a dict, make sure to free after.
*/
Dictionary DictionaryCreate();

/*
    Frees a dict.
*/
void DictionaryFree(Dictionary d);

/*
    Clears a dict.
*/
void DictionaryClear(Dictionary d);

/*
    Copies the dictionary
*/
Dictionary DictionaryCopy(Dictionary d);

/*
    Data args dictionary
*/
DataArgs DataArgsDictionary();

/*
    Returns how many keys are in the dictionary.
*/
int DictionarySize(Dictionary d);

/*
    Returns if the dict has the key.
*/
bool DictionaryHas(Dictionary d, String key);

/*
    Returns the data address at the key, NULL if not found.
*/
void *DictionaryGet(Dictionary d, String key);

/*
    Removes the key from the dict. Returns if successful.
*/
bool DictionaryDelete(Dictionary d, String key);

/*
    Sets the key to be the data.
*/
bool DictionarySet(Dictionary d, String key, void *data, DataArgs dataArgs);

/*
    Returns a list of the dict's keys.
*/
List DictionaryKeys(Dictionary d);

#endif /* __CHELPERDICT__ */
