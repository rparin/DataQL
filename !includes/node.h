#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;

template <typename ITEM_TYPE>
struct node
{
public:
	node(const ITEM_TYPE& item = ITEM_TYPE(), 
		node<ITEM_TYPE>* next = nullptr,
		node<ITEM_TYPE>* prev = nullptr);

	template <typename U>
	friend ostream& operator <<(ostream& outs, 
		const node<U> &printMe);

	ITEM_TYPE _item; //crate
	node* _next; //label
	node* _prev; //label
};

//initializes head to NULL
template<typename ITEM_TYPE>
node<ITEM_TYPE>* init_head(node<ITEM_TYPE>*& head) {
	head = nullptr;
}

//Node constructor
template<typename ITEM_TYPE>
node<ITEM_TYPE>::node(const ITEM_TYPE& item, 
	node<ITEM_TYPE>* next, node<ITEM_TYPE>* prev)
	:_item(item), _next(next), _prev(prev){
}

//Node insertion operator (prints item in node)
template<typename U>
ostream& operator <<(ostream& outs, 
	const node<U> &printMe){
	outs << printMe._item;
	return outs;
}
#endif //NODE_H