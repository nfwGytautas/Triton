#include "TRpch.h"
#include "WindowsRenderable.h"

#include <glad\glad.h>
#include "WindowsModel.h"
#include "WindowsTexture.h"
#include "Triton\Logger\Log.h"

#include "Triton\Core\Platform.h"

namespace Triton
{
	std::shared_ptr<Triton::Core::Renderable> Platform::Create(std::shared_ptr<Triton::Core::Shader> aShader, std::shared_ptr<Data::Mesh> aMesh)
	{
		return std::make_shared<Triton::Core::WindowsRenderable>(aShader, aMesh);
	}

	namespace Core
	{
		WindowsRenderable::WindowsRenderable(std::shared_ptr<Shader> aShader, std::shared_ptr<Data::Mesh> aMesh)
			: Renderable(aShader, aMesh)
		{
			m_CastMesh = dynamic_cast<WindowsMesh*>(aMesh.get());
		}

		void WindowsRenderable::Prepare()
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void WindowsRenderable::Bind()
		{
			Prepare();

			m_Shader->Enable();

			//Bind the mesh
			m_CastMesh->Bind();

			for (auto& entry : m_Materials)
			{
				//Cast the texture to windows texture
				WindowsTexture& castTexture = dynamic_cast<WindowsTexture&>(*entry.Material->GetTexture());

				castTexture.Bind(entry.ShaderLocation);

				m_Shader->SetUniform(entry.ShaderName, entry.ShaderLocation);
			}

			for (auto& uniform : m_Uniforms)
			{
				uniform->Set(*m_Shader.get());
			}

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}

		void WindowsRenderable::Unbind()
		{
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			//Unbind the mesh
			m_CastMesh->Unbind();

			for (auto& entry : m_Materials)
			{
				//Cast the texture to windows texture
				WindowsTexture& castTexture = dynamic_cast<WindowsTexture&>(*entry.Material->GetTexture());
				castTexture.Unbind(entry.ShaderLocation);
			}

			m_Shader->Disable();
		}

		void WindowsRenderable::Render()
		{
			for (auto& uniform : m_RenderUniforms)
			{
				uniform->Set(*m_Shader.get());
			}

			glDrawElements(GL_TRIANGLES, m_CastMesh->GetIndiceCount(), GL_UNSIGNED_INT, (void*)0);
		}

	}
}