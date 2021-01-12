#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// void computeMatricesFromInputs();
// void computeMatricesFromInputs(glm::vec3 &position, GLFWwindow *window);
void computeMatricesFromInputs(glm::vec3 &flat_position, glm::vec3 &sphere_position, GLFWwindow *window);

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif