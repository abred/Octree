/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDial>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGroupBox *groupBox;
    QVBoxLayout *GLSpace;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QDial *dial;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1920, 1080);
        Widget->setStyleSheet(QString::fromUtf8("\n"
"        border: 0px solid #000;\n"
"      "));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 1919, 1080));
        GLSpace = new QVBoxLayout(groupBox);
        GLSpace->setSpacing(0);
        GLSpace->setContentsMargins(0, 0, 0, 0);
        GLSpace->setObjectName(QString::fromUtf8("GLSpace"));
        pushButton_1 = new QPushButton(Widget);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(100, 300, 150, 150));
        QIcon icon;
        icon.addFile(QString::fromUtf8("Laub.png"), QSize(), QIcon::Disabled, QIcon::On);
        pushButton_1->setIcon(icon);
        pushButton_1->setIconSize(QSize(120, 120));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(100, 450, 150, 150));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("Laub_lang.png"), QSize(), QIcon::Disabled, QIcon::On);
        pushButton_2->setIcon(icon1);
        pushButton_2->setIconSize(QSize(120, 120));
        pushButton_3 = new QPushButton(Widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(100, 600, 150, 150));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("Pinie.png"), QSize(), QIcon::Disabled, QIcon::On);
        pushButton_3->setIcon(icon2);
        pushButton_3->setIconSize(QSize(120, 120));
        pushButton_4 = new QPushButton(Widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(100, 100, 150, 150));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("Wind.png"), QSize(), QIcon::Disabled, QIcon::On);
        pushButton_4->setIcon(icon3);
        pushButton_4->setIconSize(QSize(120, 120));
        dial = new QDial(Widget);
        dial->setObjectName(QString::fromUtf8("dial"));
        dial->setGeometry(QRect(100, 900, 150, 150));
        dial->setFocusPolicy(Qt::WheelFocus);
        dial->setStyleSheet(QString::fromUtf8("\n"
"          border: 0px solid #000;\n"
"        "));
        dial->setMinimum(0);
        dial->setMaximum(9);
        dial->setValue(0);
        dial->setSliderPosition(0);
        dial->setTracking(true);
        dial->setOrientation(Qt::Horizontal);
        dial->setInvertedAppearance(false);
        dial->setInvertedControls(false);
        dial->setWrapping(false);
        dial->setNotchesVisible(true);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "TreeHugger", 0, QApplication::UnicodeUTF8));
        pushButton_1->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
