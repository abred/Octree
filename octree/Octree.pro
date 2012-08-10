QT        += opengl
CONFIG += console debug_and_release


TEMPLATE = app
Debug:TARGET = Octree.debug
Debug:DEFINES += DEBUG
Release:TARGET = Octree
DEPENDPATH += .
INCLUDEPATH += . include/ 

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS_RELEASE -= -g

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
