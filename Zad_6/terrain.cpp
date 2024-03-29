#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <iterator>
#include <string>

#define SRTM_SIZE 1201
#define RADIUS 637

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
bool sphere_mode = false;
bool auto_LoD = true;
glm::vec2 flat_middle_of_all_tiles;

glm::vec3
global_flat_to_spherical(glm::vec3 global_flat_coordinates)
{
	return sphere_scale *
		   glm::vec3(
			   (RADIUS + global_flat_coordinates.z) * cos(radians(global_flat_coordinates.y)) * cos(radians(global_flat_coordinates.x)),
			   (RADIUS + global_flat_coordinates.z) * cos(radians(global_flat_coordinates.y)) * sin(radians(global_flat_coordinates.x)),
			   (RADIUS + global_flat_coordinates.z) * sin(radians(global_flat_coordinates.y)));
}
glm::vec3 global_flat_to_flat(glm::vec3 global_flat_coordinates)
{
	return glm::vec3(
		global_flat_coordinates.x * cos(radians(flat_middle_of_all_tiles.y)),
		global_flat_coordinates.y,
		global_flat_coordinates.z);
}
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
	flat_middle_of_all_tiles = glm::vec2(
		(GLfloat)(longitude_to + longitude_from + 1.0f) / 2.0f,
		(GLfloat)(latitude_to + latitude_from + 1.0f) / 2.0f);

	glm::vec3 player_global_position_flat = glm::vec3(flat_middle_of_all_tiles, 3.0);
	glm::vec3 player_position_flat = global_flat_to_flat(player_global_position_flat);
	glm::vec3 player_position_sphere = global_flat_to_spherical(player_position_flat);

	printf("Zaczynam na global:(%f, %f, %f), flat:(%f, %f, %f), spherical: (%f, %f, %f)\n",
		   player_global_position_flat.x,
		   player_global_position_flat.y,
		   player_global_position_flat.z,
		   player_position_flat.x,
		   player_position_flat.y,
		   player_position_flat.z,
		   player_position_sphere.x,
		   player_position_sphere.y,
		   player_position_sphere.z);

	std::vector<MapTile> flat_map_tiles;
	std::vector<MapTile> spherical_map_tiles;
	for (int current_longitude = longitude_from; current_longitude <= longitude_to; current_longitude++) // po longitude = X
	{
		for (int current_latitude = latitude_from; current_latitude <= latitude_to; current_latitude++) // po latitude = Y
		{
			if (!map_data[current_longitude][current_latitude].empty())
			{
				MapTile current_flat_tile(
					&map_data[current_longitude][current_latitude],
					glm::vec2((GLfloat)(current_longitude), (GLfloat)current_latitude),
					true,
					flat_middle_of_all_tiles);
				flat_map_tiles.push_back(current_flat_tile);
				MapTile current_spherical_tile(
					&map_data[current_longitude][current_latitude],
					glm::vec2((GLfloat)(current_longitude), (GLfloat)current_latitude),
					false,
					flat_middle_of_all_tiles);
				spherical_map_tiles.push_back(current_spherical_tile);
			}
			printf("Processing tile: (%d, %d)\n", current_latitude, current_longitude);
		}
	}

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP;
	unsigned short current_lod_level = 0, fps_amount = 0, triangles_drawn = 0;

	double last_speed_time_check = glfwGetTime();
	double last_sphere_change = glfwGetTime();
	double last_lod_change = glfwGetTime();

	glm::vec3 viewPosition = player_position_flat;
	printf("\nShowing Data\n");
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		computeMatricesFromInputs(
			player_global_position_flat,
			player_position_flat,
			player_position_sphere,
			win(),
			sphere_mode,
			flat_middle_of_all_tiles);
		player_position_flat = global_flat_to_flat(player_global_position_flat);
		player_position_sphere = global_flat_to_spherical(player_global_position_flat);

		if (sphere_mode)
		{
			viewPosition = player_position_sphere;
		}
		else
		{
			viewPosition = player_position_flat;
		}

		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		ViewportOne(0, 0, wd, ht);

		triangles_drawn = 0;
		if (sphere_mode)
			for (auto curr_tile : spherical_map_tiles)
				triangles_drawn += curr_tile.draw(MVP, current_lod_level);
		else
			for (auto curr_tile : flat_map_tiles)
				triangles_drawn += curr_tile.draw(MVP, current_lod_level);

		double currentTime = glfwGetTime();
		fps_amount++;
		if (currentTime - last_speed_time_check >= 1.0)
		{
			printf("FPS: %d | Triangles drawn: %d | LoD level: %d ", fps_amount, triangles_drawn, current_lod_level);
			if (auto_LoD)
			{
				if (fps_amount < 10 && current_lod_level < 4)
				{
					current_lod_level += 1;
					printf("+");
				}
				if (fps_amount > 15 && current_lod_level > 0)
				{
					current_lod_level -= 1;
					printf("-");
				}
			}
			printf("\n");
			fps_amount = 0;
			last_speed_time_check += 1.0;
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		{
			double currentTime = glfwGetTime();
			if (currentTime - last_lod_change >= 1.0)
			{
				if (auto_LoD)
					printf("auto LoD off\n");
				else
					printf("auto LoD on\n");
				auto_LoD = !auto_LoD;
				last_lod_change = glfwGetTime();
			}
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		{
			double currentTime = glfwGetTime();
			if (currentTime - last_sphere_change >= 1.0)
			{
				if (sphere_mode)
				{
					printf("Flat map\n");
					player_position_flat.z -= 10;
				}
				else
				{
					printf("Spherical map\n");
					player_position_flat.z += 10;
				}
				sphere_mode = !sphere_mode;
				last_sphere_change = glfwGetTime();
			}
		}

		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && current_lod_level < 4)
			current_lod_level += 1;
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && current_lod_level > 0)
			current_lod_level -= 1;

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
			 glfwWindowShouldClose(window) == 0);

	glfwTerminate();
}
int main(int argc, char *argv[])
{
	if (argc != 8)
	{
		printf("Wrong arguments.\n");
		printf("eg. ./terrain ./data/ -lat 50 51 -lon 16 17\n");
		exit(EXIT_FAILURE);
	}

	data_folder_name = argv[1];
	latitude_from = atoi(argv[3]);
	latitude_to = atoi(argv[4]);
	longitude_from = atoi(argv[6]);
	longitude_to = atoi(argv[7]);

	main_window.Init(1000, 1000, "Terrain", 0, 33);
	main_window.ReadData();
	main_window.MainLoop();
	return 0;
}
