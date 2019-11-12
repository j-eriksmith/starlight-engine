#include "Camera.h"
#include "Renderer.h"
#include <iostream>

EscKeyPressInfo::EscKeyPressInfo(int cd, float lastFrame)
	:coolDown(cd), frameLastPressed(lastFrame), movementEnabled(true)
{
}

bool EscKeyPressInfo::CanToggleMovement(float currentFrame)
{
	return currentFrame - frameLastPressed > coolDown;
}

void EscKeyPressInfo::ToggleMovementEnabled(float curFrame)
{
	movementEnabled = !movementEnabled;
	frameLastPressed = curFrame;
}

std::shared_ptr<Camera> Camera::currentCamera = nullptr;
GLFWwindow* Camera::window = nullptr;

void Camera::CreateCameraContext(GLFWwindow* w)
{
	window = w;
}

std::shared_ptr<Camera> Camera::CreateCamera()
{
	std::shared_ptr<Camera> newCam = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	currentCamera = newCam;
	return newCam;
}

std::shared_ptr<Camera> Camera::CreateCamera(const glm::vec3& cameraPos, const glm::vec3& viewTarget, const glm::vec3& upVector)
{
	std::shared_ptr<Camera> newCam = std::make_shared<Camera>(cameraPos, viewTarget, upVector);
	currentCamera = newCam;
	return newCam;
}

Camera::Camera()
	: cameraPos(0.0f, 0.0f, 0.0f), // Default camera is positioned at the world space's origin.
	  viewTarget(0.0f, 0.0f, -1.0f), // Default camera looks down the negative z-axis (Uses right-handed coord system)
	  upVector(0.0f, 1.0f, 0.0f),
	  lastMouseX(0.0f),
	  lastMouseY(0.0f),
	  mouseOffsetX(0.0f),
	  mouseOffsetY(0.0f),
	  movementInfo(EscKeyPressInfo(1.0, glfwGetTime())),
	  pitch(0.0f),
	  yaw(-90.0f),
	  deltaTime(0.0f),
	  lastFrame(0.0f)
{
	SetInputBindings();
}

Camera::Camera(const glm::vec3& cPos, const glm::vec3& vTarget, const glm::vec3& upVec)
	: cameraPos(cPos),
	  viewTarget(vTarget),
	  upVector(upVec),
	  lastMouseX(0.0f),
	  lastMouseY(0.0f),
	  mouseOffsetX(0.0f),
	  mouseOffsetY(0.0f),
	  movementInfo(EscKeyPressInfo(1.0, glfwGetTime())),
	  pitch(0.0f),
	  yaw(-90.0f),
	  deltaTime(0.0f),
	  lastFrame(0.0f)
{
	SetInputBindings();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 lookAt(1.0);
	currentCamera->ApplyViewMatrix(lookAt);
	return lookAt;
}

void Camera::ApplyViewMatrix(glm::mat4& transform)
{
	transform = transform * glm::lookAt(cameraPos, cameraPos + viewTarget, upVector) ;
}

void Camera::Translate(const glm::vec3& translation)
{
	cameraPos += translation;
}

void Camera::Rotate(const glm::vec3& viewTargetOffset)
{
	viewTarget += viewTargetOffset;
}

// Used to capture x and y components of the user's mouse movement.
void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float sensitivity = 0.05f;
	currentCamera->mouseOffsetX = xpos - currentCamera->lastMouseX;
	currentCamera->mouseOffsetY = currentCamera->lastMouseY - ypos; // reversed since y-coords range from bottom to top
	currentCamera->lastMouseX = xpos;
	currentCamera->lastMouseY = ypos;
	currentCamera->mouseOffsetX *= sensitivity;
	currentCamera->mouseOffsetY *= sensitivity;
	if (currentCamera->movementInfo.movementEnabled)
	{

		// add new offsets to pitch and yaw values
		currentCamera->pitch += currentCamera->mouseOffsetY;
		currentCamera->yaw += currentCamera->mouseOffsetX;
		if (currentCamera->pitch > 89.0f)
		{
			currentCamera->pitch = 89.0f;
		}
		if (currentCamera->pitch < -89.0f)
		{
			currentCamera->pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(currentCamera->yaw)) * cos(glm::radians(currentCamera->pitch));
		front.y = sin(glm::radians(currentCamera->pitch));
		front.z = sin(glm::radians(currentCamera->yaw)) * cos(glm::radians(currentCamera->pitch));
		currentCamera->viewTarget = glm::normalize(front);
	}

}

void Camera::ProcessInput()
{
	if (movementInfo.CanToggleMovement(glfwGetTime()) && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		ToggleMovement();
	}
	if (movementInfo.movementEnabled)
	{
		ProcessMovement();
	}

}

void Camera::ProcessMovement()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	//float cameraSpeed = 0.05f;
	float modifier = deltaTime * 2.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += modifier * viewTarget;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= modifier * viewTarget;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(viewTarget, upVector)) * modifier;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(viewTarget, upVector)) * modifier;
	}
}

void Camera::ToggleMovement()
{
	if (movementInfo.movementEnabled)
	{
		GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
	}
	else
	{
		GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
	}
	movementInfo.ToggleMovementEnabled(glfwGetTime());
}

void Camera::SetInputBindings()
{
	// Sets the input mode for the mouse cursor. GLFW_CURSOR_DISABLED hides the cursor once it enters the 
	// window. It prevents the mouse from leaving the window, which allows for unlimited movement inside the current window.
	GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));

	// When the mouse first enters the window, our lastMouseX and lastMouseY will be set to the 
	// current coordinates of our mouse.
	// NOTE: If window does not have focus when this fn is called, the fn call will fail silently.
	GLCall(glfwSetCursorPos(window, static_cast<double>(lastMouseX), static_cast<double>(lastMouseY)));

	// Every time a mouse movement occurs, the mouse_callback function will be triggered.
	GLCall(glfwSetCursorPosCallback(window, mouse_callback));
}



