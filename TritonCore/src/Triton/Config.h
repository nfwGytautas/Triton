#pragma once

#ifndef TR_DISABLE_GUI
	#include "GUI\GUICollection.h"

	#define TR_GUI_CONTEXT(name) ImGuiContext* name;

	#define TR_GUI_IMPLEMENTATION \
		std::unique_ptr<UI::GUICollection> prtc_GUIS;\
		private:\
			ImGuiContext* m_GUIContext;\
		protected:
#else
	#define TR_GUI_CONTEXT(name)

	#define TR_GUI_IMPLEMENTATION
#endif

#define TR_DEFAULT_SHADER prtc_Shader