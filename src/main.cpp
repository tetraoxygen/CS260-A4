#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <cstdlib>

const int NUM_STARTS = 3;
enum START_TYPES {RANDOM, INORDER, REVERSED};

const int NUM_LENGTHS = 4;
const int ARRAY_LENGTHS[NUM_LENGTHS] = { 100, 1000, 10000, 100000 };

// Array content generators  -------------

/**
*	Destructively fills an array with random numbers.
*		@param arr - the array to fill
*		@param arrSize - the size of the array
*/
void genRandomDataSet(int arr[], int arrSize);

/**
*	Destructively fills an array with in-order numbers.
*		@param arr - the array to fill
*		@param arrSize - the size of the array
*/
void genInOrderDataSet(int arr[], int arrSize);

/**
*	Destructively fills an array with reverse-order numbers.
*		@param arr - the array to fill
*		@param arrSize - the size of the array
*/
void genReverseDataSet(int arr[], int arrSize);

/**
*	Swaps 2 elements.
*		@param element1 - the first element to swap
*		@param element2 - the second element to swap
*/
void swap(int* element1, int* element2);

// Sorting algorithms -------------

/**
*	Helper function for QuickSort. Partitions a list, moving everything less than the element at the highIndex to the left of that index, and everything greater to the right.
*		@param arr - the array to partition
*		@param lowIndex - the lower bound of the sublist (as a list index)
*		@param highIndex - the upper bound of the sublist (as a list index)
*/
int partition(int arr[], int lowIndex, int highIndex);


/**
*	Sorts a list using the QuickSort algorithm.
*		@param arr - the array to sort
*		@param highIndex - the upper bound of the sublist (as a list index)
*		@param lowIndex - the low bound of the list (only for use of the function recursively, defaults to 0)
*/
void quickSort(int arr[], int highIndex, int lowIndex = 0);


int main() {
	// Open the file
	std::ofstream fout;
	fout.open("sorts.csv");

	typedef void (*arrGen)( int[], int ); // define a type for the generator functions
	static arrGen gen_function_pointers[NUM_STARTS] = { genRandomDataSet, genInOrderDataSet, genReverseDataSet }; // create an array of pointers to the array generator functions
	
	clock_t t;
	double elapsed;
	
	static int arr100[100];
	static int arr1000[1000];
	static int arr10000[10000];
	static int arr100000[100000];
	int *holdingArr[4] = {arr100, arr1000, arr10000, arr100000};
	
	fout << "QuickSort, ";
	for (int i = 0; i < NUM_LENGTHS; i++) {
		fout << ARRAY_LENGTHS[i];
		if (i + 1 != NUM_LENGTHS) { // Only show a comma if it's not the last of the lengths
			fout << ", ";
		}
	}
	fout << std::endl;
	
	
	for (int startType = 0; startType < NUM_STARTS; startType++) {
		switch(startType) {
			case RANDOM:
				fout << "Random, ";
				break;
			case INORDER:
				fout << "In Order, ";
				break;
			case REVERSED:
				fout << "Reversed, ";
				break;
			default:
				return -1;
		}
		
		for (int i = 0; i < NUM_LENGTHS; i++) {
			// Create the array
			int currentSize = ARRAY_LENGTHS[i];
			int *currentArr = holdingArr[i];
			
			// Fill it with the appropriate data
			(*gen_function_pointers[startType])(currentArr, currentSize);
			
			// Get the time before running the sort
			t = clock();
			
			// Sort the array
			quickSort(currentArr, currentSize);
			
			// Get the time elapsed, then convert it to seconds
			t = clock() - t;
			elapsed = ((double)t)/CLOCKS_PER_SEC;
			
			fout << elapsed << "s";
			
			if (i + 1 != NUM_LENGTHS) { // Only show a comma if it's not the last of the lengths
				fout << ", ";
			}
		}
		fout << std::endl;
	}
	fout.close(); // Close the file
}


// --------------------------- 
void genRandomDataSet(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		arr[i] = rand() % arrSize;
	}
}

// --------------------------- 
void genInOrderDataSet(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		arr[i] = i + 1; // i + 1 is so we start at 1
	}
}

// --------------------------- 
void genReverseDataSet(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		arr[i] = arrSize - i;
	}
}

// --------------------------- 
void quickSort(int arr[], int highIndex, int lowIndex) {
	if (lowIndex < highIndex) { // Make sure that the low index is actually lower than the high index
		int partitioningIndex = partition(arr, lowIndex, highIndex);
		
		quickSort(arr, partitioningIndex - 1, lowIndex); // Sort before the partition
		quickSort(arr, highIndex, partitioningIndex + 1); // Sort after the partition
	}
}

// --------------------------- 
int partition(int arr[], int lowIndex, int highIndex) {
	int pivot = arr[highIndex];
	int i = lowIndex - 1; // Index of smaller element
	
	// Iterate through the subarray
	for (int j = lowIndex; j < highIndex; j++) {
		if (arr[j] <= pivot) {
			i++; // Increment the index of the smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[highIndex]);
	return i + 1;
}

// --------------------------- 
void swap(int* element1, int* element2) {
	// Store the first element temporarily
	int temp = *element1;
	// Swap the elements
	*element1 = *element2;
	*element2 = temp;
}