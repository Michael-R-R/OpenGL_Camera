#pragma once
class Camera
{
public:
	Camera();

	// Variables
	bool first_mouse_enter;

	float last_x;
	float last_y;
	float yaw;
	float pitch;

	// Camera
	float fov;
	float camera_speed;
	float sensitivity;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	// Delta Time
	float delta_time;
	float last_frame;

	// Functions
	void Calculate_DeltaTime();
	void Camera_Input(GLFWwindow* window);


private:

};

