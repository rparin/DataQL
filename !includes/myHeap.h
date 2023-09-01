#ifndef MYHEAP_H
#define MYHEAP_H

#include <iostream>
#include <cassert>
#include <iomanip>
#include "oneD.h"
using namespace std;

template <typename T>
class Heap {
public:

	//Default Constructor
	Heap(unsigned int size = 100, 
		bool maxHeap = true);

	//Big Three:
	Heap(const Heap& other);
	~Heap();
	Heap& operator =(const Heap& rhs);

	//Member Functions
	void push(const T& insert_me);
	T pop();
	bool search(T find_me, int& found_index);
	void clear();

	//Accessors
	unsigned int size() const;
	unsigned int capacity() const;
	bool is_empty() const;
	void print_tree(ostream& outs = cout) const;

	//OUTPUT:
	template <typename U>
	friend ostream& operator 
		<<(ostream& outs, const Heap<U>& print_me);

	void print_tree(unsigned int root, 
		int level = 0, ostream& outs = cout) const;

private:
	T* _heapArr;
	unsigned int _size;
	unsigned int _capacity;
	bool _maxHeap;	//True for Maximum heap, False for Minimum Heap
	bool _debug;

	//Helper Functions
	bool is_leaf(unsigned int i) const;
	unsigned int parent_index(unsigned int i) const;
	unsigned int left_child_index(unsigned int i)const;
	unsigned int right_child_index(unsigned int i) const;
	unsigned int big_small_child_index(unsigned int i) const;
	void swap_with_parent(unsigned int i);
	void swap_with_children(unsigned int i);
};

