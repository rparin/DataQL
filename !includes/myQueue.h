#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <iostream>
#include <cassert>
#include "linked_list.h"
using namespace std;

template <typename ITEM_TYPE>
class Queue {
public:
	class Iterator {
	public:

		//give List access to Iterator
		friend class Queue;

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

		//true if _ptr is nullptr
		bool is_null() {
			if (_ptr == nullptr) return true;
			return false;
		}

		//member access operator
		node<ITEM_TYPE>* operator ->() {
			assert(_ptr);
			return _ptr;
		}

		//true if left != right
		friend bool operator !=(const Iterator& left,
			const Iterator& right) {
			return (left._ptr != right._ptr);
		}

		//true if left == right
		friend bool operator ==(const Iterator& left,
			const Iterator& right) {
			return (left._ptr == right._ptr);
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
	Queue();

	//Big Three
	~Queue();
	Queue(const Queue<ITEM_TYPE>& copyThis);
	Queue& operator =(const Queue& RHS);

	//Puts item in at the back
	ITEM_TYPE push(ITEM_TYPE i);

	//Returns item at the front/head
	ITEM_TYPE pop();

	//NULL if item not there
	ITEM_TYPE search(const ITEM_TYPE& key);		

	//return the item at index
	const ITEM_TYPE& operator[](const int index) const;
	ITEM_TYPE& operator[](int index);

	//return the item at the head of the Queue
	Iterator begin() const;

	//return a null pointer
	Iterator end() const;

	//Print Queue
	void Print() const;

	//Return the number of items in the Queue
	int size() const;

	//Returns true if stack is empty
	bool is_empty() const;

	template <typename U>
	friend ostream& operator <<(ostream& outs, 
		const Queue<U>& obj);

private:
	node<ITEM_TYPE>* _head;
	node<ITEM_TYPE>* _tail;
	int _size;
	bool _debug;
};

template<typename ITEM_TYPE>
Queue<ITEM_TYPE>::Queue()
{
	_debug = false;
	_head = nullptr;
	_tail = nullptr;
	_size = 0;
	if (_debug) cout << "Constructor Executed" << endl;
}

//Big Three: Desctructor
template<typename ITEM_TYPE>
Queue<ITEM_TYPE>::~Queue()
{
	_clear_list(_head);
	if (_debug) cout << "Destructor Executed" << endl;
}

//Big Three: Copy Constructor
template<typename ITEM_TYPE>
Queue<ITEM_TYPE>::Queue(const Queue<ITEM_TYPE>& copyThis)
{
	//Copy Info from other object
	_debug = copyThis._debug;
	_size = copyThis._size;
	_head = nullptr;

	//Set head and tail to copied list pointers
	_tail = _copy(copyThis._head, _head);

	if (_debug) cout << "Copy CTOR Executed" << endl;
}

//Big Three: Assignment
template<typename ITEM_TYPE>
Queue<ITEM_TYPE>& Queue<ITEM_TYPE>::
operator=(const Queue<ITEM_TYPE>& rhs)
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

	//Set New Head and Tail
	_tail = _copy(rhs._head, _head);

	if (_debug) cout << "Assignment Executed" << endl;

	return *this;
}

template<typename ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::push(ITEM_TYPE i)
{
	_size++;
	//Put item after the last item
	_tail = _insert_after(_head, _tail, i);
	return _tail->_item;
}

template<typename ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::pop()
{
	assert(is_empty() == false);
	_size--;
	if (_size == 0) _tail = nullptr;
	return _delete_head(_head);
}

template<typename ITEM_TYPE>
void Queue<ITEM_TYPE>::
Print() const
{
	_print(_head);
}

template<typename ITEM_TYPE>
int Queue<ITEM_TYPE>::size() const
{
	return _size;
}

template<typename ITEM_TYPE>
bool Queue<ITEM_TYPE>::is_empty() const
{
	if (size() == 0 && _head == nullptr && _tail == nullptr)
		return true;

	assert(size() > 0 && _head != nullptr && _tail != nullptr);
	return false;
}

template<typename ITEM_TYPE>
ITEM_TYPE Queue<ITEM_TYPE>::
search(const ITEM_TYPE& key)
{
	node<ITEM_TYPE>* found = _search(_head, key);

	if (found) return found->_item;

	return NULL;
}

template<typename ITEM_TYPE>
const ITEM_TYPE& Queue<ITEM_TYPE>::operator[]
(const int index) const
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
ITEM_TYPE& Queue<ITEM_TYPE>::operator[](int index)
{
	return at(_head, index);
}

template<typename ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator
Queue<ITEM_TYPE>::begin() const
{
	assert(is_empty() == false);
	return Iterator(_head);
}

template<typename ITEM_TYPE>
typename Queue<ITEM_TYPE>::Iterator Queue<ITEM_TYPE>::end() const
{
	assert(size() > 0);
	return Iterator();
}

template<typename U>
ostream& operator<<(ostream& outs, 
	const Queue<U>& list_obj)
{
	_print_link(list_obj._head);
	return outs;
}

#endif //MYQUEUE_H