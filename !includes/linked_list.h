#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <cassert>
#include "node.h"
using namespace std;

//print items in link list
template <typename ITEM_TYPE>
void _print(node<ITEM_TYPE>* head);

//Print linked list with arrows
template<typename ITEM_TYPE>
void _print_link(node<ITEM_TYPE>* head, bool dual = false);

//return ptr to key or NULL
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _search(node<ITEM_TYPE>* head,          
	ITEM_TYPE key);

//insert at the head of list
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_head(node<ITEM_TYPE> *&head,         
	ITEM_TYPE insertThis);

//insert after ptr
template<typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_after(node<ITEM_TYPE>*& head,
	node<ITEM_TYPE>* afterThis, ITEM_TYPE insertThis);

template <typename ITEM_TYPE>
ITEM_TYPE _delete_head(node<ITEM_TYPE>*& head);

//delete, return item
template <typename ITEM_TYPE>
ITEM_TYPE _delete_node(node<ITEM_TYPE>*&head,                 
	node<ITEM_TYPE>* deleteThis);

//delete all the nodes
template <typename ITEM_TYPE>
void _clear_list(node<ITEM_TYPE>*& head);

//_prints link list backwards
template <typename ITEM_TYPE>
void _print_backwards(node<ITEM_TYPE> *lastNode);

//ptr to previous node
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _previous_node(node<ITEM_TYPE>* head,
	node<ITEM_TYPE>* prevToThis);

//insert before ptr
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_before(node<ITEM_TYPE>*& head,       
	node<ITEM_TYPE>* beforeThis,
	ITEM_TYPE insertThis);

//duplicate the list
//Returns pointer to last node
template<typename ITEM_TYPE>
node<ITEM_TYPE>* _copy(node<ITEM_TYPE>* head, 
	node<ITEM_TYPE>* &copy);

//Last Node in the list
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head);

//item at this position
template <typename ITEM_TYPE>
ITEM_TYPE& at(node<ITEM_TYPE>* head, int pos);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>* &head,
	ITEM_TYPE& item, bool ascending = true, bool unique = true);

//insert or add if a dup
template <typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted_and_add(node<ITEM_TYPE>* &head,
	ITEM_TYPE item, bool ascending = true);

template <typename ITEM_TYPE>
node<ITEM_TYPE>* _where_this_goes(node<ITEM_TYPE>* head,
	ITEM_TYPE item,
	bool ascending = true);

template <typename ITEM_TYPE>
int _index_of(node<ITEM_TYPE>* head, 
	node<ITEM_TYPE>* mark);

template<typename ITEM_TYPE>
void _print(node<ITEM_TYPE>* head){

	node<ITEM_TYPE>* walker = head;
	while (walker != nullptr) {

		cout << *walker << " ";
		walker = walker->_next;
	}
}

template<typename ITEM_TYPE>
void _print_link(node<ITEM_TYPE>* head, bool dual) {

	bool first = true;

	node<ITEM_TYPE>* walker = head;
	while (walker != nullptr) {
		
		//Print arrow for two way linked list
		if(dual)
			if (!first) cout << "<-";

		cout << "[" << *walker << "]->";
		walker = walker->_next;

		first = false;
	}

	cout << "|||";
}

