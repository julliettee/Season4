#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#define SLEEP_MIN_MAX  7
#define SLEEP_AVERAGE  12
#define THREAD_AMOUNT  2

HANDLE ghMutex;

 struct ArrayData {
	int n;
	int* a;
	int min, max;
	int average;
} ;

int GetMin(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}
int GetMax(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}
DWORD WINAPI FindMinMax(LPVOID lpParam) {
	struct ArrayData* data = (struct ArrayData*)lpParam;
	data->min = data->a[0];
	data->max = data->a[0];
	for (int i = 0; i < data->n; ++i) {
		data->min = GetMin(data->min, data->a[i]);
		Sleep(SLEEP_MIN_MAX);
		data->max = GetMax(data->max, data->a[i]);
		Sleep(SLEEP_MIN_MAX);
	}
	WaitForSingleObject(ghMutex, INFINITE);
	printf("Thread finish work. Min = %d, Max = %d \n", data->min, data->max);
	ReleaseMutex(ghMutex);
}

DWORD WINAPI FindAverage(LPVOID lpParam) {
	struct ArrayData* data = (struct ArrayData*)lpParam;
	data->average = 0;
	for (int i = 0; i < data->n; ++i) {
		data->average += data->a[i];
		Sleep(SLEEP_AVERAGE);
	}
	data->average /= data->n;

	WaitForSingleObject(ghMutex, INFINITE);
	printf("Thread finish work. Average = %d \n", data->average);
	ReleaseMutex(ghMutex);

}


int main()
{
	
	struct ArrayData myData;
    printf("Enter a size:\n");

    scanf_s("%d", &myData.n);
    while (myData.n <= 0) {
        printf("Array size should be positive integer. Enter correct value.\n");
        scanf_s("%d", &myData.n);
    }
	myData.a = malloc(sizeof(int) * myData.n);
    if (myData.a == NULL) {
        printf("Somethig go wrong. Try again latter.");
        return 0;
    }
    printf("Enter a elements:\n");
    for (int i = 0; i < myData.n; i++) {
        scanf_s("%d", &myData.a[i]);
    }
	ghMutex = CreateMutex(NULL, FALSE, NULL);
	HANDLE threads[THREAD_AMOUNT];

	DWORD idMinMax, idAverage;

	threads[0] = CreateThread(NULL, 0, FindMinMax, &myData, 0, &idMinMax);
	if (threads[0] == NULL) {
		printf("Error: can't create MinMax thread.\n");
		return 1;
	}
	threads[1] = CreateThread(NULL, 0, FindAverage, &myData, 0, &idAverage);
	if (threads[1] == NULL) {
		printf("Error: can't create Average thread.\n");
		return 2;
	}

	printf("Threads are created.\nMinMax thread ID: %d\nAverage  thread ID: %d\n", idMinMax, idAverage);

	WaitForMultipleObjects(THREAD_AMOUNT, threads, TRUE, INFINITE);
	printf("All min & max elements are replaced with average value. \nResult: ");
	for (int i = 0; i < myData.n; i++) {
		if (myData.a[i] == myData.max || myData.a[i] == myData.min) {
			myData.a[i] = myData.average;
		}
		printf("%d ", myData.a[i]);
	}
	CloseHandle(threads[0]);
	CloseHandle(threads[1]);
	CloseHandle(ghMutex);
    free(myData.a);
}
