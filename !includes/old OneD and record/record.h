#ifndef RECORD_H
#define RECORD_H

#include <iostream>
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

#endif //RECORD_H