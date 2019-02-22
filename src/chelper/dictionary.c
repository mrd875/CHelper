#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "internal/arraylist.h"
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
/*The minimuum load factor before shrinking the array.*/
#define DictionaryMinLoadFactor 1.5

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

    kvo = (DictionaryKVP)a;

    assert(kvo->owner != NULL);

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

    assert(kv->owner != NULL);

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

    if (strcmp(kv->key, key) != 0)
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

void FreeDictionary(void *d)
{
    assert(d != NULL);

    DictionaryFree((Dictionary)d);
}

void *CopyDictionary(void *d)
{
    assert(d != NULL);

    return DictionaryCopy((Dictionary)d);
}

void DictionaryClear(Dictionary d)
{
    size_t i, size;
    ArrayList l;

    assert(d != NULL);

    size = d->size;

    for (i = 0; i < size; i++)
    {
        l = d->data[i];

        if (l == NULL)
            continue;

        ArrayListFree(l);

        d->data[i] = NULL;
    }
}

Dictionary DictionaryCopy(Dictionary d)
{
    Dictionary a;
    size_t i, size;
    ArrayList *data, l;

    assert(d != NULL);

    size = d->size;

    a = DictionaryCreate(d->free_fn, d->copy_fn);
    free(a->data);

    data = calloc(sizeof(ArrayList *), size);
    assert(data != NULL);

    for (i = 0; i < size; i++)
    {
        l = d->data[i];

        if (l == NULL)
            continue;

        data[i] = ArrayListCopy(l);
    }

    a->data = data;
    a->size = size;
    a->length = d->length;

    return a;
}

size_t DictionaryLength(Dictionary d)
{
    assert(d != NULL);

    return d->length;
}

/*Returns a hashed array pos for the key*/
size_t _DictionaryGetArrayPos(Dictionary d, String key)
{
    assert(d != NULL);
    assert(key != NULL);

    return StringHash(key) % d->size;
}

/*Returns an arraylist for the KVPs.*/
ArrayList _DictionaryGetArrayList()
{
    return ArrayListCreate(-1, &DictionaryKVPFree, &DictionaryKVPCopy);
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

/*Resizes the dictionary.*/
void _DictionaryResize(Dictionary d, size_t newSize)
{
    ArrayList *data, *oldData, l, l2;
    size_t i, oldSize, arrPos;
    DictionaryKVP kv;

    assert(d != NULL);
    assert(newSize > 0);

    data = calloc(sizeof(ArrayList *), newSize);
    assert(data != NULL);

    oldData = d->data;
    oldSize = d->size;
    d->data = data;
    d->size = newSize;

    for (i = 0; i < oldSize; i++)
    {
        l = oldData[i];

        if (l == NULL)
            continue;

        ArrayListClearFree(l);

        ArrayListCursorBeforeHead(l);
        while (ArrayListCursorHasNext(l) == true)
        {
            ArrayListCursorNext(l);

            kv = ArrayListCursorGet(l);

            arrPos = _DictionaryGetArrayPos(d, kv->key);

            l2 = d->data[arrPos];

            if (l2 == NULL)
            {
                d->data[arrPos] = _DictionaryGetArrayList();
                l2 = d->data[arrPos];
            }

            ArrayListAdd(l2, kv);
        }

        ArrayListFree(l);
    }
}

/*Returns the load factor of the dictionary*/
double _DictionaryGetLoadFactor(Dictionary d)
{
    assert(d != NULL);

    return (double)d->length / (double)d->size;
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

    if (ArrayListCursorRemove(l) != true)
        return false;

    d->length--;

    if (d->size > DictionaryMinSize && _DictionaryGetLoadFactor(d) < DictionaryMinLoadFactor)
    {
        _DictionaryResize(d, d->size / 2);
    }

    return true;
}

bool DictionarySet(Dictionary d, String key, void *data)
{
    ArrayList l;
    size_t arrPos;

    assert(d != NULL);
    assert(key != NULL);

    arrPos = _DictionaryGetArrayPos(d, key);

    l = d->data[arrPos];

    if (l == NULL)
    {
        d->data[arrPos] = _DictionaryGetArrayList();
        l = d->data[arrPos];
    }

    ArrayListCursorBeforeHead(l);
    if (ArrayListCursorSearchNext(l, &DictionaryKVPKeyEqual, key) == true)
    {
        if (ArrayListCursorRemove(l) == false)
            return false;

        d->length--;
    }

    if (ArrayListAdd(l, DictionaryKVPCreate(d, key, data)) != true)
        return false;

    d->length++;

    if (_DictionaryGetLoadFactor(d) > DictionaryMaxLoadFactor)
    {
        _DictionaryResize(d, d->size * 2);
    }

    return true;
}

ArrayList DictionaryKeys(Dictionary d);
