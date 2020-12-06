#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// void computeMatricesFromInputs();
// void computeMatricesFromInputs(glm::vec3 &position, GLFWwindow *window);
glm::vec3 computeMatricesFromInputs(glm::vec3 &position, GLFWwindow *window);
glm::vec3 computeViewMatrices(glm::vec3 &position, GLFWwindow *window);

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif