#include "TRPythonIMPL.h"

Triton::Scripting::TRPythonHandler::TRPythonHandler()
{
	py::initialize_interpreter();

#ifdef TR_RELEASE
	std::string config = py::module::import("triton_config").attr("setup").call(0).cast<std::string>();
	py::module::import("sys").attr("path").cast<py::list>().append(config);
#endif // TR_RELEASE

#ifdef TR_DEBUG
	std::string config = py::module::import("triton_config").attr("setup").call(1).cast<std::string>();
	py::module::import("sys").attr("path").cast<py::list>().append(config);
#endif // TR_DEBUG

}

Triton::Scripting::TRPythonHandler::~TRPythonHandler()
{
	py::finalize_interpreter();
}

Triton::PythonModule Triton::Scripting::TRPythonHandler::CreateModule(const char* aModuleName)
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
	if (aImport & Triton::Scripting::TritonImport::Registry)
	{
		aModule.import("TritonEntity");
	}
	if (aImport & Triton::Scripting::TritonImport::TritonCore)
	{
		aModule.import("TritonCore");
	}
}
