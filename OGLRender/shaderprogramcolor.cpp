#include "shaderprogramcolor.h"


ShaderProgramColor::ShaderProgramColor()
{
    // load & compile & link shaders
	load("colorshader.vert","colorshader.frag");

    // get id of uniforms
    idOfProjectionMatrix = glGetUniformLocation(m_programId, "projectionMatrix");
    idOfViewMatrix = glGetUniformLocation(m_programId, "viewMatrix");

    // get id of attribute
    idOfVertexAttribute = glGetAttribLocation(m_programId, "vertex_in");

	idOfColorUniform = glGetUniformLocation(m_programId, "color");
}
