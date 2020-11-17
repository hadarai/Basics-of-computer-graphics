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
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>

#include "objects/BackgroundCube/BackgroundCube.hpp"
#include "objects/PlayerSphere/PlayerSphere.hpp"
#include "ObstacleTriangle.hpp"

int amount_of_columns = 10;
int amount_of_rows = amount_of_columns;
int amount_of_layers = amount_of_rows;

void GLAPIENTRY
MessageCallback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar *message,
				const void *userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
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

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Maze 3D", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	BackgroundCube test_cube;
	ObstacleTriangle test_triangle;
	PlayerSphere test_sphere;

	glm::vec3 pos1_tri = glm::vec3(-0.7, -1.0, -1.0);
	glm::vec3 pos2_tri = glm::vec3(-0.7, -1.0, -0.8);
	glm::vec3 pos3_tri = glm::vec3(-0.7, -0.9, -0.7);
	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ========================================================
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		test_cube.draw(MVP);
		test_triangle.draw(MVP, pos1_tri, pos2_tri, pos3_tri);
		test_sphere.draw(MVP, glm::vec3(-0.9f, -0.9f, -0.9f));

		// ==========================================================

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}