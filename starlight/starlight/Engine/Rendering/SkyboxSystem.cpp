#include "SkyboxSystem.h"
#include "ShaderSystem.h"
#include "stb_image/stb_image.h"
#include "TransformComponent.h"
#include "CubemapComponent.h"
#include "ShaderComponent.h"
#include "Camera.h"

void SkyboxSystem::Update(float deltaTime)
{
	if (Components.size() > 1)
	{
		Log("More than one entity for SkyboxSystem to process!");
	}
	auto& SkyboxTuple = Components[0];
	CubemapComponent* cubemap = std::get<CubemapComponent*>(SkyboxTuple);
	ShaderComponent* shader = std::get<ShaderComponent*>(SkyboxTuple);
	Draw(cubemap, shader);
}

void SkyboxSystem::Draw(CubemapComponent* cubemap, ShaderComponent* shader) const
{
	GLCall(glDepthFunc(GL_LEQUAL));
	ShaderSystem::Bind(*shader);
	ShaderSystem::SetUniform1i(*shader, "skybox", 0);
	ShaderSystem::SetUniformMat4f(*shader, "view", glm::mat4(glm::mat3(Camera::GetViewMatrix())));
	ShaderSystem::SetUniformMat4f(*shader, "projection", shader->projectionMatrix);

	GLCall(glActiveTexture(GL_TEXTURE0));
	cubemap->CubemapVAO.Bind();
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, cubemap->CubemapTexture));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	GLCall(glBindVertexArray(0));
	GLCall(glDepthFunc(GL_LESS));
}

void SkyboxSystem::LoadTextures(const std::array<std::string, 6>& texturesToLoad, CubemapComponent& cubemapComp)
{
	unsigned int TextureID;
	GLCall(glGenTextures(1, &TextureID));
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	stbi_set_flip_vertically_on_load(0);
	
	int Width, Height, NumChannels;
	for (size_t i = 0; i < texturesToLoad.size(); ++i)
	{
		unsigned char* TextureData = stbi_load(texturesToLoad[i].c_str(), &Width, &Height, &NumChannels, 0);
		if (TextureData)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
		}
		else
		{
			Log("SkyboxSystem::LoadTextures -- Failed to load texture data for file " << texturesToLoad[i]);
		}
		stbi_image_free(TextureData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// VAO / VBO setup
	cubemapComp.CubemapVAO.Bind();
	cubemapComp.CubemapVBO.Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapComp.SkyboxVertices), &cubemapComp.SkyboxVertices, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
}
