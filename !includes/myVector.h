#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <initializer_list>
#include <cassert>
#include "oneD.h"
using namespace std;

template <typename T>
class Vector {
public:

	//Default Constructor
	Vector(int size = 3);

	//List Init Constructor
	Vector(initializer_list<T> l);

	//Big Three:
	Vector(const Vector& other);
	~Vector();
	Vector& operator =(const Vector& rhs);

	//Member Access Functions:
	const T operator [](const int index) const;
	T& operator [](const int index);
	T& at(int index);
	const T at(int index) const;
	T& front() const;
	T& back() const;
	
	//Inserts item to vector
	Vector& operator +=(const T& item);

	//Appends Vector item to this object
	Vector& operator +=(const Vector<T>& vec);

	//Push and Pop functions:
	void push_back(const T& item); //Insert item
	T pop_back(); //removes last item

	//Equality Operator
	template <typename U>
	friend bool operator ==(const Vector<U>& lhs, const Vector<U>& rhs);

	//Insert and Erase:
	void insert(int insert_here, const T& insert_this);
	void erase(int erase_index);
	void clear();

	//Search
	int index_of(const T& item);
	bool contains(const T& item);

	//size and capacity:
	void set_size(int size);
	void set_capacity(int capacity);
	int size() const;
	int capacity() const;
	bool empty() const;

	//OUTPUT:
	template <typename U>
	friend ostream& operator <<(ostream& outs, const Vector<U>& _a);

private:
	int _size;
	int _capacity;
	T* _v;
	bool _debug;
};


