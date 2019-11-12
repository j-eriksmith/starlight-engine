#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filePath)
	:m_FilePath(filePath), m_RendererID(0)
{
	// Write our shader program
	ShaderProgramSource shaders = ParseShader(m_FilePath);
	m_RendererID = CreateShader(shaders.VertexShader, shaders.FragmentShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{

	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2 )
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{

	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
	int loc = GetUniformLocation(name);
	GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "WARNING: Uniform '" << name << "' not found" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}



ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType shaderType = ShaderType::NONE;
	std::ifstream stream(filePath);
	std::string line;
	// array of two stringstreams, one for the vertex shader and one for the fragment shader
	std::stringstream ss[2];
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				// vertex mode
				shaderType = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// fragment mode
				shaderType = ShaderType::FRAGMENT;
			}
		}
		else
		{
			int index = shaderType != ShaderType::NONE ? (int)shaderType : 0;
			ss[index] << line << "\n";
		}
	}
	std::cout << "vertex shader: " << ss[0].str() << std::endl;
	std::cout << "fragment shader: " << ss[1].str() << std::endl;
	return ShaderProgramSource(ss[0].str(), ss[1].str());
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// creates empty shader object and returns an id # by which it can be referenced
	// this shader object maintains the source code for the shader
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	// Replaces the source code in a shader object. In this case, we're replacing 
	// either the vertex or fragment shaders with our source code for each shader.
	glShaderSource(id, 1, &src, nullptr);
	// Compiles the source code that we just replaced in our vertex / fragment shader
	glCompileShader(id);
	// Error handling.
	// Check if shader has compiled properly.
	int result;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		std::unique_ptr<char[]> message(new char[length]);
		glGetShaderInfoLog(id, length, &length, message.get());
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex Shader: " : "Fragment Shader: ") <<
			message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	std::cout << "Successfully compiled shader!" << std::endl;
	return id;
}