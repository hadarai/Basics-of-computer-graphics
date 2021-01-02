// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/Window.hpp>

#include "objects/Sphere/Sphere.hpp"

Window main_window;

// int LastUsedParticle = 0;

void Window::MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 player_position = glm::vec3(-2.0, 1.0, 0.0);
	// glm::vec3 win_position = glm::vec3(1.05, 1.05, 1.05);
	glm::vec3 new_player_position;

	// glm::vec3 upper_light_position = glm::vec3(0.0, 6.0, 0.0);
	float bubblesSpawnRate = 0.4;

	Sphere player_position_representation;

	// bool first_person_view = true;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP_first_view;
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		new_player_position = computeMatricesFromInputs(player_position, win());
		// printf("gracz jest: (%f, %f, %f). Gracz bedzie: (%f, %f, %f)\n", player_position.x, player_position.y, player_position.z, new_player_position.x, new_player_position.y, new_player_position.z);

		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP_first_view = ProjectionMatrix * ViewMatrix * ModelMatrix;

		ViewportOne(0, 0, wd, ht);
		glm::vec3 viewPosition = player_position;

		// Errors("xD");
		// std::vector<glm::vec3> bubbles_positions = just_bubbles.move_bubbles_higher();

		// printf("Teraz patrze z (%f, %f, %f)\n", viewPosition.x, viewPosition.y, viewPosition.z);
		// aquarium_cuboid.draw(MVP_first_view, viewPosition, player_position, upper_light_position);
		// just_bubbles.draw(MVP_first_view, viewPosition, player_position, upper_light_position);
		// if (!first_person_view)
		// player_position_representation.draw(MVP_first_view, player_position, viewPosition);

		glfwSwapBuffers(window);
		glfwPollEvents();
		WaitForFixedFPS(1000.0 / 60.0);
	} while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}
int main(int argc, char *argv[])
{
	int seed = time(NULL);

	if (argc > 2)
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

	srand(seed);
	main_window.Init(1280, 720, "Terrain", 0, 33);
	main_window.MainLoop();
	return 0;
}
