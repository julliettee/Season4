#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "Helpers.h"

extern int* array;
extern int arraySize;
extern enum enStatus* states;
extern int n;
extern HANDLE ghMutex;




void WINAPI marker(LPVOID params) {
	int id = (int)params - 1;
	srand(id + 1);
	printf("Thread %d started \n", id + 1);
	while (1) {
		while (states[id] != Active) {
		}
		int markedElementsCount = 0;
		WaitForSingleObject(ghMutex, INFINITE);
		for (int i = 0; i < arraySize; ++i) {
			if (array[i] == id + 1) {
				markedElementsCount++;
			}
		}
		ReleaseMutex(ghMutex);
		while (1) {
			int digit = rand();
			digit %= arraySize;
			WaitForSingleObject(ghMutex, INFINITE);
			if (array[digit] == 0) {
				markedElementsCount++;
				Sleep(5);
				array[digit] = id + 1;
				Sleep(5);
			}
			else {
				printf(" Thread: %d\n " \
					"Amount of marked elements: % d\n " \
					"Can't mark element %d \n",
					id + 1, markedElementsCount, digit);
				states[id] = Pause;
				ReleaseMutex(ghMutex);
				break;
			}
			ReleaseMutex(ghMutex);
		}

		while (states[id] == Pause) {
		}
		if (states[id] == Finishing) {
			WaitForSingleObject(ghMutex, INFINITE);
			for (int i = 0; i < arraySize; ++i) {
				if (array[i] == id + 1) {
					array[i] = 0;
				}
			}
			states[id] = Finished;
			ReleaseMutex(ghMutex);
			break;
		}
	}

}