QT        += opengl
CONFIG += console


TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += . include/ 

QMAKE_CXXFLAGS += -std=c++0x -g -O0

LIBS =  -L/home/mahu/qt/lib  -L/usr/lib/ -L/usr/local/lib -L../lib -lGL -lGLEW

# Input
HEADERS += mainWindow.h \
           openGLQtContext.h\
           volumeLoader.h\
           brick.h\
           brickTree.h\
	   comperator.h \
	   textureAtlas.h\
		
           
FORMS += ui/widget.ui

SOURCES += main.cpp \
           mainWindow.cpp \
           openGLQtContext.cpp\
           volumeLoader.cpp\
           brick.cpp\
           brickTree.cpp\
	   comperator.cpp \
	   textureAtlas.cpp
