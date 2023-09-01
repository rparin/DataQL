#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <cassert>
#include <iomanip>
#include "Tree_Node.h"
using namespace std;

//Max capacity for dynamic array
//Used in Tree to Array
const int MAX_CAP = 100;

//Inserts item
//Update duplicates defaults to false
template <typename T>
void tree_insert(tree_node<T>* &root, 
	const T& insert_me, bool dup = false);

//Searches tree and returns root pointing to item
template <typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T& target);

//Searches tree and returns true or false if item found
template <typename T>
bool tree_search(tree_node<T>* root, const T& target,
                 tree_node<T>* &found_ptr);

//Prints Tree Horizontally
template<typename T>
void tree_print(tree_node<T>* root, int level= 0, 
                                    ostream& outs = cout);

//prints detailes info about each node
template<typename T>
void tree_print_debug
(tree_node<T>* root, int level = 0, ostream& outs = cout);

//Prints Items in Tree preorder (Root, Left, Right)
template<typename T>
void tree_print_preorder
(const tree_node<T>* root, ostream& outs = cout);

//Prints Items in Tree Inorder (Left, Root, Right)
template<typename T>
void tree_print_inorder
(const tree_node<T>* root, ostream& outs = cout);

//Prints Items in Tree in postorder (Left, Right, Root)
template<typename T>
void tree_print_postorder
(const tree_node<T>* root, ostream& outs = cout);

//clear the tree
template <typename T>       
void tree_clear(tree_node<T>* &root);

//erase target from the tree
template <typename T>
bool tree_erase(tree_node<T>*& root, const T& target);

//erase rightmost node from the tree
// store the item in max_value
template <typename T>       
void tree_remove_max(tree_node<T>* &root, T& max_value);

//return copy of tree pointed to by root
template <typename T>       
tree_node<T>* tree_copy(tree_node<T>* root);

//Union of both trees = dest
template <typename T>       
void tree_add(tree_node<T>* & dest, const tree_node<T>* src);

//Sorted list to Tree
template<typename T>
tree_node<T>* tree_from_sorted_list(const T* a, int size);

//Creates a balnced tree from list
template<typename T>
tree_node<T>* tree_from_list(const T* a, int size);

//Get Tree's Right most value
template<typename T>
tree_node<T>* get_right_most(const tree_node<T>* &root);

//Get Tree's Left most value
template<typename T>
tree_node<T>* get_left_most(const tree_node<T>*& root);

//Merges Two Unique arrays to a third array 
//containing all of the elements with no duplicates
//Arrays input has to be unique, sorted and comparable
template<typename T>
T* Union(T* a1, int size1, T* a2, int size2, int& newSize);

//Copy a portion of one array to another
template<typename T>
void copyArr(T* arr, T* start, T* stop, int& size);

//Returns an array of the tree inorder
template<typename T>
void tree_to_inorder_arr(const tree_node<T>* root, 
int arr[], int &size);

//Returns an array of the tree postorder
template<typename T>
void tree_to_postorder_arr(const tree_node<T>* root, 
int arr[], int& size);

// ---------------- ROTATIONS --------------------------
template <typename T>
tree_node<T>* rotate_right(tree_node<T>*& root);

template <typename T>
tree_node<T>* rotate_left(tree_node<T>*& root);

//decide which rotate is needed based on balance factor
template <typename T>
tree_node<T>* rotate(tree_node<T>*& root);

template<typename T>
void tree_insert(tree_node<T>*& root, const T &insert_me, bool dup)
{

	//Create new root with given item
	if (root == nullptr) {
		root = new tree_node<T>(insert_me);
	}

	//No Duplicates
	else if (dup == false && insert_me == root->_item) {
		return;
	}

	//Update Duplicates
	else if (dup && insert_me == root->_item) {
		root->_item = insert_me;
		return;
	}
		
	//Insert Item to left subtree
	else if (insert_me < root->_item) {
		tree_insert(root->_left, insert_me);
	}

	//Insert Item to right subtree
	else {
		tree_insert(root->_right, insert_me);
	}

	//Update the height for every insert
	root->update_height();

	//Rotate Root if necessary
	rotate(root);
}

