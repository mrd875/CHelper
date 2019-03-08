#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "avltree.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

/*The avl tree node*/
typedef struct AVLTreeNode *AVLTreeNode;

struct AVLTreeNode
{
    /*The data*/
    void *data;
    /*The key*/
    int key;
    /*The height of this tree*/
    size_t height;
    /*The owner of this node*/
    AVLTree owner;

    /*Left subtree*/
    AVLTreeNode left;
    /*Right subtree*/
    AVLTreeNode right;
};

struct AVLTree
{
    /*How to free data*/
    free_fn_t free_fn;
    /*How to copy*/
    copy_fn_t copy_fn;

    /*How many items in the tree*/
    size_t length;

    /*The root node of the tree*/
    AVLTreeNode root;
};

/*Returns max of the two*/
int AVLMax(int a, int b)
{
    if (a > b)
        return a;

    return b;
}

/*Returns the height of the subtree*/
int AVLTreeNodeGetHeight(AVLTreeNode n)
{
    if (n == NULL)
        return 0;

    return n->height;
}

/*Gets the blance*/
int AVLTreeNodeGetBalance(AVLTreeNode n)
{
    if (n == NULL)
        return 0;

    return AVLTreeNodeGetHeight(n->left) - AVLTreeNodeGetHeight(n->right);
}

/*Creates a node*/
AVLTreeNode AVLTreeNodeCreate(AVLTree t, int key, void *data)
{
    AVLTreeNode n;

    n = malloc(sizeof(struct AVLTreeNode));
    assert(n != NULL);

    n->key = key;
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->owner = t;
    n->height = 1;

    return n;
}

/*Frees the node's data*/
void AVLTreeNodeDataFree(AVLTreeNode n)
{
    assert(n != NULL);
    assert(n->owner != NULL);

    if (n->owner->free_fn != NULL)
        n->owner->free_fn(n->data);
}

/*Frees the node and its data*/
void AVLTreeNodeFree(AVLTreeNode n)
{
    assert(n != NULL);

    AVLTreeNodeDataFree(n);

    free(n);
}

/*Recursivly frees the whole subtree*/
void AVLTreeNodeFreeTree(AVLTreeNode n)
{
    if (n == NULL)
        return;

    AVLTreeNodeFreeTree(n->left);
    AVLTreeNodeFreeTree(n->right);

    AVLTreeNodeFree(n);
}

/*Copies the node's data*/
void *AVLTreeNodeDataCopy(AVLTreeNode n)
{
    void *data;

    assert(n != NULL);
    assert(n->owner != NULL);

    data = NULL;

    if (n->owner->copy_fn != NULL)
        data = n->owner->copy_fn(n->data);

    return data;
}

/*Right rotates the avl tree node.*/
AVLTreeNode AVLTreeNodeRightRotate(AVLTreeNode n)
{
    AVLTreeNode x, y;

    assert(n != NULL);
    assert(n->left != NULL);

    x = n->left;
    y = x->right;

    x->right = n;
    n->left = y;

    n->height = AVLMax(AVLTreeNodeGetHeight(n->left), AVLTreeNodeGetHeight(n->right)) + 1;
    x->height = AVLMax(AVLTreeNodeGetHeight(x->left), AVLTreeNodeGetHeight(x->right)) + 1;

    return x;
}

/*Left rotates the avl tree node.*/
AVLTreeNode AVLTreeNodeLeftRotate(AVLTreeNode n)
{
    AVLTreeNode x, y;

    assert(n != NULL);
    assert(n->right != NULL);

    x = n->right;
    y = x->left;

    x->left = n;
    n->right = y;

    n->height = AVLMax(AVLTreeNodeGetHeight(n->left), AVLTreeNodeGetHeight(n->right)) + 1;
    x->height = AVLMax(AVLTreeNodeGetHeight(x->left), AVLTreeNodeGetHeight(x->right)) + 1;

    return x;
}

/*Get the left most node from this node*/
AVLTreeNode AVLTreeNodeMin(AVLTreeNode n)
{
    AVLTreeNode a;

    assert(n != NULL);

    a = n;
    while (a->left != NULL)
        a = a->left;

    return a;
}

/*Restores the avl invarient on the node.*/
AVLTreeNode AVLTreeNodeRestoreAVL(AVLTreeNode n)
{
    int balance;

    if (n == NULL)
        return NULL;

    n->height = AVLMax(AVLTreeNodeGetHeight(n->left), AVLTreeNodeGetHeight(n->right)) + 1;

    balance = AVLTreeNodeGetBalance(n);

    if (balance > 1)
    {
        if (AVLTreeNodeGetBalance(n->left) < 0)
            n->left = AVLTreeNodeLeftRotate(n->left);

        return AVLTreeNodeRightRotate(n);
    }

    if (balance < -1)
    {
        if (AVLTreeNodeGetBalance(n->right) > 0)
            n->right = AVLTreeNodeRightRotate(n->right);

        return AVLTreeNodeLeftRotate(n);
    }

    return n;
}

/*Adds data to the node resursive*/
AVLTreeNode AVLTreeNodeAdd(AVLTree t, AVLTreeNode n, int key, void *data)
{
    int nodeKey;

    if (n == NULL)
    {
        assert(t != NULL);
        t->length++;
        return AVLTreeNodeCreate(t, key, data);
    }

    nodeKey = n->key;

    if (key < nodeKey)
    {
        n->left = AVLTreeNodeAdd(t, n->left, key, data);
    }
    else
    {
        n->right = AVLTreeNodeAdd(t, n->right, key, data);
    }

    return AVLTreeNodeRestoreAVL(n);
}

