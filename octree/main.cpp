#include <GL/glew.h>

#include <iostream>


#include <QApplication>



#include "mainWindow.h"



int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	if (!QGLFormat::hasOpenGL()) {
		std::cout << "This system does not support OpenGL." << std::endl;
		return -1;
	}

	MainWindow mw;
	mw.resize(800, 600);
	mw.move(200,200);
	//mw.showFullScreen();
	mw.show();
	return a.exec();
}
