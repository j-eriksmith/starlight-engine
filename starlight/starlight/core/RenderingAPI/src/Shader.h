#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
// struct that contains strings representing
struct ShaderProgramSource
{
	ShaderProgramSource()
	{}
	ShaderProgramSource(std::string vs, std::string fs)
		:VertexShader(vs), FragmentShader(fs)
	{}
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;

    int GetUniformLocation(const std::string& name);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	ShaderProgramSource ParseShader(const std::string& filePath);

	unsigned int CompileShader(unsigned int type, const std::string& source);

public:
	Shader(const std::string& fileName);

	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);

};