#pragma once

// Std. Includes
#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
	THIS IS EXTREMELY OLD CODE I HAD LYING AROUND 
	FROM ONE OF MY OLD PROJECTS, SORRY IF IT DOESNT
	FOLLOW MY GAME ENGINES CODING STYLE, I TRIED TO KEEP
	IT "UP TO" STYLE AS MUCH AS I COULD.
*/

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CAMERA
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 500.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = position;
		this->world_up = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->update_cam_vectors();
	}

	// Constructor with scalar values
	Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(pos_x, pos_y, pos_z);
		this->world_up = glm::vec3(up_x, up_y, up_z);
		this->yaw = yaw;
		this->pitch = pitch;
		this->update_cam_vectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 get_view_matrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void process_keyboard(CAMERA direction, float delta_time)
	{
		float velocity = this->movement_speed * delta_time;

		if (direction == FORWARD)
		{
			this->position += this->front * velocity;
		}

		if (direction == BACKWARD)
		{
			this->position -= this->front * velocity;
		}

		if (direction == LEFT)
		{
			this->position -= this->right * velocity;
		}

		if (direction == RIGHT)
		{
			this->position += this->right * velocity;
		}
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true)
	{
		x_offset *= this->mouse_sensitivity;
		y_offset *= this->mouse_sensitivity;

		this->yaw += x_offset;
		this->pitch += y_offset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrain_pitch)
		{
			if (this->pitch > 89.0f)
			{
				this->pitch = 89.0f;
			}

			if (this->pitch < -89.0f)
			{
				this->pitch = -89.0f;
			}
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->update_cam_vectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void process_mouse_scroll(float y_offset)
	{
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		{
			this->zoom -= y_offset;
		}

		if (this->zoom <= 1.0f)
		{
			this->zoom = 1.0f;
		}

		if (this->zoom >= 45.0f)
		{
			this->zoom = 45.0f;
		}
	}

	float get_zoom()
	{
		return this->zoom;
	}

	glm::vec3 position;
	float yaw;
	float pitch;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;

private:
	// Camera Attributes

	// Camera options
	float movement_speed;
	float mouse_sensitivity;
	float zoom;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void update_cam_vectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->right = glm::normalize(glm::cross(this->front, this->world_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};