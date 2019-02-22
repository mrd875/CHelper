#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

/*The key value pair*/
typedef struct DictionaryKVP *DictionaryKVP;

struct Dictionary
{
    /*The function to copy values*/
    copy_fn_t copy_fn;
    /*The function to free values*/
    free_fn_t free_fn;
    /*The array of lists to store the key values.*/
    ArrayList *data;
    /*The current amount of lists in the array.*/
    size_t size;
    /* The amount of keys in the dictionary*/
    size_t length;
};

struct DictionaryKVP
{
    /*Key*/
    String key;
    /*Value*/
    void *data;
    /*Owner*/
    Dictionary owner;
};

/*The minimum size of the dictionary for the array of lists.*/
#define DictionaryMinSize 32
/*The maximum load factor before expanding the array.*/
#define DictionaryMaxLoadFactor 1.5

/*Creates the KVP*/
DictionaryKVP DictionaryKVPCreate(Dictionary d, String key, void *data)
{
    DictionaryKVP kv;

    assert(key != NULL);

    kv = malloc(sizeof(struct DictionaryKVP));
    assert(kv != NULL);

    kv->key = StringCopy(key);
    kv->data = data;
    kv->owner = d;

    return kv;
}

/*Copies a kvp*/
void *DictionaryKVPCopy(void *a)
{
    DictionaryKVP kv, kvo;
    void *data;

    assert(a != NULL);

    kvo = (DictionaryKVP *)a;

    data = NULL;
    if (kvo->owner->copy_fn != NULL)
        data = kvo->owner->copy_fn(kvo->data);

    kv = DictionaryKVPCreate(kvo->owner, kvo->key, data);

    return kv;
}

/*Frees a kvp*/
void DictionaryKVPFree(void *a)
{
    DictionaryKVP kv;

    assert(a != NULL);

    kv = (DictionaryKVP)a;

    if (kv->owner->free_fn != NULL)
        kv->owner->free_fn(kv->data);

    free(kv->key);
    free(kv);
}

/*Compares the kv to the key.*/
bool DictionaryKVPKeyEqual(void *a, void *b)
{
    DictionaryKVP kv;
    String key;

    assert(a != NULL);
    assert(b != NULL);

    kv = (DictionaryKVP)a;
    key = (String)b;

    if (strcmp(kv->key, key))
        return false;

    return true;
}

Dictionary DictionaryCreate(free_fn_t free_fn, copy_fn_t copy_fn)
{
    Dictionary d;
    ArrayList *data;

    d = malloc(sizeof(struct Dictionary));
    assert(d != NULL);

    d->free_fn = free_fn;
    d->copy_fn = copy_fn;
    d->length = 0;

    data = calloc(sizeof(ArrayList *), DictionaryMinSize);
    assert(data != NULL);

    d->data = data;
    d->size = DictionaryMinSize;

    return d;
}

void DictionaryFree(Dictionary d)
{
    assert(d != NULL);

    DictionaryClear(d);

    free(d->data);
    free(d);
}

void DictionaryClear(Dictionary d)
{
    size_t i;

    assert(d != NULL);

    for (i = 0; i < d->size; i++)
    {
        if (d->data[i] == NULL)
            continue;

        ArrayListFree(d->data[i]);
        d->data[i] = NULL;
    }
}

Dictionary DictionaryCopy(Dictionary d)
{
    Dictionary a;
    size_t i;
    ArrayList *data;

    assert(d != NULL);

    a = DictionaryCreate(d->free_fn, d->copy_fn);
    free(a->data);

    data = calloc(sizeof(ArrayList *), d->size);
    assert(data != NULL);

    for (i = 0; i < d->size; i++)
    {
        if (d->data[i] == NULL)
            continue;

        data[i] = ArrayListCopy(d->data[i]);
    }

    a->data = data;
    a->size = d->size;

    return a;
}

size_t DictionaryLength(Dictionary d)
{
    assert(d != NULL);

    return d->length;
}

size_t _DictionaryGetArrayPos(Dictionary d, String key)
{
    assert(d != NULL);
    assert(key != NULL);

    return StringHash(key) % d->size;
}

bool DictionaryHas(Dictionary d, String key)
{
    ArrayList l;

    assert(d != NULL);
    assert(key != NULL);

    l = d->data[_DictionaryGetArrayPos(d, key)];

    if (l == NULL)
        return false;

    if (ArrayListContains(l, &DictionaryKVPKeyEqual, key) == false)
        return false;

    return true;
}

void *DictionaryGet(Dictionary d, String key)
{
    ArrayList l;

    assert(d != NULL);
    assert(key != NULL);

    l = d->data[_DictionaryGetArrayPos(d, key)];

    if (l == NULL)
        return NULL;

    ArrayListCursorBeforeHead(l);
    if (ArrayListCursorSearchNext(l, &DictionaryKVPKeyEqual, key) == false)
        return NULL;

    return ArrayListCursorGet(l);
}

bool DictionaryRemove(Dictionary d, String key)
{
    ArrayList l;

    assert(d != NULL);
    assert(key != NULL);

    l = d->data[_DictionaryGetArrayPos(d, key)];

    if (l == NULL)
        return false;

    ArrayListCursorBeforeHead(l);
    if (ArrayListCursorSearchNext(l, &DictionaryKVPKeyEqual, key) == false)
        return false;

    return ArrayListCursorRemove(l);
}

/*
    Sets the key to be the data.
*/
bool DictionarySet(Dictionary d, String key, void *data);

/*
    Returns a list of the dict's keys.
*/
ArrayList DictionaryKeys(Dictionary d);
