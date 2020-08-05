#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <Windows.h>
#include <cstdio>
#include <cinttypes>
#include <QtWidgets/QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow win(argc, argv);
	win.show();
	return app.exec();
}
