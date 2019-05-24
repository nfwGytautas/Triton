#include "GUIConsole.h"

// Portable helpers
static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

Triton::Tools::GUIConsole::GUIConsole()
{
	IsOpen = true;

	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	Commands["HELP"] = [&]() 
	{
		AddLog("Commands:");
			for (auto& it : Commands)
				AddLog("- %s", it.first); 
	};
	Commands["EXIT"] = [&]()
	{
		IsOpen = false;
	};
	AutoScroll = true;
	ScrollToBottom = true;
}

Triton::Tools::GUIConsole::~GUIConsole()
{
	ClearLog();
	for (int i = 0; i < History.size(); i++)
		free(History[i]);
}

void Triton::Tools::GUIConsole::Visualize()
{
	if (!IsOpen)
	{
		return;
	}

	ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Triton editor console", &IsOpen))
	{
		ImGui::End();
		return;
	}

	ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

	ImGui::Separator();

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		if (ImGui::Checkbox("Auto-scroll", &AutoScroll))
			if (AutoScroll)
				ScrollToBottom = true;
		ImGui::EndPopup();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	// Options, Filter
	if (ImGui::Button("Options"))
		ImGui::OpenPopup("Options");
	ImGui::SameLine();
	Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
	ImGui::Separator();

	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

	for (int i = 0; i < Items.size(); i++)
	{
		const char* item = Items[i];
		if (!Filter.PassFilter(item))
			continue;

		// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
		bool pop_color = false;
		if (strstr(item, "[error]")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
		else if (strncmp(item, "# ", 2) == 0) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true; }
		ImGui::TextUnformatted(item);
		if (pop_color)
			ImGui::PopStyleColor();
	}

	if (ScrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();


	// Command-line
	bool reclaim_focus = false;
	if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), 
		ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, 
		[](ImGuiInputTextCallbackData* data) { 
			GUIConsole* console = (GUIConsole*)data->UserData;
			return console->TextEditCallback(data);
		},
		(void*)this))
	{
		char* s = InputBuf;
		Strtrim(s);
		if (s[0])
			ExecCommand(s);
		strcpy(s, "");
		reclaim_focus = true;
	}

	// Auto-focus on window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();
}

void Triton::Tools::GUIConsole::Update(float aDelta)
{
	IO().DeltaTime = aDelta;
	if (IO().KeysDown[96])
	{
		IsOpen = true;
		IO().InputQueueCharacters.clear();
	}
}

void Triton::Tools::GUIConsole::ClearLog()
{
	for (int i = 0; i < Items.size(); i++)
		free(Items[i]);
	Items.clear();
	ScrollToBottom = true;
}

void Triton::Tools::GUIConsole::AddLog(const char* fmt, ...)
{
	// FIXME-OPT
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Items.push_back(Strdup(buf));
	if (AutoScroll)
		ScrollToBottom = true;
}

void Triton::Tools::GUIConsole::ExecCommand(const char* command_line)
{
	AddLog("# %s\n", command_line);

	// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
	HistoryPos = -1;
	for (int i = History.size() - 1; i >= 0; i--)
		if (Stricmp(History[i], command_line) == 0)
		{
			free(History[i]);
			History.erase(History.begin() + i);
			break;
		}
	History.push_back(Strdup(command_line));

	bool found = false;
	for (auto& it : Commands)
	{
		if (Stricmp(command_line, it.first) == 0)
		{
			it.second();
			found = true;
			break;
		}
	}

	if(!found)
	{
		AddLog("Unknown command: '%s'\n", command_line);
	}

	// On commad input, we scroll to bottom even if AutoScroll==false
	ScrollToBottom = true;
}

void Triton::Tools::GUIConsole::AddCommand(const char* aCommandString, std::function<void()> aFunction)
{
	if (Commands.find(aCommandString) != Commands.end())
	{
		throw "Command already exists in console";
	}

	Commands[aCommandString] = aFunction;
}

int Triton::Tools::GUIConsole::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		// Example of TEXT COMPLETION

		// Locate beginning of current word
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf)
		{
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		// Build a list of candidates
		ImVector<const char*> candidates;
		for (auto& it : Commands)
			if (Strnicmp(it.first, word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(it.first);

		if (candidates.Size == 0)
		{
			// No match
			AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
		}
		else if (candidates.Size == 1)
		{
			// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
			data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
			data->InsertChars(data->CursorPos, candidates[0]);
			data->InsertChars(data->CursorPos, " ");
		}
		else
		{
			// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
			int match_len = (int)(word_end - word_start);
			for (;;)
			{
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}

			if (match_len > 0)
			{
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
			}

			// List matches
			AddLog("Possible matches:\n");
			for (int i = 0; i < candidates.Size; i++)
				AddLog("- %s\n", candidates[i]);
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackHistory:
	{
		// Example of HISTORY
		const int prev_history_pos = HistoryPos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (HistoryPos == -1)
				HistoryPos = History.size() - 1;
			else if (HistoryPos > 0)
				HistoryPos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (HistoryPos != -1)
				if (++HistoryPos >= History.size())
					HistoryPos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != HistoryPos)
		{
			const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str);
		}
	}
	}
	return 0;
}