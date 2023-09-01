#ifndef MPAIR_H
#define MPAIR_H

#include <iostream>
#include "myBPlusTree.h"
#include "myVector.h"
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    //BPlusTree<V> value_list;
    Vector<V> value_list;

    //default CTOR / CTOR with a key and no value: 
    //this will create an empty BTree
    MPair(const K& k=K()) {
        key = k;
    }

    //CTOR with a key AND a value: pushes the value into the value_list
    MPair(const K& k, const V& v) {
        key = k;
        //value_list.insert(v);
        value_list += v;
    }

    //CTOR with a key and a BTree of values (to replace value_list)
    MPair(const K& k, const BPlusTree<V>& vlist) {
        key = k;
        value_list = vlist;
    }

    //------------------------Operators-------------------------------------

    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
        //outs << print_me.key << ":"; print_me.value_list.print_inorder();
        outs << print_me.key << ":" << print_me.value_list;
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        return (lhs.key == rhs.key);
    }

    friend bool operator !=(const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        return (lhs.key != rhs.key);
    }

    friend bool operator < (const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        return (lhs.key < rhs.key);
    }

    friend bool operator > (const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        return (lhs.key > rhs.key);
    }

    friend bool operator <= (const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        if (lhs == rhs) return true;
        else return (lhs.key < rhs.key);
    }

    friend bool operator >= (const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        if (lhs == rhs) return true;
        else return (lhs.key > rhs.key);
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, 
        const MPair<K, V>& rhs) {
        
        return MPair(lhs.key, unionVec(lhs.value_list, rhs.value_list));
    }

    MPair<K, V> operator +=(const MPair<K, V>& rhs) {
        value_list = unionVec(value_list, rhs.value_list);
        return *this;
    }
};
#endif //MPAIR_H
