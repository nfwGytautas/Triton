#pragma once

#include "Triton\AppState\Storables.h"
#include "Triton\Entity\Registry.h"

namespace Triton
{
	namespace Storage
	{
		struct EntityWraper
		{
			Triton::ECS::Entity entity;

			EntityWraper(){ }
			EntityWraper(Triton::ECS::Entity aEntity) : entity(aEntity) { }
			virtual ~EntityWraper(){ }
		};

		struct Entity_Storable : Storable
		{
			py::object value;

			Entity_Storable(py::object aValue) : value(aValue) {}
			Entity_Storable() : value() {}
		};

		struct List_Storable : Storable
		{
			py::object value;

			List_Storable(py::object aValue) : value(aValue) {}
			List_Storable() : value() {}
		};
	}
}