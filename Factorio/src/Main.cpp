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

    window = glfwCreateWindow(1024, 512, "Hello World", NULL, NULL);
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


    {

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();

        Application application(input);

        while (!glfwWindowShouldClose(window))
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            application.OnUpdate(0);
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