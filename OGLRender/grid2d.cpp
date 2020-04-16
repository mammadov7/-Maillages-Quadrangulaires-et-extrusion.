#include "grid2d.h"

Grid2d::Grid2d()
{}

void Grid2d::gl_init()
{
	m_shader_color = new ShaderProgramColor();
	std::vector<float> points={-1,0,0, 1,0,0, 0,-1,0, 0,1,0};
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(float), &points.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}

void Grid2d::draw(const glm::vec3& color)
{
	glm::mat4 id;
	m_shader_color->startUseProgram();
	m_shader_color->sendViewMatrix(id);
	m_shader_color->sendProjectionMatrix(id);
	glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(color));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
	m_shader_color->stopUseProgram();
}
