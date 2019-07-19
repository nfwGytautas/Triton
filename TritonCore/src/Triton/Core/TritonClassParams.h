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
			"inputManager"
		};

		const Triton::Core::ClassRegisterParams ContextRegisterParams
		{
			false, // IsProtected
			{(size_t)TritonClasses::InputManager, (size_t)TritonClasses::AssetManager, (size_t)TritonClasses::MainRenderBuffer}, // Receivers
			"graphicsContext" // Name
		};

		const Triton::Core::ClassRegisterParams CacheRegisterParams
		{
			true, // IsProtected
			{(size_t)TritonClasses::AssetManager}, // Receivers
			"cache" // Name
		};

		const Triton::Core::ClassRegisterParams AssetManagerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"assetManager" // Name
		};

		const Triton::Core::ClassRegisterParams ThreadManagerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"threadManager" // Name
		};

		const Triton::Core::ClassRegisterParams MainRenderBufferRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"mainRenderBuffer" // Name
		};

		const Triton::Core::ClassRegisterParams TimerRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"timer" // Name
		};

		const Triton::Core::ClassRegisterParams GameWindowRegisterParams
		{
			false, // IsProtected
			{}, // Receivers
			"gameWindow" // Name
		};
	}
}