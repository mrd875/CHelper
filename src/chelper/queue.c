#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "queue.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

struct Queue
{
    /*The data*/
    ArrayList data;
};

/*Creates a new Queue*/
Queue QueueCreate(free_fn_t free_fn, copy_fn_t copy_fn);

/* Copies the Queue. */
Queue QueueCopy(Queue h);

/*Frees the Queue*/
void QueueFree(Queue h);

/*CLears the Queue*/
void QueueClear(Queue h);

/*Gets the amount of elements in the Queue.*/
size_t QueueLength(Queue h);

/*Gets the item in the Queue*/
void *QueueGet(Queue h);

/*Removes the item from the Queue*/
bool QueueRemove(Queue h);

/*Adds the item to the Queue*/
bool QueueAdd(Queue h, void *data);

/*Removes from Queue, no free*/
bool QueueRemoveNoFree(Queue h);
