#include "TRpch.h"
#include "RenderChain.h"

#include <glad\glad.h>
#include "Triton\Logger\Log.h"

void Triton::RenderChain::Execute(Core::ContextState& aState)
{
	while (!m_Actions.empty()) {
		m_Actions.front()->Execute(aState);
		m_Actions.pop();
	}
}