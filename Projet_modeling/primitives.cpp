#include <cmath>
#include "primitives.h"


void Primitives::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}

void Primitives::add_cylinder(int sides, float radius, std::vector<int>& indices)
{
	int beg = m_points.size();
	float a = M_PI/4.0f;
	float b = 2.0f*M_PI/sides;
	for (int i=0;i<sides;++i)
	{
		m_points.push_back(Vec3(radius*cos(a),radius*sin(a),-0.5));
		m_points.push_back(Vec3(radius*cos(a),radius*sin(a),0.5));
		a+=b;
	}

	int cb = m_points.size();
	m_points.push_back(Vec3(0.0,0.0,-0.5));
	int ch = m_points.size();
	m_points.push_back(Vec3(0.0,0.0, 0.5));

	int sides2 = 2*sides;

	for (int i=0;i<sides;++i)
	{
		indices.push_back(beg+2*i);
		indices.push_back(beg+(2*i+2)%sides2);
		indices.push_back(beg+(2*i+1)%sides2);
		indices.push_back(beg+(2*i+3)%sides2);
		indices.push_back(beg+(2*i+1)%sides2);
		indices.push_back(beg+(2*i+2)%sides2);

		indices.push_back(beg+(2*i+2)%sides2);
		indices.push_back(beg+2*i);
		indices.push_back(cb);

		indices.push_back(beg+(2*i+1)%sides2);
		indices.push_back(beg+(2*i+3)%sides2);
		indices.push_back(ch);
	}
}



void Primitives::add_cone(int sides, float radius, std::vector<int>& indices)
{
	int beg = m_points.size();
	float a = 0.0f;
	float b = 2.0f*M_PI/sides;
	for (int i=0;i<sides;++i)
	{
		m_points.push_back(Vec3(radius*cos(a),radius*sin(a),-0.5));
		a+=b;
	}

	int cb = m_points.size();
	m_points.push_back(Vec3(0.0,0.0,-0.5));
	int ch = m_points.size();
	m_points.push_back(Vec3(0.0,0.0, 0.5));

	for (int i=0;i<sides;++i)
	{
		indices.push_back(beg+i);
		indices.push_back(beg+(i+1)%sides);
		indices.push_back(ch);

		indices.push_back(beg+(i+1)%sides);
		indices.push_back(beg+i);
		indices.push_back(cb);
	}
}





void Primitives::add_sphere(int sides, float radius, std::vector<int>& indices)
{
	int beg = m_points.size();

	int nbPara = sides;
	int nbMeri = sides;

	float a1 = (180.0/(nbPara+1))*M_PI/180.0;
	float a2 = (360.0/nbMeri)*M_PI/180.0;

	// les paralleles
	for (int i= 0; i< nbPara; ++i)
	{
		float angle = -M_PI/2.0 + a1*(i+1);
		float z = radius*sin(angle);
		float rad = radius*cos(angle);

		for  (int j=0; j< nbMeri; ++j)
		{
			m_points.push_back(Vec3(rad*cos(a2*j), rad*sin(a2*j),z));
		}
	}
	// les poles
	m_points.push_back(Vec3(0.0,0.0,-radius));
	m_points.push_back(Vec3(0.0,0.0, radius));

	// triangles
	for (int i= 0; i< (nbPara-1); ++i)
	{
		for  (int j=0; j< nbMeri; ++j)
		{
			indices.push_back(beg+nbMeri*i+j);
			indices.push_back(beg+nbMeri*i+(j+1)%nbMeri);
			indices.push_back(beg+nbMeri*(i+1)+(j+1)%nbMeri);
			indices.push_back(beg+nbMeri*((i+1))+(j+1)%nbMeri);
			indices.push_back(beg+nbMeri*((i+1))+j);
			indices.push_back(beg+nbMeri*i+j);
		}
	}
	// poles
	for  (int j=0; j< nbMeri; ++j)
	{
		indices.push_back(beg+nbMeri*nbPara);
		indices.push_back(beg+(j+1)%nbMeri);
		indices.push_back(beg+j);
	}
	for  (int j=0; j< nbMeri; ++j)
	{
		indices.push_back(beg+nbMeri*nbPara+1);
		indices.push_back(beg+nbMeri*(nbPara-1)+j);
		indices.push_back(beg+nbMeri*(nbPara-1)+(j+1)%nbMeri);
	}
}


void Primitives::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();

	//VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	const std::vector<Vec3>& points = getPoints();
	glBufferData(GL_ARRAY_BUFFER, 3 * points.size() * sizeof(GLfloat), &(points.front()[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//EBO indices
	glGenBuffers(1, &m_ebo_cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices_cube.size() * sizeof(int), &(m_indices_cube.front()), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo_cone);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_cone);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices_cone.size() * sizeof(int), &(m_indices_cone.front()), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo_cylinder);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_cylinder);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices_cylinder.size() * sizeof(int), &(m_indices_cylinder.front()), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo_sphere);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_sphere);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices_sphere.size() * sizeof(int), &(m_indices_sphere.front()), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Primitives::draw_cube(const Mat4& transfo, const Vec3& color) const
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix*transfo);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glUniform3fv(m_shader_flat->idOfBColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo_cube);
	glDrawElements(GL_TRIANGLES, m_indices_cube.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}

void Primitives::draw_cylinder(const Mat4& transfo, const Vec3& color) const
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix*transfo);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glUniform3fv(m_shader_flat->idOfBColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo_cylinder);
	glDrawElements(GL_TRIANGLES, m_indices_cylinder.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}

void Primitives::draw_cone(const Mat4& transfo, const Vec3& color) const
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix*transfo);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glUniform3fv(m_shader_flat->idOfBColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo_cone);
	glDrawElements(GL_TRIANGLES, m_indices_cone.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}

void Primitives::draw_sphere(const Mat4& transfo, const Vec3& color) const
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix*transfo);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glUniform3fv(m_shader_flat->idOfBColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo_sphere);
	glDrawElements(GL_TRIANGLES, m_indices_sphere.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}


Primitives::Primitives()
{
	add_cylinder(32, 0.5f, m_indices_cylinder);
	add_cone(32,0.5f,m_indices_cone);
	add_cylinder(4,0.7071,m_indices_cube);
	add_sphere(32,0.5f,m_indices_sphere);
}
