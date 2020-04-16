#ifndef GRID2D_H
#define GRID2D_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <OGLRender/shaderprogramcolor.h>
#include <vector>

class OGLRENDER_API Grid2d
{
	GLuint m_vao;
	GLuint m_vbo;
	ShaderProgramColor* m_shader_color;

public:
	Grid2d();
	void draw(const glm::vec3& color);
	void gl_init();
};


#endif // GRID2D_H
