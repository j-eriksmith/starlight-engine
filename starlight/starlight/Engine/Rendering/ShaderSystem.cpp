#include "ShaderSystem.h"

ShaderComponentPtr ShaderSystem::CreateShaderComponent(const std::string& filePath)
{
	// Write our shader program
	ShaderProgramSource shaders(ParseShader(filePath));
	unsigned int id = CreateShader(shaders.VertexShader, shaders.FragmentShader);
	// TODO: Remove hard-coded screen height and width values
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)960 / (float)960, 0.1f, 100.0f);
	return ShaderComponentPtr(new (MemMgr::Alloc(sizeof(ShaderComponent), MemMgr::AllocatorType::GlobalData)) ShaderComponent(id, projection));
}

void ShaderSystem::Bind(const ShaderComponent& sh)
{
	GLCall(glUseProgram(sh.id));
}

void ShaderSystem::Unbind(const ShaderComponent& sh)
{
	GLCall(glUseProgram(0));
}

void ShaderSystem::BindTextures(ShaderComponent& shader, const Mesh& mesh)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	// Bind each of our mesh's textures to uniforms in our shader and use
	// them to draw our mesh
	for (unsigned int i = 0; i < mesh.textures.size(); ++i)
	{
		// Activate a new texture unit that we will bind our texture object to
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = mesh.textures[i].type;

		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		//Bind each Texture to a uniform in the given shader
		ShaderSystem::SetUniform1i(shader, name + number, i);
		GLCall(glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id));
	}
	// Reset our active texture back to the default
	glActiveTexture(GL_TEXTURE0);
}

void ShaderSystem::SetUniform1i(ShaderComponent& sh, const std::string& name, int value)
{

	GLCall(glUniform1i(GetUniformLocation(sh, name), value));
}

void ShaderSystem::SetUniform1f(ShaderComponent& sh, const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(sh, name), value));
}

void ShaderSystem::SetUniform3f(ShaderComponent& sh, const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(GetUniformLocation(sh, name), v0, v1, v2));
}

void ShaderSystem::SetUniform4f(ShaderComponent& sh, const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(sh, name), v0, v1, v2, v3));
}

void ShaderSystem::SetUniformMat4f(ShaderComponent& sh, const std::string& name, const glm::mat4& mat)
{
	int loc = GetUniformLocation(sh, name);
	GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]));
}

void ShaderSystem::SetUniformMat4f(ShaderComponent& sh, const std::string& name, const Matrix4& mat)
{
	int loc = GetUniformLocation(sh, name);
	// cast to float* might break things
	GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, reinterpret_cast<const float*>(&mat)));
}

void ShaderSystem::Update(float deltaTime)
{
}

int ShaderSystem::GetUniformLocation(ShaderComponent& sh, const std::string& name)
{
	if (sh.uniformLocationCache.find(name) != sh.uniformLocationCache.end())
	{
		return sh.uniformLocationCache[name];
	}
		
	GLCall(int location = glGetUniformLocation(sh.id, name.c_str()));
	if (location == -1)
		Log("WARNING: Uniform '" << name << "' not found");
	sh.uniformLocationCache[name] = location;
	return location;
}

unsigned int ShaderSystem::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
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

ShaderProgramSource ShaderSystem::ParseShader(const std::string& filePath)
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
	Log("vertex shader: " << ss[0].str());
	Log("fragment shader: " << ss[1].str());
	return ShaderProgramSource(ss[0].str(), ss[1].str());
}

unsigned int ShaderSystem::CompileShader(unsigned int type, const std::string & source)
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
		Log("Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex Shader: " : "Fragment Shader: ") <<
			message);
		glDeleteShader(id);
		return 0;
	}
	Log("Successfully compiled shader!");
	return id;
}

void ShaderSystem::TransferData(ShaderComponent* src, ShaderComponent* dst)
{
	dst->uniformLocationCache = src->uniformLocationCache;
	dst->projectionMatrix = src->projectionMatrix;
	Log("src id " << src->id);
	dst->id = src->id;
	Log("dst->id " << dst->id);
}