#ifndef __MAINWINDOW_HPP_
#define __MAINWINDOW_HPP_
#include <GL/glew.h>

#include <QMainWindow>
#include <QApplication>


#include "openGLQtContext.h"


namespace Ui 
{ 
	class Widget; 
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow (QWidget *parent = 0);
	~MainWindow ();



public slots:
//		void touchSlot(QTouchEvent* touchEvent);

signals:
		void clickedButton1();


protected:

private:
	Ui::Widget *ui;
	OpenGLQtContext * qgl;
	

private slots:
    void pushButton1Clicked();



};

#endif // __MAINWINDOW_HPP_
