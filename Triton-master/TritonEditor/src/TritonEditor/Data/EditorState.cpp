#include "EditorState.h"

namespace Triton
{
	void EditorState::onMessage(size_t message, void* payload)
	{
		switch (message)
		{
		case (size_t)Triton::Core::TritonMessageType::ClassRegistered:
		{
			return;
		}
		}
	}
}