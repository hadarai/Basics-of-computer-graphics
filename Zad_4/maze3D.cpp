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

float gcd(float a, float b)
{
	if (a < b)
		return gcd(b, a);
	// base case
	if (fabs(b) < 0.001)
		return a;
	else
		return (gcd(b, a - floor(a / b) * b));
}

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
	if (player.x < -2 || player.x > 2 || player.y < -2 || player.y > 2 || player.z < -2 || player.z > 2)
		return false;

	vec3 vector_1_on_plane = triangle_vertex2 - triangle_vertex1;
	vec3 vector_2_on_plane = triangle_vertex3 - triangle_vertex1;
	//iloczyn wektorowy dwoch wektorów na tej płaszczyźnie
	vec3 plane_normal_vector = cross(vector_1_on_plane, vector_2_on_plane);
	// to jest wektor prostopadły do płaszczyzny.

	//Tutaj liczę współczynnik D równania prostej postaci Ax+By+Cz+D=0
	float D = -(plane_normal_vector.x * triangle_vertex1.x + plane_normal_vector.y * triangle_vertex1.y + plane_normal_vector.z * triangle_vertex1.z);
	//Uzyskując pełne równanie płąszczyzny
	plane_normal_vector = normalize(plane_normal_vector);
	vec4 plane_equasion = vec4(plane_normal_vector, D);
	vec3 player_on_triangle_plane = player + plane_normal_vector;
	float dist = distance_between_point_and_plane(player, plane_equasion);
	// =======================================================================

	vec3 R;
	vec3 R1 = player - dist * plane_normal_vector;
	vec3 R2 = player + dist * plane_normal_vector;

	if (distance_between_point_and_plane(R1, plane_equasion) < distance_between_point_and_plane(R2, plane_equasion))
		R = R1;
	else
		R = R2;
	// ======================================================================
	float is_player_on_plane = abs(R.x * plane_equasion.x + R.y * plane_equasion.y + R.z * plane_equasion.z + plane_equasion.w);
	// float is_player_on_plane = player_on_triangle_plane
	printf("PLAYER ON TRIANGLE PLANE: %f\n", is_player_on_plane);

	if (dist > 0.05)
		return false;
	// wiem, ze ogleglosc miedzy graczem i plaszczyzną wyznaczoną przez wierzcholki trójkąta jest mniejsza niż promień kulki gracza
	printf("Sprawdzam dalej kolizje.\n");
	printf("Sprawdzam z trojkatem o pierwszym wierzcholku w: (%f, %f, %f)", triangle_vertex1.x, triangle_vertex1.y, triangle_vertex1.z);
	// printf("Sprawdzam kolizje gracza: (%f, %f, %f) z trojkatem o wierzcholkach: (%f, %f, %f), (%f, %f, %f), (%f, %f, %f).\n", player.x, player.y, player.z, triangle_vertex1.x, triangle_vertex1.y, triangle_vertex1.z, triangle_vertex2.x, triangle_vertex2.y, triangle_vertex2.z, triangle_vertex3.x, triangle_vertex3.y, triangle_vertex3.z);
	// if (player.x > -1.1 && player.y > -1.1 && player.z > -1.1)
	// 	return true;
	return false;
}

void Window::secondViewPortSetup(int width, int height)
{
	int extra_viewport_side = abs(width - height);
	Viewport(width - extra_viewport_side, height - extra_viewport_side, extra_viewport_side, extra_viewport_side);
}

// int main(int argc, char *argv[])
void Window::MainLoop()
{
	int side = min(wd, ht);
	collision_check_player_with_triangle(vec3(4.0, -4.0, 3.0), vec3(1, -2, 1), vec3(4, -2, -2), vec3(4, 1, 4));
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// glEnable(GL_CULL_FACE);

	// glm::vec3 pos1_tri = glm::vec3(0.707899, 0.716031, 0.093660);
	// glm::vec3 pos1_tri = glm::vec3(1.0, 1.0, 1.0);
	// glm::vec3 pos2_tri = glm::vec3(0.786788, 0.808426, 0.033422);
	// glm::vec3 pos3_tri = glm::vec3(0.869499, 0.719856, 0.096147);

	// glm::vec3 pos4_tri = glm::vec3(0.707899, 0.716031, 1.093660);
	// glm::vec3 pos5_tri = glm::vec3(0.786788, 1.808426, 0.033422);
	// glm::vec3 pos6_tri = glm::vec3(0.869499, 0.719856, 1.096147);
	glm::vec3 win_position = glm::vec3(1.05, 1.05, 1.05);

	// Cube test_cube;
	// Triangle test_triangle(pos4_tri, pos2_tri, pos3_tri);
	// Triangle test_triangle;
	Triangle triangles[amount_of_columns][amount_of_rows][amount_of_layers];
	Sphere player_position_representation("objects/Sphere/PlayerSphereFragmentShader.fragmentshader");
	Sphere win_sphere("objects/Sphere/WinSphereFragmentShader.fragmentshader");
	float x_move, y_move, z_move;
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

				triangles[i][j][k].setBuffers(first_triangle_vertex[i][j][k], second_triangle_vertex[i][j][k], third_triangle_vertex[i][j][k]);
			}
		}
	}
	glm::vec3 player_position = glm::vec3(-1.1, -1.1, -1.1);
	glm::vec3 new_player_position;
	// glm::vec3 second_camera_position = glm::vec3(0.5, 2.0, 0.5);
	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		new_player_position = computeMatricesFromInputs(player_position, win());
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP_first_view = ProjectionMatrix * ViewMatrix * ModelMatrix;
		ViewportOne(0, 0, side, side);
		// test_cube.draw(MVP);
		// test_sphere.draw(MVP, glm::vec3(-0.9f, -0.9f, -0.9f));
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
		// printf("Gracz jest na: (%f, %f, %f)\n", player_position.x, player_position.y, player_position.z);
		// printf("Nowa pozycja gracza: (%f, %f, %f)\n", new_player_position.x, new_player_position.y, new_player_position.z);
		// =========================================
		secondViewPortSetup(wd, ht);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		computeViewMatrices(win());
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
		ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP_second_view = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// test_sphere.draw(MVP_second_view, glm::vec3(-0.9f, -0.9f, -0.9f));
		player_position_representation.draw(MVP_second_view, player_position);
		win_sphere.draw(MVP_second_view, win_position);

		for (int i = 0; i < amount_of_columns; i++) // X
		{
			// int j = 0;
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
						//tutaj kolizja zachodzi
						we_have_collision = true;
						break;
					}
					// triangles[i][j][k].draw(MVP_second_view);
				}
			}
		}
		if (!we_have_collision)
			player_position = new_player_position;
		printf("Koniec klatki\n\n\n\n");
		// ==========================================================
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		WaitForFixedFPS(1000.0 / 60.0);
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
