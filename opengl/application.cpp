#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.hpp"
#include "camera.hpp"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "Mesh.h"
#include "Model.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using glm::vec3;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// window_settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool enable_mouse = false;

int main()
{
#pragma region LibInit
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
        std::cout << "error" << std::endl;
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
    const char* glsl_version = "#version 460";
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init(glsl_version);
    io.Fonts->AddFontFromFileTTF("FONTS/SIMHEI.TTF", 20);

    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);

    
    stbi_set_flip_vertically_on_load(true);
#pragma endregion
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
#pragma region ModelConfig

    std::string model_path = "nashida/nashida.obj";
    glm::vec3 ModelPosition{0.f, -0.5f, 0.f};
    glm::vec3 ModelScale(0.1f, 0.1f, -0.1f);

    // build and compile shaders
    Shader modelShader("shaders/1.model_loading.vs", "shaders/1.model_loading.frag");
    Shader lightShader("shaders/1.light_cube.vs", "shaders/1.light_cube.frag");
    
    // load models
	Model usedModel(model_path.c_str());
#pragma endregion
#pragma region LightConfig

    float vertices_light[] = {
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
    -0.5f,  0.5f, -0.5f,
    };
    
    unsigned int lightVAO;
    unsigned int lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_light), vertices_light, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::vec3 pointLightPositions[] = {
        glm::vec3(1.5f,3.f,-3.f),
        glm::vec3(-1.5f,3.f,-3.f)
    };

    PointLight pointLights[] = {
    PointLight(pointLightPositions[0],glm::vec3(0.01f,0.01f,0.01f),glm::vec3(0.1f, 0.1f, 0.1f),glm::vec3(0.2f, 0.15f, 0.15f),1.0f,0.09f,0.032f),
   
    PointLight(pointLightPositions[1],glm::vec3(0.01f,0.01f,0.01f),glm::vec3(0.1f, 0.1f, 0.1f),glm::vec3(0.2f, 0.15f, 0.15f),     1.0f,0.09f,0.032f),
    };

    SpotLight spotLight(camera.Position, camera.Front, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)),1.0f, 0.09f, 0.032f);

    DirLight dirLight(glm::vec3(0, 0.3f, -0.3f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.7f, 0.7f, 0.7f));
#pragma endregion
#pragma region shader_Set

    modelShader.use();
    modelShader.setFloat("material.shininess", 32.0f);
	modelShader.setDirLight("dirLight", dirLight);
	//modelShader.setSpotLight("spotLight", camera,spotLight);
    modelShader.setVec3("viewPos", camera.Position);
	modelShader.setPointLight("pointLights[0]", pointLights[0]);
	modelShader.setPointLight("pointLights[1]", pointLights[1]);

#pragma endregion
    //draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render
    while (!glfwWindowShouldClose(window))
    {
#pragma region PreRender
        glfwPollEvents();

        // input
        if (enable_mouse && !io.WantCaptureMouse)
        {
            processInput(window);
        }
        if (glfwGetKey(window, GLFW_KEY_PAUSE) == GLFW_PRESS)
            enable_mouse = enable_mouse ? false : true;
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#pragma endregion
#pragma region main render
        modelShader.use();
#pragma region model render

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ModelPosition); 
        model = glm::scale(model,ModelScale);	// scale down the model
        modelShader.setMat4("model", model);
        usedModel.Draw(modelShader);

#pragma endregion
#pragma region light render
        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("cubeColor", glm::vec3(0.1f, 0.1f, 0.1f));
        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 2; i++)
        {
            glm::mat4 lightModel(1.0f);
            lightModel = glm::translate(lightModel, pointLightPositions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
            lightShader.setMat4("model", lightModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
#pragma endregion
#pragma endregion
#pragma region imgui render
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);


        ImGui::Begin("Settings");  
        
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x , main_viewport->WorkPos.y), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(100, 300), ImGuiCond_FirstUseEver);
        
        ImGui::Text("Render funtion");            
        ImGui::Checkbox("Demo Window", &show_demo_window);   
		ImGui::Checkbox("Mouse Enable", &enable_mouse);
		ImGui::SliderFloat3("Model Position", &ModelPosition.x, -4.0f, 4.0f);
		ImGui::InputFloat3("Model Scale", &ModelScale.x);
		ImGui::SliderFloat3("Light Position", &pointLightPositions[0].x, -4.0f, 4.0f);
		ImGui::SliderFloat3("Light Position", &pointLightPositions[1].x, -4.0f, 4.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!enable_mouse || ImGui::GetIO().WantCaptureMouse)
	{
		firstMouse = true;
		return;
	}
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!enable_mouse || ImGui::GetIO().WantCaptureMouse)
        return;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}