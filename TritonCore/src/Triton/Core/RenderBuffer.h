#pragma once

#include "Triton\Core\Wrapers\Mesh.h"
#include "Triton\Core\Wrapers\Material.h"
#include "Triton\Core\Wrapers\Image.h"
#include "Triton\Core\Wrapers\Viewport.h"

#include "Triton\Config.h"

namespace Triton
{
	// Struct containing the state of the buffer
	struct BufferState
	{
		unsigned int VAOIndiceCount = 0;
		unsigned int FramebufferHeight = 0;
		unsigned int FramebufferWidth = 0;

		reference<PType::Shader> Shader;

		reference<Triton::PType::Context> Context;

		reference<PType::Framebuffer> Framebuffer;
	};

	class RenderCommand
	{
	public:
		virtual ~RenderCommand() {}

		virtual void execute(BufferState& bufferState) = 0;
	};

	namespace RCommands
	{

		class PushViewport : public RenderCommand
		{
		public:
			PushViewport(reference<Data::Viewport> viewport) : m_Viewport(viewport) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Viewport->Framebuffer->enable();

				bufferState.Framebuffer = m_Viewport->Framebuffer;
				bufferState.Context->renderer->height = m_Viewport->Height;
				bufferState.Context->renderer->width = m_Viewport->Width;
				bufferState.Context->renderer->recalc_matrices();
			}

		private:
			reference<Data::Viewport> m_Viewport;
		};

		class ClearCurrentViewport : public RenderCommand
		{
		public:
			ClearCurrentViewport() {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				bufferState.Framebuffer->clear(TR_CLEAR_COLOR);
			}
		};

		class PushMesh : public RenderCommand
		{
		public:
			PushMesh(reference<Data::Mesh> mesh) : m_Mesh(mesh) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Mesh->VAO->enable();
				bufferState.VAOIndiceCount = m_Mesh->VAO->getIndiceCount();
			}

		private:
			reference<Data::Mesh> m_Mesh;
		};

		class PushShader : public RenderCommand
		{
		public:
			PushShader(reference<PType::Shader> shader) : m_Shader(shader) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Shader->enable();
				bufferState.Shader = m_Shader;
			}

		private:
			reference<PType::Shader> m_Shader;
		};

		class PushMaterial : public RenderCommand
		{
		public:
			PushMaterial(reference<Data::Material> material) : m_Material(material) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Material->Shader->Program->enable();
				m_Material->Texture->enable();
				bufferState.Shader = m_Material->Shader->Program;

				bufferState.Shader->setBufferValue("LightBuffer", "specularPower", &m_Material->Shininess);

				//Shader->setUniformInt("material.matTexture", object()->Slot);
				//Shader->setUniformVector3("material.ambient", Ambient);
				//Shader->setUniformVector3("material.diffuse", Diffuse);
				//Shader->setUniformVector3("material.specular", Specular);
			}

		private:
			reference<Data::Material> m_Material;
		};

		class PushImage : public RenderCommand
		{
		public:
			PushImage(reference<Data::Image> image) : m_Image(image) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Image->Bitmap->enable();
				bufferState.VAOIndiceCount = m_Image->Bitmap->getIndiceCount();
			}

		private:
			reference<Data::Image> m_Image;
		};

		class UpdateUniformValues : public RenderCommand 
		{
		public:
			UpdateUniformValues(std::function<void(reference<PType::Shader>)> func)
				: m_Func(func) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Func(bufferState.Shader);
			}

		private:
			std::function<void(reference<PType::Shader>)> m_Func;
		};

		class UpdateShaderBuffer : public RenderCommand
		{
		public:
			UpdateShaderBuffer(PType::BufferUpdateType bufferToUpdate) : m_typeToUpdate(bufferToUpdate) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				bufferState.Shader->updateBuffers(m_typeToUpdate);
			}

		private:
			PType::BufferUpdateType m_typeToUpdate;
		};

		class ChangeContextSetting : public RenderCommand
		{
		public:
			ChangeContextSetting(std::function<void(reference<PType::Context>)> func)
				: m_Func(func) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Func(bufferState.Context);
			}

		private:
			std::function<void(reference<PType::Context>)> m_Func;
		};

		class PushCustomAction : public RenderCommand
		{
		public:
			PushCustomAction(std::function<void(BufferState&)> func)
				: m_Func(func) {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override
			{
				m_Func(bufferState);
			}

		private:
			std::function<void(BufferState&)> m_Func;
		};

		class Draw : public RenderCommand
		{
		public:
			Draw() {}

			// Inherited via RenderCommand
			virtual void execute(BufferState& bufferState) override;
		};
	}

	namespace Core
	{

		class RenderBuffer : public TritonClass
		{
		public:
			RenderBuffer();

			void newFrame();

			template <class T, typename... Args>
			void addCommand(Args&&... args)
			{
				m_commands.push_back(new T(std::forward<Args>(args)...));
			}

			void clear();

			void exec();

			virtual void onMessage(size_t message, void* payload) override;
		private:
			reference<PType::Context> m_context;

			std::vector<RenderCommand*> m_commands;
			BufferState m_bufferState;
		};

	}
}