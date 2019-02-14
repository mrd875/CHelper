#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"
#include "list.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

/*
    This is a key value pair for the dict.
*/
typedef struct DictionaryKVP *DictionaryKVP;

struct DictionaryKVP
{
    /* The key */
    String key;

    /* The data */
    void *data;

    /* The function to free the data. */
    void (*free_routine)(void *data);

    /* The function to copy the data. */
    void *(*copy_routine)(void *data);

    /* Size of the data */
    int size;
};

struct Dictionary
{
    /* Holds the keys and values. */
    List data;
};

DictionaryKVP DictionaryKVPCreate()
{
    DictionaryKVP kv;

    kv = malloc(sizeof(struct DictionaryKVP));
    assert(kv != NULL);

    kv->data = NULL;
    kv->key = NULL;
    kv->size = 0;
    kv->free_routine = NULL;
    kv->copy_routine = NULL;

    return kv;
}

void DictionaryKVPFreeData(DictionaryKVP kv)
{
    assert(kv != NULL);

    if (kv->free_routine != NULL)
        kv->free_routine(kv->data);
}

void *DictionaryKVPCopyData(DictionaryKVP kv)
{
    assert(kv != NULL);

    if (kv->copy_routine == NULL)
        return NULL;

    return kv->copy_routine(kv->data);
}

void *DictionaryKVPCopy(void *a)
{
    DictionaryKVP kv, o;

    assert(a != NULL);

    o = (DictionaryKVP)a;

    kv = DictionaryKVPCreate();
    kv->size = o->size;
    kv->copy_routine = o->copy_routine;
    kv->free_routine = o->free_routine;
    kv->data = DictionaryKVPCopyData(o);
    kv->key = StringCopy(o->key);

    return kv;
}

void DictionaryKVPFree(void *a)
{
    DictionaryKVP kv;

    assert(a != NULL);

    kv = (DictionaryKVP)a;

    DictionaryKVPFreeData(kv);

    free(kv->key);

    free(kv);
}

DataArgs DataArgsDictionaryKVP()
{
    DataArgs dataArgs;

    dataArgs.copy_routine = &DictionaryKVPCopy;
    dataArgs.free_routine = &DictionaryKVPFree;
    dataArgs.size = sizeof(struct DictionaryKVP);

    return dataArgs;
}

bool DictionaryKVPCompare(void *a, void *b)
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

Dictionary DictionaryCreate()
{
    Dictionary d;

    debugp("Creating a dict");

    d = malloc(sizeof(struct Dictionary));
    assert(d != NULL);

    d->data = ListCreate(-1);

    return d;
}

void DictionaryFree(Dictionary d)
{
    assert(d != NULL);

    ListFree(d->data);

    free(d);
}

void DictionaryClear(Dictionary d)
{
    assert(d != NULL);

    ListClear(d->data);
}

Dictionary DictionaryCopy(Dictionary d)
{
    Dictionary newD;

    assert(d != NULL);

    newD = DictionaryCreate();

    ListFree(newD->data);
    newD->data = ListCopy(d->data);

    return newD;
}

int DictionarySize(Dictionary d)
{
    assert(d != NULL);

    return ListSize(d->data);
}

bool DictionaryHas(Dictionary d, String key)
{
    assert(d != NULL);
    assert(key != NULL);

    if (ListSearch(d->data, &DictionaryKVPCompare, key) == -1)
        return false;

    return true;
}

void *DictionaryGet(Dictionary d, String key)
{
    assert(d != NULL);
    assert(key != NULL);

    ListCursorBeforeHead(d->data);
    if (ListCursorSearchNext(d->data, &DictionaryKVPCompare, key) == false)
        return NULL;

    assert(ListCursorIsNull(d->data) == false);

    return ListCursorGet(d->data);
}

bool DictionaryDelete(Dictionary d, String key)
{
    assert(d != NULL);
    assert(key != NULL);

    ListCursorBeforeHead(d->data);
    if (ListCursorSearchNext(d->data, &DictionaryKVPCompare, key) == false)
        return false;

    assert(ListCursorIsNull(d->data) == false);

    return ListCursorDelete(d->data);
}

bool DictionarySet(Dictionary d, String key, void *data, DataArgs dataArgs)
{
    DictionaryKVP kv;

    assert(d != NULL);
    assert(key != NULL);

    DictionaryDelete(d, key);

    kv = DictionaryKVPCreate();
    kv->key = StringCopy(key);
    kv->data = data;
    kv->size = dataArgs.size;
    kv->free_routine = dataArgs.free_routine;
    kv->copy_routine = dataArgs.copy_routine;

    ListAddFirst(d->data, kv, DataArgsDictionaryKVP());
    return true;
}

List DictionaryKeys(Dictionary d)
{
    List l;
    DictionaryKVP kv;

    assert(d != NULL);

    l = ListCreate(-1);

    ListCursorBeforeHead(d->data);
    while (ListCursorHasNext(d->data) == true)
    {
        ListCursorNext(d->data);

        kv = ListCursorGet(d->data);

        ListAddLast(l, StringCopy(kv->key), DataArgsString(kv->key));
    }

    return l;
}

void FreeDictionary(void *a)
{
    DictionaryFree((Dictionary)a);
}

void *CopyDictionary(void *a)
{
    return DictionaryCopy((Dictionary)a);
}

DataArgs DataArgsDictionary()
{
    DataArgs d;

    d.size = sizeof(struct Dictionary);
    d.free_routine = &FreeDictionary;
    d.copy_routine = &CopyDictionary;

    return d;
}
