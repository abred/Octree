#include <GL/glew.h>

#include <iostream>
#include <stdexcept>


#include <QApplication>



#include "mainWindow.h"



int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	if (!QGLFormat::hasOpenGL()) {
		std::cout << "This system does not support OpenGL." << std::endl;
		return -1;
	}

	try
	{
		MainWindow mw(argv[1]);
		mw.resize(800, 600);
		mw.move(200,200);
		//mw.showFullScreen();
		mw.show();
		return a.exec();
	}
	catch(std::logic_error& e)
	{
		std::cerr << "exception: " << e.what() << " (Did you forget to provide a filename?)" << std::endl;
		exit(1);
	}
	
	return 0;
	
}
