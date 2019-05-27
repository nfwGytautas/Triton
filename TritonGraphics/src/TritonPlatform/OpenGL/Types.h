#pragma once

#include "TritonPlatform\CrossTypes\Types.h"

// General types expanded on in platform specific way.

#include <GLFW\glfw3.h>
#include <glad\glad.h>

namespace Triton
{
	namespace PType
	{
		class GLContext;
		class GLFactory;
		class GLRenderer;

		class GLTexture : public Texture
		{
		public:
			virtual ~GLTexture() { }

			virtual void enable() override;
			virtual void disable() override;
		protected:
			// Inherited via VAO
			virtual void create(FactoryCreateParams* createParams) override;
			virtual void destroy(FactoryDestroyParams* destroyParams) override;
		private:
			unsigned int m_textureID;

			friend GLRenderer;
			friend GLFactory;
		};

		class GLVAO : public VAO
		{
		public:
			virtual ~GLVAO() { }

			virtual void enable() override;
			virtual void disable() override;
		protected:
			// Inherited via VAO
			virtual void create(FactoryCreateParams* createParams) override;
			virtual void destroy(FactoryDestroyParams* destroyParams) override;
		private:
			unsigned int m_vaoID;
			unsigned int m_indiceCount;

			unsigned int m_iboID;
			std::vector<unsigned int> m_vbos;

			friend GLRenderer;
			friend GLFactory;
		};

		class GLShader : public Shader
		{
		public:
			virtual ~GLShader() { }

			// Inherited via Shader
			virtual void setUniformInt(const std::string & aUniformName, const int & aParameter) override;
			virtual void setUniformFloat(const std::string & aUniformName, const float & aParameter) override;
			virtual void setUniformVector2(const std::string & aUniformName, const Vector2 aParameter) override;
			virtual void setUniformVector3(const std::string & aUniformName, const Vector3 aParameter) override;
			virtual void setUniformVector4(const std::string & aUniformName, const Vector4 aParameter) override;
			virtual void setUniformMatrix44(const std::string & aUniformName, const Matrix44& aParameter) override;
			virtual void enable() override;
			virtual void disable() override;

		protected:
			// Inherited via Shader
			virtual void create(FactoryCreateParams* createParams) override;
			virtual void destroy(FactoryDestroyParams* destroyParams) override;

		protected:
			int GetUniformLocation(const std::string& aName);

			GLShader() 
			{ }
		private:
			unsigned int m_shaderID;

			std::unordered_map<std::string, int> m_UniformLocationCache;

			friend GLFactory;
		};

		class GLFactory : public Factory
		{
		public:
			FactoryObject* createShader(FactoryCreateParams* createParams) override;
			FactoryObject* createVAO(FactoryCreateParams* createParams) override;
			FactoryObject* createTexture(FactoryCreateParams * createParams) override;

			void destroyObject(FactoryObject* object, FactoryDestroyParams* destroyParams) override;
		private:

			friend GLContext;
		};

		class GLWindow : public Window
		{
		public:
			// Inherited via Window
			virtual void create(unsigned int width, unsigned height) override;
			virtual std::tuple<int, int> getWindowSize() override;
			virtual bool windowClosed() override;
			virtual void update() override;
			virtual void setVsync(bool value) override;
			virtual void showCursor(bool value) override;
			virtual void clear(float r, float g, float b, float a) override;
			virtual void destroy() override;
			
		private:
			GLFWwindow* m_Window;

			friend GLContext;
		};

		class GLRenderer : public Renderer
		{
		public:
			// Inherited via Renderer
			virtual void create() override;
			virtual void render(PlatformObject* renderable) override;
			virtual void default() override;
		private:

			friend GLContext;
		};

		class GLContext : public Context
		{
		public:
			Core::EventReceiver* receiver;
		public:
			virtual ~GLContext();

			// Inherited via Context
			virtual bool init() override;
			virtual void shutdown() override;
			virtual void setContextEventCallBacks(Core::EventReceiver* receiver) override;
			virtual void update() override;
			virtual void setViewPort(int x, int y, int width, int height) override;
		};
	}
}

#include "Init.h"

#include "Window.h"

#include "Renderer.h"

#include "Shader.h"

#include "Factory.h"

#include "VAO.h"

#include "Texture.h"