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

#include <Window.hpp>

#include <common/shader.hpp>
#include <common/controls.hpp>

#include "objects/Cube/Cube.hpp"
#include "objects/Sphere/Sphere.hpp"
#include "objects/Triangle/Triangle.hpp"

int amount_of_columns = 10;
int amount_of_rows = amount_of_columns;
int amount_of_layers = amount_of_rows;

Window main_window;

bool wincheck(vec3 player, vec3 end_line)
{
	if (player.x > 0.5 && player.y > 0.5 && player.z > 0.5)
	{
		if (pow(0.05 * 2, 2) > pow(player.x - end_line.x, 2) + pow(player.y - end_line.y, 2) + pow(player.z - end_line.z, 2))
			return true;
	}
	return false;
}

float distance_between_point_and_plane(vec3 point, vec4 plane_equasion)
{
	return fabs(plane_equasion.x * point.x +
				plane_equasion.y * point.y +
				plane_equasion.z * point.z +
				plane_equasion.w) /
		   sqrt(
			   pow(plane_equasion.x, 2) +
			   pow(plane_equasion.y, 2) +
			   pow(plane_equasion.z, 2));
}
float triangle_area(vec3 A, vec3 B, vec3 C)
{
	vec3 AB = B - A;
	vec3 AC = C - A;
	return length(cross(AB, AC)) / 2;
}

bool collision_check_player_with_triangle(vec3 player, vec3 triangle_vertex1, vec3 triangle_vertex2, vec3 triangle_vertex3)
{
	vec3 plane_normal_vector = cross(triangle_vertex2 - triangle_vertex1, triangle_vertex3 - triangle_vertex1);
	// to jest wektor prostopadły do płaszczyzny.

	//Tutaj liczę współczynnik D równania prostej postaci Ax+By+Cz+D=0
	float D = -(plane_normal_vector.x * triangle_vertex1.x + plane_normal_vector.y * triangle_vertex1.y + plane_normal_vector.z * triangle_vertex1.z);
	//Uzyskując pełne równanie płąszczyzny
	vec4 plane_equasion = vec4(plane_normal_vector, D);
	// oraz wektor normalny plaszczyzny wyznaczanej przez trojkat
	plane_normal_vector = normalize(plane_normal_vector);
	//tutaj liczona jest odleglosc między pozycją gracza, a plaszczyzną wyznaczoną przez trójkąt
	float dist = distance_between_point_and_plane(player, plane_equasion);
	//jesli jest to odleglosc większa niż 0.005 to na pewno nie ma kolizji
	if (dist > 0.005)
	{
		return false;
	}
	// =======================================================================
	//Tutaj jest sprawdzenie, rzut punktu gracza na plaszczyznę wyznaczaną przez trójkąt
	vec3 R;
	vec3 R1 = player - dist * plane_normal_vector;
	vec3 R2 = player + dist * plane_normal_vector;

	if (distance_between_point_and_plane(R1, plane_equasion) < distance_between_point_and_plane(R2, plane_equasion))
		R = R1;
	else
		R = R2;

	float is_player_on_plane = abs(R.x * plane_equasion.x + R.y * plane_equasion.y + R.z * plane_equasion.z + plane_equasion.w);
	// ======================================================================
	// liczę pole trojkąta ABC
	float correct_triangle_area = triangle_area(triangle_vertex1, triangle_vertex2, triangle_vertex3);
	// liczę pola trójkątów RBC, ARC, ABR
	float first = triangle_area(R, triangle_vertex2, triangle_vertex3);
	float second = triangle_area(R, triangle_vertex1, triangle_vertex2);
	float third = triangle_area(R, triangle_vertex1, triangle_vertex3);
	// Sprawdzam, czy gracz zrzutowany na płaszczynę trójkąta jest wewnątrz pola trójkąta.
	// W taki sposób, że sumuję pola trójkątów mniejszych, odejmuję pole poprawne i powinno mi wyjść 0 jeśli rzut gracza jest wewnątrz pola.
	if (fabs(first + second + third - correct_triangle_area) < 0.05)
	{
		return true;
	}
	//jesli nie to jestesmy na plaszczyznie trojkata, ale nie w trojkacie
	return false;
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

	float x_move, y_move, z_move;
	Triangle triangles[amount_of_columns][amount_of_rows][amount_of_layers];
	glm::vec3 first_triangle_vertex[amount_of_rows][amount_of_columns][amount_of_layers];
	glm::vec3 second_triangle_vertex[amount_of_rows][amount_of_columns][amount_of_layers];
	glm::vec3 third_triangle_vertex[amount_of_rows][amount_of_columns][amount_of_layers];
	for (int i = 0; i < amount_of_columns; i++) // X
	{
		for (int j = 0; j < amount_of_rows; j++) // Y
		{
			for (int k = 0; k < amount_of_layers; k++) // Z
			{
				glm::vec3 crate_center =
					glm::vec3(-1.0 + i * (2.0 / amount_of_columns),
							  -1.0 + j * (2.0 / amount_of_rows),
							  -1.0 + k * (2.0 / amount_of_layers));
				x_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				z_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				y_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				first_triangle_vertex[i][j][k] = crate_center + glm::vec3(x_move, y_move, z_move);
				x_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				z_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				y_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				second_triangle_vertex[i][j][k] = crate_center + glm::vec3(x_move, y_move, z_move);
				x_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				z_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				y_move = (((float)rand() / (float)(RAND_MAX)) * (2.0 / amount_of_columns)) - (1.0 / amount_of_columns);
				third_triangle_vertex[i][j][k] = crate_center + glm::vec3(x_move, y_move, z_move);
				triangles[i][j][k].setBuffers(first_triangle_vertex[i][j][k], second_triangle_vertex[i][j][k], third_triangle_vertex[i][j][k]);
			}
		}
	}

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

		for (int i = 0; i < amount_of_columns; i++) // X
		{
			for (int j = 0; j < amount_of_rows; j++) // Y
			{
				for (int k = 0; k < amount_of_layers; k++) // Z
				{
					triangles[i][j][k].draw(MVP_first_view);
				}
			}
		}
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

		for (int i = 0; i < amount_of_columns; i++) // X
		{
			for (int j = 0; j < amount_of_rows; j++) // Y
			{
				for (int k = 0; k < amount_of_layers; k++) // Z
				{
					triangles[i][j][k].draw(MVP_second_view);
				}
			}
		}
		if (wincheck(player_position, win_position))
		{
			printf("Congratulations!\n");
			exit(EXIT_SUCCESS);
		}
		bool we_have_collision = false;
		for (int i = 0; i < amount_of_columns; i++) // X
		{
			for (int j = 0; j < amount_of_rows; j++) // Y
			{
				for (int k = 0; k < amount_of_layers; k++) // Z
				{
					if (collision_check_player_with_triangle(new_player_position, first_triangle_vertex[i][j][k], second_triangle_vertex[i][j][k], third_triangle_vertex[i][j][k]))
					{
						we_have_collision = true;
						break;
					}
				}
			}
		}

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
