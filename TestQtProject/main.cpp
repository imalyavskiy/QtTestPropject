#include "mainwindow.h"
#include <QApplication>
#include <windows.h>
#include <iostream>

#ifdef _DEBUG
int main(int argc, char *argv[])
{
	int result = 0;
	std::cout << "==== DEBUG CONSOLE STARTED ====" << std::endl;
	
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();

		result = a.exec();
	}
	
	std::cout << "==== DEBUG CONSOLE STOPPED ====" << std::endl;
	return result;
}

#else // defined NDEBUG
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow)
{
	int result = 0;
	
	int argc = 0;
	char** argv = nullptr;

	{
		LPWSTR* lpArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
		argv = (char**)malloc(argc * sizeof(char*));
		int size, i = 0;
		for (; i < argc; ++i)
		{
			size = wcslen(lpArgv[i]) + 1;
			argv[i] = (char*)malloc(size);
			wcstombs(argv[i], lpArgv[i], size);
		}
		LocalFree(lpArgv);
	}

	//////////////////////////////////////////////////////////////////////////
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	
	result = a.exec();
	//////////////////////////////////////////////////////////////////////////

	{
		int i = 0;
		for (; i < argc; ++i)
			free(argv[i]);
		free(argv);
	}

	return result;
}
#endif