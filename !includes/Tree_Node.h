    #ifndef TREE_NODE_H
#define TREE_NODE_H

#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

template <typename T>
struct tree_node{

    //Constructor
    tree_node(T item = T(), tree_node* left = NULL,
        tree_node* right = NULL) :
        _item(item), _left(left), _right(right)
    {
        //Starting height
        update_height();
    }

    //balance factor = height of the left and right subtrees
    int balance_factor()
    {
        //a NULL child has a height of -1
        //a leaf has a height of 0
        int l,r;

        if (_right == nullptr) r = -1;
        else r = _right->_height;

        if (_left == nullptr) l = -1;
        else l = _left->_height;

        return l - r;
    }
    
    int height()
    {
        return _height;
    }

    // Height of a node is 1 + height of the "taller" child
    int update_height()
    {
        //A leaf node has a height of zero: 1 + max(-1,-1)
        if (_left == nullptr && _right == nullptr)
            _height = 0;

        else if (_left == nullptr)
            _height = 1 + max(-1, _right->_height);

        else if (_right == nullptr)
            _height = 1 + max(_left->_height, -1);

        else
            _height =  1 + max(_left->_height, _right->_height);

		return _height;
    }

    friend ostream& operator << 
		(ostream& outs, const tree_node<T>& t_node){
        outs<<"|"<<t_node._item<<"|";
        return outs;
    }

    T _item;
    tree_node<T>* _left;
    tree_node<T>* _right;
    int _height;
};


#endif //TREE_NODE_H