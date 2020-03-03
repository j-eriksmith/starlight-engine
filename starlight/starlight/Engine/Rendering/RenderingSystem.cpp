#include "RenderingSystem.h"
#include "ShaderSystem.h"
#include "Camera.h"

void RenderingSystem::Update(float deltaTime)
{
	Log("Updating...");
	for (auto& ComponentTuple : Components)
	{
		RenderableComponent* renderComponent = std::get<RenderableComponent*>(ComponentTuple);
		ShaderComponent* shaderComponent = std::get<ShaderComponent*>(ComponentTuple);
		if (renderComponent->shouldRender)
		{
			TransformComponent* transformComponent = std::get<TransformComponent*>(ComponentTuple);
			Draw(renderComponent, transformComponent, shaderComponent);
		}
	}
}

void RenderingSystem::Draw(RenderableComponent* rc, TransformComponent* tc, ShaderComponent* sh)
{
	Log("Rendering...");
	//Log("ShaderComponent Mem Address: " << reinterpret_cast<uintptr_t>(sh));
	ShaderSystem::Bind(*sh);
	// Add the transform as the 'model' matrix uniform into the shader component
	//std::cout << tc->Data;
	ShaderSystem::SetUniformMat4f(*sh, "model", *tc->Data.GetMat4());
	// TODO: Bind the view and projection matrices to the shader component
	ShaderSystem::SetUniformMat4f(*sh, "view", Camera::GetViewMatrix());
	// Convert 'Mesh-Draw() into a proper ECS rendering implementation
	ShaderSystem::SetUniformMat4f(*sh, "projection", sh->projectionMatrix);
	for (const std::shared_ptr<Mesh>& mesh : rc->meshes)
	{
		//mesh->BindTextures(*sh);
		ShaderSystem::BindTextures(*sh, *mesh);
		// GL render call
		mesh->vao.Bind();
		mesh->ibo.Bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GLCall(glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
	}
}

void RenderingSystem::TransferData(RenderableComponent* src, RenderableComponent* dst)
{
	dst->meshes = src->meshes;
	dst->bb = src->bb;
	dst->shouldRender = src->shouldRender;
}

void RenderingSystem::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	// Clear the depth buffer from previous frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}