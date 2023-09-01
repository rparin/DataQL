#ifndef MYAVL_H
#define MYAVL_H

#include "AVL.h"
using namespace std;

template <typename T>
class AVL {
public:

    //Default Constructors
    AVL();
    AVL(const T* list, int size = -1, bool sorted = true);

    //Big Three
    AVL(const AVL<T>& copy_me);
    AVL<T>& operator =(const AVL<T>& rhs);
    ~AVL();

    //Member Functions
    void insert(const T& insert_me);
    bool remove(const T& target);
    bool search(const T& target, tree_node<T>* & found_ptr);
    void clear();
    bool isempty() const;
    int size() const { return _size; }

    //Operators
    AVL<T>& operator +=(const AVL<T>& rhs);

    //Print
    template<typename U>
    friend ostream& operator <<(ostream& outs, const AVL<U>& tree);
    void printInorder(ostream& outs = cout) const;

private:
    tree_node<T>* _root;
    bool _debug;
    int _size;
};

template<typename T>
AVL<T>::AVL():
    _root(nullptr), _debug(false), _size(0){

    if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
AVL<T>::AVL(const T* list, int size, bool sorted):
    _debug(false), _size(0) {
    if(sorted) 
        _root = tree_from_sorted_list(list, size);

    else {
        //Create balanced tree from list
        _root = tree_from_list(list, size);
    }

    if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
AVL<T>::AVL(const AVL<T>& copy_me)
{
    //Copy Items to tree
    _root = tree_copy(copy_me._root);
    _debug = copy_me._debug;
    _size = copy_me._size;

    if (_debug) cout << "Copy CTOR Executed" << endl;
}

template<typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& rhs)
{
    //Check for self-reference
    if (this == &rhs) return *this;

    //Set private variables
    _debug = rhs._debug;

    //Clear Tree
    tree_clear(_root);

    //Copy Items to tree
    _root = tree_copy(rhs._root);
    _size = rhs._size;

    if (_debug) cout << "Assignment Executed" << endl;

    return *this;
}

template<typename T>
AVL<T>::~AVL()
{
    //Clear and deallocate items from tree
    tree_clear(_root);

    if (_debug) cout << "Destructor Executed" << endl;
}

template<typename T>
void AVL<T>::insert(const T& insert_me)
{
    tree_insert(_root, insert_me, true);
    _size++;
}

template<typename T>
bool AVL<T>::remove(const T& target)
{
    _size--;
    return tree_erase(_root, target);
}

template<typename T>
bool AVL<T>::search(const T& target, tree_node<T>*& found_ptr)
{
    return tree_search(_root, target, found_ptr);
}

template<typename T>
void AVL<T>::clear()
{
    //Clear and deallocate items from tree
    tree_clear(_root);
    _size = 0;
}

template<typename T>
bool AVL<T>::isempty() const
{
    return (_size == 0);
}

template<typename T>
AVL<T>& AVL<T>::operator+=(const AVL<T>& rhs)
{
    tree_add(this->_root, rhs._root);
    return *this;
}

template<typename T>
void AVL<T>::printInorder(ostream& outs) const
{
    tree_print_inorder(_root, outs);
}

template<typename U>
ostream& operator<<(ostream& outs, const AVL<U>& tree)
{
    if (tree._debug) tree_print_debug(tree._root, 0, outs);
    else tree_print(tree._root, 0, outs);

    return outs;
}

#endif //MYAVL_H
