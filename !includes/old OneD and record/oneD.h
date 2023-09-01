#ifndef ONED_H
#define ONED_H

#include <iostream>
using namespace std;

int num_elements(int* arr);

template <typename T>
ostream& print(T* oneD, int size, ostream& outs = cout);

template<typename T>
T* search(T arr[], int size, T target);

template<typename T>
T* allocate(int capacity);

template<typename T>
void init(T arr[], int size, T init_item);

template<typename T>
void append(T* arr, int& size, T item);

template<typename T>
void append(T* arr, int& size, T item)
{
	// size: 4         V----walker: arr + 4
	// arr ->a b c d <item>
	T* walker = arr + size;

	//Add the element to the array
	*walker = item;

	//Increase size for the new array
	size++;
}

template<typename T>
T* allocate(int capacity) {
	//example of calling function in main: int arr* = allocate<int>(20);
	//This tells the program to return a dynamic arry of ints with a capacity of 20

	//example of calling function in templated function
	//T arr* = allocate<T>(20);

	//cout << "Allocated a new list with capacity: " << capacity << endl;
	return new T[capacity];
}

template <typename T>
ostream& print(T* oneD, int size, ostream& outs) {
	for (T* walker = oneD; walker - oneD < size; walker++) {
		outs << *walker << " ";
	}
	outs << endl;

	return outs;
}

template<typename T>
void init(T arr[], int size, T init_item) {
	for (T* walker = arr; walker - arr < size; walker++) {
		*walker = init_item;
	}
}

template<typename T>
T* search(T arr[], int size, T target) {
	for (T* walker = arr; walker - arr < size; walker++) {

		if (target == *walker) {
			//returns the target element
			return walker;
		}
	}
	//returns null pointer if element not found in array
	return nullptr;
}

#endif // ONED_H