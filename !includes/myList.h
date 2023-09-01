#ifndef MYLIST_H
#define MYLIST_H

#include <iostream>
#include <cassert>
#include <cstdlib>
#include "linked_list.h"
using namespace std;

template <typename ITEM_TYPE>
class List {
public:
	class Iterator {
	public:

		//give List access to Iterator
		friend class List;

		//default ctor
		Iterator() { _ptr = nullptr; }

		//Point Iterator to where p is pointing to
		Iterator(node<ITEM_TYPE>* p) { _ptr = p; }

		//dereference operator
		const ITEM_TYPE operator *() const {
			assert(_ptr); return _ptr->_item;
		}
		ITEM_TYPE& operator *() {
			assert(_ptr); return _ptr->_item;
		}

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
	List();

	//Big Three
	~List();
	List(const List<ITEM_TYPE>& copyThis);
	List& operator =(const List& RHS);

	//insert i at the head of list
	Iterator insert_head(ITEM_TYPE i);

	//insert i after iMarker
	Iterator insert_after(ITEM_TYPE i,
		Iterator& mark);

	//insert i before iMarker
	Iterator insert_before(ITEM_TYPE i,
		Iterator& mark);

	//insert i randomly
	//Iterator insert_rand(ITEM_TYPE i);

	//delete node pointed to by iMarker
	ITEM_TYPE remove(Iterator& mark);
	void clear();

	//Returns null if key not found
	Iterator search(Iterator& mark, const ITEM_TYPE& key);
	Iterator search(const ITEM_TYPE& key);

	//get the previous node to iMarker
	Iterator prev(Iterator& mark);

	//return the item at index
	const ITEM_TYPE& operator[](const int index) const;
	ITEM_TYPE& operator[](int index);

	//return the head of the List
	//node<ITEM_TYPE>* Begin() const;                   
	Iterator begin() const;

	//return the tail of the List
	Iterator last_node() const;

	Iterator end() const;

	//Returns the index of a node pointer given
	int index_of(Iterator& mark);

	//print the List marking the pointer input
	void print() const;

	//Return the number of items in the list
	int size() const;

	template <typename U>
	friend ostream& operator <<(ostream& outs,
		const List<U>& obj);

private:
	node<ITEM_TYPE>* _head;
	int _size;
	bool _debug;
};

template<typename ITEM_TYPE>
List<ITEM_TYPE>::List()
{
	_debug = false;
	_head = nullptr;
	_size = 0;
	if (_debug) cout << "Constructor Executed" << endl;
}

//Big Three: Desctructor
template<typename ITEM_TYPE>
List<ITEM_TYPE>::~List()
{
	_clear_list(_head); _head = nullptr;
	if (_debug) cout << "Destructor Executed" << endl;
}

//Big Three: Copy Constructor
template<typename ITEM_TYPE>
List<ITEM_TYPE>::List(const List<ITEM_TYPE>& copyThis)
{
	//Copy Info from other object
	_debug = copyThis._debug;
	_size = copyThis._size;
	_head = nullptr;

	//Copy items
	_copy(copyThis._head, _head);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

//Big Three: Assignment
template<typename ITEM_TYPE>
List<ITEM_TYPE>& List<ITEM_TYPE>::
operator=(const List<ITEM_TYPE>& rhs)
{
	//Check for self-reference
	if (this == &rhs) return *this;

	//Delete dynamic space if it exits already
	if (_head != nullptr) {
		_clear_list(_head);
	}

	//Copy Info from other object
	_debug = rhs._debug;
	_size = rhs._size;
	_head = nullptr;

	//Copy items
	_copy(rhs._head, _head);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::insert_head(ITEM_TYPE i)
{
	_size++;
	_head = _insert_head(_head, i);
	return Iterator(_head);
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::
insert_after(ITEM_TYPE i, Iterator& mark)
{
	_size++;
	node<ITEM_TYPE>* after = _insert_after(_head, mark._ptr, i);
	return Iterator(after);
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator List<ITEM_TYPE>::
insert_before(ITEM_TYPE i, Iterator& mark)
{
	_size++;
	node<ITEM_TYPE>* before = _insert_before(_head, mark._ptr, i);
	return Iterator(before);
}

//Insert random function
//template<typename ITEM_TYPE>
//typename List<ITEM_TYPE>::Iterator
//List<ITEM_TYPE>::insert_rand(ITEM_TYPE i)
//{
//	node<ITEM_TYPE>* mark = nullptr;
//
//	if (size() != 0) {
//		int rand_num = rand() % size();
//
//		node<ITEM_TYPE> item = *this[rand_num];
//
//		mark = _search(_head, item);
//	}
//
//	node<ITEM_TYPE>* insert = nullptr;
//	int after = rand() % 100 + 1;
//	int before = rand() % 100 + 1;
//
//	if (mark == nullptr || after > before)
//		insert = _insert_after(_head, mark, i);
//	else
//		insert = _insert_before(_head, mark, i);
//
//	_size++;
//	return Iterator(insert);
//}

template<typename ITEM_TYPE>
ITEM_TYPE List<ITEM_TYPE>::
remove(Iterator& mark)
{
	_size--;
	return _delete_node(_head, mark._ptr);
}

template<typename ITEM_TYPE>
void List<ITEM_TYPE>::clear()
{
	_clear_list(_head); _head = nullptr;
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator
List<ITEM_TYPE>::search(Iterator& mark, const ITEM_TYPE& key)
{
	if (mark._ptr)
		return Iterator(_search(mark._ptr, key));
	else
		return Iterator();
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator
List<ITEM_TYPE>::search(const ITEM_TYPE& key)
{
	return Iterator(_search(_head, key));
}

template<typename ITEM_TYPE>
void List<ITEM_TYPE>::
print() const
{
	_print(_head);
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator
List<ITEM_TYPE>::prev(Iterator& mark)
{
	node<ITEM_TYPE>* prev = _previous_node(_head, mark._ptr);
	return Iterator(prev);
}

template<typename ITEM_TYPE>
const ITEM_TYPE& List<ITEM_TYPE>::operator[]
(const int index) const
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
ITEM_TYPE& List<ITEM_TYPE>::operator[](int index)
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator
List<ITEM_TYPE>::begin() const
{
	return Iterator(_head);
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator
List<ITEM_TYPE>::last_node() const
{
	node<ITEM_TYPE>* last = _last_node(_head);
	return Iterator(last);
}

template<typename ITEM_TYPE>
typename List<ITEM_TYPE>::Iterator
List<ITEM_TYPE>::end() const
{
	return Iterator();
}

template<typename ITEM_TYPE>
int List<ITEM_TYPE>::index_of(Iterator& mark)
{
	return _index_of(_head, mark._ptr);
}

template<typename ITEM_TYPE>
int List<ITEM_TYPE>::size() const
{
	return _size;
}

template<typename U>
ostream& operator<<(ostream& outs,
	const List<U>& Sorted_List_obj)
{
	_print_link(Sorted_List_obj._head);
	return outs;
}

#endif // MYLIST_H