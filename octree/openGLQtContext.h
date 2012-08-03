#ifndef __OPENGLQTCONTEXT_H__
#define __OPENGLQTCONTEXT_H__

#define BUFFER_OFFSET(i) ((char*) NULL + (i))

//Eigende Header
#include <GL/glew.h>
//
//GLM-Header
	//GLM_SWIZZLE: erlaubt GLSL-ähnlichen Zugriff auf Vektorkomponenten, 
	//z.B. glm::vec4 a();
	//	   glm::vec2 b();
	//	   B.yx = A.wy;
	//     B = A.xx;
	//Vorsicht! Kann Namenskonflikte mit anderen Headern/Libraries erzeugen
//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
//
//QT-Header
//#include <QtOpenGL>
#include <QApplication>
#include <QGLWidget>
#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>

//
//STL-Header
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <sstream>
#include <fstream>
#include <cmath>
//


//#include <IL/il.h>
#include "volumeLoader.h"
#include "brick.h"
#include "brickTree.h"




class OpenGLQtContext : public QGLWidget
{
	Q_OBJECT	// nötig um Qt signals/slots zu nutzen
	
	public:

							OpenGLQtContext(QGLFormat *context, QWidget *parent = 0);
							~OpenGLQtContext();

	
	protected:

			// überschriebene Funktionen der Elternklassen
			void 			initializeGL();
			void 			paintGL();
			void 			resizeGL(int width, int height);

			void 			mousePressEvent(QMouseEvent *);
			//void 			mouseReleaseEvent(QMouseEvent *);
			void 			mouseMoveEvent(QMouseEvent *);
			void			keyPressEvent(QKeyEvent *);


	private:
			void initScene();
			void initMatrices();
			void initShader();

			glm::mat4	mModelMatrix;
			glm::mat4	mViewMatrix;
			glm::mat4	mModelViewMatrix;
			glm::mat4	mNormalMatrix;
			glm::mat4	mProjectionMatrix;
			glm::mat4	mMVPMatrix;

			GLuint		mShaderID;

			GLuint		mVao;
			GLuint		mVBuf;
			GLuint		mIBuf;

			QTimer *        mTimer;
			BrickTree*	mTree;

			int 		mFrameCounter;


	public slots:
			void 		fps();


	private:

	signals:
			void		setTitle(QString const&);


};


#endif // OPENGLQTCONTEXT_H__
