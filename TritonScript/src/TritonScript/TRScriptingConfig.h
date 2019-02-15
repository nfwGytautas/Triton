#pragma once

#ifdef TR_SCRIPTING_ENABLED

	#ifdef TR_SCRIPTING_LANG_PYTHON
		#include "Implementations\TRPythonIMPL.h"
		#define TR_PYTHON_SCRIPTING_HANDLE \
				std::unique_ptr<Triton::Scripting::TRPythonHandler> prtc_PyHandler = std::make_unique<Triton::Scripting::TRPythonHandler>();\
				std::unique_ptr<Triton::Scripting::TRPythonVariableManager> prtc_PyVariableHandler = std::make_unique<Triton::Scripting::TRPythonVariableManager>();\
				Triton::PythonModule prtc_py_PreExecution = prtc_PyHandler->CreateTRModule("TRPreExecution");\
				Triton::PythonModule prtc_py_Update = prtc_PyHandler->CreateTRModule("TRUpdate");\
	
		#define TR_PYTHON_SCRIPTING_PRE_EXECUTION(registry) prtc_py_PreExecution.attr("entry").call(registry);
		#define TR_PYTHON_SCRIPTING_UPDATE(registry, delta) prtc_py_Update.attr("entry").call(registry, delta);
		#define TR_PYTHON_SCRIPTING_RELOAD \
				prtc_py_PreExecution.reload();\
				prtc_py_Update.reload();\
		
	#else
		#define TR_PYTHON_SCRIPTING_HANDLE 
		#define TR_PYTHON_SCRIPTING_PRE_EXECUTION
		#define TR_PYTHON_SCRIPTING_UPDATE
		#define TR_PYTHON_SCRIPTING_RELOAD 
	#endif

	#define TR_INCLUDE_SCRIPTING \
			protected:\
			TR_PYTHON_SCRIPTING_HANDLE\
			private:\

	#define TR_SCRIPTING_UPDATE(registry, delta) \
			TR_PYTHON_SCRIPTING_UPDATE(registry, delta)\

	#define TR_SCRIPTING_SETUP(registry) \
			prtc_PyHandler->ImportTritonModule(prtc_py_PreExecution, Triton::Scripting::TritonImport::TritonCore | Triton::Scripting::TritonImport::Components | Triton::Scripting::TritonImport::Data | Triton::Scripting::TritonImport::Math);\
			prtc_PyHandler->ImportTritonModule(prtc_py_Update, Triton::Scripting::TritonImport::TritonCore | Triton::Scripting::TritonImport::Components | Triton::Scripting::TritonImport::Data | Triton::Scripting::TritonImport::Math);\
			TR_PYTHON_SCRIPTING_PRE_EXECUTION(registry)\

	#define TR_SCRIPTING_RELOAD \
			TR_PYTHON_SCRIPTING_RELOAD\

#endif