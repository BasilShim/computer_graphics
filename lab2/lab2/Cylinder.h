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

//void pushTriangleToVector(std::vector<GLfloat>& points, glm::vec3 point);
void pushPointToVector(std::vector<GLfloat>& points, glm::vec3 point);

class Cylinder
{
public:
	Cylinder() = default;
	Cylinder(glm::vec3 _center, GLfloat _height, GLfloat _radius, GLint _numberOfSides);

	void setCenter(glm::vec3 _center);
	void setRadius(GLfloat _radius);
	void setHeight(GLfloat _height);
	void setNumOfSectors(GLint _numOfSectors);

	bool isChanged() const;

	std::vector<GLfloat> getIndices();
private:
	std::vector<glm::vec3> getPoints(bool side);
	glm::vec3 center = glm::vec3();
	glm::vec3 centerUp = glm::vec3();
	glm::vec3 centerBottom = glm::vec3();
	GLfloat height = 1;
	GLfloat radius = 0.5;
	GLint numberOfSides = 360;
	mutable bool changed = false;
};

