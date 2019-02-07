#pragma once

#ifdef TR_SCRIPTING_ENABLED

	#ifdef TR_SCRIPTING_LANG_PYTHON
		#include "Implementations\TRPythonIMPL.h"
		#define TR_PYTHON_SCRIPTING_HANDLE std::unique_ptr<Triton::Scripting::TRPythonHandler> prtc_PyHandler = std::make_unique<Triton::Scripting::TRPythonHandler>();
	#else
		#define TR_PYTHON_SCRIPTING_HANDLE 
	#endif

	#define TR_INCLUDE_SCRIPTING \
			protected:\
			TR_PYTHON_SCRIPTING_HANDLE\
			private:\

#endif