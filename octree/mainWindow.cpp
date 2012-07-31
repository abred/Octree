#include "mainWindow.h"

#include "ui_widget.h"


MainWindow::MainWindow(QWidget *parent) : 
			QMainWindow(parent),
			ui(new Ui::Widget)

{
	QGLFormat* form = new QGLFormat();
	form->setVersion(4, 2);

	form->setProfile (QGLFormat::CoreProfile);

	qgl = new OpenGLQtContext (form, (QWidget*)0);

	delete form;	form = 0;

	ui->setupUi(this);
	ui->GLSpace->addWidget(qgl);
	

//	connect(this, SIGNAL(clickedButton1()), this, SLOT(pushButton1Clicked()));
	connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(pushButton1Clicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete qgl;
}



void MainWindow::pushButton1Clicked()
{

}


