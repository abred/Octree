#include "mainWindow.h"

#include "ui_widget.h"


MainWindow::MainWindow(QWidget *parent) : 
			QMainWindow(parent),
			ui(new Ui::MainWindow)
{
	QGLFormat* format = new QGLFormat();
	format->setVersion(4, 2);
	format->setProfile (QGLFormat::CoreProfile);

	qgl = new OpenGLQtContext (format, (QWidget*)this);
	delete format;	
	format = 0;

	ui->setupUi(this);
	ui->gridLayout->addWidget(qgl, 0, 1, 4, 7);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete qgl;
}



void MainWindow::pushButton1Clicked()
{

}


