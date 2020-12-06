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

#include "objects/Cuboid/Cuboid.hpp"
#include "objects/Sphere/Sphere.hpp"
#include "objects/Particle/Particle.hpp"
#include "objects/Bubbles/Bubbles.hpp"

Window main_window;

int LastUsedParticle = 0;

bool distance_between_points(vec3 p1, vec3 p2)
{
	return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2);
}

bool is_point_still_in_aquarium(vec3 p)
{
	// p = p + vec3(0.01, 0.01, 0.01);
	return (-2.0f <= p.x && p.x <= 2.0f &&
			0.0f <= p.y && p.y <= 2.0f &&
			-1.0f <= p.z && p.z <= 1.0f);
}

void Window::MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 player_position = glm::vec3(-2.0, 1.0, 0.0);
	glm::vec3 win_position = glm::vec3(1.05, 1.05, 1.05);
	glm::vec3 new_player_position;

	Sphere player_position_representation;
	Cuboid aquarium_cuboid;
	Bubbles just_bubbles;

	bool first_person_view = true;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP_first_view;
	do
	{
		// printf("playerpos: (%f, %f, %f)\n", player_position.x, player_position.y, player_position.z);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		new_player_position = first_person_view ? computeMatricesFromInputs(player_position, win()) : computeViewMatrices(player_position, win());
		// printf("gracz jest: (%f, %f, %f). Gracz bedzie: (%f, %f, %f)\n", player_position.x, player_position.y, player_position.z, new_player_position.x, new_player_position.y, new_player_position.z);
		// player_position = computeMatricesFromInputs(player_position, win());
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP_first_view = ProjectionMatrix * ViewMatrix * ModelMatrix;

		ViewportOne(0, 0, wd, ht);
		glm::vec3 viewPosition = first_person_view ? player_position : glm::vec3(0.0, 6.0, 0.0);
		// win_sphere.draw(MVP_first_view, win_position);

		// test_particle.draw(MVP_first_view);
		Errors("xD");
		std::vector<glm::vec3> bubbles_positions = just_bubbles.move_bubbles_higher();

		aquarium_cuboid.draw(MVP_first_view, viewPosition);
		just_bubbles.draw(MVP_first_view, viewPosition);
		if (!first_person_view)
		{
			player_position_representation.draw(MVP_first_view, player_position, viewPosition);
			printf("rysuje reprezentacje gracza\n");
		}

		for (auto &bub_pos : bubbles_positions)
		{
			// distance_between_points(player_position, bub_pos);
			if (glm::distance(player_position, bub_pos) < 0.15)
			{
				printf("gratuluje przegranej\n");
				exit(EXIT_SUCCESS);
			}
		}
		if (is_point_still_in_aquarium(new_player_position))
			player_position = new_player_position;

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			first_person_view = !first_person_view;
		}
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
	main_window.Init(1280, 720, "Aquarium", 0, 33);
	main_window.MainLoop();
	return 0;
}
