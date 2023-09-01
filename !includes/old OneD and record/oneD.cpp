#include "oneD.h"

int num_elements(int* oneD)
{
	int size = 0;
	while (*oneD != -1) {
		oneD++;
		size++;
	}

	return size;
}