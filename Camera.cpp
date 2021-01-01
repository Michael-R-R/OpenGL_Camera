#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "Camera.h"

Camera::Camera()
{
	first_mouse_enter = true;

	last_x = 1200 / 2;
	last_y = 800 / 2;
	yaw = -90.0f;
	pitch = 0.0f;

	fov = 45.0f;
	camera_speed = 2.5;
	sensitivity = 0.1f;

	position = glm::vec3(0.0f, 0.0f, 3.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	delta_time = 0.0f;
	last_frame = 0.0f;
}

void Camera::Calculate_DeltaTime()
{
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
}

void Camera::Camera_Input(GLFWwindow* window)
{
	camera_speed = 2.5 * delta_time;

	// WASD Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += camera_speed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= camera_speed * front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= camera_speed * glm::normalize(glm::cross(front, up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += camera_speed * glm::normalize(glm::cross(front, up));
	}

	// Up and Down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position.y += camera_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position.y -= camera_speed;
	}
}