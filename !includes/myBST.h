#ifndef MYBST_H
#define MYBST_H

#include "BST.h"
using namespace std;

template <typename T>
class BST {
public:

    //Default Constructors
    BST();
    BST(const T* sorted_list, int size = -1);

    //Big Three
    BST(const BST<T>& copy_me);
    BST<T>& operator =(const BST<T>& rhs);
    ~BST();

    //Member Functions
    void insert(const T& insert_me);
    bool remove(const T& target);
    bool search(const T& target, tree_node<T>* & found_ptr);
    void clear();
    bool isempty() const;
    int size() const { return _size; }

    //Operators
    BST<T>& operator +=(const BST<T>& rhs);

    //Print
    template<typename U>
    friend ostream& operator <<(ostream& outs, const BST<U>& tree);
    void printInorder(ostream& outs = cout) const;

private:
    tree_node<T>* _root;
    bool _debug;
    int _size;
};

template<typename T>
BST<T>::BST():
    _root(nullptr), _debug(false), _size(0) {

    if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
BST<T>::BST(const T* sorted_list, int size):
    _debug(false), _size(0)
{
    //Create tree from sorted list
    _root = tree_from_sorted_list(sorted_list, size);

    if (_debug) cout << "Constructor Executed" << endl;
}

template<typename T>
BST<T>::BST(const BST<T>& copy_me)
{
    //Copy Items to tree
    _root = tree_copy(copy_me._root);
    _debug = copy_me._debug;
    _size = copy_me._size;

    if (_debug) cout << "Copy CTOR Executed" << endl;
}

template<typename T>
BST<T>& BST<T>::operator=(const BST<T>& rhs)
{
    //Check for self-reference
    if (this == &rhs) return *this;

    //Set private variables
    _debug = rhs._debug;

    //Clear Tree
    tree_clear(_root);

    //Copy Items to tree
    _root = tree_copy(rhs._root);
    _size = rhs.size;

    if (_debug) cout << "Assignment Executed" << endl;

    return *this;
}

template<typename T>
BST<T>::~BST()
{
    //Clear and deallocate items from tree
    tree_clear(_root);

    if (_debug) cout << "Destructor Executed" << endl;
}

template<typename T>
void BST<T>::insert(const T& insert_me)
{
    tree_insert(_root, insert_me);
}

template<typename T>
bool BST<T>::remove(const T& target)
{
    return tree_erase(_root, target);
}

template<typename T>
bool BST<T>::search(const T& target, tree_node<T>*& found_ptr)
{
    return tree_search(_root, target, found_ptr);
}

template<typename T>
void BST<T>::clear()
{
    //Clear and deallocate items from tree
    tree_clear(_root);
}

template<typename T>
bool BST<T>::isempty() const
{
    return (_size == 0);
}

template<typename T>
BST<T>& BST<T>::operator+=(const BST<T>& rhs)
{
    tree_add(this->_root, rhs._root);
    return *this;
}

template<typename T>
void BST<T>::printInorder(ostream& outs) const
{
    tree_print_inorder(_root, outs);
}

template<typename U>
ostream& operator<<(ostream& outs, const BST<U>& tree)
{
    if (tree._debug) tree_print_debug(tree._root, 0, outs);
    else tree_print(tree._root, 0, outs);

    return outs;
}

#endif //MYBST_H
