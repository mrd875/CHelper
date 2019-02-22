#ifndef __CHELPERDICT__
#define __CHELPERDICT__

#include <stdbool.h>
#include <stdlib.h>

/* This is the array list struct. */
typedef struct Dictionary *Dictionary;

#include "common.h"
#include "string.h"
#include "arraylist.h"

/*
    Creates a dict, make sure to free after.
*/
Dictionary DictionaryCreate(free_fn_t free_fn, copy_fn_t copy_fn);

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
    Returns how many keys are in the dictionary.
*/
size_t DictionaryLength(Dictionary d);

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
bool DictionaryRemove(Dictionary d, String key);

/*
    Sets the key to be the data.
*/
bool DictionarySet(Dictionary d, String key, void *data);

/*
    Returns a list of the dict's keys.
*/
ArrayList DictionaryKeys(Dictionary d);

/*Free dict*/
void FreeDictionary(void *d);

/*Copy dict*/
void *CopyDictionary(void *d);

#endif /* __CHELPERDICT__ */
