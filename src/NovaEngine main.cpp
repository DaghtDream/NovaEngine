#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "Core/Scene.h"
#include "DataComponents/Camera.h"
#include "DataComponents/Mesh.h"
#include "Command/CreateObjectCommand.h"
#include "MeshFactory.h"
#include <glm.hpp>
#include "Capabilities/Render.h"
#include "Capabilities/CameraController.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 430 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	int workX, workY, workWidth, workHeight;
	glfwGetMonitorWorkarea(primaryMonitor, &workX, &workY, &workWidth, &workHeight);

    GLFWwindow* window = glfwCreateWindow(workWidth, workHeight, "OpenGL Window", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwGetWindowPos(window, &workX,&workY);
    glfwFocusWindow(window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

	float scaleFactor = 2.0f;

	ImGui::GetStyle().ScaleAllSizes(scaleFactor);

	ImFontConfig fontConfig;
	fontConfig.SizePixels = 20.0f * scaleFactor;
	io.Fonts->AddFontDefault(&fontConfig);


    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        std::cerr << "ImGui_ImplGlfw_InitForOpenGL failed!" << std::endl;
        return -1;
    }
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
    {
        std::cerr << "ImGui_ImplOpenGL3_Init failed!" << std::endl;
        return -1;
    }

    SceneManager::Scene scene;

    scene.PushCommand<SceneManager::Command::CreateObjectCommand>("CameraObject");
    scene.PushCommand<SceneManager::Command::CreateObjectCommand>("MeshObject");
    scene.FlushCommand();

    Camera camera;
    camera.IsMainCamera = true;
    scene.AddComponent<Camera>(0, camera);

    Mesh mesh = Rendering::MeshFactory::CreateRectangle();
    scene.AddComponent<Mesh>(1, mesh);
    scene.GetObject(1)->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    scene.GetObject(0)->position = glm::vec3(0.0f, 0.0f, 3.0f);

    SceneManager::Capability::Render* renderCapability = new SceneManager::Capability::Render();
    scene.AddCapability(renderCapability);

    SceneManager::Capability::CameraController* cameraControllerCapability =
        new SceneManager::Capability::CameraController(glfwGetCurrentContext());
    scene.AddCapability(cameraControllerCapability);

	SceneManager::GameObject* cameraObject = scene.GetObject(0);
    Camera* mainCamera = nullptr;
	

    scene.SceneBegin();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGuizmo::BeginFrame();

        ImGui::Begin("Debug Panel");

        float FPS = 1.0f / ImGui::GetIO().DeltaTime;
        ImGui::Text("FPS: %.1f", FPS);

		ImGui::Text("Object Count: %llu", scene.GetObjectCount());

        if (scene.GetComponent(0, mainCamera) && mainCamera != nullptr)
        {
            ImGui::SliderFloat("FOV", &mainCamera->fov, 20.0f, 120.0f);
			ImGui::DragFloat3("Camera Position", &cameraObject->position.x, 0.1f);
        }

        ImGui::End();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        

        scene.Update(0.016f);

        ImGui::Render();
        ImDrawData* draw_data = ImGui::GetDrawData();
        if (draw_data)
            ImGui_ImplOpenGL3_RenderDrawData(draw_data);

        glfwSwapBuffers(window);
    }

    scene.SceneEnd();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}