// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <iterator>
#include <string>

#define SRTM_SIZE 1201
// #define R 6378
#define RADIUS 378

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

Window main_window;

char *data_folder_name;
int latitude_from;
int latitude_to;
int longitude_from;
int longitude_to;
const float sphere_scale = 0.1;

#include "objects/Sphere/Sphere.hpp"
#include "objects/MapTile/MapTile.hpp"

void Window::ReadData(void)
{
	for (int current_latitude = latitude_from; current_latitude <= latitude_to; current_latitude++)
	{
		for (int current_longitude = longitude_from; current_longitude <= longitude_to; current_longitude++)
		{
			char filename[100];
			char latitude_text[4];
			char longitude_text[5];
			latitude_text[3] = 0;
			longitude_text[4] = 0;
			// ================= Setup latitude and longitude letter
			if (current_latitude > 0)
				latitude_text[0] = 'N';
			else
			{
				printf("South hemisphere not supported yet.");
				exit(EXIT_SUCCESS);
				latitude_text[0] = 'S';
			}

			// printf("dupa1");
			if (current_longitude > 0)
				longitude_text[0] = 'E';
			else
			{
				printf("West hemisphere not supported yet.");
				exit(EXIT_SUCCESS);
				longitude_text[0] = 'W';
			}

			//================= Set-up latitude Y
			if (current_latitude < 10)
			{
				latitude_text[1] = '0';
				latitude_text[2] = '0' + current_latitude;
			}
			else
			{
				latitude_text[1] = '0' + (current_latitude / 10);
				latitude_text[2] = '0' + (current_latitude % 10);
			}
			//================= Set-up longitude X
			short current_correct_longitude = abs(current_longitude);
			if (current_correct_longitude < 10)
			{
				longitude_text[1] = '0';
				longitude_text[2] = '0';
				longitude_text[3] = '0' + (current_correct_longitude);
			}
			else if (current_correct_longitude < 100)
			{
				longitude_text[1] = '0';
				longitude_text[2] = '0' + (current_correct_longitude / 10);
				longitude_text[3] = '0' + (current_correct_longitude % 10);
			}
			else
			{
				longitude_text[1] = '0' + (current_correct_longitude / 100);
				longitude_text[2] = '0' + ((current_correct_longitude / 10) % 10);
				longitude_text[3] = '0' + (current_correct_longitude % 10);
			}
			std::snprintf(filename, 100, "%s%s%s.hgt", data_folder_name, latitude_text, longitude_text);

			// Tutaj longitude może mieć wartość -28, a w tablicy to adres -28 + 360/2 = 152, gdzie dla -180 jest 0, a dla 0 jest 180
			map_data[current_longitude][current_latitude] = ReadFile(filename);
		}
	}
}

