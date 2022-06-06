#pragma warning(disable:4996)


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>


int main(int argc, char** argv) {
	FILE* binaryFile = NULL;
	binaryFile = fopen(argv[1], "a");
	int maxAmountMessages = atoi(argv[2]);
	if (binaryFile == NULL) {
		printf("Error opening file!");
		return 0;
	}
	HANDLE hMutexSenders = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, L"SendersMutex");
	HANDLE hFileAccessMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, L"FileAccess");
	HANDLE hMutexReceiver = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, L"ReceiverMutex");

	if (hMutexSenders == NULL) {
		printf("Open mutex senders failed!");
		return 0;
	}
	if (hFileAccessMutex == NULL) {
		printf("Open mutex access failed!");
		return 0;
	}
	if (hMutexReceiver == NULL) {
		printf("Open mutex receiver failed!");
		return 0;
	}
	ReleaseSemaphore(hMutexSenders, 1, NULL);
	BOOL go = TRUE;
	while (go) {
			printf("Choose option:\n");
			printf("1. Write in file\n");
			printf("2. Finish work\n");

			int option = 0;
			scanf("%d", &option);

			switch (option) {
				case 1: {
					char msg[21];
					printf("Enter message\n");
					scanf("%s", &msg);
					msg[20] = '\0';
					printf("Waiting for access...\n");
					WaitForSingleObject(hMutexSenders, INFINITE);
					WaitForSingleObject(hFileAccessMutex, INFINITE);
					fprintf(binaryFile, "%s\n", msg);
					fflush(binaryFile);
					printf("Message was written!\n");
					ReleaseMutex(hFileAccessMutex);
					ReleaseMutex(hMutexReceiver);
					// ReleaseSemaphore(hMutexSenders, 1, NULL);
					break;

				}
			case 2: {
				go = FALSE;

			}
			default: {
				break;
			}
			}

	}

	CloseHandle(hMutexReceiver);
	CloseHandle(hMutexSenders);
	CloseHandle(hFileAccessMutex);

}