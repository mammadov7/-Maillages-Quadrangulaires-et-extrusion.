#include "shader.h"

#include <fstream>

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

std::string* Shader::s_shaderPath=NULL;

Shader::Shader(GLenum type)
{
	if (s_shaderPath==NULL)
	{
		s_shaderPath = new std::string(STRINGIFY(SHADERPATH));
		std::cout << "SHADER PATH = " <<*s_shaderPath<< std::endl;
	}

	m_shaderId = glCreateShader(type);
}


Shader::~Shader()
{
	glDeleteShader(m_shaderId);
}



/**
 * @brief Shader::compileShader
 * @param filename fichier source
 * @return
 */
bool Shader::compileShader(const std::string& filename)
{
	//Charge le fichier source
	std::string src = readFileSrc(*s_shaderPath+'/'+filename);

	// envoit du code du shader au driver
	const char *shaderSource = src.c_str();
	glShaderSource(m_shaderId, 1, &shaderSource, NULL);

	//Compilation du shader
	glCompileShader(m_shaderId);

	// info de compilation
	if (!printInfoCompileShader(filename))
	{
		std::size_t p = src.find("130");
		src[p]='3';
		const char *shaderSource = src.c_str();
		glShaderSource(m_shaderId, 1, &shaderSource, NULL);

		glCompileShader(m_shaderId);
	}
	if (printInfoCompileShader(filename))
		std::cout << "compil of "<< filename << " OK"<< std::endl;
	return true;
}


/**
 * Ouvre un fichier source shader et le lit
 * @param filename fichier source
 * @return le code source si ok sinon une chaine vide
 */
std::string Shader::readFileSrc(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (file)
	{
		std::string str;
		file.seekg(0, std::ios::end);
		str.reserve(file.tellg());
		file.seekg(0, std::ios::beg);
		str.assign((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
		return str;
	}
	std::cerr << "Shader : cannot open " <<  filename << std::endl;
	return "";

}


/**
* Affiche les erreurs de compilation des shaders
*
* @param shader
* @return false en cas d'erreur
*/
bool Shader::printInfoCompileShader(const std::string& msg)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &infologLength);

	if(infologLength > 1)
	{
		infoLog = (char *)malloc(infologLength+1);
		glGetShaderInfoLog(m_shaderId, infologLength, &charsWritten, infoLog);

		std::cerr << "--------------"<< std::endl << "compilation de " << msg <<  " : "<<std::endl << infoLog <<std::endl<< "--------------"<< std::endl;
		free(infoLog);
	}

	return (infologLength==1);
}

