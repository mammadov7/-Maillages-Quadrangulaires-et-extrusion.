#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>


#include <GL/glew.h>

#ifdef WIN32
#ifndef OGLRENDER_API
#if defined OGLRENDER_DLL_EXPORT
#define OGLRENDER_API __declspec(dllexport)
#else
#define OGLRENDER_API __declspec(dllimport)
#endif
#endif
#else
#define OGLRENDER_API
#endif




class Shader
{
public:
	/**
	 * @brief Shader
	 * @param type  GL_VERTEX_SHADER / GL_FRAGMENT_SHADER
	 */
	Shader(GLenum type);

	~Shader();

	GLuint shaderId() const			{ return m_shaderId; }

	/**
	 * @brief compileShader
	 * @param filename filename containing the source
	 * @return
	 */
	bool compileShader(const std::string& filename);


protected:
	/// id of shader
	GLuint m_shaderId;

	/**
	 * @brief readFileSrc
	 * @param filename
	 * @return
	 */
	std::string readFileSrc(const std::string& filename);

	/**
	 * @brief printInfoCompileShader
	 * @param msg
	 * @return
	 */
	bool printInfoCompileShader(const std::string& msg);

	/// path for shader source file reading in static
	static std::string* s_shaderPath;
};

//bool printGLErrors(const char* msg);

#endif
