#ifndef MYPQUEUE_H
#define MYPQUEUE_H

#include <iostream>
#include <cassert>
#include "myHeap.h"
using namespace std;

//For timestamp
#include <chrono>
using namespace std::chrono;

template <typename T>
struct info {
    T _item;
    int _priority;
    time_t _time; //Store time in nanoseconds
    bool _debug;

    //Constructors
    info() {
        _item = T();
        _priority = -1;
        _debug = false;
        _time = duration_cast<nanoseconds>
            (system_clock::now().time_since_epoch()).count();
    }

    info(T i, int p) {
        _item = i;
        _priority = p;
        _debug = false;
        _time = duration_cast<nanoseconds>
            (system_clock::now().time_since_epoch()).count();
    }

    //Output
    template <typename U>
    friend ostream& operator <<
        (ostream& outs, const info<U>& print_me) {
        outs << print_me._item
            << ":" << print_me._priority;

        if (print_me._debug) {
            unsigned int time = print_me._time % 100000;
            outs << ":" << time;
        }

        return outs;
    }

    //Operators
    friend bool operator <(const info<T>& lhs, const info<T>& rhs) {
        if (lhs._priority < rhs._priority) return true;

        //Same Priority
        else if (lhs._priority == rhs._priority) {
            if (lhs._time >= rhs._time) return true;
            else return false;
        }

        else return false;
    }

    friend bool operator >(const info<T>& lhs, const info<T>& rhs) {
        if (lhs._priority > rhs._priority) return true;

        //Same Priority
        else if (lhs._priority == rhs._priority) {
            if (lhs._time <= rhs._time) return true;
            else return false;
        }

        else return false;
    }

    friend bool operator <=(const info<T>& lhs, const info<T>& rhs) {
        return lhs < rhs;
    }
    friend bool operator >=(const info<T>& lhs, const info<T>& rhs) {
        return lhs > rhs;
    }
};

template <typename T>
class PQueue
{
public:

    //Default Constructor
    PQueue(bool max = true); //Default Max Priority

    //Member Functions
    void push(const T& value, int p);
    T pop();
    void clear();

    //Accessors
    bool is_empty() const;
    int size() const;
    void print_tree(ostream& outs = cout) const;

    template <typename U>
    friend ostream& operator <<(ostream& outs, 
        const PQueue<U>& print_me);

private:
    Heap<info<T>> _pQ;
};

template<typename T>
PQueue<T>::PQueue(bool max):
    _pQ(max) {
}

template<typename T>
void PQueue<T>::push(const T& value, int p) {
    _pQ.push(info<T>(value, p));
}

template<typename T>
T PQueue<T>::pop()
{
    info<T>popped = _pQ.pop();

    if (popped._debug) {
        unsigned int time = popped._time % 100000;
        cout << time << " ";
    }

    return popped._item;
}

template<typename T>
void PQueue<T>::clear()
{
    _pQ.clear();
}

template<typename T>
bool PQueue<T>::is_empty() const
{
    return _pQ.is_empty();
}

template<typename T>
int PQueue<T>::size() const
{
    return _pQ.size();
}

template<typename T>
void PQueue<T>::print_tree(ostream& outs) const
{
    _pQ.print_tree(0,0,outs);
}

template<typename U>
ostream& operator<<(ostream& outs, const PQueue<U>& print_me)
{
    print_me.print_tree(outs);
    return outs;
}

#endif //MYPQUEUE_H