#define _USE_MATH_DEFINES

#include "Cylinder.h"

// бахаем три точки в вектор
//void pushTriangleToVector(std::vector<GLfloat>& points, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
//{
//	points.push_back(point1.x);
//	points.push_back(point1.y);
//	points.push_back(point1.z);
//    points.push_back(point2.x);
//    points.push_back(point2.y);
//    points.push_back(point2.z);
//    points.push_back(point3.x);
//    points.push_back(point3.y);
//    points.push_back(point3.z);
//}

void pushPointToVector(std::vector<GLfloat>& points, glm::vec3 point)
{
    points.push_back(point.x);
    points.push_back(point.y);
    points.push_back(point.z);
}

void Cylinder::setCenter(glm::vec3 _center)
{
    if (centerUp == _center) {
        return;
    }
    center = _center;
    centerUp = _center;
    centerBottom = _center;
    changed = true;
}

void Cylinder::setRadius(GLfloat _radius)
{
    if (radius == _radius) {
        return;
    }
    radius = _radius;
    changed = true;
}
void Cylinder::setHeight(GLfloat _height)
{
    if (height == _height) {
        return;
    }
    height = _height;
    changed = true;
}
void Cylinder::setNumOfSectors(GLint _numOfSectors)
{
    if (numberOfSides == _numOfSectors) {
        return;
    }
    numberOfSides = _numOfSectors;
    changed = true;
}

bool Cylinder::isChanged() const {
    return changed;
}

// супер конструктор
Cylinder::Cylinder(glm::vec3 _center, GLfloat _height, GLfloat _radius, GLint _numberOfSides)
{
    setCenter(_center);
    setHeight(_height);
    setRadius(_radius);
    setNumOfSectors(_numberOfSides);
    changed = false;
}

// рассчитываем точки круга относительно центра
std::vector<glm::vec3> Cylinder::getPoints(bool side)
{
    int numberOfVertices = numberOfSides + 2;
    std::vector<glm::vec3> points;
    glm::vec3 center = glm::vec3();
    glm::vec3 temp = glm::vec3();

    if (side)
    {
        centerUp.z = height / 2.0f;
        temp = centerUp;
        center = temp;
        points.push_back(center);
    }
    else
    {
        centerBottom.z = -height / 2.0f;
        glm::vec3 temp = centerBottom;
        center = temp;
        points.push_back(center);
    }

    GLfloat doublePi = 2.0f * M_PI;

    for (int i = 1; i < numberOfVertices; i++)
    {
        temp.x = center.x + (radius * cos(i * doublePi / numberOfSides));
        temp.y = center.y + (1.4f * radius * sin(i * doublePi / numberOfSides));
        temp.z = center.z;

        points.push_back(temp);
    }

    return points;
}

//тупо трегольнички в флотский вектор, где за раз бахаем три точки, т.е. 9 координат по-флотски
std::vector<GLfloat> Cylinder::getIndices()
{
    std::vector<glm::vec3> pointsUp = getPoints(true);
    std::vector<glm::vec3> pointsBottom = getPoints(false);
    std::vector<GLfloat> triangles;

    // треугольники верхнего основани€
    //--------------------------------
    for (int i = 0; i < pointsUp.size(); i++)
    {
        pushPointToVector(triangles, centerUp);
        pushPointToVector(triangles, pointsUp[i]);
        pushPointToVector(triangles, pointsUp[(i + 1) % pointsUp.size()]);
    }
    // треугольники нижнего основани€
    //--------------------------------
    for (int i = 0; i < pointsBottom.size(); i++)
    {
        pushPointToVector(triangles, centerBottom);
        pushPointToVector(triangles, pointsBottom[i]);
        pushPointToVector(triangles, pointsBottom[(i + 1) % pointsBottom.size()]);
    }
    // треугольники стенки цилиндра
    //--------------------------------
    for (int i = 0; i < pointsBottom.size(); i++)
    {
        //первый треугольник
        pushPointToVector(triangles, pointsUp[i]);
        pushPointToVector(triangles, pointsBottom[i]);
        pushPointToVector(triangles, pointsBottom[(i + 1) % pointsBottom.size()]);
        // второй треугольник
        pushPointToVector(triangles, pointsUp[(i + 1) % pointsBottom.size()]);
        pushPointToVector(triangles, pointsUp[i]);
        pushPointToVector(triangles, pointsBottom[(i + 1) % pointsBottom.size()]);
    }

    return triangles;
}