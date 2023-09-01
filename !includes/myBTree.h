#ifndef MYBTREE_H
#define MYBTREE_H

#include <iostream>
#include <iomanip>
#include "Array_Utility_Functions.h"
using namespace std;

template <typename T>
class BTree {
public:

	//Default Constructor
	BTree(bool dups_ok = false);

	//Big Three:
	BTree(const BTree<T>& other);
	~BTree();
	BTree<T>& operator =(const BTree<T>& rhs);

	//clear this object (delete all nodes etc.)
	void clear();

	//copy other into this object
	void copy(const BTree<T>& other);

	//Return Pointer to Item in tree
	T* find(const T& entry);

	//Returns true if item in tree
	bool contains(const T& entry) const;

	//Inserts item if item not in tree
	T& get(const T& entry);

	//Asserts if item not found
	//Returns reference to item
	T& get_existing(const T& entry);

	//insert entry item into the tree
	void insert(const T& entry);

	//remove entry item into the tree
	void remove(const T& entry);

	int get_size() const { return _size; }

	bool is_valid();

	bool is_empty() const { return _size == 0; }

	BTree<T>& operator +=(const T& item);

	//OStream Operators
	template <typename U> //Given Object
	friend ostream& operator<<(ostream& outs,
		const BTree<U>& print_me);
	template <typename U> //Given Ptr
	friend ostream& operator<<(ostream& outs,
		const BTree<U>* print_me);
	void print_inorder() const;

private:
	static const int MINIMUM = 1;
	static const int MAXIMUM = 2 * MINIMUM;

	//Returns true if node is a leaf
	bool isLeaf() const;

	//Allows MAXIMUM+1 data elements in the root
	bool loose_insert(const T& entry);

	//Fix excess of data elements in child i
	void fix_excess(int i);

	//remove element functions:

	//allows MINIMUM-1 data elements in the root
	bool loose_remove(const T& entry);

	//fix shortage of data elements in child i
	void fix_shortage(int i);

	//remove the biggest child of this tree->entry
	void remove_biggest(T& entry);

	//transfer one element LEFT from child i
	void rotate_left(int i);

	//transfer one element RIGHT from child i
	void rotate_right(int i);

	//merge subset i with subset i+1
	void merge_with_next_subset(int i);

	void get_inorder(T* arr, int& size) const;

	//print a readable version of the tree
	void print_tree(int level = 0, ostream& outs = cout) const;

	T _data[MAXIMUM + 1];
	BTree<T>* _subset[MAXIMUM + 2];
	int _data_count;
	int _child_count;
	int _size;
	bool _dups_ok;
	bool _debug;
};

template<typename T>
BTree<T>::BTree(bool dups_ok) :
	_data(), _subset(), _size(0),
	_data_count(0), _child_count(0),
	_dups_ok(dups_ok), _debug(false) {

	if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
BTree<T>::BTree(const BTree<T>& other) :
	_data(), _subset(), _size(0),
	_data_count(0), _child_count(0),
	_dups_ok(_dups_ok), _debug(other._debug) {

	//Clear and copy stuff from other
	clear();
	copy(other);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

template<typename T>
BTree<T>::~BTree() {

	clear();
	if (_debug) cout << "Destructor Executed" << endl;
}

template<typename T>
BTree<T>& BTree<T>::operator=(const BTree<T>& rhs) {

	//Check for self-reference
	if (this == &rhs) return *this;

	//Clear and copy stuff from rhs
	this->clear();
	this->copy(rhs);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}


template<typename T>
void BTree<T>::clear() {
	if (!isLeaf()) {

		//Clear Children
		for (int i = 0; i < _child_count; i++) {
			_subset[i]->clear();
			delete _subset[i];
			_subset[i] = nullptr;
		}

		//Set Counters to 0
		_data_count = 0;
		_child_count = 0;
	}

	//Set Counters to 0
	_data_count = 0;
	_child_count = 0;
	_size = 0;
}

template<typename T>
void BTree<T>::copy(const BTree<T>& other) {

	//Copy data to tree
	copy_array(_data, other._data,
		_data_count, other._data_count);
	_size = other._size; _debug = other._debug;

	//If I have no kids but other has kids
	if (isLeaf()) {

		//Go through other's children
		for (int i = 0; i < other._child_count; i++) {

			//Make a kid for me
			BTree<T>* bt = new BTree<T>(other._dups_ok);
			bt->_size = other._size; bt->_debug = other._debug;

			//Check if this kid has kids and get its kids
			bt->copy(*other._subset[i]);

			//add/connect this kid to its parent
			attach_item(_subset, _child_count, bt);
		}
	}

}

template<typename T>
T* BTree<T>::find(const T& entry) {
	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	if (found) return &_data[i];

	if (found == false && isLeaf()) return nullptr;

	//go to tree containing entry
	return _subset[i]->find(entry);
}

template<typename T>
bool BTree<T>::contains(const T& entry) const {
	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	if (found) return true;

	if (found == false && isLeaf()) return false;

	//go to tree containing entry
	return _subset[i]->contains(entry);
}

template<typename T>
T& BTree<T>::get(const T& entry)
{
	T* item = find(entry);

	//Insert item if not in tree
	if (item == nullptr) insert(entry);

	else return *item;

	//Get item after insert
	return get_existing(entry);
}

template<typename T>
T& BTree<T>::get_existing(const T& entry)
{
	T* item = find(entry);

	//Item not in tree
	assert(item != nullptr);

	return *item;
}

template<typename T>
void BTree<T>::insert(const T& entry) {

	if (loose_insert(entry)) _size++;

	//Fix main root
	if (_data_count > MAXIMUM) {

		//Create Child version of main root
		BTree<T>* child = new BTree<T>(_dups_ok);

		//Copy root data and subset to child
		copy_array(child->_data, _data,
			child->_data_count, _data_count);
		copy_array(child->_subset, _subset,
			child->_child_count, _child_count);

		//Clear out root and point to child
		_data_count = 0;
		_child_count = 1;
		_subset[0] = child;

		//Fix my child
		fix_excess(0);
	}
}

template<typename T>
void BTree<T>::remove(const T& entry) {
	if (loose_remove(entry)) _size--;

	//Fix main root
	if (_data_count < MINIMUM && _data_count >= 0) {

		//1. Hold onto the child w/a shrink_ptr
		BTree<T>* shrink_ptr = _subset[0];

		//2. Shallow Copy shrink_ptr into the root
		copy_array(_data, shrink_ptr->_data,
			_data_count, shrink_ptr->_data_count);
		copy_array(_subset, shrink_ptr->_subset,
			_child_count, shrink_ptr->_child_count);

		//3. Set child and data count of shrink_ptr to zero
		shrink_ptr->_child_count = 0;
		shrink_ptr->_data_count = 0;

		//4. Delete shrink_ptr
		if (_child_count > 0) delete shrink_ptr;
	}
}

template<typename T>
bool BTree<T>::is_valid()
{
	T* arr = new T[_size];
	int size = _size - 1; get_inorder(arr, size);

	T prev = arr[0];

	//Go through tree inorder
	for (int i = 1; i < _size; i++) {

		//If prev item is greater than current item
		//Tree is not valid
		if (prev > arr[i]) return false;

		//Set prev element
		prev = arr[i];
	}

	delete[] arr;

	//Tree is valid
	return true;
}

template<typename T>
BTree<T>& BTree<T>::operator+=(const T& item) {
	insert(item);
	return *this;
}

template<typename T>
bool BTree<T>::isLeaf() const
{
	return (_child_count == 0);
}

template<typename T>
bool BTree<T>::loose_insert(const T& entry)
{
	int i = first_ge(_data, _data_count, entry);

	bool found = (i < _data_count && entry == _data[i]);
	bool inserted = false;

	//Update Duplicate
	if (found && _dups_ok) {
		_data[i] = entry;
		return false;
	}

	//Item already in Tree, Do nothing
	else if (found && !_dups_ok) {
		return false;
	}

	//Insert item to data array
	if (isLeaf()) {
		ordered_insert(_data, _data_count, entry);
		inserted = true;
	}
	else {
		inserted = _subset[i]->loose_insert(entry);

		//Fix child if necessary
		if (_subset[i]->_data_count > MAXIMUM) {
			fix_excess(i);
		}
	}

	return inserted;
}

template<typename T>
void BTree<T>::fix_excess(int i) {

	//Add tree to its rightful spot
	insert_item(_subset, i + 1, _child_count,
		new BTree<T>(_dups_ok));

	//Move data from subset to new subset
	split(_subset[i]->_data, _subset[i]->_data_count,
		_subset[i + 1]->_data, _subset[i + 1]->_data_count);

	//Move _subset ptrs from one subset to new subset
	split(_subset[i]->_subset, _subset[i]->_child_count,
		_subset[i + 1]->_subset, _subset[i + 1]->_child_count);

	//Detach item from splitted array
	T entry;
	detach_item(_subset[i]->_data,
		_subset[i]->_data_count, entry);

	//Insert Removed Item to root
	ordered_insert(_data, _data_count, entry);
}

template<typename T>
bool BTree<T>::loose_remove(const T& entry) {
	bool debug = false;
	int i = first_ge(_data, _data_count, entry);
	T item = entry;
	bool found = (i < _data_count && entry == _data[i]);

	//leaf && found target: just remove the target
	if (isLeaf() && found) {
		if (debug) cout << "isLeaf: Item in tree removed" << endl;
		delete_item(_data, i, _data_count, item);

		return true;
	}

	//leaf && not found target: there is nothing to do
	else if (isLeaf() && !found) {
		if (debug) cout << "Item not in tree" << endl;
		return false;
	}

	//not leaf and found: replace target with largest child of subset[i]
	else if (!isLeaf() && found) {
		if (debug)
			cout << "Not Leaf: Replacing target" <<
			"with largest child" << endl;
		_subset[i]->remove_biggest(item);
		_data[i] = item; //Replace Target with large child

		//Fix child if necessary
		if (_subset[i]->_data_count < MINIMUM) {
			fix_shortage(i);
		}

		return true;
	}

	//not leaf and not found target: recursive call to loose_remove
	else if (!isLeaf() && !found) {
		bool removed = _subset[i]->loose_remove(entry);

		//Fix child if necessary
		if (_subset[i]->_data_count < MINIMUM) {
			fix_shortage(i);
		}

		return removed;
	}
}

template<typename T>
void BTree<T>::fix_shortage(int i) {

	bool debug = false;

	if (i + 1 < _child_count && _subset[i + 1]->_data_count > MINIMUM) {
		if (debug) cout << "Rotate Left" << endl;
		rotate_left(i);
	}
	else if (i > 0 && _subset[i - 1]->_data_count > MINIMUM) {
		if (debug) cout << "Rotate Right" << endl;
		rotate_right(i);
	}
	else if (i + 1 < _child_count &&
		_subset[i + 1]->_data_count == MINIMUM) {
		if (debug) cout << "Merge with Right " << endl;
		merge_with_next_subset(i);
	}
	else if (i > 0 && _subset[i - 1]->_data_count == MINIMUM) {
		if (debug) cout << "Merge with Left" << endl;
		merge_with_next_subset(i - 1);
	}
}

template<typename T>
void BTree<T>::remove_biggest(T& entry) {
	// Keep looking in the last subtree (recursive)
	// until you get to a leaf.
	// Then, detach the last (biggest) data item
	// after the recursive call, fix shortage.

	bool debug = false;

	if (!isLeaf()) {
		int i = _child_count - 1;
		_subset[i]->remove_biggest(entry);


		//Fix child if necessary
		if (_subset[i]->_data_count < MINIMUM) {
			fix_shortage(i);
		}
	}

	if (isLeaf()) {

		if (debug) {
			int i = index_of_maximal(_data, _data_count);
			cout << "MAX: " << _data[i] << endl;
		}

		detach_item(_data, _data_count, entry);
	}

}

template<typename T>
void BTree<T>::rotate_left(int i) {

	bool debug = false;

	//Delete and store item in root
	T item;
	delete_item(_data, i, _data_count, item);

	//Add deleted item to child i
	attach_item(_subset[i]->_data, _subset[i]->_data_count, item);

	//Delete and store first item in child i + 1
	delete_item(_subset[i + 1]->_data, 0,
		_subset[i + 1]->_data_count, item);

	//Add deleted item to main root
	ordered_insert(_data, _data_count, item);

	//Transfer Children
	if (!_subset[i]->isLeaf()) {
		if (debug) cout << "ROTATE LEFT TRANSFER KIDS" << endl;

		//remove first subset from right child
		BTree* temp_ptr = nullptr;
		delete_item(_subset[i + 1]->_subset, 0,
			_subset[i + 1]->_child_count, temp_ptr);

		//Attach removed subset to child i
		attach_item(_subset[i]->_subset,
			_subset[i]->_child_count, temp_ptr);
	}
}

template<typename T>
void BTree<T>::rotate_right(int i) {

	bool debug = false;

	//Delete and store item in root
	T item;
	delete_item(_data, i - 1, _data_count, item);

	//Add deleted item to child i
	attach_item(_subset[i]->_data,
		_subset[i]->_data_count, item);

	//Remove last item from child i-1
	detach_item(_subset[i - 1]->_data,
		_subset[i - 1]->_data_count, item);

	//Add deleted item to main root
	ordered_insert(_data, _data_count, item);

	//Transfer Children
	if (!_subset[i]->isLeaf()) {

		if (debug) cout << "ROTATE RIGHT TRANSFER KIDS" << endl;

		BTree* temp_ptr = nullptr;

		//remove last subset from left child
		delete_item(_subset[i - 1]->_subset,
			_subset[i - 1]->_child_count - 1,
			_subset[i - 1]->_child_count, temp_ptr);

		//Insert removed subset to child i first index
		insert_item(_subset[i]->_subset, 0,
			_subset[i]->_child_count, temp_ptr);
	}
}

template<typename T>
void BTree<T>::merge_with_next_subset(int i) {

	T item; delete_item(_data, i, _data_count, item);

	//append it to child[i]->data
	attach_item(_subset[i]->_data, _subset[i]->_data_count, item);

	//Move all data items from subset[i+1]->data to subset[i]->data
	merge(_subset[i]->_data, _subset[i]->_data_count,
		_subset[i + 1]->_data, _subset[i + 1]->_data_count);

	//Move all subset pointers from subset[i+1]->subset to subset[i]->subset
	merge(_subset[i]->_subset, _subset[i]->_child_count,
		_subset[i + 1]->_subset, _subset[i + 1]->_child_count);

	//store in a temp_ptr
	BTree* temp_ptr = nullptr;

	//Remove subset from array and decrease childcount
	delete_item(_subset, i + 1, _child_count, temp_ptr);

	//Delete temp ptr
	temp_ptr->_child_count = 0;
	delete temp_ptr;
}

template<typename T>
void BTree<T>::get_inorder(T* arr, int& size) const
{
	//Go to the Last subset
	if (!isLeaf()) {
		_subset[_child_count - 1]->get_inorder(arr, size);
	}

	//Go through data from the end to the start
	for (int i = _data_count - 1; i >= 0; i--) {

		//Add item to array backwards
		arr[size] = _data[i];
		size--;

		//Go Through subset
		if (!isLeaf()) {
			_subset[i]->get_inorder(arr, size);
		}

	}
}

template<typename T>
void BTree<T>::print_tree(int level, ostream& outs) const {

	//Print the Last subset
	if (!isLeaf()) {
		_subset[_child_count - 1]->print_tree(level + 1, outs);
	}

	//Go through data from the end to the start
	for (int i = _data_count - 1; i >= 0; i--) {

		//Print data
		int width = static_cast<int>(4 * level);
		outs << setw(width) << "[" << _data[i] << "]" << endl;

		//Go Through subset
		if (!isLeaf()) {
			_subset[i]->print_tree(level + 1, outs);
		}

	}
}

template<typename T>
void BTree<T>::print_inorder() const
{
	//Go through data from the end to the start
	for (int i = 0; i < _data_count; i++) {

		//Go Through subset
		if (!isLeaf()) {
			_subset[i]->print_inorder();
		}

		//Print data
		cout << " " << _data[i];

	}

	//Print the Last subset
	if (!isLeaf()) {
		_subset[_child_count - 1]->print_inorder();
	}
}

template<typename U>
ostream& operator<<(ostream& outs, const BTree<U>& print_me) {
	print_me.print_tree(0, outs);
	return outs;
}

template<typename U>
ostream& operator<<(ostream& outs, const BTree<U>* print_me) {
	print_me->print_tree(0, outs);
	return outs;
}

#endif //MYBTREE_H