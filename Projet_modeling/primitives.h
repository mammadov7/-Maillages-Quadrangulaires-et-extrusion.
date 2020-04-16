#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>
#include <OGLRender/shaderprogramflat.h>
#include <matrices.h>

class Primitives
{
	Mat4 viewMatrix;
	Mat4 projectionMatrix;

	std::vector<Vec3> m_points;
	std::vector<int> m_indices_cube;
	std::vector<int> m_indices_sphere;
	std::vector<int> m_indices_cylinder;
	std::vector<int> m_indices_cone;

	void add_cylinder(int sides, float radius, std::vector<int>& indices);
	void add_cone(int sides, float radius, std::vector<int>& indices);
	void add_sphere(int sides, float radius, std::vector<int>& indices);

	ShaderProgramFlat* m_shader_flat;
	GLuint m_vao;
	GLuint m_vbo;

	GLuint m_ebo_cube;
	GLuint m_ebo_cylinder;
	GLuint m_ebo_cone;
	GLuint m_ebo_sphere;

public:
	Primitives();

	void gl_init();

	void set_matrices(const Mat4& view, const Mat4& projection);

	void draw_cube(const Mat4& transfo, const Vec3& color) const;

	void draw_cone(const Mat4& transfo, const Vec3& color) const;

	void draw_sphere(const Mat4& transfo, const Vec3& color) const;

	void draw_cylinder(const Mat4& transfo, const Vec3& color) const;

	inline const std::vector<Vec3>& getPoints() const { return m_points;}

	inline const std::vector<int>& getCubeIndices() const {return m_indices_cube;}

	inline const std::vector<int>& getConeIndices() const {return m_indices_cone;}

	inline const std::vector<int>& getCylinderIndices() const {return m_indices_cylinder;}

};

#endif // PRIMITIVES_H
