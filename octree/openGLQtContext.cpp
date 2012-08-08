#include "openGLQtContext.h"

//
//readTextFile
//
char* readTextFile(std::string fileName)
{
	std::ifstream file (fileName.c_str(), std::ifstream::in);

	std::string text;

	while (file.good())
	{
	std::string s;

	getline (file, s);
	text += "\n" + s;
	}

	char* target = new char [text.size()+1];
	strcpy (target, text.c_str());

	return target;
}
//


//
// from g-truc ogl-samples
//
void GLAPIENTRY debugOutput (GLenum source,
							 GLenum type,
							 GLuint id,
							 GLenum severity, 
							 GLsizei length, 
							 const GLchar* message,
							 GLvoid* userParam)
{
	char debSource[32], debType[32], debSev[32];
	if(source == GL_DEBUG_SOURCE_API_ARB)
		strcpy(debSource, "OpenGL");
	else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
		strcpy(debSource, "Windows");
	else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
		strcpy(debSource, "Shader Compiler");
	else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
		strcpy(debSource, "Third Party");
	else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)
		strcpy(debSource, "Application");
	else if(source == GL_DEBUG_SOURCE_OTHER_ARB)
		strcpy(debSource, "Other");
	 
	if(type == GL_DEBUG_TYPE_ERROR_ARB)
		strcpy(debType, "error");
	else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
		strcpy(debType, "deprecated behavior");
	else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
		strcpy(debType, "undefined behavior");
	else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)
		strcpy(debType, "portability");
	else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
		strcpy(debType, "performance");
	else if(type == GL_DEBUG_TYPE_OTHER_ARB)
		strcpy(debType, "message");
	 
	if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)
		strcpy(debSev, "high");
	else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
		strcpy(debSev, "medium");
	else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)
		strcpy(debSev, "low");

	std::cout << debSource << ": " << debType << " (" << debSev << ") " << id << ": " << message << std::endl;
}



//
//Konstruktoren, Destruktoren
//
OpenGLQtContext::OpenGLQtContext(QGLFormat* context, QWidget *parent) :
		QGLWidget 		(*context, parent)
		
{
	// Qt kann in mehrere Contexte zeichnen, dies setzt unseren neu erstellten als aktuellen 
	makeCurrent();

	setFocusPolicy ( Qt::ClickFocus);
	

	mFrameCounter = 0;
	mTimer = new QTimer(this);
	 	connect(mTimer, SIGNAL(timeout()), this, SLOT(fps()));
	 	mTimer->start(1000);
	
}
//


OpenGLQtContext::~OpenGLQtContext()
{

}
//


//
// initializeGL
//
// virtuelle Funktion der Elternklasse wird hierdurch überschrieben
// initialisieren von glew, setzen von hintergrundfarbe und einigen OpenGL-states
//
void OpenGLQtContext::initializeGL()
{
	glewExperimental = GL_TRUE;
	GLenum glewInitResult = glewInit();
	if(GLEW_OK != glewInitResult)
	{
		std::cout << "QGL: ERROR "<<glewGetErrorString(glewInitResult) << std::endl;
		exit(EXIT_FAILURE);
	}			


	if (glewIsExtensionSupported("GL_ARB_debug_output"))
	{
	std::cout << "QGL: ARB debug output verfuegbar" << std::endl;
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageCallbackARB(&debugOutput, nullptr);
	}
	else
	{
		std::cout << "QGL: Kein debug output verfuegbar oder deaktiviert" << std::endl;
	}


	std::cout << "QGL: OpenGL Version: " << glGetString(GL_VERSION) << std::endl << std::endl;


//	glClearDepth(1.0f);
	glClearColor( 0.0f, 1.0f, 0.0f, 0.0f );

//	glDisable(GL_PROGRAM_POINT_SIZE);
//	glPointSize(5.0f);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable( GL_BLEND );
	glDisable(GL_DEPTH_CLAMP);

	glEnable( GL_DEPTH_TEST );
	//glDepthFunc(GL_LESS);
//	glCullFace(GL_BACK);
//	glDisable(GL_CULL_FACE);
//	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	initScene();
}
//


