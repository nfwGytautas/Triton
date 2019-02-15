#pragma once

#include "Triton\Core\Data\Structures\Mesh.h"
#include "Triton\Core\Data\Structures\Texture.h"
#include "Triton\Core\Data\Structures\Material.h"
#include "Triton\Entity\Registry.h"

namespace Triton
{
	namespace Storage
	{
		class TRITON_API Storable
		{
		public:
			virtual ~Storable() { }
		};

		//struct TRITON_API Float_Storable : Storable
		//{
		//	float value;
		//
		//	Float_Storable(float aValue) : value(aValue) {}
		//	Float_Storable() : value(0) {}
		//};
		//
		//struct TRITON_API Bool_Storable : Storable
		//{
		//	bool value;
		//
		//	Bool_Storable(bool aValue) : value(aValue) {}
		//	Bool_Storable() : value(false) {}
		//};
		//
		//struct TRITON_API Int_Storable : Storable
		//{
		//	int value;
		//
		//	Int_Storable(int aValue) : value(aValue) {}
		//	Int_Storable() : value(0) {}
		//};
		//
		//struct TRITON_API Texture_Storable : Storable
		//{
		//	std::shared_ptr<Triton::Data::Texture> value;
		//
		//	Texture_Storable(std::shared_ptr<Triton::Data::Texture> aValue) : value(aValue) {}
		//	Texture_Storable() : value() {}
		//};
		//
		//struct TRITON_API Material_Storable : Storable
		//{
		//	std::shared_ptr<Triton::Data::Material> value;
		//
		//	Material_Storable(std::shared_ptr<Triton::Data::Material> aValue) : value(aValue) {}
		//	Material_Storable() : value() {}
		//};
		//
		//struct TRITON_API Mesh_Storable : Storable
		//{
		//	std::shared_ptr<Triton::Data::Mesh> value;
		//
		//	Mesh_Storable(std::shared_ptr<Triton::Data::Mesh> aValue) : value(aValue) {}
		//	Mesh_Storable() : value() {}
		//};
	}
}