#include <Triton.h>
#include <string>

bool keyReleased(const Triton::Event& event)
{
	const Triton::KeyReleasedEvent& kpe = dynamic_cast<const Triton::KeyReleasedEvent&>(event);
	TR_TRACE("INSIDE KEY RELEASE {0}", kpe.ToString());
	return true;
}

class Sandbox : public Triton::Application , private Triton::EventListener
{
public:
	Sandbox()
	{
		this->Listen<Triton::KeyPressedEvent>(Triton::EventBehavior(std::bind(&Sandbox::keyPressed, this, std::placeholders::_1)));
		this->Listen<Triton::KeyReleasedEvent>(Triton::EventBehavior(keyReleased));
	}

	~Sandbox()
	{
		delete Cube;
		delete camera;
	}

	void PreExecutionSetup() override
	{
		camera = new Triton::Camera(Triton::Vector3(0.0f, 0.0f, 3.0f), Triton::Vector3(0.0f, 0.0f, 0.0f));		

		Cube = new Triton::Core::Model();	
		CubeMesh = Triton::Core::Mesh::Create(CubeVertices);
		//CubeMesh = Triton::Core::Mesh::Create(TestVertices, false);
		Cube->AddMesh(CubeMesh);

		Triton::Matrix44 transformation = Triton::Core::CreateTransformationMatrix(Triton::Vector3(0, 0, 0), Triton::Vector3(0, 0, 0), Triton::Vector3(1, 1, 1));
		prtc_Shader->SetUniform("transformationMatrix", transformation);
	}

	void OnUpdate() override
	{
		camera->OnUpdate();
		prtc_Shader->SetUniform("viewMatrix", camera->ViewMatrix());
		Cube->Render();
	}

private:
	bool keyPressed(const Triton::Event& event)
	{
		const Triton::KeyPressedEvent& kpe = dynamic_cast<const Triton::KeyPressedEvent&>(event);
		TR_TRACE("INSIDE KEY PRESS {0}", kpe.ToString());
		float cameraSpeed = 0.1f; // adjust accordingly
		int keycode = kpe.GetKeyCode();

		if (kpe.GetKeyCode() == (int)'W')
		{
			camera->Position += cameraSpeed * camera->ViewDirection;
		}
		if (kpe.GetKeyCode() == (int)'S')
		{
			camera->Position -= cameraSpeed * camera->ViewDirection;
		}
		if (kpe.GetKeyCode() == (int)'A')
		{
			camera->Position -= glm::normalize(glm::cross(camera->ViewDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		}
		if (kpe.GetKeyCode() == (int)'D')
		{
			camera->Position += glm::normalize(glm::cross(camera->ViewDirection, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
		}

		TR_INFO("{0};{1};{2}", camera->Position.x, camera->Position.y, camera->Position.z);
		return true;
	}
private:
	Triton::Camera* camera;

	std::vector<float> CubeVertices = {
			 -0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f, 
			 0.5f,  0.5f, -0.5f, 
			 0.5f,  0.5f, -0.5f, 
			-0.5f,  0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f, 

			-0.5f, -0.5f,  0.5f, 
			 0.5f, -0.5f,  0.5f, 
			 0.5f,  0.5f,  0.5f, 
			 0.5f,  0.5f,  0.5f, 
			-0.5f,  0.5f,  0.5f, 
			-0.5f, -0.5f,  0.5f, 

			-0.5f,  0.5f,  0.5f, 
			-0.5f,  0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f, 
			-0.5f, -0.5f, -0.5f, 
			-0.5f, -0.5f,  0.5f, 
			-0.5f,  0.5f,  0.5f, 

			 0.5f,  0.5f,  0.5f, 
			 0.5f,  0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f,  0.5f, 
			 0.5f,  0.5f,  0.5f, 

			-0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f,  0.5f, 
			 0.5f, -0.5f,  0.5f, 
			-0.5f, -0.5f,  0.5f, 
			-0.5f, -0.5f, -0.5f, 

			-0.5f,  0.5f, -0.5f, 
			 0.5f,  0.5f, -0.5f, 
			 0.5f,  0.5f,  0.5f, 
			 0.5f,  0.5f,  0.5f, 
			-0.5f,  0.5f,  0.5f, 
			-0.5f,  0.5f, -0.5f
	};

	Triton::Core::Model* Cube;
	Triton::Core::Mesh* CubeMesh;
};

Triton::Application* Triton::CreateApplication()
{
	return new Sandbox();
}