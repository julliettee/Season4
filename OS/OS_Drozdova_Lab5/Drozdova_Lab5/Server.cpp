#pragma warning(disable:4996)

#define FILE_NAME 255

#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include "Helper.h"

using namespace std;

char filename[FILE_NAME];
fstream file;


HANDLE* hWriteAccess;
HANDLE* hReadAccess;
int* countOfReaders;
CRITICAL_SECTION cs;


bool Finder(int id, employee& person, DWORD& pos) {
	EnterCriticalSection(&cs);
	file.seekg(0, ios::beg);
	int N;
	file.read((char*)&N, sizeof(int));
	for (pos = 0; pos < N; ++pos) {
		file.read((char*)&person, sizeof(person));
		if (person.id == id)
			break;
	}
	LeaveCriticalSection(&cs);
	if (pos == N)
		return false;
	return true;
}

void ReadFile(employee& person, DWORD pos) {
	EnterCriticalSection(&cs);
	file.seekp(sizeof(int) + pos * sizeof(person), ios::beg);
	file.read((char*)&person, sizeof(person));
	LeaveCriticalSection(&cs);
}


void WriteFile(employee person, DWORD pos) {
	EnterCriticalSection(&cs);
	file.seekp(sizeof(int) + pos * sizeof(person), ios::beg);
	file.write((char*)&person, sizeof(person));
	LeaveCriticalSection(&cs);
}

DWORD WINAPI WorkingThreadFunc(LPVOID param) {
	NamedPipeInfo pr = *(NamedPipeInfo*)param;
	command my_command;
	DWORD bytes;
	DWORD pos;
	do {
		ReadFile(pr.hMyPipe, (char*)&my_command, sizeof(my_command), &bytes, 0);
		cout << "i read your command!\n";
		if (my_command.type == 0) { // ������ �� ��������
			cout << "client want to read...\n";
			employee person;
			my_command.result = Finder(my_command.id, person, pos);
			WriteFile(pr.hMyPipe, &my_command, sizeof(my_command), &bytes, 0);
			if (my_command.result) {
				cout << "i can read!\n";
				countOfReaders[pos]++;
				WaitForSingleObject(hWriteAccess[pos], INFINITE);		//���� ���� ���������� ������
				ResetEvent(hReadAccess[pos]); // ������� �� �� ��� ������ � �� �������� ���
				cout << "start reading...\n";
				ReadFile(person, pos);
				cout << "i finish reading\n";
				cout << "i'm writing data in pipe\n";
				WriteFile(pr.hMyPipe, &person, sizeof(person), &bytes, 0); // �������� �� �� ��� ��������
				cout << "i wrote data in pipe...\n";
				countOfReaders[pos]--;
				if (!countOfReaders[pos]) {
					SetEvent(hReadAccess[pos]); // �������, �� �� �������� � ������ �������, ���� �����
				}
			}
			else {
				cout << "i can't read!\n";
			}
		}
		else if (my_command.type == 1) { // ������ �� ������
			employee person;
			cout << "client want to write...\n";
			my_command.result = Finder(my_command.id, person, pos); // ���� ������ � ������ ��������
			WriteFile(pr.hMyPipe, &my_command, sizeof(my_command), &bytes, 0); // ������� ���� ����� ����� �� ��
			if (my_command.result) {
				cout << "i can write!\n";
				cout << "waiting...\n";
				WaitForSingleObject(hReadAccess[pos], INFINITE);		//���� ���� ���������� ������
				WaitForSingleObject(hWriteAccess[pos], INFINITE);		//���� ���� ���������� ������
				ResetEvent(hWriteAccess[pos]); // �������, ��� �� �����, ����� ����� �� ������ ��� ������
				ReadFile(person, pos);	// ������ �� ������
				WriteFile(pr.hMyPipe, &person, sizeof(person), &bytes, 0); // ������� ������� �� ��� ������
				//��������� ������
				ReadFile(pr.hMyPipe, (char*)&person, sizeof(person), &bytes, 0); // ������ ���������, �� �� ����� ��������
				WriteFile(person, pos);
				cout << "new data written\n";
				SetEvent(hWriteAccess[pos]);
			}

		}
	} while (my_command.type != 3);
	
	DisconnectNamedPipe(pr.hMyPipe);
	return 0;
}

