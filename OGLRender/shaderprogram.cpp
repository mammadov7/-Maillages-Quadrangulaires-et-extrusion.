#include "shaderprogram.h"



ShaderProgram::ShaderProgram():
    m_vertShader(NULL),
    m_fragShader(NULL)
{
	m_programId = glCreateProgram();
}


ShaderProgram::~ShaderProgram()
{
    if (m_vertShader)
        delete m_vertShader;
    if (m_fragShader)
        delete m_fragShader;

	glDeleteProgram(m_programId);
}



bool ShaderProgram::printInfoLinkProgram()
{
	//Print log if needed
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(m_programId, infologLength, &charsWritten, infoLog);

		std::cerr << "Link message :" << infoLog;
		free(infoLog);
	}

	return (infologLength == 1);
}


void ShaderProgram::load(const std::string& vert_name, const std::string& frag_name)
{
    //Création du vertex shader
    m_vertShader = new Shader(GL_VERTEX_SHADER);
    m_vertShader->compileShader(vert_name);

    //Création du fragment shader
    m_fragShader = new Shader(GL_FRAGMENT_SHADER);
    m_fragShader->compileShader(frag_name);

    //Attachement des shaders au programme et link
    glAttachShader(m_programId, m_vertShader->shaderId());
    glAttachShader(m_programId, m_fragShader->shaderId());
    glLinkProgram(m_programId);
    // puis detache (?)
    glDetachShader(m_programId, m_fragShader->shaderId());
    glDetachShader(m_programId, m_vertShader->shaderId());
}

