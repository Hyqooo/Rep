#include <stdio.h>
#include <stdlib.h>

typedef struct Array{
	int *numberArray;
	int heap_size;
	int length;
}array_t;

int heapSort(array_t);
int buildHeap(array_t);
int heapift(array_t, int);
int swap(array_t, int, int);

int main(){
	int i;
	array_t heap;

	heap.numberArray = NULL;

	printf("Input size of the array: ");
	scanf("%d", &heap.length);
	heap.heap_size = heap.length;

	heap.numberArray = (int*)malloc((heap.length + 1) * sizeof(int));
	if (!heap.numberArray) return 1;

	fillArray(heap);

	heapSort(heap);

	printf("Sorted array: ");
	for (i = 0; i < heap.length; i++){
		printf("%d ", heap.numberArray[i]);
	}
	printf("\n");

	if (heap.numberArray[0] == 1){
		for (i = 1; i < heap.length; i++){
			if (heap.numberArray[i] - heap.numberArray[i - 1] > 1){
				printf("There's no number %d\n", heap.numberArray[i - 1] + 1);
			}
		}
	}else{
		printf("There's no number 1\n");
	}

	free(heap.numberArray);
}

int fillArray(array_t heap){
	int i, temp;

	printf("Input numbers: ");
	for (i = 0; i < heap.length; i++){
		if (scanf("%d", &temp) == 1){
			heap.numberArray[i] = temp;
		}else{
			heap.length = i;
			break;
		}
	}
}

int heapify(array_t heap, int nodeN){
	int left, right, largest;
	
	left = 2 * nodeN + 1;
	right = 2 * nodeN + 2;

	if (left <= heap.heap_size - 1 && heap.numberArray[left] > heap.numberArray[nodeN]){
		largest = left;
	}else{
		largest = nodeN;
	}

	if (right <= heap.heap_size - 1 && heap.numberArray[right] > heap.numberArray[largest]){
		largest = right;
	}

	if (largest != nodeN){
		swap(heap, nodeN, largest);
		heapify(heap, largest);
	}

	return 0;
}

int buildHeap(array_t heap){
	int i;

	heap.heap_size = heap.length;
	for (i = heap.length / 2; i > -1; i--){
		heapify(heap, i);
	}

	return 0;
}

int heapSort(array_t heap){
	int i;
	
	buildHeap(heap);

	for (i = heap.length - 1; i; i--){
		swap(heap, 0, i);
		heap.heap_size--;
		heapify(heap, 0);
	}
	
	return 0;
}

int swap(array_t heap, int n_1, int n_2){
	int temp;

	temp = heap.numberArray[n_1];
	heap.numberArray[n_1] = heap.numberArray[n_2];
	heap.numberArray[n_2] = temp;

	return 0;
}
