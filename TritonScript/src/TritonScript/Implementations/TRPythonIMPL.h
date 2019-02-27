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

#include "Triton\Core\Data\Structures\Mesh.h"

#define TR_PYTHON_SCRIPT_GUARD(x, onFail) \
		try\
		{\
			x;\
		}\
		catch (const std::runtime_error &re) {\
			TR_WARN(re.what());\
			onFail;\
		}\

#define TR_STRINGIZE(A, B) A ## B

namespace Triton
{
	namespace Scripting
	{
		enum TritonImport
		{
			Math = BIT(0),
			Data = BIT(1),
			Components = BIT(2),
			TritonCore = BIT(3)
		};
	}
}

namespace Triton
{
	using PythonModule = py::module;

	namespace Scripting
	{
		class TRITON_SCRIPTING_API TRPythonScriptingInterface
		{
		public:
			static void Init();
			static void Disable();
		public:
			TRPythonScriptingInterface();
			virtual ~TRPythonScriptingInterface();

			//Module control
			PythonModule py_CreateModule(const char* aModuleName);
			PythonModule py_CreateTRModule(const char* aModuleName);
			void py_ImportTritonModule(Triton::PythonModule& aModule, int aImport);
			void py_ReloadModules();

			//Variable control
			void py_ChangeVariable(std::string aVariableName, py::object aValue);
			py::object py_GetVariable(std::string aVariableName);

			//Resource control
			template <class T>
			void py_ChangeResource(std::string aVariableName, T aValue)
			{
				py::module TritonStorage = py::module::import("TritonStorage");
				TritonStorage.import("TritonData");
				py::object changeVar = TritonStorage.attr("change_resource");
				TR_PYTHON_SCRIPT_GUARD(changeVar(aVariableName, aValue));
			}
			py::object py_GetResource(std::string aVariableName);

		private:
			void py_SetupModules();
		protected:
			Triton::PythonModule prtc_py_PreExecution;
			Triton::PythonModule prtc_py_Update;
		};
	}
}