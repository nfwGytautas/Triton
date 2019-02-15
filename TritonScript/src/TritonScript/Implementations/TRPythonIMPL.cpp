#include "TRPythonIMPL.h"

Triton::Scripting::TRPythonHandler::TRPythonHandler()
{
	py::initialize_interpreter();

#ifdef TR_RELEASE
	py::module::import("triton_config").attr("setup").call(0);
#endif // TR_RELEASE

#ifdef TR_DEBUG
	py::module::import("triton_config").attr("setup").call(1);
	
#endif // TR_DEBUG

}

Triton::Scripting::TRPythonHandler::~TRPythonHandler()
{
	py::finalize_interpreter();
}

Triton::PythonModule Triton::Scripting::TRPythonHandler::CreateModule(const char* aModuleName)
{
	std::string path = std::string("User." + std::string(aModuleName));
	TR_PYTHON_SCRIPT_GUARD(return py::module::import(path.c_str()), TR_WARN("Script guard triggered"));
}

Triton::PythonModule Triton::Scripting::TRPythonHandler::CreateTRModule(const char* aModuleName)
{
	TR_PYTHON_SCRIPT_GUARD(return py::module::import(aModuleName), TR_WARN("Script guard triggered"));
}

void Triton::Scripting::TRPythonHandler::ImportTritonModule(Triton::PythonModule& aModule, int aImport)
{
	if (aImport & Triton::Scripting::TritonImport::Math)
	{
		aModule.import("TritonMath");
	}
	if (aImport & Triton::Scripting::TritonImport::Data)
	{
		aModule.import("TritonData");
	}
	if (aImport & Triton::Scripting::TritonImport::Components)
	{
		aModule.import("TritonComponents");
	}
	if (aImport & Triton::Scripting::TritonImport::TritonCore)
	{
		aModule.import("TritonCore");
	}
}

Triton::Scripting::TRPythonVariableManager::TRPythonVariableManager()
{
}

Triton::Scripting::TRPythonVariableManager::~TRPythonVariableManager()
{
}

void Triton::Scripting::TRPythonVariableManager::ChangeVariable(std::string aVariableName, py::object aValue)
{
	py::module TritonStorage = py::module::import("TritonStorage");
	py::object changeVar = TritonStorage.attr("ChangeVariable");
	TR_PYTHON_SCRIPT_GUARD(changeVar(aVariableName, aValue));
}

py::object Triton::Scripting::TRPythonVariableManager::GetVariable(std::string aVariableName)
{
	py::object TritonStorage = py::module::import("TritonStorage");
	py::object getVar = TritonStorage.attr("GetVariable");
	py::object result;
	TR_PYTHON_SCRIPT_GUARD(result = getVar(aVariableName));
	return result;
}