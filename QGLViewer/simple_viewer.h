#ifndef _SIMPLE_VIEWER_H_
#define _SIMPLE_VIEWER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <functional>
#include <iostream>
#include "qglviewer.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCoreApplication>


class SimpleViewer: public QGLViewer
{
	glm::vec3 back_color_;
	float scene_radius_;
public:
	std::function<void()> f_init;
	std::function<void()> f_draw;
	std::function<void(Qt::MouseButton, float, float)> f_mousePress;
	std::function<void(int,Qt::KeyboardModifiers)> f_keyPress;
	std::function<void()> f_animate;
	std::function<void()> f_close;
	std::function<void(Qt::MouseButton,const glm::vec3&,const glm::vec3&)> f_mousePress3D;

	inline static void init_gl(int gl_major=3, int gl_minor=3)
	{
		QGLFormat glFormat;
		glFormat.setVersion(gl_major,gl_minor);
		glFormat.setProfile(QGLFormat::CoreProfile);
		glFormat.setSampleBuffers( true );
		QGLFormat::setDefaultFormat(glFormat);
	}

	inline SimpleViewer(const glm::vec3& back_color, float scene_radius):
		back_color_(back_color),scene_radius_(scene_radius)
	{
		f_init = [](){};
		f_draw = [](){};
		f_mousePress = [](Qt::MouseButton, float, float){};
		f_keyPress = [](int,Qt::KeyboardModifiers){};
		f_animate = [](){};
		f_close = [](){};
		f_mousePress3D = [](Qt::MouseButton,const glm::vec3&,const glm::vec3&){};
	}


protected:
	inline void init()
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

	inline void draw()
	{
		makeCurrent();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		f_draw();
	}

	inline void mousePressEvent(QMouseEvent *event)
	{
		float x = (float(event->x()) / this->width())*2.0 - 1.0;
		float y = (float(this->height() - event->y()) / this->height())*2.0 - 1.0;
		f_mousePress(event->button(),x,y);

		qglviewer::Vec Pq = camera()->unprojectedCoordinatesOf(qglviewer::Vec(event->x(), event->y(), -1.0));
		qglviewer::Vec Qq = camera()->unprojectedCoordinatesOf(qglviewer::Vec(event->x(), event->y(), 1.0));
		glm::vec3 P(Pq[0],Pq[1],Pq[2]);
		glm::vec3 Dir(Qq[0]-Pq[0],Qq[1]-Pq[1],Qq[2]-Pq[2]);
		f_mousePress3D(event->button(),P,Dir);

		QGLViewer::mousePressEvent(event);
	}

	inline void keyPressEvent(QKeyEvent *event)
	{
		int e = event->key();

		if (e == Qt::Key_Escape)
			QCoreApplication::quit();

		f_keyPress(event->key(),event->modifiers());
		QGLViewer::keyPressEvent(event);
	}


	inline void animate()
	{
		f_animate();
	}

	inline void closeEvent(QCloseEvent*)
	{
		f_close();
	}


public:
	inline glm::mat4 getCurrentModelViewMatrix() const
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


	inline glm::mat4 getCurrentProjectionMatrix() const
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
};

#endif
