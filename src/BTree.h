
#ifndef BTREE_H
#define BTREE_H

typedef long Key;

template <class Value>
class KeyValue
{
public:
    Key k;
    Value v;
};


template <u_int order, class Value>
class BTreeNode
{
    static_assert(order >= 3, "ordem mínima é 3");

    unsigned int nKeys;
    bool isLeaf;
    BTreeNode<order, Value> *pChildren[order];
    KeyValue<Value> kvData[order - 1];

    inline bool isFull() const { return this->nKeys == order - 1; }

    u_int select(KeyValue<Value> kv) const;
    
    BTreeNode<order, Value> *divideNode(
        KeyValue<Value> &kv,
        BTreeNode<order, Value> *child);

    BTreeNode<order, Value> *insertNode(
        KeyValue<Value> &kv,
        BTreeNode<order, Value> *child);


public:
    BTreeNode<order, Value> *insert(KeyValue<Value> &kv);
    // what if key is already in the tree?
};

#endif
