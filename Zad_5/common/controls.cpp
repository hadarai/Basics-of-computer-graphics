// Include GLFW
#include <GLFW/glfw3.h>
// extern GLFWwindow *window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.
// extern Window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

// position
// glm::vec3 position = glm::vec3(-0.9, 0.9, 0.9);
// horizontal angle : toward -Z
float horizontalAngle = 3.14f / 2; //
// vertical angle : 0, look at the horizon
// float verticalAngle = 13 * 3.14f / 90;
float verticalAngle = 0;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.0005f;

glm::mat4 getViewMatrix(void)
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(void)
{
    return ProjectionMatrix;
}
glm::vec3 computeMatricesFromInputs(glm::vec3 &position, GLFWwindow *window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    // printf("Okno jest rozmiaru: %dx%d\n", width, height);
    int side = min(width, height);
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, side / 2, side / 2); //zmienić by używać glfwGetWindowSize()

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(side / 2 - xpos);
    verticalAngle += mouseSpeed * float(side / 2 - ypos);
    // printf("horizontal angle: %f, vertical angle: %f\n", horizontalAngle, verticalAngle);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f));

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    float FoV = initialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
        position,             // Camera is here
        position + direction, // and looks here : at the same position, plus "direction"
        up                    // Head is up (set to 0,-1,0 to look upside-down)
    );
    // printf("Kamera jest na: (%f, %f, %f)\n", position.x, position.y, position.z);
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;

    glm::vec3 new_player_position = position;
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        new_player_position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        new_player_position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        new_player_position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        new_player_position -= right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        new_player_position.y += 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        new_player_position.y -= 0.05;
    }

    return new_player_position;
}

glm::vec3 computeViewMatrices(glm::vec3 &position, GLFWwindow *window)
{

    glm::vec3 camera_position = glm::vec3(0.0, 6.0, 0.0);
    int width, height;
    float specialFoV = 45.0f;
    float horizontalAngle2 = 0;
    // vertical angle : 0, look at the horizon
    float verticalAngle2 = -3.14f / 2;

    glfwGetWindowSize(window, &width, &height);
    // printf("Okno jest rozmiaru: %dx%d\n", width, height);
    int side = min(width, height);
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle2) * sin(horizontalAngle2),
        sin(verticalAngle2),
        cos(verticalAngle2) * cos(horizontalAngle2));

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle2 - 3.14f / 2.0f),
        0,
        cos(horizontalAngle2 - 3.14f / 2.0f));

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    float FoV = specialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
        camera_position,             // Camera is here
        camera_position + direction, // and looks here : at the same position, plus "direction"
        up                           // Head is up (set to 0,-1,0 to look upside-down)
    );
    // printf("Kamera jest na: (%f, %f, %f)\n", position.x, position.y, position.z);
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;

    glm::vec3 new_player_position = position;
    float lookUpSpeed = 0.01;
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        new_player_position.x += lookUpSpeed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        new_player_position.x -= lookUpSpeed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        new_player_position.z += lookUpSpeed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        new_player_position.z -= lookUpSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        new_player_position.y += lookUpSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        new_player_position.y -= lookUpSpeed;
    }

    return new_player_position;
}