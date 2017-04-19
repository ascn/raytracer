#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/geometry/geometry.h>

class Triangle : public Geometry {
public:
    Triangle(QString name, glm::vec3 vertices[3], glm::vec3 normals[3], Material *material);
    bool intersect(const Ray &ray, Intersection *intersection) const;
    glm::vec3 triangleInterpolation(glm::vec3 P, const glm::vec3 vertices[3], const glm::vec3 attributes[3]) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
    float getArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const;
    glm::vec3 getNormal(glm::vec3 point) const;
    glm::vec3 texMap(Intersection *isect);
    glm::vec3 normals[3];
    glm::vec3 vertices[3]; // vertex coordinates in WORLD SPACE
    float S;
};

#endif // __TRIANGLE_H__