template<typename T>
tree_node<T>* tree_search(tree_node<T>* root, const T &target)
{
	const bool debug = false;

	//Empty Tree or item not in tree
	if (root == nullptr) {
		if (debug) cout 
		<< "Tree-Search: Not Found: Root is Null" 
		<< endl;
		return nullptr;
	}

	//Item Found return root pointing to item
	else if (root->_item == target) {
		if (debug) cout << "Tree-Search: Found Target: ["
			<< root->_item << "[" << endl;
		return root;
	}

	//Go to the left of the tree
	else if (target < root->_item) {
		if (debug) cout << "Tree-Search: Going Left" << endl;
		return tree_search(root->_left, target);
	}

	//Go to the right of the tree
	else if (target > root->_item) {
		if (debug) cout << "Tree-Search: Going Right" << endl;
		return tree_search(root->_right, target);
	}

	//Error, should not go here
	else {
		if (debug) cout << "ERROR IN TREE SEARCH" << endl;
		assert(0);
		return nullptr;
	}

}

template<typename T>
bool tree_search
(tree_node<T>* root, const T &target, tree_node<T>*& found_ptr)
{
	//Set ptr to item
	found_ptr = tree_search(root, target);

	//If item is found then tree_search will return pointer
	if (found_ptr != nullptr) return true;

	//If item is not found then tree_search will return nullptr
	else return false;
}

template<typename T>
void tree_print(tree_node<T>* root, int level, ostream & outs)
{
	//Don't print a blank tree
	if (root != nullptr)
	{
		//Go to the end of the right tree
		tree_print(root->_right, level + 1);
		
		int width = static_cast<int>(4 * level);
		int bf = root->balance_factor();

		//Print Item
		outs << setw(width) << "[" << root->_item << "]" << endl;
		
		//Make sure tree is balanced
		assert(abs(bf) <= 1);

		//After going to the right
		//Go to the end of the left tree
		tree_print(root->_left, level + 1);
	}

}

template<typename T>
void tree_print_debug(tree_node<T>* root, int level, ostream& outs)
{
	if (root != nullptr)
	{
		//Go to the end of the right tree
		tree_print_debug(root->_right, level + 1);

		int width = static_cast<int>(4 * level);
		int bf = root->balance_factor();

		//Print Item
		outs << setw(width) << "[" << root->_item

			//Print Height and Balance Factor
			<< "] H:" << root->height() << " BF: " << bf << endl;

		//Make sure tree is balanced
		assert(abs(bf) <= 1);

		//Go to the end of the left tree
		//After going to the right
		tree_print_debug(root->_left, level + 1);

	}
}

template<typename T>
void tree_print_preorder(const tree_node<T>* root, ostream& outs)
{
	if (root != nullptr)
	{
		//Print Item
		outs << "[" << root->_item << "] ";

		//Go to the end of the left tree
		tree_print_preorder(root->_left);

		//Go to the end of the right tree
		//After going to the left
		tree_print_preorder(root->_right);
	}
}

template<typename T>
void tree_print_inorder(const tree_node<T>* root, ostream& outs)
{
	if (root != nullptr)
	{
		//Go to the end of the left tree
		tree_print_inorder(root->_left);

		//Print Item
		outs << "[" << root->_item << "] ";

		//Go to the end of the right tree
		//After going to the left
		tree_print_inorder(root->_right);
	}
}

template<typename T>
void tree_print_postorder(const tree_node<T>* root, ostream& outs)
{
	if (root != nullptr)
	{
		//Go to the end of the left tree
		tree_print_postorder(root->_left);

		//Go to the end of the right tree
		//After going to the left
		tree_print_postorder(root->_right);

		//Print Item
		outs << "[" << root->_item << "] ";
	}
}

template<typename T>
void tree_clear(tree_node<T>*& root)
{
	if (root != nullptr) {
		//Delete the left subtree
		tree_clear(root->_left);

		//Delete the right subtree
		tree_clear(root->_right);

		//delete the root and set roots to nullptr
		delete root;
		root = nullptr;
	}
}

template<typename T>
bool tree_erase(tree_node<T>*& root, const T& target)
{
	const bool debug = false;

	//Empty Tree or target not in tree
	if (root == nullptr) {
		if (debug) cout 
		<< "Tree-Erase: Not Found: Root is Null" 
		<< endl;
		return false;
	}

	//Item Found, return root pointing to item
	else if (root->_item == target) {
		if (debug) cout << "Tree-Erase: Found Target: ["
			<< root->_item << "]" << endl;

		//Bypass target and connect to right subtree
		if (root->_left == nullptr) {

			//Hold onto target node
			tree_node<T>* temp_root = root;

			//Connect to right subtree
			root = root->_right;

			//Delete target node
			delete temp_root;
		}

		//Target has a left child
		//replace target with left child's rightmost (largest)
		//child. Eliminate the rightmost child of the left subtree
		else {

			//Get and delete Left child's rightmost
			T temp_item;
			tree_remove_max(root->_left, temp_item);

			//Set replace item at root
			root->_item = temp_item;

			//Update the height for the erased item
			root->update_height();
			rotate(root);

		}

		return true;
	}

	//Go to the left of the tree
	else if (target < root->_item) {
		if (debug) cout << "Tree-Erase: Going Left" << endl;
		tree_erase(root->_left, target);
		root->update_height();
		rotate(root);
	}

	//Go to the right of the tree
	else if (target > root->_item) {
		if (debug) cout << "Tree-Erase: Going Right" << endl;
		tree_erase(root->_right, target);
		root->update_height();
		rotate(root);
	}

	return false;
}

