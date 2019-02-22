#pragma once

#include "Implementations\TRPythonIMPL.h"

#define TR_PYTHON_SCRIPTING_PRE_EXECUTION(registry) prtc_py_PreExecution.attr("entry").call(registry);
#define TR_PYTHON_SCRIPTING_UPDATE(registry, delta) prtc_py_Update.attr("entry").call(registry, delta);

#define TR_SCRIPTING_UPDATE(registry, delta) \
			TR_PYTHON_SCRIPTING_UPDATE(registry, delta)\

#define TR_SCRIPTING_SETUP(registry) \
			prtc_PyHandler->ImportTritonModule(prtc_py_PreExecution, Triton::Scripting::TritonImport::TritonCore | Triton::Scripting::TritonImport::Components | Triton::Scripting::TritonImport::Data | Triton::Scripting::TritonImport::Math);\
			prtc_PyHandler->ImportTritonModule(prtc_py_Update, Triton::Scripting::TritonImport::TritonCore | Triton::Scripting::TritonImport::Components | Triton::Scripting::TritonImport::Data | Triton::Scripting::TritonImport::Math);\
			TR_PYTHON_SCRIPTING_PRE_EXECUTION(registry)