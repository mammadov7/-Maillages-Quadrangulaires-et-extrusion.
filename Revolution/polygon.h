#ifndef POLYGON_EDITOR_H
#define POLYGON_EDITOR_H

#include <GL/glew.h>
#include <OGLRender/shaderprogramcolor.h>
#include <vector>

#include <matrices.h>


class PolygonEditor
{
	std::vector<Vec3> m_points;
	GLuint m_vao;
	GLuint m_vbo;
	ShaderProgramColor* m_shader_color;

public:
	PolygonEditor();

	void draw(const Vec3& color);

	void add_vertex(float x, float y);

	void remove_last();

	void clear();

	void gl_init();

	void lisse();

	inline const std::vector<Vec3>& vertices() { return m_points; }
};

#endif // POLYGON_EDITOR_H
