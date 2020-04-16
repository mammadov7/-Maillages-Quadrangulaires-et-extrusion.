#ifndef _SIMPLE_VIEWER_H_
#define _SIMPLE_VIEWER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <functional>
#include <iostream>
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/quaternion.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCoreApplication>
#include "ShaderProgram.h"


#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

class SimpleViewer: public QGLViewer
{
public:
	std::function<void()> f_init;
	std::function<void()> f_draw;
	std::function<void(Qt::MouseButton, float, float)> f_mousePress;
	std::function<void(int,Qt::KeyboardModifiers)> f_keyPress;
	std::function<void()> f_animate;
	std::function<void()> f_close;

	/**
	 * @brief init_gl
	 * @warning NEED TO BE CALLED FIRST
	 * @param gl_major
	 * @param gl_minor
	 */
	inline static void init_gl(int gl_major=3, int gl_minor=3)
	{
		QGLFormat glFormat;
		glFormat.setVersion(gl_major,gl_minor);
        glFormat.setProfile( QGLFormat::CoreProfile );
		glFormat.setSampleBuffers( true );
		QGLFormat::setDefaultFormat(glFormat);
	}

	SimpleViewer(const glm::vec3& back_color, float scene_radius);

	glm::mat4 getCurrentModelViewMatrix() const;

	glm::mat4 getCurrentProjectionMatrix() const;

protected:
	void init();

	void draw();

	void mousePressEvent(QMouseEvent *event);

	void keyPressEvent(QKeyEvent *event);

	void animate();

	void closeEvent(QCloseEvent*);

	glm::vec3 back_color_;
	float scene_radius_;
};

#endif
