#include "mainWindow.h"

#include "ui_widget.h"


MainWindow::MainWindow(std::string const& filename, QWidget *parent) : 
			QMainWindow(parent),
			ui(new Ui::MainWindow)
{
	QGLFormat* format = new QGLFormat();
	format->setVersion(4, 2);
	format->setProfile (QGLFormat::CoreProfile);

	qgl = new OpenGLQtContext (filename, format, (QWidget*)this);
	delete format;	
	format = 0;

	ui->setupUi(this);
	ui->gridLayout->addWidget(qgl, 0, 1, 4, 7);
	
	connect(qgl, SIGNAL(setTitle(QString const&)), this, SLOT(setWindowTitle(QString const&)));
	
	int max = ui->verticalSlider->maximum();
	int min = ui->verticalSlider->minimum();
	int step = max/8;
	
	ui->verticalSlider->setValue(min);
	ui->verticalSlider_9->setValue(min);
	ui->verticalSlider_17->setValue(min);
	ui->verticalSlider_25->setValue(max);
	
	ui->verticalSlider_2->setValue(1*int(step)+min);
	ui->verticalSlider_10->setValue(1*int(step)+min);
	ui->verticalSlider_18->setValue(1*int(step)+min);
	ui->verticalSlider_26->setValue(max);
	
	ui->verticalSlider_3->setValue(2*int(step)+min);
	ui->verticalSlider_11->setValue(2*int(step)+min);
	ui->verticalSlider_19->setValue(2*int(step)+min);
	ui->verticalSlider_27->setValue(max);
	
	ui->verticalSlider_4->setValue(3*int(step)+min);
	ui->verticalSlider_12->setValue(3*int(step)+min);
	ui->verticalSlider_20->setValue(3*int(step)+min);
	ui->verticalSlider_28->setValue(max);
	
	ui->verticalSlider_5->setValue(4*int(step)+min);
	ui->verticalSlider_13->setValue(4*int(step)+min);
	ui->verticalSlider_21->setValue(4*int(step)+min);
	ui->verticalSlider_29->setValue(max);
	
	ui->verticalSlider_6->setValue(5*int(step)+min);
	ui->verticalSlider_14->setValue(5*int(step)+min);
	ui->verticalSlider_22->setValue(5*int(step)+min);
	ui->verticalSlider_30->setValue(max);
	
	ui->verticalSlider_7->setValue(6*int(step)+min);
	ui->verticalSlider_15->setValue(6*int(step)+min);
	ui->verticalSlider_23->setValue(6*int(step)+min);
	ui->verticalSlider_31->setValue(max);
	
	ui->verticalSlider_8->setValue(7*int(step)+min);
	ui->verticalSlider_16->setValue(7*int(step)+min);
	ui->verticalSlider_24->setValue(7*int(step)+min);
	ui->verticalSlider_32->setValue(max);
	
	ui->verticalSlider_33->setValue(max);
	ui->verticalSlider_34->setValue(max);
	ui->verticalSlider_35->setValue(max);
	ui->verticalSlider_36->setValue(max);
	
	
	connect(ui->verticalSlider,   SIGNAL(valueChanged(int)), this, SLOT(redSlider1(int)));
	connect(ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(redSlider2(int)));
	connect(ui->verticalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(redSlider3(int)));
	connect(ui->verticalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(redSlider4(int)));
	connect(ui->verticalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(redSlider5(int)));
	connect(ui->verticalSlider_6, SIGNAL(valueChanged(int)), this, SLOT(redSlider6(int)));
	connect(ui->verticalSlider_7, SIGNAL(valueChanged(int)), this, SLOT(redSlider7(int)));
	connect(ui->verticalSlider_8, SIGNAL(valueChanged(int)), this, SLOT(redSlider8(int)));
	connect(ui->verticalSlider_33, SIGNAL(valueChanged(int)), this, SLOT(redSlider9(int)));
	
	connect(ui->verticalSlider_9,  SIGNAL(valueChanged(int)), this, SLOT(greenSlider1(int)));
	connect(ui->verticalSlider_10, SIGNAL(valueChanged(int)), this, SLOT(greenSlider2(int)));
	connect(ui->verticalSlider_11, SIGNAL(valueChanged(int)), this, SLOT(greenSlider3(int)));
	connect(ui->verticalSlider_12, SIGNAL(valueChanged(int)), this, SLOT(greenSlider4(int)));
	connect(ui->verticalSlider_13, SIGNAL(valueChanged(int)), this, SLOT(greenSlider5(int)));
	connect(ui->verticalSlider_14, SIGNAL(valueChanged(int)), this, SLOT(greenSlider6(int)));
	connect(ui->verticalSlider_15, SIGNAL(valueChanged(int)), this, SLOT(greenSlider7(int)));
	connect(ui->verticalSlider_16, SIGNAL(valueChanged(int)), this, SLOT(greenSlider8(int)));
	connect(ui->verticalSlider_34, SIGNAL(valueChanged(int)), this, SLOT(greenSlider9(int)));
	
	connect(ui->verticalSlider_17, SIGNAL(valueChanged(int)), this, SLOT(blueSlider1(int)));
	connect(ui->verticalSlider_18, SIGNAL(valueChanged(int)), this, SLOT(blueSlider2(int)));
	connect(ui->verticalSlider_19, SIGNAL(valueChanged(int)), this, SLOT(blueSlider3(int)));
	connect(ui->verticalSlider_20, SIGNAL(valueChanged(int)), this, SLOT(blueSlider4(int)));
	connect(ui->verticalSlider_21, SIGNAL(valueChanged(int)), this, SLOT(blueSlider5(int)));
	connect(ui->verticalSlider_22, SIGNAL(valueChanged(int)), this, SLOT(blueSlider6(int)));
	connect(ui->verticalSlider_23, SIGNAL(valueChanged(int)), this, SLOT(blueSlider7(int)));
	connect(ui->verticalSlider_24, SIGNAL(valueChanged(int)), this, SLOT(blueSlider8(int)));
	connect(ui->verticalSlider_35, SIGNAL(valueChanged(int)), this, SLOT(blueSlider9(int)));
	
	connect(ui->verticalSlider_25, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider1(int)));
	connect(ui->verticalSlider_26, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider2(int)));
	connect(ui->verticalSlider_27, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider3(int)));
	connect(ui->verticalSlider_28, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider4(int)));
	connect(ui->verticalSlider_29, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider5(int)));
	connect(ui->verticalSlider_30, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider6(int)));
	connect(ui->verticalSlider_31, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider7(int)));
	connect(ui->verticalSlider_32, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider8(int)));
	connect(ui->verticalSlider_36, SIGNAL(valueChanged(int)), this, SLOT(alphaSlider9(int)));
	

	for (unsigned int i = 0; i < VALUERANGE; ++i)
	{
		trans[i][0] = float(i)/float(VALUERANGE);
		trans[i][1] = float(i)/float(VALUERANGE);
		trans[i][2] = float(i)/float(VALUERANGE);
		trans[i][3] = 1.0f;
		
//		std::cout << trans[i][0] << " " << trans[i][0] << " " << trans[i][0] << std::endl;
	}
}

