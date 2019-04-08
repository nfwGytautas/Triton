#pragma once

#ifndef TR_DISABLE_EDITOR_TOOLS
	#include "TritonShell\Tools\GUIConsole.h"
	#include "TritonShell\Tools\GUIComponentVisualizer.h"

	#define TR_EDITOR_TOOLS \
		std::shared_ptr<Tools::GUIConsole> prtc_Console;\
		std::shared_ptr<Tools::GUIComponentVisualizer> prtc_ComponentVisualizer;
#else
	#define TR_EDITOR_TOOLS 
#endif