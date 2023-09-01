#ifndef ONED_H
#define ONED_H

#include <iostream>
#include <string>
using namespace std;

template<typename T>
void init(T arr[], int size, T init_item);

template<typename T>
T* allocate(int capacity);

template<typename T>
T* reallocate(T* list, int size, int new_size);

template<typename T>
T* copy_allocate(T* poly, int size, int new_size);

template <typename T>
ostream& print(T* oneD, int size, ostream& outs = cout);

template<typename T>
T* search(T* list, int size, const T& target);

template<typename T>
void copy(T* dest, T* source, int& size);

template<typename T>
void append(T* arr, int& size, const T& item);

template<typename T>
T* add_entry(T* list, const T& new_entry, 
	int& size, int& capacity);

template<typename T>
T* remove_entry(T* list, const T& delete_me, 
	int& size, int& capacity);

template<typename T>
T* remove_index(T* list, const int& delete_index, 
	int& size, int& capacity);

template<typename T>
T* remove(T* list, int& size, int& capacity);

template<typename T>
void shift_left(T* list, int &size, T*& index_ptr);

template<typename T>
void shift_right(T* list, int &size, T*& index_ptr);

template<typename T>
T* allocate(int capacity) {
	//example of call in main: int arr* = allocate<int>(20);
	//Program to return a dynamic arry of ints of capacity 20

	//example of calling function in templated function
	//T arr* = allocate<T>(20);

	bool debug = false;
	if (debug) {
		cout << "Allocated a new list with capacity: "
			<< capacity << endl;
	}

	T* list = new T[capacity];

	//Fill list with lowest element of Type T
	init(list, capacity, T());

	return list;
}

template<typename T>
void init(T arr[], int size, T init_item) {
	for (T* walker = arr; walker - arr < size; walker++) {
		*walker = init_item;
	}
}


template<typename T>
T* reallocate(T* list, int size, int new_size)
{
	//Create new dynamic space with the capacity argument
	T* new_list = allocate<T>(new_size);

	//copy list to new list
	copy(new_list, list, size);

	//delete old list
	delete[] list;

	//set pointer to point to new list
	return new_list;
}

template<typename T>
T* copy_allocate(T* list, int size, int new_size)
{
	//Create new space for the list
	T *new_list = allocate<double>(new_size);

	//Copy items
	copy(new_list, list, size);

	return new_list;
}


template<typename T>
T* remove_entry(T* list, const T& delete_me, 
			   int& size, int& capacity) {
	T* return_list = list;

	//Create a new dynamic array with half the capacity
	//if size is 1/4 of the capacity
	int quarter_size = static_cast<int>(.25 * capacity);
	if (size <= quarter_size) {
		capacity /= 2;
		return_list = reallocate(list, size, capacity);
	}

	//get pointer to Item to delete
	T* delete_ptr = search(return_list, size, delete_me);

	//remove element from list by shifting elements over
	if(delete_ptr)
		shift_left(return_list, size, delete_ptr);

	return return_list;
}

template<typename T>
T* add_entry(T* list, const T& new_entry,
	int& size, int& capacity) {

	T* return_list = list;

	//Create a new dynamic array with double the capacity
	//if size is greater than or equal to half of the capacity
	if (size >= capacity / 2) {
		capacity *= 2;
		return_list = reallocate(list, size, capacity);
	}

	//Add new entry to list input
	append(return_list, size, new_entry);

	return return_list;
}

template<typename T>
T* remove_index(T* list, const int& delete_index,
				int& size, int& capacity) {

	T* return_list = list;

	//Create a new dynamic array with half the capacity
	//if size is 1/4 of the capacity
	int quarter_size = static_cast<int>(.25 * capacity);
	if (size <= quarter_size) {
		capacity /= 2;
		return_list = reallocate(list, size, capacity);
	}

	//get pointer to Item to delete
	T* delete_ptr = return_list + delete_index;

	//remove element from list by shifting elements over
	shift_left(return_list, size, delete_ptr);

	return return_list;
}

template<typename T>
T* remove (T* list, int& size, int& capacity) {
	T* return_list = list;

	//Create a new dynamic array with half the capacity
	//if size is 1/4 of the capacity
	int quarter_size = static_cast<int>(.25 * capacity);
	if (size <= quarter_size) {
		capacity /= 2;
		return_list = reallocate(list, size, capacity);
	}

	size--;

	return return_list;
}

template<typename T>
void append(T* arr, int& size, const T& item)
{
	// size: 4         V----walker: arr + 4
	// arr ->a b c d <item>
	T* walker = arr + size;

	//Add the element to the array
	*walker = item;

	//Increase size for the new array
	size++;
}

template <typename T>
ostream& print(T* oneD, int size, ostream& outs) {
	for (T* walker = oneD; walker - oneD < size; walker++) {
		outs << *walker;

		//Print Space if necessary
		if (walker - oneD < size-1) outs << " ";
	}

	//go through array backwards using pointers
	//for (T*walker = list + size - 1; 
	//walker - list >= 0; walker--) {
	//	cout << *walker << " ";
	//}
	//
	//cout << endl;

	return outs;
}

template<typename T>
T* search(T* list, int size, const T& target) {
	for (T* walker = list; walker - list < size; walker++) {

		if (target == *walker) {
			//returns the target element
			return walker;
		}
	}
	//returns null pointer if element not found in array
	return nullptr;
}

template<typename T>
void copy(T* dest, T* source, int& size) {

	//go through source array
	for (T *walker = source; walker - source < size; walker++) {

		//Copy contents of source item to destination array
		*dest = *walker;

		//Increase walker for destination array
		dest++;
	}
}

template<typename T>
void shift_left(T* list, int& size, T*& index_ptr) {

	//Get the item you want to shift over
	T* shift_item = index_ptr + 1;

	for (T* walker = index_ptr; walker - list < size; walker++) {
		*walker = *shift_item;
		shift_item++;
	}

	//decrease size by 1 to account for 
	//overwrite the item shifted over
	size--;
}

template<typename T>
void shift_right(T* list, int& size, T*& index_ptr) {

	//Pointer to last element
	T* back_ptr = list + size - 1;

	//Stoping point
	int stop_i = index_ptr - list;

	T* prev = back_ptr;

	//go through array backwards starting at 2nd last element
	for (T* walker = back_ptr - 1;
		walker - list >= stop_i; walker--) {

		//Set pointer to previous element to current walker
		*prev = *walker;

		//Decrement previous element pointer
		prev--;
	}
}


#endif // ONED_H