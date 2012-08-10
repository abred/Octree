#ifndef __MAINWINDOW_HPP_
#define __MAINWINDOW_HPP_
#include <GL/glew.h>

#include <QMainWindow>
#include <QApplication>
#include <QResizeEvent>

#include "openGLQtContext.h"

#include "utility.h"

namespace Ui 
{ 
	class MainWindow; 
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow (std::string const& filename, QWidget *parent = 0);
	~MainWindow ();

private:
	Ui::MainWindow *ui;
	OpenGLQtContext * qgl;
	float trans[VALUERANGE][4];
	

	void changeTransfer(int value, int slider, int comp);


public slots:
	void redSlider1(int value);
	void redSlider2(int value);
	void redSlider3(int value);
	void redSlider4(int value);
	void redSlider5(int value);
	void redSlider6(int value);
	void redSlider7(int value);
	void redSlider8(int value);
	void redSlider9(int value);
	
	void greenSlider1(int value);
	void greenSlider2(int value);
	void greenSlider3(int value);
	void greenSlider4(int value);
	void greenSlider5(int value);
	void greenSlider6(int value);
	void greenSlider7(int value);
	void greenSlider8(int value);
	void greenSlider9(int value);
	
	void blueSlider1(int value);
	void blueSlider2(int value);
	void blueSlider3(int value);
	void blueSlider4(int value);
	void blueSlider5(int value);
	void blueSlider6(int value);
	void blueSlider7(int value);
	void blueSlider8(int value);
	void blueSlider9(int value);
	
	void alphaSlider1(int value);
	void alphaSlider2(int value);
	void alphaSlider3(int value);
	void alphaSlider4(int value);
	void alphaSlider5(int value);
	void alphaSlider6(int value);
	void alphaSlider7(int value);
	void alphaSlider8(int value);
	void alphaSlider9(int value);


	


};

#endif // __MAINWINDOW_HPP_
