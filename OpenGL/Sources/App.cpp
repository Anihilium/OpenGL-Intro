#include <App.hpp>

namespace Core
{
	App::~App()
	{
		mManager.Clear();
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	bool App::Init(AppInitializer& init)
	{
		mWidth = init.width; mHeight = init.height;
		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, init.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, init.minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		// glfw Window creation
		// --------------------
		Window = glfwCreateWindow(init.width, init.height, init.name, NULL, NULL);
		if (Window == NULL)
		{
			std::cout << "Failed to create GLFW Window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(Window);
		glfwSetFramebufferSizeCallback(Window, init.framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}


		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(init.glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glfwGetWindowSize(Window, &mWidth, &mHeight);
		mCam.Aspect = mWidth / mHeight;

		return true;
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void App::processInput(GLFWwindow* Window)
	{
		if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(Window, true);

		if(glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			mInWindow = true;
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			mInWindow = false;
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}


		double newMouseX, newMouseY;
		float deltaX, deltaY;
		glfwGetCursorPos(Window, &newMouseX, &newMouseY);
		if (mFirstMouse)
		{
			mMouseX = newMouseX;
			mMouseY = newMouseY;
			mFirstMouse = false;
		}
		deltaX = (float)(newMouseX - mMouseX);
		deltaY = (float)(mMouseY - newMouseY);
		mMouseX = newMouseX;
		mMouseY = newMouseY;

		if (!mInWindow)
			return;

		mCam.CamInputs.deltaX = deltaX;
		mCam.CamInputs.deltaY = deltaY;
		mCam.CamInputs.moveForward = glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS;
		mCam.CamInputs.moveBackward = glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS;
		mCam.CamInputs.moveDown = glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
		mCam.CamInputs.moveUp = glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS;
		mCam.CamInputs.moveLeft = glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS;
		mCam.CamInputs.moveRight = glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS;
	}

	void App::InitResources()
	{
		Resources::Shader* shader = mManager.Create<Resources::Shader>("shader", "");
		ASSERT(shader->SetVertexShader("Resources/Shaders/shader.vert"), "Could not load the vertex shader");
		ASSERT(shader->SetFragmentShader("Resources/Shaders/shader.frag"), "Could not load the fragment shader");
		ASSERT(shader->Link(), "Error while linking the shaders");
		mShaderProgram = shader->GetShaderProgram();

		Resources::Texture* catTex = mManager.Create<Resources::Texture>("cat", "Resources/Textures/cat.png");
		Resources::Texture* dogTex = mManager.Create<Resources::Texture>("dog", "Resources/Textures/dog.png");
		Resources::Texture* wallTex = mManager.Create<Resources::Texture>("wall", "Resources/Textures/wall.jpg");
		Resources::Texture* potaTex = mManager.Create<Resources::Texture>("potato", "Resources/Textures/potatOS.png");
		Resources::Texture* chocoTex = mManager.Create<Resources::Texture>("chocoboTex", "Resources/Textures/chocobo.png");
		Resources::Texture* gordoTex = mManager.Create<Resources::Texture>("gordoTex", "Resources/Textures/Gordo.png");
		Resources::Texture* ferdiTex = mManager.Create<Resources::Texture>("ferdiTex", "Resources/Textures/ferdinand.png");

		auto start = std::chrono::steady_clock::now();

		Resources::Model* cube = mManager.Create<Resources::Model>("cube", "Resources/Obj/cube.obj");
		Resources::Model* potatOS = mManager.Create<Resources::Model>("potatos", "Resources/Obj/potatOS.obj");
		Resources::Model* chocobo = mManager.Create<Resources::Model>("chocobo", "Resources/Obj/chocobo.obj");
		Resources::Model* gordo = mManager.Create<Resources::Model>("gordo", "Resources/Obj/Gordo.obj");
		Resources::Model* ferdinand = mManager.Create<Resources::Model>("ferdinand", "Resources/Obj/Ferdinand.obj");

		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << "ms\n";

		float sizeMult = 0.01f;

		mGraph.RootChildren.push_back({ DataStructure::GameObject(Physics::Transform(Vec3(-3.5f, 0.f, -6.f), Vec3(sizeMult, sizeMult, sizeMult)), LowRenderer::Mesh(chocobo, mShaderProgram, chocoTex)) });
		mGraph.RootChildren.push_back({ DataStructure::GameObject(Physics::Transform(Vec3(0.f, 0.f, -6.f), Vec3(0.1f, 0.1f, 0.1f)), LowRenderer::Mesh(potatOS, mShaderProgram, potaTex)) });
		mGraph.RootChildren.push_back({ DataStructure::GameObject(Physics::Transform(Vec3(5.5f, 0.f, -6.f), Vec3(1.f, 1.f, 1.f)), LowRenderer::Mesh(cube, mShaderProgram, catTex)) });
		mGraph.RootChildren.push_back({ DataStructure::GameObject(Physics::Transform(Vec3(), Vec3(0.1f, 0.1f, 0.1f)), LowRenderer::Mesh(cube, mShaderProgram, dogTex)) });
		mGraph.RootChildren.push_back({ DataStructure::GameObject(Physics::Transform(Vec3(0.f, 0.f, -15.f), Vec3(5.f, 8.f, 1.f), Vec3(0.f, 0.f, 0.f)), LowRenderer::Mesh(cube, mShaderProgram, wallTex))});

		DataStructure::SceneNode dogCube{ DataStructure::GameObject(Physics::Transform(Vec3(0.f, -3.f * 1.f / sizeMult, 0.f), Vec3(0.5f / sizeMult, 0.5f / sizeMult, 0.5f / sizeMult)), LowRenderer::Mesh(cube, mShaderProgram, dogTex)) };
		dogCube.Parent = &mGraph.RootChildren[0];
		mGraph.RootChildren[0].Children.push_back(dogCube);

		DataStructure::SceneNode gordoChild{ DataStructure::GameObject(Physics::Transform(Vec3(3.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f)), LowRenderer::Mesh(gordo, mShaderProgram, gordoTex)) };
		gordoChild.Parent = &mGraph.RootChildren[2];
		mGraph.RootChildren[2].Children.push_back(gordoChild);

		DataStructure::SceneNode ferdnand{ DataStructure::GameObject(Physics::Transform(Vec3(-5.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f)), LowRenderer::Mesh(ferdinand, mShaderProgram, ferdiTex)) };
		ferdnand.Parent = &mGraph.RootChildren[2].Children[0];
		mGraph.RootChildren[2].Children[0].Children.push_back(ferdnand);

		mDirectionalLights.push_back(LowRenderer::DirectionalLight(Vec3(-1.f, -1.f, -1.f)));
		mPointLights.push_back(LowRenderer::PointLight());
		mSpotLights.push_back(LowRenderer::SpotLight(Vec3(), Vec3(), cosf(20.f * M_PI / 180.f), cosf(40.f * M_PI / 180.f), Vec4(1.f, 1.f, 1.f), Vec4(1.f, 1.f, 1.f), Vec4(1.f, 1.f, 1.f), Vec3(1.f, 0.045f, 0.0075f)));
	}

	void App::Update()
	{
		float currentFrame = glfwGetTime();
		mDeltaTime = currentFrame - mLastFrame;
		mLastFrame = currentFrame;
		// input
		// -----
		glfwPollEvents();
		processInput(Window);

		glfwGetWindowSize(Window, &mWidth, &mHeight);
		mCam.Aspect = (float)mWidth / (float)mHeight;

		mCam.Update(mDeltaTime);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static float rot = 0.f;
		rot += 50.f * mDeltaTime;
		mGraph.RootChildren[0].Object.Transform.Rotation = Vec3(rot, 0.f, 0.f);
		mGraph.RootChildren[1].Object.Transform.Rotation = Vec3(0.f, rot, 0.f);
		mGraph.RootChildren[2].Object.Transform.Rotation = Vec3(0.f, 0.f, rot);
		mGraph.RootChildren[0].Children[0].Object.Transform.Rotation = Vec3(0.f, rot, rot);

		Vec3 lightPos = Vec3(0.f, 0.f, -6) + Vec3(cosf(rot/2.f * M_PI / 180.f), 0.f, sinf(rot/2.f * M_PI / 180.f)) * 8.f;
		mGraph.RootChildren[3].Object.Transform.Position = lightPos;

		mGraph.RootChildren[2].Children[0].Object.Transform.Position = Vec3(0.f, 3.f, 2.f * cosf(rot * M_PI / 180.f));

		mGraph.RootChildren[2].Children[0].Children[0].Object.Transform.Rotation = Vec3(0.f, rot, 0.f);

		mPointLights[0].Position = lightPos;
		mSpotLights[0].Position = mCam.Position;
		mSpotLights[0].Direction = mCam.Front;

		mGraph.Update(mCam, mDirectionalLights, mPointLights, mSpotLights);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(Window);
	}
}