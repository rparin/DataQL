#ifndef SORT_FUNCTIONS_H
#define SORT_FUNCTIONS_H

#include <iostream>
#include <ctime>
#include <string>
#include <cassert>
#include "myHeap.h"
using namespace std;

template <typename T>
void bubble_sort(T arr[], unsigned int size);

template <typename T>
void insertion_sort(T arr[], unsigned int size);

template <typename T>
void quick_sort(T arr[], unsigned int size);

template <typename T>
unsigned int qPartition(T arr[], unsigned int size);

template<typename T>
void merge_sort(T arr[], unsigned int size);

template<typename T>
void merge(T arr[], unsigned int size);

template <typename T>
void heap_sort(T arr[], unsigned int size);

//Merges Two arrays to a third array 
//Includes Duplicates
template<typename T>
T* Union(T* a1, int size1, T* a2, int size2, int& newSize);

//Copy a portion of one array to another
template<typename T>
void copyArr(T* arr, T* start, T* stop, int& size);

//Exchanges two items
template <typename T>
void _swap(T& item1, T& item2);

//shuffle elements of a[]
template <typename T>
void shuffle(T a[], unsigned int  size);

//Copy elements from one array to another
template <typename T>
void copy_array(T dest[], const T src[],
	unsigned int size);

//return true if a[] is in ascending order
template <typename T>
bool verify(const T a[], unsigned int  size);

//return a string "SORTED" or 
//"NOT SORTED" depending if a[] is sorted
template <typename T>
string verifystr(const T a[], unsigned int  size);

//Prints all items in array
template <typename T>
void print(T arr[], unsigned int size);

//print a[] from start to end
template <typename T>
void print_array_segment(T a[], unsigned int start,
	unsigned int end);

template <typename T>
void bubble_sort(T arr[], unsigned int size) {

	//Get starting element 
	T prev = arr[0];

	bool swapped = true;

	//When no swap occurs then array is sorted
	while (swapped) {

		//Reset swap
		swapped = false;

		//Go through array and 
		//Compare previous element to current element
		for (unsigned int i = 1; i < size; i++) {

			if (prev > arr[i]) {
				_swap(arr[i - 1], arr[i]);
				swapped = true;
			}
			else prev = arr[i];

		}

		//Reset prev element
		prev = arr[0];
	}
}

template <typename T>
void insertion_sort(T arr[], unsigned int size) {

	unsigned int tempIndex = 0;

	//Go through array
	for (unsigned int i = 1; i < size; i++) {

		//Continuously Move item back (Insert)
		while (arr[tempIndex] > arr[i]) {

			_swap(arr[tempIndex], arr[i]);

			//Done Sorting
			if (tempIndex == 0) break;
			tempIndex--; i--;
		}

		//Increment temp index 
		tempIndex++;
	}
}

template <typename T>
void quick_sort(T arr[], unsigned int size) {

	if (size > 0) {
		
		//sort array
		unsigned int p = qPartition(arr, size);
		
		//sort left half of array
		quick_sort(arr, p - 1);
		
		//sort right half of array
		quick_sort(&arr[p], size - p);

	}
}

template<typename T>
unsigned int qPartition(T arr[], unsigned int size)
{
	//Starting left items
	T* lItemPtr = arr + 1;

	//go through array using walkers
	for (T* walker = arr + 1; walker - arr <
		static_cast<int>(size); walker++) {

		//Put items to the start of array if less than pivot
		if (*walker < *arr) {
			_swap(*walker, *lItemPtr);
			lItemPtr++;
		}
	}

	//Put pivot item to its rightful place
	T* pPtr = lItemPtr - 1;
	_swap(*arr, *pPtr);

	//Return Index to Pivot
	return lItemPtr - arr;
}

template<typename T>
void merge_sort(T arr[], unsigned int size)
{
	if (size > 0) {

		//Get mid
		unsigned int mid = size / 2;

		//Sort Left half
		merge_sort(arr, mid);

		//Sort Right Half
		merge_sort(&arr[mid +1], size - mid - 1);

		//Merge Left and Right
		merge(arr, size);
	}
}

template<typename T>
void merge(T arr[],unsigned int size)
{
	unsigned int a1S = 0;
	unsigned int a2S = 0;
	unsigned int index = 0;
	unsigned int mid = size / 2;

	//Create Left Array
	T* a1 = allocate<T>(size);
	for (T* walker = arr;
		walker - arr < static_cast<int>(mid+1);
		walker++, index++) {

		a1[index] = *walker;

		//Increase size of a1 array
		a1S++;
	}

	//Create Right Array
	index = 0;
	T* a2 = allocate<T>(size);
	for (T* walker = arr + mid +1; 
		walker - arr < static_cast<int>(size+1);
		walker++, index++) {

		a2[index] = *walker;

		//Increase size of a2 array
		a2S++;
	}

	//Size of Merged Arrays
	int uSize = 0;

	//Modified To Include Duplicates
	T* result = Union(a1, a1S, a2, a2S, uSize);

	//Set Merged Array to Original array
	copy_array(arr, result, uSize);

	//delete dynamically created arrays
	delete[] a1;
	delete[] a2;
	delete[] result;
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
		//set newElement to the 
		//lowest Value of the two compared Elements
		T newElement;
		if (*a1ptr == *a2ptr) {
			newElement = *a1ptr;
			a1ptr++;
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
void heap_sort(T arr[], unsigned int size)
{
	Heap<T> _arr(size);

	//Insert Items to Heap
	for (unsigned int i = 0; i < size; i++) {
		_arr.push(arr[i]);
	}

	//Copy Items to array
	for (int i = size - 1; i >= 0; i--) {
		arr[i] = _arr.pop();
	}
}

template <typename T>
void _swap(T& item1, T& item2) {
	T temp = item1;
	item1 = item2;
	item2 = temp;
}

template<typename T>
void shuffle(T a[], unsigned int size) {

	if (size <= 1) return;

	//Random seed for random numbers each compile run
	srand(static_cast<unsigned>(time(0)));

	unsigned int i1 = 0, i2 = 0;
	for (unsigned int i = 0; i < size * 3; i++) {

		//Get 2 random indexes
		while (i1 == i2) {
			i1 = rand() % size;
			i2 = rand() % size;
		}

		//Ensure indexes are within bounds
		assert(i1 < size && i2 < size);

		//Swap items
		_swap(a[i1], a[i2]);

		//Reset Indexes
		i1 = 0; i2 = 0;
	}
}

template<typename T>
void copy_array(T dest[], const T src[],
	unsigned int size) {
	if (size == 0) return;
	for (unsigned int i = 0; i < size; i++) {
		dest[i] = src[i];
	}
}

template<typename T>
bool verify(const T a[], unsigned int size)
{
	//Get starting element 
	T prev = a[0];

	//Go through array and 
	//Compare previous element to current element 
	for (unsigned int i = 1; i < size; i++) {

		if (prev > a[i]) return false;

		else prev = a[i];
	}
	return true;
}

template<typename T>
string verifystr(const T a[], unsigned int size)
{
	if (verify(a, size)) return "SORTED";
	else return "NOT SORTED";
}

template <typename T>
void print(T arr[], unsigned int size) {

	for (unsigned int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

template<typename T>
void print_array_segment(T a[], unsigned int start,
	unsigned int end) {

	for (unsigned int i = start; i < end; i++) {
		cout << a[i] << " ";
	}
	cout << endl;
}

#endif //SORT_FUNCTIONS_H
