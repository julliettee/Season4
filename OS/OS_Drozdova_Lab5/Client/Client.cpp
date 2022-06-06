#pragma warning(disable:4996)

#include <windows.h>
#include <iostream>
#include "../Drozdova_Lab5/Helper.h"

using namespace std;




int main(int argc, char** argv)
{
	char 	c;		// служебный символ
	HANDLE 	hNamedPipe;
	char 	pipeName[80];

	// вводим имя машины в сети, на которой работает сервер
	
	wsprintfA(pipeName, argv[1]);

	// связываемся с именованным каналом
	hNamedPipe = CreateFileA(
		pipeName,				// имя канала
		GENERIC_READ | GENERIC_WRITE,		// читаем и записываем в канал
		FILE_SHARE_READ | FILE_SHARE_WRITE,	// разрешаем чтение и запись в канал
		(LPSECURITY_ATTRIBUTES)NULL,		// защита по умолчанию
		OPEN_EXISTING,				// открываем существующий канал
		FILE_ATTRIBUTE_NORMAL,			// атрибуты по умолчанию
		(HANDLE)NULL				// дополнительных атрибутов нет
	);

	// проверяем связь с каналом
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
				WriteFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // отправляем сообщеньку, что мы хотим прочитать
				ReadFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // читаем ответ
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
				WriteFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // говорим, что мы хотим апдейтить запись
				ReadFile(hNamedPipe, &cm, sizeof(cm), &bytes, 0); // ждем ответ от сервера
				if (cm.result == true) {
					ReadFile(hNamedPipe, (char*)&st, sizeof(st), &bytes, 0); // прочитали что сейчас
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
					WriteFile(hNamedPipe, &st, sizeof(st), &bytes, 0); // говорим, что нужно перезаписать
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
	// завершаем процесс
	cout << "Press any char to finish the client: ";
	cin >> c;

	return (1);
}