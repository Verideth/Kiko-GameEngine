#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream> 
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class GameManager
{
public:
	GameManager(bool is_on);
	void init();
	void render();

	glm::mat4 projection;
	glm::mat4 view;

	GLint view_loc;
	GLint model_loc;
	GLint proj_loc;
	GLFWwindow* window;
	bool is_running;
};

extern GameManager* game_manager;