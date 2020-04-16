#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "shader.h"




class OGLRENDER_API ShaderProgram
{
public:
    ShaderProgram();

    ~ShaderProgram();

	GLuint programId() const				{ return m_programId; }
	Shader* vertShader() const				{ return m_vertShader; }
	Shader* fragShader() const				{ return m_fragShader; }

    inline void startUseProgram()					{ glUseProgram(m_programId); }
    inline void stopUseProgram()					{ glUseProgram(0);	}

	inline void sendViewMatrix(const glm::mat4& viewMatrix)
	{
		glUniformMatrix4fv(idOfViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		if (idOfNormalMatrix >= 0)
		{
			glm::mat4 nm = glm::inverseTranspose(viewMatrix);
			glUniformMatrix4fv(idOfNormalMatrix, 1, GL_FALSE, glm::value_ptr(nm));
		}
	}

	inline void sendProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		glUniformMatrix4fv(idOfProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}


	/// uniform id pour matrice de projection
	GLint idOfProjectionMatrix;

	/// uniform id pour matrice de model-view
	GLint idOfViewMatrix;

	/// uniform id pour matrice de normal
	GLint idOfNormalMatrix;

protected:

	GLuint m_programId;
	Shader* m_vertShader;
	Shader* m_fragShader;

	/// link information error if necessary
	bool printInfoLinkProgram();

    /**
     * @brief load & compile shaders
     * @param vert_name vertex shader file name
     * @param frag_name fragment shader file name
     */
    void load(const std::string& vert_name, const std::string& frag_name);


};

#endif
