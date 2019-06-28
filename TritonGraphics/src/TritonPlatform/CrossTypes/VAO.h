#pragma once

#include "TritonPlatform/mathematical.h"
#include "Factory.h"

namespace Triton
{
	namespace PType
	{

		class VAOCreateParams : public FactoryCreateParams
		{
		public:
			struct Vertex
			{
				Vector3 Vertice;
				Vector2 UV;
				Vector3 Normal;
			};

			unsigned int ByteWidth = 0;

			std::vector<Vertex> vertices;

			std::vector<unsigned int> indices;
		};

		class VAO : public Renderable
		{
		public:
			virtual ~VAO() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual unsigned int getIndiceCount() = 0;	
		protected:
			virtual void create(FactoryCreateParams* createParams) override = 0;
		protected:
			VAO()
			{ }
		};

	}
}