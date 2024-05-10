#include <stdexcept>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.h"

Window::Window(int inWidth, int inHeight, const std::string& inTitle)
{
    if (glfwInit() == 0)
	{
        throw std::runtime_error("Error glfw Init");
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 	//Contex for OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* monitor = nullptr;

	rawWindow = glfwCreateWindow(inWidth, inHeight, inTitle.c_str(), monitor, NULL);
	if (!rawWindow)
	{
        throw std::runtime_error("Error glfw creating window");
	}
	
	glfwMakeContextCurrent(rawWindow);

	if (gladLoadGL() == 0)
	{
        throw std::runtime_error("Error loading GL functions");
	}

    PrintInfo();

    deltaTime = 0.0f;
}

Window::~Window()
{
    glfwDestroyWindow(rawWindow);
}

bool Window::IsOpened() const
{
    return !glfwWindowShouldClose(rawWindow);
}

float Window::DeltaTime() const
{
    return deltaTime;
}

void Window::SetTitle(const std::string& inTitle)
{
    glfwSetWindowTitle(rawWindow, inTitle.c_str());
}

void Window::Update()
{
    static float lastTime = glfwGetTime();

	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glfwSwapBuffers(rawWindow);
	glfwPollEvents();
}

void Window::PrintInfo() const
{
    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl; 	    // GPU
	std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl; 	        // GPU group
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}