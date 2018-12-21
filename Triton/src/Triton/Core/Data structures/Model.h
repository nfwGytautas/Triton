#pragma once

#include "Triton\TRMacros.h"

namespace Triton {
	namespace Core {

		class TRITON_API Mesh
		{
		public:
			virtual ~Mesh() { };

			virtual void Render() = 0;

			static Mesh* Create(std::vector<float>& aVertices, bool a3D = true);
		private:
		};

		//Class that handles the model functionality
		class TRITON_API Model
		{
		public:
			virtual ~Model() { };

			virtual void Render();
			virtual void AddMesh(Mesh* aMesh);

			virtual unsigned int GetWidth() const;
			virtual unsigned int GetHeight() const;
		private:
			std::vector<Mesh*> m_Meshes;

			unsigned int m_Width;
			unsigned int m_Height;
		};
	}
}