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
int max(int a, int b)
{
    if (a > b)
        return a;

    return b;
}

/*Returns the height of the subtree*/
int height(AVLTreeNode n)
{
    if (n == NULL)
        return 0;

    return n->height;
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

    n->height = max(height(n->left), height(n->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

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

    n->height = max(height(n->left), height(n->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

/*Gets the blance*/
int getBalance(AVLTreeNode n)
{
    if (n == NULL)
        return 0;

    return height(n->left) - height(n->right);
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

AVLTreeNode AVLTreeNodeSet(AVLTree t, AVLTreeNode node, int key, void *data)
{
    int nodeKey, balance;

    if (node == NULL)
    {
        return AVLTreeNodeCreate(t, key, data);
    }

    nodeKey = node->key;

    if (key < nodeKey)
    {
        node->left = AVLTreeNodeSet(t, node->left, key, data);
    }
    else if (key > nodeKey)
    {
        node->right = AVLTreeNodeSet(t, node->right, key, data);
    }
    else
    {
        AVLTreeNodeDataFree(node);
        node->data = data;
        return node;
    }

    node->height = max(height(node->left), height(node->right)) + 1;

    balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
    {
        return AVLTreeNodeRightRotate(node);
    }

    if (balance < -1 && key > node->right->key)
    {
        return AVLTreeNodeLeftRotate(node);
    }

    if (balance > 1 && key > node->left->key)
    {
        node->left = AVLTreeNodeLeftRotate(node->left);
        return AVLTreeNodeRightRotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = AVLTreeNodeRightRotate(node->right);
        return AVLTreeNodeLeftRotate(node);
    }

    return node;
}

AVLTreeNode AVLTreeNodeRemove(AVLTreeNode n, int key)
{
    int nodeKey, balance;
    AVLTreeNode temp;

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
        }
        else
        {
            temp = AVLTreeNodeMin(n->right);
        }
    }

    if (n == NULL)
        return NULL;

    n->height = max(height(n->left), height(n->right)) + 1;

    balance = getBalance(n);

    if (balance > 1 && getBalance(n->left) >= 0)
        return AVLTreeNodeRightRotate(n);

    if (balance > 1 && getBalance(n->left) < 0)
    {
        n->left = AVLTreeNodeLeftRotate(n->left);
        return AVLTreeNodeRightRotate(n);
    }

    if (balance < -1 && getBalance(n->right) <= 0)
        return AVLTreeNodeLeftRotate(n);

    if (balance < -1 && getBalance(n->right) > 0)
    {
        n->right = AVLTreeNodeRightRotate(n->right);
        return AVLTreeNodeLeftRotate(n);
    }

    return n;
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

void AVLTreeClear(AVLTree t)
{
    assert(t != NULL);
    AVLTreeNodeFreeTree(t->root);
    t->length = 0;
}

bool AVLTreeSet(AVLTree t, int key, void *data)
{
    assert(t != NULL);

    t->root = AVLTreeNodeSet(t, t->root, key, data);
    t->length++;

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
    assert(t != NULL);

    t->root = AVLTreeNodeRemove(t->root, key);
    t->length--;

    return true;
}
