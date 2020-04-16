#ifndef SHADERPROGRAMFLAT_H
#define SHADERPROGRAMFLAT_H

#include "shaderprogram.h"

class OGLRENDER_API ShaderProgramFlat: public ShaderProgram
{
public:

	/// attribute id
	GLint idOfVertexAttribute;

	GLint idOfColorUniform;
	GLint idOfBColorUniform;

	ShaderProgramFlat();

};

#endif // SHADERPROGRAMFLAT_H
