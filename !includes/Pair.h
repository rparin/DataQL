#ifndef PAIR_H
#define PAIR_H

#include <iostream>
using namespace std;

template <typename K, typename V>
struct Pair{
    K _key;
    V _value;

    Pair(const K& k = K(), const V& v = V());

    template <typename X, typename Y>
    friend ostream& operator <<(ostream& outs, 
        const Pair<X, Y>& print_me);

    template <typename X, typename Y>
    friend bool operator ==(const Pair<X, Y>& lhs, 
        const Pair<X, Y>& rhs);

    template <typename X, typename Y>
    friend bool operator !=(const Pair<X, Y>& lhs,
        const Pair<X, Y>& rhs);

    template <typename X, typename Y>
    friend bool operator <(const Pair<X, Y>& lhs, 
        const Pair<X, Y>& rhs);

    template <typename X, typename Y>
    friend bool operator >(const Pair<X, Y>& lhs, 
        const Pair<X, Y>& rhs);

    template <typename X, typename Y>
    friend bool operator <=(const Pair<X, Y>& lhs, 
        const Pair<X, Y>& rhs);

    template <typename X, typename Y>
    friend bool operator >=(const Pair<X, Y>& lhs, 
        const Pair<X, Y>& rhs);

    template <typename X, typename Y>
    friend Pair<X, Y> operator +(const Pair<X, Y>& lhs, 
        const Pair<X, Y>& rhs);

    Pair<K, V> operator +=(const Pair<K, V>& rhs);
};

template<typename K, typename V>
Pair<K, V>::Pair(const K& k, const V& v):
    _key(k), _value(v) {
}

template<typename X, typename Y>
ostream& operator <<(ostream& outs, const Pair<X, Y>& print_me) {
    outs << print_me._key << ":" << print_me._value;
    return outs;
}

template<typename X, typename Y>
bool operator ==(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs) {
    return (lhs._key == rhs._key);
}

template<typename X, typename Y>
bool operator!=(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs){
    return (!(lhs== rhs));
}

template<typename X, typename Y>
bool operator <(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs) {
    return (lhs._key < rhs._key);
}

template<typename X, typename Y>
bool operator >(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs) {
    return (lhs._key > rhs._key);
}

template<typename X, typename Y>
bool operator <=(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs) {
    if (lhs == rhs) return true;
    else return lhs < rhs;
}

template<typename X, typename Y>
bool operator>=(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs) {
    if (lhs == rhs) return true;
    else return lhs > rhs;
}

template<typename X, typename Y>
Pair<X, Y> operator+(const Pair<X, Y>& lhs, const Pair<X, Y>& rhs){
    return lhs;
}

template<typename K, typename V>
Pair<K, V> Pair<K, V>::operator+=(const Pair<K, V>& rhs) {
    *this = rhs;
    return *this;
}

#endif //PAIR_H