template<typename T>
Heap<T>::Heap(unsigned int size, bool maxHeap):
	 _size(0), _debug(false),
	_capacity(size), _maxHeap(maxHeap) {

	_heapArr = allocate<T>(_capacity);

	if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
Heap<T>::Heap(const Heap& other)
{
	//Copy Info from other class
	_size = other._size;
	_capacity = other._capacity;
	_maxHeap = other._maxHeap;
	_debug = other._debug;

	//Create new dynamic location
	_heapArr = allocate<T>(_size);

	//Copy items
	int cap = _size; //function requires int
	copy(_heapArr, other._heapArr, cap);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

template<typename T>
Heap<T>::~Heap()
{
	delete[] _heapArr;
	_heapArr = nullptr;
	if (_debug) cout << "Destructor Executed" << endl;
}

template<typename T>
Heap<T>& Heap<T>::operator=(const Heap& rhs)
{
	//Check for self-reference
	if (this == &rhs) return *this;

	//Delete dynamic space if it exits already
	if (_heapArr != nullptr) delete[] _heapArr;

	//Copy Info from other class
	_size = rhs._size;
	_capacity = rhs._capacity;
	_maxHeap = rhs._maxHeap;
	_debug = rhs._debug;

	//Create new dynamic location
	_heapArr = allocate<T>(_size);

	//Copy items
	int cap = _size; //function requires int
	copy(_heapArr, rhs._heapArr, cap);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename T>
void Heap<T>::push(const T& insert_me)
{
	assert(_size < _capacity);

	//Insert at size
	_heapArr[_size] = insert_me;

	//Move item up the tree if necessary
	swap_with_parent(_size);

	//Increase size
	_size++;
}

template<typename T>
T Heap<T>::pop()
{
	if (_size > 0) {
		//Hold onto the root
		T root = _heapArr[0];

		//Replace root with last item in tree
		_heapArr[0] = _heapArr[_size - 1];

		//Decrase Size
		_size--;

		//Let new root sink into place
		swap_with_children(0);

		//Return item at top of heap
		return root;
	}

	return T();
}

template<typename T>
bool Heap<T>::search(T find_me, int& found_index)
{
	//Not Found
	found_index = -1;

	for (unsigned int i = 0; i < _size; i++) {

		//Found
		if (_heapArr[i] == find_me) {
			found_index = i;
			return true;
		}
	}

	return false;
}

template<typename T>
void Heap<T>::clear()
{
	_size = 0;
}

template<typename T>
unsigned int Heap<T>::size() const
{
	return _size;
}

template<typename T>
unsigned int Heap<T>::capacity() const
{
	return _capacity;
}

template<typename T>
bool Heap<T>::is_empty() const
{
	return _size == 0;
}

template<typename T>
void Heap<T>::print_tree(ostream& outs) const
{
	print_tree(0, 0, outs);
}

template<typename T>
bool Heap<T>::is_leaf(unsigned int i) const
{
	//Get index of left and right child
	unsigned int l_index = left_child_index(i);
	unsigned int r_index = right_child_index(i);

	//Item at index is a leaf if
	//it has no left or right child
	return l_index >= _size && r_index >= _size;
}

template<typename T>
unsigned int Heap<T>::parent_index(unsigned int i) const
{
	if (i == 0) return 0;
	else  return (i - 1) / 2;
}

template<typename T>
unsigned int Heap<T>::left_child_index(unsigned int i) const
{
	return (2 * i) + 1;
}

template<typename T>
unsigned int Heap<T>::right_child_index(unsigned int i) const
{
	return (2 * i) + 2;
}

template<typename T>
unsigned int Heap<T>::
big_small_child_index(unsigned int i) const
{
	//Indexes
	unsigned int l_index = left_child_index(i);
	unsigned int r_index = right_child_index(i);

	//Tree only has one child
	if (r_index >= _size) return l_index;

	//Items
	T l_item = _heapArr[l_index];
	T r_item = _heapArr[r_index];

	if (_maxHeap) 
		if (l_item > r_item) return l_index;
		else return r_index;

	else
		if (l_item < r_item) return l_index;
		else return r_index;

}

template<typename T>
void Heap<T>::swap_with_parent(unsigned int i)
{
	//Set Index's
	unsigned int p_index = parent_index(i);
	unsigned int c_index = i;
	
	//Items
	T p_item = _heapArr[p_index];
	T c_item = _heapArr[c_index];

	while (true) {

		if (_maxHeap) {
			if (p_item >= c_item) break;
		}
		else {
			if (p_item <= c_item) break;
		}

		//Swap
		_heapArr[c_index] = p_item;
		_heapArr[p_index] = c_item;

		//Update Child Index
		c_index = p_index;

		//Get New Parent 
		p_index = parent_index(p_index);
		p_item = _heapArr[p_index];

	}
}

template<typename T>
void Heap<T>::swap_with_children(unsigned int i)
{
	//Set Index's
	unsigned int p_index = i;
	unsigned int c_index = big_small_child_index(i);

	//Items
	T p_item = _heapArr[p_index];
	T c_item = _heapArr[c_index];

	while (true) {
		if (_maxHeap) {
			if (p_item >= c_item) break;
		}
		else {
			if (p_item <= c_item) break;
		}

		//Swap
		_heapArr[c_index] = p_item;
		_heapArr[p_index] = c_item;

		//Update Parent Index
		p_index = c_index;

		//Get New Children
		c_index = big_small_child_index(p_index);

		//Check for valid child 
		if (c_index > _size) break;
		else c_item = _heapArr[c_index];
	}
}

template<typename T>
void Heap<T>::print_tree
(unsigned int root, int level, ostream& outs) const
{
	if (root < _size) {

		//Go to the end of the right tree
		print_tree(right_child_index(root), level + 1, outs);

		int width = static_cast<int>(4 * level);

		//Print Item
		outs << setw(width) 
			<< "[" << _heapArr[root] << "]" << endl;

		//Go to the end of the left tree
		//After going to the right
		print_tree(left_child_index(root), level + 1, outs);
	}

}

template<typename U>
ostream& operator<<(ostream& outs, const Heap<U>& print_me)
{
	print_me.print_tree(outs);

	return outs;
}

#endif //MYHEAP_H