template<typename T>
void tree_remove_max(tree_node<T>*& root, T& max_value)
{
	if (root != nullptr)
	{
		//Once you reach the end of the right subtree
		if (root->_right == nullptr) {
			
			//Set the max value to be 'returned'
			max_value = root->_item;

			//calls erase and reconnects subtrees if necessary
			tree_erase(root, root->_item);
			

		}
		else {
			//Go to the end of the right tree
			tree_remove_max(root->_right, max_value);
		}

		//Rotate root if necessary
		rotate(root);

	}
}

template<typename T>
tree_node<T>* tree_copy(tree_node<T>* root)
{
	if (root != nullptr) {
		return new tree_node<T>

			//Return the item
			(root->_item,

			//Copy the left subtree
			tree_copy(root->_left),

			//Copy the right subtree
			tree_copy(root->_right));
	}

	//If trying to copy empty tree then return nullptr
	else return nullptr;
}

template<typename T>
void tree_add(tree_node<T>*& dest, const tree_node<T>* src)
{
	//Create dynamic array			//Sizes
	T* dest_arr = new T[MAX_CAP];	int dest_size = 0;
	T* src_arr = new T[MAX_CAP];	int src_size = 0;

	//Get arrays of trees
	tree_to_inorder_arr(dest, dest_arr, dest_size);
	tree_to_inorder_arr(src, src_arr, src_size);

	// get/create dynamic union array of both tree arrays
	int union_size = 0;
	T* union_arr = Union(dest_arr, dest_size, 
						 src_arr,  src_size, 
										union_size);

	//clear the destination tree as we are creating a 
		//new tree to store the union of both trees
	tree_clear(dest);

	//Set/create dest tree
	dest = tree_from_sorted_list(union_arr, union_size);

	//Delete the created arrays
	delete[] dest_arr;
	delete[] src_arr;
	delete[] union_arr;
}

template<typename T>
tree_node<T>* tree_from_sorted_list(const T* a, int size)
{
	//End of array or empty array
	if (size == 0) return nullptr;

	//Create null root(s)
	tree_node<T>* root = nullptr;

	//Get midpoint
	int mid = size / 2;

	//When first called this will be the main root
	//		containing the array's middle value
	tree_insert(root, a[mid]);		//Insert the midpoint value

	//Create the left subtree
	//Go to the left center of the 
	//center of the array etc.
	root->_left = tree_from_sorted_list(a, mid);

	//Get the size for the right array
	int right_size = (size - mid - 1);	

	//Go to the right center of the 
	//center of the array etc.
	root->_right = tree_from_sorted_list(&a[mid+1], right_size);
										
	//Update the height
	root->update_height();

	return root;
}

template<typename T>
tree_node<T>* tree_from_list(const T* a, int size)
{
	tree_node<T>* root = nullptr;
	for (int i = 0; i < size; i++) {
		tree_insert(root, a[i]);
	}
	return root;
}

template<typename T>
tree_node<T>* get_right_most(const tree_node<T>* &root)
{
	if (root != nullptr)
	{
		//Once you reach the end of the right subtree
		if (root->_right == nullptr) {

			return root;
		}
		else {
			//Go to the end of the right tree
			get_right_most(root->_right);
		}

	}
}

template<typename T>
tree_node<T>* get_left_most(const tree_node<T>*& root)
{
	if (root != nullptr)
	{
		//Once you reach the end of the left subtree
		if (root->_left == nullptr) {

			return root;
		}
		else {
			//Go to the end of the right tree
			get_left_most(root->_left);
		}
		
	}
}

