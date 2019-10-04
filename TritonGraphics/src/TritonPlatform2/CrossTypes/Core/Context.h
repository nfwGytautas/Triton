#pragma once

#include "TritonTypes/reference.h"

#include "Window.h"
#include "Renderer.h"

#include "TritonPlatform2/CrossTypes/Shader.h"
#include "TritonPlatform2/CrossTypes/VAO.h"
#include "TritonPlatform2/CrossTypes/Texture.h"
#include "TritonPlatform2/CrossTypes/Bitmap.h"

#include "TritonTypes/IO.h"

#include "TritonPlatform2/CrossTypes/Core/State/Mouse.h"
#include "TritonPlatform2/CrossTypes/Core/State/Keyboard.h"

#include "TritonTypes/Settings.h"

namespace Triton
{
	namespace Graphics
	{
		/**
		 * The context used by most APIs, each API implements it's own instance of the Context
		 * the correct one can be created using static Context method create
		 */
		class Context
		{
		public:
			virtual ~Context() { }

			/**
			 * Create a context
			 *
			 * @return The API specific context cast to Context base class
			 */
			static Context* create();

			/**
			 * Create a new empty window
			 *
			 * @return New API specific window cast to Window base class
			 */
			virtual Window* newWindow() = 0;

			/**
			 * Create a new renderer from a window
			 *
			 * @param window The window to create the renderer for
			 * @return New API specific renderer cast to Renderer base class
			 */
			virtual Renderer* newRenderer(Window* window) = 0;

			/**
			 * Initialize the Context class
			 *
			 * @param settings Engine settings passed from engine init method
			 * @return Boolean that specifies if the initialization was successful
			 */
			virtual bool init(EngineSettings& settings) = 0;

			/**
			 * Shutdown the context and clean all the resources
			 */
			virtual void shutdown() = 0;

			/**
			 * Create a new shader from the create params
			 *
			 * @param createParams The parameters of the shader
			 * @return reference to the newly created shader
			 */
			virtual Shader* newShader(const IO::ShaderData& createParams) = 0;

			/**
			 * Create a new VAO from specified params
			 *
			 * @param createParams The parameters of the VAO not this is a single Mesh of MeshData not entire mesh data structure
			 * @return reference to the newly created VAO
			 */
			virtual VAO* newVAO(const IO::Mesh& createParams) = 0;

			/**
			 * Create a new texture from the specified params
			 *
			 * @param createParams The parameters of the image
			 * @return reference to the newly created texture
			 */
			virtual Texture* newTexture(const IO::ImageData& createParams) = 0;

			/**
			 * Create a new CubeTexture from the create params
			 *
			 * @param createParams The parameters of the cube texture
			 * @return reference to the newly created cube texture
			 */
			virtual CubeTexture* newCubeTexture(const IO::ImageArrayData& createParams) = 0;

			/**
			 * Create a new frame buffer
			 *
			 * @param width The width of the new frame buffer
			 * @param height The height of the new frame buffer
			 * @return reference to the newly created frame buffer
			 */
			virtual Framebuffer* newFramebuffer(unsigned int width, unsigned int height) = 0;

			/**
			 * Create a new bitmap from the create params
			 *
			 * @param createParams The parameters of the bitmap
			 * @return reference to the newly created bitmap
			 */
			virtual Bitmap* newBitmap(const BitmapCreateParams& createParams) = 0;

			/**
			 * Returns the keyboard state of the context
			 *
			 * @return keyboard state associated with current context
			 */
			IO::Keyboard& keyboard() const;

			/**
			 * Returns the mouse state of the context
			 *
			 * @return mouse state associated with current context
			 */
			IO::Mouse& mouse() const;
		protected:
			/// Variable to track if the context has been initialized or not
			bool m_initialized = false;

			/// Variable used to track the keyboard state, all windows share a single keyboard state
			IO::Keyboard* m_keyboardState;

			/// Variable used to track the mouse state, all windows share a single mouse state
			IO::Mouse* m_mouseState;
		};
	}
}