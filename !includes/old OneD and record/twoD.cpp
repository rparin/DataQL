#include "twoD.h"

bool index_is_valid(int* arr, int row, int col) {

	//Get number of Rows in array
	int num_rows = num_elements(arr);

	//Check Row
	if (row > num_rows)
		return false;

	//Get Number of Columns in Row
	int *walker = arr + row;
	int num_col = *walker;

	//Check Column
	if (col >= num_col)
		return false;
		
	else
		return true;
}