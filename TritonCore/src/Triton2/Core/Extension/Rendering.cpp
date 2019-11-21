#include "TRpch.h"
#include "Rendering.h"

#include "TritonPlatform2/CrossTypes/Core/Renderer.h"
#include "Triton2/Core/Extension/Entity.h"
#include "Triton2/Core/Extension/Graphics.h"
#include "Triton2/Scene/Scene.h"
#include "Triton2/Core/AssetManager.h"
#include "Triton2/Scene/ECS.h"

#include "Triton2/Instance/Engine.h"
#include "Triton2/Core/Extension/Text.h"

#include "Triton2/Utility/Log.h"
#include "Triton2/Utility/Timer.h"

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
					shader->buffer_lights() = scene->lights();
					shader->buffer_matrices().Projection = renderer->projection();
					shader->buffer_matrices().View = scene->activeCamera()->viewMatrix();

					Vector3& pos = scene->activeCamera()->getPosition();
					Vector3& dir = scene->activeCamera()->getViewDirection();
					shader->buffer_camera().Position = Vector4(pos.x, pos.y, pos.z, 1.0f);
					shader->buffer_camera().ViewDirection = Vector4(dir.x, dir.y, dir.z, 1.0f);

					shader->enable();

					batch.Material->enableTextures();

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

		void renderText(const std::string& text, const std::string& fontName, Vector2 position, Graphics::Renderer* renderer, Core::AssetManager* assetManager)
		{
			// TEMPORARY
			static reference<Graphics::VAO> vao = nullptr;

			if (assetManager->hasAsset("textShader") && assetManager->hasAsset(fontName))
			{
				reference<Graphics::Texture> texture = nullptr;
				auto shader = assetManager->getAsset("textShader").as<ShaderAsset>();
				auto font = assetManager->getAsset(fontName).as<FontAsset>();

				if (shader->isCreated() && font->isCreated())
				{
					shader->shader()->buffer_matrices().Projection = renderer->orthographic();

					shader->shader()->enable();

					renderer->depthBufferState(false);
					renderer->alphaBlending(true);

					auto[width, height] = renderer->size();
					float x = 0 - (width / 2) + position.x;
					float y = 0 - (height / 2) + position.y;

					Triton::IO::Mesh data = Extension::buildVertexArray(font, text, x, y);
					if (!vao.valid())
					{
						data.DynamicBuffer = 1;
						vao = Engine::getInstance().graphicsContext().newVAO(data);
					}
					else
					{
						vao->updateVertices(data.vertices);
					}
					vao->enable();

					texture = font->texture();
					texture->enable();

					shader->shader()->update_matrices();
					renderer->render(vao->getIndiceCount());

					renderer->depthBufferState(true);
					renderer->alphaBlending(false);
				}
			}
		}
	}
}