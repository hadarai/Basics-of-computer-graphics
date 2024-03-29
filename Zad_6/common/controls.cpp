// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

float horizontalAngle = -3.14f / 2;
float verticalAngle = 0;
float initialFoV = 45.0f;

float speed = 2.0f;
float mouseSpeed = 0.0005f;

glm::mat4 getViewMatrix(void)
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(void)
{
    return ProjectionMatrix;
}
void computeMatricesFromInputs(
    glm::vec3 &global_position,
    glm::vec3 &flat_position,
    glm::vec3 &sphere_position,
    GLFWwindow *window,
    bool sphere_mode,
    glm::vec2 flat_middle_of_all_tiles)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    int side = min(width, height);
    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    // Camera setup according to map mode
    if (sphere_mode)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glfwSetCursorPos(window, side / 2, side / 2);

        verticalAngle -= mouseSpeed * float(side / 2 - xpos);
        horizontalAngle += mouseSpeed * float(side / 2 - ypos);
    }
    else
    {
        horizontalAngle = -3.14f;
        verticalAngle = 0;
    }

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle));

    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f));

    glm::vec3 zero = glm::vec3(0.0f);
    glm::vec3 up = glm::normalize(glm::cross(right, direction));

    float FoV = initialFoV;

    ProjectionMatrix = glm::perspective(glm::radians(FoV), (GLfloat)(width / height), 0.1f, 100.0f);
    if (sphere_mode)
    {
        up = glm::normalize(glm::cross(up, direction));
        ViewMatrix = glm::lookAt(
            sphere_position,
            sphere_position + direction,
            up);
    }
    else
    {
        ViewMatrix = glm::lookAt(
            flat_position,
            flat_position + direction,
            up);
    }
    lastTime = currentTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        global_position.y += (speed / 50);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        global_position.y -= (speed / 50);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        global_position.x += (speed / 50);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        global_position.x -= (speed / 50);
    }
    if (sphere_mode) //sphere_mode: ON
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && flat_position.z > 0.0f)
        {
            global_position.z -= (speed / 5);
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && flat_position.z < 15.0f)
        {
            global_position.z += (speed / 5);
        }
    }
    else //sphere_mode: OFF
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && global_position.z > 0.0f)
        {
            global_position.z -= (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && global_position.z < 5.0f)
        {
            global_position.z += (speed / 50);
        }
    }
}
