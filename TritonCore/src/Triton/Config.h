#pragma once

#ifndef TR_DISABLE_GUI
	//#include "GUI\GUICollection.h"
	//
	//#define TR_GUI_CONTEXT(name) ImGuiContext* name;
	//
	//#define TR_GUI_IMPLEMENTATION \
	//	std::unique_ptr<UI::GUICollection> prtc_GUIS;\
	//	private:\
	//		ImGuiContext* m_GUIContext;\
	//	protected:
#else
	#define TR_GUI_CONTEXT(name)

	#define TR_GUI_IMPLEMENTATION
#endif

//#ifndef TR_DISABLE_EDITOR_TOOLS
//	#include "Triton\Tools\GUIConsole.h"
//	#include "Triton\Tools\GUIComponentVisualizer.h"
//
//	#define TR_EDITOR_TOOLS \
//		std::shared_ptr<Tools::GUIConsole> prtc_Console;\
//		std::shared_ptr<Tools::GUIComponentVisualizer> prtc_ComponentVisualizer;
//#else
//	#define TR_EDITOR_TOOLS 
//#endif