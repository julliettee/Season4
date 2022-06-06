#pragma warning(disable:4996)


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#define FILE_NAME_LENGTH 256
#define MESSAGE_LENGTH 20



int main() {
	FILE* binaryFile = NULL;
	char binFileName[FILE_NAME_LENGTH];
	printf("Enter binary file's name:\n");
	scanf("%s", binFileName);
	binaryFile = fopen(binFileName, "w+");
	if (binaryFile == NULL) {
		printf("Error creatng file!");
		return 0;
	}
	printf("Bin file was created!\n");
	int amountMessages;
	printf("Enter  amount of messages in file:\n");
	scanf("%d", &amountMessages);
	if (amountMessages <= 0) {
		printf("Incorrect value!");
		return 0;
	}
	int nSenders;
	printf("Enter senders amount:\n");
	scanf("%d", &nSenders);
	HANDLE processhandles[10];

	HANDLE hMutexSenders, hFileAccessMutex, hMutexReceiver;
	hMutexSenders = CreateSemaphoreA(NULL, 0, nSenders, L"SendersMutex");
	hFileAccessMutex = CreateMutexA(NULL, FALSE, L"FileAccess");
	hMutexReceiver = CreateMutexA(NULL, FALSE, L"ReceiverMutex");
	for (int i = 0; i < nSenders; i++) {
		char senderFullName[512];

		sprintf(senderFullName, "Sender.exe %s %d", binFileName, amountMessages);

		STARTUPINFO cif;
		ZeroMemory(&cif, sizeof(STARTUPINFO));
		cif.cb = (sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		 
		if (!CreateProcessA(NULL, senderFullName, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,NULL, NULL, &cif, &pi)) {
			printf("Can't create process!\n");
		}
		processhandles[i] = pi.hProcess;
	}


	for (int i = 0; i < nSenders; i++) {
		WaitForSingleObject(hMutexSenders, INFINITE);
	}

	printf("All processes were created!\n");


	BOOL go = TRUE;
	
	while (go) {
		printf("Choose option:\n");
		printf("1. Read file\n");
		printf("2. Finish work\n");

		int option = 0;
		scanf("%d", &option);

		switch (option) {
			case 1:
			{
				WaitForSingleObject(hMutexReceiver, INFINITE);
				WaitForSingleObject(hFileAccessMutex, INFINITE);
				binaryFile = fopen(binFileName, "rb");
				
				char line[MESSAGE_LENGTH];
				int msgCount = 0;
				while (fgets(line, MESSAGE_LENGTH, binaryFile)) {
					msgCount++;
					printf("%s", line);
				}
				binaryFile = fopen(binFileName, "w+");

				ReleaseMutex(hFileAccessMutex);
				ReleaseSemaphore(hMutexSenders, nSenders, NULL);
				break;
			}
			case 2:{
				go = FALSE;
				break;
			}
			default:
				break;
			}

	}

	for (int i = 0; i < nSenders; i++) {
		CloseHandle(processhandles[i]);
	}

	CloseHandle(hMutexReceiver);
	CloseHandle(hMutexSenders);
	CloseHandle(hFileAccessMutex);


}