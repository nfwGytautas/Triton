#include "TRpch.h"
#include "GUICollection.h"

#ifndef TR_DISABLE_GUI

Triton::UI::GUICollection::GUICollection()
{
}

Triton::UI::GUICollection::GUICollection(size_t aSize)
	: mGUIS(aSize)
{
}

Triton::UI::GUICollection::~GUICollection()
{
}

void Triton::UI::GUICollection::RefreshCollection()
{
	for (auto& gui : mGUIS)
	{
		gui->Refresh();
	}
}

void Triton::UI::GUICollection::DrawCollection()
{
	for (auto& gui : mGUIS)
	{
		gui->Draw();
	}
}

void Triton::UI::GUICollection::VisualizeCollection()
{
	for (auto& gui : mGUIS)
	{
		gui->Visualize();
	}
}

void Triton::UI::GUICollection::UpdateCollection(float aDelta)
{
	for (auto& gui : mGUIS)
	{
		gui->Update(aDelta);
	}
}

void Triton::UI::GUICollection::AddGUI(std::shared_ptr<GUI> aGUI)
{
	mGUIS.push_back(aGUI);
}

#endif