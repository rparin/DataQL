#ifndef MYHASH
#define MYHASH

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <iomanip>
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
class Hash
{
public:
    //static const size_t CAPACITY = 811;
    static const size_t CAPACITY = 19;

    // CONSTRUCTOR
    //Defaults to Open Hash
    //false = Double Hash
    Hash(bool open = true);

    // MODIFICATION MEMBER FUNCTIONS
    void insert(const int& key, const T& value);
    void insert(const record<T>& rec);
    bool remove(int& key, T& value);
    bool remove(record<T>& rec);
    void clear();

    // CONSTANT MEMBER FUNCTIONS
    bool is_present(int& key) const;
    bool is_present(record<T>& rec) const;
    bool search(int key, T& value) const;
    bool search(record<T> rec) const;
    size_t size() const { return _used; }
    size_t collisions() const { return _collisions; }

    //Output
    template <typename U>
    friend ostream& operator <<(ostream& outs, const Hash<U>& print_me);

private:
    // MEMBER CONSTANTS
    //-- These are used in the key field of special records
    static const int NEVER_USED = -1;
    static const int PREVIOUSLY_USED = -2;

    // MEMBER VARIABLES
    //For dynamic CAPACITY make this a vector of records<T>
    record<T> _data[CAPACITY]; 
    size_t _used;
    bool _open;
    bool _debug;
    size_t _collisions;

    // HELPER FUNCTIONS
    size_t hash(int key) const;
    size_t hash2(int key) const;
    size_t next_index(size_t index) const;
    size_t next_index(size_t index, size_t offset) const;
    void find_index(int key, bool& found, size_t& index) const;
    bool never_used(size_t index) const;
    bool is_vacant(size_t index) const;
};

template<typename T>
Hash<T>::Hash(bool open):
    _data(), _used(0), _collisions(0),
    _debug(true), _open(open) {

    //Fill array with never used
    for (size_t i = 0; i < CAPACITY; ++i)
        _data[i]._key = NEVER_USED;
}

template<typename T>
void Hash<T>::insert(const int& key, const T& value)
{
    bool already_present;   // True if entry is already in the Hash
    size_t index;           //data[index] is location for the new entry
    
    assert(key >= 0);
    size_t hkey = hash(key);

    // Set index so that data[index] 
    //is the spot to place the new entry.
    find_index(key, already_present, index);

    // If the key wasn't already there, 
    //then find the location for the new entry.
    if (!already_present) {
        assert(size() < CAPACITY);

        index = hkey;
        while (!is_vacant(index)) {
            if(_open) index = next_index(index);
            else index = next_index(index, key);
        }
        if (hkey != index) _collisions++;
            
        ++_used;
    }

    //Store Record
    _data[index]._key = key;
    _data[index]._value = value;
}

template<typename T>
void Hash<T>::insert(const record<T>& rec)
{
    insert(rec._key, rec._value);
}

template<typename T>
bool Hash<T>::remove(int& key, T& value)
{
    assert(key >= 0);
    bool found;      // True if key occurs somewhere in the Hash
    size_t index;   // Spot where data[index] == key
    value = T();
    size_t hkey = hash(key);

    find_index(key, found, index);

    // The key was found, so remove 
    //this record and reduce used by 1.
    if (found) {

        if (hkey != index) _collisions--;

        //Store deleted value
        value = _data[index]._value;

        // Indicates a spot that's no longer in use.
        _data[index]._key = PREVIOUSLY_USED; 
        --_used;
    }

    //Return the value removed
    return found;
}

template<typename T>
bool Hash<T>::remove(record<T>& rec)
{
    return remove(rec._key, rec._value);
}

template<typename T>
void Hash<T>::clear()
{
    for (size_t i = 0; i < CAPACITY; ++i) {
        _data[i]._key = NEVER_USED;
        _data[i]._value = NEVER_USED;
    }
    _used = 0;
    _collisions = 0;

}

template<typename T>
bool Hash<T>::is_present(int& key) const
{
    bool found;
    size_t index;

    assert(key >= 0);

    find_index(key, found, index);
    return found;
}

template<typename T>
bool Hash<T>::is_present(record<T>& rec) const
{
    return is_present(rec._key);
}

template<typename T>
bool Hash<T>::search(int key, T& value) const
{
    size_t index;
    bool found;

    assert(key >= 0);

    find_index(key, found, index);
    if (found) value = _data[index]._value;

    return found;
}

template<typename T>
bool Hash<T>::search(record<T> rec) const
{
    return search(rec._key,rec._value);
}

template<typename T>
size_t Hash<T>::hash(int key) const
{
    return (key % CAPACITY);
}

template<typename T>
size_t Hash<T>::hash2(int key) const
{
    return  1 + (key % (CAPACITY - 2));
}

template<typename T>
size_t Hash<T>::
next_index(size_t index) const
{
    return ((index + 1) % CAPACITY);
}

template<typename T>
size_t Hash<T>::
next_index(size_t index, size_t offset) const
{
    return ((index + hash2(offset)) % CAPACITY);
}


template<typename T>
void Hash<T>::find_index(int key, bool& found, size_t& index) const
{
    // Number of entries that have been examined
    size_t count; 

    count = 0;
    index = hash(key);

    while ((count < CAPACITY) 
        && (_data[index]._key != NEVER_USED)
        && (_data[index]._key != key)) {
        ++count;
        if (_open) index = next_index(index);
        else index = next_index(index, key);
    }

    found = (_data[index]._key == key);
}

template<typename T>
bool Hash<T>::never_used(size_t index) const
{
    return (_data[index]._key == NEVER_USED);
}

template<typename T>
bool Hash<T>::is_vacant(size_t index) const
{
    return (_data[index]._key == NEVER_USED)
        || (_data[index]._key == PREVIOUSLY_USED);
}

template<typename U>
ostream& operator<<(ostream& outs, const Hash<U>& print_me)
{
    size_t hash;
    for (size_t i = 0; i < print_me.CAPACITY; ++i) {
        outs << "[" << setfill('0') << setw(3) << i << "] ";

        //If space was deleted
        if (print_me._data[i]._key == print_me.PREVIOUSLY_USED) {
            outs << "- - - - - - - -";
        }

        else if (print_me._data[i]._key != print_me.NEVER_USED) {
            hash = print_me.hash(print_me._data[i]._key);
            outs << print_me._data[i];

            //Print out if collision occurs
            if (print_me._debug) {
                outs << "(" << setfill('0') << setw(3)
                    << hash << ")";
                if (hash != i) outs << "*";
            }
        }
        outs << endl;
    }
    return outs;
}

#endif  //MYHASH
