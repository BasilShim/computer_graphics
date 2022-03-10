#pragma once
#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <glm.hpp>
#include <gtx/normal.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void pushTriangleToVector(std::vector<GLfloat>& points, glm::vec3 point);

class Obelisk
{
public:
	std::vector<GLfloat> getIndices();
private:
	std::vector<glm::vec3> getPoints();
};

