#include "TRpch.h"
#include "Rendering.h"

#include "TritonPlatform2/CrossTypes/Core/Renderer.h"
#include "Triton2/Core/Extension/Entity.h"
#include "Triton2/Core/Extension/Graphics.h"
#include "Triton2/Scene/Scene.h"
#include "Triton2/Core/AssetManager.h"
#include "Triton2/Scene/ECS.h"

namespace Triton
{
	namespace Extension
	{
		void renderScene(reference<Scene> scene, Graphics::Renderer* renderer, Core::AssetManager* assetManager)
		{
			std::vector<ECS::Entity> renderables = entitiesToRender(scene);

			std::vector<RenderBatch> batches = constructBatches(scene, renderables, assetManager);

			reference<ECS::Registry>& registry = scene->entities();

			std::string currentMesh = "";
			std::string currentMaterial = "";

			reference<Graphics::VAO> vao = nullptr;
			reference<Graphics::Texture> texture = nullptr;
			reference<Graphics::Shader> shader = nullptr;

			for (const RenderBatch& batch : batches)
			{
				// Bind material if different one
				if (batch.Material->getName() != currentMaterial)
				{
					shader = batch.Material->shader()->shader();
					shader->enable();

					texture = batch.Material->mainTexture()->texture();
					texture->enable();

					currentMaterial = batch.Material->getName();
				}

				// Bind mesh if different one
				if (batch.Mesh->getName() != currentMesh)
				{
					vao = batch.Mesh->VAO();
					vao->enable();

					currentMesh = batch.Mesh->getName();
				}				

				// Loop over batch entities
				for (const ECS::Entity& entity : batch.Entities)
				{
					Components::Transform& transform = registry->get<Components::Transform>(entity);
					Matrix44 transformation = Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
					shader->buffer_matrices().Model = transformation;
					shader->update_matrices();
					renderer->render(vao->getIndiceCount());
				}
			}
		}
	}
}