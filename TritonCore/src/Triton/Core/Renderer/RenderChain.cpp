#include "TRpch.h"
#include "RenderChain.h"

#include <glad\glad.h>
#include "Triton\Logger\Log.h"

void Triton::RenderChain::Execute()
{
	while (!m_Actions.empty()) {
		m_Actions.front()->Execute();
		m_Actions.pop();
	}
}