void Window::MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 player_position_flat =
		glm::vec3(
			(GLfloat)(longitude_from),
			(GLfloat)(latitude_from),
			3.0);
	glm::vec3 player_position_sphere =
		sphere_scale *
		glm::vec3(
			(RADIUS + player_position_flat.z) * cos(radians(player_position_flat.y)) * cos(radians(player_position_flat.x)),
			(RADIUS + player_position_flat.z) * cos(radians(player_position_flat.y)) * sin(radians(player_position_flat.x)),
			(RADIUS + player_position_flat.z) * sin(radians(player_position_flat.y)));
	// player_position_sphere = glm::vec3(1.0, 0.0, 0.0);
	printf("Zaczynam na (%f, %f, %f), czyli (%f, %f, %f)\n",
		   player_position_flat.x,
		   player_position_flat.y,
		   player_position_flat.z,
		   player_position_sphere.x,
		   player_position_sphere.y,
		   player_position_sphere.z);

	std::vector<MapTile> all_map_tiles;
	for (int current_longitude = longitude_from; current_longitude <= longitude_to; current_longitude++) // po longitude = X
	{
		for (int current_latitude = latitude_from; current_latitude <= latitude_to; current_latitude++) // po latitude = Y
		{
			if (!map_data[current_longitude][current_latitude].empty())
			{
				MapTile current_tile(
					&map_data[current_longitude][current_latitude],
					glm::vec2((GLfloat)(current_longitude), (GLfloat)current_latitude));
				all_map_tiles.push_back(current_tile);
			}
			printf("Processing tile: (%d, %d)\n", current_latitude, current_longitude);
		}
	}

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP_first_view;
	unsigned short current_lod_level = 0, fps_amount = 0, triangles_drawn = 0;

	double lastTime = glfwGetTime();

	// int fps_amount = 0;

	printf("\nShowing Data\n");
	do
	{

		// printf("Jestem na (%f, %f, %f), czyli (%f, %f, %f)\n",
		// 	   player_position_flat.x,
		// 	   player_position_flat.y,
		// 	   player_position_flat.z,
		// 	   player_position_sphere.x,
		// 	   player_position_sphere.y,
		// 	   player_position_sphere.z);

		// Measure speed
		double currentTime = glfwGetTime();
		fps_amount++;
		if (currentTime - lastTime >= 1.0)
		{
			// printf and reset timer
			printf("FPS: %d | Triangles drawn: %d | LoD level: %d\n", fps_amount, triangles_drawn, current_lod_level);
			// printf("%f ms/frame | fps: %d\n", 1000.0 / double(fps_amount), fps_amount);
			if (fps_amount < 10 && current_lod_level < 4)
			{
				current_lod_level += 1;
			}
			if (fps_amount > 15 && current_lod_level > 0)
			{
				current_lod_level -= 1;
			}
			fps_amount = 0;
			lastTime += 1.0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		computeMatricesFromInputs(player_position_flat, player_position_sphere, win());
		player_position_sphere =
			sphere_scale *
			glm::vec3(
				(RADIUS + player_position_flat.z) * cos(radians(player_position_flat.y)) * cos(radians(player_position_flat.x)),
				(RADIUS + player_position_flat.z) * cos(radians(player_position_flat.y)) * sin(radians(player_position_flat.x)),
				(RADIUS + player_position_flat.z) * sin(radians(player_position_flat.y)));

		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP_first_view = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// player_position_sphere =
		// 	sphere_scale *
		// 	glm::vec3(
		// 		(RADIUS + radians(player_position_flat.z)) * cos(radians(player_position_flat.y)) * cos(radians(player_position_flat.x)),
		// 		(RADIUS + radians(player_position_flat.z)) * cos(radians(player_position_flat.y)) * sin(radians(player_position_flat.x)),
		// 		(RADIUS + radians(player_position_flat.z)) * sin(radians(player_position_flat.y)));
		ViewportOne(0, 0, wd, ht);
		glm::vec3 viewPosition = player_position_sphere;

		// earth.draw(MVP_first_view, player_position, viewPosition);
		// tile_test.draw(MVP_first_view, player_position);
		triangles_drawn = 0;
		for (auto curr_tile : all_map_tiles)
		{
			triangles_drawn += curr_tile.draw(MVP_first_view, current_lod_level);
		}
		// first_tile.draw(MVP_first_view, player_position);
		// second_tile.draw(MVP_first_view, player_position);
		// Errors("po rysowaniu");

		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && current_lod_level < 4)
			current_lod_level += 1;
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && current_lod_level > 0)
			current_lod_level -= 1;

		glfwSwapBuffers(window);
		glfwPollEvents();
		// WaitForFixedFPS(1000.0 / 60.0);
	} while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}
int main(int argc, char *argv[])
{
	// printf("I got: %d arguments\n", argc);
	// exit(EXIT_SUCCESS);
	if (argc != 8)
	{
		printf("Wrong arguments.\n");
		printf("eg. ./terrain ./data/ -lat 50 51 -lon 16 17\n");

		exit(EXIT_FAILURE);
	}

	data_folder_name = argv[1];

	// if (argv[2] == "-lat")
	// {
	latitude_from = atoi(argv[3]);
	latitude_to = atoi(argv[4]);
	// }
	// if (argv[5] == "-lon")
	// {
	longitude_from = atoi(argv[6]);
	longitude_to = atoi(argv[7]);
	// }

	// printf("I got that I should print data from: %s\n", data_folder_name);
	// printf("with latitude starting at: %d and ending with: %d\n", latitude_from, latitude_to);
	// printf("with longitude starting at: %d and ending with: %d\n", longitude_from, longitude_to);

	main_window.Init(1000, 1000, "Terrain", 0, 33);
	main_window.ReadData();
	main_window.MainLoop();
	return 0;
}
