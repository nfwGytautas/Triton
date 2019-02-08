#pragma once

#include "TRMacros.h"
#include "TRPythonModule.h"

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
			void ImportTritonModule(Triton::PythonModule& aModule, int aImport);
		private:
			
		};
	}
}