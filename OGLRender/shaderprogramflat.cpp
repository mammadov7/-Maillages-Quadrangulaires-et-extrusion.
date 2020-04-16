#include "shaderprogramflat.h"

ShaderProgramFlat::ShaderProgramFlat()
{
	// load & compile & link shaders
	load("flatshader.vert","flatshader.frag");

	// get id of uniforms
	idOfProjectionMatrix = glGetUniformLocation(m_programId, "projectionMatrix");
	idOfViewMatrix = glGetUniformLocation(m_programId, "viewMatrix");

	// get id of attribute
	idOfVertexAttribute = glGetAttribLocation(m_programId, "vertex_in");

	idOfColorUniform = glGetUniformLocation(m_programId, "color");
	idOfBColorUniform = glGetUniformLocation(m_programId, "bcolor");
}


