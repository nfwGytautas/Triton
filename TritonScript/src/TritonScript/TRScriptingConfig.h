#pragma once

#ifdef TR_SCRIPTING_ENABLED

	#ifdef TR_SCRIPTING_LANG_PYTHON
		#include "Implementations\TRPythonIMPL.h"
		#define TR_PYTHON_SCRIPTING_HANDLE \
				std::unique_ptr<Triton::Scripting::TRPythonHandler> prtc_PyHandler = std::make_unique<Triton::Scripting::TRPythonHandler>();\
				Triton::PythonModule prtc_py_PreExecution = prtc_PyHandler->CreateModule("TRPreExecution");\
				Triton::PythonModule prtc_py_Update = prtc_PyHandler->CreateModule("TRUpdate");\
	
		#define TR_PYTHON_SCRIPTING_PRE_EXECUTION(registry, appstate) prtc_py_PreExecution.attr("entry").call(registry, appstate);
		#define TR_PYTHON_SCRIPTING_UPDATE(registry, appstate, delta) prtc_py_Update.attr("entry").call(registry, appstate, delta);
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

	#define TR_SCRIPTING_UPDATE(registry, appstate, delta) \
			TR_PYTHON_SCRIPTING_UPDATE(registry, appstate, delta)\

	#define TR_SCRIPTING_SETUP(registry, appstate) \
			prtc_PyHandler->ImportTritonModule(prtc_py_PreExecution, Triton::Scripting::TritonImport::TritonCore | Triton::Scripting::TritonImport::Registry | Triton::Scripting::TritonImport::Components | Triton::Scripting::TritonImport::Data | Triton::Scripting::TritonImport::Math);\
			prtc_PyHandler->ImportTritonModule(prtc_py_Update, Triton::Scripting::TritonImport::TritonCore | Triton::Scripting::TritonImport::Registry | Triton::Scripting::TritonImport::Components | Triton::Scripting::TritonImport::Data | Triton::Scripting::TritonImport::Math);\
			TR_PYTHON_SCRIPTING_PRE_EXECUTION(registry, appstate)\

	#define TR_SCRIPTING_RELOAD \
			TR_PYTHON_SCRIPTING_RELOAD\

#endif