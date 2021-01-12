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
// vertical angle : 0, look at the horizon
// float verticalAngle = 13 * 3.14f / 90;
float verticalAngle = 0;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 2.0f; // 2 units / second
float mouseSpeed = 0.0005f;
bool free_camera_mode = false;
glm::mat4 getViewMatrix(void)
{
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(void)
{
    return ProjectionMatrix;
}
void computeMatricesFromInputs(glm::vec3 &flat_position, glm::vec3 &sphere_position, GLFWwindow *window, bool &sphere_mode)
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

    if (sphere_mode) //toggle camera movement?
    {
        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, side / 2, side / 2); //zmienić by używać glfwGetWindowSize()

        // Compute new orientation
        // horizontalAngle += mouseSpeed * float(side / 2 - xpos);
        // verticalAngle += mouseSpeed * float(side / 2 - ypos);
        verticalAngle -= mouseSpeed * float(side / 2 - xpos);
        horizontalAngle += mouseSpeed * float(side / 2 - ypos);
    }
    //printf("horizontal angle: %f, vertical angle: %f\n", horizontalAngle, verticalAngle);

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
    if (sphere_mode)
        up = glm::normalize(glm::cross(up, direction));

    float FoV = initialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 1.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
        flat_position,             // Camera is here
        flat_position + direction, // and looks here : at the same position, plus "direction"
        up                         // Head is up (set to 0,-1,0 to look upside-down)
    );
    // printf("Kamera jest na: (%f, %f, %f)\n", position.x, position.y, position.z);
    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;

    if (!sphere_mode)
    {
        horizontalAngle = -3.14f;
        verticalAngle = 0;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            flat_position.y += (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            flat_position.y -= (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            flat_position.x += (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            flat_position.x -= (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && flat_position.z > 0.0f)
        {
            flat_position.z -= (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && flat_position.z < 5.0f)
        {
            flat_position.z += (speed / 50);
        }
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            flat_position.y += (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            flat_position.y -= (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            flat_position.x += (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            flat_position.x -= (speed / 50);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && flat_position.z > 0.0f)
        {
            flat_position.z -= (speed / 5);
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && flat_position.z < 15.0f)
        {
            flat_position.z += (speed / 5);
        }
    }
}
