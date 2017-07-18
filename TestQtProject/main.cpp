#include <iostream>

#include <QtWidgets/QApplication>

#include "interviewmodel.h"
#include "editboxdelegate.h"
#include "tableview.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QSizePolicy>

int 
AppMain(int argc, char* argv[])
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	QApplication app(argc, argv);
	QMainWindow mainWindow;

 	QAbstractItemModel *data_model = CDataTableModel::createInstance(0, 2, &mainWindow);
 	QItemSelectionModel *selection_model = new QItemSelectionModel(data_model);
 
 	CTableView *table = new CTableView;
 	table->setModel(data_model);
 	table->setSelectionModel(selection_model);
 	table->horizontalHeader()->setSectionsMovable(false);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
 	table->verticalHeader()->setSectionsMovable(false);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
 	
	// Set StaticContents to enable minimal repaints on resizes.
 	table->viewport()->setAttribute(Qt::WA_StaticContents);

	CEditBoxDelegate editBoxDelegate;
	table->setItemDelegate(&editBoxDelegate);
		
	mainWindow.setCentralWidget(table);
	mainWindow.setWindowIcon(QPixmap(":/images/interview.png"));
	mainWindow.setWindowTitle("Interview");
	
	mainWindow.setFixedWidth(WINDOW_WIDTH);
	mainWindow.setFixedHeight(WINDOW_HEIGHT);

	mainWindow.show();

	return app.exec();
}

#ifdef _DEBUG
int 
main(int argc, char *argv[])
{
	int result = 0;
	std::cout << "==== DEBUG CONSOLE STARTED ====" << std::endl;

	//////////////////////////////////////////////////////////////////////////
	Q_INIT_RESOURCE(interview);
	result = AppMain(argc, argv);
	//////////////////////////////////////////////////////////////////////////
	
	std::cout << "==== DEBUG CONSOLE STOPPED ====" << std::endl;
	return result;
}

#else // defined NDEBUG
#include <windows.h>

int
WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpCmdLine, int nCmdShow)
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
	Q_INIT_RESOURCE(interview);
	result = AppMain(argc, argv);
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