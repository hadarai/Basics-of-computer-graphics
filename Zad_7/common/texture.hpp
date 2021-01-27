#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#pragma once

// Load a .BMP file using our custom loader
GLuint loadBMP(const char *imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char *imagepath);

#endif