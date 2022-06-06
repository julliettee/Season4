#pragma warning(disable:4996)

#include <windows.h>
#include <iostream>
#include "../Drozdova_Lab5/Helper.h"

using namespace std;




int main(int argc, char** argv)
{
	char 	c;		// ��������� ������
	HANDLE 	hNamedPipe;
	char 	pipeName[80];

	// ������ ��� ������ � ����, �� ������� �������� ������
	
	wsprintfA(pipeName, argv[1]);

	// ����������� � ����������� �������
	hNamedPipe = CreateFileA(
		pipeName,				// ��� ������
		GENERIC_READ | GENERIC_WRITE,		// ������ � ���������� � �����
		FILE_SHARE_READ | FILE_SHARE_WRITE,	// ��������� ������ � ������ � �����
		(LPSECURITY_ATTRIBUTES)NULL,		// ������ �� ���������
		OPEN_EXISTING,				// ��������� ������������ �����
		FILE_ATTRIBUTE_NORMAL,			// �������� �� ���������
		(HANDLE)NULL				// �������������� ��������� ���
	);

	// ��������� ����� � �������
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		return (0);
	}
	cout << "Client succesfully connected!\n";




	char comm;
	DWORD bytes;
	employee st;
	command cm;
	cout << "Choose option:\n\tu - update\n\tr - read\n\te - exit\n";
	cout << "\nEnter command:";
	while (cin >> comm) {
		if (comm == 'e') {
			cm.type = 3;
			WriteFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0);
			break;
		}
		else {
			if (comm == 'r') {
				cm.type = 0;
				cout << "Enter employee's id:";
				cin >> cm.id;
				WriteFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // ���������� ����������, ��� �� ����� ���������
				ReadFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // ������ �����
				if (cm.result == true) {
					system("pause");
					cout << "i start reading from pipe\n";
					ReadFile(hNamedPipe, (char*)&st, sizeof(st), &bytes, 0);
					cout << "i read it\n";
					cout << "\nEmployee's id: " << st.id;
					cout << "\nName: " << st.name;
					cout << "\nHours: " << st.hours << endl;
				

				}
				else {
					cout << "No record with id #" << cm.id;
				}
			}
			else if (comm == 'u') {
				cm.type = 1;
				cout << "Enter employee's id:";
				cin >> cm.id;
				employee st;
				WriteFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // �������, ��� �� ����� ��������� ������
				ReadFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // ���� ����� �� �������
				if (cm.result == true) {
					ReadFile(hNamedPipe, (char*)&st, sizeof(st), &bytes, 0); // ��������� ��� ������
					cout << "Employee's id : " << st.id;
					cout << "\nName: " << st.name;
					cout << "\nHours: " << st.hours << endl;
					cout << "\nEnter new employee's id: ";
					cin >> st.id;
					cin.get();
					cout << "Enter new name: ";
					cin.getline(st.name, 9);
					cout << "Enter new hours: ";
					cin >> st.hours;
					system("pause");
					WriteFile(hNamedPipe, &st, sizeof(st), &bytes, 0); // �������, ��� ����� ������������
				}
				else {
					cout << "No record with id #" << cm.id;
				}
			}
			else {
				cout << "Incorretc command!";
			}
		}
		cout << "\nEnter command:";
	}
	// ��������� �������
	cout << "Press any char to finish the client: ";
	cin >> c;

	return (1);
}