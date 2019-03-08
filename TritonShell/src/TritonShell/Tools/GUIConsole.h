#pragma once

#include "Triton\GUI\GUI.h"

namespace Triton
{
	namespace Tools
	{

		//ImGui console implementation for TritonShell apps
		class GUIConsole : public UI::GUI
		{
		public:
			GUIConsole();
			~GUIConsole();

			void Visualize() override;

			void Update(float aDelta) override;

			virtual void ExecCommand(const char* command_line);

			void AddCommand(const char* aCommandString, std::function<void()> aFunction);
		protected:
			void ClearLog();
			virtual void AddLog(const char* fmt, ...);

		private:
			int TextEditCallback(ImGuiInputTextCallbackData* data);
		private:
			char InputBuf[256];
			std::vector<char*> Items;
			std::vector<char*> History;
			std::unordered_map<const char*, std::function<void()>> Commands;
			int HistoryPos;
			ImGuiTextFilter Filter;
			bool AutoScroll;
			bool ScrollToBottom;
		};

	}
}