//
// initScene
//
// Erstellen von Shadern, Objekten (z.B. Zylinder)
// Setzen von Matrizen
//
void OpenGLQtContext::initScene()
{
	VolumeLoader* l = new VolumeLoader("res/coronal_w1024_h1024_d1080_c1_b16.raw");
//	VolumeLoader* l = new VolumeLoader("res/foot_w256_h256_d256_c1_b8.raw");
	l->loadData();
	mDim = l->getDimension();
	
	mTree = new BrickTree(l->getData() , l->getDimension().width , l->getDimension().height , l->getDimension().depth, glm::vec3(0.0f, 0.0f, 3.0f));

	delete l;

	initShader();
	initMatrices();

	std::vector<glm::vec3> vertices;
	{
		vertices.push_back(glm::vec3( 0.0f,  0.0f,  1.0f));//0
		vertices.push_back(glm::vec3( 1.0f,  0.0f,  1.0f));//1
		vertices.push_back(glm::vec3( 1.0f,  1.0f,  1.0f));//2
		vertices.push_back(glm::vec3( 0.0f,  1.0f,  1.0f));//3
		
		vertices.push_back(glm::vec3( 0.0f,  0.0f,  0.0f));//4
		vertices.push_back(glm::vec3( 1.0f,  0.0f,  0.0f));//5
		vertices.push_back(glm::vec3( 1.0f,  1.0f,  0.0f));//6
		vertices.push_back(glm::vec3( 0.0f,  1.0f,  0.0f));//7
	}
	std::cout << "#vertices: " << vertices.size() << std::endl;

	std::vector<GLuint> indices;
	{
		indices.push_back(GLuint(0));
		indices.push_back(GLuint(1));
		indices.push_back(GLuint(3));
		
		indices.push_back(GLuint(1));
		indices.push_back(GLuint(2));
		indices.push_back(GLuint(3));

		indices.push_back(GLuint(1));
		indices.push_back(GLuint(5));
		indices.push_back(GLuint(2));

		indices.push_back(GLuint(5));
		indices.push_back(GLuint(6));
		indices.push_back(GLuint(2));

		indices.push_back(GLuint(3));
		indices.push_back(GLuint(2));
		indices.push_back(GLuint(7));

		indices.push_back(GLuint(2));
		indices.push_back(GLuint(6));
		indices.push_back(GLuint(7));

		indices.push_back(GLuint(5));
		indices.push_back(GLuint(4));
		indices.push_back(GLuint(7));

		indices.push_back(GLuint(5));
		indices.push_back(GLuint(7));
		indices.push_back(GLuint(6));

		indices.push_back(GLuint(0));	
		indices.push_back(GLuint(4));
		indices.push_back(GLuint(1));

		indices.push_back(GLuint(1));
		indices.push_back(GLuint(4));
		indices.push_back(GLuint(5));

		indices.push_back(GLuint(0));
		indices.push_back(GLuint(3));
		indices.push_back(GLuint(4));

		indices.push_back(GLuint(3));
		indices.push_back(GLuint(7));
		indices.push_back(GLuint(4));
	}
	std::cout << "#indices: " << indices.size() << std::endl;

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVBuf);
	glGenBuffers(1, &mIBuf);

	glBindVertexArray(mVao);
	{	
		glBindBuffer(GL_ARRAY_BUFFER, mVBuf);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

		
	}
	glBindVertexArray(0);
}
//


//
// initMatrices
//
//
void OpenGLQtContext::initMatrices()
{
	mModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -0.5f));
	mViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mModelViewMatrix = mViewMatrix * mModelMatrix ;
	mMVInverseMatrix  = glm::inverse(mModelViewMatrix);
	mProjectionMatrix = glm::perspective(60.0f, float(800) / float(600), 0.1f, 1000.f);
	mMVPMatrix = mProjectionMatrix * mModelViewMatrix;
	mMVPInverseMatrix = glm::inverse(mMVPMatrix);

	glUseProgram(mShaderID);
	GLuint mvpMatrixLocation_ = glGetUniformLocation(mShaderID, "MVP");
	glUniformMatrix4fv(mvpMatrixLocation_, 1, GL_FALSE, &mMVPMatrix[0][0]);
	GLuint mvpIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVPInverse");
	glUniformMatrix4fv(mvpIMatrixLocation_, 1, GL_FALSE, &mMVPInverseMatrix[0][0]);
	GLuint mvIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVInverse");
	glUniformMatrix4fv(mvIMatrixLocation_, 1, GL_FALSE, &mMVInverseMatrix[0][0]);
	GLuint mvMatrixLocation_ = glGetUniformLocation(mShaderID, "MV");
	glUniformMatrix4fv(mvMatrixLocation_, 1, GL_FALSE, &mModelViewMatrix[0][0]);

}
//

