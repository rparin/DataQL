#ifndef MYAVLHASH
#define MYAVLHASH

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iomanip>
#include "myAVL.h"
using namespace std;

template <typename T>
struct record {
    int _key;
    T _value;

    record(int k = 0, T v = T()) :_key(k), _value(v) {}

    //Operators
    friend bool operator ==
        (const record& left, const record& right) {
        if (left._key == right._key) return true;
        else return false;
    }
    friend bool operator <
        (const record& left, const record& right) {
        if (left._key < right._key) return true;
        else return false;
    }
    friend bool operator >
        (const record& left, const record& right) {
        return (!(left < right));
    }
    friend bool operator <=
        (const record& left, const record& right) {
        if (left == right) return true;
        else return (left < right);
    }
    friend bool operator >=
        (const record& left, const record& right) {
        if (left == right) return true;
        else return (left > right);
    }

    //Not used, Needed for sorted list
    //Insert sorted and add
    record& operator +=(const record& rec) {
        this->_value += rec._value;
        return *this;
    }

    //Output
    friend ostream& operator <<
        (ostream& outs, const record& print_me) {
        outs << setfill('0') << setw(4) << print_me._key
            << ":" << print_me._value;
        return outs;
    }
};

template <typename T>
class AVL_Hash
{
public:
    //static const size_t CAPACITY = 811;
    static const size_t CAPACITY = 17;

    // CONSTRUCTOR
    AVL_Hash();

    // MODIFICATION MEMBER FUNCTIONS
    void insert(const int& key, const T& value);
    void insert(const record<T>& rec);
    bool remove(int& key, T& value);
    bool remove(record<T>& rec);
    void clear();

    // CONSTANT MEMBER FUNCTIONS
    bool is_present(const int& key) const;
    bool is_present(const record<T>& rec) const;
    bool search(const int& key, T& value) const;
    bool search(record<T>& rec) const;
    size_t size() const { return _used; }

    template <typename U>
    friend ostream& operator 
        <<(ostream& outs, const AVL_Hash<U>& print_me);

private:

    // MEMBER VARIABLES
    AVL<record<T> > _data[CAPACITY];   //Array of List of records
    size_t _used;
    bool _debug;

    // HELPER FUNCTIONS
    size_t hash(int key) const;
};

template<typename T>
AVL_Hash<T>::AVL_Hash():
    _data(), _used(0), _debug(true) {
}

template<typename T>
void AVL_Hash<T>::insert(const int& key, const T& value)
{
    size_t index;           

    assert(key >= 0);

    index = hash(key);

    _data[index].insert(record<T>(key, value));
    ++_used;
}

template<typename T>
void AVL_Hash<T>::insert(const record<T>& rec)
{
    insert(rec._key, rec._value);
}

template<typename T>
bool AVL_Hash<T>::remove(int& key, T& value)
{
    assert(key >= 0);
    value = T();
    size_t index = hash(key);
    AVL<record<T> > temp = _data[index];
    bool found = search(key, value);

    if (found) {
        temp.remove(record<T>(key, value));
        _used--;
    }
    
    //Update avl
    _data[index] = temp;

    return found;
}

template<typename T>
bool AVL_Hash<T>::remove(record<T>& rec)
{
    return remove(rec._key, rec._value);
}

template<typename T>
void AVL_Hash<T>::clear()
{
    for (size_t i = 0; i < CAPACITY; i++) {
        _data[i].clear();
    }
    _data->clear();
    _used = 0;
}

template<typename T>
bool AVL_Hash<T>::is_present(const int& key) const
{
    assert(key >= 0);
    size_t index = hash(key);

    AVL<record<T> > temp = _data[index];
    tree_node<record<T> >* t_ptr;
    
    return temp.search(key, t_ptr);
}

template<typename T>
bool AVL_Hash<T>::is_present(const record<T>& rec) const
{
    return is_present(rec._key);
}

template<typename T>
bool AVL_Hash<T>::search(const int& key, T& value) const
{
    assert(key >= 0);
    size_t index = hash(key);

    AVL<record<T> > temp = _data[index];
    tree_node<record<T> >* t_ptr;
    bool found = temp.search(key, t_ptr);

    if(found) value = t_ptr->_item._value;

    return found;
}

template<typename T>
bool AVL_Hash<T>::search(record<T>& rec) const
{
    return search(rec._key, rec._value);
}

template<typename T>
size_t AVL_Hash<T>::hash(int key) const
{
    return (key % CAPACITY);
}

template<typename U>
ostream& operator<<(ostream& outs, const AVL_Hash<U>& print_me)
{
    for (size_t i = 0; i < print_me.CAPACITY; ++i) {
        outs << "[" << setfill('0') << setw(3) << i << "] ";

        if(!print_me._data[i].isempty())
            print_me._data[i].printInorder(outs);

        outs << endl;
    }

    return outs;
}

#endif  //MYAVLHASH