#include "obelisk.h"

void pushTriangleToVector(std::vector<GLfloat>& points, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
    points.push_back(point1.x);
    points.push_back(point1.y);
    points.push_back(point1.z);
    points.push_back(point2.x);
    points.push_back(point2.y);
    points.push_back(point2.z);
    points.push_back(point3.x);
    points.push_back(point3.y);
    points.push_back(point3.z);
}

// рассчитываем точки круга относительно центра
std::vector<glm::vec3> Obelisk::getPoints()
{
    std::vector<glm::vec3> points;

    // верхнее основание
    points.push_back(glm::vec3(0.3, -0.3, 0.3)); // 0
    points.push_back(glm::vec3(-0.3, -0.3, 0.3)); // 1
    points.push_back(glm::vec3(-0.3, -0.3, -0.3)); // 2
    points.push_back(glm::vec3(0.3, -0.3, -0.3)); // 3
    // нижнее основание
    points.push_back(glm::vec3(-0.2, 0.3, 0.2)); // 4
    points.push_back(glm::vec3(0.2, 0.3, 0.2)); // 5
    points.push_back(glm::vec3(-0.2, 0.3, -0.2)); // 6
    points.push_back(glm::vec3(0.2, 0.3, -0.2)); // 7

    return points;
}

//тупо трегольнички в флотский вектор, где за раз бахаем три точки, т.е. 9 координат по-флотски
std::vector<GLfloat> Obelisk::getIndices()
{
    std::vector<glm::vec3> points = getPoints();
    std::vector<GLfloat> triangles;

    pushTriangleToVector(triangles, points[7], points[4], points[5]);
    pushTriangleToVector(triangles, points[7], points[6], points[4]);

    pushTriangleToVector(triangles, points[0], points[1], points[2]);
    pushTriangleToVector(triangles, points[3], points[0], points[2]);

    pushTriangleToVector(triangles, points[0], points[5], points[7]);
    pushTriangleToVector(triangles, points[0], points[3], points[7]);

    pushTriangleToVector(triangles, points[1], points[5], points[0]);
    pushTriangleToVector(triangles, points[4], points[5], points[1]);

    pushTriangleToVector(triangles, points[6], points[4], points[1]);
    pushTriangleToVector(triangles, points[6], points[2], points[1]);

    pushTriangleToVector(triangles, points[2], points[6], points[7]);
    pushTriangleToVector(triangles, points[2], points[3], points[7]);

    return triangles;
}