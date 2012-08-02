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
	
	VolumeLoader* l = new VolumeLoader("res/foot_w256_h256_d256_c1_b8.raw");
	l->loadData();
	
	mTree = new BrickTree(l->getData() , l->getDimension().width , l->getDimension().height , l->getDimension().depth, glm::vec3(0.0f, 0.0f, -10.0f));

	
    
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


	glClearDepth(1.0f);
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable( GL_BLEND );


	//glEnable( GL_DEPTH_TEST );
	//glDepthFunc(GL_LESS);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	initShader();
	initMatrices();

	std::vector<glm::vec2> vertices;
	{
		vertices.push_back(glm::vec2(-1.0f, -1.0f));
		vertices.push_back(glm::vec2( 1.0f, -1.0f));
		vertices.push_back(glm::vec2(-1.0f,  1.0f));
		vertices.push_back(glm::vec2( 1.0f,  1.0f));
	}
	std::cout << "#vertices: " << vertices.size() << std::endl;

	std::vector<GLuint> indices;
	{
		indices.push_back(GLuint(0));
		indices.push_back(GLuint(1));
		indices.push_back(GLuint(2));
		indices.push_back(GLuint(3));
	}
	std::cout << "#indices: " << indices.size() << std::endl;

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVBuf);
	glGenBuffers(1, &mIBuf);

	glBindVertexArray(mVao);
	{	
		glBindBuffer(GL_ARRAY_BUFFER, mVBuf);
		glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
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
	mModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	mViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0));
	mModelViewMatrix = mViewMatrix * mModelMatrix;
	mNormalMatrix = glm::transpose(glm::inverse(mModelViewMatrix));
	mProjectionMatrix = glm::perspective(60.0f, float(800) / float(600), 0.1f, 100.f);
	mMVPMatrix = mProjectionMatrix * mModelViewMatrix;


	glUseProgram(mShaderID);
	GLuint mvpMatrixLocation = glGetUniformLocation(mShaderID, "MVP");
	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mMVPMatrix[0][0]);
	GLuint mvMatrixLocation = glGetUniformLocation(mShaderID, "MV");
	glUniformMatrix4fv(mvMatrixLocation, 1, GL_FALSE, &mModelViewMatrix[0][0]);
	GLuint normalMatrixLocation = glGetUniformLocation(mShaderID, "normalMatrix");
	glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, &mNormalMatrix[0][0]);
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

	glUseProgram(mShaderID);
	GLuint mvpMatrixLocation = glGetUniformLocation(mShaderID, "MVP");
	glUniformMatrix4fv(mvpMatrixLocation, 1, GL_FALSE, &mMVPMatrix[0][0]);
	glBindVertexArray(mVao);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
		
	mTree->updateCut(glm::vec3(578.0f, 120.0f, 564.0f));
	
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
std::cout << "pblub";
	glViewport(0, 0, width, height);
	resize(width, height);
	mProjectionMatrix = glm::perspective(60.0f, float(width) / float(height), 0.1f, 100.f);

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

		
		default:		
					std::cout << "QGL: Key nicht belegt" << std::endl;
	}

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
}
//


//
// mouseMoveEvent
//
// wird aufgerufen, wenn die Maus bewegt wird
//
void OpenGLQtContext::mouseMoveEvent(QMouseEvent *event)
{
	//int dx = event->x() - lastPos_.x();
	//int dy = event->y() - lastPos_.y();


	//lastPos_ = event->pos();

	update();
}
//


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

