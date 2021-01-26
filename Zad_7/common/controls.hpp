#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// void computeMatricesFromInputs(glm::vec3 &position, GLFWwindow *window);
void computeMatricesFromInputs(glm::vec3 &position, GLFWwindow *window);

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif