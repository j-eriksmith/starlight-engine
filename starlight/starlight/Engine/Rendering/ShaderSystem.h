#pragma once
#include "System.h"
#include "ShaderComponent.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "math/matrix4.h"
#include "Renderer.h"
#include "MemMgr.h"
#include <fstream>
#include <sstream>

using ShaderComponentPtr = std::shared_ptr<ShaderComponent>;

class ShaderSystem : public System<ShaderComponent>
{
	using BaseType = System<ShaderComponent>;
public:
	ShaderSystem()
		: BaseType(nullptr) {}
	ShaderSystem(Engine* pEntityEngine)
		: BaseType(pEntityEngine) {}

	virtual void Update(float deltaTime) override;

	static int GetUniformLocation(ShaderComponent& sh, const std::string& name);

	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	static ShaderProgramSource ParseShader(const std::string& filePath);

	static unsigned int CompileShader(unsigned int type, const std::string& source);

	static ShaderComponentPtr CreateShaderComponent(const std::string& fileName);

	static void Bind(const ShaderComponent& sh);
	static void Unbind(const ShaderComponent& sh);

	// Set uniforms
	static void SetUniform1i(ShaderComponent& sh, const std::string& name, int value);
	static void SetUniform1f(ShaderComponent& sh, const std::string& name, float value);
	static void SetUniform4f(ShaderComponent& sh, const std::string& name, float v0, float v1, float v2, float v3);
	static void SetUniform3f(ShaderComponent& sh, const std::string& name, float v0, float v1, float v2);
	static void SetUniformMat4f(ShaderComponent& sh, const std::string& name, const glm::mat4& mat);
	static void SetUniformMat4f(ShaderComponent& sh, const std::string& name, const Matrix4& mat);
};