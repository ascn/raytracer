#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/geometry/geometry.h>

class Triangle : public Geometry {
public:
    Triangle(QString name, glm::vec3 vertices[3], glm::vec3 normals[3], Material *material);
    bool intersect(const Ray &ray, Intersection *intersection) const;
    float getArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const;
    glm::vec3 getNormal(glm::vec3 point) const;
    glm::vec3 normals[3];
    glm::vec3 vertices[3]; // vertex coordinates in WORLD SPACE
    float S;
};

#endif // __TRIANGLE_H__