template<typename T>
Vector<T>::Vector(int size):
	_v(),_size(0), _capacity(size), _debug(false) {
	_v = allocate<T>(_capacity);

	if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
Vector<T>::Vector(initializer_list<T> l):
	_v(), _size(0),
	_capacity(l.size()), _debug(false) {

	//Allocate space for vector
	_v = allocate<T>(_capacity);

	//Add items to vector
	for (T item : l) push_back(item);
}

template<typename T>
Vector<T>::Vector(const Vector& other) :
	_v(), _size(0), _capacity(0), _debug(false) {

	//Copy Info from other class
	_size = other._size;
	_capacity = other._capacity;
	_debug = other._debug;

	//Create new dynamic location
	_v = allocate<T>(_capacity);

	//Copy items
	copy(_v, other._v, _size);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

template<typename T>
Vector<T>::~Vector()
{
	delete[] _v;
	_v = nullptr;
	if (_debug) cout << "Destructor Executed" << endl;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
{
	//Check for self-reference
	if (this == &rhs) return *this;

	//Delete dynamic space if it exits already
	if (_v != nullptr) delete[] _v;

	//Copy Info from other class
	_size = rhs._size;
	_capacity = rhs._capacity;

	//Create new dynamic location
	_v = allocate<T>(_capacity);

	//Copy items
	copy(_v, rhs._v, _size);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename T>
const T Vector<T>::operator[](const int index) const
{
	return at(index);
}

template<typename T>
T& Vector<T>::operator[](const int index)
{
	return at(index);
}

template<typename T>
T& Vector<T>::at(int index)
{

	assert(index >= 0 && index <= _size);

	T* index_ptr = _v + index;

	return *index_ptr;
}

template<typename T>
const T Vector<T>::at(int index) const
{
	assert(index >= 0 && index <= _size);

	T* index_ptr = _v + index;

	return *index_ptr;
}

template<typename T>
T& Vector<T>::front() const
{
	return *_v;
}

template<typename T>
T& Vector<T>::back() const
{
	//Pointer to last element
	T* back = _v + _size - 1;

	return *back;
}

template<typename T>
Vector<T>& Vector<T>::operator+=(const T& item) {
	push_back(item);

	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& vec)
{
	for(int i = 0; i < vec._size; i++)
		push_back(vec[i]);

	return *this;
}

template<typename T>
void Vector<T>::push_back(const T& item)
{
	_v = add_entry(_v, item, _size, _capacity);
}

template<typename T>
T Vector<T>::pop_back()
{
	assert(_size > 0);

	T removed = back();

	//Remove last element
	_v = remove<T>(_v, _size, _capacity);

	//return last element
	return removed;
}

template<typename T>
void Vector<T>::insert(int insert_here, const T& insert_this)
{
	assert(insert_here <= _size - 1 && insert_here >= 0);

	push_back(insert_this);

	//pointer to point to index to insert
	T* index_ptr = _v + insert_here;

	shift_right(_v, _size, index_ptr);

	at(insert_here) = insert_this;
}

template<typename T>
void Vector<T>::erase(int erase_index)
{
	assert(erase_index <= _size - 1 && erase_index >= 0);

	_v = remove_index(_v, erase_index, _size, _capacity);
}

template<typename T>
int Vector<T>::index_of(const T& item)
{
	int index = -1;

	//Get pointer to item, null ptr if not found
	T* item_ptr = search(_v, _size, item);

	//Subtract pointers to get location
	if (item_ptr != nullptr) index = item_ptr - _v;

	//assert(index >= 0);

	return index;
}

template<typename T>
bool Vector<T>::contains(const T& item) {
	for (int i = 0; i < _size; i++) {
		if (item == this->at(i)) return true;
	}
	return false;
}

template<typename T>
void Vector<T>::clear() {
	//Delete Vector
	this->~Vector();

	//Create and set new vector
	Vector<T> a;
	*this = a;
}

template<typename T>
void Vector<T>::set_size(int size)
{
	assert(size >= 0);

	//Append lowest element for each type to increase size
	for (int i = _size; i < size; i++)
		_v = add_entry(_v, T(), _size, _capacity);

	_size = size;

	//Create a new dynamic array with half the capacity
	//if size is 1/4 of the capacity
	int quarter_size = static_cast<int>(.25 * _capacity);
	if (_size <= quarter_size)
		set_capacity(_capacity / 2);

}

template<typename T>
void Vector<T>::set_capacity(int capacity)
{
	assert(capacity >= _size);

	//Set capacity
	_capacity = capacity;

	//set new space
	_v = reallocate(_v, _size, _capacity);
}

template<typename T>
int Vector<T>::size() const
{
	return _size;
}

template<typename T>
int Vector<T>::capacity() const
{
	return _capacity;
}

template<typename T>
bool Vector<T>::empty() const
{
	if (_size == 0)
		return true;

	return false;
}

template<typename U>
bool operator==(const Vector<U>& lhs, const Vector<U>& rhs) {
	if (lhs.size() == rhs.size()) {
		for (int i = 0; i < lhs.size(); i++) {
			if (lhs[i] != rhs[i]) return false;
		}
	}
	return true;
}

template<typename U>
ostream& operator<<(ostream& outs, const Vector<U>& print_me)
{
	print(print_me._v, print_me._size, outs);
	return outs;
}

//-----------------Vector Functions-----------------------\\

template<typename T>
Vector<T> unionVec(const Vector<T>& v1, const Vector<T>& v2) {
	Vector<T> vUnion;
	int i1 = 0, i2 = 0;

	//Go through both vectors and stop 
	//when you reach the end of one vector
	while (i1 < v1.size() && i2 < v2.size()) {
		//Get item from v1
		//v1 has lesser item
		if (v1[i1] < v2[i2]) {

			//Add Item
			vUnion += v1[i1];

			//Move v1
			i1++;
		}

		//Get item from v2
		//v1 Has greater item
		else if (v1[i1] > v2[i2]) {

			//Add Item
			vUnion += v2[i2];

			//Move v2
			i2++;
		}

		//v1 item is equal to v2 item
		else {
			//Move dup index item
			i1++;
		}
	}

	//Add rest of v1 items to vUnion
	for (; i1 < v1.size(); i1++) {
		vUnion += v1[i1];
	}

	//Add rest of v2 items to vUnion
	for (; i2 < v2.size(); i2++) {
		vUnion += v2[i2];
	}

	return vUnion;
}

template<typename T>
Vector<T> intrsectVect(const Vector<T>& v1,
	const Vector<T>& v2) {
	Vector<T> result;
	int i1 = 0, i2 = 0;

	//Go through both vectors and stop 
	//when you reach the end of one vector
	while (i1 < v1.size() && i2 < v2.size()) {

		//Items not equal to
		if (v1[i1] < v2[i2]) {
			//Move v1
			i1++;
		}

		//Items not equal to
		else if (v1[i1] > v2[i2]) {

			//Move v2
			i2++;
		}

		//v1 item is equal to v2 item
		else {

			//Add item to result
			result += v1[i1];

			//Move indexes up
			i1++; i2++;
		}
	}
	return result;
}

#endif //VECTOR_H