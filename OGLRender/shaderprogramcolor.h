#ifndef SHADERPROGRAM_SIMPLE_H
#define SHADERPROGRAM_SIMPLE_H

#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include "shaderprogram.h"

class OGLRENDER_API ShaderProgramColor: public ShaderProgram
{
public:

	/// attribute id
	GLint idOfVertexAttribute;

	GLint idOfColorUniform;

    ShaderProgramColor();
};

#endif
