// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
// #include <algorithm>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
// GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/Window.hpp>

#include "objects/Cube/Cube.hpp"
#include "objects/Sphere/Sphere.hpp"

int amount_of_columns = 10;
int amount_of_rows = amount_of_columns;
int amount_of_layers = amount_of_rows;

Window main_window;

bool distance_between_points(vec3 p1, vec3 p2)
{
	return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2);
}

void Window::secondViewPortSetup(int width, int height)
{
	int extra_viewport_side = abs(width - height);
	Viewport(width - extra_viewport_side, height - extra_viewport_side, extra_viewport_side, extra_viewport_side);
}

void Window::MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glm::vec3 player_position = glm::vec3(-1.1, -1.1, -1.1);
	glm::vec3 win_position = glm::vec3(1.05, 1.05, 1.05);
	glm::vec3 new_player_position;

	Sphere player_position_representation("objects/Sphere/PlayerSphereFragmentShader.fragmentshader");
	Sphere win_sphere("objects/Sphere/WinSphereFragmentShader.fragmentshader");

	do
	{
		int side = min(wd, ht);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		new_player_position = computeMatricesFromInputs(player_position, win());
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP_first_view = ProjectionMatrix * ViewMatrix * ModelMatrix;
		ViewportOne(0, 0, side, side);
		win_sphere.draw(MVP_first_view, win_position);

		// ==================================================================
		secondViewPortSetup(wd, ht);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		computeViewMatrices(win());
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP_second_view = ProjectionMatrix * ViewMatrix * ModelMatrix;

		player_position_representation.draw(MVP_second_view, player_position);
		win_sphere.draw(MVP_second_view, win_position);

		bool we_have_collision = false;

		if (!we_have_collision)
			player_position = new_player_position;
		// ==========================================================
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		WaitForFixedFPS(1000.0 / 60.0);
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	glfwTerminate();

	// return 0;
}
int main(int argc, char *argv[])
{
	int seed = time(NULL);

	if (argc > 3)
	{
		printf("Wrong arguments.");
		exit(EXIT_FAILURE);
	}

	if (argc < 2)
	{
		printf("Assuming seed: %d\n", seed);
	}
	else
	{
		seed = atoi(argv[1]);
		printf("Given seed: %d\n", seed);
	}

	if (argc < 3)
	{
		printf("Assuming board side length: %d\n", amount_of_columns);
		amount_of_columns = 10;
		amount_of_rows = 10;
		amount_of_layers = 10;
	}
	else
	{
		amount_of_columns = atoi(argv[2]);
		printf("Given side length: %d\n", amount_of_columns);
		amount_of_rows = amount_of_columns;
		amount_of_layers = amount_of_rows;
	}

	srand(seed);
	main_window.Init(1280, 720, "Maze 3D", 0, 33);
	main_window.MainLoop();
	return 0;
}
