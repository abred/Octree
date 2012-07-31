QT        += opengl
CONFIG += console


TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . include/

LIBS =  -L/home/abred/qtDebug/lib/ -L/usr/lib64/ -L/usr/lib/ -L/usr/local/lib -L../lib -lGL -lGLEW -lIL

# Input
HEADERS += mainWindow.h \
           openGLQtContext.h
           
FORMS += ui/widget.ui

SOURCES += main.cpp \
           mainWindow.cpp \
           openGLQtContext.cpp
