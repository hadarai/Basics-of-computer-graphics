#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(
    glm::vec3 &flat_position,
    glm::vec3 &sphere_position,
    GLFWwindow *window,
    bool &sphere_mode);

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif