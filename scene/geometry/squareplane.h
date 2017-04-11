#ifndef __SQUAREPLANE_H__
#define __SQUAREPLANE_H__

#include "geometry.h"

class SquarePlane : public Geometry {
    SquarePlane(QString name, Transform transform, Material *material);
    float dotproduct(glm::vec3 a, glm::vec3 b);
    float getT(glm::vec3 n, glm::vec3 s, glm::vec3 origin, glm::vec3 direction);
    bool intersect(const Ray &ray, Intersection *intersection) const;
};

#endif // __SQUAREPLANE_H__