//
// initShader
//
//
void OpenGLQtContext::initShader()
{
	std::string const vsFile("shader/quad.vert");
	//std::string const gsFile(".geom");
	std::string const fsFile("shader/quad.frag");

	mShaderID = glCreateProgram();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vsSource = readTextFile(vsFile);

		//GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		//const char* gsSource = readTextFile(gsFile);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fsSource = readTextFile(fsFile);


		glShaderSource(vertexShader, 1, &vsSource, nullptr);
		glCompileShader(vertexShader);

		//glShaderSource(geometryShader, 1, &gsSource, nullptr);
		//glCompileShader(geometryShader);

		glShaderSource(fragmentShader, 1, &fsSource, nullptr);
		glCompileShader(fragmentShader);

		glAttachShader(mShaderID, vertexShader);
		//glAttachShader(mShaderID, geometryShader);
		glAttachShader(mShaderID, fragmentShader);

		glLinkProgram(mShaderID);

		glDeleteShader(vertexShader);
		//glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);

		glUseProgram(mShaderID);
	
	GLuint indTexLocation = glGetUniformLocation(mShaderID, "indexTexture");
	glUniform1i(indTexLocation, 1);
	GLuint texAtlLocation = glGetUniformLocation(mShaderID, "textureAtlas");
	glUniform1i(texAtlLocation, 0);
	
	GLuint numsamLocation = glGetUniformLocation(mShaderID, "numSamples");
	glUniform1i(numsamLocation , 1000);
	
	GLuint stepSizeLocation = glGetUniformLocation(mShaderID, "stepSize");
	glUniform1f(stepSizeLocation , 0.01f );
	
	GLuint brickSizeLoc = glGetUniformLocation(mShaderID, "BRICKSIZE");
	glUniform1i(brickSizeLoc, BRICKSIZE);
	
	GLuint valueRangeLoc = glGetUniformLocation(mShaderID, "VALUERANGE");
	glUniform1f(valueRangeLoc, VALUERANGE);
	
	GLuint widthLoc = glGetUniformLocation(mShaderID, "width");
	glUniform1f(widthLoc, mDim.width);
	
	GLuint heightLoc = glGetUniformLocation(mShaderID, "height");
	glUniform1f(heightLoc, mDim.height);
	
	GLuint depthLoc = glGetUniformLocation(mShaderID, "depth");
	glUniform1f(depthLoc, mDim.depth);
	
	
}


