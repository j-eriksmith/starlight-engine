#include "RenderingSystem.h"
#include "ShaderSystem.h"

void RenderingSystem::Update(float deltaTime)
{
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
	ShaderSystem::Bind(*sh);
	// Add the transform as the 'model' matrix uniform into the shader component
	ShaderSystem::SetUniformMat4f(*sh, "model", *(tc->Data.GetMat4()));
	// TODO: Bind the view and projection matrices to the shader component
	ShaderSystem::SetUniformMat4f(*sh, "view", glm::mat4());
	// Convert 'Mesh-Draw() into a proper ECS rendering implementation
	ShaderSystem::SetUniformMat4f(*sh, "projection", sh->projectionMatrix);
	for (const std::shared_ptr<Mesh>& mesh : rc->meshes)
	{
		mesh->BindTextures(*sh);
		// GL render call
		mesh->vao.Bind();
		mesh->ibo.Bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GLCall(glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
	}
}