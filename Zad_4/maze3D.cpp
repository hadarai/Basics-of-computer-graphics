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
// GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <Window.hpp>

#include <common/shader.hpp>
#include <common/controls.hpp>

#include "objects/Cube/Cube.hpp"
#include "objects/PlayerSphere/PlayerSphere.hpp"
#include "objects/Triangle/Triangle.hpp"

int amount_of_columns = 10;
int amount_of_rows = amount_of_columns;
int amount_of_layers = amount_of_rows;

Window main_window;

// int main(int argc, char *argv[])
void Window::MainLoop()
{
	int side = min(wd, ht);
	ViewportOne(0, 0, side, side);

	// if (!glfwInit())
	// {
	// 	fprintf(stderr, "Failed to initialize GLFW\n");
	// 	getchar();
	// 	// return -1;
	// 	exit(EXIT_FAILURE);
	// }

	// glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	// window = glfwCreateWindow(1024, 768, "Maze 3D", NULL, NULL);
	// if (window == NULL)
	// {
	// 	fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
	// 	getchar();
	// 	glfwTerminate();
	// 	// return -1;
	// 	exit(EXIT_FAILURE);
	// }
	// glfwMakeContextCurrent(window);

	// Initialize GLEW
	// glewExperimental = true; // Needed for core profile
	// if (glewInit() != GLEW_OK)
	// {
	// 	fprintf(stderr, "Failed to initialize GLEW\n");
	// 	getchar();
	// 	glfwTerminate();

	// 	exit(EXIT_FAILURE);
	// }
	// // During init, enable debug output
	// glEnable(GL_DEBUG_OUTPUT);
	// glDebugMessageCallback(MessageCallback, 0);

	// Ensure we can capture the escape key being pressed below
	// glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	// glfwPollEvents();
	// glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// glm::vec3 pos1_tri = glm::vec3(0.707899, 0.716031, 0.093660);
	glm::vec3 pos1_tri = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 pos2_tri = glm::vec3(0.786788, 0.808426, 0.033422);
	glm::vec3 pos3_tri = glm::vec3(0.869499, 0.719856, 0.096147);

	glm::vec3 pos4_tri = glm::vec3(0.707899, 0.716031, 1.093660);
	glm::vec3 pos5_tri = glm::vec3(0.786788, 1.808426, 0.033422);
	glm::vec3 pos6_tri = glm::vec3(0.869499, 0.719856, 1.096147);

	// Cube test_cube;
	// Triangle test_triangle(pos4_tri, pos2_tri, pos3_tri);
	// Triangle test_triangle2(pos4_tri, pos5_tri, pos6_tri);
	// Triangle test_triangle;
	// Triangle test_triangle2;

	// test_triangle.setBuffers(pos4_tri, pos2_tri, pos3_tri);
	// test_triangle2.setBuffers(pos4_tri, pos5_tri, pos6_tri);

	Triangle triangles[amount_of_columns][amount_of_rows][amount_of_layers];
	// PlayerSphere test_sphere;
	float x_move, y_move, z_move;
	glm::vec3 first_triangle_vertex[amount_of_rows][amount_of_columns][amount_of_layers];
	glm::vec3 second_triangle_vertex[amount_of_rows][amount_of_columns][amount_of_layers];
	glm::vec3 third_triangle_vertex[amount_of_rows][amount_of_columns][amount_of_layers];
	for (int i = 0; i < amount_of_columns; i++) // X
	{
		// int j = 0;
		for (int j = 0; j < amount_of_rows; j++) // Y
		{
			// int i = 0, j = 0;
			for (int k = 0; k < amount_of_layers; k++) // Z
			{
				glm::vec3 crate_center =
					glm::vec3(-1.0 + i * (2.0 / amount_of_columns),
							  -1.0 + j * (2.0 / amount_of_rows),
							  -1.0 + k * (2.0 / amount_of_layers));
				// printf("Mamy trojkat o srodku:(%f, %f, %f)\n", crate_center.x, crate_center.y, crate_center.z);
				x_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				y_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				z_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				// printf("Mamy trojkat przesuniety o (%f, %f, %f)\n", x_move, y_move, z_move);
				first_triangle_vertex[i][j][k] = crate_center + glm::vec3(x_move, y_move, z_move);
				// printf("Mamy wierzcholek trojkata na pozycji (%f, %f, %f)\n", first_triangle_vertex[i][j][k].x, first_triangle_vertex[i][j][k].y, first_triangle_vertex[i][j][k].z);
				x_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				y_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				z_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				second_triangle_vertex[i][j][k] = crate_center + glm::vec3(x_move, y_move, z_move);
				x_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				y_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				z_move = (((float)rand() / (float)(RAND_MAX)) / 10 * 2) - 0.1;
				third_triangle_vertex[i][j][k] = crate_center + glm::vec3(x_move, y_move, z_move);
				// printf("Mamy przykladowy trojkat o wierzcholkach: (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)\n",
				// 	   first_triangle_vertex[i][j][k].x,
				// 	   first_triangle_vertex[i][j][k].y,
				// 	   first_triangle_vertex[i][j][k].z,
				// 	   second_triangle_vertex[i][j][k].x,
				// 	   second_triangle_vertex[i][j][k].y,
				// 	   second_triangle_vertex[i][j][k].z,
				// 	   third_triangle_vertex[i][j][k].x,
				// 	   third_triangle_vertex[i][j][k].y,
				// 	   third_triangle_vertex[i][j][k].z);

				triangles[i][j][k].setBuffers(first_triangle_vertex[i][j][k], second_triangle_vertex[i][j][k], third_triangle_vertex[i][j][k]);
			}
		}
	}
	// test_triangle2.setBuffers(pos4_tri, pos5_tri, pos6_tri);
	// int i = 1, j = 1, k = 1;
	// test_triangle2.set_vertexes(pos4_tri, pos5_tri, pos6_tri);
	glm::vec3 player_position = glm::vec3(-1.0, -1.0, -1.0);

	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ========================================================
		computeMatricesFromInputs(player_position, win());
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// test_cube.draw(MVP);
		// test_triangle.draw(MVP);
		// test_triangle2.draw(MVP);
		// test_sphere.draw(MVP, glm::vec3(-0.9f, -0.9f, -0.9f));
		for (int i = 0; i < amount_of_columns; i++) // X
		{
			// int j = 0;
			for (int j = 0; j < amount_of_rows; j++) // Y
			{
				for (int k = 0; k < amount_of_layers; k++) // Z
				{
					triangles[i][j][k].draw(MVP);
				}
			}
		}
		// ==========================================================
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
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
	}
	else
	{
		amount_of_columns = atoi(argv[2]);
		printf("Given side length: %d\n", amount_of_columns);
		amount_of_rows = amount_of_columns;
	}

	srand(seed);
	main_window.Init(1280, 720, "Maze 3D", 0, 33);
	main_window.MainLoop();
	return 0;
}
