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
	mw.resize(1800, 900);
	mw.move(800,200);
	mw.showFullScreen();
	//widget.show();
	return a.exec();
}