//
// paintGL
//
// virtuelle Funktion der Elternklasse wird hierdurch überschrieben
// Render-Funktion
//
void OpenGLQtContext::paintGL()
{	
	// buffer leeren
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	mMVPMatrix = mProjectionMatrix * mModelViewMatrix;

	glm::vec4 cam = glm::inverse(mModelViewMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glUseProgram(mShaderID);
	GLuint camLocation = glGetUniformLocation(mShaderID, "camPosition");
	glUniform4fv(camLocation, 1, &cam[0]);
	
	++mFrameCounter;
	glm::vec4 cam2 = glm::inverse(mViewMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	mTree->updateCut(glm::vec3(cam.x, cam.y, cam.z));
//	std::cout << "cam  " << cam.x << " " << cam.y << " " << cam.z << std::endl;
//	std::cout << "cam2 " << cam2.x << " " << cam2.y << " " << cam2.z << std::endl;
	
	
//	glUseProgram(mShaderID);
//	GLuint mvpMatrixLocation = glGetUniformLocation(mShaderID, "MVP");
//	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mMVPMatrix[0][0]);
	glBindVertexArray(mVao);
//		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	
//	glm::vec3 cam(0.0f, 0.0f, 0.0f);

//	srand(time(NULL));
//	float ranX = (float)(rand() %10000) - 5000;
//	srand(time(NULL));
//	float ranY = (float)(rand() %10000- 5000) + ranX;
//	srand(time(NULL));
//	float ranZ = (float)(rand() %10000- 5000) - ranY;
//	cam = glm::vec3(cam.x +ranX , cam.y + ranY , cam.z + ranZ );
	

	


	update();
}
//


//
// resizeGL
//
// virtuelle Funktion der Elternklasse wird hierdurch überschrieben
//
void OpenGLQtContext::resizeGL(int width, int height)
{
//	std::cout << "pblub";
	mWidth = width;
	mHeight = height;
	glViewport(0, 0, width, height);
	resize(width, height);
	mProjectionMatrix = glm::perspective(60.0f, float(width) / float(height), 0.1f, 1000.f);
	mMVPMatrix = mProjectionMatrix * mModelViewMatrix;
	mMVPInverseMatrix = glm::inverse(mMVPMatrix);

	glUseProgram(mShaderID);
	GLuint mvpMatrixLocation_ = glGetUniformLocation(mShaderID, "MVP");
	glUniformMatrix4fv(mvpMatrixLocation_, 1, GL_FALSE, &mMVPMatrix[0][0]);
	GLuint mvpIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVPInverse");
	glUniformMatrix4fv(mvpIMatrixLocation_, 1, GL_FALSE, &mMVPInverseMatrix[0][0]);
	
	GLuint wsLocation = glGetUniformLocation(mShaderID, "windowSize");
	glUniform2f(wsLocation, float(width), float(height));
	
	
	float focalLength = 1.0f/ std::tan(1.047f/2.0f);
	
	GLuint flLocation = glGetUniformLocation(mShaderID, "focalLength");
	glUniform1f(flLocation, focalLength);
	
	
}
//




//
// keyPressEvent
//
// wird aufgerufen, sobald eine Taste gedrückt wird
//
void OpenGLQtContext::keyPressEvent(QKeyEvent* event)
{
	int key = event->key();
	if (!hasFocus())
	{
		std::cout << "QGL: keinen Focus!" << std::endl;
	}
	
	
	switch (key)
	{
		case Qt::Key_Escape:
					qApp->quit();
					break;
		case Qt::Key_Left:
					mViewMatrix = glm::translate (glm::mat4(1.0f), glm::vec3 (0.1f, 0.0f, 0.0f)) * mViewMatrix;
					break;

		case Qt::Key_Right:
					mViewMatrix = glm::translate (glm::mat4(1.0f), glm::vec3 (-0.1f, 0.0f, 0.0f)) * mViewMatrix;
					break;

		case Qt::Key_Up:
					mViewMatrix = glm::translate (glm::mat4(1.0f), glm::vec3 (0.0f, 0.1f, 0.0f)) * mViewMatrix;
					break;

		case Qt::Key_Down:
					mViewMatrix = glm::translate (glm::mat4(1.0f), glm::vec3 (0.0f, -0.1f, 0.0f)) * mViewMatrix;
					break;

		case Qt::Key_PageUp:
					mViewMatrix = glm::translate (glm::mat4(1.0f), glm::vec3 (0.0f, 0.0f, 0.1f)) * mViewMatrix;
					break;

		case Qt::Key_PageDown:
					mViewMatrix = glm::translate (glm::mat4(1.0f), glm::vec3 (0.0f, 0.0f, -0.1f)) * mViewMatrix;
					break;
		
		default:		
					std::cout << "QGL: Key nicht belegt" << std::endl;
	}

		mModelViewMatrix = mViewMatrix * mModelMatrix ;
		mMVPMatrix = mProjectionMatrix * mModelViewMatrix;
		mMVPInverseMatrix = glm::inverse(mMVPMatrix);
		mMVInverseMatrix  = glm::inverse(mModelViewMatrix);
		
		glUseProgram(mShaderID);
		GLuint mvpMatrixLocation_ = glGetUniformLocation(mShaderID, "MVP");
		glUniformMatrix4fv(mvpMatrixLocation_, 1, GL_FALSE, &mMVPMatrix[0][0]);
		GLuint mvpIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVPInverse");
		glUniformMatrix4fv(mvpIMatrixLocation_, 1, GL_FALSE, &mMVPInverseMatrix[0][0]);
		GLuint mvIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVInverse");
		glUniformMatrix4fv(mvIMatrixLocation_, 1, GL_FALSE, &mMVInverseMatrix[0][0]);
		GLuint mvMatrixLocation_ = glGetUniformLocation(mShaderID, "MV");
		glUniformMatrix4fv(mvMatrixLocation_, 1, GL_FALSE, &mModelViewMatrix[0][0]);

		update();
}
//


//
// mousePressEvent
//
// wird aufgerufen, wenn eine Maustaste gedrückt wird
//
void OpenGLQtContext::mousePressEvent(QMouseEvent *event)
{
	//lastPos_ = event->pos();
	glm::vec3 v(0.0f);
	float d;
	v.x = (2.0f * event->x() - mWidth) / float(mWidth);
	v.y = (mHeight - 2.0 * event->y()) / float(mHeight);
	// v.z = 0.0f;
	d = glm::length(v);
	d = d < 1.0f ? d : 1.0;
	v.z = std::sqrt(1.001 - d*d);
	v = glm::normalize(v);
std::cout << "blb";
	mLastPos = v;
}
//


//
// mouseMoveEvent
//
// wird aufgerufen, wenn die Maus bewegt wird
//
void OpenGLQtContext::mouseMoveEvent(QMouseEvent *event)
{
//	int dx = event->x() - mLastPos.x();
//	int dy = event->y() - mLastPos.y();


//	mLastPos = event->pos();


	if (event->buttons() & Qt::LeftButton)
	{
		glm::vec3 v(0.0f);
		float d;
		v.x = (2.0f * event->x() - mWidth) / float(mWidth);
		v.y = (mHeight - 2.0 * event->y()) / float(mHeight);
		// v.z = 0.0f;
		d = glm::length(v);
		d = d < 1.0f ? d : 1.0;
		v.z = std::sqrt(1.001 - d*d);
		v = glm::normalize(v);

		glm::vec3 curPos = v;
		
		glm::vec3 dir = curPos - mLastPos;
		float velo = glm::length(dir);
		if (velo > 0.0001f)
		{
			glm::vec3 rotAxis = glm::cross(mLastPos, curPos);
			float rotAngle = velo * 50.0f;
			
			mModelMatrix = glm::rotate(glm::mat4(1.0f), rotAngle, rotAxis) * mModelMatrix;
		}
		mLastPos = curPos;
	}

//	if (event->buttons() & Qt::LeftButton)
//	{
//		if (dx < 0)
//		{
//			glm::vec4 tmp = glm::inverse(mModelMatrix) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
//			mModelMatrix = mModelMatrix * glm::rotate(glm::mat4(1.0f), float(-M_PI), glm::vec3(tmp.x, tmp.y, tmp.z));
//		}
//		else
//		{
//			glm::vec4 tmp = glm::inverse(mModelMatrix) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
//			mModelMatrix = mModelMatrix * glm::rotate(glm::mat4(1.0f), float(M_PI), glm::vec3(tmp.x, tmp.y, tmp.z));
//		}
//		if (dy < 0)
//		{
//			glm::vec4 tmp = glm::inverse(mModelMatrix) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
//			mModelMatrix = mModelMatrix * glm::rotate(glm::mat4(1.0f), float(-M_PI), glm::vec3(tmp.x, tmp.y, tmp.z));
//		}
//		else
//		{
//			glm::vec4 tmp = glm::inverse(mModelMatrix) * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
//			mModelMatrix = mModelMatrix * glm::rotate(glm::mat4(1.0f), float(M_PI), glm::vec3(tmp.x, tmp.y, tmp.z));
//		}
//	}
//	else if (event->buttons() & Qt::RightButton)
//	{
		// if (dx < 0)
		//
		// viewMatrix_ = viewMatrix_ * glm::rotate(glm::mat4(1.0f), float(-M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
		// else
		// viewMatrix_ = viewMatrix_ * glm::rotate(glm::mat4(1.0f), float(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
		//
		// if (dy < 0)
		// viewMatrix_ = viewMatrix_ * glm::rotate(glm::mat4(1.0f), float(-M_PI), glm::vec3(1.0f, 0.0f, 0.0f));
		// else
		// viewMatrix_ = viewMatrix_ * glm::rotate(glm::mat4(1.0f), float(M_PI), glm::vec3(1.0f, 0.0f, 0.0f));
//	}

		mModelViewMatrix = mViewMatrix * mModelMatrix ;
		mMVPMatrix = mProjectionMatrix * mModelViewMatrix;
		mMVPInverseMatrix = glm::inverse(mMVPMatrix);
		mMVInverseMatrix  = glm::inverse(mModelViewMatrix);

		glUseProgram(mShaderID);
		GLuint mvpMatrixLocation_ = glGetUniformLocation(mShaderID, "MVP");
		glUniformMatrix4fv(mvpMatrixLocation_, 1, GL_FALSE, &mMVPMatrix[0][0]);
		GLuint mvpIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVPInverse");
		glUniformMatrix4fv(mvpIMatrixLocation_, 1, GL_FALSE, &mMVPInverseMatrix[0][0]);
		GLuint mvMatrixLocation_ = glGetUniformLocation(mShaderID, "MV");
		glUniformMatrix4fv(mvMatrixLocation_, 1, GL_FALSE, &mModelViewMatrix[0][0]);
		GLuint mvIMatrixLocation_ = glGetUniformLocation(mShaderID, "MVInverse");
		glUniformMatrix4fv(mvIMatrixLocation_, 1, GL_FALSE, &mMVInverseMatrix[0][0]);
		
		
		update();
}
//

void OpenGLQtContext::fps()
{
//	std::cout<< mFrameCounter << "fps"<< std::endl;
	emit setTitle(QString::number(mFrameCounter));
	mFrameCounter = 0;
}

//
// mouseReleaseEvent
//
// wird aufgerufen, wenn eine Maustaste losgelassen wird
//
//void OpenGLQtContext::mouseReleaseEvent(QMouseEvent *event)
//{
//
//}
//

