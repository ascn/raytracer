#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <scene/geometry/geometry.h>

class Sphere : public Geometry {
public:
    Sphere(QString name, Transform transform, Material *material);
    float getT(float a, float b, float c) const;
    bool intersect(const Ray &ray, Intersection *intersection) const;
};

#endif // __SPHERE_H__
