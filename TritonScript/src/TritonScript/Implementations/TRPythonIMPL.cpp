#include "TRPythonIMPL.h"

Triton::Scripting::TRPythonHandler::TRPythonHandler()
	:m_ScopeGuard(), m_MainMod(py::module::import("__main__")), m_Scope(m_MainMod.attr("__dict__"))
{

}

Triton::Scripting::TRPythonHandler::~TRPythonHandler()
{

}

bool Triton::Scripting::TRPythonHandler::EvalFile(const char* aFile)
{
	py::eval_file(aFile, m_Scope);
	return true;
}