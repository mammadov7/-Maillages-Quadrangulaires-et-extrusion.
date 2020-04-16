#include "simple_viewer.h"

SimpleViewer::SimpleViewer(const glm::vec3& back_color, float scene_radius):
	back_color_(back_color),scene_radius_(scene_radius)
{
	f_init = [](){};
	f_draw = [](){};
	f_mousePress = [](Qt::MouseButton, float, float){};
	f_keyPress = [](int,Qt::KeyboardModifiers){};
	f_animate = [](){};
	f_close = [](){};
}


void SimpleViewer::init()
{
	makeCurrent();
	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		qDebug("Error %s", glewGetErrorString(glewErr) ) ;
	}
	std::cout << "GL VERSION = " << glGetString(GL_VERSION) << std::endl;
	glClearColor(back_color_[0],back_color_[1],back_color_[2],0.0);
	this->setSceneRadius(scene_radius_);
	this->setSceneCenter(qglviewer::Vec(0.0,0.0,0.0));
	camera()->showEntireScene();

	f_init();
}

void SimpleViewer::draw()
{
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	f_draw();
}

void SimpleViewer::mousePressEvent(QMouseEvent *event)
{
	float x = (float(event->x()) / this->width())*2.0 - 1.0;
	float y = (float(this->height() - event->y()) / this->height())*2.0 - 1.0;
	f_mousePress(event->button(),x,y);
	QGLViewer::mousePressEvent(event);
	QGLViewer::update();
}

void SimpleViewer::keyPressEvent(QKeyEvent *event)
{
	int e = event->key();

	if (e == Qt::Key_Escape)
		QCoreApplication::quit();

	f_keyPress(event->key(),event->modifiers());
	QGLViewer::keyPressEvent(event);
	QGLViewer::update();
}


void SimpleViewer::animate()
{
	f_animate();
}

void SimpleViewer::closeEvent(QCloseEvent*)
{
	f_close();
}


glm::mat4 SimpleViewer::getCurrentModelViewMatrix() const
{
	GLdouble gl_mvm[16];
	camera()->getModelViewMatrix(gl_mvm);
	glm::mat4 mvm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			mvm[i][j] = float(gl_mvm[i*4+j]);
	}
	return mvm;
}


glm::mat4 SimpleViewer::getCurrentProjectionMatrix() const
{
	GLdouble gl_pm[16];
	camera()->getProjectionMatrix(gl_pm);
	glm::mat4 pm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			pm[i][j] = float(gl_pm[i*4+j]);
	}
	return pm;
}
