#ifndef ARRAY_UTILITY_FUNCTIONS_H
#define ARRAY_UTILITY_FUNCTIONS_H

#include <iostream>
#include <cassert>
using namespace std;

//return the larger of the two items
template <typename T>
T maximal(const T& a, const T& b);    

//return index of the largest item in data
template <typename T>
int index_of_maximal(T data[], int n);                 

//swap the two items
template <typename T>
void _swap(T& a, T& b);  

//return the index of the first element 
//in data that is not less than entry
template <typename T>
int first_ge(const T data[], int size, const T& entry); 

//insert entry into the sorted array
//data with length n, assumes array is sorted
template <typename T>
void ordered_insert(T data[], int& n, T entry);        

//append entry to the right of data
template <typename T>
void attach_item(T data[], int& n, const T& entry);    

//insert entry at index i in data
template <typename T>
void insert_item(T data[], int i, int& n, T entry);    

//remove the last element in data and place
//it in entry
template <typename T>
void detach_item(T data[], int& n, T& entry);

//delete item at index i and place it in entry
template <typename T>
void delete_item(T data[], int i, int& n, T& entry);   

//move n/2 elements from the 
//right of data1 to data2
template <typename T>
void split(T data1[], int& n1, T data2[], int& n2);

//append data2 to the right of data1
template <typename T>
void merge(T data1[], int& n1, T data2[], int& n2);   

//item > all data[i]
template <typename T>
bool is_gt(const T data[], int n, const T& item);       

//item <= all data[i]
template <typename T>
bool is_le(const T data[], int n, const T& item);       

//copy src[] into dest[]
template <typename T>
void copy_array(T dest[], const T src[],
	int& dest_size, int src_size);

//Shifts a portion of the array to the left
template<typename T>
void shift_left(T arr[], int& size, T* index_ptr);

//Shifts a portion of the array to the right
template<typename T>
void shift_right(T ar[],const int& size, T* index_ptr);

//print array data
//Highlights item at pos
template <typename T>
void print_array(const T data[], int n, int pos = -1);

template<typename T>
T maximal(const T& a, const T& b)
{
	//A is greater item
	if (a > b) return a;

	//B is greater item
	else return b;
}

template<typename T>
int index_of_maximal(T data[], int n)
{
	assert(n > 0);

	//Store Maximum
	int index = 0;
	T max = data[index];

	//Go through array and get max item index
	for (int i = 1; i < n; i++) {
		if (data[i] > max) {
			max = data[i];
			index = i;
		}
	}

	return index;
}

template<typename T>
void _swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<typename T>
int first_ge(const T data[], int size, const T& entry)
{
	if (size == 0) return size;
	
	//Item is larger than the last element
	if (entry > data[size - 1]) return size;
	
	//Get Index for item not less than entry
	int index = 0;
	for (int i = 0; i < size; i++) {
		index++;
		if (entry <= data[i]) break;
	}

	//Decrement index for prev item
	if(index > 0) index--;

	return index;
}

template<typename T>
void ordered_insert(T data[], int& n, T entry)
{
	int i = first_ge(data, n, entry);
	insert_item(data, i, n, entry);
}

template<typename T>
void attach_item(T data[], int& n, const T& entry)
{
	//Put entry in last spot in data
	data[n] = entry; n++;
}

template<typename T>
void insert_item(T data[], int i, int& n, T entry)
{
	//Shift Array To the right
	n++; shift_right(data, n, &data[i]);

	//Insert item
	data[i] = entry; 
}

template<typename T>
void detach_item(T data[], int& n, T& entry)
{
	//Put last item in data in entry
	entry = data[n-1]; n--;
}

template<typename T>
void delete_item(T data[], int i, int& n, T& entry)
{
	entry = data[i];
	shift_left(data, n, &data[i]);
}

template<typename T>
void split(T data1[], int& n1, T data2[], int& n2) {

	int rn1 = n1 / 2; //right half size
	n1 = n1 - rn1; //new data 1 size

	//Merge data1 right half to data2 array
	merge(data2, n2, &data1[n1], rn1);
}

template<typename T>
void merge(T data1[], int& n1, T data2[], int& n2) {

	//Go Through data2 and add to data1
	for (int i = 0; i < n2; i++) {
		attach_item(data1, n1, data2[i]);
	}
}

template<typename T>
bool is_gt(const T data[], int n, const T& item)
{
	//Go Through array and compare 
	//if item is less array item
	for (int i = 0; i < n; i++) {
		if (item <= data[i]) return false;
	}

	return true;
}

template<typename T>
bool is_le(const T data[], int n, const T& item)
{
	//Item is less than or equal to item 
	//if not greater than
	return (!(is_gt(data, n, item)));
}

template<typename T>
void copy_array(T dest[], const T src[], 
	int& dest_size, int src_size) {

	//assert(dest_size >= src_size);

	//Set new Size
	dest_size = src_size;

	//Go through src array and copy items to dest
	for (int i = 0; i < src_size; i++) {
		dest[i] = src[i];
	}
}

template<typename T>
void shift_left(T arr[], int& size, T* index_ptr) {

	//Get the item you want to shift over
	T* shift_item = index_ptr + 1;

	for (T* walker = index_ptr; walker - arr < size; walker++) {
		*walker = *shift_item;
		shift_item++;
	}

	//decrease size by 1 to account for 
	//overwrite the item shifted over
	size--;
}

template<typename T>
void shift_right(T arr[],const int& size, T* index_ptr) {

	//Pointer to last element
	T* back_ptr = arr + size - 1;

	//Stoping point
	int stop_i = index_ptr - arr;

	T* prev = back_ptr;

	//go through array backwards starting at 2nd last element
	for (T* walker = back_ptr - 1;
		walker - arr >= stop_i; walker--) {

		//Set pointer to previous element to current walker
		*prev = *walker;

		//Decrement previous element pointer
		prev--;
	}
}

template<typename T>
void print_array(const T data[], int n, int pos)
{
	for (int i = 0; i < n; i++) {

		//Highlight Item at index
		if (i == pos) cout << "[" << data[i] << "]";

		//Print Item normally
		else cout << data[i];

		cout << " ";
	}
	cout << endl;
}

#endif //ARRAY_UTILITY_FUNCTIONS_H