void FileCreation() {
	cout << "Enter name of binary file: ";
	cin.getline(filename, FILE_NAME - 1);
	file.open(filename, ios::binary | ios::out | ios::in | ios::trunc);
	cout << "Enter amount of records: ";
	int N;
	cin >> N;
	hWriteAccess = new HANDLE[N];
	hReadAccess = new HANDLE[N];
	countOfReaders = new int[N];
	file.write((char*)&N, sizeof(int));
	for (int i = 0; i < N; ++i) {
		countOfReaders[i] = 0;
		hWriteAccess[i] = CreateEvent(0, TRUE, TRUE, NULL);
		hReadAccess[i] = CreateEvent(0, TRUE, TRUE, NULL);
		employee st;
		cout << "\n--------- Record #" << i + 1 << " ---------\n";
		cout << "Enter employee's id: ";
		cin >> st.id;
		cout << "Enter name: ";
		cin.get();
		cin.getline(st.name, 9);
		cout << "Enter hours: ";
		cin >> st.hours;
		file.write((char*)&st, sizeof(st));
	}
}

int main() {
#pragma region init
	char 	c;	// ��������� ������
	SECURITY_ATTRIBUTES sa;	// �������� ������
	SECURITY_DESCRIPTOR sd;	// ���������� ������
	vector <HANDLE> vhNamedPipe;
	char 	lpszInMessage[80];	// ��� ��������� �� �������
	DWORD 	dwBytesRead;		// ��� ����� ����������� ����
	char 	lpszOutMessage[] = "The server has received a message."; // �������� ���������
	DWORD 	dwBytesWrite;		// ��� ����� ���������� ����

	// ������������� ��������� ������
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;	// ���������� ������ �������������
	// �������������� ���������� ������
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	// ������������� �������� ������, �������� ������ ���� �������������
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	InitializeCriticalSection(&cs);
	FileCreation();
#pragma endregion

	int nClient;
	std::cout << "Enter amount of clients: \n";
	std::cin >> nClient;	

	HANDLE* hThread = new HANDLE[nClient];
	DWORD* ThreadID = new DWORD[nClient];
	NamedPipeInfo* allPipes = new NamedPipeInfo[nClient];
	for (int i = 0; i < nClient; i++) {
		std::cout << "Process #" << i + 1 << " info\n";
		allPipes[i].myNum = i;

#pragma region create named pipe
		char 	pipeName[80];
		wsprintfA(pipeName, "\\\\.\\pipe\\demo_pipe%d",i);

		HANDLE 	hNamedPipe = CreateNamedPipeA(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);
		if (hNamedPipe == INVALID_HANDLE_VALUE) {
			cerr << "Creation of the named pipe failed. \nThe last error code: " << GetLastError() << endl;
			std::cout << "Press any char to finish server: ";
			std::cin >> c;
			return (0);
		}
		else {
			cerr << "Succesfull creation of the named pipe\n";
		}
		allPipes[i].hMyPipe = hNamedPipe;
#pragma endregion		
#pragma region create process
		STARTUPINFO cif;
		ZeroMemory(&cif, sizeof(STARTUPINFO));
		cif.cb = (sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		char 	clientName[80];
		wsprintfA(clientName, "Client.exe %s", pipeName);
		wchar_t wtext[80];
		mbstowcs(wtext, clientName, strlen(clientName) + 1);//Plus null
		LPWSTR ptr = wtext;
		if (!CreateProcess( NULL, ptr, NULL,NULL,TRUE,CREATE_NEW_CONSOLE,NULL,NULL,&cif,&pi)){
			cerr << "Creation of process failed.\nThe last error code: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			std::cout << "Press any char to finish the server: ";
			std::cin >> c;
			return (0);
		}
		else {
			cerr << "Successfull creation of process!\n";
		}
#pragma endregion
#pragma region connect with client
		// ���� ���� ������ �������� � �������
		std::cout << "The server is waiting for connection with a client..." << endl;
		if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL)) {
			cerr << "The connection failed.\nThe last error code: " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			std::cout << "Press any char to finish the server: ";
			std::cin >> c;
			return (0);
		}
		else {
			cerr << "Connection success!\n";
		}
#pragma endregion
		hThread[i] = CreateThread(NULL, 0, WorkingThreadFunc, (void*)&allPipes[i], 0, ThreadID + i);
	}
	WaitForMultipleObjects(nClient, hThread, TRUE, INFINITE);
	delete[] hThread;
	delete[] ThreadID;
	delete[] hWriteAccess;
	delete[] hReadAccess;
	cout << "\n\n\nFinal result:\n";
	file.seekg(0, ios::beg);
	int N;
	file.read((char*)&N, sizeof(int));
	employee st;
	for (int i = 0; i < N; ++i) {
		file.read((char*)&st, sizeof(st));
		cout << "\n--------- Record #" << i + 1 << " ---------\n";
		cout << "Enter employee's number: " << st.id;
		cout << "\nName: " << st.name;
		cout << "\nHours: " << st.hours << endl;
	}
	// ��������� �������
	std::cout << "Press any char to finish the server ";
	std::cin >> c;
	
	file.close();
	return 0;

}