MainWindow::~MainWindow()
{
	delete ui;
	delete qgl;
}

void MainWindow::changeTransfer(int value, int slider, int comp)
{
//	std::cout << "slider " << value << " " << slider << std::endl;
	float step = float(VALUERANGE)/ 8.0f;
	int nextVal = 0;
	int prevVal = 0;
	
	switch(slider)
	{
		// red
		case 1:
			nextVal = ui->verticalSlider_2->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		case 2:
			nextVal = ui->verticalSlider_3->value();
			prevVal = ui->verticalSlider->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 3:
			nextVal = ui->verticalSlider_4->value();
			prevVal = ui->verticalSlider_2->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 4:
			nextVal = ui->verticalSlider_5->value();
			prevVal = ui->verticalSlider_3->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 5:
			nextVal = ui->verticalSlider_6->value();
			prevVal = ui->verticalSlider_4->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 6:
			nextVal = ui->verticalSlider_7->value();
			prevVal = ui->verticalSlider_5->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 7:
			nextVal = ui->verticalSlider_8->value();
			prevVal = ui->verticalSlider_6->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 8:
			nextVal = ui->verticalSlider_33->value();
			prevVal = ui->verticalSlider_7->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		// green
		case 9:
			nextVal = ui->verticalSlider_10->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		case 10:
			nextVal = ui->verticalSlider_11->value();
			prevVal = ui->verticalSlider_9->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 11:
			nextVal = ui->verticalSlider_12->value();
			prevVal = ui->verticalSlider_10->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 12:
			nextVal = ui->verticalSlider_13->value();
			prevVal = ui->verticalSlider_11->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 13:
			nextVal = ui->verticalSlider_14->value();
			prevVal = ui->verticalSlider_12->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 14:
			nextVal = ui->verticalSlider_15->value();
			prevVal = ui->verticalSlider_13->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 15:
			nextVal = ui->verticalSlider_16->value();
			prevVal = ui->verticalSlider_14->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 16:
			nextVal = ui->verticalSlider_34->value();
			prevVal = ui->verticalSlider_15->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		// blue
		case 17:
			nextVal = ui->verticalSlider_17->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		case 18:
			nextVal = ui->verticalSlider_19->value();
			prevVal = ui->verticalSlider_17->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 19:
			nextVal = ui->verticalSlider_20->value();
			prevVal = ui->verticalSlider_18->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 20:
			nextVal = ui->verticalSlider_21->value();
			prevVal = ui->verticalSlider_19->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 21:
			nextVal = ui->verticalSlider_22->value();
			prevVal = ui->verticalSlider_20->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 22:
			nextVal = ui->verticalSlider_23->value();
			prevVal = ui->verticalSlider_21->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 23:
			nextVal = ui->verticalSlider_24->value();
			prevVal = ui->verticalSlider_22->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 24:
			nextVal = ui->verticalSlider_35->value();
			prevVal = ui->verticalSlider_23->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		// alpha
		case 25:
			nextVal = ui->verticalSlider_25->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		case 26:
			nextVal = ui->verticalSlider_27->value();
			prevVal = ui->verticalSlider_25->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+1*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 27:
			nextVal = ui->verticalSlider_28->value();
			prevVal = ui->verticalSlider_26->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+2*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 28:
			nextVal = ui->verticalSlider_29->value();
			prevVal = ui->verticalSlider_27->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+3*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 29:
			nextVal = ui->verticalSlider_30->value();
			prevVal = ui->verticalSlider_28->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+4*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 30:
			nextVal = ui->verticalSlider_31->value();
			prevVal = ui->verticalSlider_29->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+5*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 31:
			nextVal = ui->verticalSlider_32->value();
			prevVal = ui->verticalSlider_30->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+6*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
		case 32:
			nextVal = ui->verticalSlider_36->value();
			prevVal = ui->verticalSlider_31->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+7*int(step)][comp] = (float(nextVal-value)/step * i + value)/99.0f;
			}
			break;
			
		case 33:
			prevVal = ui->verticalSlider_8->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+8*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			break;
		
		case 34:
			prevVal = ui->verticalSlider_16->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+8*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			break;

		case 35:
			prevVal = ui->verticalSlider_24->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+8*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			break;

		case 36:
			prevVal = ui->verticalSlider_32->value();
			for (unsigned int i = 0; i < (unsigned int)step; ++i)
			{
				trans[i+8*int(step)-int(step)][comp] = (float(value-prevVal)/step * i + prevVal)/99.0f;
			}
			break;
	}
	
	glActiveTexture(GL_TEXTURE2);	
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, VALUERANGE , GL_RGBA, GL_FLOAT , &trans[0][0]);
}


