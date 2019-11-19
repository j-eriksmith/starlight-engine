#include "Window.h"
#include "Renderer.h"
#include <iostream>

Window::Window(int h, int w)
	: height(h),
	  width(w),
	  window(nullptr)
{
	/* Initialize the library */
	if (!glfwInit())
	{
		std::cerr << "Window::Window -- GLFW Library failed to initialize." << std::endl;
	}
	window = glfwCreateWindow(height, width, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cerr << "Window::Window -- GLFW window failed to initialize." << std::endl;
		glfwTerminate();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// sets the current opengl profile to be the 'core' profile. Default is the compatibility profile, which is the latest version
	// you have in your graphics drivers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	// TODO: Better understand what glViewport does
	glViewport(0, 0, width, height);

	//Enable Blending
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	// Enable z-buffer depth testing
	glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
	glfwTerminate();
}

void Window::EndFrame()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}

GLFWwindow* Window::GetWindow()
{
	return window;
}

void Window::PrintGLVersion()
{
	std::cout << glGetString(GL_VERSION) << std::endl;
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}