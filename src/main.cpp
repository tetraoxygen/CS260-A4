#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <cstdlib>

const int NUM_STARTS = 3;
enum START_TYPES { RANDOM, INORDER, REVERSED };
const std::string FRIENDLY_START_NAMES[NUM_STARTS] = { "Random", "In Order", "Reversed" };

const int NUM_SORTS = 3;
enum SORT_TYPES { QUICK, BUBBLE, INSERTION };
const std::string FRIENDLY_SORT_NAMES[NUM_SORTS] = { "QuickSort", "BubbleSort", "InsertionSort" };

const int NUM_LENGTHS = 4;
const int ARRAY_LENGTHS[NUM_LENGTHS] = { 100, 1000, 10000, 100000 };

typedef void (*arrGen)( int[], int ); // define a type for the generator functions
typedef void (*sorter)( int[], int ); // define a type for the sorting functions

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
void quickSortRecursive(int arr[], int highIndex, int lowIndex);

/**
*	Sorts a list using the QuickSort algorithm by calling quickSortRecursive
*		@param arr - the array to sort
*		@param size - the size of the array
*/
void quickSort(int arr[], int size);

/**
*	Sorts a list using the BubbleSort algorithm.
*		@param arr - the array to sort
*		@param size - the size of the array
*/
void bubbleSort(int arr[], int size);

/**
*	Sorts a list using the InsertionSort algorithm.
*		@param arr - the array to sort
*		@param size - the size of the array
*/
void insertionSort(int arr[], int size);

static arrGen gen_function_pointers[NUM_STARTS] = { genRandomDataSet, genInOrderDataSet, genReverseDataSet }; // create an array of pointers to the array generator functions
static sorter sort_function_pointers[NUM_SORTS] = { quickSort, bubbleSort, insertionSort }; // create an array of pointers to the array generator functions

int main() {
	// Open the file
	std::ofstream fout;
	fout.open("sorts.csv");
	
	clock_t t;
	double elapsed;
	
	static int arr100[100];
	static int arr1000[1000];
	static int arr10000[10000];
	static int arr100000[100000];
	int *holdingArr[4] = {arr100, arr1000, arr10000, arr100000};
	
	for (int sortType = 0; sortType < NUM_SORTS; sortType++) {

		fout << FRIENDLY_SORT_NAMES[sortType] << ", ";
		for (int i = 0; i < NUM_LENGTHS; i++) {
			fout << ARRAY_LENGTHS[i];
			if (i + 1 != NUM_LENGTHS) { // Only show a comma if it's not the last of the lengths
				fout << ", ";
			}
		}
		fout << std::endl;
	
		for (int startType = 0; startType < NUM_STARTS; startType++) {
			fout << FRIENDLY_START_NAMES[startType] << ", ";
			
			for (int i = 0; i < NUM_LENGTHS; i++) {
				// Create the array
				int currentSize = ARRAY_LENGTHS[i];
				int *currentArr = holdingArr[i];
				
				// Fill it with the appropriate data
				(*gen_function_pointers[startType])(currentArr, currentSize);
				
				// Get the time before running the sort
				t = clock();
				
				// Sort the array
				(*sort_function_pointers[sortType])(currentArr, currentSize);
				
				// Get the time elapsed, then convert it to milliseconds
				t = clock() - t;
				elapsed = (((double)t)/CLOCKS_PER_SEC) * 1000;
				
				fout << elapsed;
				
				if (i + 1 != NUM_LENGTHS) { // Only show a comma if it's not the last of the lengths
					fout << ", ";
				}
			}
			fout << std::endl;
		}
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
void swap(int* element1, int* element2) {
	// Store the first element temporarily
	int temp = *element1;
	// Swap the elements
	*element1 = *element2;
	*element2 = temp;
}

// --------------------------- 
void quickSort(int arr[], int highIndex) {
	quickSortRecursive(arr, highIndex, 0);
}

// --------------------------- 
void quickSortRecursive(int arr[], int highIndex, int lowIndex) {
	if (lowIndex < highIndex) { // Make sure that the low index is actually lower than the high index
		int partitioningIndex = partition(arr, lowIndex, highIndex);
		
		quickSortRecursive(arr, partitioningIndex - 1, lowIndex); // Sort before the partition
		quickSortRecursive(arr, highIndex, partitioningIndex + 1); // Sort after the partition
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
void bubbleSort(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		bool hasSwapped = false;
		for (int j = 0; j < size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(&arr[j], &arr[j+1]);
				hasSwapped = true;
			}
		}
		if (!hasSwapped) { // There were no swaps in this pass, so the array was sorted
			break;
		}
	}
}

// --------------------------- 
void insertionSort(int arr[], int size) {
	int j;
	for (int i = 0; i < size; i++) {
		int key = arr[i];
		
		j = i - 1;
		
		while (j > 0 && arr[j] == key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}