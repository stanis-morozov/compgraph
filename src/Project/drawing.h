#pragma once

#include <GL/glew.h>
#include <GL/glfw3.h>

#include <GL/freeglut.h>

#include <vector>
#include <iostream>

#include "vertex.h"

const int RECTANGLES_NUMBER = 3;
const double BLOCKLENGTH = 10.0 / 3;

void constructBox(GLuint &VBO_DOWN, GLuint &VBO_UP, GLuint &VBO_LEFT, GLuint &VBO_RIGHT, GLuint &VBO_FRONT, GLuint &VBO_BACK);

void drawBox(GLuint VBO_UP, GLuint VBO_LEFT, GLuint VBO_RIGHT, GLuint VBO_FRONT, GLuint VBO_BACK);

void drawBoxDown(GLuint VBO_DOWN);