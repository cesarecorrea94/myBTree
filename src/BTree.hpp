
#ifndef BTREE_HPP
#define BTREE_HPP

#include <cassert>
#include <stdlib.h>
#include "BTree.h"

template <u_int order, class Value>
u_int BTreeNode<order, Value>::select(KeyValue<Value> kv) const
{
    for (u_int ii = 0; ii < this->nKeys; ii++)
    {
        if (kv.k < this->kvData[ii].k)
            return ii;
        else if (kv.k == this->kvData[ii].k)
        {
            // this->kvData[ii].v = kv.v; // ??
            throw false;
        }
    }
    return this->nKeys;
}

template <u_int order, class Value>
BTreeNode<order, Value> *BTreeNode<order, Value>::insert(
    KeyValue<Value> &kv)
{
    if (this->isLeaf)
        return this->insertNode(kv, NULL);

    u_int iChild = this->select(kv);
    BTreeNode<order, Value> *newChild = this->pChildren[iChild]->insert(kv);
    if (newChild)
        return this->insertNode(kv, newChild);
    return NULL;
}

template <u_int order, class Value>
BTreeNode<order, Value> *BTreeNode<order, Value>::insertNode(
    KeyValue<Value> &kv,
    BTreeNode<order, Value> *child)
{
    if (this->isFull())
        return this->divideNode(kv, child);

    u_int ii = this->select(kv);
    memmove(this->kvData[ii + 1], this->kvData[ii],
            (this->nKeys - ii) * sizeof(KeyValue<Value>));
    if (!this->isLeaf)
    {
        memmove(this->pChildren[ii + 2], this->pChildren[ii + 1],
                (this->nKeys - ii) * sizeof(void *));
        this->pChildren[ii + 1] = child;
    }
    this->kvData[ii] = kv;
    this->nKeys++;
    return NULL;
}

template <u_int order, class Value>
BTreeNode<order, Value> *BTreeNode<order, Value>::divideNode(
    KeyValue<Value> &kv,
    BTreeNode<order, Value> *child)
{
    BTreeNode<order, Value> *newChild = new BTreeNode<order, Value>();
    newChild->nKeys = this->nKeys / 2;
    this->nKeys -= newChild->nKeys;
    memcpy(newChild->kvData, this->kvData[this->nKeys],
           newChild->nKeys * sizeof(KeyValue<Value>));
    if (!this->isLeaf)
        memcpy(newChild->pChildren[1], this->pChildren[this->nKeys + 1],
               newChild->nKeys * sizeof(void *));
    if (kv.k < newChild->kvData[0].k)
    {
        this->insertNode(kv, child);
        this->nKeys--;
        kv = this->kvData[this->nKeys];
        if (!this->isLeaf)
            newChild->pChildren[0] = this->pChildren[this->nKeys + 1];
    }
    else
    {
        newChild->insertNode(kv, child);
        kv = newChild->kvData[0];
        newChild->nKeys--;
        memmove(newChild->kvData[0], newChild->kvData[1],
                newChild->nKeys * sizeof(KeyValue<Value>));
        if (!this->isLeaf)
            memmove(newChild->pChildren[0], newChild->pChildren[1],
                    (newChild->nKeys + 1) * sizeof(void *));
    }
    return newChild;
}

#endif
