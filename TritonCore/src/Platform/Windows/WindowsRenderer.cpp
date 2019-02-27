#include "TRpch.h"
#include "WindowsRenderer.h"

#include <glad\glad.h>
#include "WindowsModel.h"
#include "WindowsTexture.h"
#include "Triton\Logger\Log.h"

Triton::Core::Renderer* Triton::Core::Renderer::Create(Shader* aShader)
{
	WindowsShader* wShader = dynamic_cast<WindowsShader*>(aShader);
	return new Core::WindowsRenderer(wShader);
}

Triton::Core::WindowsRenderer::WindowsRenderer(WindowsShader* aShader)
	:m_Shader(aShader)
{
}

Triton::Core::WindowsRenderer::~WindowsRenderer()
{
}

void Triton::Core::WindowsRenderer::Prepare()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Triton::Core::WindowsRenderer::SetDefaultValues()
{
	
}

void Triton::Core::WindowsRenderer::Render(Data::RenderOrder& aRenderOrder)
{
	Prepare();
	for (Data::RenderBatch& batch : *aRenderOrder.Batches)
	{
		//Cast the mesh to windows mesh
		WindowsMesh& castMesh = dynamic_cast<WindowsMesh&>(*batch.Mesh);
		//Bind the mesh
		castMesh.Bind();

		//Cast the texture to windows texture
		WindowsTexture& castTexture = dynamic_cast<WindowsTexture&>(*batch.Material->GetTexture());

		castTexture.Bind(0);
		m_Shader->SetUniform("material.matTexture", 0);

		m_Shader->SetUniform("material.diffuse", batch.Material->GetDiffuse());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Render the mesh for every transformation
		for (auto transform : batch.Transformations)
		{
			m_Shader->SetUniform("transformationMatrix", *transform.get());	
			glDrawElements(GL_TRIANGLES, castMesh.GetIndiceCount(), GL_UNSIGNED_INT, (void*)0);
			//glDrawArrays(GL_TRIANGLES, 0, castMesh.GetIndiceCount());
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//Unbind the mesh
		castMesh.Unbind();
		castTexture.Unbind();
	}
}
