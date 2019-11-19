#include <GLAD/include/glad.h>
#include <GLFW/include/glfw3.h>

class Window
{
private:
	double height;
	double width;
	GLFWwindow* window;

public:
	Window(int h, int w);

	~Window();

	void EndFrame();

	GLFWwindow* GetWindow();

	void PrintGLVersion();

	bool ShouldClose();


};