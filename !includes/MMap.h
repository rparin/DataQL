#ifndef MMAP_H
#define MMAP_H

#include <iostream>
#include "MPair.h"
using namespace std;

template <typename K, typename V>
class MMap {
public:
    typedef BPlusTree<MPair<K, V> > map_base;

    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = nullptr) :
            _it(it) {}

        Iterator operator ++() {
            _it++;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused) {
            Iterator hold = it;
            ++it;
            return hold;
        }

        MPair<K, V> operator *() {
            return *_it;
        }

        friend bool operator ==
            (const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }

        friend bool operator !=
            (const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }


    private:
        typename map_base::Iterator _it;
    };

    MMap(bool _dups_ok = true);
    MMap(const K& k, const V& v = V());

//  Iterators
    Iterator begin() const;
    Iterator end() const;

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const Vector<V>& operator[](const K& key) const;
    Vector<V>& operator[](const K& key);

    //Asserts if Key not in list
    void print_values(const K& key) const;

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const;
    Vector<V>& get(const K& key);

    Iterator find(const K& key);

    //How Many Elements in Value list
    int count(const K& key);

    //return first that goes AFTER key (>)
    Iterator upper_bound(const K& key);

    // return first that goes NOT BEFORE (>=)
    Iterator lower_bound(const K& key);

    bool is_valid();

    template <typename X, typename Y>
    friend ostream& operator<<(ostream& outs, const MMap<X, Y>& print_me);

private:
    BPlusTree<MPair<K, V> > mmap;
};

template<typename K, typename V>
MMap<K, V>::MMap(bool _dups_ok):
    mmap(_dups_ok) {
}

template<typename K, typename V>
MMap<K, V>::MMap(const K& k, const V& v) :
    mmap() {
    insert(k, v);
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::begin() const {
    return mmap.begin();
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end() const {
    return Iterator(nullptr);
}

template<typename K, typename V>
int MMap<K, V>::size() const {
    return mmap.get_size();
}

template<typename K, typename V>
bool MMap<K, V>::empty() const {
    return mmap.is_empty();
}

template<typename K, typename V>
const Vector<V>& MMap<K, V>::operator[](const K& key) const {

    //Asserts if key not in map
    return mmap.get(key).value_list;
}

template<typename K, typename V>
Vector<V>& MMap<K, V>::operator[](const K& key) {

    //Adds item if item not in map
    return mmap.get(key).value_list;
}

template<typename K, typename V>
void MMap<K, V>::print_values(const K& key) const{
    cout << mmap.get(key).value_list << endl;
}

template<typename K, typename V>
void MMap<K, V>::insert(const K& k, const V& v) {
    mmap.get(k).value_list += v;
   //mmap.insert(MPair<K, V>(k, v));
}

template<typename K, typename V>
void MMap<K, V>::erase(const K& key) {
    mmap.remove(key);
}

template<typename K, typename V>
void MMap<K, V>::clear() {
    mmap.clear();
}

template<typename K, typename V>
bool MMap<K, V>::contains(const K& key) const {
    return mmap.contains(key);
}

template<typename K, typename V>
Vector<V>& MMap<K, V>::get(const K& key) {
    return mmap.get(key).value_list;
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K& key) {
    return mmap.find(key);
}

template<typename K, typename V>
int MMap<K, V>::count(const K& key) {
    return mmap.get(key).value_list.size();
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>
::upper_bound(const K& key){
    return mmap.upper_bound(key);
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>
::lower_bound(const K& key){
    return mmap.lower_bound(key);
}

template<typename K, typename V>
bool MMap<K, V>::is_valid() {
    return mmap.is_valid();
}

template<typename X, typename Y>
ostream& operator<<(ostream& outs, const MMap<X, Y>& print_me) {
    //outs << print_me.mmap;

    typename MMap<X, Y>::Iterator it = print_me.begin();
    for (it = print_me.begin(); it != print_me.end(); it++) {
        outs << "\t" << *it << endl;
    }

    return outs;
}

#endif //MMAP_H