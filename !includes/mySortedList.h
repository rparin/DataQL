#ifndef MYSORTEDLIST_H
#define MYSORTEDLIST_H

#include <iostream>
#include <cassert>
#include "linked_list.h"
using namespace std;

template <typename ITEM_TYPE>
class Sorted_List {
public:
	class Iterator {
	public:
		//give Sorted_List access to Iterator
		friend class Sorted_List;

		//default ctor
		Iterator() { _ptr = nullptr; }

		//Point Iterator to where p is pointing to
		Iterator(node<ITEM_TYPE>* p) { _ptr = p; }

		//dereference operator
		const ITEM_TYPE operator *() const {
			assert(_ptr); return _ptr->_item;}
		ITEM_TYPE& operator *() {
			assert(_ptr); return _ptr->_item;}
		
		//member access operator
		node<ITEM_TYPE>* operator ->() {
			assert(_ptr);
			return _ptr;
		}

		//true if _ptr is nullptr
		bool is_null() {
			if (_ptr == nullptr) return true;
			return false;
		}

		//true if left != right
		friend bool operator !=(const Iterator& left,
			const Iterator& right) {
			return (!(left._ptr == right._ptr));
		}

		//true if left == right
		friend bool operator ==(const Iterator& left, 
			const Iterator& right) {
			if (left._ptr == right._ptr) return true;
			return false;
		}

		//member operator: ++it
		//increment it then return incremented value
		Iterator& operator++() {
			assert(_ptr);
			_ptr = _ptr->_next;
			return *this;
		}

		//friend operator: it++
		//increment it then return the non incremented value
		friend Iterator operator++(Iterator& it, int unused) {
			assert(it.is_null() == false);
			Iterator hold = it;
			++it;
			return hold;
		}

	private:
		//pointer being encapsulated
		node<ITEM_TYPE>* _ptr;                         
	};

	//Default Constructor
	Sorted_List(bool order = true, bool unique = true);

	//Big Three
	~Sorted_List();
	Sorted_List(const Sorted_List<ITEM_TYPE>& copyThis);
	Sorted_List& operator =(const Sorted_List& RHS);

	//insert i at the head of Sorted_List
	Iterator insert(ITEM_TYPE i);

	//delete node pointed to by iMarker
	ITEM_TYPE remove(Iterator& mark);

	//Returns null if key not found
	Iterator search(Iterator& mark, const ITEM_TYPE& key);
	Iterator search(const ITEM_TYPE& key);
														
	//get the previous node to iMarker
	Iterator prev(Iterator& mark);

	//return the item at index
	const ITEM_TYPE& operator[](const int index) const;                   
	ITEM_TYPE& operator[](int index);

	//return the head of the Sorted_List                
	Iterator begin() const;

	//return the tail of the Sorted_List
	//node<ITEM_TYPE>* End() const; 
	Iterator end() const;

	//Returns the index of a node pointer given
	int index_of(Iterator& mark);

	//print the Sorted_List marking the pointer input
	void print() const;

	void clear();

	template <typename U>
	friend ostream& operator <<(ostream& outs, 
		const Sorted_List<U>& obj);

private:
	node<ITEM_TYPE>* _head;
	bool _debug;
	bool _order;
	bool _unique;
};

template<typename ITEM_TYPE>
Sorted_List<ITEM_TYPE>::Sorted_List(bool order, bool unique)
{
	_debug = false;
	_head = nullptr;
	_order = order;
	_unique = unique;
	if (_debug) cout << "Constructor Executed" << endl;
}

//Big Three: Destructor
template<typename ITEM_TYPE>
Sorted_List<ITEM_TYPE>::~Sorted_List()
{
	_clear_list(_head);
	if (_debug) cout << "Destructor Executed" << endl;
}

//Big Three: Copy Constructor
template<typename ITEM_TYPE>
Sorted_List<ITEM_TYPE>::Sorted_List
(const Sorted_List<ITEM_TYPE>& copyThis)
{

	//Copy Info from other object
	_debug = copyThis._debug;
	_order = copyThis._order;
	_unique = copyThis._unique;
	_head = nullptr;

	//Copy items
	_copy(copyThis._head, _head);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

//Big Three: Assignment
template<typename ITEM_TYPE>
Sorted_List<ITEM_TYPE>& Sorted_List<ITEM_TYPE>::
operator=(const Sorted_List<ITEM_TYPE>& rhs)
{
	//Check for self-reference
	if (this == &rhs) return *this;

	//Delete dynamic space if it exits already
	if (_head != nullptr) {
		_clear_list(_head);
	}

	//Copy Info from other object
	_debug = rhs._debug;
	_order = rhs._order;
	_unique = rhs._unique;
	_head = nullptr;

	//Copy items
	_copy(rhs._head, _head);
	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename ITEM_TYPE>
typename Sorted_List<ITEM_TYPE>::Iterator
Sorted_List<ITEM_TYPE>::insert(ITEM_TYPE i)
{
	node<ITEM_TYPE>* inserted;

	//For Integers
	//inserted = _insert_sorted(_head, i, _order, _unique);

	//For Poly
	if (_unique)
		inserted = _insert_sorted(_head, i, _order, _unique);
	else
		inserted = _insert_sorted_and_add(_head, i, _order);

	return Iterator(inserted);
}

template<typename ITEM_TYPE>
ITEM_TYPE Sorted_List<ITEM_TYPE>::
remove(Iterator& mark)
{
	return _delete_node(_head, mark._ptr);
}

template<typename ITEM_TYPE>
typename Sorted_List<ITEM_TYPE>::Iterator 
Sorted_List<ITEM_TYPE>::search(Iterator& mark, const ITEM_TYPE& key)
{
	if (mark._ptr)
		return Iterator(_search(mark._ptr, key));
	else
		return Iterator();
}

template<typename ITEM_TYPE>
typename Sorted_List<ITEM_TYPE>::Iterator
Sorted_List<ITEM_TYPE>::search(const ITEM_TYPE& key)
{
	return Iterator(_search(_head, key));
}

template<typename ITEM_TYPE>
void Sorted_List<ITEM_TYPE>::
print() const
{
	_print(_head);
}

template<typename ITEM_TYPE>
void Sorted_List<ITEM_TYPE>::clear()
{
	_clear_list(_head);
}

template<typename ITEM_TYPE>
typename Sorted_List<ITEM_TYPE>::Iterator 
Sorted_List<ITEM_TYPE>::prev(Iterator& mark)
{
	node<ITEM_TYPE>* prev = _previous_node(_head, mark._ptr);
	return Iterator(prev);
}

template<typename ITEM_TYPE>
const ITEM_TYPE& Sorted_List<ITEM_TYPE>::operator[]
(const int index) const
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
ITEM_TYPE& Sorted_List<ITEM_TYPE>::operator[](int index)
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
typename Sorted_List<ITEM_TYPE>::Iterator 
Sorted_List<ITEM_TYPE>::begin() const
{
	return Iterator(_head);
}

template<typename ITEM_TYPE>
typename Sorted_List<ITEM_TYPE>::Iterator 
Sorted_List<ITEM_TYPE>::end() const
{
	return Iterator(nullptr);
}

template<typename ITEM_TYPE>
int Sorted_List<ITEM_TYPE>::index_of(Iterator& mark)
{
	return _index_of(_head, mark._ptr);
}

template<typename U>
ostream& operator<<(ostream& outs, 
	const Sorted_List<U>& Sorted_List_obj)
{
	_print_link(Sorted_List_obj._head);
	return outs;
}

#endif // MYSORTEDLIST_H