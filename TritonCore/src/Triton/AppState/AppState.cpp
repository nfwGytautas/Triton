#include "TRpch.h"
#include "AppState.h"


Triton::Storage::AppState::AppState()
{
}


Triton::Storage::AppState::~AppState()
{
}

void Triton::Storage::AppState::Store(std::string aName, std::shared_ptr<Triton::Storage::Storable> aStorable)
{
	if (m_Variables.find(aName) != m_Variables.end())
	{
		throw "Variable with name already exists";
	}
	else
	{
		m_Variables[aName] = aStorable;
	}
}

std::shared_ptr<Triton::Storage::Storable> Triton::Storage::AppState::Take(std::string aName)
{
	if (m_Variables.find(aName) != m_Variables.end())
	{
		return m_Variables[aName];
	}
	else
	{
		throw "Variable with name does not exist";
	}
}

void Triton::Storage::AppState::Clear()
{
	m_Variables.clear();
}
