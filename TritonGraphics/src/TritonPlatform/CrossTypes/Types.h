#pragma once

#include "Base.h"
#include "Factory.h"
#include "Shader.h"
#include "VAO.h"
#include "Texture.h"
#include "Bitmap.h"

namespace Triton
{
	namespace Core
	{
		// Forward declaration of the InputManager to speed up build times
		class InputManager;
	}

	namespace PType
	{
		class Window
		{
		public:
			virtual ~Window() { }

			// Create the window
			virtual void create(unsigned int width, unsigned height) = 0;

			// Get window size
			virtual std::tuple<int, int> getWindowSize() = 0;

			// Check if the window is closed
			virtual bool windowClosed() = 0;

			// Update the window
			virtual void update() = 0;

			// Set window vsync property
			virtual void setVsync(bool value) = 0;

			// Set cursor visability
			virtual void showCursor(bool value) = 0;

			// Clear the contents of the window
			virtual void clear(float r, float g, float b, float a) = 0;

			// Destroy the window
			virtual void destroy() = 0;
		};

		class Renderer
		{
		public:
			virtual ~Renderer() { }

			// Create the renderer
			virtual void create() = 0;

			// Start new frame
			virtual void newFrame(float red, float green, float blue, float alpha) = 0;

			// End frame
			virtual void endFrame() = 0;

			// Render object
			virtual void render(reference<Renderable>& renderable) = 0;

			// Set renderer values to default ones
			virtual void default() = 0;

			// Returns a projection matrix from the values the renderer has
			virtual Matrix44 projection() = 0;

			// Returns orthographic matrix for use in 2D rendering
			virtual Matrix44 orthographic() = 0;

			// Recalculates the matrices
			virtual void recalc_matrices() = 0;
		public:
			float fov;
			float nearPlane;
			float farPlane;
		protected:
			unsigned int m_width;
			unsigned int m_height;
		};

		class Context
		{
		public:
			virtual ~Context() { }

			// Window object
			Window* window;
			// Renderer object
			Renderer* renderer;
			// Factoy object
			Factory* factory;

			// Init the context
			virtual bool init() = 0;

			// Additional init if required mostly used by DirectX
			virtual bool init_additional() { return true; }

			// Shutdown context and free resources
			virtual void shutdown() = 0;

			// Set event callbacks
			virtual void setContextEventCallBacks(Core::InputManager* iManager) = 0;

			// Update the context
			virtual void update() = 0;

			// Update viewport
			virtual void setViewPort(int x, int y, int width, int height) = 0;

			// Fill's packet data
			virtual void fillPacket(PlatformObject* packet) = 0;

			// Sets depth buffer state
			virtual void depthBufferState(bool state) = 0;

			// Sets the culling state false for disabled
			virtual void cullBufferState(bool state) = 0;

			// Sets cursor position to specified points
			virtual void setCursorPos(double x, double y) = 0;
		};
	}
}