#include "meshtri.h"
#include "matrices.h"



void MeshTri::clear()
{
	m_points.clear();
	m_normals.clear();
	m_line_indices.clear();
	m_tri_indices.clear();
}


int MeshTri::add_vertex(const Vec3& P)
{
	// ajouter P à m_points
	// renvoyer l'indice du point
}

int MeshTri::add_normal(const Vec3& N)
{
	// ajouter P à m_normals
	// renvoyer l'indice du point
}

void MeshTri::add_line(int i1, int i2)
{

}


void MeshTri::add_tri(int i1, int i2, int i3)
{

}

void MeshTri::add_quad(int i1, int i2, int i3, int i4)
{

}


void MeshTri::create_pyramide()
{
	clear();
	// ajouter les 5 points
	// ajouter les 8 lignes (segments)
	// ajouter les 5 faces
	gl_update();
}

void MeshTri::anneau()
{
	clear();
	// ajouter les points
	// ajouter les lignes
	// ajouter les faces
	gl_update();
}

void MeshTri::spirale()
{
	clear();
	// ajouter les points
	// ajouter les lignes
	// ajouter les faces
	gl_update();
}


void MeshTri::revolution(const std::vector<Vec3>& poly)
{
	clear();
	// ajouter les points
	// ajouter les normales au sommets
	// ajouter les lignes
	// ajouter les faces

	// Faire varier angle 0 -> 360 par pas de D degre
	// Faire tourner les sommets du polygon -> nouveau points

	// on obtient une grille de M x N (360/D x poly.nb) points

	// pour la creation des quads qui relient ces points,
	// attention la derniere rangee doit etre reliee a la premiere

	gl_update();
}




MeshTri::MeshTri()
{}



void MeshTri::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_phong = new ShaderProgramPhong();
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo2);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//VAO2
	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_phong->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_phong->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glEnableVertexAttribArray(m_shader_phong->idOfNormalAttribute);
	glVertexAttribPointer(m_shader_phong->idOfNormalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);


	//EBO indices
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_ebo2);
}

void MeshTri::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (!m_normals.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
		glBufferData(GL_ARRAY_BUFFER, 3 * m_normals.size() * sizeof(GLfloat), &(m_normals[0][0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//EBO indices
	if (!m_tri_indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_tri_indices.size() * sizeof(int), &(m_tri_indices[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (!m_line_indices.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_line_indices.size() * sizeof(int), &(m_line_indices[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}



void MeshTri::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}

void MeshTri::draw_lines(const Vec3& color)
{
	m_shader_color->startUseProgram();

	m_shader_color->sendViewMatrix(viewMatrix);
	m_shader_color->sendProjectionMatrix(projectionMatrix);

	glBindVertexArray(m_vao);

	if (has_faces())
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);
		glUniform3f(m_shader_color->idOfColorUniform, 0,0,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
		glDrawElements(GL_TRIANGLES, m_tri_indices.size(),GL_UNSIGNED_INT,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(color));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo2);
	glDrawElements(GL_LINES, m_line_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_color->stopUseProgram();
}


void MeshTri::draw_flat(const Vec3& color)
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_tri_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}


void MeshTri::draw_smooth(const Vec3& color)
{
	m_shader_phong->startUseProgram();

	m_shader_phong->sendViewMatrix(viewMatrix);
	m_shader_phong->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_phong->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_tri_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_phong->stopUseProgram();
}




