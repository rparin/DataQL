#ifndef MAP_H
#define MAP_H

#include <iostream>
#include "Pair.h"
#include "myBPlusTree.h"
using namespace std;

template <typename K, typename V>
class Map {
public:
    typedef BPlusTree<Pair<K, V> > map_base;

    class Iterator{
    public:
        friend class Map;
        Iterator (typename map_base::Iterator it = nullptr):
            _it(it) {
        }

        Iterator operator ++() {
            _it++;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused) {
            Iterator hold = it;
            ++it;
            return hold;
        }

        Pair<K, V> operator *() {
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

//  Default Constructor
    Map(bool _dups_ok = true);

//  Iterators
    Iterator begin() const;
    Iterator end() const;

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    //return first that goes AFTER key (>)
    Iterator upper_bound(const K& key);

    // return first that goes NOT BEFORE (>=)
    Iterator lower_bound(const K& key);

    bool is_valid(){ return _map.is_valid(); }

    template<typename X, typename Y>
    friend ostream& operator<<
        (ostream& outs, const Map<X, Y>& print_me);

    void print_inorder();

private:
    int _key_count;
    BPlusTree<Pair<K, V>> _map;
};

template<typename K, typename V>
Map<K, V>::Map(bool _dups_ok) :
    _map(_dups_ok), _key_count(0) {
}

template<typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::begin() const{
    return _map.begin();
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() const{
    return Iterator(nullptr);
}

template<typename K, typename V>
int Map<K, V>::size() const {
    return _map.get_size();
}

template<typename K, typename V>
bool Map<K, V>::empty() const {
    return _map.is_empty();
}

template<typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    return _map.get(Pair<K, V>(key))._value;
}

template<typename K, typename V>
V& Map<K, V>::at(const K& key) {
    return _map.get(Pair<K, V>(key))._value;
}

template<typename K, typename V>
const V& Map<K, V>::at(const K& key) const {
    return _map.get(Pair<K, V>(key))._value;
}

template<typename K, typename V>
void Map<K, V>::insert(const K& k, const V& v) {
    _map.insert(Pair<K, V>(k, v));
    _key_count = _map.get_size();
}

template<typename K, typename V>
void Map<K, V>::erase(const K& key) {
    _map.remove(Pair<K, V>(key));
    _key_count = _map.get_size();
}

template<typename K, typename V>
void Map<K, V>::clear() {
    _map.clear();
}

template<typename K, typename V>
V Map<K, V>::get(const K& key) {
    return _map.get(Pair<K, V>(key))._value;
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key) {
    return _map.find(key);
}

template<typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) const {
    return _map.contains(target);
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::upper_bound(const K& key){
    return _map.upper_bound(key);
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::lower_bound(const K& key) {
    return _map.lower_bound(key);
}

template<typename K, typename V>
void Map<K, V>::print_inorder(){
    _map.print_inorder();
}

template<typename X, typename Y>
ostream& operator<<(ostream& outs, 
    const Map<X, Y>& print_me) {

    //outs << print_me._map << endl;
    typename Map<X, Y>::Iterator it;
    for (it = print_me.begin(); it != print_me.end(); it++) {
        outs << "\t" << *it << endl;
    }

    return outs;
}

#endif //MAP_H
