#ifndef __MAINWINDOW_HPP_
#define __MAINWINDOW_HPP_
#include <GL/glew.h>

#include <QMainWindow>
#include <QApplication>
#include <QResizeEvent>

#include "openGLQtContext.h"


namespace Ui 
{ 
	class MainWindow; 
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow (QWidget *parent = 0);
	~MainWindow ();

private:
	Ui::MainWindow *ui;
	OpenGLQtContext * qgl;
	


};

#endif // __MAINWINDOW_HPP_
