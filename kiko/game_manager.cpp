#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>
#include "game_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "cam.h"
#include "shader.h"
#include "map.h"

int32_t WINDOW_WIDTH = 800;
int32_t WINDOW_HEIGHT = 600;

KikoBSP map;

Shader core_shader;
float last_x = WINDOW_WIDTH / 2.0;
float last_y = WINDOW_HEIGHT / 2.0;

bool first_mouse = true;
Camera camera = Camera(glm::vec3(0.0, 0.0, 0.0));

float delta_time = 0.0f;
float last_frame = 0.0f;

bool keys[1024];

/* Moves/alters the camera positions based on user input */
void do_movement()
{	
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.process_keyboard(CAMERA::FORWARD, delta_time);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.process_keyboard(CAMERA::BACKWARD, delta_time);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.process_keyboard(CAMERA::LEFT, delta_time);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.process_keyboard(CAMERA::RIGHT, delta_time);
	}
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}

	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;

	last_x = x_pos;
	last_y = y_pos;

	camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	camera.process_mouse_scroll(y_offset);
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

GameManager::GameManager(bool is_on)
{
	this->is_running = is_on;

	if (is_on)
	{
		std::printf("GAME IS RUNNING!\n");
	}
}

void GameManager::init()
{
	glfwInit();

	this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kiko", nullptr, nullptr);

	if (this->window)
	{
		glfwMakeContextCurrent(this->window);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(this->window, key_callback);
		glfwSetCursorPosCallback(this->window, mouse_callback);
		glfwSetScrollCallback(this->window, scroll_callback);
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwGetFramebufferSize(this->window, &WINDOW_WIDTH, &WINDOW_HEIGHT);

		glewExperimental = GL_TRUE;
		glewInit();

		if (glewInit() != GLEW_OK)
		{
			std::printf("ERROR: GLEW COULDN'T INITIALIZE!\n");
		}

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_MULTISAMPLE);

		core_shader.load_shader("shaders/core.vs", "shaders/core.fs");	

		map.load_map("maps/spirit3t3.bsp");

		std::printf("FINISHED INIT!\n");
	}
	else
	{
		std::printf("ERROR: WINDOW FAILED TO CREATE!\n");
	}

	if (!glfwInit())
	{
		std::printf("ERROR: COULDN'T INITIALIZE GLFW3\n");
	}
}

void GameManager::render()
{
	while (!glfwWindowShouldClose(this->window) && this->is_running == true)
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		glClearColor(0.1, 0.6, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
		
		glLoadIdentity();                 // Reset the model-view matrix

		glfwPollEvents();
		do_movement();

		core_shader.use();
		
		this->projection = glm::perspective<float>(camera.get_zoom(), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 1000.0);
		this->view = camera.get_view_matrix();
		this->model_loc = glGetUniformLocation(core_shader.program, "model");
		this->view_loc = glGetUniformLocation(core_shader.program, "view");
		this->proj_loc = glGetUniformLocation(core_shader.program, "projection");

		glUniformMatrix4fv(this->view_loc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(this->proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
		
		map.shader.use();
		map.render(glm::vec3(0.0, 0.0, 0.0));

		glfwSwapBuffers(this->window);
	}

	map.cleanup_map();

	glfwTerminate();
}