#ifndef MYSTACK_H
#define MYSTACK_H

#include <iostream>
#include <cassert>
#include "linked_list.h"
using namespace std;

template <typename ITEM_TYPE>
class Stack {
public:
	class Iterator {
	public:

		//give List access to Iterator
		friend class Stack;

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
	Stack();

	//Big Three
	~Stack();
	Stack(const Stack<ITEM_TYPE>& copyThis);
	Stack& operator =(const Stack& RHS);

	//Puts item in at the front
	ITEM_TYPE push(ITEM_TYPE i);

	//removes and returns first item in stack
	ITEM_TYPE pop();

	//NULL if item not found
	ITEM_TYPE search(const ITEM_TYPE& key);

	//return the item at index
	const ITEM_TYPE& operator[](const int index) const;                   
	ITEM_TYPE& operator[](int index);             

	//return the item at the head of the Stack
	Iterator begin() const;

	//returns null
	Iterator end() const;

	//print the items in the stack
	void Print() const;

	//Return the number of items in the Stack
	int size() const;

	//Returns true if stack is empty
	bool is_empty() const;

	template <typename U>
	friend ostream& operator <<(ostream& outs, 
		const Stack<U>& obj);

private:
	node<ITEM_TYPE>* _head;
	int _size;
	bool _debug;
};

//Default Constructor
template<typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack()
{
	_debug = false;
	_head = nullptr;
	_size = 0;
	if (_debug) cout << "Constructor Executed" << endl;
}

//Big Three: Desctructor
template<typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack()
{
	_clear_list(_head); _head = nullptr;
	if (_debug) cout << "Destructor Executed" << endl;
}

//Big Three: Copy Constructor
template<typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyThis)
{
	//Copy Info from other object
	_debug = copyThis._debug;
	_size = copyThis._size;

	//Create new head
	node<ITEM_TYPE>* a_copy = nullptr;

	//Copy items
	_copy(copyThis._head, a_copy);
	_head = a_copy;

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

//Big Three: Assignment
template<typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::
operator=(const Stack<ITEM_TYPE>& rhs)
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

	//Copy items
	_copy(rhs._head, _head);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::push(ITEM_TYPE i)
{
	_size++;
	_head = _insert_head(_head, i);
	return _head->_item;
}

template<typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop()
{
	assert(size() > 0);
	_size--;
	return _delete_head(_head);
}

template<typename ITEM_TYPE>
void Stack<ITEM_TYPE>::
Print() const
{
	_print(_head);
}

template<typename ITEM_TYPE>
int Stack<ITEM_TYPE>::size() const
{
	return _size;
}

template<typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::is_empty() const
{
	if (_size == 0 && _head == nullptr) return true;
	return false;
}

template<typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::
search(const ITEM_TYPE& key)
{
	node<ITEM_TYPE>* found = _search(_head, key);

	if (found) return found->_item;

	return NULL;
}

template<typename ITEM_TYPE>
const ITEM_TYPE& Stack<ITEM_TYPE>::operator[]
(const int index) const
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
ITEM_TYPE& Stack<ITEM_TYPE>::operator[](int index)
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator
Stack<ITEM_TYPE>::begin() const
{
	assert(size() > 0);
	return  Iterator(_head);
}

template<typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator 
Stack<ITEM_TYPE>::end() const
{
	assert(size() > 0);
	return Iterator();
}

template<typename U>
ostream& operator<<(ostream& outs, 
	const Stack<U>& list_obj)
{
	_print_link(list_obj._head);
	return outs;
}

#endif //MYSTACK_H