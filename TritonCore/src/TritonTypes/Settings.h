#pragma once

namespace Triton
{
	// Forward declarations
	namespace IO
	{
		struct Keyboard;
		struct Mouse;
	}

	/**
	 * Initialization settings for the engine
	 * Default values are provided for the default Triton engine initialization
	 *
	 * Parameters that have a prefix 'in_'  show that the parameter is checked during initialization
	 * Parameters that have a prefix 'out_' show that the parameter is set during initialization
	 */
	struct EngineSettings
	{
		/**
		 * [IN]
		 * If this is set to true then the context keyboard and mouse instances will
		 * be returned as relay_ptr.
		 * Effects:
		 *  out_keyboard, out_mouse
		 */
		bool in_customInputLoop = false;

		/**
		 * [OUT]
		 * The instance of context keyboard wrapped in a relay_ptr.
		 * Invalid if in_customInputLoop is false
		 * Effected by:
		 *  in_customInputLoop
		 */
		relay_ptr<IO::Keyboard> out_keyboard;

		/**
		 * [OUT]
		 * The instance of context mouse wrapped in a relay_ptr.
		 * Invalid if in_customInputLoop is false
		 * Effected by:
		 *  in_customInputLoop
		 */
		relay_ptr<IO::Mouse> out_mouse;
	};
}