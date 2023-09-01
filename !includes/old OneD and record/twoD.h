#ifndef TWOD_H
#define TWOD_H

#include <iostream>
#include <cassert>
#include "oneD.h"

using namespace std;

bool index_is_valid(int* arr, int row, int col);

template <typename T>
T& get(T** twod, int row, int col);

template<typename T>
T* search(T** twod, int* sizes, T target);

template<typename T>
T** allocate(int* sizes);

template <typename T>
T read(T** twod, int row, int col);

template <typename T>
void write(T** twod, int row, int col, const T& item);

template <typename T>
ostream& print(T** twod, int* sizes, ostream& outs = cout);

template<typename T>
void init(T** twod, int* sizes, T init_item = T());

template<typename T>
void delete_arr(T** twod, int size);

template<typename T>
void get_row_col(T** twod, int* sizes, T target, int& row, int& col);

template<typename T>
T** allocate(int* sizes) {

	int capacity = num_elements(sizes) + 1;

	//Create a 2d Pointer
	T** twoD = new T * [capacity];

	for (T** walker = twoD; walker - twoD < capacity; walker++, sizes++) {

		//Add null pointer to the end
		if (walker - twoD == capacity - 1)
			* walker = nullptr;

		//Create an array with x Columns
		else
			*walker = new T[*sizes];
	}

	return twoD;
}

template <typename T>
T read(T** twod, int row, int col) {

	//END Program if index invalid
	//assert(index_is_valid(*twod, row, col));

	//Go through pointer pointer 
	//Go to row in pointer pointer
	twod += row;

	//Derefrence pointer pointer
	//Derefrenced pointer pointer is an array
	//Go to column number in that array
	//return *twod + col;

	T* value_ptr = *twod + col;

	return *value_ptr;
}

template <typename T>
T& get(T** twod, int row, int col) {

	//END Program if index invalid
	//assert(index_is_valid(*twod, row, col));

	//Go through pointer pointer 
	//Go to row in pointer pointer
	twod += row;

	//Derefrence pointer pointer
	//Derefrenced pointer pointer is an array
	//Go to column number in that array

	T* value_ptr = *twod + col;

	return *value_ptr;
}

template <typename T>
void write(T** twod, int row, int col, const T& item) {
    
    //END Program if index invalid
    //assert(index_is_valid(*twod, row, col));
    
    
	//Go through pointer pointer 
	//Go to row in pointer pointer
	twod += row;

	//Derefrence pointer pointer
	//Derefrenced pointer pointer is an array
	//Go to column number in that array
	T* walker = *twod + col;

	//Set item
	*(walker) = item;
}

template <typename T>
ostream& print(T** twod, int* sizes, ostream& outs) {
	for (twod; *twod != nullptr; twod++, sizes++) {
		print(*twod, *sizes);
	}
	outs << endl;

	return outs;
}

template<typename T>
void init(T** twod, int* sizes, T init_item) {

	//Walker not needed as using null ptr to find end of array
	for (twod; *twod != nullptr; twod++, sizes++) {
		init(*twod, *sizes, init_item);
	}
}

template<typename T>
T* search(T** twod, int* sizes, T target) {

	//Go through pointer pointer (array starting value)
	for (twod; *twod != nullptr; twod++, sizes++) {

		//Go through array
		T* item = search(*twod, *sizes, target);

		//Return pointer to item if found
		if (item)
			return item;
	}

	return nullptr;
}

template<typename T>
void get_row_col(T** twod, int* sizes, T target, int &row, int &col) {

	int row_count = 0;
	//Go through pointer pointer (array starting value)
	for (twod; *twod != nullptr; twod++, sizes++) {

		//Go through array
		for (T* walker = *twod; walker - *twod < *sizes; walker++) {

			if (target == *walker) {
				row = row_count;
				col = walker - *twod;
			}
		}
		row_count++;
	}
}

template<typename T>
void delete_arr(T** twod, int size) {

	for (T** walker = twod; walker - twod < size; walker++) {
		delete[] *walker;
	}

	delete[] twod;
}

#endif // TWOD_H