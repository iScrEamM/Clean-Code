#include "Shader.h"

Shader::Shader() :
	shaderProgram(0)
{
	shaderProgram = glCreateProgram();
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
	shaderProgram = glCreateProgram();
	CreateShaderProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	if (shaderProgram > 0) {
		//Delete program only if is active
		glDeleteProgram(shaderProgram);
	}
}

std::string Shader::LoadFromFile(const std::string& filePath)
{
	std::ifstream openFile(filePath);
	std::stringstream sstream;

	if (!openFile.is_open())
	{
		std::cout << "Could not find the file: " << filePath << std::endl;
	}

	sstream << openFile.rdbuf();
	
	return sstream.str();
}

void Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmenShader)
{
	//duno why I couldn't just do simple
	//const GLchar* vertexPath = loadFromFile(vertexShader).c_str();
	//It works doe, so...
	std::string vertexPathString = LoadFromFile(vertexShader);
	std::string fragmentPathString = LoadFromFile(fragmenShader);

	const GLchar* vertexPath = vertexPathString.c_str();
	const GLchar* fragmentPath = fragmentPathString.c_str();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderID, 1, &vertexPath, NULL);
	glCompileShader(vertexShaderID);

	ShaderCompiledError(vertexShaderID);

	glShaderSource(fragmentShaderID, 1, &fragmentPath, NULL);
	glCompileShader(fragmentShaderID);

	ShaderCompiledError(fragmentShaderID);

	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);

	glLinkProgram(shaderProgram);

	ShaderLinkedError(vertexShaderID, fragmentShaderID);

	glDetachShader(shaderProgram, vertexShaderID);
	glDetachShader(shaderProgram, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(shaderProgram);
}

void Shader::ShaderCompiledError(GLuint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(shaderID);

		std::cout << '%s\n' << &infoLog[0] << std::endl;

		return;
	}
}

void Shader::ShaderLinkedError(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	GLint isLinked = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(shaderProgram);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		std::cout << '%s\n' << &infoLog[0] << std::endl;

		return;
	}
}

GLint Shader::GetUniformLocation(const std::string &uniformName) const
{
	return glGetUniformLocation(shaderProgram, uniformName.c_str());
}

void Shader::useProgram()
{
	glUseProgram(shaderProgram);
}

GLuint Shader::getShaderProgram() const
{
	return shaderProgram;
}

void Shader::UniformMatrix4fv(glm::mat4 matrix, std::string name)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