/*Removes data from the node, recursive*/
AVLTreeNode AVLTreeNodeRemove(AVLTreeNode n, int key)
{
    int nodeKey;
    AVLTreeNode temp;
    void *data;

    if (n == NULL)
        return NULL;

    nodeKey = n->key;

    if (key < nodeKey)
        n->left = AVLTreeNodeRemove(n->left, key);
    else if (key > nodeKey)
        n->right = AVLTreeNodeRemove(n->right, key);
    else
    {
        if (n->left == NULL || n->right == NULL)
        {
            temp = n->left;
            if (temp == NULL)
                temp = n->right;

            assert(n->owner != NULL);

            n->owner->length--;
            AVLTreeNodeFree(n);
            n = temp;
        }
        else
        {
            temp = AVLTreeNodeMin(n->right);

            data = temp->data;
            temp->data = n->data;

            n->data = data;
            n->key = temp->key;

            n->right = AVLTreeNodeRemove(n->right, temp->key);
        }
    }

    return AVLTreeNodeRestoreAVL(n);
}

AVLTree AVLTreeCreate(free_fn_t free_fn, copy_fn_t copy_fn)
{
    AVLTree t;

    t = malloc(sizeof(struct AVLTree));
    assert(t != NULL);

    t->copy_fn = copy_fn;
    t->free_fn = free_fn;
    t->length = 0;
    t->root = NULL;

    return t;
}

void AVLTreeFree(AVLTree t)
{
    assert(t != NULL);

    AVLTreeClear(t);

    free(t);
}

size_t AVLTreeLength(AVLTree t)
{
    assert(t != NULL);

    return t->length;
}

void AVLTreeClear(AVLTree t)
{
    assert(t != NULL);
    AVLTreeNodeFreeTree(t->root);
    t->root = NULL;
    t->length = 0;
}

bool AVLTreeAdd(AVLTree t, int key, void *data)
{
    size_t oldSize;
    assert(t != NULL);

    oldSize = AVLTreeLength(t);

    t->root = AVLTreeNodeAdd(t, t->root, key, data);

    if (oldSize == AVLTreeLength(t))
        return false;

    return true;
}

void *AVLTreeGet(AVLTree t, int key)
{
    AVLTreeNode n;

    assert(t != NULL);

    n = t->root;

    while (n != NULL)
    {
        if (key == n->key)
            return n->data;

        if (key < n->key)
            n = n->left;
        else
            n = n->right;
    }

    return NULL;
}

bool AVLTreeHas(AVLTree t, int key)
{
    AVLTreeNode n;

    assert(t != NULL);

    n = t->root;

    while (n != NULL)
    {
        if (key == n->key)
            return true;

        if (key < n->key)
            n = n->left;
        else
            n = n->right;
    }

    return false;
}

bool AVLTreeRemove(AVLTree t, int key)
{
    size_t oldSize;
    assert(t != NULL);

    oldSize = AVLTreeLength(t);

    t->root = AVLTreeNodeRemove(t->root, key);

    if (oldSize == AVLTreeLength(t))
        return false;

    return true;
}

bool AVLTreeRemoveNoFree(AVLTree t, int key)
{
    free_fn_t free_fn;
    bool o;

    assert(t != NULL);

    free_fn = t->free_fn;

    t->free_fn = NULL;

    o = AVLTreeRemove(t, key);

    t->free_fn = free_fn;

    return o;
}

/*Does recursively inorder.*/
size_t AVLTreeNodeInorderForEach(AVLTreeNode n, foreach_fn_t foreach_fn, size_t i)
{
    if (n == NULL)
        return i;

    i = AVLTreeNodeInorderForEach(n->left, foreach_fn, i);

    foreach_fn(n->data, i);
    i++;

    i = AVLTreeNodeInorderForEach(n->right, foreach_fn, i);

    return i;
}

void AVLTreeInorderForEach(AVLTree t, foreach_fn_t foreach_fn)
{
    assert(t != NULL);
    assert(foreach_fn != NULL);

    AVLTreeNodeInorderForEach(t->root, foreach_fn, 0);
}

String AVLTreeNodePrintKeys(AVLTreeNode n, size_t depth)
{
    size_t i;
    String blanks, left, right, here, answer;

    blanks = StringCopy("");
    for (i = 0; i < depth - 1; i++)
    {
        blanks = StringAdd(blanks, "            ");
    }

    if (n == NULL)
    {
        left = StringCopy("");
        here = StringFormat("%lu/%lu: -", depth, AVLTreeNodeGetHeight(n));
        right = StringCopy("");
    }
    else
    {
        left = AVLTreeNodePrintKeys(n->left, depth + 1);
        here = StringFormat("%lu/%lu: %d", depth, AVLTreeNodeGetHeight(n), n->key);
        right = AVLTreeNodePrintKeys(n->right, depth + 1);
    }

    answer = StringCopy(left);
    answer = StringAdd(answer, "\n");
    answer = StringAdd(answer, blanks);
    answer = StringAdd(answer, here);
    answer = StringAdd(answer, "\n");
    answer = StringAdd(answer, right);

    free(left);
    free(right);
    free(blanks);
    free(here);

    return answer;
}

void AVLTreePrintKeys(AVLTree t)
{
    String s;

    assert(t != NULL);

    s = AVLTreeNodePrintKeys(t->root, 1);

    printf("%s\n", s);

    free(s);
}
