// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#define SRTM_SIZE 1201

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
#include "objects/MapTile/MapTile.hpp"

Window main_window;

char *data_folder_name;
int latitude_from;
int latitude_to;
int longitude_from;
int longitude_to;

void Window::ReadData(void)
{
	// printf("powalone\n");
	// map_data.reserve(latitude_to - latitude_from);
	// for (auto elem : map_data)
	// 	elem.reserve(longitude_to - longitude_from);
	// printf("powaloone2\n");
	for (int current_latitude = latitude_from; current_latitude <= latitude_to; current_latitude++)
	{
		for (int current_longitude = longitude_from; current_longitude <= longitude_to; current_longitude++)
		{
			// printf("Ogarniam teraz: N%d E%d\n", current_latitude, current_longitude);
			char filename[100];
			char latitude_text[4];
			char longitude_text[5];
			latitude_text[3] = 0;
			longitude_text[4] = 0;
			if (current_latitude > 0)
				latitude_text[0] = 'N';
			else
				latitude_text[0] = 'S';
			// printf("dupa1");
			if (current_longitude > 0)
				longitude_text[0] = 'E';
			else
				longitude_text[0] = 'W';
			// printf("dupa2");
			if (current_latitude < 10)
			{
				latitude_text[1] = '0';
				// printf("\nLITERKA: %c\n", '0' + current_latitude);
				latitude_text[2] = '0' + current_latitude;
			}
			else
			{
				latitude_text[1] = '0' + (current_latitude / 10);
				latitude_text[2] = '0' + (current_latitude % 10);
			}
			// printf("dupa3");
			if (current_longitude < 10)
			{
				longitude_text[1] = '0';
				longitude_text[2] = '0';
				longitude_text[3] = '0' + (current_longitude);
			}
			else if (current_longitude < 100)
			{
				longitude_text[1] = '0';
				longitude_text[2] = '0' + (current_longitude / 10);
				longitude_text[3] = '0' + (current_longitude % 10);
			}
			else
			{
				longitude_text[1] = '0' + (current_longitude / 100);
				longitude_text[2] = '0' + ((current_longitude / 10) % 10);
				longitude_text[3] = '0' + (current_longitude % 10);
			}
			std::snprintf(filename, 100, "%s%s%s.hgt", data_folder_name, latitude_text, longitude_text);
			// printf("dupa1\n");
			// printf("Czytam: %s\n\n", filename);

			map_data[current_latitude][current_longitude] = ReadFile(filename);
			// printf("dupa2\n");
		}
	}
}
std::vector<short> Window::ReadFile(std::string filepath)
// short height[SRTM_SIZE][SRTM_SIZE] Window::ReadData(std::string filepath)
{

	std::ifstream input(filepath, std::ios::binary);
	// copies all data into buffer
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	std::vector<short> heights;
	if (buffer.size() == 0)
	{
		std::cout << "Error reading file: " << filepath << "\n";
		return heights;
	}
	std::cout << "Read: " << filepath << "\n";
	// printf("%ld\n", buffer.size() / SRTM_SIZE);
	for (int i = 0; i < buffer.size(); i = i + 2)
	{
		short val = (buffer[i] << 8) | buffer[i + 1];
		heights.push_back(val);
	}
	// for (int i = 0; i < heights.size(); i++)
	// {
	// 	printf("%hd ", heights[i]);
	// }
	// printf("Rozmiar bufora: %ld\nPrzeczytałem %ld danych\n", buffer.size(), heights.size());
	return heights;
}

void Window::MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 player_position = glm::vec3((GLfloat)latitude_from, (GLfloat)longitude_from, 3.0);
	// glm::vec3 new_player_position;
	// std::vector<std::vector<MapTile>> whole_map;
	// for (int current_latitude = latitude_from; current_latitude <= latitude_to; current_latitude++)
	// {
	// 	for (int current_longitude = longitude_from; current_longitude <= longitude_to; current_longitude++)
	// 	{
	// 		MapTile temp_awful(&(map_data[current_latitude][current_longitude]), glm::vec2(current_latitude, current_longitude));
	// 		whole_map[current_latitude][current_longitude] = temp_awful;
	// 	}
	// }

	// std::vector<short> first_tile_data = ReadFile("data/N50E016.hgt");
	// std::vector<short> second_tile_data = ReadFile("data/N45E006.hgt");

	// for (int i = 0; i < SRTM_SIZE; i++)
	// {
	// 	printf("%d: ", i);
	// 	for (int j = 0; j < SRTM_SIZE; j++)
	// 	{
	// 		printf("%d, ", height_map[i * SRTM_SIZE + j]);
	// 		/* code */
	// 	}
	// 	printf("\n");
	// }
	// exit(EXIT_SUCCESS);
	// for (auto elem : map_data[51][128])
	// {
	// 	std::cout << elem;
	// }
	std::vector<MapTile> all_map_tiles;
	for (int current_latitude = latitude_from; current_latitude <= latitude_to; current_latitude++)
	{
		for (int current_longitude = longitude_from; current_longitude <= longitude_to; current_longitude++)
		{
			if (!map_data[current_latitude][current_longitude].empty())
			{
				MapTile current_tile(&map_data[current_latitude][current_longitude], glm::vec2((GLfloat)current_latitude, (GLfloat)current_longitude));
				all_map_tiles.push_back(current_tile);
			}
			printf("Obrobiltem teraz (%d, %d)\n", current_latitude, current_longitude);
		}
	}
	// MapTile tile_test(&map_data[51][128], glm::vec2(2.0f, 0.0f));
	// MapTile first_tile(&first_tile_data, glm::vec2(0.0f, 0.0f));
	// MapTile second_tile(&second_tile_data, glm::vec2(1.0f, 0.0f));
	// flat_map.setShaders();
	// flat_map.setBuffers(&height_map);

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP_first_view;
	printf("\nShowing Data\n");
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		player_position = computeMatricesFromInputs(player_position, win());

		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		MVP_first_view = ProjectionMatrix * ViewMatrix * ModelMatrix;

		ViewportOne(0, 0, wd, ht);
		glm::vec3 viewPosition = player_position;

		// Errors("przed rysowaniem");

		// printf("Teraz jestem na (%f, %f, %f)\n", player_position.x, player_position.y, player_position.z);

		// earth.draw(MVP_first_view, player_position, viewPosition);
		// tile_test.draw(MVP_first_view, player_position);
		for (auto curr_tile : all_map_tiles)
		{
			curr_tile.draw(MVP_first_view, player_position);
		}
		// first_tile.draw(MVP_first_view, player_position);
		// second_tile.draw(MVP_first_view, player_position);
		Errors("po rysowaniu");

		glfwSwapBuffers(window);
		glfwPollEvents();
		WaitForFixedFPS(1000.0 / 60.0);
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

	main_window.Init(1280, 720, "Terrain", 0, 33);
	main_window.ReadData();
	main_window.MainLoop();
	return 0;
}
