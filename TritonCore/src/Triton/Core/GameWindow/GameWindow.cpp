#include <TRpch.h>
#include "GameWindow.h"

#include "Triton/Managers/AssetManager.h"

namespace Triton
{
	void GameWindow::onMessage(size_t message, void* payload)
	{
		switch (message)
		{
		case (size_t)Core::TritonMessageType::ClassRegistered:
			auto& assetManager = this->getClassByID((size_t)Core::TritonClasses::AssetManager).as<Manager::AssetManager>();

			// Asset creation description
			Triton::Resource::AssetCreateParams asset_desc;
			asset_desc.Type = Triton::Resource::AssetCreateParams::AssetType::VIEWPORT;
			asset_desc.Width = 1280;
			asset_desc.Height = 720;

			m_gameViewport = assetManager->createAsset(asset_desc).as<Triton::Data::Viewport>();

			return;
		}
	}
}