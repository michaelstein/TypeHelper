#include "QtWidgets/QApplication"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication qapp(argc, argv);
	qapp.setApplicationName("TypeHelper");
	qapp.setOrganizationName("insaneFactory");
	qapp.setOrganizationDomain("http://insanefactory.com/");
	qapp.setQuitOnLastWindowClosed(true);

	MainWindow win;
	win.show();

	return qapp.exec();
}
