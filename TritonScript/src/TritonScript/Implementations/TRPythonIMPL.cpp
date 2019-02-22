#include "TRPythonIMPL.h"

Triton::Scripting::TRPythonScriptingInterface::TRPythonScriptingInterface()
{
	py::initialize_interpreter();

#ifdef TR_RELEASE
	py::module::import("triton_config").attr("setup").call(0);
#endif // TR_RELEASE

#ifdef TR_DEBUG
	py::module::import("triton_config").attr("setup").call(1);

#endif // TR_DEBUG

	prtc_py_PreExecution = py_CreateTRModule("User.Setup");
	prtc_py_Update = py_CreateTRModule("User.Update");

	py_SetupModules();
}

Triton::Scripting::TRPythonScriptingInterface::~TRPythonScriptingInterface()
{
	py::finalize_interpreter();
}

Triton::PythonModule Triton::Scripting::TRPythonScriptingInterface::py_CreateModule(const char* aModuleName)
{
	std::string path = std::string("User." + std::string(aModuleName));
	TR_PYTHON_SCRIPT_GUARD(return py::module::import(path.c_str()), TR_WARN("Script guard triggered"));
}

Triton::PythonModule Triton::Scripting::TRPythonScriptingInterface::py_CreateTRModule(const char* aModuleName)
{
	TR_PYTHON_SCRIPT_GUARD(return py::module::import(aModuleName), TR_WARN("Script guard triggered"));
}

void Triton::Scripting::TRPythonScriptingInterface::py_ImportTritonModule(Triton::PythonModule& aModule, int aImport)
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

void Triton::Scripting::TRPythonScriptingInterface::py_ReloadModules()
{
	prtc_py_PreExecution.reload();
	prtc_py_Update.reload();

	py_SetupModules();
}

void Triton::Scripting::TRPythonScriptingInterface::py_ChangeVariable(std::string aVariableName, py::object aValue)
{
	py::module TritonStorage = py::module::import("TritonStorage");
	py::object changeVar = TritonStorage.attr("ChangeVariable");
	TR_PYTHON_SCRIPT_GUARD(changeVar(aVariableName, aValue));
}

py::object Triton::Scripting::TRPythonScriptingInterface::py_GetVariable(std::string aVariableName)
{
	py::object TritonStorage = py::module::import("TritonStorage");
	py::object getVar = TritonStorage.attr("GetVariable");
	py::object result;
	TR_PYTHON_SCRIPT_GUARD(result = getVar(aVariableName));
	return result;
}

void Triton::Scripting::TRPythonScriptingInterface::py_SetupModules()
{
	py_ImportTritonModule(prtc_py_PreExecution, 
		Triton::Scripting::TritonImport::TritonCore | 
		Triton::Scripting::TritonImport::Components | 
		Triton::Scripting::TritonImport::Data | 
		Triton::Scripting::TritonImport::Math);

	py_ImportTritonModule(prtc_py_Update,
		Triton::Scripting::TritonImport::TritonCore |
		Triton::Scripting::TritonImport::Components |
		Triton::Scripting::TritonImport::Data |
		Triton::Scripting::TritonImport::Math);
}
