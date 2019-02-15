#pragma once

#include "TritonScript\TRMacros.h"

#define _CRT_SECURE_NO_WARNINGS
#include <pybind11\pybind11.h>
#include <pybind11\eval.h>
#include <pybind11\embed.h>
#include <pybind11\operators.h>
#include <pybind11\stl.h>
#include <pybind11\complex.h>
#include <pybind11\stl_bind.h>
namespace py = pybind11;

#include "TRPythonModule.h"

#include "Triton\Core\Data\Structures\Mesh.h"

namespace Triton
{
	using PythonModule = py::module;

	namespace Scripting
	{
		class TRITON_SCRIPTING_API TRPythonHandler
		{
		public:
			TRPythonHandler();
			virtual ~TRPythonHandler();

			PythonModule CreateModule(const char* aModuleName);
			PythonModule CreateTRModule(const char* aModuleName);
			void ImportTritonModule(Triton::PythonModule& aModule, int aImport);
		private:
			
		};

		class TRITON_SCRIPTING_API TRPythonVariableManager
		{
		public:
			TRPythonVariableManager();
			virtual ~TRPythonVariableManager();

			void ChangeVariable(std::string aVariableName, py::object aValue);
			py::object GetVariable(std::string aVariableName);

			template <class T>
			void ChangeResource(std::string aVariableName, T aValue)
			{
				py::module TritonStorage = py::module::import("TritonStorage");
				TritonStorage.import("TritonData");
				py::object changeVar = TritonStorage.attr("ChangeResource");
				TR_PYTHON_SCRIPT_GUARD(changeVar(aVariableName, aValue));
			}
			py::object GetResource(std::string aVariableName);
		private:

		};
	}
}