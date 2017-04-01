#include <windows.h>
#include <iostream>

#include <QApplication>

#include "interviewmodel.h"

#include <QApplication>
#include <QHeaderView>
#include <QListView>
#include <QSplitter>
#include <QTableView>
#include <QTreeView>

int AppMain(int argc, char* argv[])
{
	Q_INIT_RESOURCE(interview);

	QApplication app(argc, argv);
	QSplitter page;

	QAbstractItemModel *data = new CInterviewModel(1000, 10, &page);
	QItemSelectionModel *selections = new QItemSelectionModel(data);

	QTableView *table = new QTableView;
	table->setModel(data);
	table->setSelectionModel(selections);
	table->horizontalHeader()->setSectionsMovable(true);
	table->verticalHeader()->setSectionsMovable(true);
	// Set StaticContents to enable minimal repaints on resizes.
	table->viewport()->setAttribute(Qt::WA_StaticContents);
	page.addWidget(table);

	QTreeView *tree = new QTreeView;
	tree->setModel(data);
	tree->setSelectionModel(selections);
	tree->setUniformRowHeights(true);
	tree->header()->setStretchLastSection(false);
	tree->viewport()->setAttribute(Qt::WA_StaticContents);
	// Disable the focus rect to get minimal repaints when scrolling on Mac.
	tree->setAttribute(Qt::WA_MacShowFocusRect, false);
	page.addWidget(tree);

	QListView *list = new QListView;
	list->setModel(data);
	list->setSelectionModel(selections);
	list->setViewMode(QListView::IconMode);
	list->setSelectionMode(QAbstractItemView::ExtendedSelection);
	list->setAlternatingRowColors(false);
	list->viewport()->setAttribute(Qt::WA_StaticContents);
	list->setAttribute(Qt::WA_MacShowFocusRect, false);
	page.addWidget(list);

	page.setWindowIcon(QPixmap(":/images/interview.png"));
	page.setWindowTitle("Interview");
	page.show();

	return app.exec();
}

#ifdef _DEBUG
int main(int argc, char *argv[])
{
	int result = 0;
	std::cout << "==== DEBUG CONSOLE STARTED ====" << std::endl;
	
	result = AppMain(argc, argv);
	
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