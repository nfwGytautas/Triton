#pragma once

#include <Triton/Core/TritonClass.h>
#include <Triton/Entity/Registry.h>

namespace Triton
{
	const Core::ClassRegisterParams c_nameMapRegisterParams =
	{
		false, // IsProtected
		{}, // Receivers
		"nameMap", // Name
		Layers::c_nullLayer, // Update priority
		Layers::c_nullLayer, // PreRender priority
		Layers::c_nullLayer, // Render priority
		(
			Core::ReceivedMessages::None
		) // Received messages
	};

	// Class that maps id's to string representations
	class NameMap : public Core::TritonClass
	{
	public:
		std::unordered_map<ECS::Entity, std::string> EntityNames;
	};

}