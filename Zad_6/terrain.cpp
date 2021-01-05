// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <fstream>
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

std::vector<short> Window::ReadData(std::string filepath)
// short height[SRTM_SIZE][SRTM_SIZE] Window::ReadData(std::string filepath)
{
	std::ifstream input(filepath, std::ios::binary);
	// copies all data into buffer
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	printf("%ld\n", buffer.size() / SRTM_SIZE);
	std::vector<short> heights;
	for (int i = 0; i < buffer.size(); i = i + 2)
	{
		short val = (buffer[i] << 8) | buffer[i + 1];
		heights.push_back(val);
	}
	// for (int i = 0; i < heights.size(); i++)
	// {
	// 	printf("%hd ", heights[i]);
	// }
	printf("Rozmiar bufora: %ld\nPrzeczytałem %ld danych\n", buffer.size(), heights.size());
	return heights;
}

void Window::MainLoop()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 player_position = glm::vec3(0.0, 0.0, 3.0);
	// glm::vec3 new_player_position;

	std::vector<short> first_tile_data = ReadData("data/N50E016.hgt");
	std::vector<short> second_tile_data = ReadData("data/N45E006.hgt");

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

	MapTile first_tile(&first_tile_data, 0.0f);
	MapTile second_tile(&second_tile_data, 1.0f);
	// flat_map.setShaders();
	// flat_map.setBuffers(&height_map);

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ModelMatrix;
	glm::mat4 MVP_first_view;
	printf("\nPATRZENIE\n\n");
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

		Errors("przed rysowaniem");

		// printf("Teraz jestem na (%f, %f, %f)\n", player_position.x, player_position.y, player_position.z);

		// earth.draw(MVP_first_view, player_position, viewPosition);
		first_tile.draw(MVP_first_view, player_position);
		second_tile.draw(MVP_first_view, player_position);
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
	printf("I got: %d arguments\n", argc);
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

	printf("I got that I should print data from: %s\n", data_folder_name);
	printf("with latitude starting at: %d and ending with: %d\n", latitude_from, latitude_to);
	printf("with longitude starting at: %d and ending with: %d\n", longitude_from, longitude_to);

	main_window.Init(1280, 720, "Terrain", 0, 33);
	main_window.MainLoop();
	return 0;
}