template<typename T>
T* Union(T* a1, int size1, T* a2, int size2, int& unionSize) {

	//Create dynamic array
	T* result = new T[size1 + size2];

	//Initialize Start Pointers
	T* a1ptr = a1;
	T* a2ptr = a2;
	T* resultptr = result;

	//End Pointers
	T* a1Endptr = a1ptr + size1;
	T* a2Endptr = a2ptr + size2;

	int travSize = size1 + size2;

	for (int i = 0; i < travSize; i++) {

		//Compare Elements from a1 and a2 array and 
		//set newElement to the lowest Value of the 
		//two compared Elements
		T newElement;
		if (*a1ptr == *a2ptr) {
			newElement = *a1ptr;
			a1ptr++;
			a2ptr++;
		}
		else if (*a1ptr < *a2ptr) {
			newElement = *a1ptr;
			a1ptr++;
		}
		else {
			newElement = *a2ptr;
			a2ptr++;
		}

		//Put newElement into Result Array
		*resultptr = newElement;
		resultptr++;
		unionSize++;

		//copy contents of a2 array to result array
		if (a1ptr == a1Endptr) {
			copyArr(resultptr, a2ptr, a2Endptr, unionSize);
			break;
		}

		//copy contents of a1 array to result array
		if (a2ptr == a2Endptr) {
			copyArr(resultptr, a1ptr, a1Endptr, unionSize);
			break;
		}
	}

	//return dynamic array
	return result;
}

template<typename T>
void copyArr(T* arr, T* start, T* stop, int& size) {
	while (start < stop) {
		//Put element into array
		*arr = *start;

		//Move Pointers
		start++;
		arr++;

		//Increase print Size
		size++;
	}
}

template<typename T>
void tree_to_inorder_arr(const tree_node<T>* root, 
int arr[], int& size)
{
	if (root != nullptr)
	{
		//Go to the end of the left tree
		tree_to_inorder_arr(root->_left, arr, size);

		//Add item to array
		arr[size] = root->_item;

		//Increment size
		size++;

		//Go to the end of the right tree
		//After going to the left
		tree_to_inorder_arr(root->_right, arr, size);
	}
}

template<typename T>
void tree_to_postorder_arr(const tree_node<T>* root, 
int arr[], int& size)
{
	if (root != nullptr)
	{
		//Go to the end of the left tree
		tree_to_inorder_arr(root->_left, arr, size);

		//Go to the end of the right tree
		//After going to the left
		tree_to_inorder_arr(root->_right, arr, size);

		//Add item to array
		arr[size] = root->_item;

		//Increment size
		size++;
	}
}

template<typename T>
tree_node<T>* rotate_left(tree_node<T>*& root)
{
	//Step 1: Hold Onto New Root
	tree_node<T>* newRoot = root->_right;

	//Step 2: Point current Root's right Child 
	//		  to new Root's Left Child
	root->_right = newRoot->_left;

	//Step 3: Point new Root's Left Child to old Root
	newRoot->_left = root;

	//Set new root
	root = newRoot;

	//Update Rotated Root
	root->_left->update_height();

	return root;
}

template<typename T>
tree_node<T>* rotate_right(tree_node<T>*& root)
{
	//Step 1: Hold Onto New Root
	tree_node<T>* newRoot = root->_left;

	//Step 2: Point current Root's left Child 
	//		  to new Root's right Child
	root->_left = newRoot->_right;

	//Step 3: Point new Root's right Child to old Root
	newRoot->_right = root;

	//Set new root
	root = newRoot;

	//Update Rotated Root
	root->_right->update_height();

	return root;
}

template<typename T>
tree_node<T>* rotate(tree_node<T>*& root)
{
	bool debug = false;

	if (root) {

		//Tree imbalanced
		if (abs(root->balance_factor()) >= 2) {

			//Right Heavy Root
			if (root->balance_factor() < 0) {
				if(debug) cout << "Rotate Right Root: " 
				<< root->_item << endl;

				//Left Leaning Child (Zig Zag)
				if (root->_right->balance_factor() == 1) {
					if (debug) cout << "Right Zig Zag"<< endl;

					rotate_right(root->_right);
					rotate_left(root);
				}

				//Right Leaning Child (One Rotate)
				else {
					if (debug) cout << "Right Leaning" << endl;
					rotate_left(root);
				}
			}

			//Left Heavy Root
			else {
				if (debug) cout << "Rotate Left Root: " 
				<< root->_item << endl;

				//Right Leaning Child (Zig Zag)
				if (root->_left->balance_factor() == -1) {
					if (debug) cout << "Left Zig Zag" << endl;
					rotate_left(root->_left);
					rotate_right(root);
				}

				//Left Leaning Child (One Rotate)
				else {
					if (debug) cout << "Left Leaning" << endl;
					rotate_right(root);
				}
			}
		}

		root->update_height();
	}

	return root;
}

#endif //AVL_H
