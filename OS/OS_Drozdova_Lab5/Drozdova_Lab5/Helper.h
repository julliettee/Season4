#pragma once
#include <Windows.h>

struct command {
	int id;
	bool result;
	char type;
};

struct employee {
	int id;
	char name[10];
	double hours;
};
struct NamedPipeInfo {
	HANDLE hMyPipe;
	int		myNum;
};
