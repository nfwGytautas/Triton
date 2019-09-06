#pragma once

#include "TritonTypes/reference.h"
#include "TritonPlatform2/CrossTypes/Core/Window.h"
#include "TritonPlatform2/CrossTypes/Base.h"
#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	namespace Graphics
	{
		/**
		 * The renderer is similar to the context in that each API has a specific way to render
		 * the renderer is created using the context and requires a valid window so that the render
		 * could know where to render to
		 */
		class Renderer
		{
		public:
			virtual ~Renderer() { }

			/**
			 * Destroys the renderer
			 */
			virtual void destroy() = 0;

			/**
			 * Starts a new frame for the renderer with specified pixel color 
			 *
			 * @param red color value for the red pixel color
			 * @param green color value for the green pixel color
			 * @param blue color value for the blue pixel color
			 * @param alpha color value for the alpha pixel color
			 */
			virtual void newFrame(float red, float green, float blue, float alpha) = 0;

			/**
			 * End the frame and flush the contents
			 */
			virtual void endFrame() = 0;

			/**
			 * Restore the renderer to it's default state
			 */
			virtual void default() = 0;

			/**
			 * Set the view port for rendering
			 *
			 * @param x start horizontal coordinate
			 * @param y start vertical coordinate
			 * @param width the width of new view port
			 * @param height the height of new view port
			 */
			virtual void setViewPort(int x, int y, int width, int height) = 0;

			/**
			 * Set depth buffer state
			 *
			 * @param state if false then the depth will not be calculate
			 */
			virtual void depthBufferState(bool state) = 0;

			/**
			 * Set cull buffer state
			 *
			 * @param state if false then the back face culling will not be active
			 */
			virtual void cullBufferState(bool state) = 0;

			/**
			 * Render a renderable
			 *
			 * @param renderable the renderable to render
			 */
			virtual void render(reference<Renderable>& renderable) = 0;

			/**
			 * Render the current bound vertex array with specified index count
			 *
			 * @param indiceCount The amount of indices to render
			 */
			virtual void render(unsigned int indiceCount) = 0;

			/**
			 * Set the renderer vertical sync variable
			 *
			 * @param value New v sync value
			 */
			void vsync(bool value);

			/**
			 * Set the renderer field of vision
			 *
			 * @param value New v sync value
			 */
			void fov(float value);

			/**
			 * Set the renderer near clipping plane
			 *
			 * @param value New v sync value
			 */
			void nearPlane(float value);

			/**
			 * Set the renderer far clipping plane
			 *
			 * @param value New v sync value
			 */
			void farPlane(float value);

			/**
			 * Get the projection matrix
			 *
			 * @return Projection matrix value
			 */
			Matrix44 projection();

			/**
			 * Get the orthographic matrix
			 *
			 * @return Orthographic matrix value
			 */
			Matrix44 orthographic();
		protected:
			/**
			 * Recalculates the matrices for the renderer
			 */
			void recalcMatrices();
		protected:
			/// Window that the renderer is rendering to
			Window* m_renderingTo;

			/// Variable used to keep track if the renderer should present the content synced up or not
			bool m_vsync = false;

			/// FOV of the renderer
			float m_fov = 3.141592654f / 4.0f;

			/// The near plane where renderer starts culling objects
			float m_nearPlane = 0.1f;

			/// The far plane where renderer starts culling objects
			float m_farPlane = 100.0f;

			/// Renderer calculated projection matrix
			Matrix44 m_projectionMatrix;
			/// Renderer calculated orthographic matrix
			Matrix44 m_orthographicMatrix;
			/// Renderer calculated world matrix
			Matrix44 m_worldMatrix;
		};
	}
}