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

/*Adds the key into the tree*/
bool AVLTreeAdd(AVLTree t, int key, void *data);

/*Has the key*/
bool AVLTreeHas(AVLTree t, int key);

/*gets the key*/
void *AVLTreeGet(AVLTree t, int key);

/*Removes the key*/
bool AVLTreeRemove(AVLTree t, int key);

/*Does an inorder traversal.*/
void AVLTreeInorderForEach(AVLTree t, foreach_fn_t foreach_fn);

/*Prints the tree's keys*/
void AVLTreePrintKeys(AVLTree t);

/*Removes the key from the tree, no free data*/
bool AVLTreeRemoveNoFree(AVLTree t, int key);

#endif /* __CHELPERAVL__ */
