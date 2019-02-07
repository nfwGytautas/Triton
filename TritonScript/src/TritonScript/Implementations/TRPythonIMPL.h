#pragma once

#include "TRMacros.h"

#define _CRT_SECURE_NO_WARNINGS
#include <pybind11\pybind11.h>
#include <pybind11\eval.h>
#include <pybind11\embed.h>
namespace py = pybind11;

#include "Triton\Core\Data\Structures\Material.h"

PYBIND11_EMBEDDED_MODULE(Vector3, m) {
	py::class_<Triton::Vector3, std::shared_ptr<Triton::Vector3>>(m, "Vector3")
		.def(py::init<float, float, float>())
		.def_readwrite("x", &Triton::Vector3::x)
		.def_readwrite("y", &Triton::Vector3::y)
		.def_readwrite("z", &Triton::Vector3::z);
}

PYBIND11_EMBEDDED_MODULE(Material, m) {
	py::class_<Triton::Data::Material, std::shared_ptr<Triton::Data::Material>>(m, "Material")
		.def(py::init<std::shared_ptr<Triton::Data::Texture>>())
		.def(py::init<>())
		.def("SetDiffuse", &Triton::Data::Material::SetDiffuse)
		.def("GetDiffuse", &Triton::Data::Material::GetDiffuse)
		.def("GetTexture", &Triton::Data::Material::GetTexture);
}

namespace Triton
{
	namespace Scripting
	{
		class TRITON_SCRIPTING_API TRPythonHandler
		{
		public:
			TRPythonHandler();
			virtual ~TRPythonHandler();

			bool EvalFile(const char* aFile);

			py::module MainModule()
			{
				return m_MainMod;
			}
		private:
			py::scoped_interpreter m_ScopeGuard;
			py::module m_MainMod;
			py::object m_Scope;
		};
	}
}