#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(
    glm::vec3 &global_position,
    glm::vec3 &flat_position,
    glm::vec3 &sphere_position,
    GLFWwindow *window,
    bool sphere_mode,
    glm::vec2 flat_middle_of_all_tiles);

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif