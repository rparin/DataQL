#ifndef MYBPLUSTREE_H
#define MYBPLUSTREE_H

#include <iostream>
#include <iomanip>
#include "Array_Utility_Functions.h"
using namespace std;

template <typename T>
class BPlusTree {
public:
	class Iterator {
	public:
		friend class BPlusTree;
		Iterator(const BPlusTree<T>* _it = nullptr, int key_ptr = 0) :
			_it(_it), _key_ptr(key_ptr) {}

		const T operator *() const {
			assert(_it); //_it can't be nullptr
			assert(_key_ptr < _it->_data_count);
			return _it->_data[_key_ptr];
		}

		T operator *() {
			assert(_it); //_it can't be nullptr
			assert(_key_ptr < _it->_data_count);
			return _it->_data[_key_ptr];
		}

		Iterator operator++() {
			assert(_it); //_it can't be nullptr
			_key_ptr++;

			//Go to next node and reset key
			if (_key_ptr >= _it->_data_count) {
				_it = _it->_next;
				_key_ptr = 0;
			}

			return *this;
		}

		friend Iterator operator++(Iterator& it, int unused) {
			assert(it.is_null() == false);
			Iterator hold = it;
			++it;
			return hold;
		}

		friend bool operator ==(const Iterator& lhs, 
			const Iterator& rhs) {
			return (lhs._it == rhs._it);
		}

		friend bool operator !=(const Iterator& lhs, 
			const Iterator& rhs) {
			return (lhs._it != rhs._it);
		}

		void print_Iterator() {
			if(_it) cout << *_it;
		}

		bool is_null() {
			return (_it == nullptr);
		}
	private:
		const BPlusTree<T>* _it;
		int _key_ptr;
	};

	//Default Constructor
	BPlusTree(bool dups_ok = true);

	//Big Three:
	BPlusTree(const BPlusTree<T>& other);
	~BPlusTree();
	BPlusTree<T>& operator =(const BPlusTree<T>& rhs);

	//clear this object (delete all nodes etc.)
	void clear();

	//copy other into this object
	void copy(const BPlusTree<T>& other);

	//Returns true if item in tree
	bool contains(const T& entry) const;

	//Inserts item if item not in tree
	T& get(const T& entry);

	//return a reference to entry in the tree
	const T& get(const T& entry) const;          

	//Asserts if item not found
	//Returns reference to item
	T& get_existing(const T& entry);

	//insert entry item into the tree
	void insert(const T& entry);
	BPlusTree<T>& operator +=(const T& item);

	//remove entry item into the tree
	void remove(const T& entry);

	int get_size() const { return _size; }

	//bool is_valid();

	bool is_empty() const { return (_size == 0); }

	//Prints every item in list in order
	void print_inorder() const;

	//OStream Operators
	template <typename U> //Given Object
	friend ostream& operator<<(ostream& outs,
		const BPlusTree<U>& print_me);
	template <typename U> //Given Ptr
	friend ostream& operator<<(ostream& outs,
		const BPlusTree<U>* print_me);

	//Iterator stuff
	Iterator begin() const;
	Iterator end() const;
	
	//return an iterator to this key. NULL if not there.
	Iterator find(const T& entry);             

	//return first that goes AFTER key (>)
	Iterator upper_bound(const T& key); 

	//return first that goes NOT BEFORE (>=)
	Iterator lower_bound(const T& key);

private:
	static const int MINIMUM = 1;
	static const int MAXIMUM = 2 * MINIMUM;

	//print a readable version of the tree
	void print_tree(int level = 0, ostream& outs = cout) const;

	//copy other into this object
	void copy(const BPlusTree<T>& other, BPlusTree*& last_node);

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
	BPlusTree<T>* fix_shortage(int i);

	//remove the biggest child of this tree->entry
	void remove_biggest(T& entry);

	//transfer one element LEFT from child i
	void rotate_left(int i);

	//transfer one element RIGHT from child i
	void rotate_right(int i);

	//merge subset i with subset i+1
	BPlusTree<T>* merge_with_next_subset(int i);

	//Return Pointer to Item in tree
	T* find_ptr(const T& entry);

	//Chain List Functions:

	//Pointer to smallest item in tree
	const BPlusTree<T>* get_smallest_node() const;

	//Pointer to largest item in tree
	const BPlusTree<T>* get_biggest_node() const;

	//Sets entry = leftmost leaf
	void get_smallest(T& entry);        

	//Sets entry = rightmost leaf
	void get_biggest(T& entry);

	T _data[MAXIMUM + 1];
	BPlusTree<T>* _subset[MAXIMUM + 2];
	BPlusTree* _next;
	int _data_count;
	int _child_count;
	int _size;
	bool _dups_ok;
	bool _debug;
};

template<typename T>
BPlusTree<T>::BPlusTree(bool dups_ok):
	_data(), _subset(),_size(0), _next(nullptr),
	_data_count(0), _child_count(0),
	_dups_ok(dups_ok),_debug(false) {

	if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other) :
	_data(), _subset(), _next(nullptr), _size(0),
	_data_count(0), _child_count(0),
	_dups_ok(_dups_ok), _debug(other._debug) {

	//Clear and copy stuff from other
	clear();
	copy(other);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

template<typename T>
BPlusTree<T>::~BPlusTree() {

	clear();
	if (_debug) cout << "Destructor Executed" << endl;
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator=(const BPlusTree<T>& rhs) {

	//Check for self-reference
	if (this == &rhs) return *this;

	//Clear and copy stuff from rhs
	clear();
	copy(rhs);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename T>
void BPlusTree<T>::clear() {

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
	_next = nullptr;
	_size = 0;
}

template<typename T>
void BPlusTree<T>::copy(const BPlusTree<T>& other) {
	clear(); //Tree must be empty
	BPlusTree* temp = nullptr;
	copy(other, temp);
}

template<typename T>
void BPlusTree<T>::copy(const BPlusTree<T>& other, 
BPlusTree*& last_node) {

	//Copy data to tree
	copy_array(_data, other._data, _data_count, other._data_count);
	_size = other._size; _debug = other._debug;

	//Go through other's children
	for (int i = 0; i < other._child_count; i++) {

		//Make a kid for me, copy other's kids stuff
		BPlusTree<T>* bt = new BPlusTree<T>(other._dups_ok);
		bt->_size = other._size; bt->_debug = other._debug;

		//Check if this kid has kids and get its kids
		bt->copy(*other._subset[i], last_node);

		//add/connect this kid to its parent
		attach_item(_subset, _child_count, bt);

		//Check if the child I made is a leaf
		if (bt->isLeaf()) {

			//Last node is null, thus I am the left most node
			if (last_node == nullptr) last_node = bt;

			//Set last node's next to point to my child and
			//last node itself to point ot my child
			else {
				last_node->_next = bt;
				last_node = bt;
			}
		}
	}
}

template<typename T>
bool BPlusTree<T>::contains(const T& entry) const{

	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	//Item found
	if (found && isLeaf()) return true;

	//go to bottom of tree containing entry
	if (found && !isLeaf()) return _subset[i + 1]->contains(entry);

	//Item Not found
	if (!found && isLeaf()) return false;

	//go to tree containing entry
	if (!found && !isLeaf()) return _subset[i]->contains(entry);
}

template<typename T>
T& BPlusTree<T>::get(const T& entry)
{
	T* item = find_ptr(entry);

	//Insert item if not in tree
	if (item == nullptr) insert(entry);

	else return *item;
	
	//Get item after insert
	return get_existing(entry);
}

template<typename T>
const T& BPlusTree<T>::get(const T& entry) const {

	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	//Return item
	if (found && isLeaf()) return _data[i];

	//go to bottom of tree containing entry
	if (found && !isLeaf()) return _subset[i + 1]->get(entry);

	//Item Not found
	if (!found && isLeaf()) assert(found == true);

	//go to tree containing entry
	if (!found && !isLeaf()) return _subset[i]->get(entry);
}

template<typename T>
T& BPlusTree<T>::get_existing(const T& entry) {
	T* item = find_ptr(entry);

	//Item not in tree
	assert(item != nullptr);

	return *item;
}

template<typename T>
void BPlusTree<T>::insert(const T& entry) {

	if (loose_insert(entry)) _size++;

	//Fix main root
	if (_data_count > MAXIMUM) {

		//Create Child version of main root
		BPlusTree<T>* child = new BPlusTree<T>(_dups_ok);

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
BPlusTree<T>& BPlusTree<T>::operator+=(const T& item) {
	insert(item);
	return *this;
}

template<typename T>
void BPlusTree<T>::remove(const T& entry) {
	if (loose_remove(entry)) _size--;

	//Fix main root
	if (_data_count < MINIMUM) {

		//1. Hold onto the child w/a shrink_ptr
		BPlusTree<T>* shrink_ptr = _subset[0];

		//2. Shallow Copy shrink_ptr into the root
		copy_array(_data, shrink_ptr->_data, 
			_data_count, shrink_ptr->_data_count);
		copy_array(_subset, shrink_ptr->_subset, 
			_child_count, shrink_ptr->_child_count);

		//3. Set child and data count of shrink_ptr to zero
		shrink_ptr->_child_count = 0;
		shrink_ptr->_data_count = 0;

		//4. Delete shrink_ptr
		if(_child_count > 0) delete shrink_ptr;
	}
}

template<typename T>
void BPlusTree<T>::print_inorder() const{

	const BPlusTree* next = get_smallest_node();

	//Go through _next ptrs
	while (next) {

		for (int i = 0; i < next->_data_count; i++) {
			cout << next->_data[i] << " ";
		}
		next = next->_next;
	}
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::
begin() const {
	if(is_empty()) return Iterator(nullptr);
	else return Iterator(get_smallest_node());
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::
end() const{
	return Iterator(nullptr);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::
find(const T& entry) {
	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	//Return item
	if (found && isLeaf()) return Iterator(this, i);

	//go to bottom of tree containing entry
	if (found && !isLeaf()) return _subset[i + 1]->find(entry);

	//Item Not found
	if (!found && isLeaf()) return Iterator(nullptr);

	//go to tree containing entry
	if (!found && !isLeaf()) return _subset[i]->find(entry);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::
upper_bound(const T& entry) {
	//Get index of where entry should be
	int i = first_ge(_data,_data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	//Return item
	if (found && isLeaf()) {
		if(i < MAXIMUM-MINIMUM && this->_next == nullptr)
			return Iterator(this, i+1);
		else return Iterator(this->_next, i);
	}

	//go to bottom of tree containing entry
	if (found && !isLeaf()) return _subset[i + 1]->upper_bound(entry);

	//Item Not found
	if (!found && isLeaf()) {
		//Entry Below first node
		if (i - 1 < 0 || _data[i - 1] > entry) 
			return Iterator(this, i);

		//Item > Entry is in last node
		if(this->_next == nullptr && 
			_data_count < MAXIMUM) return Iterator(this, i);

		//Return Item > Entry is in Next Node
		return Iterator(this->_next);
	}

	//go to tree containing entry
	if (!found && !isLeaf()) return _subset[i]->upper_bound(entry);
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::
lower_bound(const T& entry) {
	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	//Return item
	if (found && isLeaf()) return Iterator(this, i);

	//go to bottom of tree containing entry
	if (found && !isLeaf()) return _subset[i + 1]->lower_bound(entry);

	//Item Not found
	if (!found && isLeaf()) {

		//Entry Below first node
		if( i - 1 <  0 || _data[i-1] > entry) 
			return Iterator(this, i);

		//Item >= Entry Node DNE
		if (this->_next == nullptr &&
			i < MINIMUM) return Iterator(this, i);

		//Item >= Entry is in next node
		return Iterator(this->_next);
	}

	//go to tree containing entry
	if (!found && !isLeaf()) return _subset[i]->lower_bound(entry);
}

template<typename T>
bool BPlusTree<T>::isLeaf() const
{
	return (_child_count == 0);
}

template<typename T>
bool BPlusTree<T>::loose_insert(const T& entry) {

	int i = first_ge(_data, _data_count, entry);

	bool found = (i < _data_count  && entry == _data[i]);

	bool inserted = false;

	//Its a duplicate
	if (found && isLeaf()) {

		//Update duplicate
		if (_dups_ok) _data[i] += entry;
	}

	//Go to end of tree containing item
	else if (found && !isLeaf()) {

		//Update item at the end of tree
		if (_dups_ok) _data[i] += entry;

		inserted = _subset[i + 1]->loose_insert(entry);

		//Fix child if necessary
		if (_subset[i]->_data_count > MAXIMUM) {
			fix_excess(i);
		}
	}

	//Insert here
	else if (!found && isLeaf()) {
		ordered_insert(_data, _data_count, entry);
		inserted = true;
	}

	//Go follow bread crumbs
	else if (!found && !isLeaf()) {
		inserted = _subset[i]->loose_insert(entry);

		//Fix child if necessary
		if (_subset[i]->_data_count > MAXIMUM) {
			fix_excess(i);
		}
	}

	//Return whether item inserted 
	//or updated duplicate
	return inserted;
}

template<typename T>
T* BPlusTree<T>::find_ptr(const T& entry) {

	bool debug = false;

	//Get index of where entry should be
	int i = first_ge(_data, _data_count, entry);

	//check if entry found
	bool found = (i < _data_count && entry == _data[i]);

	//Return item
	if (found && isLeaf()) {
		if (debug) print_array(_data, _data_count);
		return &_data[i];
	}

	//go to bottom of tree containing entry
	if (found && !isLeaf()) return _subset[i + 1]->find_ptr(entry);

	//Item Not found
	if (!found && isLeaf()) return nullptr;

	//go to tree containing entry
	if (!found && !isLeaf()) return _subset[i]->find_ptr(entry);
}

template<typename T>
const BPlusTree<T>* BPlusTree<T>::get_smallest_node() const{
	
	//If Children has no children 
	//return ptr to children
	if (isLeaf()) {
		return this;
	}

	//Go To Left Child
	if (!isLeaf()) {
		return _subset[0]->get_smallest_node();
	}
}

template<typename T>
const BPlusTree<T>* BPlusTree<T>::get_biggest_node() const{

	//If Children has no children 
	//return ptr to children
	if (isLeaf()) {
		return this;
	}

	//Got to right child
	if (!isLeaf()) {
		return _subset[_child_count - 1]->get_biggest_node();
	}
}

template<typename T>
void BPlusTree<T>::get_smallest(T& entry) {
	//replace entry with the left_most leaf of this tree
	// Keep looking in the first subtree (recursive)
	// until you get to a leaf.
	// Then, replace entry with data[0] of the leaf node

	const BPlusTree<T>* temp = get_smallest_node();
	entry = temp->_data[0];
}

template<typename T>
void BPlusTree<T>::get_biggest(T& entry) {

	const BPlusTree<T>* temp = get_biggest_node();
	entry = temp->_data[temp->_data_count-1];
}

template<typename T>
void BPlusTree<T>::fix_excess(int i) {
	
	//Add tree to its rightful spot
	BPlusTree<T>* newTree = new BPlusTree<T>(_dups_ok);
	insert_item(_subset, i + 1, _child_count, newTree);

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

	//BPlusTree Code
	//Add item to bottom of tree
	if (_subset[i + 1]->isLeaf()) {
		ordered_insert(_subset[i+1]->_data, 
			_subset[i+1]->_data_count, entry);
	}

	//BTree Code -Insert Removed Item to root
	ordered_insert(_data, _data_count, entry);

	//BPlusTree Code
	if (_subset[i]->isLeaf()) {

		//Set i+1's next to subset i's next
		_subset[i + 1]->_next = _subset[i]->_next;

		//Set i's next to the newTree
		_subset[i]->_next = newTree;
	}

}

template<typename T>
bool BPlusTree<T>::loose_remove(const T& entry) {
	bool debug = false;
	int i = first_ge(_data, _data_count, entry);
	T item = entry;
	bool found = (i < _data_count && entry == _data[i]);

	//leaf && found target: just remove the target
	if (found && isLeaf()) {
		if (debug) cout << "isLeaf: Item in tree removed" << endl;
		delete_item(_data, i, _data_count, item);

		return true;
	}

	//leaf && not found target: there is nothing to do
	else if (isLeaf() && !found) {
		if(debug) cout << "Item not in tree" << endl;
		return false;
	}

	//not leaf and found:
	else if (!isLeaf() && found) {
		_subset[i + 1]->loose_remove(entry);

		//Fix child if necessary
		if (_subset[i+1]->_data_count < MINIMUM) {
			BPlusTree<T>*  temp = fix_shortage(i + 1);

			//Look in returned subset data for breadcrumb
			if (temp->_child_count > 0) {
				temp->get_biggest(item);
				for (int i = 0; i < temp->_data_count; i++) {
					if (temp->_data[i] == entry) {
						temp->_data[i] = item;
						return true;
					}
				}
			}
			
		}

		//Look in main root for Breadcrumb
		if (_child_count > i + 1) {
			_subset[i + 1]->get_smallest(item);
			for (int i = 0; i < _data_count; i++) {
				if (_data[i] == entry) {
					_data[i] = item;
					return true;
				}
			}
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
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i) {

	bool debug = true;
	if (i + 1 < _child_count && _subset[i+1]->_data_count > MINIMUM) {
		if(debug) cout << "Rotate Left" << endl;
		rotate_left(i);
		return _subset[i];
	}
	else if (i > 0 && _subset[i - 1]->_data_count > MINIMUM){
		if (debug) cout << "Rotate Right" << endl;
		rotate_right(i);
		return _subset[i];
	}
	else if (i + 1 < _child_count && 
		_subset[i + 1]->_data_count == MINIMUM) {
		if (debug) cout << "Merge with Right " << endl;
		return merge_with_next_subset(i);
	}
	else if(i > 0 && _subset[i - 1]->_data_count == MINIMUM){
		if (debug) cout << "Merge with Left" << endl;
		return merge_with_next_subset(i - 1);
	}
}

template<typename T>
void BPlusTree<T>::remove_biggest(T& entry) {
	// Keep looking in the last subTree (recursive)
	// until you get to a leaf.
	// Then, detach the last (biggest) data item
	// after the recursive call, fix shortage.

	bool debug = false;

	if (!isLeaf()) {
		int i = _child_count-1;
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
void BPlusTree<T>::rotate_left(int i) {

	//BPT Code
	if (_subset[i]->isLeaf()) {
		T item;
		delete_item(_subset[i + 1]->_data, 0, _subset[i + 1]->_data_count, item);

		//Add deleted item to child i
		attach_item(_subset[i]->_data, _subset[i]->_data_count, item);

		//cout << _data[i] << endl;
		_data[i] = _subset[i + 1]->_data[0];

	}

	//BTree Code
	else {
		bool debug = true;

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
			BPlusTree* temp_ptr = nullptr;
			delete_item(_subset[i + 1]->_subset, 0,
				_subset[i + 1]->_child_count, temp_ptr);

			//Attach removed subset to child i
			attach_item(_subset[i]->_subset,
				_subset[i]->_child_count, temp_ptr);
		}
	}

}

template<typename T>
void BPlusTree<T>::rotate_right(int i) {

	//BPT Code
	if (_subset[i]->isLeaf()) {
		//Transfer subset[i] data item to the front of subset[i+1]
		T last;
		delete_item(_subset[i - 1]->_data, i, 
			_subset[i - 1]->_data_count, last);

		BPlusTree<T>* bt = new BPlusTree<T>(_dups_ok); bt->insert(last);
		insert_item(_subset, _child_count, _child_count, bt);

		if (last == 0) _subset[i - 1]->_next = nullptr;
		else {
			//Update Next
			_subset[i - 1]->_next = bt;

			//update data
			_data[i - 1] = last;
		}
	}

	//BT Code
	else {
		bool debug = true;

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

			BPlusTree* temp_ptr = nullptr;

			//remove last subset from left child
			delete_item(_subset[i - 1]->_subset,
				_subset[i - 1]->_child_count - 1,
				_subset[i - 1]->_child_count, temp_ptr);

			//Insert removed subset to child i first index
			insert_item(_subset[i]->_subset, 0,
				_subset[i]->_child_count, temp_ptr);

		}
	}
}

template<typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i) {

	//Delete data[i]
	T item; delete_item(_data, i, _data_count, item);

	//BPT Code
	if (!_subset[i]->isLeaf()) {
		attach_item(_subset[i]->_data, _subset[i]->_data_count, item);
	}

	//Move all data items from subset[i+1]->data to subset[i]->data
	merge(_subset[i]->_data, _subset[i]->_data_count,
		_subset[i + 1]->_data, _subset[i + 1]->_data_count);

	//Move all subset pointers from subset[i+1]->subset to subset[i]->subset
	merge(_subset[i]->_subset, _subset[i]->_child_count,
		_subset[i + 1]->_subset, _subset[i + 1]->_child_count);

	//store in a temp_ptr
	BPlusTree* temp_ptr = nullptr;

	//BPT Code
	if (_subset[i]->isLeaf()) {
		_subset[i]->_next = _subset[i+1]->_next;
	}

	//Remove subset from array and decrease childcount
	delete_item(_subset, i + 1, _child_count, temp_ptr);

	//Delete temp ptr
	temp_ptr->_child_count = 0;
	delete temp_ptr;

	return _subset[i];
}

template<typename T>
void BPlusTree<T>::print_tree(int level, ostream& outs) const {

	bool debug = true;

	//Print the Last subset
	if (!isLeaf()) {
		_subset[_child_count - 1]->print_tree(level + 1, outs);
	}

	//Go through data from the end to the start
	for (int i = _data_count - 1; i >= 0; i--) {

		//Print data
		int width = static_cast<int>(4 * level);
		outs << setw(width) << "[" << _data[i] << "]";

		if (debug) {
			if (_next == nullptr) outs << "=";
		}

		outs << endl;

		//Go Through subset
		if (!isLeaf()) {
			_subset[i]->print_tree(level + 1, outs);
		}

	}
}

template<typename U>
ostream& operator<<(ostream& outs, const BPlusTree<U>& print_me) {
	print_me.print_tree(0, outs);
	return outs;
}

template<typename U>
ostream& operator<<(ostream& outs, const BPlusTree<U>* print_me){
	print_me->print_tree(0, outs);
	return outs;
}

#endif //MYBPLUSTREE_H