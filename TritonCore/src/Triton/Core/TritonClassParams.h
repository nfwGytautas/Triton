#pragma once

// FILE CONTAINING ALL THE TRITON CLASS REGISTER PARAMS OBJECTS FOR EVERY
// ENGINE OBJECT THE CONTENTS OF THE FILE SHOULD NOT BE CHANGED UNLESS YOU
// KNOW WHAT YOU ARE DOING SINCE PROTECTED CLASSES SHOULDN'T BE INTERFERED WITH

#include "TritonClass.h"

namespace Triton
{
	namespace Core
	{
		const ClassRegisterParams InputRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"inputManager", // Name
			Layers::c_eventDispatchLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				ReceivedMessages::ClassRegistered | 
				ReceivedMessages::Update
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams ContextRegisterParams
		{
			false, // IsProtected
			{(size_t)TritonClasses::InputManager, (size_t)TritonClasses::AssetManager, (size_t)TritonClasses::MainRenderBuffer}, // Receivers
			"graphicsContext", // Name
			Layers::c_hardwareLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				ReceivedMessages::ClassRegistered |
				ReceivedMessages::Update
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams CacheRegisterParams
		{
			true, // IsProtected
			{(size_t)TritonClasses::AssetManager}, // Receivers
			"cache", // Name
			Layers::c_nullLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				ReceivedMessages::None
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams AssetManagerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"assetManager", // Name
			Layers::c_nullLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				ReceivedMessages::None
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams ThreadManagerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"threadManager", // Name
			Layers::c_nullLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				ReceivedMessages::None
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams MainRenderBufferRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"mainRenderBuffer", // Name
			Layers::c_nullLayer, // Update priority
			Layers::c_prepareRenderBufferLayer, // PreRender priority
			Layers::c_executeRenderBufferLayer, // Render priority
			(
				ReceivedMessages::PreRender |
				ReceivedMessages::Render |
				ReceivedMessages::ClassRegistered
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams TimerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"timer", // Name
			Layers::c_deltaLayer, // Update priority
			Layers::c_nullLayer, // PreRender priority
			Layers::c_deltaLayer, // Render priority
			(
				ReceivedMessages::Update |
				ReceivedMessages::Render |
				ReceivedMessages::ClassRegistered
			) // Received messages
		};

		const Triton::Core::ClassRegisterParams GameWindowRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"gameWindow", // Name
			Layers::c_nullLayer, // Update priority
			Layers::c_prepareRenderBufferLayer, // PreRender priority
			Layers::c_nullLayer, // Render priority
			(
				ReceivedMessages::ClassRegistered |
				ReceivedMessages::PreRender
			) // Received messages
		};
	}
}