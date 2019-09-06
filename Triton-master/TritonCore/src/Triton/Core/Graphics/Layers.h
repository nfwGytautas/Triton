#pragma once

namespace Triton
{
	namespace Layers
	{
		using Layer = unsigned short;

		// The max supported layer count in Triton
		const Layer c_maxLayers = 15;

		// The last layer that is allowed by Triton
		const Layer c_lastLayer = c_maxLayers - 1;

		// Hardware input layer, in this layer the engine
		// receives input from the hardware
		const Layer c_hardwareLayer = 0;

		// Event dispatch layer, in this layer the engine
		// dispatches all events in the buffer that were received
		// from hardware input
		const Layer c_eventDispatchLayer = 1;

		// Delta layer, in this layer the engine calculates
		// the amount of time it took for the engine to complete an 
		// update and render cycle
		const Layer c_deltaLayer = c_lastLayer;

		// Prepare render layer, in this layer the engine prepares
		// the main render buffer for a render cycle, clearing it and
		// reseting the state of the hardware
		const Layer c_prepareRenderBufferLayer = 0;

		// Execute render layer, in this layer the engine executes
		// all the commands in the render buffer
		const Layer c_executeRenderBufferLayer = c_lastLayer - 1;

		// Layer that won't be registered
		const Layer c_nullLayer = c_maxLayers + 1;

		// The layer fro script update
		const Layer c_scriptUpdateLayer = c_deltaLayer - 1;

		// Empty update layers there can be more than 8 if needed
		namespace Update
		{
			const Layer c_layer1 = c_eventDispatchLayer + 1;
			const Layer c_layer2 = c_layer1 + 1;
			const Layer c_layer3 = c_layer2 + 1;
			const Layer c_layer4 = c_layer3 + 1;
			const Layer c_layer5 = c_layer4 + 1;
			const Layer c_layer6 = c_layer5 + 1;
			const Layer c_layer7 = c_layer6 + 1;
			const Layer c_layer8 = c_layer7 + 1;
		}

		// Empty pre render layers there can be more than 8 if needed
		namespace PreRender
		{
			const Layer c_layer1 = c_prepareRenderBufferLayer + 1;
			const Layer c_layer2 = c_layer1 + 1;
			const Layer c_layer3 = c_layer2 + 1;
			const Layer c_layer4 = c_layer3 + 1;
			const Layer c_layer5 = c_layer4 + 1;
			const Layer c_layer6 = c_layer5 + 1;
			const Layer c_layer7 = c_layer6 + 1;
			const Layer c_layer8 = c_layer7 + 1;
		}

		// Empty render layers there can be more than 8 if needed
		namespace Render
		{
			const Layer c_layer1 = 0 + 1;
			const Layer c_layer2 = c_layer1 + 1;
			const Layer c_layer3 = c_layer2 + 1;
			const Layer c_layer4 = c_layer3 + 1;
			const Layer c_layer5 = c_layer4 + 1;
			const Layer c_layer6 = c_layer5 + 1;
			const Layer c_layer7 = c_layer6 + 1;
			const Layer c_layer8 = c_layer7 + 1;
		}
	}
}