#include <glew.h>
#include <glfw3.h>

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Input.h"
#include "Application.h"

static Input input;

static void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        input.SetKeyDown(key);
    }
    if (action == GLFW_RELEASE)
    {
        input.SetKeyUp(key);
    }
}

static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    input.SetMousePosition(xpos, ypos);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cout << "glfwInit() failed.\n";
        std::cin.get();
        return -1;
    }

    //int monitorCount;
    //GLFWmonitor* monitor = glfwGetMonitors(&monitorCount)[1];
    ////GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //
    //const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //
    //glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    //glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    //glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    //glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    //
    //window = glfwCreateWindow(mode->width, mode->height, "", monitor, NULL);
    //
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    window = glfwCreateWindow(1024, 512, "", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        std::cout << "glfwCreateWindow() failed.\n";
        std::cin.get();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); 

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewInit() failed.\n";
        std::cin.get();
        return -1;
    }

    //int width = mode->width;
    //int height = mode->height;
    int width = 1024;
    int height = 512;


    {
        Application application(input, width, height);

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();

        bool running = true;
        while (running) //!glfwWindowShouldClose(window)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (!application.OnUpdate(0)) running = false;
            application.OnRender();
            application.OnImGuiRender();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}