template<typename ITEM_TYPE>
void _print_backwards(node<ITEM_TYPE>* lastNode) {

	bool first = true;

	node<ITEM_TYPE>* walker = lastNode;
	while (walker) {

		if (!first) cout << "<-";

		cout << "[" << walker->_item << "]->";
		walker = walker->_prev;

		first = false;
	}
	cout << "|||";

	//Recursive Way
	//call function until you reach null
	//if (head != nullptr) {
	//	_print_backwards(head->_next);
	//	cout << "[" << *head << "]->";
	//}
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _search(node<ITEM_TYPE>* head, 
	ITEM_TYPE key){

	node<ITEM_TYPE>* walker = head;
	while (walker != nullptr) {

		//return item if found
		if (walker->_item == key)
			return walker;

		//go through link list
		walker = walker->_next;
	}

	return nullptr;
}

template<typename ITEM_TYPE>
ITEM_TYPE& at(node<ITEM_TYPE>* head, int pos)
{
	assert(pos >= 0 && head != nullptr);

	node<ITEM_TYPE>* walker = head;

	//Counter
	int count = 0;
	while (walker != nullptr) {

		//Return item if found
		if (count == pos) return walker->_item;

		//Increase counter and walker
		walker = walker->_next;
		count++;
	}

	//assert if not pos not in list
	assert(count == pos-1);
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_head(node<ITEM_TYPE>*& head, 
	ITEM_TYPE insertThis){

	//create new node with given item
	node<ITEM_TYPE> *temp = new node<ITEM_TYPE>(insertThis);

	//Set previous node
	if (head) head->_prev = temp;

	//point node's next to where head pointer is pointing to
	temp->_next = head;

	//point head to this new head pointer
	head = temp;

	return head;
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_after(node<ITEM_TYPE>*& head, 
	node<ITEM_TYPE>* afterThis, ITEM_TYPE insertThis){
	
	//Insert in head if null
	if (afterThis == nullptr)
		return _insert_head(head, insertThis);

	_insert_head(afterThis->_next, insertThis);
	return afterThis->_next;
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_before(node<ITEM_TYPE>*& head,
	node<ITEM_TYPE>* beforeThis, ITEM_TYPE insertThis) {

	//return pointer to inserted node
	return _insert_after(head, beforeThis->_prev, insertThis);
}

template<typename ITEM_TYPE>
ITEM_TYPE _delete_head(node<ITEM_TYPE>*& head)
{
	//point a pointer to the node being removed
	node<ITEM_TYPE>* delete_this = head;

	//Hold on to the deleted item
	ITEM_TYPE del_item = delete_this->_item;

	//point the headptr to the next node if it exits
	if (head->_next != nullptr) head = head->_next;
	else head = nullptr;

	//delete the node
	delete delete_this;

	//return deleted item
	return del_item;
}

template<typename ITEM_TYPE>
ITEM_TYPE _delete_node(node<ITEM_TYPE>*& head, 
	node<ITEM_TYPE>* deleteThis){

	assert(deleteThis != nullptr);

	//return lowest element type if head is null
	if (head == nullptr) return ITEM_TYPE();

	if (head == deleteThis)
		return _delete_head(head);;

	//Get node to re-link list
	//node<ITEM_TYPE>* prev = deleteThis->_prev;

	node<ITEM_TYPE>* prev = 
		_previous_node(head, deleteThis);
	if (prev == nullptr) return ITEM_TYPE();

	//re-link list and delete node
	return _delete_head(prev->_next);;
}

template<typename ITEM_TYPE>
void _clear_list(node<ITEM_TYPE>*& head)
{
	//delete heads until link list is cleared
	while (head != nullptr) {
		_delete_head(head);
	}
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _previous_node(node<ITEM_TYPE>* head, 
	node<ITEM_TYPE>* prevToThis){

	if (prevToThis == head)
		return nullptr;

	assert(prevToThis != nullptr);

	node<ITEM_TYPE>* walker = head;
	while (walker->_next != nullptr) {

		//return node if found
		if (walker->_next == prevToThis)
			return walker;
		walker = walker->_next;
	}

	//assert if node not in list
	assert(prevToThis == head);
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _last_node(node<ITEM_TYPE>* head)
{
	node<ITEM_TYPE>* walker = head;
    
    if(head == nullptr)
        return head;

	//go through link list and stop before the end
	while (walker->_next != nullptr) {
		walker = walker->_next;
	}

	return walker;
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _copy(node<ITEM_TYPE>* head, 
	node<ITEM_TYPE>* &copy){

	node<ITEM_TYPE>* walker = head;	//walker
	node<ITEM_TYPE>* after = nullptr;//store the last node

	//add nodes to copy
	while (walker != nullptr) {
		after = _insert_after(copy, after, walker->_item);
		walker = walker->_next;
	}

	//return pointer to the end of copied node
	return after;
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted(node<ITEM_TYPE>*& head, 
	ITEM_TYPE& item, bool ascending, bool unique){

	node<ITEM_TYPE>* after_this =
		_where_this_goes(head, item, ascending);

	//Update or not Update
	if (unique) {
		if (after_this != nullptr && after_this->_item == item) {
			after_this->_item = item;
			return after_this;
		}
	}

	return _insert_after(head,after_this,item);
}


template<typename ITEM_TYPE>
node<ITEM_TYPE>* _insert_sorted_and_add(node<ITEM_TYPE>*& head,
	ITEM_TYPE item, bool ascending)
{
	node<ITEM_TYPE>* after_this =
		_where_this_goes(head, item, ascending);

	//Add
	if (after_this != nullptr && after_this->_item == item) {
		after_this->_item += item;
		return after_this;
	}

	return _insert_after(head, after_this, item);
}

template<typename ITEM_TYPE>
node<ITEM_TYPE>* _where_this_goes(node<ITEM_TYPE>* head, 
	ITEM_TYPE item, bool ascending)
{
	//Empty List
	if (head == nullptr || head->_item == item) return head;

	//return null if item needs to go at very front
	if (head->_item > item && ascending)
		return nullptr;
	else if (head->_item < item && ascending == false)
		return nullptr;

	node<ITEM_TYPE>* leader = head->_next;//leader
	node<ITEM_TYPE>* follower = head;//store prev

	while (leader != nullptr) {

		if (leader->_item == item) {
			return leader;
		}

		if (ascending) {
			if (leader->_item > item) {
				return follower;
			}
				
		}
		else {
			if (leader->_item < item)
				return follower;
		}

		follower = leader;
		leader = leader->_next;
	}

	//Pointer to the very last item
	return follower;
}

template<typename ITEM_TYPE>
int _index_of(node<ITEM_TYPE>* head, node<ITEM_TYPE>* mark)
{
	node<ITEM_TYPE>* walker = head;
	int count = 0;
	while (walker != nullptr) {

		if (walker == mark)
			return count;

		walker = walker->_next;
		count++;
	}

	return -1;
}
#endif //LINKED_LIST_H
