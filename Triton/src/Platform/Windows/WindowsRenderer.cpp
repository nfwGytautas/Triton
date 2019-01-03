#include "TRpch.h"
#include "WindowsRenderer.h"

#include <GL\glew.h>
#include "Triton\Entity\Components\TritonComponents.h"
#include "WindowsModel.h"
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

void Triton::Core::WindowsRenderer::Render(ECS::Registry& aRegistry)
{
	aRegistry.view<Components::Transform, Components::MeshFilter>().each([&](const auto, auto &transform, auto &meshFilter) {
		Triton::Matrix44 transformation = Triton::Core::CreateTransformationMatrix(transform.Position, transform.Rotation, transform.Scale);
		m_Shader->SetUniform("transformationMatrix", transformation);
		for (auto mesh : meshFilter.Meshes)
		{
			WindowsMesh& castMesh = dynamic_cast<WindowsMesh&>(*mesh.get());
			castMesh.Bind();
			glDrawArrays(GL_TRIANGLES, 0, castMesh.GetIndiceCount());
			castMesh.Unbind();
		}
	});
}
