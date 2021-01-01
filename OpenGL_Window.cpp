#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "Shader.h"
#include "Camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void Process_Input(GLFWwindow* window);

// Screen
int scr_width = 1200, scr_height = 800;

// Camera object
Camera camera;

int main()
{
	// Initalize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Tell glfw to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// Configure global OpenGL state
	glEnable(GL_DEPTH_TEST);

	vector<float> vertices = {
		// positions          // texture coords
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vector <glm::vec3> cube_pos =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Create Shader program, config vertices, load textures
	Shader shader;
	shader.Create("vShader.vs", "fShader.fs");
	shader.Config(vertices);
	shader.Load_Texture("car.png", "test.png");
	shader.Unbind();

	// Set texture sampler uniforms
	shader.Use(); // Use shader before setting uniforms
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// per frame time logic
		camera.Calculate_DeltaTime();

		Process_Input(window);

		glClearColor(0.8f, 0.4f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// --- Start Drawing ---
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shader.texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shader.texture2);

		// Activate shader
		shader.Use();

		// Set the projection inside render loop since fov can change with zooming
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
		// Pass projection values to shader uniform
		shader.setMatrix4fv("projection", projection);

		// Transform
		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		// Pass view values to shader uniform
		shader.setMatrix4fv("view", view);
		
		// Render 10 cubes
		glBindVertexArray(shader.VAO);
		for (int i = 0; i < cube_pos.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube_pos[i]);
			float angle = 20 * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			// Pass model values to shader uniform
			shader.setMatrix4fv("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// --- End Drawing ---

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up VAO, VBO, Program
	shader.Clean_Up();

	// Clean up
	glfwTerminate();
	return 0;
}

// Keeps window formatted for when user resizes the application
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Checks to see if this is the first time
	// the window is capturing the mouse
	// eliminates the sudden screen jump
	if (camera.first_mouse_enter)
	{
		camera.last_x = xpos;
		camera.last_y = ypos;
		camera.first_mouse_enter = false;
	}

	float x_offset = xpos - camera.last_x;
	float y_offset = camera.last_y - ypos;
	camera.last_x = xpos;
	camera.last_y = ypos;

	x_offset *= camera.sensitivity;
	y_offset *= camera.sensitivity;

	camera.yaw += x_offset;
	camera.pitch += y_offset;

	// Clamps looking up and down to 89 degrees
	if (camera.pitch > 89.0f) { camera.pitch = 89.0f; }
	if (camera.pitch < -89.0f) { camera.pitch = -89.0f; }

	// Calculate a new camera front everytime the user
	// rotates the screen with the mouse
	glm::vec3 front;
	front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	front.y = sin(glm::radians(camera.pitch));
	front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.front = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	camera.fov -= (float)y_offset;
	if (camera.fov < 1.0f) { camera.fov = 1.0f; }
	if (camera.fov > 45.0f) { camera.fov = 45.0f; }
}

// Handles keyboard input
void Process_Input(GLFWwindow* window)
{
	// Closes window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Move camera
	camera.Camera_Input(window);
}