#ifndef SHADERPROGRAMPHONG_H
#define SHADERPROGRAMPHONG_H

#include "shaderprogram.h"

class OGLRENDER_API ShaderProgramPhong: public ShaderProgram
{
public:
public:

	/// attribute id
	GLint idOfVertexAttribute;
	GLint idOfNormalAttribute;

	GLint idOfColorUniform;
	GLint idOfBColorUniform;

	ShaderProgramPhong();
};

#endif // SHADERPROGRAMPHONG_H
