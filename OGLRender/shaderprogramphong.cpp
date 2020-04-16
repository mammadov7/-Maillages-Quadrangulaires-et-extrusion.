#include "shaderprogramphong.h"

ShaderProgramPhong::ShaderProgramPhong()
{
	// load & compile & link shaders
	load("phongshader.vert","phongshader.frag");

	// get id of uniforms
	idOfProjectionMatrix = glGetUniformLocation(m_programId, "projectionMatrix");
	idOfViewMatrix = glGetUniformLocation(m_programId, "viewMatrix");
	idOfNormalMatrix = glGetUniformLocation(m_programId, "normalMatrix");

	// get id of attribute
	idOfVertexAttribute = glGetAttribLocation(m_programId, "vertex_in");
	idOfNormalAttribute = glGetAttribLocation(m_programId, "normal_in");

	idOfColorUniform = glGetUniformLocation(m_programId, "color");
	idOfBColorUniform = glGetUniformLocation(m_programId, "bcolor");
}
