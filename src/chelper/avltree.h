#ifndef __CHELPERAVL__
#define __CHELPERAVL__

#include <stdbool.h>
#include <stdlib.h>

/* This is the AVLTree. */
typedef struct AVLTree *AVLTree;

#include "common.h"
#include "string.h"

/*Creates a new avl tree*/
AVLTree AVLTreeCreate(free_fn_t free_fn, copy_fn_t copy_fn);

/*Frees the tree*/
void AVLTreeFree(AVLTree t);

/*CLears the tree*/
void AVLTreeClear(AVLTree t);

/*Gets the amount of elements in the tree.*/
size_t AVLTreeLength(AVLTree t);

/*Sets the key into the tree*/
bool AVLTreeSet(AVLTree t, int key, void *data);

/*Has the key*/
bool AVLTreeHas(AVLTree t, int key);

/*gets the key*/
void *AVLTreeGet(AVLTree t, int key);

/*Removes the key*/
bool AVLTreeRemove(AVLTree t, int key);

#endif /* __CHELPERAVL__ */