void MainWindow::redSlider1(int value)
{
	changeTransfer(value, 1, 0);
}
void MainWindow::redSlider2(int value)
{
	changeTransfer(value, 2, 0);
}
void MainWindow::redSlider3(int value)
{
	changeTransfer(value, 3, 0);
}
void MainWindow::redSlider4(int value)
{
	changeTransfer(value, 4, 0);
}
void MainWindow::redSlider5(int value)
{
	changeTransfer(value, 5, 0);
}
void MainWindow::redSlider6(int value)
{
	changeTransfer(value, 6, 0);
}
void MainWindow::redSlider7(int value)
{
	changeTransfer(value, 7, 0);
}
void MainWindow::redSlider8(int value)
{
	changeTransfer(value, 8, 0);
}
void MainWindow::redSlider9(int value)
{
	changeTransfer(value, 33, 0);
}

void MainWindow::greenSlider1(int value)
{
	changeTransfer(value, 9, 1);
}
void MainWindow::greenSlider2(int value)
{
	changeTransfer(value, 10, 1);
}
void MainWindow::greenSlider3(int value)
{
	changeTransfer(value, 11, 1);
}
void MainWindow::greenSlider4(int value)
{
	changeTransfer(value, 12, 1);
}
void MainWindow::greenSlider5(int value)
{
	changeTransfer(value, 13, 1);
}
void MainWindow::greenSlider6(int value)
{
	changeTransfer(value, 14, 1);
}
void MainWindow::greenSlider7(int value)
{
	changeTransfer(value, 15, 1);
}
void MainWindow::greenSlider8(int value)
{
	changeTransfer(value, 16, 1);
}
void MainWindow::greenSlider9(int value)
{
	changeTransfer(value, 34, 1);
}

void MainWindow::blueSlider1(int value)
{
	changeTransfer(value, 17, 2);
}
void MainWindow::blueSlider2(int value)
{
	changeTransfer(value, 18, 2);
}
void MainWindow::blueSlider3(int value)
{
	changeTransfer(value, 19, 2);
}
void MainWindow::blueSlider4(int value)
{
	changeTransfer(value, 20, 2);
}
void MainWindow::blueSlider5(int value)
{
	changeTransfer(value, 21, 2);
}
void MainWindow::blueSlider6(int value)
{
	changeTransfer(value, 22, 2);
}
void MainWindow::blueSlider7(int value)
{
	changeTransfer(value, 23, 2);
}
void MainWindow::blueSlider8(int value)
{
	changeTransfer(value, 24, 2);
}
void MainWindow::blueSlider9(int value)
{
	changeTransfer(value, 35, 2);
}

void MainWindow::alphaSlider1(int value)
{
	changeTransfer(value, 25, 3);
}
void MainWindow::alphaSlider2(int value)
{
	changeTransfer(value, 26, 3);
}
void MainWindow::alphaSlider3(int value)
{
	changeTransfer(value, 27, 3);
}
void MainWindow::alphaSlider4(int value)
{
	changeTransfer(value, 28, 3);
}
void MainWindow::alphaSlider5(int value)
{
	changeTransfer(value, 29, 3);
}
void MainWindow::alphaSlider6(int value)
{
	changeTransfer(value, 30, 3);
}
void MainWindow::alphaSlider7(int value)
{
	changeTransfer(value, 31, 3);
}
void MainWindow::alphaSlider8(int value)
{
	changeTransfer(value, 32, 3);
}
void MainWindow::alphaSlider9(int value)
{
	changeTransfer(value, 36, 3);
}






