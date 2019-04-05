#pragma once

#include "Triton\Core\Renderer\Renderable.h"

#include "WindowsModel.h"
#include "WindowsTexture.h"

namespace Triton
{
	namespace Core
	{
		class TRITON_API WindowsRenderable : public Renderable
		{
		public:
			WindowsRenderable(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<Data::Mesh> aMesh);
			~WindowsRenderable() { }

			// Inherited via Renderable
			virtual void Bind() override;
			virtual void Unbind() override;
			virtual void Render() override;

		private:
			virtual void Prepare() override;
		private:
			WindowsMesh* m_CastMesh;
		};
	}
}