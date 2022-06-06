#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "Helpers.h"


#pragma warning(disable:4996)

int* array;
int arraySize;
int n;
enum enStatus* states;
HANDLE ghMutex;



void WINAPI marker(LPVOID params);

int CheckAnyActive() {
	int isActive = 0;
	for (int i = 0; i < n; ++i) {
		isActive |= (states[i] == Active);
	}
	return isActive;
}

int CheckAllFinished() {
	int isFinished = 1;
	for (int i = 0; i < n; ++i) {
		isFinished &= (states[i] == Finished);
	}
	return isFinished;
}

int main()
{
	HANDLE* threads;
	ghMutex = CreateMutex(NULL, FALSE, NULL);

	printf("Enter array size: \n");
	scanf("%d", &arraySize);

	array = (int*)malloc(arraySize * sizeof(int));
	for (int i = 0; i < arraySize; ++i) {
		array[i] = 0;
	}

	printf("Enter threads amount: \n");
	scanf("%d", &n);

	threads = (HANDLE*)calloc(n, sizeof(HANDLE));
	states = (enum enStatus*)calloc(n, sizeof(enum enStatus));

	for (int i = 0; i < n; ++i){
		int a = i + 1;
		states[i] = Pause;
		threads[i] = CreateThread(NULL, 0, marker, (void*)a, 0, NULL);
	}

	for (int i = 0; i < n; ++i) {
		states[i] = Active;
	}
	
	int go = 1;
	while (go) {
		while (CheckAnyActive()) {
		}

		WaitForSingleObject(ghMutex, INFINITE);
		printf("Current array values: ");
		for (int i = 0; i < arraySize; ++i) {
			printf("%d ", array[i]);
		}
		ReleaseMutex(ghMutex);

		printf("\nEnter markers number to finish its work\n");
		int index = 0;
		scanf("%d", &index);
		while (index < 0 || index > n || states[index - 1] == Finished) {
			printf("ID of marker should be from 1 till %d. \n"\
				"Thread can't be stopped, if it was stopped previosly.\n", n);
			printf("Enter correct value.\n");
			scanf("%d", &index);
		}
		states[index - 1] = Finishing;
		while (states[index - 1] != Finished) {
		}

		printf("Array values after finishing thread %d: \n", index);
		for (int i = 0; i < arraySize; ++i) {
			printf("%d ", array[i]);
		}
		printf("\n");

		for (int i = 0; i < n; ++i) {
			if (states[i] == Pause) {
				states[i] = Active;
			}
		}

		go = !CheckAllFinished();

	}
	CloseHandle(ghMutex);

}